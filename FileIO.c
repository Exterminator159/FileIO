#include "FileIO.h"

FileStruct WriteFile(const char* Dest, PVOID Source, ULONG Size, BOOLEAN Overwrite)
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
	Status = (Overwrite) ? ZwCreateFile(&File, GENERIC_WRITE, &ObjectAttribs, &IOStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, 0, ((Overwrite) ? FILE_OVERWRITE_IF : FILE_OPEN_IF), FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0) : ZwOpenFile(&File, GENERIC_WRITE, &ObjectAttribs, &IOStatusBlock, 0, FILE_SYNCHRONOUS_IO_NONALERT);
	if (NT_SUCCESS(Status))
	{
		Status = ZwWriteFile(File, NULL, NULL, NULL, &IOStatusBlock, Source, Size, NULL, NULL);
		if (NT_SUCCESS(Status))
			Status = ZwClose(File);
	}
	RtlFreeUnicodeString(&UnicodeFileName);

	FileStruct ReturnStruct;
	ReturnStruct.Status = Status;
	ReturnStruct.Value = Source;
	return ReturnStruct;
}

FileStruct ReadFile(const char* Source, ULONG Size)
{
	HANDLE File;
	OBJECT_ATTRIBUTES ObjectAttribs;
	IO_STATUS_BLOCK IOStatusBlock;
	LARGE_INTEGER ByteOffset = { 0, 0 };

	NTSTATUS Status;
	PVOID ReadValue = NULL;

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
	Status = ZwOpenFile(&File, GENERIC_READ, &ObjectAttribs, &IOStatusBlock, 0, FILE_SYNCHRONOUS_IO_NONALERT);
	if (NT_SUCCESS(Status))
	{
		Status = ZwReadFile(File, NULL, NULL, NULL, &IOStatusBlock, ReadValue, Size, &ByteOffset, NULL);
		if (NT_SUCCESS(Status))
			Status = ZwClose(File);
	}
	RtlFreeUnicodeString(&UnicodeFileName);

	FileStruct ReturnStruct;
	ReturnStruct.Status = Status;
	ReturnStruct.Value = ReadValue;
	return ReturnStruct;
}

BOOLEAN FileExists(const char* path)
{
	BOOLEAN ReturnValue;
	HANDLE File;
	OBJECT_ATTRIBUTES ObjectAttribs;
	IO_STATUS_BLOCK IOStatusBlock;

	NTSTATUS Status;

	UNICODE_STRING UnicodeFileName;
	UNICODE_STRING UnicodePrefix;
	UNICODE_STRING ConcatString;

	RtlInitUnicodeString(&UnicodePrefix, L"\\DosDevices\\");

	ANSI_STRING ANSIPath;
	RtlInitAnsiString(&ANSIPath, path);
	RtlAnsiStringToUnicodeString(&UnicodeFileName, &ANSIPath, TRUE);

	RtlInitUnicodeString(&ConcatString, NULL);
	ConcatString.MaximumLength = UnicodePrefix.Length + UnicodeFileName.Length + sizeof(WCHAR);
	ConcatString.Buffer = (PWCH)ExAllocatePoolWithTag(PagedPool, ConcatString.MaximumLength, 'tcnC');

	RtlCopyUnicodeString(&ConcatString, &UnicodePrefix);
	RtlAppendUnicodeStringToString(&ConcatString, &UnicodeFileName);

	InitializeObjectAttributes(&ObjectAttribs, &ConcatString, OBJ_KERNEL_HANDLE, NULL, NULL);
	Status = ZwOpenFile(&File, GENERIC_ALL, &ObjectAttribs, &IOStatusBlock, 0, FILE_SYNCHRONOUS_IO_NONALERT);

	if (NT_SUCCESS(Status))
	{
		ReturnValue = TRUE;
		ZwClose(File);
	}
	else
		ReturnValue = FALSE;

	RtlFreeUnicodeString(&UnicodeFileName);

	return ReturnValue;
}