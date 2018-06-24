/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "cone.h"
#include "lisc.h"
#include "drzewo.h"

using namespace glm;
using namespace std;

float wzrost = 1;
float tempo_wzrostu=0.02;
float max_wysokosc_drzewa=10;
cone stozek = cone();

GLuint k_diffTex; //Zmienna reprezentujaca teksturę
GLuint l_diffTex; //Zmienna reprezentujaca teksturę
//GLuint k_normalTex; //Zmienna reprezentujaca teksturę
//GLuint k_heightTex; //Zmienna reprezentująca teksturę

//GLuint z_diffTex; //Zmienna reprezentujaca teksturę
//GLuint z_normalTex; //Zmienna reprezentujaca teksturę
//GLuint z_heightTex; //Zmienna reprezentująca teksturę



float speed_x = 0; // [radiany/s]
float speed_y = 0; // [radiany/s]
float speed_distance=0; //[radiany/s]

float aspect=1; //Stosunek szerokości do wysokości okna

//Uchwyty na shadery
ShaderProgram *shaderProgram; //Wskaźnik na obiekt reprezentujący program cieniujący.


//uchwyty na rzeczy w lisciu:
float* leaf_vertices;
float* leaf_texCoords;
float* leaf_normals;
int leaf_vertexCount;
lisc leaf;

void loadOBJ(string path){

            vector <float> faceVertices, faceUVs, faceNormals;
            vector <float> tmpVertices, tmpUVs, tmpNormals;

            ifstream file(path.c_str(), ios::in);
            string line;

            while(getline(file, line)){

                if(line.substr(0, 2) == "v "){

                    istringstream sub(line.substr(2));
                    float x, y, z;
                    sub >> x;
                    sub >> y;
                    sub >> z;
                    tmpVertices.push_back(x);
                    tmpVertices.push_back(y);
                    tmpVertices.push_back(z);

                } else if(line.substr(0,3) == "vt "){

                    istringstream sub(line.substr(3));
                    float u, v;
                    sub >> u;
                    sub >> v;
                    tmpUVs.push_back(u);
                    tmpUVs.push_back(v);
                } else if(line.substr(0,3) == "vn "){

                    istringstream sub(line.substr(3));
                    float x, y, z;
                    sub >> x;
                    sub >> y;
                    sub >> z;
                    tmpNormals.push_back(x);
                    tmpNormals.push_back(y);
                    tmpNormals.push_back(z);

                } else if(line.substr(0, 2) == "f "){

                    string sub = line.substr(2);
                    char* dup = strdup(sub.c_str());
                    char* token = strtok(dup, " ");
                    while(token != NULL){
                        istringstream info(token);
                        vector <int> tokens;
                        string data;
                        int index;
                        while(getline(info, data, '/')){
                            index = atoi(data.c_str());
                            index--;
                            tokens.push_back(index);
                        }
                        faceVertices.push_back(tokens[0]);
                        faceUVs.push_back(tokens[1]);
                        faceNormals.push_back(tokens[2]);
                        tokens.clear();
                        token = strtok(NULL, " ");
                    }
                }
            }


            leaf_vertices = new float[faceVertices.size()*3];
            leaf_texCoords = new float[faceUVs.size()*2];
            leaf_normals = new float[faceNormals.size()*3];
            leaf_vertexCount = faceVertices.size();

            int index_out = 0;

            for(int i = 0; i < faceVertices.size(); i++){
                int vIndex = faceVertices[i]*3;
                leaf_vertices[index_out] = tmpVertices[vIndex];

                index_out++;
                vIndex++;
                leaf_vertices[index_out] = tmpVertices[vIndex];
                index_out++;
                vIndex++;
                leaf_vertices[index_out] = tmpVertices[vIndex];
                index_out++;
                vIndex++;
            }

            index_out = 0;

            for(int i = 0; i < faceUVs.size(); i++){
                int uvIndex = faceUVs[i]*2;
                leaf_texCoords[index_out] = tmpUVs[uvIndex];
                index_out++;
                uvIndex++;
                leaf_texCoords[index_out] = tmpUVs[uvIndex];
                index_out++;
                uvIndex++;
            }

            index_out = 0;

            for(int i = 0; i < faceNormals.size(); i++){
                int nIndex = faceNormals[i]*3;
                leaf_normals[index_out] = -tmpNormals[nIndex];
                index_out++;
                nIndex++;
                leaf_normals[index_out] = -tmpNormals[nIndex];
                index_out++;
                nIndex++;
                leaf_normals[index_out] = -tmpNormals[nIndex];
                index_out++;
                nIndex++;
            }
    }



//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed_y = -3.14;
		if (key == GLFW_KEY_RIGHT) speed_y = 3.14;
		if (key == GLFW_KEY_UP) speed_x = -3.14;
		if (key == GLFW_KEY_DOWN) speed_x = 3.14;
		if (key == GLFW_KEY_A) speed_distance = -3.14;
		if (key == GLFW_KEY_Z) speed_distance = 3.14;
	}


	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speed_y = 0;
		if (key == GLFW_KEY_RIGHT) speed_y = 0;
		if (key == GLFW_KEY_UP) speed_x = 0;
		if (key == GLFW_KEY_DOWN) speed_x = 0;
		if (key == GLFW_KEY_A) speed_distance = 0;
		if (key == GLFW_KEY_Z) speed_distance = 0;
	}
}

//Procedura obługi zmiany rozmiaru bufora ramki
void windowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //Obraz ma być generowany w oknie o tej rozdzielczości
    if (height!=0) {
        aspect=(float)width/(float)height; //Stosunek szerokości do wysokości okna
    } else {
        aspect=1;
    }
}


GLuint readTexture(char* filename) {
  GLuint tex;
  glActiveTexture(GL_TEXTURE0);

  //Wczytanie do pamięci komputera
  std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
  unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
  //Wczytaj obrazek
  unsigned error = lodepng::decode(image, width, height, filename);

  //Import do pamięci karty graficznej
  glGenTextures(1,&tex); //Zainicjuj jeden uchwyt
  glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
  //Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
  glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  return tex;
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1); //Czyść ekran na czarno
	glEnable(GL_DEPTH_TEST); //Włącz używanie Z-Bufora
	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury
	glfwSetFramebufferSizeCallback(window,windowResize); //Zarejestruj procedurę obsługi zmiany rozmiaru bufora ramki

	shaderProgram=new ShaderProgram("vshader.glsl",NULL,"fshader.glsl"); //Wczytaj program cieniujący
    shaderProgram->use();

	k_diffTex=readTexture("kora_albedo.png");
	l_diffTex=readTexture("Leaf.png");
	loadOBJ("Leaf.obj");
    leaf = lisc();

    leaf.leaf_normals = leaf_normals;
    leaf.leaf_texCoords = leaf_texCoords;
    leaf.leaf_vertices = leaf_vertices;
    leaf.vertexCount = leaf_vertexCount;


	//k_normalTex=readTexture("kora_normal.png");
    //k_heightTex=readTexture("kora_height.png");
    //z_diffTex=readTexture("ziemia_albedo.png");
	//z_normalTex=readTexture("ziemia_normal.png");
    //z_heightTex=readTexture("ziemia_height.png");

}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram() {
	delete shaderProgram; //Usunięcie programu cieniującego

	glDeleteTextures(1,&k_diffTex);
	glDeleteTextures(1,&l_diffTex);
	//glDeleteTextures(1,&k_normalTex);
	//glDeleteTextures(1,&k_heightTex);
    //glDeleteTextures(1,&z_diffTex);
	//glDeleteTextures(1,&z_normalTex);
	//glDeleteTextures(1,&z_heightTex);

}
/*
void rysuj_Ziemie(ShaderProgram *shaderProgram, mat4 mP, mat4 mV, mat4 mM) {

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
	glUniform1i(shaderProgram->getUniformLocation("diffuseMap"),3);
	glUniform1i(shaderProgram->getUniformLocation("normalMap"),4);
	glUniform1i(shaderProgram->getUniformLocation("heightMap"),5);


	//Przypisz tekstury do jednostek teksturujących

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,k_diffTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,k_normalTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,k_heightTex);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,z_diffTex);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D,z_normalTex);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D,z_heightTex);



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
}*/





//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y,float distance, float wzrost,drzewo D, ShaderProgram *shaderProgram) {

	//************Tutaj umieszczaj kod rysujący obraz******************l

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Wykonaj czyszczenie bufora kolorów i głębokości

	glm::mat4 P = glm::perspective(50 * PI / 180, aspect, 1.0f, 50.0f); //Wylicz macierz rzutowania

	glm::mat4 V = glm::lookAt( //Wylicz macierz widoku
		glm::vec3(0.0f, 0.0f, distance),
		glm::vec3(0.0f, 4.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));


    //ziemia

    /*mat4 Mz = mat4(1.0f);
    Mz=rotate(Mz, -PI/90 ,vec3(1.0f,0.0f,0.0f)); //żeby na wejściu było ok
    Mz=rotate(Mz,angle_x,vec3(1.0f,0.0f,0.0f));
    Mz=rotate(Mz,angle_y,vec3(0.0f,1.0f,0.0f));*/

    mat4 M = mat4(1.0f);
    M = rotate(M,angle_x,vec3(1.0f,0.0f,0.0f));
    M = rotate(M,angle_y,vec3(0.0f,1.0f,0.0f));

    D.rysuj_drzewo(P,V,M,wzrost,shaderProgram,k_diffTex,l_diffTex, leaf);


    /*
  //  M = rotate(M,110*PI/180,vec3(-1.0f,0.0f,0.0f));

    //


    lisc leaf = lisc();

    //ta czesc moze pewnie byc ladniej napisana - musze pokminic
    leaf.leaf_normals = leaf_normals;
    leaf.leaf_texCoords = leaf_texCoords;
    leaf.leaf_vertices = leaf_vertices;
    leaf.vertexCount = leaf_vertexCount;

    mat4 M_skalowane = scale(M, vec3(0.005,0.005,0.005));
  //  leaf.drawLeaf(P,V,M_skalowane,shaderProgram,l_diffTex);


    D.rysuj_drzewo(P,V,M,wzrost,shaderProgram,k_diffTex);


    //mat4 Mz_skalowane=scale(Mz, vec3(500.0f, 0.1f, 500.0f)); //musi byc
   // (value_ptr(V*Mz_skalowane));
    //glColor3d(0.0,0.7,0.0);
	//Narysuj obiekt
   // rysuj_Ziemie(shaderProgram,P,V,Mz_skalowane);
   // D.rysuj_drzewo(P, Mz, V, wzrost, shaderProgram);


    // pien
*/
	//Przerzuć tylny bufor na przedni
	glfwSwapBuffers(window);

}



int main(void)
{
    GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno
    drzewo D = drzewo(k_diffTex);
    printf ("Wysokosc drzewa: %d \n", D.wysokosc_drzewa);

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(700, 700, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	float angle_x = 0; //Kąt obrotu obiektu
	float angle_y = 0; //Kąt obrotu obiektu
    float distance = -15;

	glfwSetTime(0); //Wyzeruj licznik czasu

	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
        {
		//RUCHY KAMERĄI WZROST DRZEWA
		angle_x += speed_x*glfwGetTime(); //Zwiększ kąt o prędkość kątową razy czas jaki upłynął od poprzedniej klatki
		angle_y += speed_y*glfwGetTime(); //Zwiększ kąt o prędkość kątową razy czas jaki upłynął od poprzedniej klatki
        distance += speed_distance*glfwGetTime();

        if (distance<=-30) {distance=-30;}
        if (distance>=-5) {distance=-5;}
		glfwSetTime(0); //Wyzeruj licznik czasu


		drawScene(window,angle_x,angle_y,distance, wzrost,D,shaderProgram); //Wykonaj procedurę rysującą

		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.

		if (wzrost<=max_wysokosc_drzewa)
            {
             wzrost=wzrost+tempo_wzrostu;
            }


	}

	freeOpenGLProgram();

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
