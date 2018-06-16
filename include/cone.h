#ifndef CONE_H
#define CONE_H
#include "shaderprogram.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class cone
{

    public:
        cone();
        int vertexCount;
        float *vertices;
        float *texCoords;
        float *normals;
        void drawCone(mat4 P, mat4 V, mat4 M, ShaderProgram *shaderProgram, GLuint k_diffTex);



        virtual ~cone();

    protected:

    private:
};

namespace coneNamespace {
            extern float vertices[];
            extern float texCoords[];
            extern float normals[];
        };

#endif // CONE_H
