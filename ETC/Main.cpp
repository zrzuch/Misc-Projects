/* A beginning OpenGL program that uses OpenGL 3.3
     to draw a triangle. The triangle is now specified with 3D
     coordinates.
   A virtual camera has been defined and limited key-based
     movement has been added.
   Author: Gary M. Zoppetti, Ph.D.
*/

/*
  FileName    : Main.cpp
  Author      : Zachary Zuch
  Course      : CSCI 375
  Assignment  : Lab 4
  Description : This file is the main file to initialize GLEW and GLFW,
              and send draw calls.
*/

/******************************************************************/
// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

// Use GLEW so we can access the latest OpenGL functionality
// Always include GLEW before GLFW!
#include <GL/glew.h>
// Use GLFW so we can create windows and handle events in a
//   platform-independent way
#include <GLFW/glfw3.h>
#include <GL/glut.h>

// GLM is a header-only library, so we do not need to link to it
// It provides matrix classes and mathematical functions
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
// Need for "value_ptr", which allows us to get a pointer from a matrix
#include <glm/gtc/type_ptr.hpp>
// Transforms like lookat, perspective, rotate, scale, translate
#include <glm/gtc/matrix_transform.hpp>

/******************************************************************/
// Local includes
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Camera.h"
#include "KeyBuffer.h"
#include "Transform.h"

/******************************************************************/
// Type declarations/globals variables/prototypes

// Need a shader program to transform and light the primitives
ShaderProgram* g_shaderProgram;
Mesh* g_mesh;
Mesh* g_mesh2;
Camera g_camera;
KeyBuffer g_keyBuffer;

/******************************************************************/

void
init (GLFWwindow*& window);

void
initGlfw ();

void
initGlew ();

void
initWindow (GLFWwindow*& window);

void
resetViewport (GLFWwindow* window, int width, int height);

void
initScene ();

void
initShaders ();

void
drawScene (GLFWwindow* window);

void
onWindowSizeChange(GLFWwindow* window, int width, int height);

void
recordKeys (GLFWwindow* window, int key, int scanCode, int action,
       int modifiers);

void
processKeys (GLFWwindow* window);

void
releaseGlResources ();

void
outputGlfwError (int error, const char* description);

/******************************************************************/

int
main (int argc, char* argv[])
{
  GLFWwindow* window;
  init (window);

  // Game/render loop
  while (!glfwWindowShouldClose (window))
  {
    drawScene (window);
    // Process events in the event queue, which results in callbacks
    //   being invoked.
    glfwPollEvents ();
    processKeys (window);
  }

  releaseGlResources ();
  // Destroying the window destroys the OpenGL context
  glfwDestroyWindow (window);
  glfwTerminate ();

  return EXIT_SUCCESS;
}

/******************************************************************/

void
init (GLFWwindow*& window)
{
  // Always initialize GLFW before GLEW
  initGlfw ();
  initWindow (window);
  initGlew ();
  initScene ();
}

/******************************************************************/

void
initGlfw ()
{
  glfwSetErrorCallback (outputGlfwError);
  if (!glfwInit ())
  {
    fprintf (stderr, "Failed to init GLFW -- exiting\n");
    exit (EXIT_FAILURE);
  }
}

/******************************************************************/

void
initWindow (GLFWwindow*& window)
{
  // Necessary on MacOS! Was needed in one case on Linux too.
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow (1200, 900, "OpenGL Engine", nullptr, nullptr);
  if (window == nullptr)
  {
    fprintf (stderr, "Failed to init the window -- exiting\n");
    glfwTerminate ();
    exit (EXIT_FAILURE);
  }
  glfwSetWindowPos (window, 200, 100);

  glfwMakeContextCurrent (window);
  // Swap buffers after 1 frame
  glfwSwapInterval (1);
  glfwSetKeyCallback (window, recordKeys);
  glfwSetWindowSizeCallback(window, onWindowSizeChange);
  glfwSetFramebufferSizeCallback (window, resetViewport);

  // Specify background color
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
  // Enable depth testing so occluded surfaces aren't drawn
  glEnable (GL_DEPTH_TEST);
  // Enable the culling of back-facing triangles
  // A triangle must be wound CCW in window coordinates to be front facing
  // If it is wound CW it is back facing and will NOT be displayed!
  glEnable (GL_CULL_FACE);
  // The next two setting are default, but we'll be explicit.
  glFrontFace (GL_CCW);
  glCullFace (GL_BACK);
  // Set initial viewport size
  int width, height;
  glfwGetFramebufferSize (window, &width, &height);
  glViewport (0, 0, width, height);
}

/******************************************************************/

void
initGlew ()
{
  GLenum status = glewInit ();
  if (status != GLEW_OK)
  {
    fprintf (stderr, "Failed to initialize GLEW -- exiting"
	     " (%s).\n",
	     glewGetErrorString (status));
    exit (EXIT_FAILURE);
  }
  const GLubyte* version = glewGetString (GLEW_VERSION);
  fprintf (stderr, "Using GLEW version %s.\n", version);
  version = glGetString (GL_VERSION);
  fprintf (stderr, "Using OpenGL version %s\n", version);
}

/******************************************************************/

void
resetViewport (GLFWwindow* window, int width, int height)
{
  // Render into entire window
  // Origin for window coordinates is lower-left of window
  glViewport (0, 0, width, height);
}

/******************************************************************/

void
initScene ()
{
  // Compile and link all shader programs
  initShaders ();

  g_mesh = new Mesh();
  g_mesh->importFromFile("bear.obj");
  //g_mesh->importFromFile("Sphere.obj");
  g_mesh->prepareVao();

  g_mesh2 = new Mesh();
  g_mesh2->importFromFile("Sphere.obj");
  g_mesh2->prepareVao();
}

/******************************************************************/

void
initShaders ()
{
  // Create shader programs, which consist of linked shaders.
  // No need to use the program until we draw or set uniform variables.
  g_shaderProgram = new ShaderProgram ();
  g_shaderProgram->createVertexShader ("Vec3Norm.vert");
  g_shaderProgram->createFragmentShader ("Vec3.frag");
  g_shaderProgram->link ();
}

/******************************************************************/

void
drawScene (GLFWwindow* window)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  g_mesh->draw( g_shaderProgram, g_camera );
  g_mesh2->draw( g_shaderProgram, g_camera );

  // Swap the front and back buffers.
  // We draw to the back buffer, which is then swapped with the front
  //   for display.
  glfwSwapBuffers (window);
}

/******************************************************************/

void
onWindowSizeChange(GLFWwindow* window, int width, int height)
{
  float aspectRatio = (float)(width / height);
  g_camera.setProjection(50.0f, aspectRatio, 0.01f, 40.0f);
}

/******************************************************************/

void
recordKeys (GLFWwindow* window, int key, int scanCode, int action,
       int modifiers)
{
  // Actions are GLFW_PRESS, GLFW_RELEASE, and GLFW_REPEAT
  if ( action == GLFW_PRESS )
  {
    g_keyBuffer.setKeyDown( key );
  }
  else if ( action == GLFW_RELEASE )
  {
    g_keyBuffer.setKeyUp( key );
  }
}

/******************************************************************/

void
processKeys(GLFWwindow* window)
{
  if ( g_keyBuffer.isKeyDown( GLFW_KEY_ESCAPE ))
  {
    // Exit if ESC is pressed
    glfwSetWindowShouldClose (window, GL_TRUE);
  }
  else
  {
    static const float MOVEMENT_DELTA = 0.1f;
    static const float ANGLE_DELTA = 1.5f;
    // Translate camera/eye point using WASD keys
    if ( g_keyBuffer.isKeyDown( GLFW_KEY_W ) 
      != g_keyBuffer.isKeyDown( GLFW_KEY_S ))
    {
      if ( g_keyBuffer.isKeyDown( GLFW_KEY_W ))
      {
        g_camera.moveBack( -MOVEMENT_DELTA );
      }
      else
      {
        g_camera.moveBack( MOVEMENT_DELTA );
      }
    }
    
    if ( g_keyBuffer.isKeyDown( GLFW_KEY_A ) 
      != g_keyBuffer.isKeyDown( GLFW_KEY_D ))
    {
      if ( g_keyBuffer.isKeyDown( GLFW_KEY_A ))
      {
        g_camera.moveRight( -MOVEMENT_DELTA );
      }
      else
      {
        g_camera.moveRight( MOVEMENT_DELTA );
      }
    }
    
    if ( g_keyBuffer.isKeyDown( GLFW_KEY_F ) 
      != g_keyBuffer.isKeyDown( GLFW_KEY_C ))
    {
      if ( g_keyBuffer.isKeyDown( GLFW_KEY_F ))
      {
        g_camera.moveUp( MOVEMENT_DELTA );
      }
      else
      {
        g_camera.moveUp( -MOVEMENT_DELTA );
      }
    }
    
    if ( g_keyBuffer.isKeyDown( GLFW_KEY_I ) 
      != g_keyBuffer.isKeyDown( GLFW_KEY_K ))
    {
      if ( g_keyBuffer.isKeyDown( GLFW_KEY_I ))
      {
        g_camera.pitch( ANGLE_DELTA );
      }
      else
      {
        g_camera.pitch( -ANGLE_DELTA );
      }
    }
    
    if ( g_keyBuffer.isKeyDown( GLFW_KEY_J ) 
      != g_keyBuffer.isKeyDown( GLFW_KEY_L ))
    {
      if ( g_keyBuffer.isKeyDown( GLFW_KEY_J ))
      {
        g_camera.yaw( ANGLE_DELTA );
      }
      else
      {
        g_camera.yaw( -ANGLE_DELTA );
      }
    }
    
    if ( g_keyBuffer.isKeyDown( GLFW_KEY_N ) 
      != g_keyBuffer.isKeyDown( GLFW_KEY_M ))
    {
      if ( g_keyBuffer.isKeyDown( GLFW_KEY_N ))
      {
        g_camera.roll( ANGLE_DELTA );
      }
      else
      {
        g_camera.roll( -ANGLE_DELTA );
      }
    }
    
    if ( g_keyBuffer.isKeyDown( GLFW_KEY_R ))
    {
      g_camera.reset();
      g_mesh->reset();
    }

    if ( g_keyBuffer.isKeyDown( GLFW_KEY_1 ))
    {
      g_mesh->moveRight( -MOVEMENT_DELTA );
    }

    if ( g_keyBuffer.isKeyDown( GLFW_KEY_2 ))
    {
      g_mesh->moveUp( MOVEMENT_DELTA );
    }

    if ( g_keyBuffer.isKeyDown( GLFW_KEY_3 ))
    {
      g_mesh->moveWorld( MOVEMENT_DELTA, Vector3(1,0,0) );
    }

    if ( g_keyBuffer.isKeyDown( GLFW_KEY_4 ))
    {
      g_mesh->pitch( ANGLE_DELTA );
    }

    if ( g_keyBuffer.isKeyDown( GLFW_KEY_5 ))
    {
      g_mesh->yaw( ANGLE_DELTA );
    }

    if ( g_keyBuffer.isKeyDown( GLFW_KEY_6 ))
    {
      g_mesh->roll( ANGLE_DELTA );
    }

    if ( g_keyBuffer.isKeyDown( GLFW_KEY_7 ))
    {
      g_mesh->alignWithWorldY();
    }

    if ( g_keyBuffer.isKeyDown( GLFW_KEY_8 ))
    {
      g_mesh->scaleLocal(0.995f);
    }

    if ( g_keyBuffer.isKeyDown( GLFW_KEY_9 ))
    {
      g_mesh->scaleWorld(1.005f);
    }

    if ( g_keyBuffer.isKeyDown( GLFW_KEY_0 ))
    {
      g_mesh->shearLocalXByYz(1.0001f, 1.0001f);
    }
  }
}

/******************************************************************/
// Deallocate Resources
void
releaseGlResources ()
{
  delete g_shaderProgram;
  delete g_mesh;
  delete g_mesh2;
}

/******************************************************************/

void
outputGlfwError (int error, const char* description)
{
  fprintf (stderr, "GLFW error: %s, code %d\n", description, error);
}

/******************************************************************/