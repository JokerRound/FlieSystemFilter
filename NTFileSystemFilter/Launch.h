//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-06
// Description: 
//      The header file of lunch, and it includes all header file.
//
// Modify Log:
//      2019-02-06    Hoffman
//      Info: a. Add below funtions.
//              a.1. DriverEntry();
//              a.2. FsFilterCreateDevice();
//              a.3. FSFilterUnload();
//
//      2019-02-09    Hoffman
//      Info: a. Add below funtions.
//              a.1. FSFilterAttachToFileSystemControlDevice();
//              a.2. FSFilterDetachFromFileSystemControlDevice();
//******************************************************************************

#pragma once
#ifndef LAUCH_H_
#define LAUCH_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include <ntifs.h>
#ifdef __cplusplus
}
#endif // __cplusplus
#include "TypeShare.h"
#include "GlobalVar.h"
#include "MacroShare.h"
#include "Irp.h"
#include "Power.h"
#include "FastIo.h"
#include "CallBack.h"
#include "Auxiliary.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
DRIVER_INITIALIZE DriverEntry;
#ifdef __cplusplus
}
#endif // __cplusplus

VOID FSFilterUnload(IN PDRIVER_OBJECT pstDriverObject);

NTSTATUS FSFilterCreateDevice(IN PDRIVER_OBJECT pstDriverObject);

NTSTATUS FSFilterAttachToFileSystemControlDevice(
    IN PDEVICE_OBJECT pstDeviceObject, 
    IN PUNICODE_STRING pustrDeviceObjectName
);

NTSTATUS FSFilterDetachFromFileSystemControlDevice(
    IN PDEVICE_OBJECT pstDeviceObject
);

#endif // !LAUCH_H_
