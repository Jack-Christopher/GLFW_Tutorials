#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// camera system
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// rotation
float xRot = 0.0f ;
float yRot = 0.0f ;
float zRot = 0.0f ;


namespace opengl
{
	void init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}


	GLFWwindow* createWindow(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, const char *title)
	{
		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		return window;
	}

	void loadGlad()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			exit(EXIT_FAILURE);
		}
	}


     void prepare_VBO_VAO(unsigned int& VBO, unsigned int& VAO, float vertices[], int size_of_vertices)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, size_of_vertices, vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }


    void draw(Shader &shader, unsigned int& VAO, glm::mat4 proj, glm::vec3 &color)
    {
        shader.use();

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::rotate(model, xRot, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, yRot, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, zRot, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 MVP = proj * view * model;

        shader.setMat4("view", view);
        shader.setMat4("u_MVP", MVP);

        // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO);

        shader.setVec3("u_faceColor", color);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0); // no need to unbind it every time 
    }


	float getDeltaTime()
	{
		static float lastTime{ 0.0f };
		float currentTime{ static_cast<float>(glfwGetTime()) };
		float deltaTime{ currentTime - lastTime };
		lastTime = currentTime;
		return deltaTime;
	}
}