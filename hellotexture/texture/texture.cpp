#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <shader/shader.h>
#include<iostream>
#include<algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>
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

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);															//指定OpenGL的主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);														    //制定OpenGL的子版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);							//使用OpenGL的核心模式（可编程渲染管道的模式）
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TURE);

	GLFWwindow* window = glfwCreateWindow(1000, 1000, "LaJiOpenGL", NULL, NULL);			//创建窗口
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
	// build and compile our shader zprogram
   // ------------------------------------
	Shader ourShader("shader.vs", "shader.fs");
	//用float数组指定三个顶点的坐标 - 多边形的数组
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	float texCoords[] = {
	0.0f, 0.0f,  // lower-left corner  
	1.0f, 0.0f,  // lower-right corner
	0.5f, 1.0f   // top-center corner
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//指明顶点属性。顶点着色器给了我们极大的便利性，我们可以输入想要输入的任何属性格式。因此，我们也就要告诉OpenGL我们顶点的格式是什么样子的。
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
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

	//开始生成纹理
	 // load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture1" << std::endl;
	}
	stbi_image_free(data);																//生成纹理及其对应的mipmap之后，最好释放图像内存
	 // texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture2" << std::endl;
	}
	stbi_image_free(data);
	//生成完了

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	// or set it via the texture class
	ourShader.setInt("texture2", 1);

	//渲染循环
	while (!glfwWindowShouldClose(window))																					   //检测是否关闭窗口
	{
		processInput(window);																											  //处理输入事件

		//渲染
		glClearColor(0.5f, 0.4f, 0.3f, 1.0f);																							 	  //设置清屏颜色（黄色）
		glClear(GL_COLOR_BUFFER_BIT);		//清屏

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// render container
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// update the uniform color
		float timeValue = glfwGetTime();                                   //获取运行时间
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation= glGetUniformLocation(ourShader.ID, "ourColor1");  //查询ourColor1统一的位置，查询不到会返回-1
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		//glDrawArrays(GL_TRIANGLES, 0, 4);
		glfwSwapBuffers(window);																										 //将显示缓存和离屏缓存交换（显示离屏缓存中的内容）
		glfwPollEvents();																													   	 //检查是否有事件触发（例如鼠标输入和键盘输入事件）
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();																															 //终止进程
	return 0;
}