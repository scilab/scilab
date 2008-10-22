int csom_(n, a, b, c)
     int *n;
     float *a, *b, *c;
{   
  int k;
  for (k = 0; k < *n; ++k) 
      c[k] = a[k] + b[k];
  return(0);
}
