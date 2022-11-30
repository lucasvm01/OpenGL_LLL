#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include<GLAD/glad.h>
#include<GLFW/glfw3.h>

#include"Renderer.h"

#include"VertexBuffer.h"
#include"IndexBuffer.h"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

const unsigned int WINWIDTH = 640, WINHEIGHT = 480;

static enum ShaderType {
	NONE = -1, VERTEX = 0, FRAGMENT = 1,
};

// Compile each shader
static unsigned int CompileShader(unsigned int type, const std::string& source) {
	// Generate a shader
	unsigned int id = glCreateShader(type);
	// Cast the string to char*
	const char* src = source.c_str();
	// Bind source to the shader
	glShaderSource(id, 1, &src, nullptr);
	// Compile it
	glCompileShader(id);

	// Error handling
	// Check if shader compiled succesfully
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile shader." << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

// Create the specified shaders
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	// Generate a program id for the shaders
	unsigned int program = glCreateProgram();
	// Compile vertex shader
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	if (vs == 0) return -1;
	glAttachShader(program, vs);

	// Compile fragment shader
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	if (fs == 0) return -1;
	glAttachShader(program, fs);

	// Link shaders in the program
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

// Parse the content of the shader file
static ShaderProgramSource ParseShaderFile(const std::string& filepath) {
	std::ifstream stream(filepath);

	if (!stream) {
		std::cout << "Could not find shaders filepath." << std::endl;
	}

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

int main(void) {
	// Configuration
	GLFWwindow* window;

	// Initialize glfw
	if (!glfwInit())
		return -1;

	// Create a window and its OpenGL context
	window = glfwCreateWindow(WINWIDTH, WINHEIGHT, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	// Initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		glfwTerminate();
		return -1;
	}
	GLCall(glViewport(0, 0, WINWIDTH, WINHEIGHT));

	// Setup entities
	{
		// Vertices
		float pos[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};

		// Generate and bind a Vertex Array Object
		unsigned int VAO;
		GLCall(glGenVertexArrays(1, &VAO));
		GLCall(glBindVertexArray(VAO));

		// The constructor already binds it, so no need to call Bind()
		VertexBuffer VB(pos, 4 * 2 * sizeof(float));


		// Indices of the positions in order of drawing
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		IndexBuffer IB(indices, 6);

		// Create shaders
		ShaderProgramSource source = ParseShaderFile("res/shader/Basic.shader");

		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		if (shader == -1) {
			glfwTerminate();
			return -1;
		}
		GLCall(glUseProgram(shader));

		GLCall(int color_location = glGetUniformLocation(shader, "u_color"));
		ASSERT(color_location != -1);

		GLCall(glUniform4f(color_location, 0.8f, 0.0f, 0.7f, 0.5f));

		// Reset binds
		GLCall(glBindVertexArray(0));
		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		// Setup logic

		float r = 0.0f;
		float i = 0.05f;

		// Main loop
		while (!glfwWindowShouldClose(window)) {
			// Rendering

			// Clear window - first because of first frame
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			GLCall(glUseProgram(shader));
			GLCall(glBindVertexArray(VAO));
			IB.Bind();

			// Loop logic

			if (r > 1.0f || r < 0.0f) i *= -1;
			r += i;

			// Draw call
			GLCall(glUniform4f(color_location, r, 0.0f, 0.7f, 0.5f));
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			// Process events
			GLCall(glfwPollEvents());

			// Swap front and back buffers - show on screen
			GLCall(glfwSwapBuffers(window));
		}

		GLCall(glDeleteProgram(shader));
	}
	glfwTerminate();
	return 0;
}