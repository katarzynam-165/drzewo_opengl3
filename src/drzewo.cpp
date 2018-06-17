#include "include/drzewo.h"
#include <cstdlib>
#include <time.h>
#define GLM_FORCE_RADIANS
#include "constants.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "allmodels.h"
#include "cube.h"
#include "cone.h"
#include "shaderprogram.h"

using namespace glm;
using namespace std;

int min_wys_drzewa = 3;
int max_wys_drzewa = 3;
int grubosc_drzewa_tmp=1;


drzewo::drzewo(GLuint d)
{

    srand (time(NULL));
    losuj_wysokosc_drzewa();
    grubosc_drzewa=grubosc_drzewa_tmp;
    int i;


    for (i=0;i<=(il_galezi);i++)
    {
        printf("i: %d \n",i);
        galezie [i]=galaz();
        galezie[i].numer_matki=0;
    }

   // losuj_nowe_pokolenie_galezi(1);
    //losuj_nowe_pokolenie_galezi(2);
    //losuj_nowe_pokolenie_galezi(3);

    k_diffTex=d; //Zmienna reprezentujaca teksturę
    //k_normalTex=n; //Zmienna reprezentujaca teksturę
    //k_heightTex=h; //Zmienna reprezentująca teksturę


}


int drzewo::losuj(int mini, int maxi)
{
    int range = maxi-mini + 1;
	return rand()%range + mini;
}


void drzewo::losuj_wysokosc_drzewa()
    {
     int r=losuj(min_wys_drzewa, max_wys_drzewa);
     wysokosc_drzewa=r;
     return;
    }


void drzewo:: rysuj_prostopadloscian(mat4 P, mat4 V, mat4 M, float a, float b, float c, ShaderProgram *shaderProgram){
    mat4 Mk = M;
    Mk = scale(Mk, vec3(a/2.0f, b/2.0f,c/2.0f));
    glLoadMatrixf(value_ptr(V*Mk));
    rysuj_stozek(shaderProgram, P, V, Mk);
}

void drzewo:: rysuj_stozek(ShaderProgram *shaderProgram, mat4 mP, mat4 mV, mat4 mM) {

	;
}



void drzewo::rysuj_galaz(mat4 P, mat4 V, float wzrost,  int numer_galezi_w_tablicy, ShaderProgram *shaderProgram)
{
    galaz G=galezie[numer_galezi_w_tablicy];
   mat4 M_Matki;
    if(G.numer_matki==0)
        M_Matki=M_Pnia;
    else
    {
        M_Matki=galezie[G.numer_matki].macierz_wlasna;
    }

    float wys_matki;

    if (G.numer_matki==0){
        wys_matki=float(wzrost*wysokosc_drzewa);
    }
    else{
        wys_matki=galezie[G.numer_matki].dlugosc_galezi;
    }

    mat4 M= mat4 (1.0);
    M = translate (M_Matki, vec3(0.0f,G.wys_galezi_na_matce*wys_matki/2,0.0f));
    M = rotate(M, G.kat_na_matce , glm::vec3(0.0f, 1.0f, 0.0f));
    M = rotate(M, G.kat_do_pionu, glm::vec3(1.0f, 0.0f, 0.0f));
    M = translate (M, vec3(0.0f, (G.dlugosc_galezi/2), 0.0f));
    galezie[numer_galezi_w_tablicy].macierz_wlasna=M;
    rysuj_prostopadloscian(P, V, M,  0.4f, G.dlugosc_galezi , 0.4f, shaderProgram);

    }

void drzewo::losuj_nowe_pokolenie_galezi(int nr_pokolenia)
    {

    int i; //czyli dla pokolenia 1 jest 9 do 16, dla pokolenia  17-24
    int start= nr_pokolenia*il_galezi+1;
    int stop = start+il_galezi-1;

    for (i=start;i<=stop;i++)
        {
        // losuje galaz matkę
        int matka_start; //dla 1 będzie 0, dla 2 będzie 8
        if (nr_pokolenia==1){matka_start=0;}//dla 1 będzie 0, dla 2 będzie 9, dla 3 bęzdie 17
        else {(nr_pokolenia-1)*il_galezi+1;}
        int matka_stop= matka_start+il_galezi; //dla 1 będzie 8, dla 2 bedzie 16
        int nr_matki=losuj(matka_start, matka_stop);
        printf("Losowanie matki:%d -  wyloswowany nr_matki: %d /n" ,i, nr_matki  );
        galaz g=galaz();
        g.numer_matki=nr_matki;
       // g.macierz_matki=galezie[nr_matki].macierz_wlasna;
        }

    }



void drzewo::rysuj_drzewo(mat4 P, mat4 V, mat4 M, float wzrost, ShaderProgram *shaderProgram, GLuint k_diffTex)
{


    //M = translate(M, vec3(0.0f, wzrost*float(wysokosc_drzewa)/2.0f, 0.0f));
    //rysuj_prostopadloscian(P, V, M, float(grubosc_drzewa), wzrost*float(wysokosc_drzewa), float(grubosc_drzewa), shaderProgram);
    stozek.drawCone(P,V,M,shaderProgram,k_diffTex);


    // to bedzie dla galezi
/*
    int i;
    M_Pnia=M;


   for (i=0;i<=il_galezi;i++)
        {
        galezie[i].macierz_matki=M;
        rysuj_galaz (P, V, wzrost, i, shaderProgram);
        }



 galezie[9].numer_matki=3;
 rysuj_galaz(P, V, wzrost, 9, shaderProgram);

 galezie[10].numer_matki=4;
 rysuj_galaz(P, V, wzrost, 10, shaderProgram);


// pokolenie 2

        i=il_galezi+1;
      //  int matka=3;
      //  galezie[i].macierz_matki=galezie[galezie[i].numer_matki].macierz_wlasna;
     //   rysuj_galaz (P, V, wzrost, i, shaderProgram);



   for (i=il_galezi+1;i<=2*il_galezi;i++)
        {

        }


/*
        for (i=5;i<=10-1;i++)
        {
       // rysuj_galaz( V, wzrost, i);
        }
*/

    };

drzewo::~drzewo()
    {
        //dtor
    }
