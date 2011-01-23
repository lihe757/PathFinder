#ifndef CBOBSMAP_H
#define CBOBSMAP_H

///////////////////////////////////////////////////////////////////////
//
//		File: CBobsMap.h
//
//		Author: Mat Buckland
//
//		Desc: Class for defining the map described in chapter 3
//
///////////////////////////////////////////////////////////////////////

#include "stdlib.h"
#include "windows.h"
#include <vector>
#include "Barrier.h"
#include "defines.h"


using namespace std;




class CBobsMap
{
private:
	
	//lihx barries points 
	static const int	m_aBarriesCount[MAX_BARRIERS];
	static const SPoint	m_sp1[BARRIER_COUNT1];
	static const SPoint	m_sp2[BARRIER_COUNT2];
	static const SPoint	m_sp3[BARRIER_COUNT3];
	static const SPoint	m_sp4[BARRIER_COUNT4];
	static const SPoint	m_sp5[BARRIER_COUNT5];
	static const SPoint	m_sp6[BARRIER_COUNT6];
	//lhx	barries
	vector<CBarrier>	m_vecBarriers;
	static const SPoint m_spA;
	static const SPoint	m_spB;

	//index into the array which is the start point
	static const int	m_iStartX;
	static const int	m_iStartY;

	//and the finish point
	static const int	m_iEndX;
	static const int	m_iEndY;

	//lhx
	 HPEN				 m_OldPen ;
	 HPEN				 m_GreenPen;
	 HPEN				 m_RedPen;
	 HPEN				 m_BluePen;


public:

	
	//lihx  diagonal length
	float				 m_fDiagonalLength;
	RECT				 m_recBound;
	vector<vector<WayPoint>> m_TestRoute;
	vector<vector<SPoint>>  m_BestRoute;


	CBobsMap();

	//given a surface to draw on this function uses the windows GDI
	//to display the map.
	void Render(const int cxClient, const int cyClient, HDC surface);

	//draws whatever path may be stored in the memory
		
	void RenderOriginRoute(const int cxClient, const int cyClient, HDC surface);
	void RenderShortRoute(const int cxClient, const int cyClient, HDC surface);
	void RenderBestRoute(const int cxClient, const int cyClient, HDC surface,vector<WayPoint> bestRoute);


	bool IsValidPoint (const SPoint &point);
	bool BarrierIntersection(const SPoint &a,const SPoint &b);
	
	//lihx gather one valid path
	bool GetOneValidPath(vector<int> &vecBits,int chromolen);
	vector<SPoint> FixToBestPath(const vector<WayPoint> &waypoints );
	//calculate invlid point count
	int CalculateInvalidPointCount(const vector<SPoint> &waypoints);
	int CalculateInvalidPointCount(const vector<WayPoint> &waypoints);
	float GetPathLength(const vector<SPoint> &path);
	//transLate
	SPoint TransRelativePointToAbusolutePoint(const SPoint& src);
	//test Route
	vector<WayPoint>  Decode(const vector<int> &ycoodinate);

	double TestRoute(const vector<WayPoint> &vecWayPoints);

	


};



#endif



			