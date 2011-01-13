#pragma once
#include <vector>
#include <windows.h>
#include "utils.h"
#include "SVector2D.h"
class CBarrier
{
public:
	vector<SPoint>	vecPoint;
	int		iMaxPointCount;
	
	void Render(HDC surface,const int cxClient,const int cyClient);

	bool IsIntersect(const SPoint &a,const SPoint &b);
	bool IsPointInHouse(const SPoint &a);

	CBarrier(const SPoint *lpSpints,int nCount);
	CBarrier(void);
	~CBarrier(void);
};
