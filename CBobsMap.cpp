#include "CBobsMap.h"

const int CBobsMap::m_iStartX = START_X;
const int CBobsMap::m_iStartY = START_Y;

const int CBobsMap::m_iEndX =END_X;
const int CBobsMap::m_iEndY = END_Y;

//lhx dirct path
const SPoint CBobsMap::m_spA(m_iStartX,m_iStartY);
const SPoint CBobsMap::m_spB(m_iEndX,m_iEndY);

const SPoint CBobsMap::m_sp1[BARRIER_COUNT1]=
{
	SPoint(290, 58),
	SPoint(267 ,89),
	SPoint(295 ,109),
	SPoint(318 ,79),
	
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
	SPoint(147,118),
	SPoint(147,138),
	SPoint(182,138),
	SPoint(182,118),
	
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




//-------------------------------Render -----------------------------

void CBobsMap::Render(const int cxClient,
					  const int cyClient,
					  HDC surface)
{
	const int border = MAP_BORDER;

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


void CBobsMap::RenderOriginRoute(const int cxClient,
							const int cyClient,
							HDC surface)
{

	//grab a red pen 
	m_OldPen = (HPEN)SelectObject(surface,m_RedPen);

	Coordinate cord(m_spA,m_spB);

	vector<vector<WayPoint>>::const_iterator veciter = m_TestRoute.begin();

	
	//draw orginin path in red pen
	if(true)
	{
		while( veciter!=m_TestRoute.end())
		{
			SPoint spPre = m_spA;
			vector<WayPoint>::const_iterator iter = veciter->begin();
			while(iter != veciter->end())
			{

				SPoint spRelative =iter->relativeXY;
				spRelative.y=0;
				SPoint spRoot = cord.GetCoordinate(spRelative.x,spRelative.y);
				SPoint spPathAbsolute = iter->absoluteXY;

				DrawLine(surface,spPre,spPathAbsolute);


				spPre =spPathAbsolute;
				iter++;
			}
			DrawLine(surface,spPre,m_spB);
			veciter++;

		}

	}
			SelectObject(surface, m_OldPen);	


}

void CBobsMap::RenderShortRoute(const int cxClient,
							 const int cyClient,
							 HDC surface)
{
	//grab a green pen 
	m_OldPen = (HPEN)SelectObject(surface, m_GreenPen);

	vector<vector<SPoint>>::const_iterator veciter = m_BestRoute.begin();
	while(veciter!= m_BestRoute.end())
	{
		vector<SPoint>::const_iterator siter = veciter->begin();
		SPoint prePoint = *siter;
		siter++;
		while(siter!= veciter->end())
		{
			DrawLine(surface,prePoint,*siter);
			prePoint=*siter;
			siter++;
		}
		veciter++;
	}
	//restore the original brush
	SelectObject(surface, m_OldPen);
}

void CBobsMap::RenderBestRoute(const int cxClient, const int cyClient, HDC surface,vector<WayPoint> bestRoute)
{
		//grab a green pen 
	m_OldPen = (HPEN)SelectObject(surface, m_BluePen);

	vector<SPoint> fixed = FixToBestPath(bestRoute);
	SPoint prePoint = m_spA;
	vector<SPoint>::const_iterator iter = fixed.begin();
	while(iter!=fixed.end())
	{
		SPoint nextPoint = *iter;
		DrawLine(surface,prePoint,nextPoint);
		prePoint=nextPoint;
		iter++;
	}


		//restore the original brush
	SelectObject(surface, m_OldPen);
}



CBobsMap::CBobsMap()
{

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
		//lhx grab pens
		m_GreenPen = CreatePen(PS_SOLID, 1, RGB(0,255, 0));
		m_RedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		m_BluePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

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

bool CBobsMap::GetOneValidPath(vector<int> &vecBits,int chromolen)
{
	int testCount=0;
	int iChromoLength=chromolen;
	int iCutCount=iChromoLength+1;

	Coordinate cord(m_spA,m_spB);

	SPoint spPre=m_spA;
	
	int randomY = 0;
	//divied space 
	SVector2D line(m_spA.x-m_spB.x,m_spA.y-m_spB.y);
	int diffX = (int) Vec2DLength(line)/iCutCount;
	for(int i=1;i<=iChromoLength;i++)
	{	
		randomY = RandInt(-150,150);
		SPoint spRelative(i*diffX,randomY);
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
			
			randomY = RandInt(-150,150);
			spRelative=SPoint(i*diffX,randomY);
			spPathAbsolute=cord.GetCoordinate(spRelative.x,spRelative.y);
		}	
		//save waypoint
		vecBits.push_back(randomY);
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

int CBobsMap::CalculateInvalidPointCount(const vector<SPoint> &waypoints)
{
		int count = 0;
		if(waypoints.size()<1) return -1;
		vector<SPoint>::const_iterator iter = waypoints.begin();
		SPoint spPre =m_spA;
		iter++;
		SPoint spNext =*iter;

		while(iter!=waypoints.end())
		{
			spNext = *iter;
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
			spNext =iter->absoluteXY;
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
	SPoint preAbs,fBRel,fCRel,fAbs;
	preAbs=m_spA;

	vector<SPoint>bestPath;
	//push start point 
	bestPath.push_back(m_spA);

	int i=0;
	int index=0;
	int vecPathSize = vecPath.size();
	for(int q=0;q<vecPathSize;q++)
	{
		fBRel=vecPath[q].relativeXY;
		fAbs =vecPath[q].absoluteXY;
		for(i=q+1;i<vecPath.size();i++)
		{
			fCRel=vecPath[i].relativeXY;
			
			//if(abs(fBRel.y)>abs(fCRel.y)||Equal(fBRel.y,fCRel.y))
			//{
				bool log = BarrierIntersection(preAbs,vecPath[i].absoluteXY);
				if(!log)
				{
					fBRel = fCRel;
					fAbs =vecPath[i].absoluteXY;
					q=i;
				}
			//}

		}

		bestPath.push_back(fAbs);
		preAbs=fAbs;
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

vector<WayPoint> CBobsMap::Decode(const vector<int> &ycoodinate)
{
	vector<WayPoint> wayPoint;
	vector<int>::const_iterator iter = ycoodinate.begin();
	
	SVector2D line(m_spA.x-m_spB.x,m_spA.y-m_spB.y);
	int diffX = (int) Vec2DLength(line)/(ycoodinate.size()+1);
	int index=1;
	
	while(iter!=ycoodinate.end())
	{
	
		SPoint relativeXY(index*diffX,*iter);
		SPoint absoluteXY =TransRelativePointToAbusolutePoint(relativeXY);
		WayPoint point(absoluteXY,relativeXY);
		wayPoint.push_back(point);
		index++;
		iter++;
	}
	return wayPoint;
}

double CBobsMap::TestRoute(const vector<WayPoint> &vecWayPoints)
{
	
	vector<SPoint> vecFixedPoint = FixToBestPath(vecWayPoints);
	//add one route
	m_TestRoute.push_back(vecWayPoints);
	m_BestRoute.push_back(vecFixedPoint);
	//calculate the tourlength for each chromosome
	double fitness =0.0;
	fitness = GetPathLength(vecFixedPoint);
	
	// if route has intersection then add punishment
	fitness += 50* CalculateInvalidPointCount(vecFixedPoint);
	return fitness;
}