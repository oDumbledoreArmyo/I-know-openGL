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
	glViewport(0, 0, width, height);																									 //����OpeGL�ӿ�����
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)													//���Esc����û�а���
		glfwSetWindowShouldClose(window, true);																		    //���ô��ڹر�
}

const static float M_PI = 3.1415926f;
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const unsigned int  segCount = 1110; // �ָ����

//������ɫ����GLSL���Ա�д���﷨��C��Ϊ���ƣ� & ��ȡ������ɫ����Դ���룬�����ڴ����ļ�������const�ַ�����
const char* vertexShaderSource = "#version 330 core\n"				 //	ָ����ʹ��OpenGL�İ汾�Լ�����ģʽ���汾��3.3������ģʽ��
"layout (location = 0) in vec3 aPos;\n"											//ָ������Ҫ����һ�������л�ȡһ��vec3���͵�λ�����ݣ�����λ�����������ݵ�0ƫ��λ�ã�������������һ�����ݣ�����Ҫ��������ͷ����
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"						//�������λ��ֱ�Ӹ�ֵ�������λ�ã�gl_Position��һ�����õı�����������ʾ����λ�õġ�
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"       //�������һ����ɫ��ƬԪʹ�ã������ɫ�ǳ�ɫ��һ��ƬԪ��������OpenGL������Ⱦһ�����ص�������Ϣ��
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0"; 

/*
//��float����ָ��������������� - ����ε�����
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f,
	1.0f,0 0.5f, 0.0f,  //��������ƽ���ı��εĵ��ĸ���
	1.0f, 0.5f, 0.0f,
};
*/
float vertices[2000] = {
	0.0f
};

int main() { 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);															//ָ��OpenGL�����汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);														    //�ƶ�OpenGL���Ӱ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);							//ʹ��OpenGL�ĺ���ģʽ���ɱ����Ⱦ�ܵ���ģʽ��
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TURE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LaJiOpenGL", NULL, NULL);			//��������
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();																													   //�����������ֹ����
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

	//������ɫ�����󣬷���ֵ����������ΨһID
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//��Դ�븽�ӵ���ɫ�������ϲ��ұ��������ɫ������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//����ƬԪ��ɫ��
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//������ɫ������ɫ��������������ս�������ɫ�����������Ķ��󡣰����е���ɫ������������ɫ�����������������֮�����Ǿ���ʹ�������ɫ����������ˡ�����ɫ�������ӵ����������ʱ��OpenGL�Ὣ��һ���׶ε�������ӵ���һ���׶ε������ϡ�������Ҫ������ʹ����ɫ��������������Ѿ�����Ϥ�ˡ����������ӡ����ӡ�
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//������Ҫ����ɫ�������Ӳ��ţ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//��ʼ����Բ�Ķ�������
	float a = 0.6f; // ˮƽ����İ뾶
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
		//0��ʾ�����������ʼ������3��ʾ��3������	
	}
	//��������

	unsigned int VBO, VAO;																			//���㻺�����(VBO,Vertex Buffer Object),��ʾ�洢��GPU�Դ��еĴ�����������,����ͨ���������һ������GPU���ʹ��������ݣ�������һ�δεش�CPU�з������ݵ�GPU�����Ǹ������Ĺ��̡�
	glGenVertexArrays(1, &VAO);																		//VAOȫ���Ƕ����������Vertex Array Object����������������Զ������Եĵ��á�
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);																	//����ΨһID�����ID���������Լ�������ֻ�ܸ���OpenGL˵������Ҫһ��ΨһID�������һ���ɡ�Ȼ��OpenGL�ͻ����һ��û�ù���ΨһID�������������glGenBuffers��ʵ�ֵġ�
	glBindBuffer(GL_ARRAY_BUFFER, VBO);											//ָ������������͡�OpenGL���кܶ໺�������Ȼ����������һ��ID������֪�����ID��������ʾʲô�������ġ�����Ҫ��ȷ������������һ�����㻺����󡣰󶨵����ã��ǽ�ԭ���еĶ����滻�������µĶ�������������������GL_ARRAY_BUFFER�Ĳ������Ƕ������¶����Ĳ����ˡ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);					//���������ݸ��Ƶ�VBO������1�����ǵĶ���������Ҫ�������ĵط�����֮ǰ���ǰ󶨵�VBO������2������Ĵ�С������3������ĵ�ַ������4��ָ���Կ�Ҫ����ʲô��ʽ���������ǵ����ݣ�GL_STATIC_DRAW��ʾ��Щ���ݲ��ᾭ���ı䡣

	//ָ���������ԡ�������ɫ���������Ǽ���ı����ԣ����ǿ���������Ҫ������κ����Ը�ʽ����ˣ�����Ҳ��Ҫ����OpenGL���Ƕ���ĸ�ʽ��ʲô���ӵġ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
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
	
	printf("%f,%f,%f,%d \n", vertices[i+3], vertices[i+4], vertices[i+4], count);

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))																					   //����Ƿ�رմ���
	{
		processInput(window);																											  //���������¼�

		//��Ⱦ
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);																							 	  //����������ɫ����ɫ��
		glClear(GL_COLOR_BUFFER_BIT);		//����
		glLineWidth(2.0);

		glUseProgram(shaderProgram); //ʹ����ɫ���������
		//glDrawArrays(GL_LINE_STRIP ,0, count);
	   glDrawArrays(GL_TRIANGLE_FAN, 0, count);																				    	  //�ڶ��������Σ�ƴ��ƽ���ı���

		glfwSwapBuffers(window);																										 //����ʾ������������潻������ʾ���������е����ݣ�
		glfwPollEvents();																													   	 //����Ƿ����¼������������������ͼ��������¼���
	}
	glfwTerminate();																															 //��ֹ����
	return 0;
}