//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-06
// Description: 
//      The declare of routine deal with power.
//
// Modify Log:
//      2019-02-06    Hoffman
//      Info: a. Add below routine.
//              a.1. FSFilterPower();
//******************************************************************************
#pragma once
#ifndef POWER_H_
#define POWER_H_

NTSTATUS FSFilterPower(IN PDEVICE_OBJECT pstDeviceObject, IN PIRP pstIrp);

#endif // !POWER_H_
