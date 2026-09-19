#include <efi.h>
#include <efilib.h>
#include "file.h"

EFI_STATUS load_kernel(
    EFI_HANDLE ImageHandle,
    void **buffer,
    UINTN *size
)
{
    EFI_LOADED_IMAGE *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    EFI_FILE_HANDLE Root;
    EFI_FILE_HANDLE Kernel;

    EFI_STATUS Status;

    Status = uefi_call_wrapper(
        gBS->HandleProtocol,
        3,
        ImageHandle,
        &LoadedImageProtocol,
        (void **)&LoadedImage
    );
    if (EFI_ERROR(Status))
        return Status;

    Status = uefi_call_wrapper(
        gBS->HandleProtocol,
        3,
        LoadedImage->DeviceHandle,
        &FileSystemProtocol,
        (void **)&FileSystem
    );
    if (EFI_ERROR(Status))
        return Status;

    Status = uefi_call_wrapper(
        FileSystem->OpenVolume,
        2,
        FileSystem,
        &Root
    );
    if (EFI_ERROR(Status))
        return Status;

    Status = uefi_call_wrapper(
        Root->Open,
        5,
        Root,
        &Kernel,
        L"kernel.elf",
        EFI_FILE_MODE_READ,
        0
    );
    if (EFI_ERROR(Status))
        return Status;

    EFI_FILE_INFO *Info;
    UINTN InfoSize = sizeof(EFI_FILE_INFO) + 256;

    Status = uefi_call_wrapper(
        gBS->AllocatePool,
        3,
        EfiLoaderData,
        InfoSize,
        (void **)&Info
    );
    if (EFI_ERROR(Status))
        return Status;

    Status = uefi_call_wrapper(
        Kernel->GetInfo,
        4,
        Kernel,
        &gEfiFileInfoGuid,
        &InfoSize,
        Info
    );
    if (EFI_ERROR(Status))
        return Status;

    *size = Info->FileSize;

    Status = uefi_call_wrapper(
        gBS->AllocatePool,
        3,
        EfiLoaderData,
        *size,
        buffer
    );
    if (EFI_ERROR(Status))
        return Status;

    Status = uefi_call_wrapper(
        Kernel->Read,
        3,
        Kernel,
        size,
        *buffer
    );

    uefi_call_wrapper(Kernel->Close, 1, Kernel);

    return Status;
}