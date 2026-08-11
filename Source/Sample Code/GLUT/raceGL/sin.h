extern float sin_tabel[];

extern float SIN(int A);
extern float COS(int A);

#if 0
#define SIN(A) sin_tabel[(A>0)?((int) (A))%360:((int) (A))%360+360]
#define COS(A) SIN(A+90)
#endif
