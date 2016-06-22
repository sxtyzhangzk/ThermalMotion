#include <ctime>
#include <cstdlib>
#include <cmath>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <gl/GLU.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include "common.h"

const int FPS = 120;
const float Pi = 3.1415927f;
const double SceneSize = 1000.0;
double pRadius = 5.0;

std::vector<particle> Particles;
std::vector<obstacle> Obstacles;

void drawCircle(GLfloat ox, GLfloat oy, GLfloat radius)
{
	static const int partitions = 100;
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < partitions; i++)
	{
		glVertex2f(ox, oy);
		glVertex2f(ox + radius * cosf(2 * Pi / partitions * i), oy + radius * sinf(2 * Pi / partitions * i));
		glVertex2f(ox + radius * cosf(2 * Pi / partitions * (i + 1)), oy + radius * sinf(2 * Pi / partitions * (i + 1)));
	}
	glEnd();
}
void drawCircleOutline(float ox, float oy, float radius)
{
	static const int partitions = 100;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= partitions; i++)
		glVertex2f(ox + radius * cosf(2 * Pi / partitions * i), oy + radius * sinf(2 * Pi / partitions * i));
	glEnd();
}

void renderScene(float width, float height)
{
	float scale = min(width, height) / SceneSize;
	glLineWidth(3.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (auto &i : Obstacles)
	{
		glBegin(GL_LINE_STRIP);
		for (auto &pt : i)
			glVertex2f(scale * pt.x, scale * pt.y);
		glEnd();
	}
	for (auto &i : Particles)
	{
		glColor3f(i.r / 255.0f, i.g / 255.0f, i.b / 255.0f);
		drawCircle(scale * i.pos.x, scale * i.pos.y, pRadius);
		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(pRadius / 5);
		drawCircleOutline(scale * i.pos.x, scale * i.pos.y, pRadius);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	glfwInit();
	//glfwWindowHint(GLFW_RESIZABLE, FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow *window = glfwCreateWindow(1152, 768, "Hello GL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	//glViewport(0, 0, 800, 600);

	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_Init(window, true);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		clock_t t0 = clock();

		ImGui_ImplGlfw_NewFrame();

		ImGui::ShowTestWindow();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		//glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);
		glViewport(0, 0, width, height);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderScene(width, height);

		ImGui::Render();

		glfwSwapBuffers(window);

		clock_t t1 = clock();
		if (t1 - t0 < 1000 / FPS)
			Sleep(1000 / FPS - (t1 - t0));
	}
	return 0;
}