#include "stdafx.h"
#include "BSPtree.h"


void m_qsort(vector< pair<int, int> >& a, vector<ObjLoader>& objs, int low, int high)
{
	if (low >= high)
	{
		return;
	}
	int ll = low;
	int rr = high;
	int key = ll;/*用字表的第一个记录作为枢轴*/

	
	double key_z = objs[a[key].first].planes[a[key].second].computeGravity().z;

	while (ll < rr)
	{
		while (ll < rr && objs[a[rr].first].planes[a[rr].second].computeGravity().z >= key_z /*a[last] >= a[key]*/)
		{
			--rr;
		}

		a[ll] = a[rr];/*将比第一个小的移到低端*/

		while (ll < rr && objs[a[ll].first].planes[a[ll].second].computeGravity().z <= key_z /*a[first] <= a[key]*/)
		{
			++ll;
		}

		a[rr] = a[ll];
		/*将比第一个大的移到高端*/
	}
	ll = key;/*枢轴记录到位*/
	m_qsort(a, objs, low, ll - 1);
	m_qsort(a, objs, ll + 1, high);
}

Bnode::Bnode()
{

}
Bnode::Bnode(int objIndex, int planeIndex)
{
	this->objIndex = objIndex;
	this->planeIndex = planeIndex;
}
Bnode::~Bnode()
{

}

/*
bool BSPtree::comp(const pair<int, int>& p1, const pair<int, int>& p2)
{
	Point gra1 = objs[p1.first].planes[p1.second].computeGravity();
	Point gra2 = objs[p2.first].planes[p2.second].computeGravity();
	return gra1.z - gra2.z;
}
*/






BSPtree::BSPtree(vector<ObjLoader>& objs):objs(objs)
{	
	vector< pair<int, int> > planeIndexPairs;
	
	for (int k = 0; k < objs.size(); ++k)
	{
		for (int k2 = 0; k2 < objs[k].planes.size(); ++k2)
		{
			planeIndexPairs.push_back(pair<int, int>(k, k2));
		}
	}

	//空树
	if (planeIndexPairs.size() == 0) return;

	//按面的z轴坐标排序
	m_sort(planeIndexPairs, objs);
	
	int s_ps = planeIndexPairs.size() / 2;



}


BSPtree::~BSPtree()
{
}

void BSPtree::m_sort(vector< pair<int, int> >& ps, vector<ObjLoader>& objs)
{
	m_qsort(ps, objs, 0, ps.size() - 1);

	//debug

	
	//cout << "after sort" << endl;
	//for (int k = 0; k < ps.size(); ++k)
	//{
	//	cout << objs[ps[k].first].planes[ps[k].second].computeGravity().z << endl;
	//}
	//cout << "end print" << endl;


}

