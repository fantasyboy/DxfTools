#pragma once
#include "stdafx.h"
#include <winternl.h>

#define NTSTATUS  ULONG

extern "C"
{

	typedef LONG(WINAPI *TNtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesReaded);
	typedef LONG(WINAPI *TNtWriteVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG  NumberOfBytesWritten);
	typedef LONG(WINAPI *TNtProtectVirtualMemory)(__in HANDLE ProcessHandle,__inout PVOID *BaseAddress,__inout PSIZE_T RegionSize,__in ULONG NewProtectWin32,__out PULONG OldProtect);

	NTSYSCALLAPI
		NTSTATUS
		NTAPI
		LdrFindEntryForAddress(
			__in PVOID Address,
			__out PLDR_DATA_TABLE_ENTRY * SectionHandle
		);


	typedef struct _RDE_LDR_DATA_TABLE_ENTRY {
		LIST_ENTRY InLoadOrderLinks;
		LIST_ENTRY InMemoryOrderLinks;
		LIST_ENTRY InInitializationOrderLinks;
		PVOID DllBase;
		PVOID EntryPoint;
		ULONG SizeOfImage;
		UNICODE_STRING FullDllName;
		UNICODE_STRING BaseDllName;
		ULONG Flags;
		USHORT LoadCount;
		USHORT TlsIndex;

		union {
			LIST_ENTRY HashLinks;
			struct {
				PVOID SectionPointer;
				ULONG CheckSum;
			}s1;
		}u1;

		union {
			struct {
				ULONG TimeDateStamp;
			}s1;

			struct {
				PVOID LoadedImports;
			}s2;
		}u2;

	} RDE_LDR_DATA_TABLE_ENTRY, *PRDE_LDR_DATA_TABLE_ENTRY;


	typedef CLIENT_ID *PCLIENT_ID;

	typedef
		NTSTATUS
		(NTAPI * PSE_THREAD_START_ROUTINE)(
			__in PVOID ThreadParameter
			);


	NTSYSCALLAPI
		NTSTATUS
		NTAPI
		RtlCreateUserThread(
			__in HANDLE Process,
			__in PSECURITY_DESCRIPTOR ThreadSecurityDescriptor,
			__in BOOLEAN CreateSuspended,
			__in ULONG StackZeroBits,
			__in_opt SIZE_T MaximumStackSize,
			__in_opt SIZE_T InitialStackSize,
			__in PSE_THREAD_START_ROUTINE StartAddress,
			__in PVOID Parameter,
			__out PHANDLE Thread,
			__out CLIENT_ID* ClientId
		);
}