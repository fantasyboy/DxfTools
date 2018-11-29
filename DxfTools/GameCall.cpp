#include "stdafx.h"
#include "GameCall.h"
#include "baseaddr.h"
#include "utils.h"




void GameCall::Bulletin(std::wstring text)
{

	if (text.empty()) {
		return;
	}

	std::wstring value = L"[FAT] : ";
	value += text;
	const wchar_t* txtAdr = value.c_str();

	__asm
	{
		PUSH 0;
		PUSH 0;
		PUSH 0;
		PUSH 0;
		PUSH 0;
		PUSH 0x25;
		PUSH 0xFF00;
		PUSH txtAdr;
		MOV ECX, Base_NoticeCallECXAddr;
		MOV ECX, [ECX];
		MOV ECX, [ECX + 0x50];
		MOV EAX, Base_NoticeCallAddr;
		CALL EAX;
	}
}

DWORD GameCall::GetGameState()
{
	return utils::ReadValue<DWORD>(Base_GameStateAddr);
}

DWORD GameCall::GetPlayerLevel()
{
	return utils::ReadValue<DWORD>(Base_RoleLevelAddr);
}

#define Base_SendPacketProxyCallAddr 0x131A3206
void __declspec(naked) SendPacketProxy()
{
	__asm {
		PUSH EBP
		MOV EBP, ESP
		MOV EAX, 40050h
		PUSH EDX;
		MOV EDX, Base_SendPacketProxyCallAddr;
		CALL EDX;
		POP EDX;
		PUSH Base_SendPacketAddr + 0xD;
		RETN
	}
}

void GameCall::SendPacket()
{
	__asm
	{
		MOV ECX, Base_SendPacketBaseAddr;
		MOV ECX, [ECX];
		mov EAX, Base_SendPacketAddr;
		CALL EAX;
	}
}

void GameCall::AllocatePacket(DWORD len)
{
	__asm
	{
		PUSH len;
		MOV ECX, Base_SendPacketBaseAddr;
		MOV ECX, [ECX];
		MOV EAX, Base_AllocatePacketAddr;
		CALL EAX;
	}
}

void GameCall::EncryptPacket(DWORD arg, DWORD len)
{
	DWORD obj = { 0 };

	switch (len)
	{
	case 1:
		obj = Base_EncryptPacketAddr;
		break;
	case 2:
		obj = Base_EncryptPacketAddr + 48;
		break;
	case 4:
		obj = Base_EncryptPacketAddr + 96;
		break;
	case 8:
		obj = Base_EncryptPacketAddr + 144;
		break;
	default:
		break;
	}


	if (obj == 0) {
		return;
	}
	__asm
	{
		PUSH arg;
		MOV ECX, Base_SendPacketBaseAddr;
		MOV ECX, [ECX];
		MOV EAX, obj;
		CALL EAX;
	}
}

//void GameCall::EncryptPacket(DWORD arg, DWORD len)
//{
//	__asm
//	{
//		MOV ESI, [Base_SendPacketBaseAddr];
//		CMP[ESI + 8h], 0;
//		JNZ label;
//		MOV EAX, 80000054;
//		MOV ESP, EBP;
//		POP EBP;
//		RETN 8;
//		label:
//		MOV EAX, len;
//		ADD[ESI + 2BCC2Ch], EAX;
//		PUSH EAX;
//		MOV EAX, arg;
//		LEA EAX, [EAX];
//		PUSH EAX;
//		MOV ECX, ESI;
//		MOV EAX , Base_PasswordKeyCallAddr;
//		CALL EAX;
//		MOV ESP, EBP;
//		POP EBP;
//		RETN 8;
//	}
//}

void GameCall::ReturnToSelectRole()
{
	AllocatePacket(7);
	SendPacket();
}

void GameCall::EnterRoleByIndex(DWORD Index)
{
	AllocatePacket(4);
	EncryptPacket(Index, 4);
	SendPacket();
}

void GameCall::EreaCall(DWORD TaskId)
{
	DWORD Arg = utils::ReadValue<DWORD>(Base_AreaCallArgAddr);
	__asm
	{
		PUSH TaskId;
		PUSH - 1;
		MOV ECX, Arg;
		MOV EAX, Base_AreaCallAddr;
		CALL EAX;
	}
	
	
	DWORD Area = utils::ReadValue<DWORD>(Arg + Base_EreaPntAddr);
	DWORD pos = utils::ReadValue<DWORD>(Arg + Base_EreaPntAddr + 4);
	DWORD x = utils::ReadValue<DWORD>(Arg + Base_EreaPntAddr + 8);
	DWORD y = utils::ReadValue<DWORD>(Arg + Base_EreaPntAddr + 12);

	GameCall::RandomSleep();
	MovToPos(Area, pos, x, y);
}

void GameCall::MovToPos(DWORD Area, DWORD Pos, DWORD x, DWORD y)
{
	AllocatePacket(36);
	EncryptPacket(Area, 1);
	EncryptPacket(Pos, 1);
	EncryptPacket(x, 2);
	EncryptPacket(y, 2);
	EncryptPacket(5, 1);
	EncryptPacket(38, 2);
	EncryptPacket(2, 2);
	EncryptPacket(0, 4);
	EncryptPacket(0, 1);
	SendPacket();
}

void GameCall::SelectMap()
{
	AllocatePacket(15);
	EncryptPacket(0, 4);
	SendPacket();
}

void GameCall::EnterMap(DWORD MapId ,DWORD easy)
{
	AllocatePacket(16);
	EncryptPacket(MapId, 4);
	EncryptPacket(easy, 1);
	EncryptPacket(0, 2);
	EncryptPacket(0, 1);
	EncryptPacket(0, 1);
	EncryptPacket(65535, 2);
	EncryptPacket(0, 4);
	EncryptPacket(0, 1);
	EncryptPacket(0, 4);
	EncryptPacket(0, 1);
	SendPacket();
}

void GameCall::EnterMapForTask(DWORD MapId, DWORD easy, DWORD Taskid)
{
	AllocatePacket(16);
	EncryptPacket(MapId, 4);
	EncryptPacket(0, 1);
	EncryptPacket(0, 2);
	EncryptPacket(0, 1);
	EncryptPacket(0, 1);
	EncryptPacket(0, 4);
	EncryptPacket(Taskid, 4);
	EncryptPacket(0, 1);
	EncryptPacket(0, 4);
	SendPacket();
}

void GameCall::PickUpItem(DWORD ItemId, DWORD xPos, DWORD yPos)
{

	AllocatePacket(43);
	EncryptPacket(ItemId, 4);
	EncryptPacket(0, 1);
	EncryptPacket(1, 1);
	EncryptPacket(xPos, 2);
	EncryptPacket(yPos, 2);
	EncryptPacket(0, 2);
	EncryptPacket(xPos + 7, 2);
	EncryptPacket(yPos + 7, 2);
	EncryptPacket(0, 2);
	EncryptPacket(0, 2);
	SendPacket();
}

int GameCall::Dectypt(int _address)
{
	int eax, esi, edx;
	eax = utils::ReadValue<int>(_address);
	esi = utils::ReadValue<int>(Base_EncryptAddr);
	edx = eax;
	edx >>= 16;
	edx = utils::ReadValue<int>(esi + edx * 4 + 36);
	eax = eax & 0xffff;
	eax = utils::ReadValue<int>(edx + eax * 4 + 8468);
	edx = WORD(eax);
	esi = edx;
	esi <<= 16;
	esi = esi ^ edx;
	edx = utils::ReadValue<int>(_address + 4);
	eax = esi ^ edx;
	return eax;
}

void GameCall::Encrypt(int _address, int value, int type)
{
	int encryptID;
	int deviationParam;
	int deviationAddr;
	int data;
	int lastAddr;
	short ax;
	short si;
	encryptID = utils::ReadValue<int>(_address);
	deviationParam = utils::ReadValue<int>(Base_EncryptAddr);
	deviationParam = utils::ReadValue<int>(deviationParam + (encryptID >> 16) * 4 + 36);
	deviationAddr = deviationParam + (encryptID & 0xFFFF) * 4 + 8468;
	deviationParam = utils::ReadValue<int>(deviationAddr);
	data = deviationParam & 0xFFFF;
	data = data + (data << 16);
	ax = (short)(deviationParam & 0xFFFF);
	si = 0;
	if (type == 0)
	{
		lastAddr = _address & 15;
		switch (lastAddr)
		{
		case 0:
			si = (short)(value >> 16);
			si = (short)(si - ax);
			si = (short)(si + value);
			break;
		case 4:
			si = (short)((value & 0xFFFF) - (value >> 16));;
			break;
		case 8:
			si = (short)(value >> 16);
			si = (short)(si * value);
			break;
		case 12:
			si = (short)(value >> 16);
			si = (short)(si + value);
			si = (short)(si + ax);
			break;
		default:
			break;
		}
	}
	else if (type == 1)
		si = value & 16;
	else if (type == 2)
		si = value;
	else if (type == 3)
	{
		si = value >> 16;
		si += value;
	}
	ax = (short)(si ^ ax);
	data = data ^ value;
	utils::WriteValue((_address + (type != 4 ? 4 : 8)), data);
	utils::WriteValue((deviationAddr + 2), ax);


}

void GameCall::TaskFinish(DWORD TaskiId, DWORD times)
{
	for (DWORD i = 0; i < times; i++)
	{
		__asm
		{
			PUSH - 1;
			PUSH 0;
			PUSH TaskiId;
			MOV EAX, Base_TaskFinishCallAddr;
			CALL EAX;
		}
	}
}

void GameCall::TaskCommit(DWORD TaskId)
{
	__asm
	{
		PUSH 1;
		PUSH - 1;
		PUSH TaskId;
		MOV ECX, Base_TaskTravseAddr;
		MOV EAX, Base_TaskCommitCallAddr;
		CALL EAX;
	}
}

void GameCall::TaskAccept(DWORD TaskId)
{
	__asm
	{
		PUSH TaskId;
		MOV EAX, Base_TaskAcceptCallAddr;
		CALL EAX;
	}
}

void GameCall::ObjectSkillCall(DWORD pointer, DWORD x, DWORD  y, DWORD z, DWORD skId, DWORD hurt)
{
	__asm
	{
		push hurt;
		push skId;
		mov esi, Base_ObjectSkillCallAddr;
		call esi;
		mov esi, eax;
		add esp, 0x8;

		mov edx, 0;
		mov dword ptr[esi + Base_CampOffsetAddr], edx;
		mov edx, dword ptr[esi];
		mov eax, dword ptr[edx + 0xC0];
		push z;
		push y;
		push x;
		mov ecx, esi;
		call eax;

		mov ebx, Base_PlayerObjectAddr;
		mov eax, dword ptr[ebx];
		mov edi, dword ptr[eax + 0xC8];
		push 1;
		push 0;
		push 0;
		push 0;
		push esi;
		push 0;
		push 0xF000;
		push pointer;
		mov ecx, edi;
		mov edx, Base_ObjectSkillPointAddr;
		call edx;
	}
}

void GameCall::SkillCall(DWORD pointer, DWORD x, DWORD y, DWORD z, DWORD skId, DWORD hurt, DWORD size)
{
	int a[26] = {0};
	a[0] = pointer;
	a[2] = skId;
	a[3] = hurt;
	a[6] = x;
	a[7] = y;
	a[8] = z;
	a[23] = 0;
	a[24] = 65535;
	a[25] = 65535;
	int MemBlock = (int)&a;

	__asm
	{
		MOV ECX, MemBlock;
		mov eax, Base_SkillCallAddr;
		CALL EAX;
	}
}

void GameCall::SetSkillHurt(DWORD skId, DWORD skHurt)
{
	if (0 == skHurt) {
		return;
	}

	auto path = GameCall::GetTextPath(skId);
	if (path) {
		auto atk = GetSkAtkPath(path);
		if (path) {
			Encrypt(atk + 32, skHurt);
		}
	}
}



void GameCall::BreakDownEquipment(DWORD equPos)
{
	AllocatePacket(26);
	EncryptPacket(equPos, 2);
	EncryptPacket(0, 1);
	EncryptPacket(65535, 2);
	EncryptPacket(319, 4);
	SendPacket();

}

void GameCall::BreakDownCall(DWORD equPos)
{
	__asm
	{
		MOV ECX, Base_PlayerObjectAddr;
		MOV ECX, [ECX];
		MOV ECX, [ECX + Base_BreakDownCallEcxAgAddr];
		PUSH 319;
		PUSH equPos;
		MOV EAX, Base_BreakDownCallEcxAddr;
		CALL EAX;
	}
}

void GameCall::SellEquipment(DWORD equPos)
{
	//DWORD count = 1;

	//AllocatePacket(593);
	//EncryptPacket(2, 4);
	//EncryptPacket(0, 4);
	//SendPacket();

	//AllocatePacket(22);
	//EncryptPacket(0, 1);
	//EncryptPacket(equPos, 2);
	//EncryptPacket(count, 4);
	//EncryptPacket(1000, 4);
	//EncryptPacket(equPos * 2 + 1 + count * 2, 4);
	//SendPacket();

	AllocatePacket(22);
	EncryptPacket(0, 1);
	EncryptPacket(equPos, 2);
	EncryptPacket(1, 2);
	EncryptPacket(0, 4);
	EncryptPacket(equPos + 1, 4);
	EncryptPacket(0, 4);
	SendPacket();

}

void GameCall::AntiFlop(DWORD x, DWORD y)
{
	AllocatePacket(69);
	SendPacket();
	AllocatePacket(70);
	SendPacket();
	AllocatePacket(71);
	EncryptPacket(x, 1);
	EncryptPacket(y, 1);
	SendPacket();
}

void GameCall::ClearUpPacket()
{
	AllocatePacket(20);
	EncryptPacket(6, 4);
	EncryptPacket(16, 1);
	EncryptPacket(0, 1);
	EncryptPacket(24, 1);
	EncryptPacket(1, 1);
	EncryptPacket(32, 1);
	EncryptPacket(0, 1);
	SendPacket();
}

void GameCall::LeaveMap()
{
	AllocatePacket(42);
	SendPacket();
}

void GameCall::MemKeyBorad(DWORD vkCode, bool BKeyDown)
{
	DWORD keyBorad = utils::ReadValue<DWORD>(Base_KeybroadAddr);
	if (keyBorad) {
		BYTE szKeyDown = { 128 };
		BYTE szKeyUP = { 0 };

		SIZE_T re = { 0 };
		utils::WriteValue(keyBorad + vkCode, szKeyDown);
		Sleep(30);
		utils::WriteValue(keyBorad + vkCode, szKeyUP);
	}
}

void GameCall::MemAttack()
{
	auto player = utils::ReadValue<DWORD>(Base_PlayerObjectAddr);
	if (player) {
		auto obj = utils::ReadValue<DWORD>(player + 0x3db0);  //3db0
		if (obj) {
			utils::WriteValue<BYTE>(obj + 44, 1);
		}
	}
}

void GameCall::MonsDecHealthCall(DWORD player, DWORD object, DWORD health)
{
	__asm
	{
		PUSH 0;
		PUSH 0;
		PUSH 0;
		PUSH 0;
		PUSH 0;
		PUSH 0;
		PUSH health;
		MOV ECX, object; //攻击对象
		MOV EAX, Base_DecHealthCallAddr;
		CALL EAX;
	}
}

void GameCall::SetScore(DWORD score)
{
	__asm {
		PUSH score;
		PUSH 0;
		MOV ECX, Base_ScoreBaseAddr;
		MOV ECX, [ECX];
		MOV EAX, Base_ScoreCallAddr;
		CALL EAX;
	}
}

void GameCall::ReplaceEquipment(DWORD Id, DWORD clss)
{
	__asm
	{
		Push 1
		Push 0
		Push Id
		mov ebx, Base_CreateEquimentCallAddr
		call ebx
		add esp, 0xC
		push clss
		push 1
		push - 1
		push eax
		mov ECX, Base_PlayerObjectAddr
		mov ECX, dword ptr[ECX]
		mov ebx, Base_WearEquimentCallAddr
		call ebx
	}
}

bool GameCall::BOpenDoor()
{
	DWORD temp = utils::ReadValue<DWORD>(Base_PlayerObjectAddr);
	if (temp) {
		DWORD obj =  utils::ReadValue<DWORD>(temp + Base_MapOffsetAddr);
		if (0 == GameCall::Dectypt(obj + Base_BOpenDoorAddr)) {
			return true;
		}
	}

	return false;
}

void GameCall::MoveToNextMap(int direct)
{

	/*
0019ED34   00000002
0019ED38   7D2FEE30
0019ED3C   7D2FD280
0019ED40   7D2FD284
0019ED44   7D2FD284
0019ED48   0019EE4C
0019ED4C   FFFFFFFF
0019ED50   FFFFFFFF
	*/
	__asm
	{
		MOV ECX, Base_NoticeCallECXAddr;
		MOV ECX, [ECX - 0x8];
		MOV ECX, [ECX + Base_TimeTickAddr];
		MOV ECX, [ECX + Base_PntMoveToPosOffset];
		push 0xFF;
		push 0xFF;
		push 0;
		push 0;
		push 0;
		push 0;
		push 0;
		push direct;
		MOV EAX, Base_MoveToNextRoomCallAddr;
		CALL EAX;
	}
}

void GameCall::OnceAgainToMap()
{
	AllocatePacket(72);
	EncryptPacket(1, 1);
	EncryptPacket(0, 1);
	SendPacket();
}

void _declspec(naked) SwitchRet()
{
	__asm {
		mov esp, ebp
		pop ebp
		retn
	}
}


DWORD GameCall::GetCurrentPL()
{
	return Dectypt(Base_MaxPLAddr)  - Dectypt(Base_CurrentPLAddr);
}

DWORD GameCall::GetMaxPL()
{
	return Dectypt(Base_MaxPLAddr);
}

DWORD GameCall::GetTextPath(DWORD Id)
{
	DWORD Value = { 0 };
	__asm
	{
		PUSH Id;
		MOV ECX, Base_DirectoryAddr;
		MOV EAX, Base_PathCallAddr;
		CALL EAX;
		MOV Value, EAX;
	}
	return Value;
}

DWORD GameCall::GetSkAtkPath(DWORD Obj)
{
	DWORD Base = Base_FuBenInfoAddr;

	DWORD Value = { 0 };
	__asm
	{
		MOV ECX, Base_FuBenInfoAddr;
		MOV ECX, [ECX];
		PUSH 1;
		PUSH Obj;
		MOV EAX, Base_JiaZaiCallAddr;
		CALL EAX;
		TEST EAX, EAX;
		JE lable_tag;
		MOV EDI, EAX;
		LEA ESI, [EDI + 0xB4];
		LEA EAX, [EDI + 0xF4];
		PUSH ESI;
		PUSH EAX;
		LEA EDX, Base;
		PUSH EDX;
		MOV[EDX], 0;
		MOV EAX ,  Base_FenGeCallAddr;
		CALL EAX;
		ADD ESP, 0xC;
		MOV EAX, [EAX];
		MOV ECX, Base_PathBaseAddr;
		MOV ECX, [ECX];
		PUSH 1;
		PUSH EAX;
		MOV EAX, Base_JiaZaiCallAddr;
		CALL EAX;
	lable_tag:
		MOV Value, eax;
	}
	return Value;
}

VOID __stdcall GameCall::SwitchEPCaller(PVOID Address, PVOID Callsite, PVOID * OldCallsite)
{

	//VMProtectBegin(__FUNCTION__);
	DWORD pointer = (DWORD)Address + 1;
	DWORD next = pointer + utils::ReadValue<DWORD>(pointer) + 4;
	DWORD er = utils::ReadValue<DWORD>(next + 7);
	DWORD second = utils::ReadValue<DWORD>(er + 0x33);
	DWORD first = utils::ReadValue<DWORD>(er + 3);

	//*(BYTE*)(er) = 0;
	utils::WriteValue<BYTE>(er, 0);

	//DWORD re = first ^ second;

	//if (OldCallsite != NULL) {
	//	*(DWORD*)(OldCallsite) = re;
	//}

	//if (Callsite == NULL) {
	//	Callsite = SwitchRet;
	//}

	//re = (DWORD)Callsite ^ second;
	////char szC3C[] = { 0xC3 };
	//*(DWORD*)(er + 1 + 2) = re;
	//utils::WriteBytes(re, szC3C, 1);

	//utils::WriteValue(er + 1 + 2, re);
	//VMProtectEnd();
}

VOID __stdcall GameCall::SwitchEP(PVOID Address, PVOID Callsite, PVOID * OldCallsite)
{
	//VMProtectBegin(__FUNCTION__);
	DWORD next = (DWORD)Address;
	DWORD er = utils::ReadValue<DWORD>(next + 7);
	DWORD second = utils::ReadValue<DWORD>(er + 0x33);
	DWORD first = utils::ReadValue<DWORD>(er + 3);

	//*(BYTE*)(er) = 0;
	utils::WriteValue<BYTE>(er, 0);
	//DWORD re = first ^ second;

	//if (OldCallsite != NULL) {
	//	*(DWORD*)(OldCallsite) = re;
	//}

	//if (Callsite == NULL) {
		//Callsite = SwitchRet;
	//}

	//re = (DWORD)Callsite ^ second;

	////*(DWORD*)(er + 1 + 2) = re;
	//utils::WriteValue(er + 1 + 2, re);
	//VMProtectEnd();
}

VOID __stdcall GameCall::SwitchEPCallerNew(PVOID Address, PVOID Callsite, PVOID * OldCallsite)
{
	//VMProtectBegin(__FUNCTION__);
	//1. 获取CALL的地址
	DWORD pointer = (DWORD)Address + 1;
	//2. 根据机器码计算CALL地址
	DWORD next = pointer + utils::ReadValue<DWORD>(pointer) + 4;
	//3. 获取到加密的地址的常量
	DWORD er = utils::ReadValue<DWORD>(next + 7);
	DWORD key = utils::ReadValue<DWORD>(er + 0x33);
	//4.修改加密次数为2
	utils::WriteValue<BYTE>(er,2);
	//*(BYTE*)(er) = 2;
	//5.获取到计算第二次经过的地址
	DWORD resAds = (er + 0x1) + (1 + 1 * 4) * 2;
	//6. 在第二次经过的地址上写入自己的CALL
	utils::WriteValue<DWORD>(resAds + 0x2, (DWORD)Callsite ^ key);
	//*(DWORD*)(resAds + 0x2) = (DWORD)Callsite ^ key;
	//7. 对密钥清零

	utils::WriteValue<DWORD>(resAds + 0x6, key);
	//*(DWORD*)(resAds + 0x6) =  key;

	//VMProtectEnd();
}



int GameCall::GetPos(int obejct, int arg)
{
	int pos = { 0 };
	__try {
		__asm
		{
			MOV ECX, obejct
			MOV EAX, [ECX]
			PUSH arg
			CALL[EAX + Base_SetPosCallVTableAddr];
			MOV pos, EAX;
		}
	}__except(1){
		pos = 0;
	}
	return pos;
}

void GameCall::SetPos(int object, int x, int y, int z)
{

	__try {

		if (x == 0 && y == 0)
		{
			return;
		}

		__asm
		{
		/*	MOV ECX, object;
			PUSH z;
			push y;
			push x;
			MOV EAX, Base_SetPosCallAddr;
			CALL EAX;*/

			mov esi, object
			mov edi, [esi]
			push z
			push y
			push x
			mov eax, [edi + 000000BCh]//mov eax, [edi + 000000BCh]
			mov ecx, esi
			mov eax, eax
			call eax
		}
	}__except(1){}
}

bool GameCall::IsPlayerAlive()
{
	auto object = utils::ReadValue<DWORD>(Base_PlayerObjectAddr);
	if (object) {
		return (utils::ReadValue<DWORD>(object + Base_MonsHealOffsetAddr) > 0);
	}

	return false;
}

bool GameCall::IsTask()
{
	return (utils::ReadValue<DWORD>(Base_IsInTalkAddr) == 1);
}

DWORD GameCall::GetCurrentRoleIndex()
{
	auto RoleObj = utils::ReadValue<DWORD>(Base_RoleObjectAddr);
	if (RoleObj) {
		auto obj = utils::ReadValue<DWORD>(RoleObj + 0xB0);
		if (obj) {
			return utils::ReadValue<DWORD>(obj + 0x108);
		}
	}
	return 0;
}

DWORD GameCall::GetAllRoleCount()
{
	auto RoleObj = utils::ReadValue<DWORD>(Base_RoleObjectAddr);
	if (RoleObj) {
		auto obj = utils::ReadValue<DWORD>(RoleObj + 0xB0);
		if (obj) {
			return (utils::ReadValue<DWORD>(obj + 0xA8) - utils::ReadValue<DWORD>(obj + 0xA4)) >> 2;
		}
	}

	return 0;
}

DWORD GameCall::GetUpLevelMapId()
{
	switch (GetPlayerLevel())
	{
	case 1: return 3;
	case 3: return 5;
	case 5: return 6;
	case 8: return 9;
	case 11: return 7;
	case 13: return 8;
	case 16: return 1000;
	case 17: return 10;
	case 18: return 11;
	case 19: return 12;
	case 20: return 13;
	case 21: return 14;
	case 22: return 17;
	case 23: return 15;
	case 24: return 21;
	case 25: return 22;
	case 26: return 23;
	case 27: return 24;
	case 28: return 25;
	case 29: return 26;
	case 30: return 27;
	case 31: return 32;
	case 32: return 150;
	case 33: return 151;
	case 34: return 35;
	case 35: return 34;
	case 36: return 152;
	case 37: return 153;
	case 38: return 154;
	case 39: return 154;
	case 40: return 40;
	case 41: return 41;
	case 42: return 42;
	case 43: return 43;
	case 44: 
	case 45: return 141;
	case 46: return 61;
	case 47: return 50;
	case 48: return 51;
	case 49: return 7146;
	case 50: return 144;
	case 51: return 145;
	case 52: return 146;
	case 53: return 148;
	case 54: return 156;
	case 55: return 157;
	case 56: return 7148;
	case 57: return 158;
	case 58: return 161;
	case 59: return 162;
	case 60: return 163;
	case 61: return 164;
	case 62: return 164;
	case 63: return 80;
	case 64: return 81;
	case 65: return 7150;//82
	case 66: return 88;
	case 67: return 89;
	case 68: return 83;
	case 69: return 84;
	case 70: return 85;
	case 71: return 86;
	case 72: return 87;
	case 73: return 92;
	case 74: return 93;
	case 75: return 70;
	case 76: return 71;
	case 77: return 72;
	case 78: return 74;
	case 79: return 75;
	case 80: return 77;
	case 81: return 101;
	case 82: return 102;
	case 83: return 103;
	case 84: 
	case 85: 
	case 86: 
	case 87: 
	case 88: 
	case 89: 
	case 90: return 104;
	default:
		return 0;
	}

}

void GameCall::RandomSleep(DWORD times)
{

	srand((UINT)time(NULL));

	Sleep(rand() % 200 + times);
}

//
//void 普通攻击()
//{
//	LONG 指针;
//	指针 = *(int*)(人物基址);
//	指针 = *(int*)(指针 + 15760);
//	指针 = (指针 + 44);
//	写整数型(指针, 1);
//}
//void 后台技能A到H(int 位置)
//{
//	LONG 指针;
//	指针 = *(int*)(*(int*)(人物基址)+25632);
//	写字节集(0x0291DFA7, { 144, 144, 144, 144, 144 });
//	写整数型(指针 + 73 + 位置, 1);
//	Sleep(50);
//	写整数型(指针 + 73 + 位置, 0);
//	写字节集(0x0291DFA7, { 198, 68, 55, 86, 0 });
//}
//void 后台技能Q到Y(int 位置)
//{
//	LONG 指针;
//	指针 = *(int*)(*(int*)(人物基址)+25632);
//	写字节集(0x0291DFA7, { 144, 144, 144, 144, 144 });
//	写整数型(指针 + 85 + 位置, 1);
//	Sleep(50);
//	写整数型(指针 + 85 + 位置, 0);
//	写字节集(0x0291DFA7, { 198, 68, 55, 86, 0 });
//}
//void 释放技能(int 参数)
//{
//	if (参数 == 0)
//	{
//		后台技能A到H(1);
//	}
//	else if (参数 == 1)
//	{
//		后台技能A到H(2);
//	}
//	else if (参数 == 2)
//	{
//		后台技能A到H(3);
//	}
//	else if (参数 == 3)
//	{
//		后台技能A到H(4);
//	}
//	else if (参数 == 4)
//	{
//		后台技能A到H(5);
//	}
//	else if (参数 == 5)
//	{
//		后台技能A到H(6);
//	}
//	else if (参数 == 6)
//	{
//		后台技能Q到Y(1);
//	}
//	else if (参数 == 7)
//	{
//		后台技能Q到Y(2);
//	}
//	else if (参数 == 8)
//	{
//		后台技能Q到Y(3);
//	}
//	else if (参数 == 9)
//	{
//		后台技能Q到Y(4);
//	}
//	else if (参数 == 10)
//	{
//		后台技能Q到Y(5);
//	}
//	else if (参数 == 11)
//	{
//		后台技能Q到Y(6);
//	}
//	重置技能();
//}
