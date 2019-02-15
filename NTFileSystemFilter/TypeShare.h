//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-06
// Description: 
//      The types for sharing in this project.
//
// Modify Log:
//      2019-02-06    Hoffman
//      Info: a. Add below type.
//              a.1. struct tagDeviceExtension;
//******************************************************************************

#pragma once
#ifndef TYPESHARE_H_
#define TYPESHARE_H_
#include "MacroShare.h"

typedef struct tagDeviceExtension
{
    PDEVICE_OBJECT  pstDeviceObject_;
    PDEVICE_OBJECT  pstNextDeviceObject_;
    PDEVICE_OBJECT  pstStorageDeviceObject_;
    UNICODE_STRING  ustrDeviceName_;
    WCHAR           awcDeviceObjectBuffer_[MAX_DEVICENAME_LEN];
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

#endif // !TYPESHARE_H_
