//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-06
// Description: 
//      The routines deal with irp. 
//
// Modify Log:
//      2019-02-06    Hoffman
//      Info: a. Add below routines.
//              a.1. FSFilterIrpDefault();
//
//      2019-02-09    Hoffman
//      Info: a. Add below routines.
//              a.1. FSFilterIrpFileSystemControl();
//
//      2019-02-11    Hoffman
//      Info: a. Add below routines.
//              a.1. FSFilterIrpRead();
//              a.2. FSFilterIrpWrite();
//******************************************************************************

#pragma once
#ifndef IRP_H_
#define IRP_H_


_Dispatch_type_(IRP_MJ_CLOSE)
NTSTATUS FSFilterIrpDefault(IN PDEVICE_OBJECT pstDeviceObject,
                            IN PIRP pstIrp);

//_Dispatch_type_(IRP_MJ_CREATE)
//NTSTATUS FSFilterIrpCreate(IN PDEVICE_OBJECT pstDeviceObject,
//                           IN PIRP pstIrp);


_Dispatch_type_(IRP_MJ_READ)
NTSTATUS FSFilterIrpRead(IN PDEVICE_OBJECT pstDeviceObject, IN PIRP pstIrp);

_Dispatch_type_(IRP_MJ_WRITE)
NTSTATUS FSFilterIrpWrite(IN PDEVICE_OBJECT pstDeviceObject, IN PIRP pstIrp);

NTSTATUS FSFilterAttachMountedVolume(IN PDEVICE_OBJECT pstFilterDeviceObject, 
                                     IN PDEVICE_OBJECT pstDeviceObject, 
                                     IN PIRP pstIrp);

_Dispatch_type_(IRP_MJ_FILE_SYSTEM_CONTROL)
NTSTATUS FSFilterIrpFileSystemControl(IN PDEVICE_OBJECT pstDeviceObject, 
                                      IN PIRP pstIrp);

#endif // !IRP_H_
