#include <ctime>
#include <cstdlib>
#include <cmath>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <gl/GLU.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <string>
#include "common.h"
#include "physics_engine.h"

const int FPS = 120;
const float Pi = 3.1415927f;
const double SceneSize = 1000.0;
double pRadius = 5.0;

std::vector<particle> Particles;
std::vector<obstacle> Obstacles;

void drawCircle(GLfloat ox, GLfloat oy, GLfloat radius)
{
	static const int partitions = 15;
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
	static const int partitions = 30;
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
		glLineWidth(scale * pRadius / 5);
		drawCircleOutline(scale * i.pos.x, scale * i.pos.y, scale * pRadius);
	}
}

void setupScene()
{

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
	
	char sysroot[30];
	GetEnvironmentVariableA("SystemRoot", sysroot, sizeof(sysroot));
	std::string fontCN = sysroot;
	fontCN += "\\Fonts\\msyh.ttc";
	if (GetFileAttributesA(fontCN.c_str()) == INVALID_FILE_ATTRIBUTES)
	{
		fontCN = sysroot;
		fontCN += "\\Fonts\\simsun.ttc";
		io.Fonts->AddFontFromFileTTF(fontCN.c_str(), 18.0f, nullptr, io.Fonts->GetGlyphRangesChinese());
	}
	else
		io.Fonts->AddFontFromFileTTF(fontCN.c_str(), 18.0f, nullptr, io.Fonts->GetGlyphRangesChinese());

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	while (!glfwWindowShouldClose(window))
	{
		static int tPhy = 0, tRender = 0;
		glfwPollEvents();

		clock_t t0 = clock();

		ImGui_ImplGlfw_NewFrame();

		//ImGui::ShowTestWindow();
		ImGui::SetNextWindowSize(ImVec2(100, 300), ImGuiSetCond_FirstUseEver);
		ImGui::Begin(u8"设置");
		ImGui::Text(u8"物理模拟%02d ms, 渲染场景%02d ms", tPhy, tRender);
		ImGui::Text(u8"FPS: %f", io.Framerate);
		static float pR = 1.0f;
		ImGui::SliderFloat(u8"粒子大小", &pR, 0.5f, 10.0f);
		static int pCnt = 100;
		ImGui::SliderInt(u8"粒子数量", &pCnt, 10, 1000);
		static float pTemp = 100.0f;
		ImGui::SliderFloat(u8"温度", &pTemp, 1, 10000);
		ImGui::Button(u8"清除粒子");
		if (ImGui::Button(u8"开始模拟"))
		{
			pRadius = pR;
			initParticles(pCnt, pTemp);
		}
	
		ImGui::End();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		//glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);
		glViewport(0, 0, width, height);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		clock_t timeP0 = clock();
		onTick(1.0 / FPS);
		clock_t timeP1 = clock();

		renderScene(width, height);

		ImGui::Render();

		glfwSwapBuffers(window);

		clock_t t1 = clock();
		if (t1 - t0 < 1000 / FPS)
			Sleep(1000 / FPS - (t1 - t0));

		tPhy = timeP1 - timeP0;
		tRender = t1 - timeP1;
	}
	return 0;
}