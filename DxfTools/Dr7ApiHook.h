#pragma once
#include <memory>

class Dr7ApiHook
{
	Dr7ApiHook();
	Dr7ApiHook(const Dr7ApiHook&) = delete;
	void operator= (const Dr7ApiHook&) = delete;

	static std::unique_ptr<Dr7ApiHook> m_pInstance;
public:
	~Dr7ApiHook();
	static Dr7ApiHook* GetInstance();

	bool Init();
	static LONG NTAPI Vector_Exception_hander(PEXCEPTION_POINTERS ExceptionInfo);

	bool AddHookAddress(DWORD oldProc, DWORD newProc);
	bool InstallHook();
	bool UnStallHook();
public:
	DWORD m_oldProcAddr[4];
	DWORD m_newProcAddr[4];
	PVOID m_pVectorHandler;
};

