void  fun2(double *, int, int *);

void fun1(double * a,  int na, int * nb,  double ** b , double * positive_sum)
{
int i, k1=0;
*positive_sum=0;
fun2(a, na, nb);
*b=(double *)malloc((*nb)*sizeof(double));
*positive_sum=0;
for(i=0;i<na;i++)
  if(a[i]>0)
    {
    *(*b+k1)=a[i];
    *positive_sum += a[i];
    k1++;
    };
}