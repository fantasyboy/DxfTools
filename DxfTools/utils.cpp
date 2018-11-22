#include "stdafx.h"
#include "utils.h"

void utils::LogText(const char * _format, ...)
{
	std::string temp;
	va_list marker = { 0 };
	va_start(marker, _format);
	size_t num_of_chars = _vscprintf(_format, marker);
	if (num_of_chars > temp.capacity())
	{
		temp.reserve(num_of_chars + 1);
	}
	vsprintf_s(const_cast<char*>(temp.c_str()), num_of_chars + 1, _format, marker);
	va_end(marker);

	OutputDebugStringA(temp.c_str());
}

DWORD utils::FindPattern(DWORD startAddress, DWORD fileSize, unsigned char * pattern,const char mask[])
{
	DWORD pos = 0;
	int searchLen = strlen(mask) - 1;
	//从内存内逐个字节进行比较
	for (DWORD retAddress = startAddress; retAddress < fileSize; retAddress++)
	{
		//判断当前地址是否有效
		if (IsBadReadPtr((const void *)retAddress, 1))
		{
			return 0;
		}


		if (*(PBYTE)retAddress == pattern[pos] || mask[pos] == '?')
		{
			if (mask[pos + 1] == '\0')
			{
				return  (retAddress - searchLen);
			}

			pos++;
		}
		else
		{
			pos = 0;
		}
	}
	return NULL;
}

bool utils::WriteBytes(DWORD address, char * szData, DWORD len)
{
	__try{
		DWORD OldProtect = { 0 };
		VirtualProtectEx(HANDLE(-1),(PVOID)address, len, PAGE_EXECUTE_READWRITE, &OldProtect);
		memcpy((PVOID)address, szData, len);
		VirtualProtectEx(HANDLE(-1), (PVOID)address, len, OldProtect, &OldProtect);

		//SIZE_T re;
		//return WriteProcessMemory(HANDLE(-1), (PVOID)address, (PVOID)szData, len, &re);
	}
	__except (1) {
		return false;
	}
	return true;
}


