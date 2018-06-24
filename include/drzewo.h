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
#include "lisc.h"
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
        int il_galezi = 2;
        int il_galezi_w_pierwszym_pokoleniu = 4; // w pierwszym pokoleniu
        int il_pokolen = 8;
        int rozmiar = 1024;
        galaz galezie[1024];

        int wysokosc_drzewa;
        float grubosc_drzewa=0.4;


        int pobierz_numer_matki(int index);
        void rysuj_galaz(mat4 P, mat4 V, float wzrost,  int numer_galezi_w_tablicy, ShaderProgram *shaderProgram, GLuint k_diffTex,GLuint l_diffTex, int numer_pokolenia, lisc leaf);
        void rysuj_drzewo(mat4 P, mat4 Mz, mat4 V, float wzrost, ShaderProgram *shaderProgram,GLuint k_diffTex, GLuint l_diffTex, lisc leaf);
        int losuj(int mini, int maxi);
        void losuj_nowe_pokolenie_galezi(int nr_pokolenia);
        void rysuj_stozek(mat4 P, mat4 V, mat4 M, float h, float r, ShaderProgram *shaderProgram, GLuint k_diffTex);
};

#endif // DRZEWO_H
