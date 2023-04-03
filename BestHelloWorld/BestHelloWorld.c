#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/HiiFont.h>


EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
EFI_STATUS Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

  EFI_GRAPHICS_OUTPUT_BLT_PIXEL GColor;
  GColor.Reserved = 154;
  GColor.Red      = 183;
  GColor.Green    = 97;
  GColor.Blue     = 56;

  EFI_GRAPHICS_OUTPUT_BLT_PIXEL BufferColor;
  BufferColor.Reserved = 124;
  BufferColor.Red      = 123;
  BufferColor.Green    = 197;
  BufferColor.Blue     = 156;
    

  // Get the Graphics Output Protocol
  Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **) &gop);
  if (Status == EFI_SUCCESS) {
    gST->ConOut->Reset(SystemTable->ConOut, 1);
    gST->ConOut->SetAttribute(SystemTable->ConOut, EFI_GREEN);
    gST->ConOut->OutputString(SystemTable->ConOut, L"Hello World");
    gop->Blt(gop, &GColor, EfiBltVideoFill, 0, 0, 50, 50, 100, 200, 0);
    gST->ConOut->SetCursorPosition(SystemTable->ConOut, 10, 4);


    // if no Boot Services available then
    // QueryMode shows available modes, SetMode to choose desired one

    void* BaseAddress = (void*)gop->Mode->FrameBufferBase;
    // UINT64 BufferSize = gop->Mode->FrameBufferSize;
    // UINT32 ScreenWidth = gop->Mode->Info->HorizontalResolution;
    // UINT32 ScreenHeight = gop->Mode->Info->VerticalResolution;
    UINT32 PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

    int xPos = 630;
    int yPos = 130;
    int w = 40;
    int h = 40;

    // draw rectangle (square) using gop's framebuffer 
    UINT32 ByteOffset = 4;
    // if(xPos < 0){xPos = 0;}
    // if(yPos < 0){yPos = 0;}
    // if(w < 1){w = 1;}
    // if(h < 1){h = 1;}
    UINT32 x;
    UINT32 y      = yPos;
    UINT32 width  = (xPos + w) * ByteOffset;
    UINT32 height = yPos + h;

    for(y = yPos; y <= height; y++)
    {
        for(x = xPos * ByteOffset; x <= width; x+=ByteOffset)
        {
          *(UINT32*)(x + (y * PixelsPerScanLine * ByteOffset) + BaseAddress) = *(UINT32*)&BufferColor;
        }
    }

    // draw line
    xPos = 30;
    yPos = 330;
    w = 600;
    y      = yPos;
    width  = (xPos + w) * ByteOffset;

    for(x = xPos * ByteOffset; x <= width; x+=ByteOffset)
    {
        *(UINT32*)(x + (y * PixelsPerScanLine * ByteOffset) + BaseAddress) = *(UINT32*)&BufferColor;
    }

    
    // wait key
    gST->ConIn->Reset(SystemTable->ConIn, 1);

    EFI_INPUT_KEY Key;

    // In a while loop to see if the keyboard has a key stroke in the buffer.
    while((gST->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY);
  }
  else {
    return Status;
  }

  return EFI_SUCCESS;
}
