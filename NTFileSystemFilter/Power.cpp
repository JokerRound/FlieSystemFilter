//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-06
// Description: 
//      The achieve of routine deal with power.
//
// Modify Log:
//      2019-02-06    Hoffman
//      Info: a. Add below routine.
//              a.1. FSFilterPower();
//******************************************************************************

#include "Launch.h"
#include "Power.h"
//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-06
// Last Time:           2019-02-06
// Logical Description:  
//      Pass through to next device.
//******************************************************************************
#pragma PAGEDCODE
NTSTATUS FSFilterPower(IN PDEVICE_OBJECT pstDeviceObject,
                       IN PIRP pstIrp)
{
    PAGED_CODE();

    PDEVICE_EXTENSION pstDeviceExtension =
        (PDEVICE_EXTENSION)pstDeviceObject->DeviceExtension;

    PoStartNextPowerIrp(pstIrp);
    IoSkipCurrentIrpStackLocation(pstIrp);
    return PoCallDriver(pstDeviceExtension->pstNextDeviceObject_, pstIrp);
} //! DriverPower() END