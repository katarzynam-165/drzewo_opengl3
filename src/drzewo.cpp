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
#include "lisc.h"
#include "shaderprogram.h"
#include <math.h>

using namespace glm;
using namespace std;

int min_wys_drzewa = 2;
int max_wys_drzewa = 4;


drzewo::drzewo(GLuint d)
{

    srand (time(NULL));
    losuj_wysokosc_drzewa();
    int i;


    for (i=0;i<rozmiar;i++)
    {

        //printf("i: %d \n",i);
        galezie[i]=galaz();
        if(i<il_galezi_w_pierwszym_pokoleniu){
            galezie[i].ustaw(0,0);
            galezie[i].numer_matki=-1;
            //cout <<"1: " <<i<<"\t"<< galezie[i].numer_matki << endl;

        }else if(i>=il_galezi_w_pierwszym_pokoleniu & i<il_galezi*il_galezi_w_pierwszym_pokoleniu+il_galezi_w_pierwszym_pokoleniu) {
            galezie[i].numer_matki = int(floor((i/2) - 2));
            //cout <<"2: " <<i<<"\t"<< galezie[i].numer_matki << endl;
            //cout << galezie[pobierz_numer_matki(i)].dlugosc_galezi<< "\t"<< galezie[pobierz_numer_matki(i)].grubosc_galezi << endl;
            galezie[i].ustaw(galezie[int(floor((i/2) - 2))].dlugosc_galezi, galezie[int(floor((i/2) - 2))].grubosc_galezi);
           // cout << "i: "<<i << "\t"<< galezie[i].dlugosc_galezi << "\t"<< galezie[i].grubosc_galezi<< endl;
        } else if(i>=il_galezi*il_galezi_w_pierwszym_pokoleniu+il_galezi_w_pierwszym_pokoleniu){
            galezie[i].numer_matki = pobierz_numer_matki(i);
            //cout <<"3: " <<i<<"\t"<< galezie[i].numer_matki << endl;
            //cout << "i: "<<i << "\t"<< galezie[i].dlugosc_galezi << "\t"<< galezie[i].grubosc_galezi<< endl;
            galezie[i].ustaw(galezie[pobierz_numer_matki(i)].dlugosc_galezi, galezie[pobierz_numer_matki(i)].grubosc_galezi);
        }
    }



   // losuj_nowe_pokolenie_galezi(1);
    //losuj_nowe_pokolenie_galezi(2);
    //losuj_nowe_pokolenie_galezi(3);

    k_diffTex=d; //Zmienna reprezentujaca teksturę
    //k_normalTex=n; //Zmienna reprezentujaca teksturę
    //k_heightTex=h; //Zmienna reprezentująca teksturę


}

int drzewo::pobierz_numer_matki(int index)
{
    int numer_matki = int(floor((index-il_galezi_w_pierwszym_pokoleniu)/il_galezi));
    return numer_matki;
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


void drzewo:: rysuj_stozek(mat4 P, mat4 V, mat4 M, float h, float r, ShaderProgram *shaderProgram, GLuint k_diffTex){
    mat4 Mk = M;
    Mk= rotate(Mk, PI/2,vec3(-1.0f,0.0f,0.0f));
    //Mk = scale(Mk, vec3(r/2.0f, h/2.0f,r/2.0f));
    Mk = scale(Mk, vec3(r/2.0f, r/2.0f, h/2.0f));
    glLoadMatrixf(value_ptr(V*Mk));
    stozek.drawCone(P,V,Mk,shaderProgram,k_diffTex);
}

void drzewo::rysuj_lisc(mat4 M, mat4 P, mat4 V, ShaderProgram *shaderProgram, GLuint l_diffTex, lisc leaf, float dlugosc_galezi)
{
    mat4 M1 = M;
    M1 = translate(M1, vec3(0,0.4*dlugosc_galezi,0.3));
    M1 = rotate(M1,110 * PI / 180, vec3(-1,0,0));
    mat4 M_skalowane = scale(M1, vec3(0.002,0.002,0.002));
    leaf.drawLeaf(P,V,M_skalowane,shaderProgram,l_diffTex);

    mat4 M2 = M;
    M2 = translate(M2, vec3(0,0.2*dlugosc_galezi,0.2));
    M2 = rotate(M2,40 * PI / 180, vec3(-1,0,0));
    M_skalowane = scale(M2, vec3(0.0001,0.0001,0.0001));
    leaf.drawLeaf(P,V,M_skalowane,shaderProgram,l_diffTex);

}


void drzewo::rysuj_galaz(mat4 P, mat4 V, float wzrost,  int numer_galezi_w_tablicy, ShaderProgram *shaderProgram, GLuint k_diffTex, GLuint l_diffTex, int numer_pokolenia, lisc leaf)
{

    galaz G=galezie[numer_galezi_w_tablicy];
    mat4 M_Matki;

    if(numer_pokolenia==0){
        M_Matki=M_Pnia;
    }else if(numer_pokolenia == 1){
        G.numer_matki = floor((numer_galezi_w_tablicy/2) - 2);
        M_Matki=galezie[G.numer_matki].macierz_wlasna;
    }else if(numer_pokolenia == 2){
        G.numer_matki = floor(pobierz_numer_matki(numer_galezi_w_tablicy));
        M_Matki=galezie[G.numer_matki].macierz_wlasna;
    }

    float wys_matki;


    if (numer_pokolenia==0){
        wys_matki=float(wzrost*wysokosc_drzewa);
    }
    else {
        wys_matki=galezie[G.numer_matki].dlugosc_galezi;
    }


    mat4 M = mat4(1.0);

    M = translate(M_Matki, vec3(0.0f,G.wys_galezi_na_matce*wys_matki/2,0.0f));
    M = rotate(M, G.kat_na_matce , glm::vec3(0.0f, 1.0f, 0.0f));
    M = rotate(M, G.kat_do_pionu, glm::vec3(1.0f, 0.0f, 0.0f));
 //   M = translate (M, vec3(0.0f, (G.dlugosc_galezi/2), 0.0f));
    galezie[numer_galezi_w_tablicy].macierz_wlasna=M;
    rysuj_stozek(P, V, M, G.dlugosc_galezi, G.grubosc_galezi, shaderProgram, k_diffTex);
    rysuj_lisc(M, P, V, shaderProgram, l_diffTex, leaf, G.dlugosc_galezi);
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
        if (nr_pokolenia==1)
        {
            matka_start=0;
        }//dla 1 będzie 0, dla 2 będzie 9, dla 3 bęzdie 17
        else {
            (nr_pokolenia-1)*il_galezi+1;
        }

        int matka_stop= matka_start+il_galezi; //dla 1 będzie 8, dla 2 bedzie 16
        int nr_matki=losuj(matka_start, matka_stop);

        printf("Losowanie matki:%d -  wyloswowany nr_matki: %d /n" ,i, nr_matki  );
        galaz g=galaz();
        g.numer_matki=nr_matki;
       // g.macierz_matki=galezie[nr_matki].macierz_wlasna;
    }

}



void drzewo::rysuj_drzewo(mat4 P, mat4 V, mat4 M, float wzrost, ShaderProgram *shaderProgram, GLuint k_diffTex, GLuint l_diffTex, lisc leaf)
{
    // P I E Ń

    rysuj_stozek(P, V, M, wzrost*float(wysokosc_drzewa), float(grubosc_drzewa), shaderProgram, k_diffTex);
    M_Pnia=M;

   // mat4 M_skalowane = scale(M, vec3(0.005,0.005,0.005));
   // leaf.drawLeaf(P,V,M_skalowane,shaderProgram,l_diffTex);
//    galezie[1].macierz_matki=M;
//    galezie[1].numer_matki=0;
//    rysuj_galaz (P, V, wzrost, 3, shaderProgram, k_diffTex);

    int i;
    M_Pnia=M;


    for (i=0;i<il_galezi_w_pierwszym_pokoleniu;i++)
    {
        galezie[i].macierz_matki=M;
        rysuj_galaz (P, V, wzrost, i, shaderProgram, k_diffTex, l_diffTex, 0, leaf);
    }

    for (i=il_galezi_w_pierwszym_pokoleniu;i<il_galezi*il_galezi_w_pierwszym_pokoleniu+il_galezi_w_pierwszym_pokoleniu;i++)
    {
        //cout <<"1: " <<i<<"\t"<< int(floor((i/2) - 2)) << endl;

        galezie[i].macierz_matki=galezie[int(floor((i/2) - 2))].macierz_wlasna;
        rysuj_galaz (P, V, wzrost, i, shaderProgram, k_diffTex, l_diffTex, 1, leaf);
    }

   for (i=il_galezi*il_galezi_w_pierwszym_pokoleniu+il_galezi_w_pierwszym_pokoleniu;i<rozmiar;i++)
    {
        galezie[i].macierz_matki=galezie[pobierz_numer_matki(i)].macierz_wlasna;
        rysuj_galaz (P, V, wzrost, i, shaderProgram, k_diffTex, l_diffTex, 2, leaf);
    }





 //galezie[9].numer_matki=3;
 //rysuj_galaz(P, V, wzrost, 9, shaderProgram);

// galezie[10].numer_matki=4;
 //rysuj_galaz(P, V, wzrost, 10, shaderProgram);


// pokolenie 2

   //     i=il_galezi+1;
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
