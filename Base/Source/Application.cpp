
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SP3.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
int m_width, m_height;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


void resize_callback(GLFWwindow* window, int w, int h)
{
	m_width = w;
	m_height = h;
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}
bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}
void Application::GetCursorPos(double *xpos, double *ypos)
{
	glfwGetCursorPos(m_window, xpos, ypos);
}


Application::Application() 
{
   // theSoundEngine = NULL;
}

Application::~Application()
{
   // if (theSoundEngine != NULL)
   // {
   //    theSoundEngine->drop();
   // }
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());


	//Create a window and create its OpenGL context
	m_width = 800;
	m_height = 600;
//	m_width = mode->width;
//    m_height = mode->height;
	m_window = glfwCreateWindow(m_width, m_height, "Physics", NULL, NULL);
//  m_window = glfwCreateWindow(mode->width, mode->height, "Physics", glfwGetPrimaryMonitor(), NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    /*theSoundEngine = createIrrKlangDevice();
    BGM = theSoundEngine->addSoundSourceFromFile("music//bgm.ogg");
    BGM->setDefaultVolume(0.1f);*/

    m_dElapsedTime = 0.0;
    m_dAccumulatedTime_ThreadOne = 0.0;
    m_dAccumulatedTime_ThreadTwo = 0.0;
}

int Application::GetWindowWidth()
{
    return m_width;
}
int Application::GetWindowHeight()
{
    return m_height;
}

void Application::Run()
{
	//Main Loop
	Scene *scene = new SP3();
	scene->Init();

    HWND hwnd = GetActiveWindow();

    //theSoundEngine->play2D(BGM, true, false);

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
    while (!glfwWindowShouldClose(m_window) && !scene->quitGame)//&& !IsKeyPressed(VK_ESCAPE))
    {
        // Get the elapsed time
        m_dElapsedTime = m_timer.getElapsedTime();
        m_dAccumulatedTime_ThreadOne += m_dElapsedTime;
        m_dAccumulatedTime_ThreadTwo += m_dElapsedTime;
        if (m_dAccumulatedTime_ThreadOne > 0.03)
        {
            //GetMouseUpdate();
            //GetKeyboardUpdate();
            if (hwnd == GetActiveWindow())
            {
                scene->Update(m_dElapsedTime);
            }
            m_dAccumulatedTime_ThreadOne = 0.0;
        }
        if (m_dAccumulatedTime_ThreadTwo > 1.0)
        {
            //UpdateAI();
            m_dAccumulatedTime_ThreadTwo = 0.0;
        }
        // Render the scene
        scene->Render();
        //Swap buffers
        glfwSwapBuffers(m_window);
        //Get and organize events, like keyboard and mouse input, window resizing, etc...
        glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

    } //Check if the ESC key had been pressed or if the window had been closed
    scene->Exit();
	delete scene;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
