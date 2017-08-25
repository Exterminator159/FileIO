#include <ntddk.h>

/*
	Example:
	FileStruct WriteStatus = WriteFile("C:\\test.txt", "WriteFile!", TRUE);
	FileStruct ReadStatus = ReadFile("C:\\test.txt");
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "%s\n", ReadStatus.Value);
*/

typedef struct _FileStruct
{
	NTSTATUS Status;
	PVOID Value;
}FileStruct;

FileStruct WriteFile(const char* Dest, PVOID Source, ULONG size, BOOLEAN Overwrite);

FileStruct ReadFile(const char* Source, ULONG Size);

BOOLEAN FileExists(const char* path);