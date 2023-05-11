#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//lab 2
#include "helper/torus.h"
#include "helper/teapot.h"
#include "helper/plane.h"
#include <glm/glm.hpp>
#include "helper/objmesh.h"
#include "helper/skybox.h"
#include "helper/texture.h"
#include "helper/noisetex.h"



class SceneBasic_Uniform : public Scene
{
private:
    glm::mat4 rotationMatrix;
    
    GLSLProgram prog;

    //
    GLuint quad;
    glm::vec3 lightPos;
    void drawScene();

    //Torus torus;
    //Teapot teapot;
    Plane plane;
    std::unique_ptr<ObjMesh> mesh;
    void setMatrices();
    void compile();
    void yMovement(float, float, bool);
    SkyBox sky;
    float angle, tPrev, rotSpeed;
    GLuint textures[2];

public:
    SceneBasic_Uniform();
    bool isSkyBox = false;
    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
