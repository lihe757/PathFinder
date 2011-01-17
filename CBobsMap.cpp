#include "CBobsMap.h"


//this defines our little maze which Bob wanders
//around in
const int CBobsMap::map[MAP_HEIGHT][MAP_WIDTH] = 
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
 8, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1,
 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5,
 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

//lhx dirct path
const SPoint CBobsMap::m_spA(START_X,START_Y);
const SPoint CBobsMap::m_spB(END_X,END_Y);

const SPoint CBobsMap::m_sp1[BARRIER_COUNT1]=
{
	SPoint(290.76, 58),
	SPoint(267.74 ,89),
	SPoint(295.4 ,109),
	SPoint(318.42 ,79),
	
};
const SPoint CBobsMap::m_sp2[BARRIER_COUNT2]=
{
	SPoint(258,123),
	SPoint(217,119),
	SPoint(242,182),
	
};
const SPoint CBobsMap::m_sp3[BARRIER_COUNT3]=
{
	SPoint(317,143),
	SPoint(327,173),
	SPoint(355,164),
	SPoint(344,133),
	
};
const SPoint CBobsMap::m_sp4[BARRIER_COUNT4]=
{
	SPoint(294,275),
	SPoint(265,240),
	SPoint(226,277),
	
};
const SPoint CBobsMap::m_sp5[BARRIER_COUNT5]=
{
	SPoint(155,208),
	SPoint(167,245),
	SPoint(215,230),
	SPoint(203,192),
	
};
const SPoint CBobsMap::m_sp6[BARRIER_COUNT6]=
{
	SPoint(107,118),
	SPoint(107,138),
	SPoint(142,138),
	SPoint(142,118),
	
};

const int CBobsMap::m_aBarriesCount[MAX_BARRIERS]=
{
	 BARRIER_COUNT1	,
	 BARRIER_COUNT2	,
	 BARRIER_COUNT3	,
	 BARRIER_COUNT4	,
	 BARRIER_COUNT5	,
	 BARRIER_COUNT6
};

const int CBobsMap::m_iMapHeight = MAP_HEIGHT;
const int CBobsMap::m_iMapWidth  = MAP_WIDTH;

const int CBobsMap::m_iStartX = 14;
const int CBobsMap::m_iStartY = 7;

const int CBobsMap::m_iEndX = 0;
const int CBobsMap::m_iEndY = 2;


//-------------------------------Render -----------------------------

void CBobsMap::Render(const int cxClient,
					  const int cyClient,
					  HDC surface)
{
	const int border = MAP_BORDER;

	int BlockSizeX = (cxClient - 2*border)/m_iMapWidth;
	int BlockSizeY = (cyClient - 2*border)/m_iMapHeight;

	RECT rectan ={border,border,cxClient-border, cyClient-border};
	m_recBound.left=rectan.left;
	m_recBound.right=rectan.right;
	m_recBound.top=rectan.top;
	m_recBound.bottom=rectan.bottom;


	HBRUSH	BlackBrush, OldBrush;

	//grab a brush to fill our cells with
	BlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);






	Rectangle(surface, m_recBound.left, m_recBound.top , m_recBound.right, m_recBound.bottom);
	//grab DiagonalLength
	m_fDiagonalLength = sqrt((float)(m_recBound.right*m_recBound.right)+(m_recBound.bottom*m_recBound.bottom));
	
	
	//select them into the device conext
	OldBrush = (HBRUSH)SelectObject(surface, BlackBrush);
	//draw barriers
	for(int i=0;i<MAX_BARRIERS;i++)
	{
		m_vecBarriers[i].Render(surface,cxClient,cyClient);
	}

	DrawLine(surface , m_spA , m_spB);
	//A
	string Start = "A("+itos(m_spA.x)+string(",")+itos(m_spA.y)+string(")");	
	TextOut(surface, m_spA.x-20,m_spA.y-20, Start.c_str(), Start.size());
	//B
	string End = "B("+itos(m_spB.x)+string(",")+itos(m_spB.y)+string(")");	
	TextOut(surface, m_spB.x+20,m_spB.y+20, End.c_str(), End.size());

	//restore the original brush
	SelectObject(surface, OldBrush);

}


void CBobsMap::MemoryRender2(const int cxClient,
							const int cyClient,
							HDC surface,
							const vector<WayPoint> &wayPoint)
{
	
	HPEN	OldPen,RedPen;
	//grab a red pen 
	RedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	OldPen = (HPEN)SelectObject(surface, RedPen);

	Coordinate cord(m_spA,m_spB);

	//draw orginin path in red pen
	if(true)
	{
		SPoint spPre = m_spA;
		vector<WayPoint>::const_iterator iter = wayPoint.begin();
		while(iter != wayPoint.end())
		{
			
			SPoint spRelative =iter->relativeXY;
			spRelative.y=0;
			SPoint spRoot = cord.GetCoordinate(spRelative.x,spRelative.y);
			SPoint spPathAbsolute = iter->absoluteXY;

			//draw zhu zi
			//DrawLine(surface,spRoot,spPathAbsolute);
			//draw line to connect spPre and spPathAbsolute

			DrawLine(surface,spPre,spPathAbsolute);


			spPre =spPathAbsolute;
			iter++;
		}
			DrawLine(surface,spPre,m_spB);
	}
			SelectObject(surface, OldPen);	


}

void CBobsMap::MemoryRender3(const int cxClient,
							const int cyClient,
							HDC surface,
							const vector<SPoint> &bestPath)
{
	if(bestPath.size()<2) return ;

	HPEN	OldPen,GreenPen;
	//grab a green pen 
	GreenPen = CreatePen(PS_SOLID, 1, RGB(0,255, 0));
	OldPen	 =	(HPEN)SelectObject(surface, GreenPen);

	vector<SPoint>::const_iterator siter = bestPath.begin();
	SPoint prePoint = *siter;
	siter++;
	while(siter!= bestPath.end())
	{
		DrawLine(surface,prePoint,*siter);
		prePoint=*siter;
		siter++;
	}
	
	//restore the original brush
	SelectObject(surface, OldPen);
}


//--------------------- ResetMemory --------------------------
//
//	resets the memory map to zeros
//------------------------------------------------------------
void CBobsMap::ResetMemory()
{
	for (int y=0; y<m_iMapHeight; ++y)
	{
		for (int x=0; x<m_iMapWidth; ++x)
		{
			memory[y][x] = 0;
		}
	}
}

CBobsMap::CBobsMap()
{
	ResetMemory();

		//lhx	add Barriers
		m_vecBarriers.push_back(CBarrier(m_sp1,m_aBarriesCount[0]));
		m_vecBarriers.push_back(CBarrier(m_sp2,m_aBarriesCount[1]));
		m_vecBarriers.push_back(CBarrier(m_sp3,m_aBarriesCount[2]));
		m_vecBarriers.push_back(CBarrier(m_sp4,m_aBarriesCount[3]));
		m_vecBarriers.push_back(CBarrier(m_sp5,m_aBarriesCount[4]));
		m_vecBarriers.push_back(CBarrier(m_sp6,m_aBarriesCount[5]));
		//lhx bound barriers
		m_recBound.left=MAP_BORDER;
		m_recBound.right=WINDOW_WIDTH-MAP_BORDER;
		m_recBound.top=MAP_BORDER;
		m_recBound.bottom=WINDOW_HEIGHT-MAP_BORDER;
}


bool CBobsMap::IsValidPoint (const SPoint &point)
{
	SPoint pLeftTop(m_recBound.left,m_recBound.top);
	SPoint pRightTop(m_recBound.right,pLeftTop.y);
	SPoint pLeftButtom(pLeftTop.x,m_recBound.bottom);
	SPoint pRightButtom(pRightTop.x,pLeftButtom.y);

	
	SPoint bounds[4]=
	{
		pLeftTop,
		pRightTop,
		pRightButtom,
		pLeftButtom
	};
	
	CBarrier boundBarrier(bounds,4);

	if(boundBarrier.IsPointInHouse(point)) return true;
	return false;

}

bool CBobsMap::GetOneValidPath(vector<WayPoint> &path)
{
	int testCount=0;

	Coordinate cord(m_spA,m_spB);
	SPoint spPre=m_spA;

	//divied space 
	SVector2D line(m_spA.x-m_spB.x,m_spA.y-m_spB.y);
	int count = (int) Vec2DLength(line)/20;
	for(int i=1;i<=count;i++)
	{
		SPoint spRoot=cord.GetXProjection(i*20);
		SPoint spRelative(i*20,RandInt(-100,100));
		SPoint spPathAbsolute=cord.GetCoordinate(spRelative.x,spRelative.y);
		while(true)
		{			

			if(!BarrierIntersection(spPre,spPathAbsolute)&&IsValidPoint(spPathAbsolute))
			{
				break;
				
			}else
			{
				testCount++;
				if(testCount>100) 
				{
					return false;
				}
			}
			
			spRelative=SPoint(i*20,RandInt(-100,100));
			spPathAbsolute=cord.GetCoordinate(spRelative.x,spRelative.y);
		}	
		//save waypoint
		path.push_back(WayPoint(spPathAbsolute,spRelative));
		spPre=spPathAbsolute;
	}

	
	return true;
}

//if line intersect with barriers the return true;
bool CBobsMap::BarrierIntersection(const SPoint &a,const SPoint &b)
{
			
			for(int j=0;j<MAX_BARRIERS;j++)
			{
				bool log = m_vecBarriers[j].IsIntersect(a,b);		
				if(log)
				{
					return true;
				}
			}
		
			return false;

}

int CBobsMap::CalculateInvalidPointCount(const vector<WayPoint> &waypoints)
{
		int count = 0;
		if(waypoints.size()<1) return -1;
		vector<WayPoint>::const_iterator iter = waypoints.begin();
		SPoint spPre =m_spA;
		iter++;
		SPoint spNext =iter->absoluteXY;

		while(iter!=waypoints.end())
		{
			spNext = iter->absoluteXY;
			//no intersect with barriers and in the rectange of bounds
			if(BarrierIntersection(spPre,spNext)||!IsValidPoint(spNext))
			{
				count++;
			}
			spPre = spNext;
			iter++;
		}
		
		//check B point
		spNext = m_spB;
		if(BarrierIntersection(spPre,spNext)||!IsValidPoint(spNext))
		{
			count++;
		}
		return count;
}





vector<SPoint> CBobsMap::FixToBestPath(const vector<WayPoint> &waypoints)
{

	vector<WayPoint> vecPath=waypoints;
	Coordinate cord(m_spA,m_spB);
	//push the last point
	SPoint endPoint=cord.GetRelativePoint(m_spB.x,m_spB.y);
	vecPath.push_back(WayPoint(m_spB,endPoint));
	
	//fix waypoint
	SPoint fA,fB,fC;
	fA=m_spA;

	vector<SPoint>bestPath;
	//push start point 
	bestPath.push_back(m_spA);

	int i=0;
	int index=0;

	for(int q=0;q<vecPath.size();q++)
	{

		for(i=q;i<vecPath.size();i++)
		{
			fB=vecPath[i].absoluteXY;

			bool log = BarrierIntersection(fA,fB);
			if(!log)
			{
				fC=fB;
				q=i;
			}
				
		}

		bestPath.push_back(fC);
		fA=fC;
	}

	return bestPath;
}

float  CBobsMap::GetPathLength(const vector<SPoint> &path)
{

	float fTotalLength=0;
	vector<SPoint>::const_iterator iter = path.begin();

	SPoint start =*iter;
	iter++;
	while(iter!= path.end())
	{
		SPoint end = *iter;
		fTotalLength += start.DistanceToMe(end);
		start = end;
		iter++;
	}
	return fTotalLength;
}

SPoint CBobsMap::TransRelativePointToAbusolutePoint(const SPoint& src)
{
		
		Coordinate cord(m_spA,m_spB);
		SPoint target =cord.GetCoordinate(src.x,src.y);
		return target;
}