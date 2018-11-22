//PELoader函数借鉴linxer仿真PE加载器代码

//coded　by 燕十二

#include "stdafx.h"
#include <stdio.h>
BOOL LoadDll2Mem(PVOID &pAllocMem,DWORD &dwMemSize,char* strFileName)  
{  
	HANDLE hFile = CreateFileA(strFileName, GENERIC_READ,  
		FILE_SHARE_READ, NULL,  
		OPEN_EXISTING,  
		FILE_ATTRIBUTE_NORMAL, NULL);  
	if (INVALID_HANDLE_VALUE == hFile) {  		
		return FALSE;  
	}  

	PVOID pFileBuff = NULL;

	int nFileSize = GetFileSize(hFile, NULL);  
	
	if (nFileSize == 0)
	{
		return FALSE;
	}
	else
	{
		pFileBuff = VirtualAlloc(NULL,nFileSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	}
	DWORD dwReadSize = 0;
	if (!ReadFile(hFile, pFileBuff, nFileSize, &dwReadSize, NULL)) 
	{  
		return FALSE; 
	}

	PVOID pBase = pFileBuff;

	//判断是否是PE
	PIMAGE_DOS_HEADER pIDH = (PIMAGE_DOS_HEADER)pFileBuff;	
	if (IMAGE_DOS_SIGNATURE != pIDH->e_magic) 
	{  
		return FALSE;  
	}  

	PIMAGE_NT_HEADERS pINH = (PIMAGE_NT_HEADERS)((ULONG)pFileBuff + pIDH->e_lfanew);

	if (IMAGE_NT_SIGNATURE != pINH->Signature) 
	{  
		return FALSE;  
	}  

	dwMemSize = nFileSize;
	

	pAllocMem = pFileBuff;
  
    return TRUE;  
}  

BOOL PELoader(char *lpStaticPEBuff, PVOID& pExecuMem)
{
  long lPESignOffset = *(long *)(lpStaticPEBuff + 0x3c);
  IMAGE_NT_HEADERS *pINH = (IMAGE_NT_HEADERS *)(lpStaticPEBuff + lPESignOffset);

  long lImageSize = pINH->OptionalHeader.SizeOfImage;
  char *lpDynPEBuff = (char *)VirtualAlloc(NULL,lImageSize, MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);
  if(lpDynPEBuff == NULL)
  {
    return FALSE;
  }
  memset(lpDynPEBuff, 0, lImageSize);

  long lSectionNum = pINH->FileHeader.NumberOfSections;

  IMAGE_SECTION_HEADER *pISH = (IMAGE_SECTION_HEADER *)((char *)pINH + sizeof(IMAGE_NT_HEADERS));
  
  memcpy(lpDynPEBuff, lpStaticPEBuff, pISH->VirtualAddress);

  long lFileAlignMask = pINH->OptionalHeader.FileAlignment - 1;        
  long lSectionAlignMask = pINH->OptionalHeader.SectionAlignment - 1;  
  
  for(int nIndex = 0; nIndex < lSectionNum; nIndex++, pISH++)
  {
    memcpy(lpDynPEBuff + pISH->VirtualAddress, lpStaticPEBuff + pISH->PointerToRawData, pISH->SizeOfRawData);
  }

  if(pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size > 0)
  {
    IMAGE_IMPORT_DESCRIPTOR *pIID = (IMAGE_IMPORT_DESCRIPTOR *)(lpDynPEBuff + \
      pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    for(; pIID->Name != NULL; pIID++)
    {
      IMAGE_THUNK_DATA *pITD = (IMAGE_THUNK_DATA *)(lpDynPEBuff + pIID->FirstThunk);

	  char* pLoadName = lpDynPEBuff + pIID->Name;
      HINSTANCE hInstance = LoadLibraryA(pLoadName);
      if(hInstance == NULL)
      {

		VirtualFree(lpDynPEBuff,lImageSize,MEM_DECOMMIT);
        return FALSE;
      }

      for(; pITD->u1.Ordinal != 0; pITD++)
      {
        FARPROC fpFun;
        if(pITD->u1.Ordinal & IMAGE_ORDINAL_FLAG32)
        {
          fpFun = GetProcAddress(hInstance, (LPCSTR)(pITD->u1.Ordinal & 0x0000ffff));
        }
        else
        {
          IMAGE_IMPORT_BY_NAME * pIIBN = (IMAGE_IMPORT_BY_NAME *)(lpDynPEBuff + pITD->u1.AddressOfData);
          fpFun = GetProcAddress(hInstance, (LPCSTR)pIIBN->Name);
        }

        if(fpFun == NULL)
        {
          delete lpDynPEBuff;
          return false;
        }

        pITD->u1.Ordinal = (long)fpFun;
      }
    }
  }



  if(pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size > 0)
  {
    IMAGE_BASE_RELOCATION *pIBR = (IMAGE_BASE_RELOCATION *)(lpDynPEBuff + \
      pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

    long lDifference = (long)lpDynPEBuff - pINH->OptionalHeader.ImageBase;

    for(; pIBR->VirtualAddress != 0; )
    {
      char *lpMemPage = lpDynPEBuff + pIBR->VirtualAddress;
      long lCount = (pIBR->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) >> 1;

      short int *pRelocationItem = (short int *)((char *)pIBR + sizeof(IMAGE_BASE_RELOCATION));
      for(int nIndex = 0; nIndex < lCount; nIndex++)
      {
        int nOffset = pRelocationItem[nIndex] & 0x0fff;
        int nType = pRelocationItem[nIndex] >> 12;

        if(nType == IMAGE_REL_BASED_HIGHLOW)
        {
          *(long *)(lpMemPage + nOffset) += lDifference;
        }
        else if(nType == IMAGE_REL_BASED_DIR64)
        {
			*(ULONG_PTR *)(lpMemPage + nOffset) += (ULONG_PTR)lDifference;
		}
		else if (nType == IMAGE_REL_BASED_HIGH) 
		{
			*(WORD *)(lpMemPage + nOffset) += HIWORD(lDifference);
		}
		else if (nType == IMAGE_REL_BASED_LOW)
			*(WORD *)(lpMemPage + nOffset) += LOWORD(lDifference);

      }

      pIBR = (IMAGE_BASE_RELOCATION *)(pRelocationItem + lCount);
    }

  }

  //FlushInstructionCache((HANDLE)-1, NULL, 0);
  pExecuMem = lpDynPEBuff;

  return true;
} 


typedef BOOL  (WINAPI *DLL_MAIN)( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved);

BOOL CallDllMain(PVOID pExecMem,DWORD dwReaseon,char* pModuleName)
{
	PIMAGE_NT_HEADERS pINH = (PIMAGE_NT_HEADERS)((ULONG)pExecMem + ((PIMAGE_DOS_HEADER)pExecMem)->e_lfanew);
	DWORD dwEP = pINH->OptionalHeader.AddressOfEntryPoint;
	DLL_MAIN lpDllMain = (DLL_MAIN)((DWORD)pExecMem + dwEP);
	lpDllMain((HMODULE)pExecMem,dwReaseon,pModuleName);
	ZeroMemory(pExecMem, pINH->OptionalHeader.SizeOfHeaders);
	return TRUE;
}

BOOL LaunchDll(char *strName,DWORD dwReason)
{
	PVOID pRelocMem = NULL;
	PVOID pExecuMem = NULL;
	DWORD dwMemSize = 0;
	if (LoadDll2Mem(pRelocMem,dwMemSize,strName))
	{
		PELoader((char *)pRelocMem,pExecuMem);
		CallDllMain(pExecuMem,dwReason,strName);
		ZeroMemory(pRelocMem,dwMemSize);
		VirtualFree(pRelocMem,dwMemSize,MEM_DECOMMIT);
	}
	return TRUE;
}
