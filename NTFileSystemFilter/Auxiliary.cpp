//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-09
// Description: 
//      The achieve of auxiliary routines.
//
// Modify Log:
//      2019-02-09    Hoffman
//      Info: a. Add below routines.
//              a.1. FSFilterGetObjectName();
//
//      2019-02-11    Hoffman
//      Info: a. Add below routines.
//              a.1. FSFilterIsAttachedDevice() END 
//******************************************************************************
#include "Launch.h"
#include "Auxiliary.h"

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-09
// Last Time:           2019-02-09
// Logical Description:  
//      Querry the length of object name, then allocate memory and get the name.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterGetObjectName(IN PVOID pObject,
                               IN OUT PUNICODE_STRING *pustrObjectName)
{
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    POBJECT_NAME_INFORMATION pstObjectNameInfo = NULL;
    ULONG ulNameNeedSize = 0;

    ntStatus = ObQueryNameString(pObject, NULL, 0, &ulNameNeedSize);
    if (!NT_SUCCESS(ntStatus) && STATUS_INFO_LENGTH_MISMATCH != ntStatus)
    {
        KdPrint(("FileSystemFilter!FSFilterGetObjectName: "
                 "Get length of object name string failed.\r\n"));
        return ntStatus;
    }

    pstObjectNameInfo = 
        (POBJECT_NAME_INFORMATION)ExAllocatePoolWithTag(PagedPool,
                                                        ulNameNeedSize,
                                                        OBJECT_NAME_TAG);
    if (NULL == pstObjectNameInfo)
    {
        KdPrint(("FileSystemFilter!FSFilterGetObjectName: "
                 "Allocate memory for object name string failed.\r\n"));
        return STATUS_INVALID_ADDRESS;
    }

    ntStatus = ObQueryNameString(pObject, 
                                 pstObjectNameInfo, 
                                 ulNameNeedSize, 
                                 &ulNameNeedSize);
    if (!NT_SUCCESS(ntStatus))
    {
        KdPrint(("FileSystemFilter!FSFilterGetObjectName: "
                 "Get object's name failed.\r\n"));
        return ntStatus;
    }

    *pustrObjectName = &(pstObjectNameInfo->Name);

    return ntStatus;
} //! FSFilterGetObjectName() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-11
// Last Time:           2019-02-11
// Logical Description:  
//      Traversing the all object in device stack to look for our filter device
//      object.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterIsAttachedDevice(IN PDEVICE_OBJECT pstDeviceObject)
{
    PAGED_CODE();
    PDEVICE_OBJECT pstCurrentDeviceObject = 
        IoGetAttachedDeviceReference(pstDeviceObject);
    PDEVICE_OBJECT pstNextDeviceObject = NULL;

    // Find filter device.
    do
    {
        if (IS_MY_FILTER_DEVICE_OBJECT(pstCurrentDeviceObject))
        {
            ObDereferenceObject(pstCurrentDeviceObject);
            return TRUE;
        }
        
        pstNextDeviceObject = IoGetLowerDeviceObject(pstCurrentDeviceObject);

        // All object will decrease the rederence.
        ObDereferenceObject(pstCurrentDeviceObject);
        pstCurrentDeviceObject = pstNextDeviceObject;
    } while (NULL != pstCurrentDeviceObject);

    return FALSE;
} //! FSFilterIsAttachedDevice() END