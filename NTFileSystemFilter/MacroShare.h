//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-06
// Description: 
//      The macroes for sharing in this project.
//
// Modify Log:
//      2019-02-06    Hoffman
//      Info: a. Add below macroes.
//              a.1.  PAGEDCODE;
//              a.2.  LOCKEDCODE;
//              a.3.  INITCODE;
//              a.4.  PAGEDDATA;
//              a.5.  LOCKEDDATA; 
//              a.6.  INITDATA; 
//              a.7.  ARRAY_SIZE;
//              a.8.  DEVICE_OBJECT_LIST_TAG;
//              a.9.  CONTROL_DEVICE_NAME;
//              a.10. OLD_CONTROL_DEVICE_NAME;
//
//      2019-02-07    Hoffman
//      Info: a. Add below macroes.
//              a.1. IS_MY_FILTER_DEVICE_OBJECT;
//              a.2. IS_MY_CONTROL_DEVICE_OBJECT;
//
//      2019-02-08    Hoffman
//      Info: a. Add below macroes.
//              a.1. MAX_DEVICENAME_LEN;
//              a.2. IS_TARGET_DEVICE_TYPE;
//
//      2019-02-08    Hoffman
//      Info: a. Add below macroes.
//              a.1. FILE_SYSTEM_REC_NAME
//              a.2. OBJECT_NAME_TAG
//
//      2019-02-11   Hoffman
//      Info: a. Add below macroes.
//              a.1. ATTACH_VOLUME_DEVICE_TRY_NUM;
//              a.2. DELAY_ONE_MICROSECOND;	
//              a.3. DELAY_ONE_MILLISECOND;	
//              a.4. DELAY_ONE_SECOND;	
//******************************************************************************

#pragma once
#ifndef MACROSHARE_H_
#define MACROSHARE_H_

#define PAGEDCODE code_seg("PAGE")
#define LOCKEDCODE code_seg()
#define INITCODE code_seg("INIT")

#define PAGEDDATA data_seg("PAGE")
#define LOCKEDDATA data_seg()
#define INITDATA data_seg("INIT")

#define ARRAY_SIZE(p) (sizeof(p) / sizeof(p)[0])

#define DEVICE_OBJECT_LIST_TAG 'DOLT'
#define FAST_IO_DISPATCH_TAG 'FIDT'
#define OBJECT_NAME_TAG 'ONT'

#define CONTROL_DEVICE_NAME L"\\FileSystem\\Filters\\SFilter"
#define OLD_CONTROL_DEVICE_NAME L"\\FileSystem\\SFilter"
#define FILE_SYSTEM_REC_NAME L"\\FileSystem\\Fs_Rec"

// the limit of device name's length.
#define MAX_DEVICENAME_LEN 512

// Check the device object is filter device or not.
#define IS_MY_FILTER_DEVICE_OBJECT(_pstDeviceObject) \
    ((NULL != (_pstDeviceObject)) && \
        ((_pstDeviceObject)->DriverObject == g_pstDriverObject) && \
            (NULL != (_pstDeviceObject)->DeviceExtension))   

// Check the device object is control device or not.
#define IS_MY_CONTROL_DEVICE_OBJECT(_pstDeviceObject) \
    ((NULL != (_pstDeviceObject)) && \
        ((_pstDeviceObject)->DriverObject == g_pstDriverObject) && \
            (NULL == (_pstDeviceObject)->DeviceExtension))

// Check the device type is target type or not.
#define IS_TARGET_DEVICE_TYPE(_type) \
    (FILE_DEVICE_DISK_FILE_SYSTEM == (_type))


#define ATTACH_VOLUME_DEVICE_TRY_NUM 16

// Delay time.
#define DELAY_ONE_MICROSECOND	(-10)
#define DELAY_ONE_MILLISECOND	(DELAY_ONE_MICROSECOND*1000)
#define DELAY_ONE_SECOND		(DELAY_ONE_MILLISECOND*1000)
#endif // !MACROSHARE_H_