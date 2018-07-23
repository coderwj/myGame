
static float vertices[48] = {
    -0.9f, -0.9f, 0.0f,        //pos
    0.0f,    0.0f,    1.0f,        //normal
    0.0f,    0.0f,    0.0f,        //textcoord
    255.0f,  0.0f,    0.0f,        //color


    0.9f,  -0.9f, 0.0f,        //pos
    0.0f,    0.0f,    1.0f,        //normal
    0.0f,    0.0f,    0.0f,        //textcoord
    255.0f,  0.0f,    0.0f,        //color

    0.9f,  0.9f,  0.0f,        //pos
    0.0f,    0.0f,    1.0f,        //normal
    0.0f,    0.0f,    0.0f,        //textcoord
    0.0f,  255.0f,    0.0f,        //color

    -0.9f, 0.9f,  0.0f,        //pos
    0.0f,    0.0f,    1.0f,        //normal
    0.0f,    0.0f,    0.0f,        //textcoord
    0.0f,  255.0f,    0.0f        //color
};
static unsigned int indices[6] = {
    0, 1, 2,
    2, 3, 0
};
static unsigned int VAO, VBO, EBO;

void test_shader_object::init()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(9 * sizeof(float)));

    glBindVertexArray(0);
}


void test_shader_object::render()
{
    glEnable(GL_DEPTH_TEST);
    string temp_vs_path = Config::engine_res_path + "shader/" + "test.vs";
    string temp_fs_path = Config::engine_res_path + "shader/" + "test.fs";
    static Shader * testShader = new Shader(temp_vs_path.c_str(), temp_fs_path.c_str());

    testShader->use();

    GameScene * gamescene = GameScene::getInstance();
    if (!gamescene)
        return;
    Camera * camera = gamescene->getCamera();
    if (!camera)
        return;
    Matrix4 projection = camera->GetProjectMatrix();
    Matrix4 view;
    view.initWithLookAt(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f), Vector3(0.0f, 1.0f, 0.0f));

    Matrix4 scaleM;
    scaleM.initWithScale(Vector3(1.0f));
    Matrix4 rotateM;
    rotateM.initWithRotate(Vector3(0.0f, 1.0f, 0.0f), 0.0f);
    Matrix4 transM;
    transM.initWithTranslate(Vector3(0.0f));

    Matrix4 model = scaleM * rotateM * transM;
    testShader->setMat4("model", model);
    testShader->setMat4("view", view);
    testShader->setMat4("projection", projection);
    testShader->setFloat("time", m_nowTime);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return;
}
