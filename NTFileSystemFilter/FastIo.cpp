//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-07
// Description: 
//      The routines deal with fast I/O.
//
// Modify Log:
//      2019-02-07    Hoffman
//      Info: a. Add below routine.
//              a.1.  FSFilterFastIoCheckIfPossible():
//              a.2.  FSFilterFastIoRead();
//              a.3.  FSFilterFastIoWrite();
//              a.4.  FSFilterFastIoQueryBasicInfo();
//              a.5.  FSFilterFastIoQueryStandardInfo();
//              a.6.  FSFilterFastIoQueryOpen();
//              a.7.  FSFilterFastIoQueryNetworkOpenInfo();
//              a.8.  FSFilterFastIoLock();
//              a.9. FSFilterFastIoUnlockAll();
//              a.10. FSFilterFastIoUnlockSingle();
//              a.11. FSFilterFastIoUnlockAllByKey();
//              a.12. FSFilterFastIoDeviceControl();
//              a.13. FSFilterFastIoDetachDevice();
//              a.14. FSFilterFastIoMdlRead();
//              a.15. FSFilterFastIoMdlReadComplete();
//              a.16. FSFilterFastIoMdlReadCompleteCompressed();
//
//      2019-02-08    Hoffman
//      Info: a. Add below routine.
//              a.1.  FSFilterFastIoPrepareMdlWrite();
//              a.1.  FSFilterFastIoMdlWriteComplete():
//              a.1.  FSFilterFastIoMdlWriteCompleteCompressed():
//              a.1.  FSFilterFastIoReadCompressed();
//              a.1.  FSFilterFastIoWriteCompressed();
//
//******************************************************************************


#include "Launch.h"
#include "FastIo.h"

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Noting to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoCheckIfPossible(IN PFILE_OBJECT pstFileObject,
                                      IN PLARGE_INTEGER pstFileOffset,
                                      IN ULONG ulLength,
                                      IN BOOLEAN bWait,
                                      IN ULONG ulLockKey,
                                      IN BOOLEAN bCheckForReadOperation,
                                      OUT PIO_STATUS_BLOCK pstIoStatus,
                                      IN PDEVICE_OBJECT pstDeviceObject)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstFileOffset);
    UNREFERENCED_PARAMETER(ulLength);
    UNREFERENCED_PARAMETER(bWait);
    UNREFERENCED_PARAMETER(ulLockKey);
    UNREFERENCED_PARAMETER(bCheckForReadOperation);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoCheckIfPossible() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Noting to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoRead(IN PFILE_OBJECT pstFileObject,
                           IN PLARGE_INTEGER pstFileOffset,
                           IN ULONG ulLength,
                           IN BOOLEAN bWait,
                           IN ULONG ulLockKey,
                           OUT PVOID pBuffer,
                           OUT PIO_STATUS_BLOCK pstIoStatus,
                           IN PDEVICE_OBJECT pstDeviceObject)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstFileOffset);
    UNREFERENCED_PARAMETER(ulLength);
    UNREFERENCED_PARAMETER(bWait);
    UNREFERENCED_PARAMETER(ulLockKey);
    UNREFERENCED_PARAMETER(pBuffer);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoRead() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Noting to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoWrite(IN PFILE_OBJECT pstFileObject,
                            IN PLARGE_INTEGER pstFileOffset,
                            IN ULONG ulLength,
                            IN BOOLEAN bWait,
                            IN ULONG ulLockKey,
                            OUT PVOID pBuffer,
                            OUT PIO_STATUS_BLOCK pstIoStatus,
                            IN PDEVICE_OBJECT pstDeviceObject)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstFileOffset);
    UNREFERENCED_PARAMETER(ulLength);
    UNREFERENCED_PARAMETER(bWait);
    UNREFERENCED_PARAMETER(ulLockKey);
    UNREFERENCED_PARAMETER(pBuffer);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoWrite() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Noting to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoQueryBasicInfo(IN PFILE_OBJECT FileObject,
                                     IN BOOLEAN Wait,
                                     OUT PFILE_BASIC_INFORMATION Buffer,
                                     OUT PIO_STATUS_BLOCK IoStatus,
                                     IN PDEVICE_OBJECT DeviceObject)
{
    UNREFERENCED_PARAMETER(FileObject);
    UNREFERENCED_PARAMETER(Wait);
    UNREFERENCED_PARAMETER(Buffer);
    UNREFERENCED_PARAMETER(IoStatus);
    UNREFERENCED_PARAMETER(DeviceObject);

    return FALSE;
} //! FSFilterFastIoQueryBasicInfo() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Noting to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoQueryStandardInfo(
    IN PFILE_OBJECT pstFileObject,
    IN BOOLEAN bWait,
    OUT PFILE_STANDARD_INFORMATION pstBuffer,
    OUT PIO_STATUS_BLOCK pstIoStatus,
    IN PDEVICE_OBJECT pstDeviceObject
)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(bWait);
    UNREFERENCED_PARAMETER(pstBuffer);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject); 

    return FALSE;
} //! FSFilterFastIoQueryStandardInfo() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoQueryOpen(
    IN PIRP pstIrp,
    OUT PFILE_NETWORK_OPEN_INFORMATION pstNetworkInformation,
    IN PDEVICE_OBJECT pstDeviceObject
)
{
    UNREFERENCED_PARAMETER(pstIrp);
    UNREFERENCED_PARAMETER(pstNetworkInformation);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoQueryOpen() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Noting to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoQueryNetworkOpenInfo(
    IN PFILE_OBJECT pstFileObject,
    IN BOOLEAN bWait,
    OUT PFILE_NETWORK_OPEN_INFORMATION pstBuffer,
    OUT PIO_STATUS_BLOCK pstIoStatus,
    IN PDEVICE_OBJECT pstDeviceObject
)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(bWait);
    UNREFERENCED_PARAMETER(pstBuffer);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject); 

    return FALSE;
} //! FSFilterFastIoQueryNetworkOpenInfo() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoLock(IN PFILE_OBJECT pstFileObject,
                           IN PLARGE_INTEGER pstFileOffset,
                           IN PLARGE_INTEGER pstLength,
                           IN PEPROCESS pstProcessId,
                           IN ULONG ulKey,
                           IN BOOLEAN bFailImmediately,
                           IN BOOLEAN bExclusiveLock,
                           OUT PIO_STATUS_BLOCK pstIoStatus,
                           IN PDEVICE_OBJECT pstDeviceObject)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstFileOffset);
    UNREFERENCED_PARAMETER(pstLength);
    UNREFERENCED_PARAMETER(pstProcessId);
    UNREFERENCED_PARAMETER(ulKey);
    UNREFERENCED_PARAMETER(bFailImmediately);
    UNREFERENCED_PARAMETER(bExclusiveLock);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoLock() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoUnlockAll(IN PFILE_OBJECT pstFileObject,
                                IN PEPROCESS pstProcessId,
                                OUT PIO_STATUS_BLOCK pstIoStatus,
                                IN PDEVICE_OBJECT pstDeviceObject)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstProcessId);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoUnlockAll() END


//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-08
// Last Time:           2019-02-08
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoUnlockSingle(IN PFILE_OBJECT pstFileObject,
                                   IN PLARGE_INTEGER pstFileOffset,
                                   IN PLARGE_INTEGER pstLength,
                                   IN PEPROCESS pstProcessId,
                                   IN ULONG ulKey,
                                   OUT PIO_STATUS_BLOCK pstIoStatus,
                                   IN PDEVICE_OBJECT pstDeviceObject)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstFileOffset);
    UNREFERENCED_PARAMETER(pstLength);
    UNREFERENCED_PARAMETER(pstProcessId);
    UNREFERENCED_PARAMETER(ulKey);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoUnlockSingle() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoUnlockAllByKey(IN PFILE_OBJECT pstFileObject,
                                     IN PVOID pstProcessId,
                                     IN ULONG pstKey,
                                     OUT PIO_STATUS_BLOCK pstIoStatus,
                                     IN PDEVICE_OBJECT pstDeviceObject)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstProcessId);
    UNREFERENCED_PARAMETER(pstKey);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoUnlockAllByKey() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoDeviceControl(IN PFILE_OBJECT pstFileObject,
                                    IN BOOLEAN bWait,
                                    IN PVOID pInputBuffer OPTIONAL,
                                    IN ULONG ulInputBufferLength,
                                    OUT PVOID pOutputBuffer OPTIONAL,
                                    IN ULONG ulOutputBufferLength,
                                    IN ULONG ulIoControlCode,
                                    OUT PIO_STATUS_BLOCK pstIoStatus,
                                    IN PDEVICE_OBJECT pstDeviceObject)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(bWait);
    UNREFERENCED_PARAMETER(pInputBuffer);
    UNREFERENCED_PARAMETER(ulInputBufferLength);
    UNREFERENCED_PARAMETER(pOutputBuffer);
    UNREFERENCED_PARAMETER(ulOutputBufferLength);
    UNREFERENCED_PARAMETER(ulIoControlCode);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoDeviceControl() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
VOID FSFilterFastIoDetachDevice(IN PDEVICE_OBJECT pstSourceDevice,
                                IN PDEVICE_OBJECT pstTargetDevice)
{
    UNREFERENCED_PARAMETER(pstSourceDevice);
    UNREFERENCED_PARAMETER(pstTargetDevice);
} //! FSFilterFastIoDetachDevice() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoMdlRead(IN PFILE_OBJECT pstFileObject,
                              IN PLARGE_INTEGER pstFileOffset,
                              IN ULONG ulLength,
                              IN ULONG ulLockKey,
                              OUT PMDL *ppMdlChain,
                              OUT PIO_STATUS_BLOCK pstIoStatus,
                              IN PDEVICE_OBJECT pstDeviceObject
)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstFileOffset);
    UNREFERENCED_PARAMETER(ulLength);
    UNREFERENCED_PARAMETER(ulLockKey);
    UNREFERENCED_PARAMETER(ppMdlChain);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoMdlRead() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Nothint to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoMdlReadComplete(IN PFILE_OBJECT pstFileObject,
                                      IN PMDL pstMdlChain,
                                      IN PDEVICE_OBJECT pstDeviceObject)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstMdlChain);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoMdlReadComplete() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-07
// Last Time:           2019-02-07
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoMdlReadCompleteCompressed(
    IN PFILE_OBJECT pstFileObject,
    IN PMDL pstMdlChain,
    IN PDEVICE_OBJECT pstDeviceObject
)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstMdlChain);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoMdlReadCompleteCompressed() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-08
// Last Time:           2019-02-08
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoPrepareMdlWrite(IN PFILE_OBJECT pstFileObject,
                                      IN PLARGE_INTEGER pstFileOffset,
                                      IN ULONG ulLength,
                                      IN ULONG ulLockKey,
                                      OUT PMDL *pstMdlChain,
                                      OUT PIO_STATUS_BLOCK pstIoStatus,
                                      IN PDEVICE_OBJECT pstDeviceObject)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstFileOffset);
    UNREFERENCED_PARAMETER(ulLength);
    UNREFERENCED_PARAMETER(ulLockKey);
    UNREFERENCED_PARAMETER(pstMdlChain);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoPrepareMdlWrite() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-08
// Last Time:           2019-02-08
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoMdlWriteComplete(IN PFILE_OBJECT pstFileObject,
                                       IN PLARGE_INTEGER pstFileOffset,
                                       IN PMDL pstMdlChain,
                                       IN PDEVICE_OBJECT pstDeviceObject)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstFileOffset);
    UNREFERENCED_PARAMETER(pstMdlChain);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoMdlWriteComplete() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-08
// Last Time:           2019-02-08
// Logical Description:  
//      Nothing to do.
//******************************************************************************
#pragma PAGEDCODE
BOOLEAN FSFilterFastIoMdlWriteCompleteCompressed(
    IN PFILE_OBJECT pstFileObject,
    IN PLARGE_INTEGER pstFileOffset,
    IN PMDL pstMdlChain,
    IN PDEVICE_OBJECT pstDeviceObject
)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstFileOffset);
    UNREFERENCED_PARAMETER(pstMdlChain);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! SfFastIoMdlWriteCompleteCompressed() END

//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-08
// Last Time:           2019-02-08
// Logical Description:  
//      Nothing to do.
//******************************************************************************
BOOLEAN FSFilterFastIoReadCompressed(
    IN PFILE_OBJECT pstFileObject,
    IN PLARGE_INTEGER pstFileOffset,
    IN ULONG ulLength,
    IN ULONG ulLockKey,
    OUT PVOID pBuffer,
    OUT PMDL *ppstMdlChain,
    OUT PIO_STATUS_BLOCK pstIoStatus,
    OUT struct _COMPRESSED_DATA_INFO *pstCompressedDataInfo,
    IN ULONG ulCompressedDataInfoLength,
    IN PDEVICE_OBJECT pstDeviceObject
)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstFileOffset);
    UNREFERENCED_PARAMETER(ulLength);
    UNREFERENCED_PARAMETER(ulLockKey);
    UNREFERENCED_PARAMETER(pBuffer);
    UNREFERENCED_PARAMETER(ppstMdlChain);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstCompressedDataInfo);
    UNREFERENCED_PARAMETER(ulCompressedDataInfoLength);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;
} //! FSFilterFastIoReadCompressed() END
	
//******************************************************************************
// Author:              Hoffman
// Create Time:         2019-02-08
// Last Time:           2019-02-08
// Logical Description:  
//      Nothing to do.
//******************************************************************************
BOOLEAN FSFilterFastIoWriteCompressed(
    IN PFILE_OBJECT pstFileObject,
    IN PLARGE_INTEGER pstFileOffset,
    IN ULONG ulLength,
    IN ULONG ulLockKey,
    IN PVOID pBuffer,
    OUT PMDL *ppstMdlChain,
    OUT PIO_STATUS_BLOCK pstIoStatus,
    IN struct _COMPRESSED_DATA_INFO *pstCompressedDataInfo,
    IN ULONG ulCompressedDataInfoLength,
    IN PDEVICE_OBJECT pstDeviceObject
)
{
    UNREFERENCED_PARAMETER(pstFileObject);
    UNREFERENCED_PARAMETER(pstFileOffset);
    UNREFERENCED_PARAMETER(ulLength);
    UNREFERENCED_PARAMETER(ulLockKey);
    UNREFERENCED_PARAMETER(pBuffer);
    UNREFERENCED_PARAMETER(ppstMdlChain);
    UNREFERENCED_PARAMETER(pstIoStatus);
    UNREFERENCED_PARAMETER(pstCompressedDataInfo);
    UNREFERENCED_PARAMETER(ulCompressedDataInfoLength);
    UNREFERENCED_PARAMETER(pstDeviceObject);

    return FALSE;

} //! FSFilterFastIoWriteCompressed() END

