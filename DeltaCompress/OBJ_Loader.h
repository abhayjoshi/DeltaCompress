#ifndef __OBJ_LOADER__
#define __OBJ_LOADER__

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class OBJ_vertex{
public:
	OBJ_vertex(){}
	~OBJ_vertex(){}

	float GetX() const {return(x);}
	void SetX(float _x) {x = _x;}

	float GetY() const {return(y);}
	void SetY(float _y) {y = _y;}

	float GetZ() const {return(z);}
	void SetZ(float _z) {z = _z;}

private:
	float	x;
	float	y;
	float	z;
};

class uv_coords{
public:
	uv_coords(){}
	~uv_coords(){}

	float GetU() const {return(u);}
	void SetU(float _u) {u = _u;}

	float GetV() const {return(v);}
	void SetV(float _v) {v = _v;}

private:
	float	u;
	float	v;
};

class face{
public:
	face();
	face(const face &right);
	~face();

	long GetNumVertices() const;
	long GetNumNormals() const;
	long GetNumUVs() const;

	void AddVertex(long index);
	long GetVertex(long index) const;

	void AddNormal(long index);
	long GetNormal(long index) const;

	void AddUV(long index);
	long GetUV(long index) const;

private:
	vector<long>		*vertices;
	vector<long>		*normals;
	vector<long>		*uv;
};

class OBJ{
public:
	OBJ();
	~OBJ();

	long GetNumVertices() const;
	long GetNumNormals() const;
	long GetNumUVs() const;
	long GetNumFaces() const;

	OBJ_vertex *GetVertex(long index) const;
	OBJ_vertex *GetNormal(long index) const;
	uv_coords *GetUV(long index) const;
	face *GetFace(long index) const;

	void LoadOBJ(string file_name);
	void DumpOBJ(void);

private:
	vector<OBJ_vertex>	*vertices;
	vector<OBJ_vertex>	*normals;
	vector<uv_coords>	*uv;
	vector<face>		*faces;
};

#endif