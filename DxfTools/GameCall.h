#pragma once
namespace GameCall
{
	void Bulletin(std::wstring text); //喇叭公告
	DWORD GetGameState(); //获取游戏状态

	DWORD GetPlayerLevel();

	void SendPacket(); //发包
	void AllocatePacket(DWORD len); //缓冲CALL
	void EncryptPacket(DWORD arg, DWORD len); //加密CALL
	//void PasKeyCall(DWORD arg, DWORD len); //密钥CALL
	void ReturnToSelectRole();
	void EnterRoleByIndex(DWORD Index); //进入角色
	void EreaCall(DWORD TaskId); //区域CALL
	void MovToPos(DWORD Area, DWORD Pos, DWORD x, DWORD y); //组包移动
	void SelectMap(); //组包选图
	void EnterMap(DWORD MapId , DWORD easy); //组包进图
	void EnterMapForTask(DWORD MapId, DWORD easy, DWORD Taskid); //进入任务地图
	void PickUpItem(DWORD ItemId , DWORD xPos, DWORD yPos); //组包拾取
	int  Dectypt(int _address); //解密
	void Encrypt(int _address, int value, int type = 0); //加密
	void TaskFinish(DWORD TaskiId, DWORD times = 1);  //任务完成
	void TaskCommit(DWORD TaskId);   //任务提交
	void TaskAccept(DWORD TaskId); //接受任务
	void ObjectSkillCall(DWORD pointer , DWORD x, DWORD  y, DWORD z,DWORD skId, DWORD hurt); //OBJ技能CALL
	void SkillCall(DWORD pointer, DWORD x, DWORD  y, DWORD z, DWORD skId, DWORD hurt , DWORD size = 3); //技能CALL
	void BreakDownEquipment(DWORD equPos); //组包分解
	void BreakDownCall(DWORD equPos); //分解CALL
	void SellEquipment(DWORD equPos); //组包卖物
	void AntiFlop(DWORD x, DWORD y);//组包翻牌
	void ClearUpPacket(); //整理背包
	void LeaveMap(); //组包出图
	void MemKeyBorad(DWORD vkCode,bool BKeyDown); //内存按键
	void MemAttack(); //普通攻击
	void MonsDecHealthCall(DWORD player ,DWORD object, DWORD health); //扣血CALL
	void SetScore(DWORD score); //评分CALL
	void ReplaceEquipment(DWORD Id, DWORD clss); //换装CALL
	bool BOpenDoor(); //是否开门
	void MoveToNextMap(int direct); //过图CALL
	void OnceAgainToMap(); //再次进图
	int  GetPos(int obejct, int arg); //获取坐标
	void SetPos(int object, int x, int y, int z); //设置坐标
	bool IsPlayerAlive(); //玩家是否活着
	bool IsTask();
	DWORD GetCurrentRoleIndex(); //获取当前角色索引
	DWORD GetAllRoleCount(); //获取角色数量
	DWORD GetUpLevelMapId();  //获取适合等级的地图
	void  RandomSleep(DWORD times = 500); //随机延时
	DWORD GetCurrentPL(); //获取当前疲劳
	DWORD GetMaxPL(); //获取最大疲劳
	DWORD GetTextPath(DWORD Id); //获取文本路径
	DWORD GetSkAtkPath(DWORD Obj); //获取技能Atk目录
	void  SetSkillHurt(DWORD skId, DWORD skHurt); //设置技能伤害

	VOID __stdcall SwitchEPCaller(__in PVOID Address,
		__in PVOID Callsite,
		__in PVOID * OldCallsite);

	VOID __stdcall SwitchEP(__in PVOID Address,
		__in PVOID Callsite,
		__in PVOID * OldCallsite);

	VOID __stdcall SwitchEPCallerNew(__in PVOID Address,
		__in PVOID Callsite,
		__in PVOID * OldCallsite);
};


//DWORD 取角色位置索引()
//{
//	return READOFFSET(角色基址, 偏移_角色信息, 0x108);
//}
//DWORD 取角色总数量()
//{
//	return (READOFFSET(角色基址, 偏移_角色信息, 0xA8) -
//		READOFFSET(角色基址, 偏移_角色信息, 0xA4)) / 4;
//}