//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-07
// Description: 
//      The declares of gloab variables in this file.
//
// Modify Log:
//      2019-02-07    Hoffman
//      Info: a. Add below gloab variables.
//              a.1. g_pstDriverObject; 
//              a.2. g_pstControlDeviceObject;
//
//      2019-02-07    Hoffman
//      Info: a. Add below gloab variables.
//              a.1. g_stAttachLock;
//******************************************************************************

#pragma once
#ifndef GLOBALVAR_H_
#define GLOBALVAR_H_

extern PDRIVER_OBJECT g_pstDriverObject;
extern PDEVICE_OBJECT g_pstControlDeviceObject;
extern FAST_MUTEX g_stAttachLock;

#endif // !GLOABVAR_H_

