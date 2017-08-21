#include "FileIO.h"

FileStruct WriteFile(const char* Dest, char* Source, BOOLEAN Overwrite)
{
	HANDLE File;
	OBJECT_ATTRIBUTES ObjectAttribs;
	
	IO_STATUS_BLOCK IOStatusBlock;

	NTSTATUS Status;

	UNICODE_STRING UnicodeFileName;
	UNICODE_STRING UnicodePrefix;
	UNICODE_STRING ConcatString;

	RtlInitUnicodeString(&UnicodePrefix, L"\\DosDevices\\");

	ANSI_STRING ANSIPath;
	RtlInitAnsiString(&ANSIPath, Dest);
	RtlAnsiStringToUnicodeString(&UnicodeFileName, &ANSIPath, TRUE);

	RtlInitUnicodeString(&ConcatString, NULL);
	ConcatString.MaximumLength = UnicodePrefix.Length + UnicodeFileName.Length + sizeof(WCHAR);
	ConcatString.Buffer = (PWCH)ExAllocatePoolWithTag(PagedPool, ConcatString.MaximumLength, 'tcnC');

	RtlCopyUnicodeString(&ConcatString, &UnicodePrefix);
	RtlAppendUnicodeStringToString(&ConcatString, &UnicodeFileName);

	InitializeObjectAttributes(&ObjectAttribs, &ConcatString, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	Status = ZwCreateFile(&File, GENERIC_WRITE, &ObjectAttribs, &IOStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, 0, ((Overwrite) ? FILE_OVERWRITE_IF : FILE_OPEN_IF), FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (NT_SUCCESS(Status))
	{
		Status = ZwWriteFile(File, NULL, NULL, NULL, &IOStatusBlock, Source, (ULONG)strlen(Source) + 1, NULL, NULL);
	}
	ZwClose(File);
	RtlFreeUnicodeString(&UnicodeFileName);

	FileStruct ReturnStruct;
	ReturnStruct.Status = Status;
	ReturnStruct.Value = ReadValue;
	return ReturnStruct;
}

FileStruct ReadFile(const char* Source)
{
	HANDLE File;
	OBJECT_ATTRIBUTES ObjectAttribs;
	IO_STATUS_BLOCK IOStatusBlock;
	LARGE_INTEGER ByteOffset;
	ByteOffset.LowPart = 0;
	ByteOffset.HighPart = 0;

	NTSTATUS Status;
	char ReadValue[1024] = "";

	UNICODE_STRING UnicodeFileName;
	UNICODE_STRING UnicodePrefix;
	UNICODE_STRING ConcatString;

	RtlInitUnicodeString(&UnicodePrefix, L"\\DosDevices\\");

	ANSI_STRING ANSIPath;
	RtlInitAnsiString(&ANSIPath, Source);
	RtlAnsiStringToUnicodeString(&UnicodeFileName, &ANSIPath, TRUE);

	RtlInitUnicodeString(&ConcatString, NULL);
	ConcatString.MaximumLength = UnicodePrefix.Length + UnicodeFileName.Length + sizeof(WCHAR);
	ConcatString.Buffer = (PWCH)ExAllocatePoolWithTag(PagedPool, ConcatString.MaximumLength, 'tcnC');

	RtlCopyUnicodeString(&ConcatString, &UnicodePrefix);
	RtlAppendUnicodeStringToString(&ConcatString, &UnicodeFileName);

	InitializeObjectAttributes(&ObjectAttribs, &ConcatString, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	Status = ZwCreateFile(&File, GENERIC_READ, &ObjectAttribs, &IOStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (NT_SUCCESS(Status))
	{
		Status = ZwReadFile(File, NULL, NULL, NULL, &IOStatusBlock, ReadValue, 1024, &ByteOffset, NULL);
	}
	ZwClose(File);
	RtlFreeUnicodeString(&UnicodeFileName);

	FileStruct ReturnStruct;
	ReturnStruct.Status = Status;
	ReturnStruct.Value = ReadValue;
	return ReturnStruct;
}
