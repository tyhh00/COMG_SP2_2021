#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include "Vertex.h"
#include "Material.h"

/******************************************************************************/
/*!
		Class Mesh:
\brief	To store VBO (vertex & color buffer) and IBO (index buffer)
*/
/******************************************************************************/
class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_TRIANGLE_FAN,
		DRAW_LINES,
		DRAW_LINE_STRIP,
		DRAW_LINES_ADJACENT,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	~Mesh();
	void Render();
	void Render(unsigned offset, unsigned count);

	//10a Practical
	static void SetMaterialLoc(unsigned kA, unsigned kD, unsigned kS, unsigned nS);
	std::vector<Material> materials;
	static unsigned locationKa;
	static unsigned locationKd;
	static unsigned locationKs;
	static unsigned locationNs;

	const std::string name;
	DRAW_MODE mode;

	Material material;
	unsigned vertexBuffer;
	unsigned colorBuffer;
	unsigned indexBuffer;
	unsigned indexSize;
	unsigned textureID;

	Vector3 *botLeftPos, *topRightPos, *meshCenter;
};

#endif