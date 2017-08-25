#include <Ntifs.h>
#include <ntddk.h>
#include <Ntstrsafe.h>
#pragma comment(lib, "Ntstrsafe.lib")

#include "FileIO.h"

DRIVER_UNLOAD Unload;
DRIVER_INITIALIZE DriverEntry;

VOID Unload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "SampleDriver: Unload\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "SampleDriver: DriverEntry\n");
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Name of driver: %wZ\n", DriverObject->DriverName);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Registry path: %wZ\n", RegistryPath);

	DriverObject->DriverUnload = Unload;
	
	if(!FileExists("C:\\example.txt")
	{
		char* text = "WriteFile!";
		FileStruct WriteStatus = WriteFile("C:\\example.txt", (void*)text, (ULONG)strlen(text)+1, FALSE);
		if (NT_SUCCESS(WriteStatus.Status))
		{
			FileStruct ReadStatus = ReadFile("C:\\example.txt", 1024);
			if (NT_SUCCESS(ReadStatus.Status))
				DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "%s\n", ReadStatus.Value);
			else
				DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Error reading from file! Error code: %X\n", ReadStatus.Status);
		}
		else
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Error writing to file! Error code: %X\n", WriteStatus.Status);
	}
	else
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "File already exists!\n");
	
	return STATUS_SUCCESS;
}
