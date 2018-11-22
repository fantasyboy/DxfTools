#pragma once
#include <Windows.h>
typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;

typedef struct _LDR_MODULE {
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID BaseAddress;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	SHORT LoadCount;
	SHORT TlsIndex;
	LIST_ENTRY HashTableEntry;
	ULONG TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;

typedef struct _PEB_LDR_DATA
{
	ULONG               Length;
	BOOLEAN             Initialized;
	BYTE        reserved[3];
	PVOID               SsHandle;
	LIST_ENTRY          InLoadOrderModuleList;
	LIST_ENTRY          InMemoryOrderModuleList;
	LIST_ENTRY          InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _NT_PEB
{
	BOOLEAN InheritedAddressSpace;
	BOOLEAN ReadImageFileExecOptions;
	BOOLEAN BeingDebugged;
	BOOLEAN Spare;
	HANDLE Mutant;
	PVOID ImageBaseAddress;
	PPEB_LDR_DATA LoaderData;
	PVOID ProcessParameters;//PRTL_USER_PROCESS_PARAMETERS
	PVOID SubSystemData;
	PVOID ProcessHeap;
	PVOID FastPebLock;
	PVOID FastPebLockRoutine;//PPEBLOCKROUTINE
	PVOID FastPebUnlockRoutine;//PPEBLOCKROUTINE
	ULONG EnvironmentUpdateCount;
	PVOID *KernelCallbackTable;
	PVOID EventLogSection;
	PVOID EventLog;
	PVOID FreeList;//PPEB_FREE_BLOCK
	ULONG TlsExpansionCounter;
	PVOID TlsBitmap;
	ULONG TlsBitmapBits[0x2];
	PVOID ReadOnlySharedMemoryBase;
	PVOID ReadOnlySharedMemoryHeap;
	PVOID *ReadOnlyStaticServerData;
	PVOID AnsiCodePageData;
	PVOID OemCodePageData;
	PVOID UnicodeCaseTableData;
	ULONG NumberOfProcessors;
	ULONG NtGlobalFlag;
	BYTE Spare2[0x4];
	LARGE_INTEGER CriticalSectionTimeout;
	ULONG HeapSegmentReserve;
	ULONG HeapSegmentCommit;
	ULONG HeapDeCommitTotalFreeThreshold;
	ULONG HeapDeCommitFreeBlockThreshold;
	ULONG NumberOfHeaps;
	ULONG MaximumNumberOfHeaps;
	PVOID **ProcessHeaps;
	PVOID GdiSharedHandleTable;
	PVOID ProcessStarterHelper;
	PVOID GdiDCAttributeList;
	PVOID LoaderLock;
	ULONG OSMajorVersion;
	ULONG OSMinorVersion;
	ULONG OSBuildNumber;
	ULONG OSPlatformId;
	ULONG ImageSubSystem;
	ULONG ImageSubSystemMajorVersion;
	ULONG ImageSubSystemMinorVersion;
	ULONG GdiHandleBuffer[0x22];
	ULONG PostProcessInitRoutine;
	ULONG TlsExpansionBitmap;
	BYTE TlsExpansionBitmapBits[0x80];
	ULONG SessionId;
}NT_PEB, *pNT_PEB;

typedef struct _NT_TEB
{
	NT_TIB Tib;                         /* 00h */
	PVOID EnvironmentPointer;           /* 1Ch */
										//  CLIENT_ID Cid;                      /* 20h */
	BYTE Reserved1[8];				 //占用8个字节
	PVOID ActiveRpcInfo;                /* 28h */
	PVOID ThreadLocalStoragePointer;    /* 2Ch */
	pNT_PEB Peb;                       /* 30h */
	ULONG LastErrorValue;               /* 34h */
	ULONG CountOfOwnedCriticalSections; /* 38h */
	PVOID CsrClientThread;              /* 3Ch */
	void* Win32ThreadInfo;        /* 40h */
	ULONG Win32ClientInfo[0x1F];        /* 44h */
	PVOID WOW32Reserved;                /* C0h */
	LCID CurrentLocale;                 /* C4h */
	ULONG FpSoftwareStatusRegister;     /* C8h */
	PVOID SystemReserved1[0x36];        /* CCh */
	PVOID Spare1;                       /* 1A4h */
	LONG ExceptionCode;                 /* 1A8h */
	UCHAR SpareBytes1[0x28];            /* 1ACh */
	PVOID SystemReserved2[0xA];         /* 1D4h */
										//  GDI_TEB_BATCH GdiTebBatch;          /* 1FCh */
	BYTE Reserved3[0x4e0];				 //占用8个字节
	ULONG gdiRgn;                       /* 6DCh */
	ULONG gdiPen;                       /* 6E0h */
	ULONG gdiBrush;                     /* 6E4h */
										// CLIENT_ID RealClientId;             /* 6E8h */
	BYTE Reserved2[8];				 //占用8个字节
	PVOID GdiCachedProcessHandle;       /* 6F0h */
	ULONG GdiClientPID;                 /* 6F4h */
	ULONG GdiClientTID;                 /* 6F8h */
	PVOID GdiThreadLocaleInfo;          /* 6FCh */
	PVOID UserReserved[5];              /* 700h */
	PVOID glDispatchTable[0x118];       /* 714h */
	ULONG glReserved1[0x1A];            /* B74h */
	PVOID glReserved2;                  /* BDCh */
	PVOID glSectionInfo;                /* BE0h */
	PVOID glSection;                    /* BE4h */
	PVOID glTable;                      /* BE8h */
	PVOID glCurrentRC;                  /* BECh */
	PVOID glContext;                    /* BF0h */
	LONG LastStatusValue;        /* BF4h */
	UNICODE_STRING StaticUnicodeString; /* BF8h */
	WCHAR StaticUnicodeBuffer[0x105];   /* C00h */
	PVOID DeallocationStack;            /* E0Ch */
	PVOID TlsSlots[0x40];               /* E10h */
	LIST_ENTRY TlsLinks;                /* F10h */
	PVOID Vdm;                          /* F18h */
	PVOID ReservedForNtRpc;             /* F1Ch */
	PVOID DbgSsReserved[0x2];           /* F20h */
	ULONG HardErrorDisabled;            /* F28h */
	PVOID Instrumentation[0x10];        /* F2Ch */
	PVOID WinSockData;                  /* F6Ch */
	ULONG GdiBatchCount;                /* F70h */
	USHORT Spare2;                      /* F74h */
	BOOLEAN IsFiber;                    /* F76h */
	UCHAR Spare3;                       /* F77h */
	ULONG Spare4;                       /* F78h */
	ULONG Spare5;                       /* F7Ch */
	PVOID ReservedForOle;               /* F80h */
	ULONG WaitingOnLoaderLock;          /* F84h */
	ULONG Unknown[11];                  /* F88h */
	PVOID FlsSlots;                     /* FB4h */
	PVOID WineDebugInfo;                /* Needed for WINE DLL's  */
}NT_TEB, *pNT_TEB;

BOOL HideModuleByHandle(HMODULE hModule);




NT_TEB*  GetCurrentThreadTEB()
{
	NT_TEB* pTeb = NULL;

	_asm
	{
		mov    eax, fs:[0x18]	//获取本进程的主线程的TEB
		mov    pTeb, eax
	}
	return pTeb;
}

BOOL HideModuleByHandle(HMODULE hModule)
{
	NT_TEB* pTeb = GetCurrentThreadTEB();
	NT_PEB* pPeb = pTeb->Peb;
	PPEB_LDR_DATA pLdrData = pPeb->LoaderData;
	PLDR_MODULE  ListHead = (PLDR_MODULE)(&(pLdrData->InLoadOrderModuleList));
	PLDR_MODULE pFirstLdrModule = (PLDR_MODULE)pLdrData->InLoadOrderModuleList.Flink;
	PLDR_MODULE pLdrModule = pFirstLdrModule;
	//   PLDR_MODULE pLastModule,pNextModule;

	//LoadOrderModuleList
	while (pLdrModule != ListHead)
	{
		//判断是否是我们要屏蔽的模块
		if (pLdrModule->BaseAddress == hModule)
		{
			pLdrModule->InLoadOrderModuleList.Flink->Blink = pLdrModule->InLoadOrderModuleList.Blink;
			pLdrModule->InLoadOrderModuleList.Blink->Flink = pLdrModule->InLoadOrderModuleList.Flink;

			pLdrModule->InMemoryOrderModuleList.Flink->Blink = pLdrModule->InMemoryOrderModuleList.Blink;
			pLdrModule->InMemoryOrderModuleList.Blink->Flink = pLdrModule->InMemoryOrderModuleList.Flink;

			pLdrModule->InInitializationOrderModuleList.Flink->Blink = pLdrModule->InInitializationOrderModuleList.Blink;
			pLdrModule->InInitializationOrderModuleList.Blink->Flink = pLdrModule->InInitializationOrderModuleList.Flink;
		}

		//移动到链表的下一个
		pLdrModule = (PLDR_MODULE)pLdrModule->InLoadOrderModuleList.Flink;

	}

	return TRUE;
}
void hide_module(HMODULE hMod)
{
	PLIST_ENTRY Head, Cur;
	PPEB_LDR_DATA ldr;
	PLDR_MODULE ldm;
	__asm
	{
		mov eax, fs:[0x30]
		mov ecx, [eax + 0x0c] //Ldr    
		mov ldr, ecx
	}
	Head = &(ldr->InLoadOrderModuleList);
	Cur = Head->Flink;
	do
	{
		ldm = CONTAINING_RECORD(Cur, LDR_MODULE, InLoadOrderModuleList);
		if (hMod == ldm->BaseAddress)
		{
			ldm->InLoadOrderModuleList.Blink->Flink =
				ldm->InLoadOrderModuleList.Flink;
			ldm->InLoadOrderModuleList.Flink->Blink =
				ldm->InLoadOrderModuleList.Blink;
			ldm->InInitializationOrderModuleList.Blink->Flink =
				ldm->InInitializationOrderModuleList.Flink;
			ldm->InInitializationOrderModuleList.Flink->Blink =
				ldm->InInitializationOrderModuleList.Blink;
			ldm->InMemoryOrderModuleList.Blink->Flink =
				ldm->InMemoryOrderModuleList.Flink;
			ldm->InMemoryOrderModuleList.Flink->Blink =
				ldm->InMemoryOrderModuleList.Blink;
			break;
		}
		Cur = Cur->Flink;
	} while (Head != Cur);
}






/*调用方式：
HMODULE h = GetModuleHandle(L"********.dll");   //这里填自己劫持的名字  ****.dll
BOOL CpluginsApp::InitInstance()
{
	CWinApp::InitInstance();
	mainthread();
	HideModuleByHandle(h);     //过木马在这里调用
	return TRUE;
}*/