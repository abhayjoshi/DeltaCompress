#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "OBJ_Loader.h"

face::face()
{
	vertices = new vector<long>;
	normals = new vector<long>;
	uv = new vector<long>;
}

face::face(const face &right)
{
	long	i;

	vertices = new vector<long>;
	normals = new vector<long>;
	uv = new vector<long>;

	for(i = 0;i < right.vertices->size();i++)
		vertices->push_back(right.vertices->at(i));
	for(i = 0;i < right.normals->size();i++)
		normals->push_back(right.normals->at(i));
	for(i = 0;i < right.uv->size();i++)
		uv->push_back(right.uv->at(i));
}

face::~face()
{
	delete vertices;
	vertices = NULL;

	delete normals;
	normals = NULL;

	delete uv;
	uv = NULL;
}

long face::GetNumVertices() const
{
	return(vertices->size());
}

long face::GetNumNormals() const
{
	return(normals->size());
}

long face::GetNumUVs() const
{
	return(uv->size());
}

void face::AddVertex(long index)
{
	vertices->push_back(index);
}

long face::GetVertex(long index) const
{
	if(index < 0 || index > vertices->size() - 1)
		return(NULL);

	return(vertices->at(index));
}

void face::AddNormal(long index)
{
	normals->push_back(index);
}

long face::GetNormal(long index) const
{
	if(index < 0 || index > normals->size() - 1)
		return(NULL);

	return(normals->at(index));
}

void face::AddUV(long index)
{
	uv->push_back(index);
}

long face::GetUV(long index) const
{
	if(index < 0 || index > uv->size() - 1)
		return(NULL);

	return(uv->at(index));
}

OBJ::OBJ()
{
	vertices = new vector<OBJ_vertex>;
	normals = new vector<OBJ_vertex>;
	uv = new vector<uv_coords>;
	faces = new vector<face>;
}

OBJ::~OBJ()
{
	delete vertices;
	vertices = NULL;

	delete normals;
	normals = NULL;

	delete uv;
	uv = NULL;

	delete faces;
	faces = NULL;
}

long OBJ::GetNumVertices() const
{
	return(vertices->size());
}

long OBJ::GetNumNormals() const
{
	return(normals->size());
}

long OBJ::GetNumUVs() const
{
	return(uv->size());
}

long OBJ::GetNumFaces() const
{
	return(faces->size());
}

OBJ_vertex *OBJ::GetVertex(long index) const
{
	if(index < 0 || index > vertices->size() - 1)
		return(NULL);

	return(&vertices->at(index));
}

OBJ_vertex *OBJ::GetNormal(long index) const
{
	if(index < 0 || index > normals->size() - 1)
		return(NULL);

	return(&normals->at(index));
}

uv_coords *OBJ::GetUV(long index) const
{
	if(index < 0 || index > uv->size() - 1)
		return(NULL);

	return(&uv->at(index));
}

face *OBJ::GetFace(long index) const
{
	if(index < 0 || index > faces->size() - 1)
		return(NULL);

	return(&faces->at(index));
}

void OBJ::LoadOBJ(string file_name)
{
	FILE		*fs;
	char		c;
	bool		done,has_normals,has_uvs;
	OBJ_vertex	vert;
	uv_coords	uvs;
	face		*temp_face;
	float		x,y,z;
	float		u,v;
	int			vert_ind,norm_ind,uv_ind;
	int			read_count;

	if(fopen_s(&fs,file_name.c_str(),"r") != 0)
		return;

	done = false;
	has_normals = false;
	has_uvs = false;

	while(!done)
	{
		c = fgetc(fs);
		switch(c)
		{
			case '#': // Comment. Fall through
			case 'u': // Fall through
			case 's': // Fall through
			case 'g': // Group. Not supported. Fall through
				while(fgetc(fs) != '\n'); // Skip to the next line
			break;
			case EOF: // End of file reached. We be done.
				done = true;
			break;
			case 'v': // Loading vertices is easy. Faces, not so much...
				c = fgetc(fs); // The next character determines what type of vertex we are loading
				switch(c)
				{
					case ' ': // Loading vertices
						fscanf_s(fs,"%f %f %f\n",&x,&y,&z);
						vert.SetX(x);
						vert.SetY(y);
						vert.SetZ(z);
						vertices->push_back(vert);
					break;
					case 'n': // Loading normals
						has_normals = true;
						fscanf_s(fs,"%f %f %f\n",&x,&y,&z);
						vert.SetX(x);
						vert.SetY(y);
						vert.SetZ(z);
						normals->push_back(vert);
					break;
					case 't': // Loading UVs
						has_uvs = true;
						fscanf_s(fs,"%f %f\n",&u,&v);
						uvs.SetU(u);
						uvs.SetV(v);
						uv->push_back(uvs);
					break;
					default: // Uh oh... What are we trying to read here? Someone screwed up their OBJ exporter...
						cout << "Invalid vertex type: " << "v" << c << " Should be of type \"v \", \"vn\" or \"vt\"." << endl;
						return;
				}
			break;
			case 'f':
				if(has_normals && has_uvs)
				{
					temp_face = new face;
					while(fgetc(fs) != '\n')
					{
						vert_ind = 0;
						norm_ind = 0;
						uv_ind = 0;

						read_count = fscanf_s(fs,"%d/%d/%d",&vert_ind,&uv_ind,&norm_ind);
						if((read_count != 3) || (vert_ind == 0) || (uv_ind == 0) || (norm_ind == 0)) // Valid indices start at 1 in a .OBJ file. Don't know why... O.o
						{
							delete temp_face;
							temp_face = NULL;

							cout << "Invalid vertex index." << endl;
							return;
						}

						temp_face->AddVertex(vert_ind - 1); // We want the indices to start at 0, so subtract 1.
						temp_face->AddNormal(norm_ind - 1);
						temp_face->AddUV(uv_ind - 1);
					}
					faces->push_back(*temp_face);

					delete temp_face;
					temp_face = NULL;
				}else if(has_normals && !has_uvs){
					temp_face = new face;
					while(fgetc(fs) != '\n')
					{
						vert_ind = 0;
						norm_ind = 0;

						read_count = fscanf_s(fs,"%d//%d",&vert_ind,&norm_ind);
						if((read_count != 2) || (vert_ind == 0) || (norm_ind == 0))
						{
							delete temp_face;
							temp_face = NULL;

							cout << "Invalid vertex index." << endl;
							return;
						}

						temp_face->AddVertex(vert_ind - 1);
						temp_face->AddNormal(norm_ind - 1);
						temp_face->AddUV(0);
					}
					faces->push_back(*temp_face);

					delete temp_face;
					temp_face = NULL;
				}else if(!has_normals && has_uvs){
					temp_face = new face;
					while(fgetc(fs) != '\n')
					{
						vert_ind = 0;
						uv_ind = 0;

						read_count = fscanf_s(fs,"%d/%d/",&vert_ind,&uv_ind);
						if((read_count != 2) || (vert_ind == 0) || (uv_ind == 0))
						{
							delete temp_face;
							temp_face = NULL;

							cout << "Invalid vertex index." << endl;
							return;
						}

						temp_face->AddVertex(vert_ind - 1);
						temp_face->AddNormal(0);
						temp_face->AddUV(uv_ind - 1);
					}
					faces->push_back(*temp_face);

					delete temp_face;
					temp_face = NULL;
				}else if(!has_normals && !has_uvs){
					temp_face = new face;
					while(fgetc(fs) != '\n')
					{
						vert_ind = 0;

						read_count = fscanf_s(fs,"%d//",&vert_ind);
						if((read_count != 1) || (vert_ind == 0))
						{
							delete temp_face;
							temp_face = NULL;

							cout << "Invalid vertex index." << endl;
							return;
						}

						temp_face->AddVertex(vert_ind - 1);
						temp_face->AddNormal(0);
						temp_face->AddUV(0);
					}
					faces->push_back(*temp_face);

					delete temp_face;
					temp_face = NULL;
				}
			break;
		}
	}

	fclose(fs);
}

void OBJ::DumpOBJ(void)
{
	long	i,j;

	for(i = 0;i < GetNumVertices();i++)
		cout << GetVertex(i)->GetX() << " " << GetVertex(i)->GetY() << " " << GetVertex(i)->GetZ() << endl;

	for(i = 0;i < GetNumNormals();i++)
		cout << GetNormal(i)->GetX() << " " << GetNormal(i)->GetY() << " " << GetNormal(i)->GetZ() << endl;

	for(i = 0;i < GetNumUVs();i++)
		cout << GetUV(i)->GetU() << " " << GetUV(i)->GetV() << endl;

	for(i = 0;i < GetNumFaces();i++)
	{
		for(j = 0;j < GetFace(i)->GetNumVertices();j++)
			cout << GetFace(i)->GetVertex(j) << "/" << GetFace(i)->GetUV(j) << "/" << GetFace(i)->GetNormal(j) << " ";
		cout << endl;
	}
}