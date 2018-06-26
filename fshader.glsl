#version 330

uniform sampler2D diffuseMap; //Zmienna reprezentujaca jednostke teksturujaca
out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 i_l1; //wektor do swiatla(przestrzen oka)
in vec4 i_l2; //wektor do swiatla(przestrzen oka)
in vec4 i_v; //wektor do obserwatora(przestrzen oka)
in vec4 i_n; //wektor normalny (przestrzen oka, na razie ignorowany)

in vec2 iTexCoord0; //wspolrzedne teksturowania

void main(void) {
	vec4 v=normalize(i_v);


    vec4 n=normalize(i_n);
    vec4 l1=normalize(i_l1);
    vec4 l2=normalize(i_l2);
    vec4 r1=reflect(-l1,n);
    vec4 r2=reflect(-l2,n);

    vec4 ka=vec4(0,0,0,0); //Kolor obiektu w swietle otoczenia
    vec4 kd=texture(diffuseMap,iTexCoord0); //Kolor obiektu w swietle rozproszonym
    vec4 ks=vec4(0.1,0.1,0.1,0); //Kolor obiektu w swietle odbitym


    vec4 la=vec4(0,0,0,0); //Kolor swiatla otoczenia
    vec4 ld=vec4(1,1,1,1); //Kolor swiatla rozpraszanego
    vec4 ls=vec4(0.1f,0.1f,0.1f,0); //Kolor swiatla odbijanego

    float nl1=max(dot(n,l1),0); //Kosinus kata pomiedzy wektorami do swiatla i normalnym
    float nl2=max(dot(n,l2),0); //Kosinus kata pomiedzy wektorami do swiatla i normalnym
    float rv1=pow(max(dot(r1,v),0),1); //Kosinus kata pomiedzy wektorami do obserwatora i odbitym, podniesiony do wykladnika Phonga
    float rv2=pow(max(dot(r2,v),0),1); //Kosinus kata pomiedzy wektorami do obserwatora i odbitym, podniesiony do wykladnika Phonga

	pixelColor=ka*la+kd*ld*vec4(nl1,nl1,nl1,1)+ks*ls*vec4(rv1,rv1,rv1,0);
	pixelColor += ka*la+kd*ld*vec4(nl2,nl2,nl2,1)+ks*ls*vec4(rv2,rv2,rv2,0);
}

