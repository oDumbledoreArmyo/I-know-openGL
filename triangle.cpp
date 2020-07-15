#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<algorithm>
#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"opengl32.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);																									 //调整OpeGL视口区域
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)													//检查Esc建有没有按下
		glfwSetWindowShouldClose(window, true);																		    //设置窗口关闭
}

const static float M_PI = 3.1415926f;
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const unsigned int  segCount = 1110; // 分割份数

//顶点着色器（GLSL语言编写，语法与C极为类似） & 获取顶点着色器的源代码，储存在代码文件顶部的const字符串中
const char* vertexShaderSource = "#version 330 core\n"				 //	指明了使用OpenGL的版本以及运行模式（版本号3.3，核心模式）
"layout (location = 0) in vec3 aPos;\n"											//指明了需要从上一个步骤中获取一个vec3类型的位置数据，数据位置在输入数据的0偏移位置（类似于输入了一块数据，我们要的数据在头部）
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"						//将顶点的位置直接赋值成输入的位置，gl_Position是一个内置的变量，用来表示顶点位置的。
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"       //我们输出一个颜色供片元使用，这个颜色是橙色。一个片元，包含了OpenGL用来渲染一个像素的所有信息。
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0"; 

/*
//用float数组指定三个顶点的坐标 - 多边形的数组
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f,
	1.0f,0 0.5f, 0.0f,  //用于生成平行四边形的第四个点
	1.0f, 0.5f, 0.0f,
};
*/
float vertices[2000] = {
	0.0f
};

int main() { 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);															//指定OpenGL的主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);														    //制定OpenGL的子版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);							//使用OpenGL的核心模式（可编程渲染管道的模式）
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TURE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LaJiOpenGL", NULL, NULL);			//创建窗口
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();																													   //如果出错，则终止进程
		return -1;
	}

	glfwMakeContextCurrent(window);																								//将窗口的上下文环境设置为当前主线程的上下文环境
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);										//注册窗口回调函数

	//初始化glad，GLAD管理OpenGL的函数指针，因此我们想在调用任何OpenGL函数之前初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//创建着色器对象，返回值是这个对象的唯一ID
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//将源码附加到着色器对象上并且编译这个着色器对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//编译片元着色器
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//链接着色器。着色器程序对象是最终将所有着色器连接起来的对象。把所有的着色器对象和这个着色器程序对象连接起来之后，我们就能使用这个着色器程序对象了。将着色器都连接到程序对象上时，OpenGL会将上一个阶段的输出连接到下一个阶段的输入上。我们需要三步来使用着色器程序对象，现在已经很熟悉了。创建、附加、链接。
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//清理不需要的着色器（过河拆桥）
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//开始生成圆的顶点数组
	float a = 0.6f; // 水平方向的半径
	//float b = a * self.frame.size.width / self.frame.size.height;
	float b = 0.8f;
	float delta = 2.0f * M_PI / segCount;
	int count = 0;
	int i = 0;
	//float* vertices;
	//vertices = new float[100];
	for ( i = 0; i < segCount; i=i+3) {	
		float x = a * cos(delta * i);
		float y = b * sin(delta * i);
		float z = 0.0f;
		//printf("%f",x);
		vertices[i] = x;
		vertices[i +1] = y;
		vertices[i +2] = z;
		count++;
		//printf("%f,%f,%f,%d \n", vertices[i], vertices[i + 1],vertices[i + 2],count);
		//0表示顶点数组的起始索引，3表示有3个顶点	
	}
	//生成完了

	unsigned int VBO, VAO;																			//顶点缓存对象(VBO,Vertex Buffer Object),表示存储在GPU显存中的大量顶点数据,可以通过这个对象，一次性向GPU发送大量的数据，而不是一次次地从CPU中发送数据到GPU，这是个很慢的过程。
	glGenVertexArrays(1, &VAO);																		//VAO全称是顶点数组对象（Vertex Array Object），作用是来保存对顶点属性的调用。
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);																	//生成唯一ID。这个ID不能我们自己来定，只能告诉OpenGL说，我需要一个唯一ID，你给我一个吧。然后，OpenGL就会给你一个没用过的唯一ID，这个过程是由glGenBuffers来实现的。
	glBindBuffer(GL_ARRAY_BUFFER, VBO);											//指明缓存对象类型。OpenGL中有很多缓存对象，虽然它给了我们一个ID，但不知道这个ID是用来表示什么缓存对象的。我们要明确告诉它，这是一个顶点缓存对象。绑定的作用，是将原来有的东西替换成我们新的东西，这样，接下来对GL_ARRAY_BUFFER的操作都是对我们新东西的操作了。
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);					//将顶点数据复制到VBO。参数1：我们的顶点数据需要拷贝到的地方。（之前我们绑定的VBO）参数2：数组的大小；参数3：数组的地址；参数4：指定显卡要采用什么方式来管理我们的数据，GL_STATIC_DRAW表示这些数据不会经常改变。

	//指明顶点属性。顶点着色器给了我们极大的便利性，我们可以输入想要输入的任何属性格式。因此，我们也就要告诉OpenGL我们顶点的格式是什么样子的。
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
		/*
		我们的起始顶点的偏移为0，每个位置分量占用4字节的空间，一个顶点占用12字节空间。
		参数1：指明我们想要配置的顶点属性。类似编号的东西，之前我们设置了location = 0，就是我们在这里用到的0.
		参数2：顶点属性的大小。我们用到的顶点是一个vec3的结果，所以大小为3. 表示从数组里一次取多少个值来表示一个点。
		参数3：数据的类型。我们使用的是float类型
		参数4：指明数据是否要被规范化。这里我们设置成FALSE，不用规范化，因为我们已经规范化好了。
		参数5：表示属性的跨度。正如之前我们分析的，我们的跨度是12，就是3倍的float类型。
		参数6：指明了数据的起始偏移量。这里转成了一个void*指针类型比较奇怪，我们以后再聊。
		我们获取的顶点属性是由VBO决定的，而glVertexAttribPointer操作的是当前绑定到GL_ARRAY_BUFFER上的VBO，所以，我们当前操作的就是我们之前生成并绑定的那个VBO。
		glEnableVertexAttribArray(0)是用来让顶点属性生效的，参数0就是我们之前配置的那个顶点属性的位置。
		*/
	
	printf("%f,%f,%f,%d \n", vertices[i+3], vertices[i+4], vertices[i+4], count);

	//渲染循环
	while (!glfwWindowShouldClose(window))																					   //检测是否关闭窗口
	{
		processInput(window);																											  //处理输入事件

		//渲染
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);																							 	  //设置清屏颜色（黄色）
		glClear(GL_COLOR_BUFFER_BIT);		//清屏
		glLineWidth(2.0);

		glUseProgram(shaderProgram); //使用着色器程序对象
		//glDrawArrays(GL_LINE_STRIP ,0, count);
	   glDrawArrays(GL_TRIANGLE_FAN, 0, count);																				    	  //第二个三角形，拼成平行四边形

		glfwSwapBuffers(window);																										 //将显示缓存和离屏缓存交换（显示离屏缓存中的内容）
		glfwPollEvents();																													   	 //检查是否有事件触发（例如鼠标输入和键盘输入事件）
	}
	glfwTerminate();																															 //终止进程
	return 0;
}