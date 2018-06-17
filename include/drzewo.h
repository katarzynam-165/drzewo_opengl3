#ifndef DRZEWO_H
#define DRZEWO_H
#include <list>
#include "galaz.h"
#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "allmodels.h"
#include "cone.h"
#include "shaderprogram.h"

using namespace glm;

using namespace:: std;
class drzewo
{
private:
    ShaderProgram* shaderProgram;
    public:
        GLuint k_diffTex; //Zmienna reprezentujaca teksturê
        mat4 M_Pnia;
        cone stozek = cone();

        drzewo(GLuint d);
        virtual ~drzewo();
        void losuj_wysokosc_drzewa();
        int il_galezi=8; // w pierwszym pokoleniu
        galaz galezie[32];
        int wysokosc_drzewa;
        int grubosc_drzewa;

        void rysuj_galaz(mat4 P, mat4 V, float wzrost,  int numer_galezi_w_tablicy, ShaderProgram *shaderProgram);
        void rysuj_drzewo(mat4 P, mat4 Mz, mat4 V, float wzrost, ShaderProgram *shaderProgram,GLuint k_diffTex);
        int losuj(int mini, int maxi);
        void rysuj_prostopadloscian(mat4 P, mat4 V, mat4 M, float a, float b, float c, ShaderProgram *shaderProgram);
        void losuj_nowe_pokolenie_galezi(int nr_pokolenia);
        void rysuj_stozek(ShaderProgram *shaderProgram, mat4 mP, mat4 mV, mat4 mM);
};

#endif // DRZEWO_H
