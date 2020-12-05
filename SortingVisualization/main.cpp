#include <random>
#include <thread>
#include <GLFW/glfw3.h>
#include "algorithms.h"

void render();
void update(long long delta);

SortVisualizerTool svt(128);

void KeyFunc(GLFWwindow* window, int key, int scancode, int action, int mode);

bool running = true;
int drawMode = 0;

int main()
{
	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(1280, 720, "SortingVisualizer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, KeyFunc);

	glClearColor(0, 0, 0, 0);

	auto s = std::chrono::high_resolution_clock::now();
	auto p = s;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		p = s;
		s = std::chrono::high_resolution_clock::now();
		update(std::chrono::duration_cast<std::chrono::nanoseconds>(s-p).count());
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	running = false;

	glfwTerminate();
	return 0;
}

void glHSV3f(GLfloat h, GLfloat s, GLfloat v);

void render()
{
	SortVisualizerTool::Action action = svt.getAction();
	if (drawMode == 0)
	{
		float x = svt.getX(0);
		for (int i = 0; i < svt.size; i++)
		{
			glBegin(GL_QUADS);
			if (i == action.a || i == action.b)
				if (action.type == 1)
					glColor3f(0.0f, 0.0f, 1.0f);
				else if (action.type == 2)
					glColor3f(1.0f, 0.0f, 0.0f);
				else if (action.type == 3)
					glColor3f(0.0f, 1.0f, 1.0f);
				else
					glColor3f(1.0f, 1.0f, 1.0f);
			else
				glColor3f(1.0f, 1.0f, 1.0f);
			float y = svt.getY(i);
			glVertex2f(x, -1.0f);
			glVertex2f(x, y);
			x = svt.getX(i + 1);
			glVertex2f(x, y);
			glVertex2f(x, -1.0f);
			glEnd();
		}
	}
	else if (drawMode == 1)
	{
		float t = svt.getTheta(0);
		for (int i = 0; i < svt.size; i++)
		{
			glBegin(GL_TRIANGLES);
			if (i == action.a || i == action.b)
				if (action.type == 1)
					glColor3f(0.0f, 0.0f, 1.0f);
				else if (action.type == 2)
					glColor3f(1.0f, 0.0f, 0.0f);
				else if (action.type == 3)
					glColor3f(0.0f, 1.0f, 1.0f);
				else
					glColor3f(1.0f, 1.0f, 1.0f);
			else
				glColor3f(1.0f, 1.0f, 1.0f);
			float r = svt.getRadius(i);
			glVertex2f(0.0, 0.0);
			glVertex2f(r * sin(t), r * cos(t));
			t = svt.getTheta(i + 1);
			glVertex2f(r * sin(t), r * cos(t));
			glEnd();
		}
	}
	else if (drawMode == 2)
	{
		float t = svt.getTheta(0);
		for (int i = 0; i < svt.size; i++)
		{
			glBegin(GL_TRIANGLES);
			if (i == action.a || i == action.b)
				glColor3f(0.0f, 0.0f, 0.0f);
			else
				glHSV3f(360 * svt.getRadius(i), 1.0f, 1.0f);
			glVertex2f(0.0, 0.0);
			glVertex2f(sin(t), cos(t));
			t = svt.getTheta(i + 1);
			glVertex2f(sin(t), cos(t));
			glEnd();
		}
	}
}

void glHSV3f(GLfloat h, GLfloat s, GLfloat v)
{
	(h == 360) ? (h = 0) : (h /= 60);
	float fract = h - floor(h);
	float p = v * (1 - s);
	float q = v * (1 - s * fract);
	float t = v * (1 - s * (1 - fract));
	if (h >= 0 && h < 1)
		glColor3f(v, t, p);
	else if (h >= 1 && h < 2)
		glColor3f(q, v, p);
	else if (h >= 2 && h < 3)
		glColor3f(p, v, t);
	else if (h >= 3 && h < 4)
		glColor3f(p, q, v);
	else if (h >= 4 && h < 5)
		glColor3f(t, p, v);
	else if (h >= 5 && h < 6)
		glColor3f(v, p, q);
	else
		glColor3f(0.0f, 0.0f, 0.0f);
}

long long interval = 5000000;

void update(long long delta)
{
	while (delta>0)
	{
		if (svt.nextAction())
		{
			svt.popAction();
		}
		delta -= interval;
	}
}

void KeyFunc(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_TAB)
		{
			drawMode = (drawMode + 1) % 3;
		}
		else if (key == GLFW_KEY_1)
		{
			svt.reset(128);
		}
		else if (key == GLFW_KEY_2)
		{
			svt.reset(256);
		}
		else if (key == GLFW_KEY_3)
		{
			svt.reset(512);
		}
		else if (key == GLFW_KEY_4)
		{
			svt.reset(1024);
		}
		else if (key == GLFW_KEY_5)
		{
			svt.reset(2048);
		}
		else if (!svt.nextAction())
		{
			if (key == GLFW_KEY_SPACE)
			{
				std::default_random_engine generator;
				std::uniform_int_distribution<int> distribution(0, svt.size - 1);
				for (int i = 0; i < svt.size; i++)
					svt.actSwap(i, distribution(generator));
			}
			else if (key == GLFW_KEY_B)
			{
				BubbleSort(svt);
			}
			else if (key == GLFW_KEY_S)
			{
				if (mode & GLFW_MOD_SHIFT)
				{
					ShellSort(svt);
				}
				else
				{
					SelectionSort(svt);
				}
			}
			else if (key == GLFW_KEY_I)
			{
				InsertionSort(svt);
			}
			else if (key == GLFW_KEY_M)
			{
				MergeSort(svt);
			}
			else if (key == GLFW_KEY_Q)
			{
				QuickSort(svt);
			}
			else if (key == GLFW_KEY_H)
			{
				HeapSort(svt);
			}
		}
	}
}