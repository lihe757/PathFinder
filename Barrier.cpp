#include "Barrier.h"

CBarrier::CBarrier(void)
{
}

CBarrier::CBarrier(const SPoint *lpSpints,int nCount)
{
	for(int i=0;i<nCount;i++)
	{
		vecPoint.push_back(*(lpSpints++));
	}
}
//check a point is inner polygon
bool CBarrier::IsPointInHouse(const SPoint &a)
{


	vector<SPoint>::const_iterator iter = vecPoint.begin();
	if(iter==vecPoint.end())return false;

	SPoint tmp(iter->x,iter->y);
	SVector2D vectA(tmp.x-a.x,tmp.y-a.y);
	Vec2DNormalize(vectA);
	iter++;

	SVector2D vectB((iter->x)-a.x,(iter->y)-a.y);
	Vec2DNormalize(vectB);

	int b=Vec2DSign(vectA,vectB);
	vectA=vectB;
	iter++;

	while(iter!=vecPoint.end())
	{	
		vectB.x= (iter->x)-a.x;
		vectB.y= (iter->y)-a.y;
		Vec2DNormalize(vectB);
		int c=Vec2DSign(vectA,vectB);
		if(c!=b) return false;

		vectA=vectB;
		iter++;
	}

	vectB=SVector2D(vecPoint[0].x-a.x,vecPoint[0].y-a.y);
	Vec2DNormalize(vectB);
	int bc =Vec2DSign(vectA,vectB);
	if(bc==b) return true;


	return false;

}

bool CBarrier::IsIntersect(const SPoint &a,const SPoint &b)
{
	vector<SPoint>::const_iterator iter = vecPoint.begin();

	SPoint pA(iter->x,iter->y);
	iter++;

	SPoint pB(iter->x,iter->y);
	iter++;

	SPoint pINT(0,0);
	int flag = Intersection(a,b,pA,pB,pINT);
	if(flag >0) return true;
	pA=pB;


	while(iter!=vecPoint.end())
	{

		pB=SPoint(iter->x,iter->y);
		flag = Intersection(a,b,pA,pB,pINT);
		if(flag >0) return true;
		pA=pB;
		iter++;
	}

	//original point line
	pB=vecPoint[0];
	flag = Intersection(a,b,pA,pB,pINT);
	if(flag >0) return true;

	return false;

}

void CBarrier::Render(HDC surface,const int cxClient,const int cyClient)
{
	HBRUSH	BlackBrush, RedBrush, BlueBrush,OldBrush;
	HPEN	NullPen, OldPen;

	BlackBrush = CreateSolidBrush(RGB(0,0,0));

	POINT *points=new POINT[vecPoint.size()];

	for(int i=0;i<vecPoint.size();i++)
	{
		points[i].x=(LONG)vecPoint[i].x;
		points[i].y=(LONG)vecPoint[i].y;

	}
	OldBrush = (HBRUSH)SelectObject(surface, BlackBrush);
	Polygon(surface,points,vecPoint.size());

	delete[] points;


	////restore the original brush
	SelectObject(surface, OldBrush);

	////and pen
	//SelectObject(surface, OldPen);
}

CBarrier::~CBarrier(void)
{
}
