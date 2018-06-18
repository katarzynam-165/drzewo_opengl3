#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


//Atrybuty
in vec3 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec3 normal; //wektor normalny w wierzcholku w przestrzeni modelu
in vec2 texCoord0; //wspolrzedne teksturowania
//in vec4 c1; //Pierwsza kolumna macierzy TBN^-1 (na razie ignorowana)
//in vec4 c2; //Druga kolumna macierzy TBN^-1 (na razie ignorowana)
//in vec4 c3; //Trzecia kolumna macierzy TBN^-1 (na razie ignorowana)

//Zmienne interpolowane
out vec4 i_l1; //wektor do swiatla(przestrzen oka)
out vec4 i_l2; //wektor do swiatla(przestrzen oka)
out vec4 i_v; //wektor do obserwatora(przestrzen oka)
out vec4 i_n; //wektor normalny (przestrzen oka)
out vec2 iTexCoord0; //wspolrzedne teksturowania

uniform vec4 lp1;
uniform vec4 lp2;

void main(void) {

    /*mat4 itbn=mat4(c1,c2,c3,vec4(0,0,0,1));

    vec4 lp=vec4(0,0,-6,1); //Wspolrzedne swiatla w przestrzeni swiata

    i_l=normalize(itbn*(inverse(M)*lp-vertex));
    i_v=normalize(itbn*(inverse(V*M)*vec4(0,0,0,1)-vertex));
    i_n=normalize(itbn*normal);

    iTexCoord0=texCoord0;

	gl_Position=P*V*M*vertex;*/

	i_l1=normalize(V*lp1-V*M*vec4(vertex,1));
	i_l2=normalize(V*lp2-V*M*vec4(vertex,1));

    i_v=normalize(vec4(0,0,0,1)-V*M*vec4(vertex,1));
    i_n=normalize(V*M*vec4(-normal,0));

    iTexCoord0=texCoord0;

	gl_Position=P*V*M*vec4(vertex,1);
}


