extern void  fun2(double *, int, int *);

void fun1(double * a,  int na, int * nx,  double ** x , double * y){
        int i, k1=0;
        *y=0;
        fun2(a, na, nx);
        *x=(double *)malloc((*nx)*sizeof(double));
        *y=0;
        for(i=0;i<na;i++)
        if(a[i]>0) {
                *(*x+k1)=a[i];
                *y += a[i];
                k1++;
        };
}