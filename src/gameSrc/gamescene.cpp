
#include "glew.h"
#include "glfw3.h"

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

#include "MyEngineCore.h"


#include "GameScene.h"

#include "Shader.h"
#include "Model.h"


#include <thread>
#include <sstream>

#include "Config.h"
#include "Character.h"

#ifdef WIN32
    static const Config::engine_res_path = "../../../../../myEngine/res/";
    static const Config::game_res_path = "../../../../../res/";
    static const Config::model_path = Config::game_res_path + "models/";
    static const Config::scene_path = Config::game_res_path + "scenes/";
#else
    static const Config::engine_res_path = "../../../../../../myEngine/res/";
    static const Config::game_res_path = "../../../../../../res/";
    static const Config::model_path = Config::game_res_path + "models/";
    static const Config::scene_path = Config::game_res_path + "scenes/";
#endif

GLFWwindow * window;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, -1.0f, 0.0f);

float fov = 90.0f;


float speed_r_y = 0.0f;
float rotateAngle = 0.0f;

GameScene * GameScene::gs = 0;


void update_view()
{
    if (speed_r_y < 0.000001f && speed_r_y > -0.000001f)
    {
        return;
    }
    rotateAngle += speed_r_y;
    while (rotateAngle > 360.0f)
    {
        rotateAngle -= 360.0f;
    }
    while (rotateAngle < 0.0f)
    {
        rotateAngle += 360.0f;
    }
    GLfloat camX = sin(rotateAngle / 180.0f * float(M_PI)) * 1.0f;
    GLfloat camY = 0.0f;
    GLfloat camZ = cos(rotateAngle / 180.0f * float(M_PI)) * 1.0f;
    glm::vec3 cameraFront = glm::vec3(camX, camY, camZ);
    cameraPos = cameraTarget - cameraFront;
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 90.0f)
        fov -= float(yoffset);
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 90.0f)
        fov = 90.0f;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_A)
        {
            speed_r_y = -1.0f;
        }
        if (key == GLFW_KEY_D)
        {
            speed_r_y = 1.0f;
        }
    }
    else if(action == GLFW_RELEASE)
    {
        speed_r_y = 0.0f;
    }
}

bool GameScene::init(){
    int width = 640;
    int height = 480;


    if(glfwInit() == GL_FALSE) {
        std::cerr << "failed to init GLFW" << std::endl;
        return false;
    }

    glfwSetErrorCallback(error_callback);

    // select opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    if((window = glfwCreateWindow(width, height, "myGame", 0, 0)) == 0) {
        std::cerr << "failed to open window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK) {
        std::cerr << "failed to init GLEW" << std::endl;
        return false;
    }
    
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback); 

    mainCharacter = Character::Create("model_1");
    
    // std::string obj_file_path = Config::model_path + "character1/Dukemon-Final-2.obj";

    // Model test_model(obj_file_path);

    std::string material_name = "texture";
    std::string shader_config_path = engine_res_path + "shader/shaderConfig.xml";
    tinyxml2::XMLDocument shader_doc;
    shader_doc.LoadFile(shader_config_path.c_str());
    
    std::string vs_name = "", fs_name = "";

    tinyxml2::XMLElement* materialElement = shader_doc.FirstChildElement("shaderconfig")->FirstChildElement( "material" );
    for (;;materialElement = materialElement->NextSiblingElement("material")) {
        if(materialElement == NULL)
        {
            break;
        }
        if(material_name == materialElement->FirstChildElement("name")->GetText())
        {
            vs_name = materialElement->FirstChildElement("vertexshader")->GetText();
            fs_name = materialElement->FirstChildElement("fragmentshader")->GetText();
            break;
        }
    }
    if(vs_name == "" || fs_name == "")
    {
        assert(true);
    }


    std::string vs_path = engine_res_path + "shader/" + vs_name;
    std::string fs_path = engine_res_path + "shader/" + fs_name;

    Shader shader = Shader(vs_path.c_str(), fs_path.c_str());

    glm::vec3 model_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	model = glm::mat4(1.0);
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, model_pos);

    glm::vec3 cameraPos   = glm::vec3(4.0f, 7.0f,  5.0f);
    glm::vec3 cameraTarget = model_pos + glm::vec3(0.0f, 3.5f, 0.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

	projection = glm::perspective(90.0f, 4.0f / 3.0f, 0.1f, 100.f);

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        update_view();
        glfwPollEvents();


        glClearColor(0.0, 0.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use the shader program
        shader.use();

		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		test_model.Draw(shader);

        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
            std::cerr << error << std::endl;
            break;
        }

        glfwSwapBuffers(window);
    }

    // delete the created objects

    //glDeleteVertexArrays(1, &vao);
    //glDeleteBuffers(1, &vbo);

    glDeleteProgram(shader.ID);

    glfwDestroyWindow(window);
    glfwTerminate();
    return true;
}
void GameScene::onDestroy()
{
    if(mainCharacter)
    {
        delete(mainCharacter);
        mainCharacter = NULL;
    }
}

void GameScene::render()
{
    if(mainCharacter)
    {
        mainCharacter->render();
    }
}