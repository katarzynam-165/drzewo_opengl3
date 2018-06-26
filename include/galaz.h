#ifndef GALAZ_H
#define GALAZ_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace:: std;

class galaz
{
    public:
        galaz();
        virtual ~galaz();
        void ustaw(float dlugosc_matki, float grubosc_matki);
        int losuj(int mini, int maxi);
        void losuj_dlugosc_galezi(float dlugosc_matki);
        void losuj_grubosc_galezi(float grubosc_matki);
        void losuj_wys_galezi_na_matce();
        void losuj_kat_na_matce();
        void losuj_kat_do_pionu();
        float losuj_wymiar_liscia();


        int pokolenie;
        float kat_do_pionu;
        float dlugosc_galezi;
        float wys_galezi_na_matce;
        float wymiar_liscia;
        float kat_na_matce;
        float grubosc_galezi;
        mat4 macierz_matki;
        mat4 macierz_wlasna;
        int numer_matki;
        int numer_pokolenia; // 1 - z pnia 2 - drugie 3 - trzecie
        void nastaw_wymiary_do_pokolenia();

    private:
};

#endif // GALAZ_H
