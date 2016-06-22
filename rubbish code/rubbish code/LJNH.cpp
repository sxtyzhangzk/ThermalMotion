#include<cstdio>
#include<cmath>
using namespace std;
struct vec
{
	double a;
	double b;
	vec operator-(const vec& o) const
	{
		vec ret;
		ret.a = a - o.a;
		ret.b = b - o.b;
		return ret;
	}
	vec operator+(const vec& o) const
	{
		vec ret;
		ret.a = a + o.a;
		ret.b = b + o.b;
		return ret;
	}
	double operator*(const vec& o) const
	{
		return a*o.a + b*o.b;
	}
	vec operator*(double o) const
	{
		vec ret;
		ret.a = a*o;
		ret.b = b*o;
		return ret;
	}
};
struct pt
{
	vec x;
	vec v;
};
pt obj[1001];
const double inf = 1000;
//参数 
int obj_num;//分子个数 
double d;//刷新间隔 
double w, h;//半宽和半高 
double r;//分子半径 
		 //参数end

double n_time[1001];
int n_id[1001];
void collide(pt& a, pt& b)//碰撞 
{
	vec x1 = a.x, x2 = b.x;
	vec v1 = a.v, v2 = b.v;
	a.v = v1 - (x1 - x2)*(((v1 - v2)*(x1 - x2)) / ((x1 - x2)*(x1 - x2)));
	b.v = v2 - (x2 - x1)*(((v1 - v2)*(x1 - x2)) / ((x2 - x1)*(x2 - x1)));
	return;
}
void collide(pt& o, int wall)//上1右2下3左4 
{
	if (wall % 2)
		o.v.b = -o.v.b;
	else
		o.v.a = -o.v.a;
	return;
}
double nxtime(const pt& a, const pt& b)//下一个碰撞时间 
{
	double A = (a.v - b.v)*(a.v - b.v);
	double B = (a.x - b.x)*(a.v - b.v)*2.0;
	double C = (a.x - b.x)*(a.x - b.x) - 4.0*r*r;
	double delta = B*B - 4.0*A*C;
	if (delta<0) return -1.0;
	else return (-B - sqrt(delta)) / (2.0*A);//也许有bug 	
}
double nxtime(const pt& o, int wall)//上1右2下3左4
{
	if (wall % 2)
	{
		double p = (wall == 1 ? h : -h);
		return (h - o.x.b) / o.v.b;
	}
	else
	{
		double p = (wall == 2 ? w : -w);
		return (h - o.x.a) / o.v.a;
	}
}
void cal(int o)
{
	n_time[o] = inf;
	for (int i = 1; i <= 4; i++)
	{
		double ans = nxtime(obj[o], i);
		if (ans>0 && n_time[o]>ans)
		{
			n_time[o] = ans;
			n_id[o] = -i;
		}
	}
	for (int i = 0; i<obj_num; i++)
		if (i != o)
		{
			double ans = nxtime(obj[o], obj[i]);
			if (ans>0 && n_time[o]>ans)
			{
				n_time[o] = ans;
				n_id[o] = i;
			}
		}
	return;
}
void timefly()//下一个时间点的各个点的坐标 
{
	double t = 0;
	while (1)
	{
		double min = inf;
		int o1, o2;
		for (int i = 0; i<obj_num; i++)
			if (min>n_time[i])
			{
				min = n_time[i];
				o1 = i;
				o2 = n_id[i];
			}
		if (t + min<d)
		{
			for (int i = 0; i<obj_num; i++) obj[i].x = obj[i].x + (obj[i].v*min);
			if (o2<0)
			{
				collide(obj[o1], -o2);
				for (int i = 0; i<obj_num; i++)
					if (i == o1 || n_id[i] == o1)
						cal(i);
			}
			else
			{
				collide(obj[o1], obj[o2]);
				for (int i = 0; i<obj_num; i++)
					if (i == o1 || n_id[i] == o1 || i == o2 || n_id[i] == o2)
						cal(i);
			}
			t += min;
		}
		else break;
	}
	double tt = d - t;
	for (int i = 0; i<obj_num; i++)
	{
		obj[i].x = obj[i].x + (obj[i].v*tt);
		n_time[i] -= tt;
	}
	return;
}

