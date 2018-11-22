#include "stdafx.h"
#include "GetDirection.h"
#include "utils.h"
#include "baseaddr.h"
#include "GameCall.h"

GetDirection::GetDirection()
{
}


GetDirection::~GetDirection()
{
}

void GetDirection::Get_Direction(std::vector<std::vector<MAP_CROSS>> mapCross,POINT_3D_DWORD start, POINT_3D_DWORD end, int width, int height, std::list<POINT_3D_DWORD>& path)
{
	std::vector<A_STAR_NODE> openlist, closelist;


	mapCross[start.x][start.y].color = 0xffff;
	mapCross[end.x][end.y].color = 0xffff;

	A_STAR_NODE temp = { 0 };
	temp.currpnt = { start.x , start.y };

	openlist.push_back(temp);
	do
	{
		//遍历开放列表，取出最小F的节点地址
		DWORD MinFValue = { 0 };
		DWORD MinNumber = { 0 };
		for (auto i = 0; i != openlist.size(); i++)
		{
			if (0 == MinFValue) {
				MinFValue = openlist[i].F;
				MinNumber = i;
			}

			if (openlist[i].F < MinFValue) {
				MinFValue = openlist[i].F;
				MinNumber = i;
			}
		}

		//取出临时节点 放出关闭列表中 并从开放列表中删除
		auto TempNode = openlist.at(MinNumber);
		openlist.erase(openlist.begin() + MinNumber);
		closelist.push_back(TempNode);

		//获取路径
		for (auto y = 0; y != closelist.size(); y++)
		{
			if (closelist[y].currpnt.x == end.x && closelist[y].currpnt.y == end.y)
			{
				auto DectedNode = closelist.at(y);
				do
				{
					for (auto x = 0; x != closelist.size(); x++)
					{
						//获取当前节点的父节点
						if (closelist[x].currpnt.x == DectedNode.parentpnt.x && closelist[x].currpnt.y == DectedNode.parentpnt.y)
						{
							DectedNode = closelist[x];
							break;
						}
					}

					if (DectedNode.currpnt.x != start.x || DectedNode.currpnt.y != start.y)
					{
						mapCross[DectedNode.currpnt.x][DectedNode.currpnt.y].color = 0x00D8D8;  //嫩黄
						path.insert(path.begin(), DectedNode.currpnt);
					}

				} while (DectedNode.currpnt.x != start.x || DectedNode.currpnt.y != start.y);

				path.push_front( start);
				path.push_back(end);

				return;
			}

		}

		//取和当前坐标相邻的4个点，插入开发列表中
		for (auto i = 0; i < 4; i++)
		{
			A_STAR_NODE DectedNode = { 0 };
			if (0 == i) {
				DectedNode.currpnt.x = TempNode.currpnt.x;
				DectedNode.currpnt.y = TempNode.currpnt.y - 1;
			}
			else if (1 == i) {
				DectedNode.currpnt.x = TempNode.currpnt.x - 1;
				DectedNode.currpnt.y = TempNode.currpnt.y;
			}
			else if (2 == i) {
				DectedNode.currpnt.x = TempNode.currpnt.x + 1;
				DectedNode.currpnt.y = TempNode.currpnt.y;
			}
			else {
				DectedNode.currpnt.x = TempNode.currpnt.x;
				DectedNode.currpnt.y = TempNode.currpnt.y + 1;
			}

			//判断是否超出了地图
			if (DectedNode.currpnt.x <= 0 || DectedNode.currpnt.y <= 0 || DectedNode.currpnt.x >(width - 1) || DectedNode.currpnt.y >(height - 1)) {
				continue;
			}

			//判断是否有地图 ，没有就直接跳过
			if (mapCross[DectedNode.currpnt.x][DectedNode.currpnt.y].color == 0)
			{
				continue;
			}


			//判断是否在关闭列表中
			bool bInCloseList = false;
			for (auto i = 0; i != closelist.size(); i++)
			{
				if (closelist[i].currpnt.x == DectedNode.currpnt.x && closelist[i].currpnt.y == DectedNode.currpnt.y)
				{
					bInCloseList = true;
					break;
				}
			}


			if (bInCloseList) {
				continue;
			}

			//判断是否在开放列表中
			bool bInOpenList = false;
			for (auto i = 0; i != openlist.size(); i++) 
			{

				if (openlist[i].currpnt.x == DectedNode.currpnt.x && openlist[i].currpnt.y == DectedNode.currpnt.y)
				{
					DWORD TempGValue = 0;
					if (openlist[i].currpnt.x != TempNode.currpnt.x || openlist[i].currpnt.y != TempNode.currpnt.y){
						TempGValue = 14;
					}
					else {
						TempGValue = 10;
					}

					if ((TempNode.G + TempGValue) < openlist[i].G)
					{
						openlist[i].parentpnt = TempNode.currpnt;
					}

					bInOpenList = true;
					break;
				}
			}


			//没在开放列表中，则插入新的节点到开放列表中
			if (!bInOpenList)
			{
				DWORD TempGValue = 0;
				if (DectedNode.currpnt.x == TempNode.currpnt.x || DectedNode.currpnt.y == TempNode.currpnt.y)
				{
					TempGValue = 10;
				}
				else {
					TempGValue = 14;
				}

				DectedNode.G = TempNode.G + TempGValue;
				DectedNode.H = abs(end.x - DectedNode.currpnt.x) * 10 + abs(end.y - DectedNode.currpnt.y) * 10;
				DectedNode.F = DectedNode.G + DectedNode.H;
				DectedNode.parentpnt = TempNode.currpnt;

				openlist.insert(openlist.begin(), DectedNode);
			}
		}


	} while (openlist.size() != 0);
}

MAP_DATA GetDirection::GetData(bool bDeep)
{
	MAP_DATA map = { 0 };

	DWORD base = utils::ReadValue<DWORD>(Base_NoticeCallECXAddr - 0x8);
	if (0 == base) {
		return MAP_DATA();
	}

	DWORD base2 = utils::ReadValue<DWORD>(base + Base_TimeTickAddr);
	if (0 == base2) {
		return MAP_DATA();
	}

	DWORD base3 = utils::ReadValue<DWORD>(base2 + Base_MapOffsetAddr);
	if (0 == base3) {
		return MAP_DATA();
	}

	DWORD map_num = GameCall::Dectypt(base3 + Base_BoosRoomXPos - 24) + 1;
	DWORD basewidthheigth = utils::ReadValue<DWORD>(base3 + Base_AStarOffsetAddr1);

	map.width = utils::ReadValue<DWORD>(basewidthheigth + map_num * 8 - 8);
	map.height = utils::ReadValue<DWORD>(basewidthheigth + map_num * 8 - 4);

	DWORD temp = utils::ReadValue<DWORD>(base3 + Base_AStarOffsetAddr2);
	DWORD aray = utils::ReadValue<DWORD>(temp + map_num * 20 - 16);

	DWORD mapCount = map.width * map.height;
	for (int i = 0; i != mapCount; i++)
	{
		DWORD data = utils::ReadValue<DWORD>(aray + i * 4);
		map.MapCross.push_back(data);
	}


	map.startpnt.x = utils::ReadValue<DWORD>(base2 + Base_CurrentRoomXPos) + 1;
	map.startpnt.y = utils::ReadValue<DWORD>(base2 + Base_CurrentRoomYPos) + 1;

	if (bDeep) {
		map.endpnt.x = GameCall::Dectypt(base3 + Base_DeepBoosRoomXPos) + 1;
		map.endpnt.y = GameCall::Dectypt(base3 + Base_DeepBoosRoomYPos) + 1;
	}
	else {
		map.endpnt.x = GameCall::Dectypt(base3 + Base_BoosRoomXPos) + 1;
		map.endpnt.y = GameCall::Dectypt(base3 + Base_BoosRoomYPos) + 1;
	}


	//{
	//	std::wstringstream ss;
	//	ss << L"当前坐标： [" << map.startpnt.x << L"," << map.startpnt.y << L"] BOOS房间[" << map.endpnt.x << L"," << map.endpnt.y << L"]";
	//	GameCall::Bulletin(ss.str());
	//	Sleep(500);
	//}

	map.CastPL = GetRealPath(map.MapCross, map.width, map.height, map.startpnt, map.endpnt, map.path);
	return map;
}

DWORD GetDirection::GetRealPath(std::vector<DWORD> MapCross, DWORD width, DWORD height, POINT_3D_DWORD start, POINT_3D_DWORD end, std::vector<POINT_3D_DWORD>& realpath)
{
	std::list<POINT_3D_DWORD> crossway;

	if (start.x == end.x && start.y == end.y)
	{
		realpath.clear();
		realpath.resize(0);
		return 0;
	}


	std::vector<std::vector<MAP_CROSS>> newCross;
	newCross.clear();
	newCross.resize(width);
	for (auto i = 0; i != width; i++)
	{
		newCross.at(i).resize(height);
	}

	int i = 0;
	for (auto y = 0; y != height; y++)
	{
		for (auto x = 0; x != width; x++)
		{
			newCross[x][y].currpnt = start;
			newCross[x][y].cross = MapCross[i];
			newCross[x][y].left = GetMapDirection(MapCross[i], 0);
			newCross[x][y].right = GetMapDirection(MapCross[i], 1);
			newCross[x][y].top = GetMapDirection(MapCross[i], 2);
			newCross[x][y].bottom = GetMapDirection(MapCross[i], 3);
			newCross[x][y].color = 0xFFFFFF;
			i++;
			if (newCross[x][y].cross == 0) {
				newCross[x][y].color = 0;
			}
		}
	}


	std::vector<std::vector<MAP_CROSS>> nextCross;
	nextCross.clear();
	nextCross.resize(width * 3);
	for (auto i = 0; i != width * 3; i++)
	{
		nextCross[i].resize(height * 3);
	}


	for (auto y = 0; y != height; y++)
	{
		for (auto x = 0; x != width; x++)
		{
			nextCross[(x + 1) * 3 - 2][(y + 1) * 3 - 2].color = 0xFFFFFF;
			if (newCross[x][y].left)
			{
				nextCross[(x + 1) * 3 - 3][(y + 1) * 3 - 2].color = 0xFFFFFF;
			}
	
			if (newCross[x][y].right)
			{
				nextCross[(x + 1) * 3 - 1][(y + 1) * 3 - 2].color = 0xFFFFFF;
			}

			if (newCross[x][y].top)
			{
				nextCross[(x + 1) * 3 - 2][(y + 1) * 3 - 3].color = 0xFFFFFF;
			}

			if (newCross[x][y].bottom)
			{
				nextCross[(x + 1) * 3 - 2][(y + 1) * 3 - 1].color = 0xFFFFFF;
			}
		}
	}

	Get_Direction(nextCross, { start.x * 3 - 2 , start.y * 3 - 2 }, { end.x * 3 - 2 , end.y * 3 - 2 }, width * 3, height * 3, crossway);
	return SortPath(crossway, realpath);
}

DWORD GetDirection::SortPath(std::list<POINT_3D_DWORD> cross, std::vector<POINT_3D_DWORD>& path)
{
	int x, y;
	int k = 0;
	POINT_3D_DWORD tmp_coordinate;
	for (auto temp : cross)
	{
		x = (temp.x + 2) % 3;
		y = (temp.y + 2) % 3;
		if (x == 0 && y == 0)   //判断是否能被3整除
		{
			tmp_coordinate.x = (temp.x + 2) / 3 - 1;   //还原坐标
			tmp_coordinate.y = (temp.y + 2) / 3 - 1;
			path.push_back(tmp_coordinate);
			k++;
		}
	}
	return(k);
}

bool GetDirection::GetMapDirection(int cross_orientation, int orientation)
{
	unsigned char orientation_array[4];
	unsigned char orientation_vector[16][4] = {
	{ 0, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 1, 1, 0 },
	{ 1, 0, 0, 0 },
	{ 1, 1, 0, 0 },
	{ 1, 0, 1, 0 },
	{ 1, 1, 1, 0 },
	{ 0, 0, 0, 1 },
	{ 0, 1, 0, 1 },
	{ 0, 0, 1, 1 },
	{ 0, 1, 1, 1 },
	{ 1, 0, 0, 1 },
	{ 1, 1, 0, 1 },
	{ 1, 0, 1, 1 },
	{ 1, 1, 1, 1 } };
	if (cross_orientation >= 0 && cross_orientation <= 15)
		for (int i = 0; i < 4; i++)
			orientation_array[i] = orientation_vector[cross_orientation][i];
	else
		for (int i = 0; i < 4; i++)
			orientation_array[i] = 0;
	if (orientation_array[orientation] == 1)
		return true;
	else
		return false;
}

int GetDirection::GetNextDirection(POINT_3D_DWORD curr, POINT_3D_DWORD next)
{

	//{
	//	std::wstringstream ss;
	//	ss << L"当前坐标： [" << curr.x << L"," << curr.y << L"] 下个房间[" << next.x << L"," << next.y << L"]";
	//	GameCall::Bulletin(ss.str());
	//	Sleep(500);
	//}

	int direct = { 0 };

	int x = curr.x - next.x;
	int y = curr.y - next.y;

	if (x == 0) {
		if (y == 1) {
			//上
			direct = 2;
		}
		else {
			//下
			direct = 3;
		}
	}
	else {
		if (x == 1) {
			//左
			direct = 0;
		}
		else {
			//右
			direct = 1;
		}
	}
	return direct;
}
