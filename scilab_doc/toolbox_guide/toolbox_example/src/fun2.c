void  fun2(double * a, int na, int * nx)
{
        int i;
        *nx=0;
        for(i=0;i<na;i++)
        if (a[i]>0)  
                (*nx)++;
}