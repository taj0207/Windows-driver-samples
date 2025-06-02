#pragma once
#include <ntddk.h>

class LoopbackBuffer
{
public:
    LoopbackBuffer();
    NTSTATUS Init(_In_ ULONG Size);
    void Write(_In_reads_bytes_(ByteCount) const BYTE* Data, _In_ ULONG ByteCount);
    NTSTATUS Read(_Out_writes_bytes_to_(ByteCount, *BytesRead) BYTE* Data, _In_ ULONG ByteCount, _Out_ ULONG* BytesRead);
private:
    BYTE*       m_Buffer;
    ULONG       m_Size;
    ULONG       m_WritePos;
    ULONG       m_ReadPos;
    KSPIN_LOCK  m_Lock;
};

extern LoopbackBuffer g_LoopbackBuffer;
