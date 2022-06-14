
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
"     gl_FragColor = vec4(0.0,1.0,0.78, 1.0); \n"
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
"     gl_FragColor = vec4(0.19,0.67,0.87, 1.0); \n"
"} \n";

// For the 3° triangle
const char* vertex_shader2 =
"#version 330 core \n"
"attribute vec3 in_position; \n"
"void main() \n"
"{ \n"
"     gl_Position = vec4(in_position.x, in_position.y, in_position.z, 1.0);\n"
"} \n";

const char* fragment_shader2 =
"#version 330 core \n"
"void main (void) \n"
"{ \n"
"     gl_FragColor = vec4(0.26,0.57,0.61, 1.0); \n"
"} \n";

// For the 4° triangle
const char* vertex_shader3 =
"#version 330 core \n"
"attribute vec3 in_position; \n"
"void main() \n"
"{ \n"
"     gl_Position = vec4(in_position.x, in_position.y, in_position.z, 1.0);\n"
"} \n";

const char* fragment_shader3 =
"#version 330 core \n"
"void main (void) \n"
"{ \n"
"     gl_FragColor = vec4(0.14,0.47,0.51, 1.0); \n"
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tutorial_04", NULL, NULL);
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
        -0.5f, -0.25f, 0.0f,
         0.5f, -0.25f, 0.0f,
         0.0f,  0.5f, 0.0f, 

    };

    float vertices1[] =
    {
        // second triangle 
         0.0f,  0.5f, 0.0f, 
         0.5f,  0.25f, 0.0f, 
         0.5f, -0.25f, 0.0f  
    };

    float vertices2[] =
    {
        // third triangle 
        -0.5f, -0.25f, 0.0f, 
         0.5f, -0.25f, 0.0f, 
         0.0f, -0.5f, 0.0f  
    };

    float vertices3[] =
    {
        // fourth triangle 
        -0.5f, -0.25f, 0.0f, 
        -0.5f,  0.25f, 0.0f, 
         0.0f,  0.5f, 0.0f  
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

    //Create the triangle VBO
    GLuint triangle_vbo2;
    glGenBuffers(1, &triangle_vbo2); //generate a unique buffer ID 
    glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo2); //bind the VBO to the context
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind the VBO

    //Create a vertex shader object
    GLuint vertexShader2;
    vertexShader2 = glCreateShader(GL_VERTEX_SHADER); //ID of the shader object
    glShaderSource(vertexShader2, 1, &vertex_shader2, NULL); //attach the shader source code to the shader object
    glCompileShader(vertexShader2);//compile the shader

    //Create a fragment shader object
    GLuint fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragment_shader2, NULL);
    glCompileShader(fragmentShader2);

    //Create a shader program object
    GLuint shaderProgram2;
    shaderProgram2 = glCreateProgram(); //create the program ID
    glAttachShader(shaderProgram2, vertexShader2); //attach the shaders to the program
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2); // link the program

    //Delete the vertex andd fragment shader objects
    glDeleteShader(vertexShader2);
    glDeleteShader(fragmentShader2);

    //Create the triangle VBO
    GLuint triangle_vbo3;
    glGenBuffers(1, &triangle_vbo3); //generate a unique buffer ID 
    glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo3); //bind the VBO to the context
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind the VBO

    //Create a vertex shader object
    GLuint vertexShader3;
    vertexShader3 = glCreateShader(GL_VERTEX_SHADER); //ID of the shader object
    glShaderSource(vertexShader3, 1, &vertex_shader3, NULL); //attach the shader source code to the shader object
    glCompileShader(vertexShader3);//compile the shader

    //Create a fragment shader object
    GLuint fragmentShader3;
    fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader3, 1, &fragment_shader3, NULL);
    glCompileShader(fragmentShader3);

    //Create a shader program object
    GLuint shaderProgram3;
    shaderProgram3 = glCreateProgram(); //create the program ID
    glAttachShader(shaderProgram3, vertexShader3); //attach the shaders to the program
    glAttachShader(shaderProgram3, fragmentShader3);
    glLinkProgram(shaderProgram3); // link the program

    //Delete the vertex andd fragment shader objects
    glDeleteShader(vertexShader3);
    glDeleteShader(fragmentShader3);


    //Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        //Process input
        processInput(window);

        //Clear the buffers
        glClearColor(0.91f, 0.98f, 0.99f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

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

        glUseProgram(shaderProgram2);

        glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo2);
        GLint in_pos2 = glGetAttribLocation(shaderProgram2, "in_position");
        glVertexAttribPointer(in_pos2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
        glEnableVertexAttribArray(in_pos2);
        glDrawArrays(GL_TRIANGLES, 0, 3); //draw the 3rd triangle

        glUseProgram(shaderProgram3);

        glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo3);
        GLint in_pos3 = glGetAttribLocation(shaderProgram3, "in_position");
        glVertexAttribPointer(in_pos3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
        glEnableVertexAttribArray(in_pos3);
        glDrawArrays(GL_TRIANGLES, 0, 3); //draw the 4th triangle


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


