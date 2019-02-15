//******************************************************************************
// License:     MIT
// Author:      Hoffman
// GitHub:      https://github.com/JokerRound
// Create Time: 2019-02-07
// Description: 
//      The declare of fast io routine.
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
//******************************************************************************

#pragma once
#ifndef FASTIO_H_
#define FASTIO_H_


BOOLEAN FSFilterFastIoCheckIfPossible(IN PFILE_OBJECT pstFileObject, 
                                      IN PLARGE_INTEGER pstFileOffset, 
                                      IN ULONG ulLength, 
                                      IN BOOLEAN bWait, 
                                      IN ULONG ulLockKey, 
                                      IN BOOLEAN bCheckForReadOperation,
                                      OUT PIO_STATUS_BLOCK pstIoStatus,
                                      IN PDEVICE_OBJECT pstDeviceObject);

BOOLEAN FSFilterFastIoRead(IN PFILE_OBJECT pstFileObject, 
                           IN PLARGE_INTEGER pstFileOffset, 
                           IN ULONG ulLength, 
                           IN BOOLEAN bWait, 
                           IN ULONG ulLockKey, 
                           OUT PVOID pBuffer, 
                           OUT PIO_STATUS_BLOCK pstIoStatus, 
                           IN PDEVICE_OBJECT pstDeviceObject);

BOOLEAN FSFilterFastIoWrite(IN PFILE_OBJECT pstFileObject, 
                            IN PLARGE_INTEGER pstFileOffset, 
                            IN ULONG ulLength, 
                            IN BOOLEAN bWait, 
                            IN ULONG ulLockKey, 
                            OUT PVOID pBuffer, 
                            OUT PIO_STATUS_BLOCK pstIoStatus, 
                            IN PDEVICE_OBJECT pstDeviceObject);

BOOLEAN FSFilterFastIoQueryBasicInfo(IN PFILE_OBJECT FileObject, 
                                     IN BOOLEAN Wait, 
                                     OUT PFILE_BASIC_INFORMATION Buffer, 
                                     OUT PIO_STATUS_BLOCK IoStatus, 
                                     IN PDEVICE_OBJECT DeviceObject);

BOOLEAN FSFilterFastIoQueryStandardInfo(
    IN PFILE_OBJECT pstFileObject,
    IN BOOLEAN bWait,
    OUT PFILE_STANDARD_INFORMATION pstBuffer,
    OUT PIO_STATUS_BLOCK pstIoStatus, 
    IN PDEVICE_OBJECT pstDeviceObject
);

BOOLEAN FSFilterFastIoQueryOpen(
    IN PIRP pstIrp,
    OUT PFILE_NETWORK_OPEN_INFORMATION pstNetworkInformation, 
    IN PDEVICE_OBJECT pstDeviceObject
);

BOOLEAN FSFilterFastIoQueryNetworkOpenInfo(
    IN PFILE_OBJECT pstFileObject,
    IN BOOLEAN bWait,
    OUT PFILE_NETWORK_OPEN_INFORMATION pstBuffer, 
    OUT PIO_STATUS_BLOCK pstIoStatus, 
    IN PDEVICE_OBJECT pstDeviceObject
);

BOOLEAN FSFilterFastIoLock(IN PFILE_OBJECT pstFileObject,
                           IN PLARGE_INTEGER pstFileOffset,
                           IN PLARGE_INTEGER pstLength,
                           IN PEPROCESS pstProcessId,
                           IN ULONG ulKey,
                           IN BOOLEAN bFailImmediately,
                           IN BOOLEAN bExclusiveLock,
                           OUT PIO_STATUS_BLOCK pstIoStatus,
                           IN PDEVICE_OBJECT pstDeviceObject);

BOOLEAN FSFilterFastIoUnlockAll(IN PFILE_OBJECT pstFileObject,
                                IN PEPROCESS pstProcessId,
                                OUT PIO_STATUS_BLOCK pstIoStatus,
                                IN PDEVICE_OBJECT pstDeviceObject
);

BOOLEAN FSFilterFastIoUnlockSingle(IN PFILE_OBJECT pstFileObject,
                                   IN PLARGE_INTEGER pstFileOffset,
                                   IN PLARGE_INTEGER pstLength,
                                   IN PEPROCESS pstProcessId,
                                   IN ULONG ulKey,
                                   OUT PIO_STATUS_BLOCK pstIoStatus,
                                   IN PDEVICE_OBJECT pstDeviceObject);

BOOLEAN FSFilterFastIoUnlockAllByKey(IN PFILE_OBJECT pstFileObject,
                                     IN PVOID pstProcessId,
                                     IN ULONG pstKey,
                                     OUT PIO_STATUS_BLOCK pstIoStatus,
                                     IN PDEVICE_OBJECT pstDeviceObject);

BOOLEAN FSFilterFastIoDeviceControl(IN PFILE_OBJECT pstFileObject,
                                    IN BOOLEAN bWait,
                                    IN PVOID pInputBuffer OPTIONAL,
                                    IN ULONG ulInputBufferLength,
                                    OUT PVOID pOutputBuffer OPTIONAL,
                                    IN ULONG ulOutputBufferLength,
                                    IN ULONG ulIoControlCode,
                                    OUT PIO_STATUS_BLOCK pstIoStatus,
                                    IN PDEVICE_OBJECT pstDeviceObject);

VOID FSFilterFastIoDetachDevice(IN PDEVICE_OBJECT pstSourceDevice,
                                IN PDEVICE_OBJECT pstTargetDevice);

BOOLEAN FSFilterFastIoMdlRead(IN PFILE_OBJECT pstFileObject,
                              IN PLARGE_INTEGER pstFileOffset,
                              IN ULONG ulLength,
                              IN ULONG ulLockKey,
                              OUT PMDL *ppMdlChain,
                              OUT PIO_STATUS_BLOCK pstIoStatus,
                              IN PDEVICE_OBJECT pstDeviceObject);

BOOLEAN FSFilterFastIoMdlReadComplete(IN PFILE_OBJECT pstFileObject,
                                      IN PMDL pstMdlChain,
                                      IN PDEVICE_OBJECT pstDeviceObject);

BOOLEAN FSFilterFastIoMdlReadCompleteCompressed(
    IN PFILE_OBJECT pstFileObject,
    IN PMDL pstMdlChain,
    IN PDEVICE_OBJECT pstDeviceObject
);

BOOLEAN FSFilterFastIoPrepareMdlWrite(IN PFILE_OBJECT pstFileObject,
                                      IN PLARGE_INTEGER pstFileOffset,
                                      IN ULONG ulLength,
                                      IN ULONG ulLockKey,
                                      OUT PMDL *pstMdlChain,
                                      OUT PIO_STATUS_BLOCK pstIoStatus,
                                      IN PDEVICE_OBJECT pstDeviceObject);

BOOLEAN FSFilterFastIoMdlWriteComplete(IN PFILE_OBJECT pstFileObject,
                                       IN PLARGE_INTEGER pstFileOffset,
                                       IN PMDL pstMdlChain,
                                       IN PDEVICE_OBJECT pstDeviceObject);

BOOLEAN FSFilterFastIoMdlWriteCompleteCompressed(
    IN PFILE_OBJECT pstFileObject,
    IN PLARGE_INTEGER pstFileOffset,
    IN PMDL pstMdlChain,
    IN PDEVICE_OBJECT pstDeviceObject
);

BOOLEAN FSFilterFastIoReadCompressed(
    IN PFILE_OBJECT pstFileObject,
    IN PLARGE_INTEGER pstFileOffset,
    IN ULONG ulLength,
    IN ULONG ulLockKey,
    OUT PVOID pBuffer,
    OUT PMDL *ppstMdlChain,
    OUT PIO_STATUS_BLOCK pstIoStatus,
    OUT _COMPRESSED_DATA_INFO *pstCompressedDataInfo,
    IN ULONG ulCompressedDataInfoLength,
    IN PDEVICE_OBJECT pstDeviceObject
);

BOOLEAN FSFilterFastIoWriteCompressed(
    IN PFILE_OBJECT pstFileObject,
    IN PLARGE_INTEGER pstFileOffset,
    IN ULONG ulLength,
    IN ULONG ulLockKey,
    IN PVOID pBuffer,
    OUT PMDL *ppstMdlChain,
    OUT PIO_STATUS_BLOCK pstIoStatus,
    IN _COMPRESSED_DATA_INFO *pstCompressedDataInfo,
    IN ULONG ulCompressedDataInfoLength,
    IN PDEVICE_OBJECT pstDeviceObject
);
#endif // !FASTIO_H_