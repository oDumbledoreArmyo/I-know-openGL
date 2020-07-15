#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);																									 //调整OpeGL视口区域
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)													//检查Esc建有没有按下
		glfwSetWindowShouldClose(window, true);																		    //设置窗口关闭
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);															//指定OpenGL的主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);														    //制定OpenGL的子版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);							//使用OpenGL的核心模式（可编程渲染管道的模式）
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TURE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);			//创建窗口
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();																													   //如果出错，则终止进程
		return -1;
	}
	glfwMakeContextCurrent(window);																								//将窗口的上下文环境设置为当前主线程的上下文环境

	//初始化glad，GLAD管理OpenGL的函数指针，因此我们想在调用任何OpenGL函数之前初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//注册窗口回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//渲染循环
	while (!glfwWindowShouldClose(window))																					   //检测是否关闭窗口
	{
		processInput(window);																											  //处理输入事件

		//渲染
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);																							 	  //设置清屏颜色（黄色）
		glClear(GL_COLOR_BUFFER_BIT);																							 	  //清屏

		glfwSwapBuffers(window);																										 //将显示缓存和离屏缓存交换（显示离屏缓存中的内容）
		glfwPollEvents();																													   	 //检查是否有事件触发（例如鼠标输入和键盘输入事件）
	}
	glfwTerminate();																															 //终止进程
	return 0;
}