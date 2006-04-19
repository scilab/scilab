void  fun2(double * a, int na, int * m)
{
int i;
*m=0;
for(i=0;i<na;i++)
    if (a[i]>0) 
      (*m)++;
}