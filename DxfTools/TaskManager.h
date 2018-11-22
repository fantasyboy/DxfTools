#pragma once
#include "TaskItem.h"
#include <vector>
#include <memory>
class TaskManager
{
	TaskManager();
	TaskManager(const TaskManager&) = delete;
	void operator= (const TaskManager&) = delete;

	static std::unique_ptr<TaskManager> m_pInstance;
public:
	~TaskManager();
	static TaskManager* GetInstance();
	DWORD GetTaskItem();

	void AutoTask();
	void DelCommitTask();
	DWORD GetNTimes();
	bool IsSameTask(DWORD TaskId);
private:
	std::vector<DWORD> m_TaskList;
};

