//
//  MyTriangle.cpp
//  Triangle
//
//  Created by coderwj on 16/10/15.
//  Copyright © 2016年 coderwj. All rights reserved.
//

#include <GLTools.h>
#include <GLShaderManager.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

GLBatch triangleBatch;
GLShaderManager shaderManager;

GLfloat r;
GLfloat g;
GLfloat b;

GLfloat dsize = 0.025f;

GLfloat v[12] = {0.0f};

void reshape(int w, int h){
    glViewport(0, 0, w, h);
}

void initRenderContext(){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    shaderManager.InitializeStockShaders();

    GLfloat vVerts[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };
    
    for(int i = 0; i < 12; i++){
        v[i] = vVerts[i];
    }

    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    triangleBatch.CopyVertexData3f(v);
    triangleBatch.End();
    
    r = 1.0f;
    g = 0.0f;
    b = 0.0f;

}

void update(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    GLfloat vRed[] = {r, g, b, 1.0f};
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
    triangleBatch.Draw();

    glutSwapBuffers();
    b += dsize;
    glutPostRedisplay();
}

void input(unsigned char c, int x, int y){
    if(c == 'r'){
        r = 1.0f;
        g = 0.0f;
        b = 0.0f;
    }
    else if(c == 'g'){
        r = 0.0f;
        g = 1.0f;
        b = 0.0f;
    }
//    else if(c == 'b'){
//        r = 0.0f;
//        g = 0.0f;
//        b = 1.0f;
//    }
    glutPostRedisplay();
}

void special_input(int key, int x, int y){
    if(key == GLUT_KEY_UP){
        v[1] += dsize;
        v[4] += dsize;
        v[7] += dsize;
        v[10] += dsize;
    }
    else if(key == GLUT_KEY_DOWN){
        v[1] -= dsize;
        v[4] -= dsize;
        v[7] -= dsize;
        v[10] -= dsize;
    }
    triangleBatch.CopyVertexData3f(v);
    glutPostRedisplay();
}

int main(int argc, char * argv[]){
    gltSetWorkingDirectory(argv[0]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(960, 640);
    glutCreateWindow("MyTriangle");
    glutReshapeFunc(reshape);
    glutDisplayFunc(update);
    glutKeyboardFunc(input);
    glutSpecialFunc(special_input);

    GLenum state = glewInit();
    if(GLEW_OK != state){
        fprintf(stderr, "GLEW INIT ERROR: %s/n", glewGetErrorString(state));
        return 1;
    }

    initRenderContext();

    glutMainLoop();
    return 0;
}