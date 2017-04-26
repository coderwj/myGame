
#include "glew.h"
#include "glfw3.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>


#include "gamescene.h"

GameScene * GameScene::gs = 0;

static void PrintInfo(const tinyobj::attrib_t& attrib,
                      const std::vector<tinyobj::shape_t>& shapes,
                      const std::vector<tinyobj::material_t>& materials) {
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
    << std::endl;

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;

    for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
        printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
               static_cast<const double>(attrib.vertices[3 * v + 0]),
               static_cast<const double>(attrib.vertices[3 * v + 1]),
               static_cast<const double>(attrib.vertices[3 * v + 2]));
    }

    for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
        printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
               static_cast<const double>(attrib.normals[3 * v + 0]),
               static_cast<const double>(attrib.normals[3 * v + 1]),
               static_cast<const double>(attrib.normals[3 * v + 2]));
    }

    for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
        printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
               static_cast<const double>(attrib.texcoords[2 * v + 0]),
               static_cast<const double>(attrib.texcoords[2 * v + 1]));
    }

    // For each shape
    for (size_t i = 0; i < shapes.size(); i++) {
        printf("shape[%ld].name = %s\n", static_cast<long>(i),
               shapes[i].name.c_str());
        printf("Size of shape[%ld].indices: %lu\n", static_cast<long>(i),
               static_cast<unsigned long>(shapes[i].mesh.indices.size()));

        size_t index_offset = 0;

        assert(shapes[i].mesh.num_face_vertices.size() ==
               shapes[i].mesh.material_ids.size());

        printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
               static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

        // For each face
        for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
            size_t fnum = shapes[i].mesh.num_face_vertices[f];

            printf("  face[%ld].fnum = %ld\n", static_cast<long>(f),
                   static_cast<unsigned long>(fnum));

            // For each vertex in the face
            for (size_t v = 0; v < fnum; v++) {
                tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
                printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
                       static_cast<long>(v), idx.vertex_index, idx.normal_index,
                       idx.texcoord_index);
            }

            printf("  face[%ld].material_id = %d\n", static_cast<long>(f),
                   shapes[i].mesh.material_ids[f]);

            index_offset += fnum;
        }

        printf("shape[%ld].num_tags: %lu\n", static_cast<long>(i),
               static_cast<unsigned long>(shapes[i].mesh.tags.size()));
        for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
            printf("  tag[%ld] = %s ", static_cast<long>(t),
                   shapes[i].mesh.tags[t].name.c_str());
            printf(" ints: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
                printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
                if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
                    printf(", ");
                }
            }
            printf("]");

            printf(" floats: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
                printf("%f", static_cast<const double>(
                                                       shapes[i].mesh.tags[t].floatValues[j]));
                if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
                    printf(", ");
                }
            }
            printf("]");

            printf(" strings: [");
            for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
                printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
                if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
                    printf(", ");
                }
            }
            printf("]");
            printf("\n");
        }
    }

    for (size_t i = 0; i < materials.size(); i++) {
        printf("material[%ld].name = %s\n", static_cast<long>(i),
               materials[i].name.c_str());
        printf("  material.Ka = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].ambient[0]),
               static_cast<const double>(materials[i].ambient[1]),
               static_cast<const double>(materials[i].ambient[2]));
        printf("  material.Kd = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].diffuse[0]),
               static_cast<const double>(materials[i].diffuse[1]),
               static_cast<const double>(materials[i].diffuse[2]));
        printf("  material.Ks = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].specular[0]),
               static_cast<const double>(materials[i].specular[1]),
               static_cast<const double>(materials[i].specular[2]));
        printf("  material.Tr = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].transmittance[0]),
               static_cast<const double>(materials[i].transmittance[1]),
               static_cast<const double>(materials[i].transmittance[2]));
        printf("  material.Ke = (%f, %f ,%f)\n",
               static_cast<const double>(materials[i].emission[0]),
               static_cast<const double>(materials[i].emission[1]),
               static_cast<const double>(materials[i].emission[2]));
        printf("  material.Ns = %f\n",
               static_cast<const double>(materials[i].shininess));
        printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
        printf("  material.dissolve = %f\n",
               static_cast<const double>(materials[i].dissolve));
        printf("  material.illum = %d\n", materials[i].illum);
        printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
        printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
        printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
        printf("  material.map_Ns = %s\n",
               materials[i].specular_highlight_texname.c_str());
        printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
        printf("    bump_multiplier = %f\n", static_cast<const double>(materials[i].bump_texopt.bump_multiplier));
        printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
        printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
        printf("  <<PBR>>\n");
        printf("  material.Pr     = %f\n", static_cast<const double>(materials[i].roughness));
        printf("  material.Pm     = %f\n", static_cast<const double>(materials[i].metallic));
        printf("  material.Ps     = %f\n", static_cast<const double>(materials[i].sheen));
        printf("  material.Pc     = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
        printf("  material.Pcr    = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
        printf("  material.aniso  = %f\n", static_cast<const double>(materials[i].anisotropy));
        printf("  material.anisor = %f\n", static_cast<const double>(materials[i].anisotropy_rotation));
        printf("  material.map_Ke = %s\n", materials[i].emissive_texname.c_str());
        printf("  material.map_Pr = %s\n", materials[i].roughness_texname.c_str());
        printf("  material.map_Pm = %s\n", materials[i].metallic_texname.c_str());
        printf("  material.map_Ps = %s\n", materials[i].sheen_texname.c_str());
        printf("  material.norm   = %s\n", materials[i].normal_texname.c_str());
        std::map<std::string, std::string>::const_iterator it(
                                                              materials[i].unknown_parameter.begin());
        std::map<std::string, std::string>::const_iterator itEnd(
                                                                 materials[i].unknown_parameter.end());

        for (; it != itEnd; it++) {
            printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
        }
        printf("\n");
    }
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

// helper to check and display for shader compiler errors
bool check_shader_compile_status(GLuint obj) {
    GLint status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}

// helper to check and display for shader linker error
bool check_program_link_status(GLuint obj) {
    GLint status;
    glGetProgramiv(obj, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetProgramInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}

GLuint LoadShaders(GLFWwindow *window, const char * vertex_file_path, const char * fragment_file_path){

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += Line + "\n";
        VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += Line + "\n";
        FragmentShaderStream.close();
    }

    // Compile Vertex Shader
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Compile Fragment Shader
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    if(!check_shader_compile_status(VertexShaderID) || !check_shader_compile_status(FragmentShaderID)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }


    // Link the program
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    if(!check_program_link_status(ProgramID)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
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
    GLFWwindow *window;
    if((window = glfwCreateWindow(width, height, "01shader_vbo1", 0, 0)) == 0) {
        std::cerr << "failed to open window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK) {
        std::cerr << "failed to init GLEW" << std::endl;
        return false;
    }

    const char* filename = "../../../../../../res/models/character1/Dukemon-Final-2.obj";
    const char* basepath = "../../../../../../res/models/character1/";
    bool triangulate = false;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename,
                              basepath, triangulate);
    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        std::cout << "Failed to load .obj." << std::endl;
        return false;
    }

    //PrintInfo(attrib, shapes, materials);


    std::string vs_path = "../../../../../../myEngine/res/shader/common.vs";
    std::string ps_path = "../../../../../../myEngine/res/shader/common.ps";

    // program and shader handles
    GLuint shader_program = LoadShaders(window, vs_path.c_str(), ps_path.c_str());
    
    // obtain location of projection uniform
    GLint Model_location = glGetUniformLocation(shader_program, "Model");
    GLint View_location = glGetUniformLocation(shader_program, "View");
    GLint Projection_location = glGetUniformLocation(shader_program, "Projection");
    
    glm::mat4 Model = glm::mat4(1.0);
    Model = glm::rotate(Model, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    Model = glm::rotate(Model, 50.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::rotate(Model, -150.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    // calculate ViewProjection matrix
    glm::mat4 Projection = glm::perspective(90.0f, 4.0f / 3.0f, 0.1f, 100.f);

    // translate the world/view position
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, -20.0f));

    // make the camera rotate around the origin
    View = glm::rotate(View, 130.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, -122.5f, glm::vec3(0.0f, 1.0f, 0.0f));

    // vao and vbo handle
    GLuint vao, vbo;

    // generate and bind the vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // generate and bind the buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // set up generic attrib pointers
    glEnableVertexAttribArray(0);


    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // clear first
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader program
        glUseProgram(shader_program);

        // bind the vao
        glBindVertexArray(vao);
        
        // set the uniform
        glUniformMatrix4fv(Model_location, 1, GL_FALSE, glm::value_ptr(Model));
        glUniformMatrix4fv(View_location, 1, GL_FALSE, glm::value_ptr(View));
        glUniformMatrix4fv(Projection_location, 1, GL_FALSE, glm::value_ptr(Projection));
        
        for (size_t i = 0; i < shapes.size(); i++)
        {
            size_t index_offset = 0;
            for(size_t j = 0; j < shapes[i].mesh.num_face_vertices.size(); j++)
            {
                size_t fnum = shapes[i].mesh.num_face_vertices[j];
                GLfloat vertexData[fnum * 3];
                for(size_t k = 0; k < fnum; k++)
                {
                    tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + k];
                    vertexData[3 * k + 0] = attrib.vertices[idx.vertex_index + 0];
                    vertexData[3 * k + 1] = attrib.vertices[idx.vertex_index + 1];
                    vertexData[3 * k + 2] = attrib.vertices[idx.vertex_index + 2];
                    
                }
                glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fnum * 3, vertexData, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, fnum * sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));
                glDrawArrays(GL_TRIANGLES, 0, fnum);
                index_offset = index_offset + fnum;
                
            }
        }

        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
            std::cerr << error << std::endl;
            break;
        }

        // finally swap buffers
        glfwSwapBuffers(window);
    }

    // delete the created objects

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glDeleteProgram(shader_program);

    glfwDestroyWindow(window);
    glfwTerminate();
    return true;
}