#pragma once


#define Base_NoticeCallAddr			0x29DAB50				//公告CALL 
#define Base_NoticeCallECXAddr		0x5655038				//商店基址
#define Base_PlayerObjectAddr		0x5768830				//人物基址

#define Base_TaskTravseAddr			0x05655070				//任务基址
#define Base_TaskTravseStartAddr	0x68					//任务遍历起始地址
#define Base_TaskTravseEndAddr		0x6C					//任务遍历结束地址
#define Base_TaskTypeAddr			0x134					//任务类型
#define Base_TaskGetLevelAddr		0x490					//任务接取等级
#define Base_TaskFinishLevelAddr	0x1C4					//任务完成等级
#define Base_taskMapIdAddr			0x2B8					//任务副本
#define Base_taskSeekAddr			0x2D0					//任务条件
#define Base_TaskMaterialCountAddr  0x2F4					//任务材料  没有更新
#define Base_GameStateAddr			0x0554B34C				//游戏状态

#define Base_SendPacketAddr			0x3745830				//发包CALL
#define Base_AllocatePacketAddr		0x3743FF0				//缓冲CALL
#define Base_EncryptPacketAddr		0x3744100				//加密包CALL
#define Base_SendPacketBaseAddr		0x57DC43C	 			//发包基址

#define Base_TaskFinishCallAddr		0x01ED7C30				//任务完成CALL
#define Base_TaskCommitCallAddr		0x01ED7CB0			    //任务提交CALL
#define Base_TaskAcceptCallAddr		0x01ED7D30				//任务接受CALL

#define Base_MapOffsetAddr			0x0C8					//地图偏移
#define Base_MapStartAddr			0x0c0					//首地址
#define Base_TypeOffsetAddr			0xA4					//类型偏移
#define Base_CampOffsetAddr			0x828					//阵营偏移
#define Base_MonsHealOffsetAddr		0x3B04					//怪物血量偏移
#define Base_EncryptAddr			0x057AA0E0				//解密基址
#define Base_XPointOffsetAddr		0x1D0					//X坐标偏移 （废弃）
#define Base_ObjectSkillCallAddr	0x02B53DC0				//OBJ技能CALL 技能_OBJ
#define Base_SkillCallAddr			0x024EACA0				//技能CALL
#define Base_ObjectSkillPointAddr   0x028641A0				//写入对象
#define Base_PacketTravseAddr		0x0565503C				//背包基址
#define Base_KeybroadAddr			0x06527528				//按键基址
#define Base_ScoreCallAddr			0x0288BFD0				//评分CALL
#define Base_ScoreBaseAddr			0x05653414				//评分基址
#define Base_DecHealthCallAddr		0x026CC910				//扣血CALL

#define Base_AreaCallArgAddr		0x0579D21C				//区域CALL参数
#define Base_AreaCallAddr			0x0367D7B0				//区域CALL
#define Base_EreaPntAddr			0xCBC4					//区域坐标
#define Base_PasswordKeyCallAddr	0x0389F930				//密钥CALL
#define Base_CreateEquimentCallAddr 0x29F7EB0				//创建CALL
#define Base_WearEquimentCallAddr   0x027A9A50				//穿戴call
#define Base_TimeTickAddr			0x020A028				//时间基址
#define Base_BOpenDoorAddr			0x118					//是否开门偏移

#define Base_AStarOffsetAddr1		0x2AC					//宽高偏移
#define Base_AStarOffsetAddr2		0x2C0					//数组偏移
#define Base_AStarOffsetAddr3		0xCC					//起始坐标X
#define Base_AStarOffsetAddr4		0xD0					//起始坐标Y
#define Base_AStarOffsetAddr5		0x19C					//编号偏移
#define Base_DeepBoosRoomXPos		0xC80					//深渊boosX位置
#define Base_DeepBoosRoomYPos		0xC88					//深渊boosY位置
#define Base_CurrentRoomXPos		0xCC					//当前X位置
#define Base_CurrentRoomYPos		0xD0					//当前Y位置
#define Base_BoosRoomXPos			0xC70					//BOOSX位置
#define Base_BoosRoomYPos			0xC78					//BOOSY位置

#define Base_MoveToNextRoomCallAddr 0x01A436C0				//过图CALL
#define Base_SetPosCallAddr			0x02650770				//坐标CALL
#define Base_SetPosCallVTableAddr   0xC8					//坐标CALL虚表

#define Base_MaxPLAddr				0x05768AD8				//最大疲劳
#define Base_CurrentPLAddr			0x05768AD0				//当前疲劳
#define Base_RoleObjectAddr			0x05653AF0				//角色基址
#define Base_MapTravseOffsetAddr    0xB8					//二级偏移  地图难度遍历
#define Base_MapTravseStartAddr     0x3F0					//三级偏移  地图难度遍历首地址
#define Base_MapTravseObjectSize    0x70					//地图遍历对象大小

#define Base_RoleLevelAddr			0x05637904				//角色等级
#define Base_BreakDownCallEcxAgAddr 0x649C					//分解CALL ECX
#define Base_BreakDownCallEcxAddr	0x025CF260				//分解CALL
#define Base_HookBeiGongAddr		0x027ED35C				//自定义伤害
#define Base_IsInTalkAddr			0x057978B4				//对话基址
#define Base_HookBaiziAddr			0x026EC242				//HOOK白字
#define Base_HookGouziAddr			0x027F845D				//HOOK钩子
#define Base_HookRecvAddr			0x25167F0				//收包CALL

#define Base_PntMoveToPosOffset		0x8C					//坐标顺图 顺图偏移
#define Base_PntMoveToPosOffsetAddr	0x1744					//坐标顺图2

#define Base_DirectoryAddr			0x66CA7D4				//目录基址
#define Base_PathCallAddr			0x38BF1E0				//路径CALL
#define Base_FuBenInfoAddr			0x56259DC				//副本信息
#define Base_JiaZaiCallAddr			0x37104A0				//加载CALL
#define Base_FenGeCallAddr			0x379DE40				//分割CALL
#define	Base_PathBaseAddr			0x56259F0				//路径基址