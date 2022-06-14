
#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// For the 1° triangle
const char* vertex_shader =
"#version 330 core \n"
"attribute vec3 in_position; \n"
"void main() \n"
"{ \n"
"     gl_Position = vec4(in_position.x, in_position.y, in_position.z, 1.0);\n"
"} \n";

const char* fragment_shader =
"#version 330 core \n"
"void main (void) \n"
"{ \n"
"     gl_FragColor = vec4(0.87,0.84,0.69, 1.0); \n"
"} \n";

// For the 2° triangle
const char* vertex_shader1 =
"#version 330 core \n"
"attribute vec3 in_position; \n"
"void main() \n"
"{ \n"
"     gl_Position = vec4(in_position.x, in_position.y, in_position.z, 1.0);\n"
"} \n";

const char* fragment_shader1 =
"#version 330 core \n"
"void main (void) \n"
"{ \n"
"     gl_FragColor = vec4(0.81,0.58,0.38, 1.0); \n"
"} \n";


int main()
{
    //Initialize GLFW
    if (!glfwInit()) //if GLFW is not initialized correctly, exit
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    //Create the window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) //if the window was not created correctly, exit
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Specify OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Make the context that was created the current one
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //Set the viewport size changing function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    //Rectangle vertices
    float vertices[] =
    {
        // first triangle 
        -0.5f, 0.5f, 0.0f,     // top left 
         0.5f, 0.5f, 0.0f,   // top right
        -0.5f, -0.5f, 0.0f,  // bottom left 

    };

    float vertices1[] =
    {

        // second triangle 
        -0.5f, -0.5f, 0.0f,   // bottom left 
         0.5f, -0.5f, 0.0f, // bottom right
         0.5f,  0.5f, 0.0f  // top right 
    };


    //Create the triangle VBO
    GLuint triangle_vbo;
    glGenBuffers(1, &triangle_vbo); //generate a unique buffer ID 
    glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo); //bind the VBO to the context
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind the VBO

    //Create a vertex shader object
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); //ID of the shader object
    glShaderSource(vertexShader, 1, &vertex_shader, NULL); //attach the shader source code to the shader object
    glCompileShader(vertexShader);//compile the shader

    //Create a fragment shader object
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
    glCompileShader(fragmentShader);

    //Create a shader program object
    GLuint shaderProgram;
    shaderProgram = glCreateProgram(); //create the program ID
    glAttachShader(shaderProgram, vertexShader); //attach the shaders to the program
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // link the program

    //Delete the vertex andd fragment shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Create the triangle VBO
    GLuint triangle_vbo1;
    glGenBuffers(1, &triangle_vbo1); //generate a unique buffer ID 
    glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo1); //bind the VBO to the context
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind the VBO

    //Create a vertex shader object
    GLuint vertexShader1;
    vertexShader1 = glCreateShader(GL_VERTEX_SHADER); //ID of the shader object
    glShaderSource(vertexShader1, 1, &vertex_shader1, NULL); //attach the shader source code to the shader object
    glCompileShader(vertexShader1);//compile the shader

    //Create a fragment shader object
    GLuint fragmentShader1;
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragment_shader1, NULL);
    glCompileShader(fragmentShader1);

    //Create a shader program object
    GLuint shaderProgram1;
    shaderProgram1 = glCreateProgram(); //create the program ID
    glAttachShader(shaderProgram1, vertexShader1); //attach the shaders to the program
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1); // link the program

    //Delete the vertex andd fragment shader objects
    glDeleteShader(vertexShader1);
    glDeleteShader(fragmentShader1);


    //Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        //Process input
        processInput(window);

        //Clear the buffers
        glClearColor(1.0f, 1.0f, 0.9f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);


        //Render objects...
        glUseProgram(shaderProgram); //use a shader program

        glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo); //bind the VBO

        //tell OpenGL how to read and assign the VBO to the attribute
        glUseProgram(shaderProgram);

        glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo); //bind the VBO
        GLint in_pos = glGetAttribLocation(shaderProgram, "in_position");
        glVertexAttribPointer(in_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
        glEnableVertexAttribArray(in_pos);
        glDrawArrays(GL_TRIANGLES, 0, 3); //draw the 1st triangle

        glUseProgram(shaderProgram1);

        glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo1);
        GLint in_pos1 = glGetAttribLocation(shaderProgram1, "in_position");
        glVertexAttribPointer(in_pos1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
        glEnableVertexAttribArray(in_pos1);
        glDrawArrays(GL_TRIANGLES, 0, 3); //draw the 2nd triangle


        //Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Terminate GLFW before exit
    glfwTerminate();
    return 0;
}


//Viewport size changing function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//Input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


