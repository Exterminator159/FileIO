#include <ntddk.h>

typedef struct _FileStruct
{
	NTSTATUS Status;
	PVOID Value;
}FileStruct;

FileStruct WriteFile(const char* Dest, PVOID Source, ULONG size, BOOLEAN Overwrite);

FileStruct ReadFile(const char* Source, ULONG Size);

BOOLEAN FileExists(const char* path);
