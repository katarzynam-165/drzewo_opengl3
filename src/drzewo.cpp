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
#include "allmodels.h"
#include "cube.h"
#include "shaderprogram.h"

using namespace glm;

int min_wys_drzewa = 3;
int max_wys_drzewa = 3;
int grubosc_drzewa_tmp=1;


drzewo::drzewo(GLuint d, GLuint n, GLuint h)
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

    losuj_nowe_pokolenie_galezi(1);
    losuj_nowe_pokolenie_galezi(2);
    losuj_nowe_pokolenie_galezi(3);

    k_diffTex=d; //Zmienna reprezentujaca teksturę
    k_normalTex=n; //Zmienna reprezentujaca teksturę
    k_heightTex=h; //Zmienna reprezentująca teksturę


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

	//Kostka
float* vertices=Models::CubeInternal::vertices;
float* normals=Models::CubeInternal::normals;
float* texCoords=Models::CubeInternal::texCoords;
float* c1=Models::CubeInternal::c1;
float* c2=Models::CubeInternal::c2;
float* c3=Models::CubeInternal::c3;
int vertexCount=Models::CubeInternal::vertexCount;


	//Włączenie programu cieniującego, który ma zostać użyty do rysowania
	//W tym programie wystarczyłoby wywołać to raz, w setupShaders, ale chodzi o pokazanie,
	//że mozna zmieniać program cieniujący podczas rysowania jednej sceny
	shaderProgram->use();

	//Przekaż do shadera macierze P,V i M.
	//W linijkach poniżej, polecenie:
	//  shaderProgram->getUniformLocation("P")
	//pobiera numer przypisany zmiennej jednorodnej o podanej nazwie
	//UWAGA! "P" w powyższym poleceniu odpowiada deklaracji "uniform mat4 P;" w vertex shaderze,
	//a mP w glm::value_ptr(mP) odpowiada argumentowi  "mat4 mP;" TYM pliku.
	//Cała poniższa linijka przekazuje do zmiennej jednorodnej P w vertex shaderze dane z argumentu mP niniejszej funkcji
	//Pozostałe polecenia działają podobnie.
	//Poniższe polecenia są z grubsza odpowiednikami glLoadMatrixf ze starego opengla
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(mP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(mV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(mM));

	//Powiąż zmienne typu sampler2D z jednostkami teksturującymi
	glUniform1i(shaderProgram->getUniformLocation("diffuseMap"),0);
	glUniform1i(shaderProgram->getUniformLocation("normalMap"),1);
	glUniform1i(shaderProgram->getUniformLocation("heightMap"),2);


	//Przypisz tekstury do jednostek teksturujących
    //glActiveTexture(GL_TEXTURE3);
  //  glBindTexture(GL_TEXTURE_2D,k_diffTex);
   // glActiveTexture(GL_TEXTURE4);
  //  glBindTexture(GL_TEXTURE_2D, k_normalTex);
  //  glActiveTexture(GL_TEXTURE5);
   // glBindTexture(GL_TEXTURE_2D, k_heightTex);


	//Powiedz OpenGL że podczas rysowania nalezy przesłać dane do atrybutów o numerach wskazanych jako argument.
	//Polecenie shaderProgram->getAttribLocation("vertex") zwraca numer atrybutu o nazwie "vertex".
	//Odpowiada to deklaracji "in vec4 vertex;" w vertex shaderze.
	//Z grubsza odpowiednik polecenia glEnableClientState
    glEnableVertexAttribArray(shaderProgram->getAttribLocation("vertex"));
    glEnableVertexAttribArray(shaderProgram->getAttribLocation("normal"));
    glEnableVertexAttribArray(shaderProgram->getAttribLocation("texCoord0"));
    glEnableVertexAttribArray(shaderProgram->getAttribLocation("c1"));
    glEnableVertexAttribArray(shaderProgram->getAttribLocation("c2"));
    glEnableVertexAttribArray(shaderProgram->getAttribLocation("c3"));

    //Wskaż tablicę z której należy pobrać dane do atrybutu o konkretnym numerze.
    //Pierwszym argumentem jest numer przypisany atrybutowi, ostatnim tablica zdanymi.
    //Z grubsza są to odpowiedniki poleceń glVertexPointer,glColorPointer i glNormalPointer ze starego OpenGL
    glVertexAttribPointer(shaderProgram->getAttribLocation("vertex"),4,GL_FLOAT,false,0,vertices);
    glVertexAttribPointer(shaderProgram->getAttribLocation("normal"),4,GL_FLOAT,false,0,normals);
    glVertexAttribPointer(shaderProgram->getAttribLocation("texCoord0"),2,GL_FLOAT,false,0,texCoords);
    glVertexAttribPointer(shaderProgram->getAttribLocation("c1"),4,GL_FLOAT,false,0,c1);
    glVertexAttribPointer(shaderProgram->getAttribLocation("c2"),4,GL_FLOAT,false,0,c2);
    glVertexAttribPointer(shaderProgram->getAttribLocation("c3"),4,GL_FLOAT,false,0,c3);

	//Narysowanie obiektu
	glDrawArrays(GL_TRIANGLES,0,vertexCount);

	//Posprzątanie po sobie
    //Odpowiednik sekwencji poleceń glDisableClientState
	glDisableVertexAttribArray(shaderProgram->getAttribLocation("vertex"));
    glDisableVertexAttribArray(shaderProgram->getAttribLocation("color"));
    glDisableVertexAttribArray(shaderProgram->getAttribLocation("normal"));
    glDisableVertexAttribArray(shaderProgram->getAttribLocation("texCoord0"));
    glDisableVertexAttribArray(shaderProgram->getAttribLocation("c1"));
    glDisableVertexAttribArray(shaderProgram->getAttribLocation("c2"));
    glDisableVertexAttribArray(shaderProgram->getAttribLocation("c3"));
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
    if (G.numer_matki==0)
        {
        wys_matki=float(wzrost*wysokosc_drzewa);
        }
    else
        {wys_matki=galezie[G.numer_matki].dlugosc_galezi;}
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
        g.macierz_matki=galezie[nr_matki].macierz_wlasna;
        }

    }



void drzewo::rysuj_drzewo(mat4 P, mat4 Mz, mat4 V, float wzrost, ShaderProgram *shaderProgram)
    {

    mat4 M = mat4(1.0);
    M = translate(Mz, vec3(0.0f, wzrost*float(wysokosc_drzewa)/2.0f, 0.0f));
    rysuj_prostopadloscian(P, V, M, float(grubosc_drzewa), wzrost*float(wysokosc_drzewa), float(grubosc_drzewa), shaderProgram);
    // to bedzie dla galezi

    int i;
    M_Pnia=M;


   for (i=0;i<=il_galezi;i++)
        {
        galezie[i].macierz_matki=M;
        rysuj_galaz (P, V, wzrost, i, shaderProgram);
        }



 // galezie[9].numer_matki=3;
 //rysuj_galaz(P, V, wzrost, 9, shaderProgram);


   for (i=il_galezi+1;i<=2*il_galezi;i++)
        {

    //    rysuj_galaz(V, wzrost, i);
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
