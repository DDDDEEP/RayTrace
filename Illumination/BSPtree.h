#pragma once


class Bnode
{
public:

	//data
	int objIndex; //obj �ļ��±�
	int planeIndex; //ƽ���±�
	Bnode* left;
	Bnode* right;

	//methods
	Bnode();
	Bnode(int, int);
	~Bnode();

	

};

class BSPtree
{
public:

	Bnode root;


	BSPtree(vector<ObjLoader>&);
	~BSPtree();

	void m_sort(vector<pair<int, int>>& ps, vector<ObjLoader>& objs);

	//void sort(vector<pair<int, int>>& ps, vector<ObjLoader>& objs);
	//void sort(const vector<pair<int, int>>& ps, vector<ObjLoader>& objs);

	

	
	vector<ObjLoader>& objs;
};

