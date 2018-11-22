#include "stdafx.h"
#include "TaskItem.h"
#include "baseaddr.h"
#include "utils.h"

TaskItem::TaskItem(DWORD TaskObject) : m_TaskObject(TaskObject)
{
}

TaskItem::~TaskItem()
{
}

DWORD TaskItem::GetTaskId()
{
	return utils::ReadValue<DWORD>(m_TaskObject);
}

DWORD TaskItem::GetTaskType()
{
	return utils::ReadValue<DWORD>(m_TaskObject + Base_TaskTypeAddr);
}

DWORD TaskItem::GetTaskGetLevel()
{
	return utils::ReadValue<DWORD>(m_TaskObject + Base_TaskGetLevelAddr);
}

DWORD TaskItem::GetTaskFinishLevel()
{
	return utils::ReadValue<DWORD>(m_TaskObject + Base_TaskFinishLevelAddr);
}

wchar_t * TaskItem::GetTaskName()
{
	DWORD NameCount =  utils::ReadValue<DWORD>(m_TaskObject + 0x1C);
	if (NameCount > 7) {
		DWORD name = utils::ReadValue<DWORD>(m_TaskObject + 0x8);
		if (name) {
			return (wchar_t*)(name);
		}
	}
	else {
		return (wchar_t*)(m_TaskObject + 0x8);
	}

	return nullptr;
}

DWORD TaskItem::GetMapId()
{
	DWORD obj = utils::ReadValue<DWORD>(m_TaskObject + Base_taskMapIdAddr);
	if (obj) {
		return utils::ReadValue<DWORD>(obj);
	}
	return 0;
}

wchar_t * TaskItem::GetSeek()
{
	DWORD name = utils::ReadValue<DWORD>(m_TaskObject + Base_taskSeekAddr);
	if (name) {
		return (wchar_t*)(name);
	}
	return nullptr;
}

DWORD TaskItem::GetTaskMaterialCount()
{
	return utils::ReadValue<DWORD>(m_TaskObject + Base_TaskMaterialCountAddr);
}
