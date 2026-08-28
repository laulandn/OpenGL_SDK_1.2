#include "map.h"

#if 0
char map[SZ][SZ];

void init_map()
{
    int i,j;
    for (i=0;i<SZ;i++)
	for (j=0;j<SZ;j++)
	map[i][j] = (i+j)%2?0:3;
    map[1][1] = 4;
    map[1][2] = 14;
    map[1][3] = 2;
    map[1][4] = 12;
    map[1][5] = 5;
    map[2][1] = 8;
    map[3][1] = 10;
    map[4][1] = 6;
    map[4][2] = 5;
    map[5][2] = 6;
    map[5][3] = 15;
    map[5][4] = 13;
    map[5][5] = 7;
    map[4][5] = 11;
    map[3][5] = 1;
    map[2][5] = 9;
}
#else
char map[SZ][SZ] = {
    "               c    ",
    "   eoccccmf    c    ",
    "   i      j    k    ",
    "   b      k    gf   ",
    "   k      gf    gf  ",
    "   gpcmf   gf    j  ",
    "       gf   gf   l  ",
    " eocmf  gf   gpcmh  ",
    " i   j   j          ",
    " b   b   b eoccmf   ",
    " b   k   b i    j   ",
    " b   gf  l b    b   ",
    " k    gpnh l    b   ",
    " gf       eh    b   ",
    "  gpcccccnh     b   ",
    "                l   ",
    "   eocmf  eocccnh   ",
    "   i   gpnh         ",
    "   k        eomf    ",
    "   gpccccccnh  j    "
};

void init_map()
{
    int i,j;
    for (i=0;i<SZ;i++)
    {
	for (j=0;j<SZ;j++)
	{
	    if (map[i][j] == ' ')
	        map[i][j] = (i+j)%2?'\000':'\003';
	    else
	        map[i][j] -= 'a';
	}
    }
}
#endif
