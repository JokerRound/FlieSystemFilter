//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-07
// Description: 
//      Initialization of global varaiables.
//
// Modify Log:
//      Info: Moidfy Info
//      2019-02-07    Hoffman
//      Info: a. Add below gloab variables.
//              a.1. g_pstDriverObject; 
//              a.2. g_pstControlDeviceObject;
//
//      2019-02-07    Hoffman
//      Info: a. Add below gloab variables.
//              a.1. g_stAttachLock;
//******************************************************************************

#include "Launch.h"
#include "GlobalVar.h"

PDRIVER_OBJECT g_pstDriverObject = NULL;
PDEVICE_OBJECT g_pstControlDeviceObject = NULL;

FAST_MUTEX g_stAttachLock;
;