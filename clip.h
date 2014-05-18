#include <iostream>
using namespace std;
struct pt
{
	float x;
	float y;
	//   bool in;    //either inside or intersection point
	float loc;
	float loc2;
	pt(float _x, float _y)
	{
		x = _x;
		y = _y;
		loc = -1;
	}
	pt()
	{
		loc = -1;
	}
};



struct trgl
{
	//the first 3 points are the vertex
	//others are reserved forintersection points
	pt p[9];
};

struct instructSet
{
	bool doIns[12];
	instructSet()
	{
		for(int i = 0; i < 12; i++)
			doIns[i] = false;
	}
};
static instructSet stateSet[11];

void setStateInstr()
{
	stateSet[0].doIns[1] = true;

	stateSet[1].doIns[0] = true;
	stateSet[1].doIns[4] = true;

	stateSet[2].doIns[1] = true;
	stateSet[2].doIns[5] = true;

	stateSet[3].doIns[0] = true;
	stateSet[3].doIns[4] = true;
	stateSet[3].doIns[6] = true;

	stateSet[4].doIns[1] = true;
	stateSet[4].doIns[5] = true;
	stateSet[4].doIns[7] = true;

	stateSet[5].doIns[4] = true;
	stateSet[5].doIns[6] = true;
	stateSet[5].doIns[8] = true;

	stateSet[6].doIns[5] = true;
	stateSet[6].doIns[7] = true;
	stateSet[6].doIns[9] = true;

	stateSet[7].doIns[0] = true;
	stateSet[7].doIns[2] = true;
	stateSet[7].doIns[4] = true;
	stateSet[7].doIns[6] = true;

	stateSet[8].doIns[1] = true;
	stateSet[8].doIns[3] = true;
	stateSet[8].doIns[5] = true;
	stateSet[8].doIns[7] = true;

	stateSet[9].doIns[1] = true;
	stateSet[9].doIns[5] = true;
	stateSet[9].doIns[10] = true;
	stateSet[9].doIns[11] = true;

	stateSet[10].doIns[1] = true;
	stateSet[10].doIns[3] = true;
	stateSet[10].doIns[5] = true;
}

//int I(node *p1, node *p2, node *q1, node *q2,
//  float *alpha_p, float *alpha_q, int *xint, int *yint)
//this has to intersect where it is not the end of the lines
inline bool BIntersect(pt p1, pt p2, pt q1, pt q2)
{
	float  tp, tq, par;

	par = (float) ((p2.x - p1.x)*(q2.y - q1.y) -
		(p2.y - p1.y)*(q2.x - q1.x));

	if (!par) return 0;                               /* parallel lines */
	tp = ((q1.x - p1.x)*(q2.y - q1.y) - (q1.y - p1.y)*(q2.x - q1.x))/par;
	tq = ((p2.y - p1.y)*(q1.x - p1.x) - (p2.x - p1.x)*(q1.y - p1.y))/par;

	//touching the boundary is not inside
	if(tp<=0 || tp>=1 || tq<=0 || tq>=1) return 0;

	return 1;
}

//assumption is the coordinates are all positive
inline bool testInside(pt p, trgl t)
{
	bool inside = false;
	pt left( -999, p.y);//create(0, point->y, 0, 0, 0, 0, 0, 0, 0, 0.);
	for(int i = 0; i < 3; i++)
	{
		if(BIntersect(left, p, t.p[i], t.p[(i+1)%3]))
			inside = !inside;
	}
	return inside;
}

inline void swap(pt &p1, pt &p2)
{
	pt tmp;
	tmp = p1;
	p1 = p2;
	p2 = tmp;
}

inline float cross(float ax, float ay, float bx, float by)
{ 
    return ax*by - ay*bx; 
}

//touching boundary is also intersect
inline bool BIntersectIncludeBoundary(pt p1, pt p2, pt q1, pt q2)
{
	float  tp, tq, par;

	par = (float) ((p2.x - p1.x)*(q2.y - q1.y) -
		(p2.y - p1.y)*(q2.x - q1.x));

	if (!par) return 0;                               /* parallel lines */
	tp = ((q1.x - p1.x)*(q2.y - q1.y) - (q1.y - p1.y)*(q2.x - q1.x))/par;
	tq = ((p2.y - p1.y)*(q1.x - p1.x) - (p2.x - p1.x)*(q1.y - p1.y))/par;

	//touching the boundary is not inside
	if(tp<0 || tp>1 || tq<0 || tq>1) return 0;

	return 1;
}

inline void Intersect(pt p1, pt p2, pt q1, pt q2,
					  pt &pi, pt &qi)
{
	float tp, tq, par;

	par = (float) ((p2.x - p1.x)*(q2.y - q1.y) -
		(p2.y - p1.y)*(q2.x - q1.x));

	if (!par)
		return;                               /* parallel lines */

	tp = ((q1.x - p1.x)*(q2.y - q1.y) - (q1.y - p1.y)*(q2.x - q1.x))/par;
	tq = ((p2.y - p1.y)*(q1.x - p1.x) - (p2.x - p1.x)*(q1.y - p1.y))/par;

	if(tp<0 || tp>1 || tq<0 || tq>1)
		return;

	//    pi.in = true;
	//    qi.in = true;
	pi.x = p1.x + tp*(p2.x - p1.x);
	pi.y = p1.y + tp*(p2.y - p1.y);
	qi.x = pi.x;
	qi.y = pi.y;

	//this can be replaced with tp and tq with care
	pi.loc = tp;// dist(p1.x, p1.y, x, y) / dist(p1.x, p1.y, p2.x, p2.y);
	qi.loc = tq;// dist(q1.x, q1.y, x, y) / dist(q1.x, q1.y, q2.x, q2.y);
}

inline void AddIntersection(trgl ts, trgl tc, pt *clipped_array, int &clipped_cnt)
{
	for(int ic = 0; ic < 3; ic++)
	{
		for(int is = 0; is < 3; is++)
		{
			pt insect_s, insect_c;
			Intersect(tc.p[ic], tc.p[(ic+1)%3], ts.p[is], ts.p[(is+1)%3 ],
				insect_c, insect_s);

			if(insect_c.loc >= 0)
			{
				insect_c.loc += ic;
				if(clipped_cnt > 0)
				{
					if(insect_c.loc > clipped_array[clipped_cnt - 1].loc)
						clipped_array[clipped_cnt++] = insect_c;
					else if(insect_c.loc < clipped_array[clipped_cnt - 1].loc)
					{
						clipped_array[clipped_cnt] = clipped_array[clipped_cnt - 1];
						clipped_array[clipped_cnt - 1] = insect_c;
						clipped_cnt++;
					}
					//else :insect_c.loc == clipped_vert[isect_cnt - 1].loc
					//don't add anything
				}
				else
				{
					clipped_array[0] = insect_c;
					clipped_cnt++;
				}
			}
		}
	}
}

void ClipMerge(float a1x, float a1y, float a2x, float a2y, float a3x, float a3y, 
			   float b1x, float b1y, float b2x, float b2y, float b3x, float b3y,
			   float &c1x, float &c1y, float &c2x, float &c2y, float &c3x, float &c3y,
			   float &c4x, float &c4y, float &c5x, float &c5y, float &c6x, float &c6y,
			   int &c_nv)
{
	trgl ts, tc;

	ts.p[0].x = a1x;
	ts.p[0].y = a1y;
	ts.p[1].x = a2x;
	ts.p[1].y = a2y;
	ts.p[2].x = a3x;
	ts.p[2].y = a3y;

	tc.p[0].x = b1x;
	tc.p[0].y = b1y;
	tc.p[1].x = b2x;
	tc.p[1].y = b2y;
	tc.p[2].x = b3x;
	tc.p[2].y = b3y;


	bool dir_s = cross(	ts.p[2].x - ts.p[0].x, ts.p[2].y - ts.p[0].y,
						ts.p[1].x - ts.p[0].x, ts.p[1].y - ts.p[0].y) > 0;
	bool dir_c = cross(	tc.p[2].x - tc.p[0].x, tc.p[2].y - tc.p[0].y,
						tc.p[1].x - tc.p[0].x, tc.p[1].y - tc.p[0].y) > 0;
	if(dir_s != dir_c)	{
		swap(tc.p[0], tc.p[1]);
		//cout<<"swapped..."<<endl;
	}

	//mark inside or outside for the triangle vertices
	//and count the number of inside vertices
	pt interPts[8];
	pt clipped[8];
	int clippedCnt = 0;
	int interPtsCnt = 0;
	for(int i = 0; i < 3; i++)	{
		tc.p[i].loc = testInside(tc.p[i], ts);
		ts.p[i].loc = testInside(ts.p[i], tc);
	}

	if(tc.p[0].loc > 0 && tc.p[1].loc > 0 && tc.p[2].loc > 0)	{
		clipped[clippedCnt++] = tc.p[0];
		clipped[clippedCnt++] = tc.p[1];
		clipped[clippedCnt++] = tc.p[2];
	} else if(ts.p[0].loc > 0 && ts.p[1].loc > 0 && ts.p[2].loc > 0)	{
		clipped[clippedCnt++] = ts.p[0];
		clipped[clippedCnt++] = ts.p[1];
		clipped[clippedCnt++] = ts.p[2];
	} else {
		for(int ic = 0; ic < 3; ic++)	{
			for(int is = 0; is < 3; is++)	{
				pt insect_s, insect_c;
				Intersect(tc.p[ic], tc.p[(ic+1)%3], ts.p[is], ts.p[(is+1)%3 ],
					insect_c, insect_s);
			
				if(insect_c.loc >= 0)	{
					insect_c.loc += ic;
					insect_c.loc2 = insect_s.loc + is;
					interPts[interPtsCnt++] = insect_c;
				}
			}
		}

		//bool sIsCCW = false;

		//if(interPtsCnt > 1)
		//	sIsCCW = ((interPts[0].loc2 - interPts[1].loc2) * (interPts[0].loc - interPts[1].loc) < 0);
		//cout<<"sIsCCW :"<<sIsCCW <<endl;

		//for(int i = 0 ; i < interPtsCnt; i++)	{
		//	cout<<interPts[i].loc<<"\t";
		//}
		//cout<<endl;
		for(int i = 0 ; i < interPtsCnt; i++)	{

			int inext = (i+1) % interPtsCnt;
			//if the two intersection points are reversed, the last one is never reversed
			if(interPts[i].loc > interPts[inext].loc && inext != 0)
				swap(interPts[i], interPts[inext]);
			float loc1 = interPts[i].loc;
			float loc2 = interPts[inext].loc;
			clipped[clippedCnt++] = interPts[i];
			//in case loc2 is in the next round
			if(loc2 < loc1)
				loc2 += 3;
			//add inside vertices from c
			for(int j = ceil(loc1); j <= floor(loc2); j++ )	{
				if(tc.p[j % 3].loc > 0)
					clipped[clippedCnt++] = tc.p[j % 3];
			}

			//for the triangle s
			loc1 = interPts[i].loc2;
			loc2 = interPts[inext].loc2;

			if(loc2 < loc1)
				loc2 += 3;
			for(int j = ceil(loc1); j <= floor(loc2); j++ )	{
				if(ts.p[j % 3].loc > 0)
					clipped[clippedCnt++] = ts.p[j % 3];
			}
		}
	}


	//std::cout<<clippedCnt<<"\t";

	c1x = clipped[0].x;
	c1y = clipped[0].y;
	c2x = clipped[1].x;
	c2y = clipped[1].y;
	c3x = clipped[2].x;
	c3y = clipped[2].y;
	c4x = clipped[3].x;
	c4y = clipped[3].y;
	c5x = clipped[4].x;
	c5y = clipped[4].y;
	c6x = clipped[5].x;
	c6y = clipped[5].y;
	c_nv = clippedCnt;

}


void ClipTriangle(float a1x, float a1y, float a2x, float a2y, float a3x, float a3y, 
				  float b1x, float b1y, float b2x, float b2y, float b3x, float b3y,
				  float &c1x, float &c1y, float &c2x, float &c2y, float &c3x, float &c3y,
				  float &c4x, float &c4y, float &c5x, float &c5y, float &c6x, float &c6y,
				  int &c_nv)
{
	//   AddPoints();

	trgl ts, tc;

	int i = 0;
	ts.p[0].x = a1x;
	ts.p[0].y = a1y;
	ts.p[1].x = a2x;
	ts.p[1].y = a2y;
	ts.p[2].x = a3x;
	ts.p[2].y = a3y;

	tc.p[0].x = b1x;
	tc.p[0].y = b1y;
	tc.p[1].x = b2x;
	tc.p[1].y = b2y;
	tc.p[2].x = b3x;
	tc.p[2].y = b3y;

	//mark inside or outside for the triangle vertices
	//and count the number of inside vertices
	setStateInstr();

	i = 0;
	//mark inside or outside for the triangle vertices
	//and count the number of inside vertices
	int cnt_in_s = 0, cnt_in_c = 0;
	for(i = 0; i < 3; i++)
	{
		if(tc.p[i].loc = testInside(tc.p[i], ts))
			cnt_in_c++;

		if(ts.p[i].loc = testInside(ts.p[i], tc))
			cnt_in_s++;
	}

	//make the "in" vertices in the front of the array
	int a[3] = {0, 1, 0};
	for(i = 0; i < 3; i++)
	{
		int idx = a[i];
		if(!tc.p[idx].loc && tc.p[idx + 1].loc)
			swap(tc.p[idx], tc.p[idx + 1]);
		if(!ts.p[idx].loc && ts.p[idx + 1].loc)
			swap(ts.p[idx], ts.p[idx + 1]);
	}

	bool test;
	if(1 == cnt_in_c && 1 == cnt_in_s)
	{
		//  test1 = BIntersectIncludeBoundary(ts.p[1], ts.p[2], tc.p[1], tc.p[2]);
		//if(test1)
		test = BIntersectIncludeBoundary(ts.p[1], ts.p[2], tc.p[0], tc.p[1]);
	}

	int state = -1;
	if(0 == cnt_in_c && 0 == cnt_in_s)
		state = 0;
	else if(0 == cnt_in_c && 1 == cnt_in_s)
		state = 1;
	else if(1 == cnt_in_c && 0 == cnt_in_s)
		state = 2;
	else if(0 == cnt_in_c && 2 == cnt_in_s)
		state = 3;
	else if(2 == cnt_in_c && 0 == cnt_in_s)
		state = 4;
	else if(0 == cnt_in_c && 3 == cnt_in_s)
		state = 5;
	else if(3 == cnt_in_c && 0 == cnt_in_s)
		state = 6;
	else if(1 == cnt_in_c && 2 == cnt_in_s)
		state = 7;
	else if(2 == cnt_in_c && 1 == cnt_in_s)
		state = 8;
	else if(1 == cnt_in_c && 1 == cnt_in_s && !test)
		state = 9;
	else// if(1 == cnt_in_c && 1 == cnt_in_s && !test1) and (1 == cnt_in_c && 1 == cnt_in_s && test1 && test2)
		state = 10;
	//+cs

	pt clipped_array[6];

	int clipped_cnt = 0;
	instructSet is = stateSet[state];
	if(is.doIns[0])//+sc
		AddIntersection(tc, ts, clipped_array, clipped_cnt);
	if(is.doIns[1])//+cs
		AddIntersection(ts, tc, clipped_array, clipped_cnt);
	if(is.doIns[2])//+c0-
	{
		clipped_array[clipped_cnt] = clipped_array[clipped_cnt - 1];
		clipped_array[clipped_cnt - 1] = tc.p[0];
		clipped_cnt++;
	}
	if(is.doIns[3])//+s0-
	{
		clipped_array[clipped_cnt] = clipped_array[clipped_cnt - 1];
		clipped_array[clipped_cnt - 1] = ts.p[0];
		clipped_cnt++;
	}
	if(is.doIns[4])//+s0
		clipped_array[clipped_cnt++] = ts.p[0];
	if(is.doIns[5])//+c0
		clipped_array[clipped_cnt++] = tc.p[0];
	if(is.doIns[6])//+s1
		clipped_array[clipped_cnt++] = ts.p[1];
	if(is.doIns[7])//+c1
		clipped_array[clipped_cnt++] = tc.p[1];
	if(is.doIns[8])//+s2
		clipped_array[clipped_cnt++] = ts.p[2];
	if(is.doIns[9])//+c2
		clipped_array[clipped_cnt++] = tc.p[2];
	if(is.doIns[10])//+r0
		clipped_array[clipped_cnt++] = clipped_array[0];
	if(is.doIns[11])//+r0_s0
		clipped_array[0] = ts.p[0];

	c1x = clipped_array[0].x;
	c1y = clipped_array[0].y;
	c2x = clipped_array[1].x;
	c2y = clipped_array[1].y;
	c3x = clipped_array[2].x;
	c3y = clipped_array[2].y;
	c4x = clipped_array[3].x;
	c4y = clipped_array[3].y;
	c5x = clipped_array[4].x;
	c5y = clipped_array[4].y;
	c6x = clipped_array[5].x;
	c6y = clipped_array[5].y;
	c_nv = clipped_cnt;

}