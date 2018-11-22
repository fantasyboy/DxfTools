#pragma once
#include <vector>
#include <list>
typedef struct _POINT_3D_DWORD
{
	int x;
	int y;
	int z;
}POINT_3D_DWORD, *PPOINT_3D_DWORD;

typedef struct _A_STAR_NODE
{
	DWORD F;
	DWORD G;
	DWORD H;
	POINT_3D_DWORD currpnt;
	POINT_3D_DWORD parentpnt;
}A_STAR_NODE, *PA_STAR_NODE;


typedef struct _MAP_DATA
{
	wchar_t* mapname;
	DWORD MapId;
	std::vector<DWORD> MapCross;
	POINT_3D_DWORD startpnt;
	POINT_3D_DWORD endpnt;
	DWORD width;
	DWORD height;
	std::vector<POINT_3D_DWORD> path;
	DWORD CastPL; 
}MAP_DATA, *PMAP_DATA;

typedef struct _MAP_CROSS
{
	POINT_3D_DWORD currpnt;
	DWORD cross;
	bool left;
	bool right;
	bool top;
	bool bottom;
	DWORD color;
}MAP_CROSS, *PMAP_CROSS;

class GetDirection
{
public:
	GetDirection();
	~GetDirection();


	void Get_Direction(std::vector<std::vector<MAP_CROSS>> mapCross, POINT_3D_DWORD start, POINT_3D_DWORD end, int width, int height, std::list<POINT_3D_DWORD>& path);
	MAP_DATA GetData(bool bDeep);
	DWORD GetRealPath(std::vector<DWORD> MapCross, DWORD width, DWORD height, POINT_3D_DWORD start, POINT_3D_DWORD end, std::vector<POINT_3D_DWORD>& realpath);
	DWORD SortPath(std::list<POINT_3D_DWORD> cross, std::vector<POINT_3D_DWORD>& path);
	bool GetMapDirection(int cross_orientation, int orientation);
	int GetNextDirection(POINT_3D_DWORD curr, POINT_3D_DWORD next);
};

