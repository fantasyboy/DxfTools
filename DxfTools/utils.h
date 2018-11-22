#pragma once


namespace utils
{
	void LogText(const char* _format, ...);
	template<typename T>
	T ReadValue(DWORD address)
	{
		T  value = { 0 };
		__try {
			value = *(T*)(address);
		}
		__except (1) {
			return T();
		}
		return value;
	}

	DWORD FindPattern(DWORD startAddress, DWORD fileSize, unsigned char* pattern,const char mask[]);


	template <typename T>
	bool WriteValue(DWORD address, T Value)
	{
		__try {
			DWORD len = sizeof(T);
			DWORD OldProtect = { 0 };
			VirtualProtectEx(HANDLE(-1), (PVOID)address, len, PAGE_EXECUTE_READWRITE, &OldProtect);
			memcpy((PVOID)address, &Value, len);
			VirtualProtectEx(HANDLE(-1), (PVOID)address, len, OldProtect, &OldProtect);

	/*		SIZE_T re;
			return WriteProcessMemory(HANDLE(-1), (PVOID)address, &Value, sizeof(T), &re);*/


	/*		*(T*)(address) = Value;
			return true;*/
		}
		__except (1) {
			//LogText("HXL: –¥»Îµÿ÷∑ [%p] “Ï≥£", address);
			return false;
		}

		return true;
	}

	bool WriteBytes(DWORD address, char * szData, DWORD len);
};

