#include "scenebasic_uniform.h"
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/glutils.h"


using std::cerr;
using std::endl;
using std::string;
using glm::vec3;
using glm::mat4;

float yOfBall = -10.0f;
float yGoal = 1.0f;
bool top = false;

using glm::vec3;

//SceneBasic_Uniform::SceneBasic_Uniform() : plane(50.0f, 50.0f, 1, 1),
//                                           teapot(14, glm::mat4(1.0f)),
//                                           torus(1.75f * 0.75f ,0.75f * 0.75f, 50, 50)
//                                           
//{}

SceneBasic_Uniform::SceneBasic_Uniform() : plane(100.0f, 100.0f, 100, 100), angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f), sky(100.0f)
{
    mesh = ObjMesh::load("media/beachball.obj",
        true);
}

//void SceneBasic_Uniform::initScene()
//{
//    compile();
//    glEnable(GL_DEPTH_TEST);
//
//    view = glm::lookAt(vec3(5.0f, 5.0f, 7.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
//    projection = mat4(1.0f);
//
//    prog.setUniform("Spot.L", vec3(0.9f));
//    prog.setUniform("Spot.La", vec3(0.5f));
//    prog.setUniform("Spot.Exponent", 50.0f);
//    prog.setUniform("Spot.Cutoff", glm::radians(15.0f));
//}

void SceneBasic_Uniform::initScene()
{
    compile();

    textures[0] = Texture::loadCubeMap("media/Texture/cube/sky2/sky");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);

    textures[1] = NoiseTex::generate2DTex(6.0f);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    //
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    

    angle = glm::radians(90.0f);

    view = glm::lookAt(vec3(5.0f, 5.0f, 7.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);


    //
    GLfloat verts[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };
    GLfloat tc[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f

    };

    unsigned int handle[2];
    glGenBuffers(2, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

    glGenVertexArrays(1, &quad);
    glBindVertexArray(quad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    prog.setUniform("NoiseTex", 0);


    float x, z;
    for (int i = 0; i < 3; i++)
            {
                std::stringstream name;
                name << "lights[" << i << "].Position";
                x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
                z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
                prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 0.0f));
            }
        prog.setUniform("lights[0].La", vec3(0.0f, 0.0f, 0.1f));
        prog.setUniform("lights[1].La", vec3(0.0f, 0.1f, 0.0f));
        prog.setUniform("lights[2].La", vec3(0.1f, 0.0f, 0.0f));
    
        prog.setUniform("lights[0].Ld", vec3(0.0f, 0.0f, 1.8f));
        prog.setUniform("lights[1].Ld", vec3(0.0f, 1.8f, 0.0f));
        prog.setUniform("lights[2].Ld", vec3(1.8f, 0.0f, 0.0f));
    
        prog.setUniform("lights[0].Ls", vec3(0.0f, 0.0f, 0.8f));
        prog.setUniform("lights[1].Ls", vec3(0.0f, 0.8f, 0.0f));
        prog.setUniform("lights[2].Ls", vec3(0.8f, 0.0f, 0.0f));


        

}

//void SceneBasic_Uniform::initScene()
//{
//    compile();
//    glEnable(GL_DEPTH_TEST);
//    model = mat4(1.0f);
//    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
//    view = glm::lookAt(vec3(-1.0f, 6.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.1f, 0.0f));
//
//    projection = mat4(1.0f);
//
//    float x, z;
//    for (int i = 0; i < 3; i++)
//    {
//        std::stringstream name;
//        name << "lights[" << i << "].Position";
//        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
//        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
//        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 0.0f));
//    }
//    prog.setUniform("lights[0].La", vec3(0.0f, 0.0f, 0.1f));
//    prog.setUniform("lights[1].La", vec3(0.0f, 0.1f, 0.0f));
//    prog.setUniform("lights[2].La", vec3(0.1f, 0.0f, 0.0f));
//
//    prog.setUniform("lights[0].Ld", vec3(0.0f, 0.0f, 1.8f));
//    prog.setUniform("lights[1].Ld", vec3(0.0f, 1.8f, 0.0f));
//    prog.setUniform("lights[2].Ld", vec3(1.8f, 0.0f, 0.0f));
//
//    prog.setUniform("lights[0].Ls", vec3(0.0f, 0.0f, 0.8f));
//    prog.setUniform("lights[1].Ls", vec3(0.0f, 0.8f, 0.0f));
//    prog.setUniform("lights[2].Ls", vec3(0.8f, 0.0f, 0.0f));
//
//
//
//
//    prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
//   
//    prog.setUniform("Material.Ka", 0.9f, 0.9f, 0.9f);
//    
//    prog.setUniform("Material.Ks", 0.1f, 0.1f, 0.1f);
//    
//    prog.setUniform("Material.Shininess", 180.0f);
//
//}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
        
        float deltaT = t - tPrev;
        if (tPrev == 0.0f)
            deltaT = 0.0f;
        tPrev = t;
        angle += rotSpeed * deltaT;
        if (angle > glm::two_pi<float>())
            angle -= glm::two_pi<float>();
        

    
}

//void SceneBasic_Uniform::render()
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glm::vec4 lightPos = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
//    prog.setUniform("Spot.Position", vec3(view * lightPos));
//    glm::mat3 normalMatrix = glm::mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
//    prog.setUniform("Spot.Direction", normalMatrix * vec3(-lightPos));
//    prog.setUniform("Material.Kd", 0.2f, 0.55f, 0.9f);
//    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
//    prog.setUniform("Material.Ka", 0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f);
//    prog.setUniform("Material.Shininess", 100.0f);
//    model = mat4(1.0f);
//    model = glm::translate(model, vec3(0.0f, 0.0f, -2.0f));
//    model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
//    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
//    setMatrices();
//    teapot.render();
//    prog.setUniform("Material.Kd", 0.2f, 0.55f, 0.9f);
//    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
//    prog.setUniform("Material.Ka", 0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f);
//    prog.setUniform("Material.Shininess", 100.0f);
//    model = mat4(1.0f);
//    model = glm::translate(model, vec3(-1.0f, 0.75f, 3.0f));
//    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
//    setMatrices();
//    torus.render();
//    prog.setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
//    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
//    prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
//    prog.setUniform("Material.Shininess", 180.0f);
//    model = mat4(1.0f);
//    setMatrices();
//    plane.render();
//}

void SceneBasic_Uniform::render()
{
    view = mat4(1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    vec3  cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    prog.use();
    model = mat4(1.0f);
    prog.setUniform("noiseStrength", 0.1f);
    setMatrices();


    sky.render();


    //GLuint textures[1];
    //textures[0] = Texture::loadCubeMap("media/Texture/cube/sky2/sky");

    

    //prog.use();
    //model = mat4(1.0f);
    //setMatrices();
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
    //sky.render();


    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);    
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("noiseStrength", 0.0f);

    
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
    setMatrices();
    mesh->render();


    prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("noiseStrength", 0.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices();
    plane.render();

    //torus.render();
    //teapot.render();

    //glDeleteTextures(1, textures);

    glDeleteTextures(1, textures);
   /* drawScene();
    glFinish();*/
}

void SceneBasic_Uniform::drawScene()
{
    model = mat4(1.0f);
    setMatrices();
    glBindVertexArray(quad);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    
}

void SceneBasic_Uniform::setMatrices() {

    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ProjectionMatrix", projection);

}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);

}

void SceneBasic_Uniform::yMovement(float YMovement, float t, bool top) {
    
        yOfBall = YMovement * t;
        prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
        prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
        prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
        prog.setUniform("Material.Shininess", 180.0f);

        model = mat4(1.0f);
        model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, vec3(0.0f, yOfBall, 0.0f));
        model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
        setMatrices();
        mesh->render();

}

