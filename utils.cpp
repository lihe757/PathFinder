#include "utils.h"
#include <math.h>



//--------------------------itos------------------------------------
//	converts an integer to a string
//------------------------------------------------------------------		
string itos(int arg)
{
    ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}


//--------------------------ftos------------------------------------
//	converts a float to a string
//------------------------------------------------------------------		
string ftos(float arg)
{
    ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}
//-------------------------------------Clamp()-----------------------------------------
//
//	clamps the first argument between the second two
//
//-------------------------------------------------------------------------------------
void Clamp( double &arg, float min, float max)
{
	if (arg < min)
	{
		arg = min;
	}

	if (arg > max)
	{
		arg = max;
	}
}
///////////////////////////////
//
// a & b determine a line .the arg dist is distance of point c to point a
/////////////////////
void GetPointFromLine(SPoint &c ,const SPoint &a,const SPoint &b,float dist)
{
		float _dist=(float)abs(dist);	

		float k=(a.y-b.y)/(a.x-b.x);
		float bb=a.y-k*(a.x);
		float A=(1+k*k);
		float B=-(2*a.x+2*a.y*k-2*k*bb);
		float C=a.x*a.x+bb*bb-2*a.y*bb+a.y*a.y-_dist*_dist;

		float X1=(float) ((-B)+sqrt(B*B-4*A*C))/(2*A);
		float Y1=k*X1+bb;

		float X2=(float) ((-B)+sqrt(B*B-4*A*C))/(2*A);
		float Y2=k*X2+bb;
	
		if(dist>0)
		{
			c.x=X1;
			c.y=Y1;
		}else
		{
			c.x=X2;
			c.y=Y2;
		}
		
	
}

//has konowed k,b
void GetPointFormLine(SPoint &c,const SPoint &a,const float &k,const float &b,float dist)
{
	float _dist = (float)abs(dist);
		float A=(1+k*k);
		float B=-(2*a.x+2*a.y*k-2*k*b);
		float C=a.x*a.x+b*b-2*a.y*b+a.y*a.y-_dist*_dist;

		float X1=(float) ((-B)+sqrt(B*B-4*A*C))/(2*A);
		float Y1=k*X1+b;

		float X2=(float) ((-B)-sqrt(B*B-4*A*C))/(2*A);
		float Y2=k*X2+b;

		if(dist>0)
		{
			c.x=X1;
			c.y=Y1;
		}else
		{
			c.x=X2;
			c.y=Y2;
		}
		
}

	bool Equal(float f1, float f2) 
	{
		return (abs(f1 - f2) < 1e-4f);
	}
	//判断两点是否相等
	bool operator==(const SPoint &p1, const SPoint &p2) 
	{
		return (Equal(p1.x, p2.x) && Equal(p1.y, p2.y));
	}

	//比较两点坐标大小，先比较x坐标，若相同则比较y坐标
	bool operator> (const SPoint &p1, const SPoint &p2) 
	{
		return (p1.x > p2.x || (Equal(p1.x, p2.x) && p1.y > p2.y));
	}

	//计算两向量外积
	float operator^(const SPoint &p1, const SPoint &p2)
	{
		return (p1.x * p2.y - p1.y * p2.x);
	}





//判定两线段位置关系，并求出交点(如果存在)。
//有重合：完全重合(6)，1个端点重合且共线(5)，部分重合(4)
//无重合：两端点相交(3)，交于线上(2)，正交(1)，无交(0)，参数错误(-1)
int Intersection(SPoint p1, SPoint p2, SPoint p3, SPoint p4, SPoint &Int) {
	//保证参数p1!=p2，p3!=p4
	if (p1 == p2 || p3 == p4) {
		return -1; //返回-1代表至少有一条线段首尾重合，不能构成线段
	}
	//为方便运算，保证各线段的起点在前，终点在后。
	if (p1 > p2) {
		swap(p1, p2);
	}
	if (p3 > p4) {
		swap(p3, p4);
	}
	//求出两线段构成的向量
	SPoint v1 (p2.x - p1.x, p2.y - p1.y);
	SPoint  v2 (p4.x - p3.x, p4.y - p3.y);
	//求两向量外积，平行时外积为0
	float Corss = v1 ^ v2;
	//判定两线段是否完全重合
	if (p1 == p3 && p2 == p4) {
		return 6;
	}
	//如果起点重合
	if (p1 == p3) {
		Int = p1;
		//起点重合且共线(平行)返回5；不平行则交于端点，返回3
		return (Equal(Corss, 0) ? 5 : 3);
	}
	//如果终点重合
	if (p2 == p4) {
		Int = p2;
		//终点重合且共线(平行)返回5；不平行则交于端点，返回3
		return (Equal(Corss, 0) ? 5 : 3);
	}
	//如果两线端首尾相连
	if (p1 == p4) {
		Int = p1;
		return 3;
	}
	if (p2 == p3) {
		Int = p2;
		return 3;
	}//经过以上判断，首尾点相重的情况都被排除了
	//将线段按起点坐标排序。若线段1的起点较大，则将两线段交换
	if (p1 > p3) {
		swap(p1, p3);
		swap(p2, p4);
		//更新原先计算的向量及其外积
		swap(v1, v2);
		Corss = v1 ^ v2;
	}
	//处理两线段平行的情况
	if (Equal(Corss, 0)) {
		//做向量v1(p1, p2)和vs(p1,p3)的外积，判定是否共线
		SPoint vs (p3.x - p1.x, p3.y - p1.y);
		//外积为0则两平行线段共线，下面判定是否有重合部分
		if (Equal(v1 ^ vs, 0)) {
			//前一条线的终点大于后一条线的起点，则判定存在重合
			if (p2 > p3) {
				Int = p3;
				return 4; //返回值4代表线段部分重合
			}
		}//若三点不共线，则这两条平行线段必不共线。
		//不共线或共线但无重合的平行线均无交点
		return 0;
	} //以下为不平行的情况，先进行快速排斥试验
	//x坐标已有序，可直接比较。y坐标要先求两线段的最大和最小值
	float ymax1 = p1.y, ymin1 = p2.y, ymax2 = p3.y, ymin2 = p4.y;
	if (ymax1 < ymin1) {
		swap(ymax1, ymin1);
	}
	if (ymax2 < ymin2) {
		swap(ymax2, ymin2);
	}
	//如果以两线段为对角线的矩形不相交，则无交点
	if (p1.x > p4.x || p2.x < p3.x || ymax1 < ymin2 || ymin1 > ymax2) {
		return 0;
	}//下面进行跨立试验
	SPoint vs1  (p1.x - p3.x, p1.y - p3.y);
	SPoint vs2  (p2.x - p3.x, p2.y - p3.y);
	SPoint vt1  (p3.x - p1.x, p3.y - p1.y);
	SPoint vt2  (p4.x - p1.x, p4.y - p1.y);

	float s1v2, s2v2, t1v1, t2v1;
	//根据外积结果判定否交于线上
	if (Equal(s1v2 = vs1 ^ v2, 0) && p4 > p1 && p1 > p3) {
		Int = p1;
		return 2;
	}
	if (Equal(s2v2 = vs2 ^ v2, 0) && p4 > p2 && p2 > p3) {
		Int = p2;
		return 2;
	}
	if (Equal(t1v1 = vt1 ^ v1, 0) && p2 > p3 && p3 > p1) {
		Int = p3;
		return 2;
	}
	if (Equal(t2v1 = vt2 ^ v1, 0) && p2 > p4 && p4 > p1) {
		Int = p4;
		return 2;
	} //未交于线上，则判定是否相交
	if(s1v2 * s2v2 > 0 || t1v1 * t2v1 > 0) {
		return 0;
	} //以下为相交的情况，算法详见文档
	//计算二阶行列式的两个常数项
	float ConA = p1.x * v1.y - p1.y * v1.x;
	float ConB = p3.x * v2.y - p3.y * v2.x;
	//计算行列式D1和D2的值，除以系数行列式的值，得到交点坐标
	Int.x = (ConB * v1.x - ConA * v2.x) / Corss;
	Int.y = (ConB * v1.y - ConA * v2.y) / Corss;
	//正交返回1
	return 1;
}

void DrawLine(HDC surface,const SPoint& a,const SPoint &b)
{
		MoveToEx(surface,(LONG)a.x,(LONG)a.y,NULL);
		LineTo(surface,(LONG)b.x,(LONG)b.y);
}