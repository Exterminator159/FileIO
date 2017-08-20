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
	FileStruct WriteStatus = WriteFile("C:\\example.txt", "WriteFile!", TRUE);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "%s\n", WriteStatus.Value);

	FileStruct ReadStatus = ReadFile("C:\\example.txt");
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "%s\n", ReadStatus.Value);

	return STATUS_SUCCESS;
}