#pragma once

#include <memory>
#include <Windows.h>
class GameManager
{
	GameManager();
	GameManager(const GameManager&) = delete;
	void operator= (const GameManager&) = delete;

	static std::unique_ptr<GameManager> m_pInstance;
public:
	~GameManager();
	static GameManager* GetInstance();

	bool Init();
	static DWORD WINAPI MainThreadProc(LPVOID lpParameter);
	bool BExitGame();
private:
	HANDLE m_hMainThread;
	bool m_bExitMainThread;


	
};

void DelAllPacketItem();
void GetItemFromGround();
int GetGoodItemFromGround(bool bMove);
void SearchDoorPosAndMove(int next, bool bFis);

ULONG NTAPI ScanTenRPCS(__in PVOID Parameter);
void HookQuanPing();
void HookRecv();
void HookDuoDuan();
void HookFenLie();
void HookDecHealth();
void HookBiegongProxy();
 DWORD GetMapCurDiff(DWORD MapId);


 bool HideThread(HANDLE hThread);
