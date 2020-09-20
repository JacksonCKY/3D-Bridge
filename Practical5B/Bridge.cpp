
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "3D Bridge"

int questionnumber = 1;
int changeview = 1;
float radius = 0.275;
int slices = 30;
int stacks = 30;
float rotateBridge = 0.0;
float posX = 0;
float posY = -40;
float posZ = -80;
float rotateX = 0;
float rotateY = 0;
float rotateZ = 0;
float baseRadius = 0.01;
float topRadius = 0.3;
float height = 0.8;

BITMAP BMP;
HBITMAP hBMP = NULL;
GLuint texture = 1;

BITMAP BMP_rope, BMP_connector, BMP_buildingtop, BMP_clock, BMP_building, BMP_towerbase, BMP_roadside, BMP_bridgeroad, BMP_sky, BMP_land, BMP_seaflow1;

GLuint tex_rope = 1, tex_connector = 1, tex_buildingtop = 1, tex_clock = 1, tex_building = 1, tex_towerbase = 1, tex_roadside = 1, tex_bridgeroad = 1, tex_sky = 1, tex_land = 1, tex_seaflow1 = 1;

GLUquadricObj* myQuadricObject = gluNewQuadric();

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			PostQuitMessage(0);
			break;
		}
		case 0x41:
		{
			//A
			if (rotateBridge < 35)
				rotateBridge += 1;
			break;
		}
		case 0x53:
		{
			//S
			if (rotateBridge > 0)
				rotateBridge -= 1;
			break;
		}
		case 0x49:
		{
			//I
			posY -= 1;
			break;
		}
		case 0x4A:
		{
			//J
			posX += 1;
			break;
		}
		case 0x4B:
		{
			//K
			posY += 1;
			break;

		}
		case 0x4C:
		{
			//L
			posX -= 1;
			break;
		}
		case 0x4F:
		{
			//O
			rotateY += 2;
			break;
		}
		case 0x55:
		{
			//U
			rotateY -= 2;
			break;
		}
		case 0x47:
		{
			//G
			posZ += 1;
			break;
		}
		case 0x48:
		{
			//H
			posZ -= 1;
			break;
		}
		case 0x56:
		{
			//V
			//Change view
			if (changeview == 1) {
				glLoadIdentity();

				//glOrtho(-10, 10, -10, 10, -10, 10);
				gluPerspective(45, 1, 0.1, 300);
				rotateX = 0;
				rotateY = 0;
				rotateZ = 0;
				changeview = 0;
			}
			else {
				glLoadIdentity();
				glOrtho(-50, 50, -50, 50, 50, 300);
				rotateX = 0;
				rotateY = 0;
				rotateZ = 0;
				changeview = 1;
			}
			break;
		}
		}

		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);
	glRotatef(rotateZ, 0, 0, 1);

	glTranslatef(posX, posY, posZ);

	//------------------------ SKY ------------------------------------------------
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, tex_sky);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP_sky.bmWidth, BMP_sky.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP_sky.bmBits);
	//behind
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, 150.0f, -100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, 0.0f, -100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 150.0f, -100.0f);
	glEnd();

	//front
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, 150.0f, 100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 150.0f, 100.0f);
	glEnd();

	//left
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, 150.0f, -100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-100.0f, 150.0f, 100.0f);
	glEnd();

	//right
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(100.0f, 150.0f, -100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(100.0f, 0.0f, -100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 150.0f, 100.0f);
	glEnd();


	//--------------------- LAND --------------------------------
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, tex_land);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP_land.bmWidth, BMP_land.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP_land.bmBits);

	//left land
	//bottom
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-55.0f, 0.0f, -100.0f);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(-55.0f, 0.0f, 100.0f);
	glEnd();
	//top
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-100.0f, 5.0f, 100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, 5.0f, -100.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-55.0f, 5.0f, -100.0f);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(-55.0f, 5.0f, 100.0f);
	glEnd();
	//thickness
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-55.0f, 5.0f, -100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-55.0f, 0.0f, -100.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-55.0f, 0.0f, 100.0f);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(-55.0f, 5.0f, 100.0f);
	glEnd();

	//right land
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(55.0f, 0.0f, 100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, 0.0f, -100.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(100.0f, 0.0f, -100.0f);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(100.0f, 0.0f, 100.0f);
	glEnd();
	//thickness
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(55.0f, 5.0f, -100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, 0.0f, -100.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(55.0f, 0.0f, 100.0f);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(55.0f, 5.0f, 100.0f);
	glEnd();
	//top
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(55.0f, 5.0f, 100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, 5.0f, -100.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(100.0f, 5.0f, -100.0f);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(100.0f, 5.0f, 100.0f);
	glEnd();


	//--------------------- SEA --------------------------------
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, tex_seaflow1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP_seaflow1.bmWidth, BMP_seaflow1.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP_seaflow1.bmBits);

	//bottom
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-55.0f, 0.0f, 100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-55.0f, 0.0f, -100.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(55.0f, 0.0f, -100.0f);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(55.0f, 0.0f, 100.0f);
	glEnd();

	//top
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-55.0f, 4.0f, 100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-55.0f, 4.0f, -100.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(55.0f, 4.0f, -100.0f);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(55.0f, 4.0f, 100.0f);
	glEnd();

	//thickess
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-55.0f, 4.0f, -100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-55.0f, 0.0f, -100.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-55.0f, 0.0f, 100.0f);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(-55.0f, 4.0f, 100.0f);
	glEnd();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(55.0f, 4.0f, -100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, 0.0f, -100.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(55.0f, 0.0f, 100.0f);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(55.0f, 4.0f, 100.0f);
	glEnd();


	//---------------------------------------------------------------------------------------------
	//--------------------------------------  BRIDGE ----------------------------------------------
	//---------------------------------------------------------------------------------------------
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, tex_roadside);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP_roadside.bmWidth, BMP_roadside.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP_roadside.bmBits);

	//left
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 22.0f, -20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-70.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0f, 22.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 22.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-70.0f, 20.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0f, 22.0f, -45.0f);
	glEnd();

	//right
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(50.0f, 22.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(70.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(70.0f, 22.0f, -20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(50.0f, 22.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(70.0f, 20.0f, -40.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(70.0f, 22.0f, -40.0f);
	glEnd();

	glPushMatrix();
	//glTranslatef(posX, posY, posZ);
	glTranslatef(0, 0, 0);
	glRotatef(rotateBridge, 0, 0, 1);
	glTranslatef(0, 0, 0);
	//animation part 
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 22.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.0f, 22.0f, -15.0f);
	glEnd();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 22.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.0f, 22.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 22.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 22.0f, -15.0f);
	glEnd();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 22.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 22.0f, -45.0f);
	glEnd();


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, tex_bridgeroad);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP_bridgeroad.bmWidth, BMP_bridgeroad.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP_bridgeroad.bmBits);

	//animation part 
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, -1.0f); glVertex3f(0.0f, 22.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 22.0f, -15.0f);
	glTexCoord2f(-1.0f, 0.0f); glVertex3f(30.0f, 22.0f, -15.0f);
	glTexCoord2f(-1.0f, -1.0f); glVertex3f(30.0f, 22.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 22.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 22.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 22.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 22.0f, -45.0f);
	glEnd();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, -1.0f); glVertex3f(0.0f, 20.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 20.0f, -15.0f);
	glTexCoord2f(-1.0f, 0.0f); glVertex3f(30.0f, 20.0f, -15.0f);
	glTexCoord2f(-1.0f, -1.0f); glVertex3f(30.0f, 20.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 20.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 20.0f, -45.0f);
	glEnd();

	glPopMatrix();
	//-------------------------------------------------------------------------------
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 20.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-70.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0f, 20.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, -1.0f); glVertex3f(50.0f, 20.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0f, 20.0f, -15.0f);
	glTexCoord2f(-1.0f, 0.0f); glVertex3f(70.0f, 20.0f, -20.0f);
	glTexCoord2f(-1.0f, -1.0f); glVertex3f(70.0f, 20.0f, -40.0f);
	glEnd();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 22.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-70.0f, 22.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 22.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0f, 22.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, -1.0f); glVertex3f(50.0f, 22.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0f, 22.0f, -15.0f);
	glTexCoord2f(-1.0f, 0.0f); glVertex3f(70.0f, 22.0f, -20.0f);
	glTexCoord2f(-1.0f, -1.0f); glVertex3f(70.0f, 22.0f, -40.0f);
	glEnd();

	//---------tower----------
	//---towerbase
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, tex_towerbase);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP_towerbase.bmWidth, BMP_towerbase.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP_towerbase.bmBits);

	//left 1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-90.0f, 20.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-90.0f, 5.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-70.0f, 5.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-70.0f, 20.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-90.0f, 20.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-90.0f, 5.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-70.0f, 5.0f, -45.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-70.0f, 20.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-90.0f, 20.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-90.0f, 5.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-90.0f, 5.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-90.0f, 20.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 20.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-70.0f, 5.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-70.0f, 5.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-70.0f, 20.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-90.0f, 20.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-90.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-70.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-70.0f, 20.0f, -45.0f);
	glEnd();

	//left 2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50.0f, 20.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-30.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-30.0f, 20.0f, -10.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50.0f, 20.0f, -50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 0.0f, -50.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-30.0f, 0.0f, -50.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-30.0f, 20.0f, -50.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50.0f, 20.0f, -50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 0.0f, -50.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0f, 20.0f, -10.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 20.0f, -50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 0.0f, -50.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-30.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-30.0f, 20.0f, -10.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50.0f, 20.0f, -50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 20.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-30.0f, 20.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-30.0f, 20.0f, -50.0f);
	glEnd();


	//right 1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, 20.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0f, 20.0f, -10.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, 20.0f, -50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, 0.0f, -50.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 0.0f, -50.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0f, 20.0f, -50.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(50.0f, 20.0f, -50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0f, 0.0f, -50.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0f, 20.0f, -10.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, 20.0f, -50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, 0.0f, -50.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 20.0f, -10.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, 20.0f, -50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, 20.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 20.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0f, 20.0f, -50.0f);
	glEnd();

	//right 2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 20.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(70.0f, 5.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(90.0f, 5.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(90.0f, 20.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 20.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(70.0f, 5.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(90.0f, 5.0f, -45.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(90.0f, 20.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(90.0f, 20.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(90.0f, 5.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(90.0f, 5.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(90.0f, 20.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 20.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(70.0f, 5.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(70.0f, 5.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(70.0f, 20.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 20.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(70.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(90.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(90.0f, 20.0f, -45.0f);
	glEnd();

	//-----------buidlings
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, tex_building);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP_building.bmWidth, BMP_building.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP_building.bmBits);
	//left 1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-90.0f, 40.0f, -20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-90.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-70.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-70.0f, 40.0f, -20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-90.0f, 40.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-90.0f, 20.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-70.0f, 20.0f, -40.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-70.0f, 40.0f, -40.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 40.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-70.0f, 20.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-70.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-70.0f, 40.0f, -20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-90.0f, 40.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-90.0f, 20.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-90.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-90.0f, 40.0f, -20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-92.0f, 40.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-92.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-68.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-68.0f, 40.0f, -40.0f);
	glEnd();

	//left 2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50.0f, 60.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-30.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-30.0f, 60.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50.0f, 60.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-30.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-30.0f, 60.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 60.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-30.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-30.0f, 60.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-50.0f, 60.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0f, 60.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-52.0f, 60.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-52.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-28.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-28.0f, 60.0f, -45.0f);
	glEnd();

	//right 1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, 60.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0f, 60.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, 60.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0f, 60.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, 60.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 60.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(50.0f, 60.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0f, 20.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 20.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0f, 60.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(28.0f, 60.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(28.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(52.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(52.0f, 60.0f, -45.0f);
	glEnd();

	//right 2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 40.0f, -20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(70.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(90.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(90.0f, 40.0f, -20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 40.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(70.0f, 20.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(90.0f, 20.0f, -40.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(90.0f, 40.0f, -40.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(90.0f, 40.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(90.0f, 20.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(90.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(90.0f, 40.0f, -20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 40.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(70.0f, 20.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(70.0f, 20.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(70.0f, 40.0f, -20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(68.0f, 40.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(68.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(92.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(92.0f, 40.0f, -40.0f);
	glEnd();

	//---------clock
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, tex_clock);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP_clock.bmWidth, BMP_clock.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP_clock.bmBits);

	//left 1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-92.0f, 60.0f, -20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-92.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-68.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-68.0f, 60.0f, -20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-92.0f, 60.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-92.0f, 40.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-68.0f, 40.0f, -40.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-68.0f, 60.0f, -40.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-68.0f, 60.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-68.0f, 40.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-68.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-68.0f, 60.0f, -20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-92.0f, 60.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-92.0f, 40.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-92.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-92.0f, 60.0f, -20.0f);
	glEnd();

	//left 2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-52.0f, 80.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-52.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-28.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-28.0f, 80.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-52.0f, 80.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-52.0f, 60.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-28.0f, 60.0f, -45.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-28.0f, 80.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-28.0f, 80.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-28.0f, 60.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-28.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-28.0f, 80.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-52.0f, 80.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-52.0f, 60.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-52.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-52.0f, 80.0f, -15.0f);
	glEnd();

	//right 1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(28.0f, 80.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(28.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(52.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(52.0f, 80.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(28.0f, 80.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(28.0f, 60.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(52.0f, 60.0f, -45.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(52.0f, 80.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(52.0f, 80.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(52.0f, 60.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(52.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(52.0f, 80.0f, -15.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(28.0f, 80.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(28.0f, 60.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(28.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(28.0f, 80.0f, -15.0f);
	glEnd();

	//right 2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(68.0f, 60.0f, -20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(68.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(92.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(92.0f, 60.0f, -20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(68.0f, 60.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(68.0f, 40.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(92.0f, 40.0f, -40.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(92.0f, 60.0f, -40.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(68.0f, 60.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(68.0f, 40.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(68.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(68.0f, 60.0f, -20.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(92.0f, 60.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(92.0f, 40.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(92.0f, 40.0f, -20.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(92.0f, 60.0f, -20.0f);
	glEnd();

	//----------buildingtop
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, tex_buildingtop);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP_buildingtop.bmWidth, BMP_buildingtop.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP_buildingtop.bmBits);
	//left 1
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-92.0f, 60.0f, -20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-80.0f, 80.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-68.0f, 60.0f, -20.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-92.0f, 60.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-80.0f, 80.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-92.0f, 60.0f, -20.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-68.0f, 60.0f, -20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-80.0f, 80.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-68.0f, 60.0f, -40.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-92.0f, 60.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-80.0f, 80.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-68.0f, 60.0f, -40.0f);
	glEnd();
	
	//left 2
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-52.0f, 80.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, 100.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-28.0f, 80.0f, -15.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-52.0f, 80.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, 100.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-52.0f, 80.0f, -15.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-28.0f, 80.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, 100.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-28.0f, 80.0f, -45.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-52.0f, 80.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, 100.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-28.0f, 80.0f, -45.0f);
	glEnd();

	//right
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(28.0f, 80.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, 100.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(52.0f, 80.0f, -15.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(28.0f, 80.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, 100.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(28.0f, 80.0f, -15.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(52.0f, 80.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, 100.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(52.0f, 80.0f, -45.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(28.0f, 80.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, 100.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(52.0f, 80.0f, -45.0f);
	glEnd();

	//left 2
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(68.0f, 60.0f, -20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(80.0f, 80.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(92.0f, 60.0f, -20.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(68.0f, 60.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(80.0f, 80.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(68.0f, 60.0f, -20.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(92.0f, 60.0f, -20.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(80.0f, 80.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(92.0f, 60.0f, -40.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(68.0f, 60.0f, -40.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(80.0f, 80.0f, -30.0f);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(92.0f, 60.0f, -40.0f);
	glEnd();

	//-----------Connector
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, tex_connector);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP_connector.bmWidth, BMP_connector.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP_connector.bmBits);

	//thickness
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 60.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 57.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, 57.0f, -45.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 60.0f, -45.0f);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 60.0f, -15.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 57.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, 57.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 60.0f, -15.0f);
	glEnd();

	//bottom
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 57.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 57.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, 57.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 57.0f, -45.0f);
	glEnd();

	//top
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-30.0f, 60.0f, -45.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(30.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, 60.0f, -45.0f);
	glEnd();

	//------------------Rope-------------------------------------------
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, tex_rope);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP_rope.bmWidth, BMP_rope.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP_rope.bmBits);
	glLineWidth(3);
	//left
	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 22.0f, -22.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 40.0f, -15.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 40.0f, -22.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 60.0f, -15.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 22.0f, -22.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 60.0f, -15.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 22.0f, -38.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 40.0f, -45.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 40.0f, -38.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 60.0f, -45.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 22.0f, -38.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 60.0f, -45.0f);
	glEnd();

	//right
	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 22.0f, -22.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 40.0f, -15.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 40.0f, -22.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 60.0f, -15.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 22.0f, -22.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 60.0f, -15.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 22.0f, -38.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 40.0f, -45.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 40.0f, -38.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 60.0f, -45.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 22.0f, -38.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 60.0f, -45.0f);
	glEnd();

	//points
	glPointSize(10);
	glBegin(GL_POINTS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 60.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 40.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0f, 40.0f, -15.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 22.0f, -22.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 40.0f, -22.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 40.0f, -38.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(70.0f, 22.0f, -38.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 60.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 60.0f, -15.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 40.0f, -45.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0f, 40.0f, -15.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 22.0f, -22.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 40.0f, -22.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 40.0f, -38.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-70.0f, 22.0f, -38.0f);
	glEnd();


	glPopMatrix();

	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR commandstring, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 700, 700,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-1, 1, -1, 1, 0, 1);
	gluPerspective(45, 1, 0.1, 300);
	//glFrustum(-0.2, 0.2, -0.2, 0.2, 0.1, 10);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glEnable(GL_TEXTURE_2D);

	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "sky.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP_sky), &BMP_sky);
	glGenTextures(1, &tex_sky);

	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "land.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP_land), &BMP_land);
	glGenTextures(1, &tex_land);

	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "seaflow1.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP_seaflow1), &BMP_seaflow1);
	glGenTextures(1, &tex_seaflow1);

	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bridgeroad.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP_bridgeroad), &BMP_bridgeroad);
	glGenTextures(1, &tex_bridgeroad);

	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "roadside.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP_roadside), &BMP_roadside);
	glGenTextures(1, &tex_roadside);

	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "towerbase.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP_towerbase), &BMP_towerbase);
	glGenTextures(1, &tex_towerbase);

	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "clock.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP_clock), &BMP_clock);
	glGenTextures(1, &tex_clock);

	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "clockbuilding.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP_building), &BMP_building);
	glGenTextures(1, &tex_building);

	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "buildingtop.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP_buildingtop), &BMP_buildingtop);
	glGenTextures(1, &tex_buildingtop);

	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "connector.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP_connector), &BMP_connector);
	glGenTextures(1, &tex_connector);

	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "rope.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP_rope), &BMP_rope);
	glGenTextures(1, &tex_rope);
	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------