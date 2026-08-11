#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265
#endif

float sin_tabel[360];

void init_sin()
{
    int i;
    for (i=0;i<360;i++)
	sin_tabel[i] = sin(i*M_PI/180);
}

float SIN(int A) { return sin_tabel[(A>0)?((int) (A))%360:((int) (A))%360+360]; }
float COS(int A) { return SIN(A+90); }
