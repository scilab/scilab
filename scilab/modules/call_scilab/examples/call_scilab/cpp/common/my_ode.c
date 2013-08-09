
int odeex(int *neq, double * t, double *y, double * ydot)
{
    ydot[0] = -y[0] + 2.0;
    return 0;
}
