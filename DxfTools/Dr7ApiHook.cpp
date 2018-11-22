#include "stdafx.h"
#include "Dr7ApiHook.h"

std::unique_ptr<Dr7ApiHook> Dr7ApiHook::m_pInstance(new Dr7ApiHook());
Dr7ApiHook::Dr7ApiHook()
{
	memset(m_oldProcAddr, 0, sizeof(DWORD) * 4);
}


Dr7ApiHook::~Dr7ApiHook()
{
}

Dr7ApiHook * Dr7ApiHook::GetInstance()
{
	return m_pInstance.get();
}

bool Dr7ApiHook::Init()
{
	m_pVectorHandler = AddVectoredExceptionHandler(1, PVECTORED_EXCEPTION_HANDLER(Vector_Exception_hander));
	if (!m_pVectorHandler) {
		return false;
	}
	return true;
}

LONG Dr7ApiHook::Vector_Exception_hander(PEXCEPTION_POINTERS ExceptionInfo)
{

	//”≤∂ŒHOOKµÿ÷∑
	for (int i = 0; i < 4; i++)
	{
		if ((DWORD)ExceptionInfo->ExceptionRecord->ExceptionAddress == Dr7ApiHook::GetInstance()->m_oldProcAddr[i + 1])
		{
			ExceptionInfo->ContextRecord->Eip = Dr7ApiHook::GetInstance()->m_newProcAddr[i + 1];
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

bool Dr7ApiHook::AddHookAddress(DWORD oldProc, DWORD newProc)
{
	for (int i = 0; i < 4; i++)
	{
		if (0 == m_oldProcAddr[i + 1]) {
			m_oldProcAddr[i + 1] = oldProc;
			m_newProcAddr[i + 1] = newProc;
			return true;
		}
	}

	return false;
}

bool Dr7ApiHook::InstallHook()
{
	CONTEXT context = { 0 };
	context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	if (!GetThreadContext(GetCurrentThread(), &context)) {
		return false;
	}

	context.Dr0 = m_oldProcAddr[1];
	context.Dr1 = m_oldProcAddr[2];
	context.Dr2 = m_oldProcAddr[3];
	context.Dr3 = m_oldProcAddr[4];
	context.Dr7 = 0x405;
	if (!SetThreadContext(GetCurrentThread(), &context)) {
		return false;
	}

	return true;
}

bool Dr7ApiHook::UnStallHook()
{
	CONTEXT context = { 0 };
	context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	if (!GetThreadContext(GetCurrentThread(), &context)) {
		return false;
	}
	context.Dr0 = 0;
	context.Dr1 = 0;
	context.Dr2 = 0;
	context.Dr3 = 0;
	context.Dr7 = 0x405;

	if (!SetThreadContext(GetCurrentThread(), &context)) {
		return false;
	}

	return true;
}
