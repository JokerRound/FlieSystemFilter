//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-06
// Description: 
//      The routine achieve for deal with irp.
//
// Modify Log:
//      2019-02-06    Hoffman
//      Info: a. Add below routines.
//              a.1. FSFilterIrpDefault();
//
//      2019-02-09    Hoffman
//      Info: a. Add below routines.
//              a.1. FSFilterIrpFileSystemControl();
//              a.2. FSFilterMinoIrpLoadFileSystem();
//
//      2019-02-11    Hoffman
//      Info: a. Add below routines.
//              a.1. FSFilterAttachMountedVolume();
//              a.2. FSFilterMinorIrpMountVolumn();
//              a.3. FSFilterIrpRead();
//              a.4. FSFilterIrpWrite();
//            b. Modify below routines.
//              b.1. FSFilterMinoIrpLoadFileSystem();
//                  b.1.1. Add main logical code.
//
//      2019-02-11    Hoffman
//      Info: a. Modify below routines.
//              a.1. FSFilterIrpWrite();
//                  a.1.1. Fix a bug by it hasn't sent irp to next lowers.
//
//******************************************************************************

#include "Launch.h"

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-11
// Last Time:           2019-02-11
// Logical Description:  
//      Do some check, then set complete routine and print size of data 
//      block when read successful.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterIrpRead(IN PDEVICE_OBJECT pstDeviceObject,
                         IN PIRP pstIrp)
{
    PAGED_CODE();
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    if (IS_MY_CONTROL_DEVICE_OBJECT(pstDeviceObject))
    {
        pstIrp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
        pstIrp->IoStatus.Information = 0;
        IoCompleteRequest(pstIrp, IO_NO_INCREMENT);
        return STATUS_INVALID_DEVICE_REQUEST;
    }

    PDEVICE_EXTENSION pstDeviceExtension =
        (PDEVICE_EXTENSION)pstDeviceObject->DeviceExtension;
    // Check it's a volume device or not.
    if (NULL == pstDeviceExtension->pstStorageDeviceObject_)
    {
        return FSFilterIrpDefault(pstDeviceObject, pstIrp);
    }

    PIO_STACK_LOCATION pstStackLocation = IoGetCurrentIrpStackLocation(pstIrp);
    LARGE_INTEGER stOffset = { 0 };
    ULONG ulLength = 0;

    // Get offset and length.
    stOffset.QuadPart = pstStackLocation->Parameters.Read.ByteOffset.QuadPart;
    ulLength = pstStackLocation->Parameters.Read.Length;

    // Set complete routine and wait event of complete.
    KEVENT waitEvent;
    KeInitializeEvent(&waitEvent, NotificationEvent, FALSE);

    IoCopyCurrentIrpStackLocationToNext(pstIrp);
    IoSetCompletionRoutine(pstIrp,
                           FSFilterReadComplete,
                           &waitEvent,
                           TRUE,
                           TRUE,
                           TRUE);
    ntStatus = IoCallDriver(pstDeviceExtension->pstNextDeviceObject_, pstIrp);
    if (STATUS_PENDING == ntStatus)
    {
        ntStatus = KeWaitForSingleObject(&waitEvent,
                                         Executive,
                                         KernelMode,
                                         FALSE,
                                         NULL);
        ASSERT(STATUS_SUCCESS == ntStatus);
    }

    if (NT_SUCCESS(pstIrp->IoStatus.Status))
    {
        PVOID pBuffer = NULL;
        if (NULL != pstIrp->MdlAddress)
        {
            pBuffer = MmGetSystemAddressForMdlSafe(pstIrp->MdlAddress, 
                                                   NormalPagePriority);
        }
        else
        {
            pBuffer = pstIrp->UserBuffer;
        }

        if (NULL != pBuffer)
        {
            ulLength = pstIrp->IoStatus.Information;

            KdPrint(("Read irp: the size is %ul\r\n", ulLength));
        }
    }

    IoCompleteRequest(pstIrp, IO_NO_INCREMENT);
    return pstIrp->IoStatus.Status;
} //! FSFilterIrpRead() END

#pragma PAGEDCODE
NTSTATUS FSFilterIrpWrite(IN PDEVICE_OBJECT pstDeviceObject,
                         IN PIRP pstIrp)
{
    PAGED_CODE();
    if (IS_MY_CONTROL_DEVICE_OBJECT(pstDeviceObject))
    {
        pstIrp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
        pstIrp->IoStatus.Information = 0;
        IoCompleteRequest(pstIrp, IO_NO_INCREMENT);
        return STATUS_INVALID_DEVICE_REQUEST;
    } 

    PDEVICE_EXTENSION pstDeviceExtension =
        (PDEVICE_EXTENSION)pstDeviceObject->DeviceExtension;
    // Check it's a volume device or not.
    if (NULL == pstDeviceExtension->pstStorageDeviceObject_)
    {
        return FSFilterIrpDefault(pstDeviceObject, pstIrp);
    }

    PIO_STACK_LOCATION pstStackLocation = IoGetCurrentIrpStackLocation(pstIrp);
    LARGE_INTEGER stOffset = { 0 };
    ULONG ulLength = 0;

    // Get offset and length.
    ulLength = pstStackLocation->Parameters.Write.Length;
    PVOID pBuffer = NULL;
    if (NULL != pstIrp->MdlAddress)
    {
        pBuffer = MmGetSystemAddressForMdlSafe(pstIrp->MdlAddress,
                                               NormalPagePriority);
    }
    else
    {
        pBuffer = pstIrp->UserBuffer;
    }

    if (NULL != pBuffer)
    {
        KdPrint(("Wirte irp: The request size is %u\r\n", 
                 pstStackLocation->Parameters.Write.Length));
    }

    IoSkipCurrentIrpStackLocation(pstIrp);
    return IoCallDriver(pstDeviceExtension->pstNextDeviceObject_, pstIrp);
} //! FSFilterIrpWrite() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-11
// Last Time:           2019-02-11
// Logical Description:  
//      Get volume device object, check it has attached or not. Delete the 
//      filter device if it has attached, or begin to attach.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterAttachMountedVolume(
    IN PDEVICE_OBJECT pstFilterDeviceObject,
    IN PDEVICE_OBJECT pstDeviceObject,
    IN PIRP pstIrp
)
{
    PAGED_CODE();
    ASSERT(IS_MY_FILTER_DEVICE_OBJECT(pstFilterDeviceObject));

    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;

    // Get lock.
    ExAcquireFastMutex(&g_stAttachLock);

    do
    {
        // Check the volume install successful or not.
        if (!NT_SUCCESS(pstIrp->IoStatus.Status))
        {
            IoDeleteDevice(pstFilterDeviceObject);
            break;
        }

        // Check the volume has attached or not.
        if (FSFilterIsAttachedDevice(pstDeviceObject))
        {
            IoDeleteDevice(pstFilterDeviceObject);
            break;
        }

        // Set flag and characteristics.
        if (FlagOn(pstDeviceObject->Flags, DO_BUFFERED_IO))
        {
            SetFlag(pstFilterDeviceObject->Flags, DO_BUFFERED_IO);
        }

        if (FlagOn(pstDeviceObject->Flags, DO_DIRECT_IO))
        {
            SetFlag(pstFilterDeviceObject->Flags, DO_DIRECT_IO);
        }

        if (FlagOn(pstDeviceObject->Characteristics, FILE_DEVICE_SECURE_OPEN))
        {
            SetFlag(pstFilterDeviceObject->Characteristics,
                    FILE_DEVICE_SECURE_OPEN);
        }

        PDEVICE_EXTENSION pstDeviceExtension =
            (PDEVICE_EXTENSION)pstFilterDeviceObject->DeviceExtension;

        KIRQL currentIRQL;
        IoAcquireVpbSpinLock(&currentIRQL);
        PDEVICE_OBJECT pstVolumeDeviceObject =
            pstDeviceExtension->pstStorageDeviceObject_->Vpb->DeviceObject;
        IoReleaseVpbSpinLock(currentIRQL);

        // Try to attach the volume device.
        for (ULONG cntI = 0; cntI < ATTACH_VOLUME_DEVICE_TRY_NUM; cntI++)
        {
            ntStatus = IoAttachDeviceToDeviceStackSafe(
                pstFilterDeviceObject,
                pstVolumeDeviceObject,
                &pstDeviceExtension->pstNextDeviceObject_
            );
            if (NT_SUCCESS(ntStatus))
            {
                ClearFlag(pstFilterDeviceObject->Flags, DO_DEVICE_INITIALIZING);
                KdPrint(("FileSystemFilter!FSFilterAttachMountedVolume: "
                         "%wZ has attached successful.\r\n",
                         &pstDeviceExtension->ustrDeviceName_));
                break;
            }

            LARGE_INTEGER stInterval;
            stInterval.QuadPart = (500 * DELAY_ONE_MILLISECOND);
            KeDelayExecutionThread(KernelMode, FALSE, &stInterval);
        } //! for 'Try to attach the volume device' END
    } while (FALSE);

    // Release lock.
    ExReleaseFastMutex(&g_stAttachLock);

    return ntStatus;
} //! FSFilterAttachMountedVolume() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-11
// Last Time:           2019-02-11
// Logical Description:  
//      Create filter device object and wait the volume has mounted successful,
//      then executive the attach active in complete routine.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterMinorIrpMountVolumn(IN PDEVICE_OBJECT pstDeviceObject,
                                     IN PIRP pstIrp)
{
    PAGED_CODE();
    ASSERT(!IS_MY_CONTROL_DEVICE_OBJECT(pstDeviceObject));
    ASSERT(IS_MY_FILTER_DEVICE_OBJECT(pstDeviceObject));
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    
    PIO_STACK_LOCATION pstStackLocation = IoGetCurrentIrpStackLocation(pstIrp);

    // Create volume filter device.
    PDEVICE_OBJECT pstFilterDeviceObject = NULL;
    ntStatus = IoCreateDevice(g_pstDriverObject,
                              sizeof(DEVICE_EXTENSION),
                              NULL,
                              pstDeviceObject->DeviceType,
                              0,
                              FALSE,
                              &pstFilterDeviceObject);
    if (!NT_SUCCESS(ntStatus))
    {
        KdPrint(("FileSystemFilter!FSFilterAttachFileSystemControlDevice: "
                 "Create filter device object filed.\r\n"));

        pstIrp->IoStatus.Information = 0;
        pstIrp->IoStatus.Status = ntStatus;
        IoCompleteRequest(pstIrp, IO_NO_INCREMENT);
        return ntStatus;
    }

    // Save storage device object.
    PDEVICE_EXTENSION pstDeviceExtension =
        (PDEVICE_EXTENSION)pstFilterDeviceObject->DeviceExtension;

    // Get real device of storage.
    KIRQL currentIRQL;
    IoAcquireVpbSpinLock(&currentIRQL);
    pstDeviceExtension->pstStorageDeviceObject_ =
        pstStackLocation->Parameters.MountVolume.Vpb->RealDevice;
    IoReleaseVpbSpinLock(currentIRQL);

    // Get and save storage device name.
    RtlInitEmptyUnicodeString(
        &pstDeviceExtension->ustrDeviceName_,
        pstDeviceExtension->awcDeviceObjectBuffer_,
        sizeof(pstDeviceExtension->awcDeviceObjectBuffer_)
    );

    PUNICODE_STRING pustrStorageDeviceName = NULL;
    FSFilterGetObjectName(pstDeviceExtension->pstStorageDeviceObject_,
                          &pustrStorageDeviceName);
    if (NULL != pustrStorageDeviceName)
    {
        RtlCopyUnicodeString(&pstDeviceExtension->ustrDeviceName_,
                             pustrStorageDeviceName);

        ExFreePool(pustrStorageDeviceName);
        pustrStorageDeviceName = NULL;
    }

    // Set completion routine and wait.
    KEVENT waitEvent;
    KeInitializeEvent(&waitEvent, NotificationEvent, FALSE);

    IoCopyCurrentIrpStackLocationToNext(pstIrp);
    IoSetCompletionRoutine(pstIrp,
                           FSFilterMountDeviceComplete,
                           &waitEvent,
                           TRUE,
                           TRUE,
                           TRUE);

    ntStatus = IoCallDriver(pstDeviceExtension->pstNextDeviceObject_, pstIrp);
    if (STATUS_PENDING == ntStatus)
    {
        ntStatus = KeWaitForSingleObject(&waitEvent,
                                         Executive,
                                         KernelMode,
                                         FALSE,
                                         NULL);
        ASSERT(STATUS_SUCCESS == ntStatus);
    }

    // Attach the filter device to target device.
    ntStatus = FSFilterAttachMountedVolume(pstFilterDeviceObject,
                                           pstDeviceObject,
                                           pstIrp);

    ntStatus = pstIrp->IoStatus.Status;
    IoCompleteRequest(pstIrp, IO_NO_INCREMENT); 

    return ntStatus;
} //! FSFilterMinorIrpMountVolumn() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-09
// Last Time:           2019-02-11
// Logical Description:  
//      Detach the filter of file system recognizer, then delete the filter
//      device. Reattach the filter device if laod file system failed.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterMinoIrpLoadFileSystem(IN PDEVICE_OBJECT pstDeviceObject,
                                       IN PIRP pstIrp)
{
    PAGED_CODE();
    ASSERT(IS_MY_FILTER_DEVICE_OBJECT(pstDeviceObject));

    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION pstDeviceExtension =
        (PDEVICE_EXTENSION)pstDeviceObject->DeviceExtension;

    // Detach filter device from recognizer device.
    IoDetachDevice(pstDeviceExtension->pstNextDeviceObject_);

    KEVENT waitEvent;
    KeInitializeEvent(&waitEvent, NotificationEvent, FALSE);

    // Set completion routine.
    IoSetCompletionRoutine(pstIrp,
                           FSFilterLoadFileSystemComplete,
                           &waitEvent,
                           TRUE,
                           TRUE,
                           TRUE);

    IoCopyCurrentIrpStackLocationToNext(pstIrp);
    ntStatus = IoCallDriver(pstDeviceExtension->pstNextDeviceObject_, pstIrp);

    if (STATUS_PENDING == ntStatus)
    {
        ntStatus = KeWaitForSingleObject(&waitEvent, 
                                         Executive,
                                         KernelMode,
                                         FALSE,
                                         NULL);
        ASSERT(NT_SUCCESS(ntStatus));
    }

    if (!NT_SUCCESS(pstIrp->IoStatus.Status) &&
        STATUS_IMAGE_ALREADY_LOADED != pstIrp->IoStatus.Status)
    {
        // Reattach to recognizer.
        ntStatus = IoAttachDeviceToDeviceStackSafe(
            pstDeviceObject,
            pstDeviceExtension->pstNextDeviceObject_,
            &pstDeviceExtension->pstNextDeviceObject_
        );

        ASSERT(NT_SUCCESS(ntStatus));
    }
    else
    {
        IoDeleteDevice(pstDeviceObject);
    }

    ntStatus = pstIrp->IoStatus.Status;
    IoCompleteRequest(pstIrp, IO_NO_INCREMENT);
    return ntStatus;
} //! FSFilterMinoIrpLoadFileSystem() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-09
// Last Time:           2019-02-09
// Logical Description:  
//      Call routine to deal with target minor irp, other irp will pass through
//      to next device.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterIrpFileSystemControl(IN PDEVICE_OBJECT pstDeviceObject,
                                      IN PIRP pstIrp)
{
    PAGED_CODE();
    ASSERT(!IS_MY_CONTROL_DEVICE_OBJECT(pstDeviceObject));
    ASSERT(IS_MY_FILTER_DEVICE_OBJECT(pstDeviceObject));

    PIO_STACK_LOCATION pstStackLocation = IoGetCurrentIrpStackLocation(pstIrp);
    PDEVICE_EXTENSION pstDeviceExtension =
        (PDEVICE_EXTENSION)pstDeviceObject->DeviceExtension;

    // Deal with minior irp.
    switch (pstStackLocation->MinorFunction)
    {
        case IRP_MN_MOUNT_VOLUME:
        {
            return FSFilterMinorIrpMountVolumn(pstDeviceObject, pstIrp);
        }
        case IRP_MN_LOAD_FILE_SYSTEM:
        {
            return FSFilterMinoIrpLoadFileSystem(pstDeviceObject, pstIrp);
        }
        case IRP_MN_USER_FS_REQUEST:
        {
            switch (pstStackLocation->Parameters.
                    FileSystemControl.FsControlCode)
            {
                case FSCTL_DISMOUNT_VOLUME:
                {
                    KdPrint(("FileSystemFilter!FSFilterIrpFileSystemControl: "
                             "Dismounting volumn %wZ\r\n", 
                             &pstDeviceExtension->ustrDeviceName_));

                    break;
                }
            }

            break;
        }
        default:
        {
            break;
        }
    } //! switch 'Deal with minior irp' END

    IoSkipCurrentIrpStackLocation(pstIrp);
    return IoCallDriver(pstDeviceExtension->pstNextDeviceObject_, pstIrp);
} //! FSFilterIrpFileSystemControl() END


//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-06
// Last Time:           2019-02-06
// Logical Description:  
//      Pass the irp to driver of next floor directly.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterIrpDefault(IN PDEVICE_OBJECT pstDeviceObject,
                            IN PIRP pstIrp)
{
    PAGED_CODE();
    ASSERT(!IS_MY_CONTROL_DEVICE_OBJECT(pstDeviceObject));
    ASSERT(IS_MY_FILTER_DEVICE_OBJECT(pstDeviceObject));
    
    PDEVICE_EXTENSION pstDeviceExtension =
        (PDEVICE_EXTENSION)pstDeviceObject->DeviceExtension;

    IoSkipCurrentIrpStackLocation(pstIrp);

    return IoCallDriver(pstDeviceExtension->pstNextDeviceObject_, pstIrp);
} //! DefaultDispatchRoutine() END
