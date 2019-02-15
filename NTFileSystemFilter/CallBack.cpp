//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-09
// Description: 
//      The achieve of call back routine.
//
// Modify Log:
//      2019-02-09    Hoffman
//      Info: a. Add below routine.
//              a.1. FSFilterFsChangeNotify();
//
//      2019-02-11    Hoffman
//      Info: a. Add below routine.
//              a.1. FSFilterMountDeviceComplete();
//
//******************************************************************************

#include "Launch.h"


//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-08
// Last Time:           2019-02-08
// Logical Description:  
//      Print some debug info, then executive attach or detach operation.
//******************************************************************************
#pragma PAGEDCODE
VOID FSFilterFsChangeNotify(IN PDEVICE_OBJECT pstDeviceObject,
                            IN BOOLEAN bFSActive)
{
    PAGED_CODE();
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    PUNICODE_STRING pustrDeviceObjectName = NULL;

    ntStatus = FSFilterGetObjectName(pstDeviceObject, &pustrDeviceObjectName);
    if (!NT_SUCCESS(ntStatus))
    {
        return;
    }

    KdPrint(("Device Name: %wZ\r\n", pustrDeviceObjectName));

    if (bFSActive)
    { 
        FSFilterAttachToFileSystemControlDevice(pstDeviceObject, 
                                                pustrDeviceObjectName);
    }
    else
    {
        FSFilterDetachFromFileSystemControlDevice(pstDeviceObject);
    }
    
    if (NULL != pustrDeviceObjectName)
    {
        POBJECT_NAME_INFORMATION pstObjectNameInfo = 
            CONTAINING_RECORD(pustrDeviceObjectName, 
                              OBJECT_NAME_INFORMATION, 
                              Name);
        ExFreePoolWithTag(pstObjectNameInfo, OBJECT_NAME_TAG);
        pstObjectNameInfo = NULL;
    }

} //! FSFilterFsChangeNotify() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-11
// Last Time:           2019-02-11
// Logical Description:  
//      Set the complete event.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterMountDeviceComplete(IN PDEVICE_OBJECT pstDeviceObject,
                                     IN PIRP pstIrp,
                                     IN PVOID pContext)
{
    UNREFERENCED_PARAMETER(pstDeviceObject);
    UNREFERENCED_PARAMETER(pstIrp);

    ASSERT(IS_MY_FILTER_DEVICE_OBJECT(pstDeviceObject));
    ASSERT(NULL != pContext);

    KeSetEvent((PKEVENT)pContext, IO_NO_INCREMENT, FALSE);

    return STATUS_MORE_PROCESSING_REQUIRED;
} //! FSFilterMountDeviceComplete() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-11
// Last Time:           2019-02-11
// Logical Description:  
//      Set the event of complete.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterReadComplete(IN PDEVICE_OBJECT pstDeviceObject,
                              IN PIRP pstIrp,
                              IN PVOID pContext)
{
    UNREFERENCED_PARAMETER(pstDeviceObject);
    UNREFERENCED_PARAMETER(pstIrp);

    ASSERT(IS_MY_FILTER_DEVICE_OBJECT(pstDeviceObject));
    ASSERT(NULL != pContext);

    KeSetEvent((PKEVENT)pContext, IO_NO_INCREMENT, FALSE);

    return STATUS_MORE_PROCESSING_REQUIRED;
} //! FSFilterReadComplete() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-11
// Last Time:           2019-02-11
// Logical Description:  
//      Set the complete event.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterLoadFileSystemComplete(IN PDEVICE_OBJECT pstDeviceObject,
                                        IN PIRP pstIrp,
                                        IN PVOID pContext)
{
    UNREFERENCED_PARAMETER(pstDeviceObject);
    UNREFERENCED_PARAMETER(pstIrp);

    ASSERT(IS_MY_FILTER_DEVICE_OBJECT(pstDeviceObject));
    ASSERT(NULL != pContext);

    KeSetEvent((PKEVENT)pContext, IO_NO_INCREMENT, FALSE);

    return STATUS_MORE_PROCESSING_REQUIRED;
} //! FSFilterLoadFileSystemComplete() END
