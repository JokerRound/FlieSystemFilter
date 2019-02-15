//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-06
// Description: 
//      The entrance of driver and some routine about initialization.
//
// Modify Log:
//      2019-02-06    Hoffman
//      Info: a. Add below functions.
//              a.1. DriverEntry();
//              a.2. FsFilterCreateDevice();
//              a.3. FSFilterUnload();
//
//      2019-02-09    Hoffman
//      Info: a. Add below functions.
//              a.1. FSFilterAttachToFileSystemControlDevice();
//              a.2. FSFilterDetachFromFileSystemControlDevice();
//
//      2019-02-14    Hoffman
//      Info: a. Add below functions.
//              a.1. FSFilterAttachToMountedVolumeDevice();
//            b. Modify below functions.
//              b.1. FSFilterAttachToFileSystemControlDevice();
//                  b.1.1. Add ability for attaching mounted volume device.
//******************************************************************************
#include "Launch.h"

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-06
// Last Time:           2019-02-06
// Logical Description:  
//      Set unload routine, irp dispatch routines, fast io routines and call
//      back of file system change notify, then create device.
//******************************************************************************
#pragma INITCODE
extern "C"
_Use_decl_annotations_
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pstDriverObject,
                     IN PUNICODE_STRING pstRegistryPath)
{
    PAGED_CODE();
    UNREFERENCED_PARAMETER(pstRegistryPath);
    KdPrint(("Enter DriverEntry\n"));

    // Initialize the fast mutex.
    ExInitializeFastMutex(&g_stAttachLock);

    g_pstDriverObject = pstDriverObject;
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;

    pstDriverObject->DriverUnload = FSFilterUnload;

        // Set irp routines.
    for (ULONG cntI = 0; cntI < IRP_MJ_MAXIMUM_FUNCTION; cntI++)
    {
        pstDriverObject->MajorFunction[cntI] = FSFilterIrpDefault;
    }

    pstDriverObject->MajorFunction[IRP_MJ_POWER] = FSFilterPower;

    //pstDriverObject->MajorFunction[IRP_MJ_CREATE] = FSFilterIrpCreate;
    //pstDriverObject->MajorFunction[IRP_MJ_CREATE_MAILSLOT] = 
    //    FSFilterIrpCreate;
    //pstDriverObject->MajorFunction[IRP_MJ_CREATE_NAMED_PIPE] = 
    //    FSFilterIrpCreate;

    pstDriverObject->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = 
        FSFilterIrpFileSystemControl;

    pstDriverObject->MajorFunction[IRP_MJ_READ] = FSFilterIrpRead;
    pstDriverObject->MajorFunction[IRP_MJ_WRITE] = FSFilterIrpWrite;

    // Set Fast I/O routines.
    PFAST_IO_DISPATCH pstFastIoDispatch = NULL;
    pstFastIoDispatch = 
        (PFAST_IO_DISPATCH)ExAllocatePoolWithTag(PagedPool,
                                                 sizeof(FAST_IO_DISPATCH),
                                                 FAST_IO_DISPATCH_TAG);
    if (NULL == pstFastIoDispatch)
    {
        KdPrint(("FileSystemFilter!DriverEntry: "
                 "Allocate memory for fast io dispatch failed.\r\n"));
        return STATUS_UNSUCCESSFUL;
    }

    RtlZeroMemory(pstFastIoDispatch, sizeof(FAST_IO_DISPATCH));
    pstFastIoDispatch->SizeOfFastIoDispatch = sizeof(FAST_IO_DISPATCH);

    pstFastIoDispatch->FastIoCheckIfPossible = FSFilterFastIoCheckIfPossible;
    pstFastIoDispatch->FastIoRead = FSFilterFastIoRead;
    pstFastIoDispatch->FastIoWrite = FSFilterFastIoWrite;
    pstFastIoDispatch->FastIoQueryBasicInfo = FSFilterFastIoQueryBasicInfo;
    pstFastIoDispatch->FastIoQueryStandardInfo = 
        FSFilterFastIoQueryStandardInfo;
    pstFastIoDispatch->FastIoQueryOpen = FSFilterFastIoQueryOpen;
    pstFastIoDispatch->FastIoQueryNetworkOpenInfo = 
        FSFilterFastIoQueryNetworkOpenInfo;
    pstFastIoDispatch->FastIoLock = FSFilterFastIoLock ;
    pstFastIoDispatch->FastIoUnlockAll = FSFilterFastIoUnlockAll;
    pstFastIoDispatch->FastIoUnlockSingle = FSFilterFastIoUnlockSingle;
    pstFastIoDispatch->FastIoUnlockAllByKey = FSFilterFastIoUnlockAllByKey;
    pstFastIoDispatch->FastIoDeviceControl = FSFilterFastIoDeviceControl;
    pstFastIoDispatch->FastIoDetachDevice = FSFilterFastIoDetachDevice;
    pstFastIoDispatch->MdlRead = FSFilterFastIoMdlRead;
    pstFastIoDispatch->MdlReadComplete = FSFilterFastIoMdlReadComplete;
    pstFastIoDispatch->MdlReadCompleteCompressed = 
        FSFilterFastIoMdlReadCompleteCompressed;
    pstFastIoDispatch->PrepareMdlWrite = FSFilterFastIoPrepareMdlWrite;
    pstFastIoDispatch->MdlWriteComplete = FSFilterFastIoMdlWriteComplete;
    pstFastIoDispatch->MdlWriteCompleteCompressed = 
        FSFilterFastIoMdlWriteCompleteCompressed;
    pstFastIoDispatch->FastIoReadCompressed = FSFilterFastIoReadCompressed;
    pstFastIoDispatch->FastIoWriteCompressed = FSFilterFastIoWriteCompressed;

    pstDriverObject->FastIoDispatch = pstFastIoDispatch;

    // Register the call back when file system has change.
    ntStatus = 
        IoRegisterFsRegistrationChange(pstDriverObject,
                                       FSFilterFsChangeNotify);
    if (!NT_SUCCESS(ntStatus))
    {
        KdPrint(("FileSystemFilter!DriverEntry: "
                 "Register file system change routine failed.\r\n"));
        return ntStatus;
    }

    ntStatus = FSFilterCreateDevice(pstDriverObject);
    
    KdPrint(("DriverEntry end\n"));
    return ntStatus;
} //! DriverEntry() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-06
// Last Time:           2019-02-06
// Logical Description:  
//      Free the memory of fast io dispatch.
//******************************************************************************
#pragma PAGEDCODE
VOID FSFilterUnload(IN PDRIVER_OBJECT pstDriverObject)
{
    PAGED_CODE();

    PDEVICE_EXTENSION pstDeviceExtension = NULL;
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    ULONG ulDeviceObjectNumber = 0;
    PDEVICE_OBJECT *apstDeviceObejctList = NULL;
    ULONG ulDeiveObjectListSize = 0;

    KdPrint(("Enter DriverUnload\n"));

    IoUnregisterFsRegistrationChange(pstDriverObject, FSFilterFsChangeNotify);
 
    // Delete device and symbol.
    do
    {
        // Get number of device object.
        ntStatus = IoEnumerateDeviceObjectList(pstDriverObject,
                                               NULL,
                                               0,
                                               &ulDeviceObjectNumber);
        if (!NT_SUCCESS(ntStatus) &&
            STATUS_BUFFER_TOO_SMALL != ntStatus)
        {
            KdPrint(("FileSystemFilter!FSFilterUnload: "
                     "Get number of device object failed.\r\n"));
            break;
        }

        ulDeiveObjectListSize = sizeof(PDEVICE_OBJECT) * ulDeviceObjectNumber;

        // Allocate memory.
        apstDeviceObejctList = 
            (PDEVICE_OBJECT *)
            ExAllocatePoolWithTag(PagedPool,
                                  ulDeiveObjectListSize,
                                  DEVICE_OBJECT_LIST_TAG);
        if (NULL == apstDeviceObejctList)
        {
            KdPrint(("ExAllocatePoolWithTag failed.\r\n"));
            break;
        }

        // Get device object list.
        ntStatus = IoEnumerateDeviceObjectList(pstDriverObject,
                                               apstDeviceObejctList,
                                               ulDeiveObjectListSize,
                                               &ulDeviceObjectNumber);
        if (!NT_SUCCESS(ntStatus))
        {
            KdPrint(("IoEnumerateDeviceObjectList failed.\r\n"));
            break;
        }

        // Detach all device.
        for (ULONG cntI = 0; cntI < ulDeviceObjectNumber; cntI++)
        {
            // Normal check.
            if (NULL == apstDeviceObejctList[cntI])
            {
                continue;
            }

            pstDeviceExtension = 
                (PDEVICE_EXTENSION)apstDeviceObejctList[cntI]->DeviceExtension;
            if (NULL != pstDeviceExtension)
            {
                IoDetachDevice(pstDeviceExtension->pstNextDeviceObject_);
            }
        }

        // Wait for irp
        LARGE_INTEGER stInterval;
        stInterval.QuadPart = (5 * DELAY_ONE_SECOND);	  
		KeDelayExecutionThread(KernelMode, FALSE, &stInterval);

        // Delete all device.
        for (ULONG cntI = 0; cntI < ulDeviceObjectNumber; cntI++)
        {
            // Normal check.
            if (NULL == apstDeviceObejctList[cntI])
            {
                continue;
            }

            IoDeleteDevice(apstDeviceObejctList[cntI]);
            ObDereferenceObject(apstDeviceObejctList[cntI]);
        }

    } while (false); //! do 'Delete device and symbol' while END 

    // Free memory.
    if (NULL != apstDeviceObejctList)
    {
        ExFreePoolWithTag(apstDeviceObejctList, DEVICE_OBJECT_LIST_TAG);
    }

    // Free memory of fast io dispatch table.
    PFAST_IO_DISPATCH pstFastIoDispatch = pstDriverObject->FastIoDispatch;
    pstDriverObject->FastIoDispatch = NULL;
    if (NULL != pstFastIoDispatch)
    {
        ExFreePoolWithTag(pstFastIoDispatch,
                          FAST_IO_DISPATCH_TAG);
        pstFastIoDispatch = NULL;
    }


} //! DriverUnload() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-06
// Last Time:           2019-02-06
// Logical Description:  
//      Create control device.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterCreateDevice(IN PDRIVER_OBJECT pstDriverObject)
{
    PAGED_CODE();

    NTSTATUS ntStatus;
    UNICODE_STRING ustrDeviceName;
    RtlInitUnicodeString(&ustrDeviceName, CONTROL_DEVICE_NAME);

    // Create device object.
    ntStatus = IoCreateDevice(pstDriverObject,
                              0,
                              &ustrDeviceName,
                              FILE_DEVICE_DISK_FILE_SYSTEM,
                              FILE_DEVICE_SECURE_OPEN,
                              FALSE,
                              &g_pstControlDeviceObject);

    // Path not fond.
    if (STATUS_OBJECT_PATH_NOT_FOUND == ntStatus)
    {
        RtlInitUnicodeString(&ustrDeviceName, OLD_CONTROL_DEVICE_NAME);
        ntStatus = IoCreateDevice(pstDriverObject,
                                  0,
                                  &ustrDeviceName,
                                  FILE_DEVICE_DISK_FILE_SYSTEM,
                                  FILE_DEVICE_SECURE_OPEN,
                                  FALSE,
                                  &g_pstControlDeviceObject);
        if (!NT_SUCCESS(ntStatus))
        {
            KdPrint(("FileSystemFilter!FsFilterAddDevice: "
                     "Create \"%wZ\" deivce failed.\r\n",
                     &ustrDeviceName));
        return ntStatus;
        }
    } //! if 'Path not fond' END
    else if (!NT_SUCCESS(ntStatus))
    {
        KdPrint(("FileSystemFilter!FsFilterAddDevice: "
                 "Create \"%wZ\" deivce failed.\r\n",
                 &ustrDeviceName));
        return ntStatus;
    }

    return STATUS_SUCCESS;
} //! CreateDevice() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-14
// Last Time:           2019-02-14
// Logical Description:  
//      Get device object list of file system driver, create filter and attach
//      to the deivce if it is my target device, then save the name of device
//      free the resource, until all device has been operated.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterAttachToMountedVolumeDevice(
    IN PDEVICE_OBJECT pstFSControlDeviceObject
)
{
    PAGED_CODE();
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    ULONG ulDeviceObjectNumber = 0;
    PDEVICE_OBJECT *apstDeviceObejctList = NULL;
    ULONG ulDeiveObjectListSize = 0;

    do
    {
        // Get number of device object.
        ntStatus = 
            IoEnumerateDeviceObjectList(
                pstFSControlDeviceObject->DriverObject,
                NULL,
                0,
                &ulDeviceObjectNumber
            );
        if (!NT_SUCCESS(ntStatus) && 
            STATUS_BUFFER_TOO_SMALL != ntStatus)
        {
            KdPrint(("FileSystemFilter!FSFilterAttachToMountedVolumeDevice: "
                     "Get number device objectk failed.\r\n"));
            break;
        }

        ulDeiveObjectListSize = sizeof(PDEVICE_OBJECT) * ulDeviceObjectNumber;

        // Allocate memory.
        apstDeviceObejctList = 
            (PDEVICE_OBJECT *)
            ExAllocatePoolWithTag(PagedPool,
                                  ulDeiveObjectListSize,
                                  DEVICE_OBJECT_LIST_TAG);
        if (NULL == apstDeviceObejctList)
        {
            KdPrint(("ExAllocatePoolWithTag failed.\r\n"));
            break;
        }

        // Get device object list.
        ntStatus = 
            IoEnumerateDeviceObjectList(
                pstFSControlDeviceObject->DriverObject,
                apstDeviceObejctList,
                ulDeiveObjectListSize,
                &ulDeviceObjectNumber
            );
        if (!NT_SUCCESS(ntStatus))
        {
            KdPrint(("IoEnumerateDeviceObjectList failed.\r\n"));
            break;
        }


        // Get lock.
        ExAcquireFastMutex(&g_stAttachLock);
        // Create filter device and attach.
        for (ULONG cntI = 0; cntI < ulDeviceObjectNumber; cntI++)
        {
            // Normal check.
            if (NULL == apstDeviceObejctList[cntI])
            {
                continue;
            }

            // Check the device is my target and it's not a control device.
            if (pstFSControlDeviceObject == apstDeviceObejctList[cntI] ||
                (pstFSControlDeviceObject->DeviceType !=
                 apstDeviceObejctList[cntI]->DeviceType) ||
                FSFilterIsAttachedDevice(apstDeviceObejctList[cntI]))
            {
                continue;
            }

            // Create device.
            PDEVICE_OBJECT pstFilterDeviceObject = NULL;
            ntStatus = IoCreateDevice(g_pstDriverObject,
                                      sizeof(DEVICE_EXTENSION),
                                      NULL,
                                      apstDeviceObejctList[cntI]->DeviceType,
                                      0,
                                      FALSE,
                                      &pstFilterDeviceObject);
            if (!NT_SUCCESS(ntStatus))
            {
                KdPrint(("FileSystemFilter!"
                         "FSFilterAttachToMountedVolumeDevice: "
                         "Crate filter device failed.\r\n"));
                continue;
            }

            // Set flags same as device is attached.
            if (FlagOn(pstFSControlDeviceObject->Flags, DO_BUFFERED_IO))
            {
                SetFlag(pstFilterDeviceObject->Flags, DO_BUFFERED_IO);
            }

            if (FlagOn(pstFSControlDeviceObject->Flags, DO_DIRECT_IO))
            {
                SetFlag(pstFilterDeviceObject->Flags, DO_DIRECT_IO);
            }

            if (FlagOn(pstFSControlDeviceObject->Characteristics, 
                       FILE_DEVICE_SECURE_OPEN))
            {
                SetFlag(pstFilterDeviceObject->Characteristics,
                        FILE_DEVICE_SECURE_OPEN);
            }

            // Get device extension of filter device.
            PDEVICE_EXTENSION pstFilterDeviceExtension =
                (PDEVICE_EXTENSION)pstFilterDeviceObject->DeviceExtension;

            // Get real device.
            ntStatus =
                IoGetDiskDeviceObject(
                    apstDeviceObejctList[cntI],
                    &pstFilterDeviceExtension->pstStorageDeviceObject_
                );
            if (!NT_SUCCESS(ntStatus))
            {
                KdPrint(("FileSystemFilter!"
                         "FSFilterAttachToMountedVolumeDevice: "
                         "Get real device failed.\r\n"));
                IoDeleteDevice(pstFilterDeviceObject);
                continue;
            }

            // Get name of volume device.
            PUNICODE_STRING pustrVolumeDeviceName = NULL;
            ntStatus = FSFilterGetObjectName(apstDeviceObejctList[cntI],
                                             &pustrVolumeDeviceName);
            if (!NT_SUCCESS(ntStatus))
            {
                KdPrint(("FileSystemFilter!"
                         "FSFilterAttachToMountedVolumeDevice: "
                         "Get name of volume deivce failed.\r\n"));
            }

            // Attach filter device to mounted volume device.
            ASSERT(NULL != pustrVolumeDeviceName);
            ASSERT(NULL != pstFilterDeviceExtension);
            
            ntStatus =
                IoAttachDeviceToDeviceStackSafe(
                    pstFilterDeviceObject,
                    apstDeviceObejctList[cntI],
                    &pstFilterDeviceExtension->pstNextDeviceObject_
                );
            if (!NT_SUCCESS(ntStatus))
            {
                KdPrint(("FileSystemFilter!"
                         "FSFilterAttachToMountedVolumeDevice: "
                         "Attach to %wZ failed.\r\n",
                         pustrVolumeDeviceName));
            }

            // Save the name of volume device.
            RtlCopyUnicodeString(&pstFilterDeviceExtension->ustrDeviceName_,
                                 pustrVolumeDeviceName);

            // Free the resource of device name.
            if (NULL != pustrVolumeDeviceName)
            {
                POBJECT_NAME_INFORMATION pstObjectNameInfo =
                    CONTAINING_RECORD(pustrVolumeDeviceName,
                                      OBJECT_NAME_INFORMATION,
                                      Name);
                ExFreePoolWithTag(pstObjectNameInfo, OBJECT_NAME_TAG);
                pstObjectNameInfo = NULL;
            }

            // Set initialization has finished.
            ClearFlag(pstFilterDeviceObject->Flags, DO_DEVICE_INITIALIZING);
        } //! if 'Create filter device and attach.' END

        // Release lock.
        ExReleaseFastMutex(&g_stAttachLock);

        return STATUS_SUCCESS;
    } while (FALSE);

    return ntStatus;
} //! FSFilterAttachToMountedVolumeDevice() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-09
// Last Time:           2019-02-14
// Logical Description:  
//      Get the file system driver's object name, directly return if it's a
//      recognizer. Then, create filter device object and attach. Recode the 
//      name of device is attached.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterAttachToFileSystemControlDevice(
    IN PDEVICE_OBJECT pstDeviceObject,
    IN PUNICODE_STRING pustrDeviceObjectName
)
{
    PAGED_CODE();

    if (!IS_TARGET_DEVICE_TYPE(pstDeviceObject->DeviceType))
    {
        return STATUS_SUCCESS;
    }

    PDEVICE_OBJECT pstFilterDeviceObject = NULL;
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    PUNICODE_STRING pustrDriverObjectName = NULL;
    UNICODE_STRING ustrFileSystemRecName;

    do
    {
        ntStatus = FSFilterGetObjectName(pstDeviceObject->DriverObject, 
                                         &pustrDriverObjectName);
        if (!NT_SUCCESS(ntStatus))
        {
            break;
        }

        RtlInitUnicodeString(&ustrFileSystemRecName, FILE_SYSTEM_REC_NAME);

        if (RtlCompareUnicodeString(pustrDriverObjectName,
                                    &ustrFileSystemRecName,
                                    TRUE) == 0)
        {
            return STATUS_SUCCESS;
        }

        // Create fliter device for  control device of file system.
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
            break;
        }

        // Set flags same as device is attached.
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
        if (NULL == pstDeviceExtension)
        {
            KdPrint(("FileSystemFilter!FSFilterAttachFileSystemControlDevice: "
                     "The device extension's address is invalid.\r\n"));
            ntStatus = STATUS_INVALID_ADDRESS;
            break;
        }

        ntStatus = 
            IoAttachDeviceToDeviceStackSafe(
                pstFilterDeviceObject,
                pstDeviceObject,
                &pstDeviceExtension->pstNextDeviceObject_
            );
        if (!NT_SUCCESS(ntStatus))
        {
            KdPrint(("FileSystemFilter!FSFilterAttachFileSystemControlDevice: "
                     "Attach device failed.\r\n"));
            break;
        }

        // Recode name of device is attached.
        RtlInitEmptyUnicodeString(
            &pstDeviceExtension->ustrDeviceName_,
            pstDeviceExtension->awcDeviceObjectBuffer_,
            sizeof(pstDeviceExtension->awcDeviceObjectBuffer_)
        );
        RtlCopyUnicodeString(&pstDeviceExtension->ustrDeviceName_,
                             pustrDeviceObjectName);

        // Set the device initialized finish.
        ClearFlag(pstFilterDeviceObject->Flags, DO_DEVICE_INITIALIZING);

        ntStatus = FSFilterAttachToMountedVolumeDevice(pstDeviceObject);
        if (!NT_SUCCESS(ntStatus))
        {
            KdPrint(("FileSystemFilter!FSFilterAttachFileSystemControlDevice: "
                     "Attach volume device failed.\r\n"));
        }

        ntStatus = STATUS_SUCCESS;
    } while (FALSE);

    if (NULL != pustrDriverObjectName)
    {
        POBJECT_NAME_INFORMATION pstObjectNameInfo =
            CONTAINING_RECORD(pustrDriverObjectName, 
                              OBJECT_NAME_INFORMATION, 
                              Name);
        ExFreePoolWithTag(pstObjectNameInfo, OBJECT_NAME_TAG);
        pstObjectNameInfo = NULL;
    }

    return ntStatus;
} //! FSFilterAttachFileSystemControlDevice() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-09
// Last Time:           2019-02-09
// Logical Description:  
//      Traversing all attached device to find our filter device, detach and
//      delete it.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterDetachFromFileSystemControlDevice(
    IN PDEVICE_OBJECT pstDeviceObject
)
{
    if (!ARGUMENT_PRESENT(pstDeviceObject))
    {
        KdPrint(
            ("FilterSystemFilter!FSFilterDetachFromFileSystemControlDevice: "
             "The file system device object is invalid.\r\n");
        );
        return STATUS_INVALID_PARAMETER;
    }

    PAGED_CODE(); 

    PDEVICE_OBJECT pstAttachedDeviceObject = NULL;
    PDEVICE_EXTENSION pstDeviceExtension = NULL;

    pstAttachedDeviceObject = IoGetAttachedDeviceReference(pstDeviceObject);
    while (pstDeviceObject != pstAttachedDeviceObject)
    {
        if (IS_MY_FILTER_DEVICE_OBJECT(pstAttachedDeviceObject))
        {
            KdPrint(("Detach control deivce filter of %wZ",
                     &pstDeviceExtension->ustrDeviceName_));

            IoDetachDevice(pstDeviceObject);
            IoDeleteDevice(pstAttachedDeviceObject);
        }

        pstDeviceObject = pstAttachedDeviceObject;
        pstAttachedDeviceObject = IoGetAttachedDeviceReference(pstDeviceObject);
        ObDereferenceObject(pstDeviceObject);
    }

    return STATUS_SUCCESS;
} //! FSFilterDetachFromFileSystemControlDevice() END