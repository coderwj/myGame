
#include "glew.h"
#include "glfw3.h"

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

#include "myEngineCore.h"


#include "gamescene.h"

#include "Shader.h"
#include "Model.h"


#include <thread>
#include <sstream>


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


typedef struct {
    std::vector<float> vb;  // vertex buffer
    int numTriangles;
    size_t material_id;
} DrawObject;

std::vector<DrawObject> gDrawObjects;

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
    float v10[3];
    v10[0] = v1[0] - v0[0];
    v10[1] = v1[1] - v0[1];
    v10[2] = v1[2] - v0[2];

    float v20[3];
    v20[0] = v2[0] - v0[0];
    v20[1] = v2[1] - v0[1];
    v20[2] = v2[2] - v0[2];

    N[0] = v20[1] * v10[2] - v20[2] * v10[1];
    N[1] = v20[2] * v10[0] - v20[0] * v10[2];
    N[2] = v20[0] * v10[1] - v20[1] * v10[0];

    float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
    if (len2 > 0.0f) {
        float len = sqrtf(len2);

        N[0] /= len;
        N[1] /= len;
    }
}

static bool LoadObjAndConvert(std::vector<DrawObject>* drawObjects,
                              std::vector<tinyobj::material_t>& materials,
                              std::map<std::string, GLuint>& textures,
                              const char* filename,
                              const char* basepath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;


    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, basepath);
    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        std::cerr << "Failed to load " << filename << std::endl;
        return false;
    }

    printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
    printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
    printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
    printf("# of materials = %d\n", (int)materials.size());
    printf("# of shapes    = %d\n", (int)shapes.size());

    // Append `default` material
    materials.push_back(tinyobj::material_t());

    // Load diffuse textures
    {
        for (size_t m = 0; m < materials.size(); m++) {
            tinyobj::material_t* mp = &materials[m];

            if (mp->diffuse_texname.length() > 0) {
                // Only load the texture if it is not already loaded
                if (textures.find(mp->diffuse_texname) == textures.end()) {
                    GLuint texture_id;
                    int w, h;
                    int comp;

                    //std::string texture_filename = basepath;
                    std::string texture_pathname = basepath;
                    std::string texture_filename = texture_pathname + mp->diffuse_texname;

                    unsigned char* image = stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
                    if (!image) {
                        std::cerr << "Unable to load texture: " << texture_filename << std::endl;
                        exit(1);
                    }
                    glGenTextures(1, &texture_id);
                    glBindTexture(GL_TEXTURE_2D, texture_id);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    if (comp == 3) {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
                    }
                    else if (comp == 4) {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
                    }
                    glBindTexture(GL_TEXTURE_2D, 0);
                    stbi_image_free(image);
                    textures.insert(std::make_pair(mp->diffuse_texname, texture_id));
                }
            }
        }
    }

    {
        for (size_t s = 0; s < shapes.size(); s++) {
            DrawObject o;
            std::vector<float> vb;  // pos(3float), normal(3float), color(3float)
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                if(shapes[s].mesh.num_face_vertices[f] != 3)
                    continue;
                tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
                tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
                tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

                int current_material_id = shapes[s].mesh.material_ids[f];

                if ((current_material_id < 0) || (current_material_id >= static_cast<int>(materials.size()))) {
                    // Invaid material ID. Use default material.
                    current_material_id = materials.size() - 1; // Default material is added to the last item in `materials`.
                }
                float diffuse[3];
                for (size_t i = 0; i < 3; i++) {
                    diffuse[i] = materials[current_material_id].diffuse[i];
                }
                float tc[3][2];
                if (attrib.texcoords.size() > 0 && idx0.texcoord_index != -1) {
                    assert(attrib.texcoords.size() > 2 * idx0.texcoord_index + 1);
                    assert(attrib.texcoords.size() > 2 * idx1.texcoord_index + 1);
                    assert(attrib.texcoords.size() > 2 * idx2.texcoord_index + 1);
                    tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
                    tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
                    tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
                    tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
                    tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
                    tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
                } else {
                    tc[0][0] = 0.0f;
                    tc[0][1] = 0.0f;
                    tc[1][0] = 0.0f;
                    tc[1][1] = 0.0f;
                    tc[2][0] = 0.0f;
                    tc[2][1] = 0.0f;
                }

                float v[3][3];
                for (int k = 0; k < 3; k++) {
                    int f0 = idx0.vertex_index;
                    int f1 = idx1.vertex_index;
                    int f2 = idx2.vertex_index;
                    assert(f0 >= 0);
                    assert(f1 >= 0);
                    assert(f2 >= 0);

                    v[0][k] = attrib.vertices[3 * f0 + k];
                    v[1][k] = attrib.vertices[3 * f1 + k];
                    v[2][k] = attrib.vertices[3 * f2 + k];
                }

                float n[3][3];
                if (attrib.normals.size() > 0) {
                    int f0 = idx0.normal_index;
                    int f1 = idx1.normal_index;
                    int f2 = idx2.normal_index;
                    assert(f0 >= 0);
                    assert(f1 >= 0);
                    assert(f2 >= 0);
                    for (int k = 0; k < 3; k++) {
                        n[0][k] = attrib.normals[3 * f0 + k];
                        n[1][k] = attrib.normals[3 * f1 + k];
                        n[2][k] = attrib.normals[3 * f2 + k];
                    }
                } else {
                    // compute geometric normal
                    CalcNormal(n[0], v[0], v[1], v[2]);
                    n[1][0] = n[0][0];
                    n[1][1] = n[0][1];
                    n[1][2] = n[0][2];
                    n[2][0] = n[0][0];
                    n[2][1] = n[0][1];
                    n[2][2] = n[0][2];
                }

                for (int k = 0; k < 3; k++) {
                    vb.push_back(v[k][0]);
                    vb.push_back(v[k][1]);
                    vb.push_back(v[k][2]);
                    vb.push_back(n[k][0]);
                    vb.push_back(n[k][1]);
                    vb.push_back(n[k][2]);
                    // Combine normal and diffuse to get color.
                    float normal_factor = 0.2f;
                    float diffuse_factor = 1 - normal_factor;
                    float c[3] = {
                        n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
                        n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
                        n[k][2] * normal_factor + diffuse[2] * diffuse_factor
                    };
                    float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
                    if (len2 > 0.0f) {
                        float len = sqrtf(len2);

                        c[0] /= len;
                        c[1] /= len;
                        c[2] /= len;
                    }
                    vb.push_back(c[0] * 0.5f + 0.5f);
                    vb.push_back(c[1] * 0.5f + 0.5f);
                    vb.push_back(c[2] * 0.5f + 0.5f);

                    vb.push_back(tc[k][0]);
                    vb.push_back(tc[k][1]);
                }
            }
            o.vb = vb;

            o.numTriangles = 0;

            // OpenGL viewer does not support texturing with per-face material.
            if (shapes[s].mesh.material_ids.size() > 0 && shapes[s].mesh.material_ids.size() > s) {
                // Base case
                o.material_id = shapes[s].mesh.material_ids[s];
            } else {
                o.material_id = materials.size() - 1; // = ID for default material.
            }

            if (vb.size() > 0) {
                o.numTriangles = vb.size() / (3 + 3 + 3 + 2) * 3;
            }

            drawObjects->push_back(o);
        }
    }

    return true;
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

	std::string engine_res_path = "../../../../../../myEngine/res/";
	std::string game_res_path = "../../../../../../res/";
#ifdef WIN32
	engine_res_path = "../../../../../myEngine/res/";
	game_res_path = "../../../../../res/";
#endif
    std::string model_path = game_res_path + "models/";
    std::string scene_path = game_res_path + "scenes/";
    
    std::string obj_file_path = model_path + "character1/Dukemon-Final-2.obj";
    std::string obj_base_path = model_path + "character1/";

	char filename[2048];
	char basepath[2048];
	strncpy(filename, obj_file_path.c_str(), 2048);
	strncpy(basepath, obj_base_path.c_str(), 2048);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::material_t> materials;
    std::map<std::string, GLuint> textures;
    if (!LoadObjAndConvert(&gDrawObjects, materials, textures, filename, basepath)) {
        return false;
    }

    Model test_model(obj_file_path);

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

    // program and shader handles
    Shader shader = Shader(vs_path.c_str(), fs_path.c_str());

    // obtain location of projection uniform
	//GLint Model_location = glGetUniformLocation(shader.ID, "Model");
    //GLint View_location = glGetUniformLocation(shader.ID, "View");
    //GLint Projection_location = glGetUniformLocation(shader.ID, "Projection");

    glm::vec3 model_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	model = glm::mat4(1.0);
	model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, model_pos);

    // calculate ViewProjection matrix
    projection = glm::perspective(90.0f, 4.0f / 3.0f, 0.1f, 100.f);

    // translate the world/view position  
    glm::vec3 cameraPos   = glm::vec3(4.0f, 7.0f,  5.0f);
    glm::vec3 cameraTarget = model_pos + glm::vec3(0.0f, 3.5f, 0.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    // make the camera rotate around the origin
//    View = glm::rotate(View, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//    View = glm::rotate(View, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
//    View = glm::rotate(View, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
//
//    View = glm::translate(View, glm::vec3(0.0f, -4.0f, -6.0f));

    // vao and vbo handle
    // GLuint vao, vbo;

    // // generate and bind the vao
    // glGenVertexArrays(1, &vao);
    // glBindVertexArray(vao);

    // // generate and bind the buffer object
    // glGenBuffers(1, &vbo);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        update_view();
        glfwPollEvents();
        // glEnableVertexAttribArray(0);
        // glEnableVertexAttribArray(1);
        // glEnableVertexAttribArray(2);
        // glEnableVertexAttribArray(3);


        glClearColor(0.0, 0.0, 1.0, 1.0);
        // clear first
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // use the shader program
        shader.use();

		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		test_model.Draw(shader);

   //      // bind the vao
   //      glBindVertexArray(vao);
        
   //      Projection = glm::perspective(fov, 4.0f / 3.0f, 0.1f, 10000.f);

   //      // set the uniform
   //      glUniformMatrix4fv(Model_location, 1, GL_FALSE, glm::value_ptr(Model));
   //      glUniformMatrix4fv(View_location, 1, GL_FALSE, glm::value_ptr(View));
   //      glUniformMatrix4fv(Projection_location, 1, GL_FALSE, glm::value_ptr(Projection));

   //      GLsizei stride = (3 + 3 + 3 + 2) * sizeof(float);
   //      for (size_t i = 0; i < gDrawObjects.size(); i++) {
   //          DrawObject o = gDrawObjects[i];
   //          if (o.vb.size() == 0) {
   //              continue;
   //          }

   //          //glBindBuffer(GL_ARRAY_BUFFER, o.vb);

   //          if ((o.material_id < materials.size())) {
   //              std::string diffuse_texname = materials[o.material_id].diffuse_texname;
   //              if (textures.find(diffuse_texname) != textures.end()) {
   //                  glBindTexture(GL_TEXTURE_2D, textures[diffuse_texname]);
   //              }
   //              else{
   //                  glBindTexture(GL_TEXTURE_2D, 0);
   //              }
   //          }
   //          else
   //          {
   //              continue;
   //          }
			// shader.use();

   //          glBufferData(GL_ARRAY_BUFFER, o.vb.size() * sizeof(GLfloat), &o.vb[0], GL_STATIC_DRAW);
   //          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (char*)0);
   //          glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, stride, (char*)(3 * sizeof(float)));
   //          glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, stride, (char*)(6 * sizeof(float)));
   //          glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, stride, (char*)(9 * sizeof(float)));

   //          glDrawArrays(GL_TRIANGLES, 0, o.numTriangles * 3);
   //      }


        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
            std::cerr << error << std::endl;
            break;
        }
        
        // glDisableVertexAttribArray(0);
        // glDisableVertexAttribArray(1);
        // glDisableVertexAttribArray(2);
        // glDisableVertexAttribArray(3);
        // finally swap buffers
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