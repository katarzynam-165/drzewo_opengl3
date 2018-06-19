#ifndef LISC_H
#define LISC_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <ios>
#include <fstream>
#include <sstream>
#include <cstring>

#include "shaderprogram.h"

using namespace std;
using namespace glm;

class lisc
{
    public:
        lisc();
        virtual ~lisc();
        int wys_liscia;
        int szer_liscia;
        int typ_liscia;
        float* leaf_vertices;
        float* leaf_texCoords;
        float* leaf_normals;
        int vertexCount;



        void losuj_wys_liscia();
        void losuj_szer_liscia();
        void losuj_typ_liscia();
        void ustaw_teksture();
        int losuj (int mini, int maxi);

        void drawLeaf(mat4 P, mat4 V, mat4 M,ShaderProgram *shaderProgram, GLuint l_diffTex){
            glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(P));
            glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(V));
            glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(M));

            glUniform4f(shaderProgram->getUniformLocation("lp1"), 0, 0, -6, 1);
            glUniform4f(shaderProgram->getUniformLocation("lp2"), 0, 11, 0, 1);

            glUniform1i(shaderProgram->getUniformLocation("diffuseMap"),1);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D,l_diffTex);

            glEnableVertexAttribArray(shaderProgram->getAttribLocation("vertex"));
            glEnableVertexAttribArray(shaderProgram->getAttribLocation("normal"));
            glEnableVertexAttribArray(shaderProgram->getAttribLocation("texCoord0"));

            glVertexAttribPointer(shaderProgram->getAttribLocation("vertex"),3,GL_FLOAT,false,0,leaf_vertices);
            glVertexAttribPointer(shaderProgram->getAttribLocation("normal"),3,GL_FLOAT,false,0,leaf_normals);
            glVertexAttribPointer(shaderProgram->getAttribLocation("texCoord0"),2,GL_FLOAT,false,0,leaf_texCoords);


            //Narysowanie obiektu
            glDrawArrays(GL_TRIANGLES,0,vertexCount);

            glDisableVertexAttribArray(shaderProgram->getAttribLocation("vertex"));
            glDisableVertexAttribArray(shaderProgram->getAttribLocation("color"));
            glDisableVertexAttribArray(shaderProgram->getAttribLocation("normal"));
            glDisableVertexAttribArray(shaderProgram->getAttribLocation("texCoord0"));
        }
};

#endif // LISC_H
