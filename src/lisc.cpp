#include "lisc.h"
#include <cstdlib>
#include <time.h>

int min_wys_liscia=5;
int max_wys_liscia=8;
int min_typ_liscia=1;
int max_typ_liscia=2;
int min_szer_liscia=2;
int max_szer_liscia=4;

int lisc:: losuj(int mini, int maxi)
    {
    srand (time(NULL));
    int range = maxi-mini + 1;
	return rand()%range + mini;
    }

void lisc::losuj_wys_liscia()
    {
     int r=losuj(min_wys_liscia, max_wys_liscia);
     wys_liscia=r;
     return;
     };

void lisc:: losuj_szer_liscia()
    {
     int r=losuj(min_szer_liscia, max_szer_liscia);
     szer_liscia=r;
     return;
    };

void lisc:: losuj_typ_liscia()
    {
     int r=losuj(min_typ_liscia, max_typ_liscia);
     szer_liscia=r;
     return;
    };

lisc::lisc()
{
     losuj_wys_liscia();
     losuj_szer_liscia();
     losuj_typ_liscia();
}

lisc::~lisc()
{
    //dtor
}
