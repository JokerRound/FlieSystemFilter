//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-09
// Description: 
//      The header file of axuiliary routine.
//
// Modify Log:
//      2019-02-09    Hoffman
//      Info: a. Add below routine.
//              a.1. FSFilterGetObjectName();
//******************************************************************************
#include "Launch.h"

#pragma once
#ifndef AUXILIARY_H_
#define AUXILIARY_H_

NTSTATUS FSFilterGetObjectName(IN PVOID pObject, 
                               IN OUT PUNICODE_STRING *pustrObjectName);

BOOLEAN FSFilterIsAttachedDevice(IN PDEVICE_OBJECT pstDeviceObject);
#endif // !AUXILIARY_H_
