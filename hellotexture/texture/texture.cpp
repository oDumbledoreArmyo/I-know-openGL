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
	glViewport(0, 0, width, height);																									 //����OpeGL�ӿ�����
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)													//���Esc����û�а���
		glfwSetWindowShouldClose(window, true);																		    //���ô��ڹر�
}

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);															//ָ��OpenGL�����汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);														    //�ƶ�OpenGL���Ӱ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);							//ʹ��OpenGL�ĺ���ģʽ���ɱ����Ⱦ�ܵ���ģʽ��
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TURE);

	GLFWwindow* window = glfwCreateWindow(1000, 1000, "LaJiOpenGL", NULL, NULL);			//��������
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();																													   //�������������ֹ����
		return -1;
	}

	glfwMakeContextCurrent(window);																								//�����ڵ������Ļ�������Ϊ��ǰ���̵߳������Ļ���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);										//ע�ᴰ�ڻص�����

	//��ʼ��glad��GLAD����OpenGL�ĺ���ָ�룬����������ڵ����κ�OpenGL����֮ǰ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// build and compile our shader zprogram
   // ------------------------------------
	Shader ourShader("shader.vs", "shader.fs");
	//��float����ָ��������������� - ����ε�����
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

	//ָ���������ԡ�������ɫ���������Ǽ���ı����ԣ����ǿ���������Ҫ������κ����Ը�ʽ����ˣ�����Ҳ��Ҫ����OpenGL���Ƕ���ĸ�ʽ��ʲô���ӵġ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	/*
	���ǵ���ʼ�����ƫ��Ϊ0��ÿ��λ�÷���ռ��4�ֽڵĿռ䣬һ������ռ��12�ֽڿռ䡣
	����1��ָ��������Ҫ���õĶ������ԡ����Ʊ�ŵĶ�����֮ǰ����������location = 0�����������������õ���0.
	����2���������ԵĴ�С�������õ��Ķ�����һ��vec3�Ľ�������Դ�СΪ3. ��ʾ��������һ��ȡ���ٸ�ֵ����ʾһ���㡣
	����3�����ݵ����͡�����ʹ�õ���float����
	����4��ָ�������Ƿ�Ҫ���淶���������������ó�FALSE�����ù淶������Ϊ�����Ѿ��淶�����ˡ�
	����5����ʾ���ԵĿ�ȡ�����֮ǰ���Ƿ����ģ����ǵĿ����12������3����float���͡�
	����6��ָ�������ݵ���ʼƫ����������ת����һ��void*ָ�����ͱȽ���֣������Ժ����ġ�
	���ǻ�ȡ�Ķ�����������VBO�����ģ���glVertexAttribPointer�������ǵ�ǰ�󶨵�GL_ARRAY_BUFFER�ϵ�VBO�����ԣ����ǵ�ǰ�����ľ�������֮ǰ���ɲ��󶨵��Ǹ�VBO��
	glEnableVertexAttribArray(0)�������ö���������Ч�ģ�����0��������֮ǰ���õ��Ǹ��������Ե�λ�á�
	*/

	//��ʼ��������
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
	stbi_image_free(data);																//�������������Ӧ��mipmap֮������ͷ�ͼ���ڴ�
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
	//��������

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	// or set it via the texture class
	ourShader.setInt("texture2", 1);

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))																					   //����Ƿ�رմ���
	{
		processInput(window);																											  //���������¼�

		//��Ⱦ
		glClearColor(0.5f, 0.4f, 0.3f, 1.0f);																							 	  //����������ɫ����ɫ��
		glClear(GL_COLOR_BUFFER_BIT);		//����

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
		float timeValue = glfwGetTime();                                   //��ȡ����ʱ��
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation= glGetUniformLocation(ourShader.ID, "ourColor1");  //��ѯourColor1ͳһ��λ�ã���ѯ�����᷵��-1
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		//glDrawArrays(GL_TRIANGLES, 0, 4);
		glfwSwapBuffers(window);																										 //����ʾ������������潻������ʾ���������е����ݣ�
		glfwPollEvents();																													   	 //����Ƿ����¼������������������ͼ��������¼���
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();																															 //��ֹ����
	return 0;
}