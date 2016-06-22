#include <cmath>
#include <random>
#include "common.h"
#include<assert.h>
const double eps = 1e-6;
//particle obj[1001];
const double inf = 1000;
//参数 
int obj_num;//分子个数 
double d;//刷新间隔 
double w, h;//半宽和半高 

double n_time[1001];
int n_id[1001];

void collide(particle& a, particle& b)//碰撞 
{
	auto x1 = a.pos, x2 = b.pos;
	auto v1 = a.v, v2 = b.v;
	a.v = v1 - (x1 - x2)*(((v1 - v2)*(x1 - x2)) / ((x1 - x2)*(x1 - x2)));
	b.v = v2 - (x2 - x1)*(((v1 - v2)*(x1 - x2)) / ((x2 - x1)*(x2 - x1)));
	return;
}
void collide(particle& o, int wall)//上1右2下3左4 
{
	if (wall % 2)
		o.v.y = -o.v.y;
	else
		o.v.x = -o.v.x;
	return;
}
double nxtime(const particle& a, const particle& b)//下一个碰撞时间 
{
	double A = (a.v - b.v)*(a.v - b.v);
	double B = (a.pos - b.pos)*(a.v - b.v)*2.0;
	double C = (a.pos - b.pos)*(a.pos - b.pos) - 4.0*pRadius*pRadius;
	double delta = B*B - 4.0*A*C;
	if (delta<0) return -1.0;
	else 
	{
		if(C<-eps) return eps;
		return (-B - sqrt(delta)) / (2.0*A);//也许有bug 	
	}
}
double nxtime(const particle& o, int wall)//上1右2下3左4
{
	if (wall % 2)
	{
		//double p = (wall == 1 ? h : -h);
		//return (h - o.pos.y) / o.v.y;
		double p = (wall == 1 ? h-pRadius : 0+pRadius);
		if(h-pRadius>=o.pos.y && o.pos.y>=0+pRadius) return (p- o.pos.y) / o.v.y;
		else return eps;
	}
	else
	{
		//double p = (wall == 2 ? w : -w);
		//return (h - o.pos.x) / o.v.x;
		double p = (wall == 2 ? w-pRadius : 0+pRadius);
		if(w-pRadius>=o.pos.x && o.pos.x>=0+pRadius) return (p-  o.pos.x) / o.v.x;
		else return eps;
	}
}
void cal(int o)
{
	n_time[o] = inf;
	for (int i = 1; i <= 4; i++)
	{
		//double ans = nxtime(obj[o], i);
		double ans = nxtime(Particles[o], i);
		if (ans>0 && n_time[o]>ans)
		{
			n_time[o] = ans;
			n_id[o] = -i;
		}
	}
	for (int i = 0; i<obj_num; i++)
		if (i != o)
		{
			//double ans = nxtime(obj[o], obj[i]);
			double ans = nxtime(Particles[o], Particles[i]);
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
		if (t + min<=d)
		{
			//for (int i = 0; i<obj_num; i++) obj[i].pos = obj[i].pos + (obj[i].v*min);
			for (int i = 0; i<obj_num; i++) Particles[i].pos = Particles[i].pos + (Particles[i].v*min);
			if (o2<0)
			{
				//collide(obj[o1], -o2);
				collide(Particles[o1], -o2);
				for (int i = 0; i<obj_num; i++)
					if (i == o1 || n_id[i] == o1)
						cal(i);
					else n_time[i]-=min;
			}
			else
			{
				//collide(obj[o1], obj[o2]);
				collide(Particles[o1], Particles[o2]);
				for (int i = 0; i<obj_num; i++)
					if (i == o1 || n_id[i] == o1 || i == o2 || n_id[i] == o2)
						cal(i);
					else n_time[i]-=min;
			}
			t += min;
		}
		else break;
	}
	double tt = d - t;
	for (int i = 0; i<obj_num; i++)
	{
		//obj[i].pos = obj[i].pos + (obj[i].v*tt);
		Particles[i].pos = Particles[i].pos + (Particles[i].v*tt);
		n_time[i] -= tt;
	}
	return;
}

bool collideWithObstacle(const Point &p)
{
	for (auto &now : Obstacles)
	{
		for (int i = 0; i < now.size() - 1; i++)
		{
			Vector va = now[i] - p, vb = now[i + 1] - p;
			double len = (now[i + 1] - now[i]).length();
			if (len > eps)
			{
				if (va.cross(vb) / len <= pRadius)
					return true;
			}
			else
			{
				if (va.length() <= pRadius)
					return true;
			}
		}
	}
	return false;
}
bool collideWithParticle(const Point &p)
{
	for (auto &now : Particles)
	{
		if ((p - now.pos).length() <= 2 * pRadius)
			return true;
	}
	return false;
}
void initParticles(int size, double temperature)
{
	static const int tryTimes = 100;
	static std::default_random_engine generator;
	std::uniform_real_distribution<double> dis(pRadius, SceneSize - pRadius);
	std::normal_distribution<double> dis_vec(0, sqrt(temperature));
	Particles.clear();
	for (int idx = 0; idx < size; idx++)
	{
		for (int kase = 0; kase < tryTimes; kase++)
		{
			particle p;
			p.pos = Point(dis(generator), dis(generator));
			if (!collideWithObstacle(p.pos) && !collideWithParticle(p.pos))
			{
				p.v = Vector(dis_vec(generator), dis_vec(generator));
				Particles.push_back(p);
				break;
			}
		}
	}
	w = h = SceneSize;
	obj_num = Particles.size();
	for (int i = 0; i < Particles.size(); i++)
		cal(i);
}

void onTick(double interval)
{
	d = interval;
	w = h = SceneSize;
	obj_num = Particles.size();
	timefly();
}
