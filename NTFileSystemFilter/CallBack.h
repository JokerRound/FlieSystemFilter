//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-09
// Description: 
//      The header file of call back routine.
//
// Modify Log:
//      2019-02-09    Hoffman
//      Info: a. Add below routine.
//              a.1. FSFilterFsChangeNotify();
//
//      2019-02-09    Hoffman
//      Info: a. Add below routine.
//              a.1. FSFilterMountDeviceComplete();
//
//******************************************************************************

#pragma once
#ifndef CALLBACK_H_
#define CALLBACK_H_
VOID FSFilterFsChangeNotify(IN PDEVICE_OBJECT pstDeviceObject, 
                            IN BOOLEAN bFSActive);

NTSTATUS FSFilterMountDeviceComplete(IN PDEVICE_OBJECT pstDeviceObject, 
                                     IN PIRP pstIrp, IN PVOID pContext);

NTSTATUS FSFilterReadComplete(IN PDEVICE_OBJECT pstDeviceObject,
                              IN PIRP pstIrp,
                              IN PVOID pContext);

NTSTATUS FSFilterLoadFileSystemComplete(IN PDEVICE_OBJECT pstDeviceObject, 
                                        IN PIRP pstIrp, 
                                        IN PVOID pContext);


#endif // !CALLBACK_H_
