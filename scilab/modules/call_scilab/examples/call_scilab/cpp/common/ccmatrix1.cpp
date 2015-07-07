#include <iostream>
#include "ccmatrix.h"


class SciError {};

// CMatrix A(name,job);

extern "C" {
#include <stdio.h>
#undef _PARAMS
    int send_scilab_job(char *, long int);
    void cc_test();

}

static char buf[256];

CMatrix::CMatrix(char *name1, char *job)
{
    sprintf(buf, "%s=%s;", name1, job);
    if ( send_scilab_job(buf, strlen(buf)) != 0)
    {
        throw SciError();
    }
    else
    {
        int lp;
        C2F(cmatptr)(name1, &m, &n, &lp, strlen(name1));
        v = stk(lp);
    }
    name = new char[strlen(name1) + 1];
    strcpy(name, name1);
}


void CMatrix::scijob(char *jobname)
{
    sprintf(buf, jobname, name);
    if ( send_scilab_job(buf, strlen(buf)) != 0)
    {
        throw SciError();
    }
}

CMatrix& CMatrix::plus(CMatrix &B)
{
    sprintf(buf, "%s=%s+%s", this->name, this->name, B.name);
    if ( send_scilab_job(buf, strlen(buf)) != 0)
    {
        throw SciError();
    }
    int lp;
    C2F(cmatptr)(name, &m, &n, &lp, strlen(name));
    v = stk(lp);
    return *this;
}

void CMatrix::print()
{
    fprintf(stdout, "Matrix %s=\n", name);
    scijob("disp(%s)");
}

void CMatrix::inv()
{
    sprintf(buf, "%s=inv(%s)", name, name);
    if ( send_scilab_job(buf, strlen(buf)) != 0)
    {
        throw SciError();
    }
    int lp;
    C2F(cmatptr)(name, &m, &n, &lp, strlen(name));
    v = stk(lp);
}


void cc_test()
{
    CMatrix A("a", "ones(2,2)");
    CMatrix B("b", "8");
    A.plus(B);
    A.scijob("disp(%s);");
    A.scijob("%s=rand(2,2)");
    A.print();
    A.inv();
    A.print();
}
