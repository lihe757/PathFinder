#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "windows.h"
#include "SVector2D.h"

using namespace std;

//----------------------------------------------------------------------------
//	some random number functions.
//----------------------------------------------------------------------------

//returns a random integer between x and y
inline int	  RandInt(int x,int y) {return rand()%(y-x+1)+x;}

//returns a random float between zero and 1
inline double RandFloat()		   {return (rand())/(RAND_MAX+1.0);}

//returns a random bool
inline bool   RandBool()
{
	if (RandInt(0,1)) return true;

	else return false;
}

//returns a random float in the range -1 < n < 1
inline double RandomClamped()	   {return RandFloat() - RandFloat();}

//-----------------------------------------------------------------------
//	
//	some handy little functions
//-----------------------------------------------------------------------

//converts an integer to a std::string
string itos(int arg);

//converts an float to a std::string
string ftos (float arg);

//	clamps the first argument between the second two
void Clamp(double &arg, float min, float max);

/////////////////////////////////////////////////////////////////////
//
//	Point structure
//
/////////////////////////////////////////////////////////////////////
struct SPoint
{
	float x, y;

	SPoint(){}
	SPoint(float a, float b):x(a),y(b){}

	SPoint &operator = (const SPoint &sp)
	{
		x =sp.x;
		y =sp.y;
		return *this;
	}
	float DistanceToMe(const SPoint &p)
	{
		return (float) sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
	}
};






///////////////////////////////////////////////////
//
//Line tools
//
//
///////////////////////////////////////////////////

//get a point with constant distance to point a
void GetPointFromLine(SPoint &c,const SPoint &a,const SPoint &b,float dist);
void GetPointFormLine(SPoint &c,const SPoint &a,const float &k,const float &b,float dist);
int Intersection(SPoint p1, SPoint p2, SPoint p3, SPoint p4, SPoint &Int);



/////////////////////////////////////////////////////////////////////
//
//	Line structure
//
/////////////////////////////////////////////////////////////////////
enum Location
{
	LEFT,
	RIGHT,
	ON
};
struct Line
{
	SPoint xStart;
	SPoint xEnd;
	float	k;
	float A;
	float B;
	float C;

	Line(const SPoint &a,const SPoint &b)
	{
		k = (a.y-b.y)/(a.x-b.x);
		float bb=a.y-k*(a.x);
		A=k;
		B=-1;
		C=bb;
	}
	Line(float _k,const SPoint &p)
	{
		k =_k;
		float bb= p.y-k*p.x;
		A=k;
		B=-1;
		C=bb;
	}

	Location At(const SPoint &c)
	{
		int result = (int)(A*c.x+B*c.y+C);
		if(result >0)
		{
			return RIGHT;
		}
		else if(result <0)
		{
			return LEFT;
		}
		else if(result ==0)
		{
			return ON;
		}
		return ON;		
	}

	float DistanceToMe(const SPoint &c)
	{
			float tmp =A*c.x+B*c.y+C;
			return  (float)(tmp/sqrt(A*A+B*B));
			
	}

};

/////////////////////////////////////////////////////////////////////
//
//	Coordinate system structure
//
/////////////////////////////////////////////////////////////////////
struct Coordinate
{
	SPoint xStart;
	SPoint xEnd;
	float	k;



	Coordinate(const SPoint &a,const SPoint &b)
	{
		xStart.x=a.x;
		xStart.y=a.y;

		xEnd.x=b.x;
		xEnd.y=b.y;

		k = (a.y-b.y)/(a.x-b.x);
	}

	SPoint GetXProjection(float x)
	{
		SPoint c(0,0);
		GetPointFromLine(c,xStart,xEnd,x);
		return c;
	}
	// gather absolute coordinate
	SPoint	GetCoordinate(float x,float y)
	{
		SPoint c;
		c=GetXProjection(x);
		SPoint c2;
		float tmpk=-1/k;
		float bb=c.y-tmpk*c.x;
		GetPointFormLine(c2,c,tmpk,bb,y);
		return c2;

	}
	SPoint GetRelativePoint(float x,float y)
	{	SPoint p(x,y);
		SPoint c;
		Line lineX(xStart,xEnd);
		
		float dist = lineX.DistanceToMe(p);
		c.y=(int)dist;
		SVector2D vect(xStart.x-p.x,xStart.y-p.y);
		float d = (float)Vec2DLength(vect);
		float dx = (float)sqrt(d*d-dist*dist);

		float tmpk = -1/k;
		Line lineY(tmpk,xStart);

		Location location = lineY.At(p);
		if(location == RIGHT) dx*=-1;
		c.x=(int)dx;

		return c;


	}
};
////////////////////////////////////////////////////////////////
//
//SPoint operator override
///////////////////////////////////////////////////////////////
bool Equal(float f1, float f2) ;
	//判断两点是否相等
bool operator==(const SPoint &p1, const SPoint &p2);
	//比较两点坐标大小，先比较x坐标，若相同则比较y坐标
bool operator> (const SPoint &p1, const SPoint &p2);
float operator^(const SPoint &p1, const SPoint &p2);

///////////////////////////////////////////////////////
//
//	waypoint struct
//
//////////////////////////////////////////////////////
struct WayPoint
{
	SPoint absoluteXY;
	SPoint relativeXY;

	WayPoint(const SPoint &absXY,const SPoint &reXY)
	{
		absoluteXY=absXY;
		relativeXY=reXY;
	}

};


void DrawLine(HDC surface,const SPoint& a,const SPoint &b);
#endif