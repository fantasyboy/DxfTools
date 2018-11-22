#pragma once
class TaskItem
{
public:
	TaskItem(DWORD TaskObject);
	~TaskItem();
	DWORD GetTaskId();
	DWORD GetTaskType();
	DWORD GetTaskGetLevel();
	DWORD GetTaskFinishLevel();
	wchar_t* GetTaskName();
	DWORD GetMapId();
	wchar_t* GetSeek(); //获取类型

	DWORD GetTaskMaterialCount();
private:
	DWORD m_TaskObject;
};

