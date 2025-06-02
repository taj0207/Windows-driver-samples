#include "LoopbackBuffer.h"

LoopbackBuffer g_LoopbackBuffer;

LoopbackBuffer::LoopbackBuffer() : m_Buffer(nullptr), m_Size(0), m_WritePos(0), m_ReadPos(0)
{
    KeInitializeSpinLock(&m_Lock);
}

NTSTATUS LoopbackBuffer::Init(_In_ ULONG Size)
{
    m_Buffer = (BYTE*)ExAllocatePool2(POOL_FLAG_NON_PAGED, Size, 'lbbf');
    if (!m_Buffer)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }
    m_Size = Size;
    m_WritePos = 0;
    m_ReadPos = 0;
    RtlZeroMemory(m_Buffer, Size);
    return STATUS_SUCCESS;
}

void LoopbackBuffer::Write(_In_reads_bytes_(ByteCount) const BYTE* Data, _In_ ULONG ByteCount)
{
    if (!m_Buffer || ByteCount == 0)
        return;

    KIRQL oldIrql;
    KeAcquireSpinLock(&m_Lock, &oldIrql);
    for (ULONG i = 0; i < ByteCount; ++i)
    {
        m_Buffer[m_WritePos] = Data[i];
        m_WritePos = (m_WritePos + 1) % m_Size;
        if (m_WritePos == m_ReadPos)
        {
            m_ReadPos = (m_ReadPos + 1) % m_Size; // overwrite oldest
        }
    }
    KeReleaseSpinLock(&m_Lock, oldIrql);
}

NTSTATUS LoopbackBuffer::Read(_Out_writes_bytes_to_(ByteCount, *BytesRead) BYTE* Data, _In_ ULONG ByteCount, _Out_ ULONG* BytesRead)
{
    if (!m_Buffer || !Data || !BytesRead)
        return STATUS_INVALID_PARAMETER;

    KIRQL oldIrql;
    KeAcquireSpinLock(&m_Lock, &oldIrql);
    ULONG available = (m_WritePos >= m_ReadPos) ? (m_WritePos - m_ReadPos) : (m_Size - m_ReadPos + m_WritePos);
    ULONG toRead = min(ByteCount, available);
    for (ULONG i = 0; i < toRead; ++i)
    {
        Data[i] = m_Buffer[m_ReadPos];
        m_ReadPos = (m_ReadPos + 1) % m_Size;
    }
    KeReleaseSpinLock(&m_Lock, oldIrql);
    *BytesRead = toRead;
    return STATUS_SUCCESS;
}
