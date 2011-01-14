#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "windows.h"

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
};

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

};


void DrawLine(HDC surface,const SPoint& a,const SPoint &b);
#endif