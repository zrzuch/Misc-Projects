/*
  FileName    : Mesh.h
  Author      : Zachary Zuch
  Course      : CSCI 375
  Assignment  : Lab 3
  Description : This file is the mesh soure file which has the implementation
  				of the draw calls and vbo and vao creation and management.
*/

#include <vector>
#include <iterator>
#include <string>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "ShaderProgram.h"
#include "Mesh.h"
#include "Camera.h"
#include "Transform.h"
#include "AiScene.h"

// Initialize the mesh. Generate a VAO and VBO.
Mesh::Mesh ()
	: m_vao(), m_vbo(), m_ibo()
	, m_vertexData(), m_indices(), m_world()
{
	 glGenVertexArrays ( 1, &m_vao );
	 glGenBuffers ( 1, &m_vbo );
	 glGenBuffers ( 1, &m_ibo );		
}

// Free allocated resources. Delete generated VAO and VBO. 
Mesh::~Mesh ()
{
	glDeleteVertexArrays ( 1, &m_vao );
	glDeleteBuffers ( 1, &m_vbo );
	glDeleteBuffers ( 1, &m_ibo );
}

// Read vertex data from a file using the AiScene class. 
void
Mesh::importFromFile (const std::string& filename)
{
	AiScene scene(filename);
	std::vector<float> vertexData = scene.getAllVertexData();
	std::vector<unsigned> indices = scene.getAllFaceIndices();
	addGeometry(vertexData, indices);
}

void
Mesh::reset()
{
	m_world.reset();
}

// Key: "1"
void
Mesh::moveRight (float distance)
{
	m_world.moveRight(distance);
}

// Key: "2"
void
Mesh::moveUp (float distance)
{
	m_world.moveUp(distance);
}

void
Mesh::moveBack (float distance)
{
	m_world.moveBack(distance);
}

void
Mesh::moveLocal (float distance, const Vector3& localDirection)
{
	m_world.moveLocal( distance, localDirection );
}

// Key: "3"
void
Mesh::moveWorld (float distance, const Vector3& worldDirection)
{
	m_world.moveWorld( distance, worldDirection );
}

// Key: "4"
void
Mesh::pitch (float angleDegrees)
{
	m_world.pitch(angleDegrees);
}

// Key: "5"
void
Mesh::yaw (float angleDegrees)
{
	m_world.yaw(angleDegrees);
}

// Key: "6"
void
Mesh::roll (float angleDegrees)
{
	m_world.roll(angleDegrees);
}

void
Mesh::rotateLocal (float angleDegrees, const Vector3& axis)
{
	m_world.rotateLocal( angleDegrees, axis );
}

// Key: "7"
void
Mesh::alignWithWorldY ()
{
	m_world.alignWithWorldY();
}

// Key: "8"
void
Mesh::scaleLocal (float scale)
{
	m_world.scaleLocal(scale);
}

void
Mesh::scaleLocal (float scaleX, float scaleY, float scaleZ)
{
	m_world.scaleLocal( scaleX, scaleY, scaleZ );
}
    
// Key: "9"
void
Mesh::scaleWorld (float scale)
{
	m_world.scaleWorld(scale);
}

void
Mesh::scaleWorld (float scaleX, float scaleY, float scaleZ)
{
	m_world.scaleWorld( scaleX, scaleY, scaleZ );
}

// Key: "0"
void
Mesh::shearLocalXByYz (float shearY, float shearZ)
{
	m_world.shearLocalXByYz( shearY, shearZ );
}

void
Mesh::shearLocalYByXz (float shearX, float shearZ)
{
	m_world.shearLocalYByXz( shearX, shearZ );
}

void
Mesh::shearLocalZByXy (float shearX, float shearY)
{
	m_world.shearLocalZByXy( shearX, shearY );
}

void
Mesh::addGeometry (const std::vector<float>& vertices, 
	const std::vector<unsigned>& indices)
{
	unsigned beginSize = m_indices.size();

	m_indices.insert(std::end(m_indices), 
		std::begin(indices), std::end(indices));

	if ( m_vertexData.size() != 0 )
	{
		for (unsigned i = beginSize; i < m_indices.size(); ++i)
		{
			m_indices[i] += m_vertexData.size();
		}	
	}
	
	m_vertexData.insert(std::end(m_vertexData), 
		std::begin(vertices), std::end(vertices));
}

// Set the state of the VAO for rendering.
// Bind VAO, VBO, buffer the data, etc.
void
Mesh::prepareVao ()
{
	static const GLint POSITION_ATTRIB_INDEX = 0;
  	static const GLint NORMAL_ATTRIB_INDEX = 1;

	glBindVertexArray ( m_vao );

	glBindBuffer ( GL_ARRAY_BUFFER, m_vbo );
	glBufferData ( GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(float),
		m_vertexData.data(), GL_STATIC_DRAW );

	glEnableVertexAttribArray (POSITION_ATTRIB_INDEX);
	glVertexAttribPointer (POSITION_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 
		FLOATS_PER_VERTEX * sizeof(float), reinterpret_cast<void*> (0));

	glEnableVertexAttribArray (NORMAL_ATTRIB_INDEX);
 	glVertexAttribPointer (NORMAL_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 
 		FLOATS_PER_VERTEX * sizeof(float), 
 		reinterpret_cast<void*> ( 3 * sizeof(float)));

 	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_ibo );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned),
		m_indices.data(), GL_STATIC_DRAW );

	glBindVertexArray (0);
}

// Enable the shader program --> Set the model-view and projection uniform
// --> Bind VAO --> DrawArrays --> Unbind --> Disable the shader program 
void
Mesh::draw ( ShaderProgram* g_shaderProgram, const Camera& camera )
{
	Transform worldView = camera.getViewMatrix();
	worldView.combine(m_world);

	g_shaderProgram->enable ();
	g_shaderProgram->setUniformMatrix ( "uProjection", camera.getProjectionMatrix());
	g_shaderProgram->setUniformMatrix ( "uModelView", worldView.getTransform());
	
	glBindVertexArray(m_vao);
	glDrawElements ( GL_TRIANGLES, m_indices.size (), GL_UNSIGNED_INT, 
		reinterpret_cast<void*> (0));
	glBindVertexArray(0);

	g_shaderProgram->disable ();
}