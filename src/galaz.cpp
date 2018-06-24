#include "galaz.h"
#include <cstdlib>
#include <time.h>
#include "constants.h"
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

int min_dl_galezi=20;
int max_dl_galezi=60; // przez 10
int min_gr_galezi=1.5;
int max_gr_galezi=2; //przez 10


int galaz::losuj(int mini, int maxi)
    {
    int range = maxi-mini + 1;
	return rand()%range + mini;
    };


void galaz:: losuj_dlugosc_galezi(float dlugosc_matki)
    {
    int r=losuj(min_dl_galezi,dlugosc_matki);
    dlugosc_galezi=float(r)/10;
    };


void galaz:: losuj_grubosc_galezi(float grubosc_matki)
   {
    int r=losuj(min_gr_galezi, grubosc_matki);
    grubosc_galezi=float(r)/10;
    };

void galaz:: losuj_wys_galezi_na_matce()
    {
    int r=losuj(4,7);
    float temp=float(r)/10;
    wys_galezi_na_matce=temp;
    };



void galaz:: losuj_kat_na_matce()
    {
    int r=losuj(0,200);
    kat_na_matce=float(r*2*PI/100);
    };

//od 15 do 60
void galaz:: losuj_kat_do_pionu()
    {
    int r=losuj(3,10);
     kat_do_pionu=float(PI/r);
    };

void galaz::ustaw(float dlugosc_matki, float grubosc_matki)
{
    if (dlugosc_matki == 0)
        losuj_dlugosc_galezi(max_dl_galezi);
    else
        losuj_dlugosc_galezi(dlugosc_matki*10);



    if (grubosc_matki == 0)
        losuj_grubosc_galezi(max_gr_galezi);
    else
        grubosc_galezi = grubosc_matki * 0.7;




    losuj_wys_galezi_na_matce();
    losuj_kat_na_matce();
    losuj_kat_do_pionu();
}

galaz::galaz()
{
}

galaz::~galaz()
{
    //dtor
}
