/* Copyright INRIA/ENPC */

#include "../machine.h"

/* only used to force the linker to load all calelm functions 
 */ 

#ifndef WIN32

extern void C2F(bint2d)(),
  C2F(bint3d)(),
  C2F(atome)(),
  C2F(badd)(),
  C2F(basnms)(),
  C2F(bfeval)(),
  C2F(bgetx)(),
  C2F(bgety)(),
  C2F(bintg)(),
  C2F(bj2)(),
  C2F(bjac)(),
  C2F(bjacd)(),
  C2F(bjsolv)(),
  C2F(boptim)(),
  C2F(bresd)(),
  C2F(bresid)(),
  C2F(bschur)(),
  C2F(bsolv)(),
  C2F(bsurf)(),
  C2F(bsurfd)(),
  C2F(btof)(),
  C2F(btofm)(),
  C2F(bydot)(),
  C2F(bydot2)(),
  C2F(chkvar)(),
  C2F(cmatptr)(),
  C2F(cmplxt)(),
  C2F(creadchain)(),
  C2F(creadmat)(),
  C2F(cvdm)(),
  C2F(cvwm)(),
  C2F(cwritechain)(),
  C2F(cwritemat)(),
  C2F(dfsub)(),
  C2F(dgsub)(),
  C2F(dguess)(),
  C2F(dldsp)(),
  C2F(expsum)(),
  C2F(extlarg)(),
  C2F(factf)(),
  C2F(fmttyp)(),
  C2F(fsub)(),
  C2F(ftob)(),
  C2F(getfun)(),
  C2F(gsub)(),
  C2F(intstr)(),
  C2F(iseye)(),
  C2F(isnum)(),
  C2F(istrue)(),
  C2F(matc)(),
  C2F(matptr)(),
  C2F(matz)(),
  C2F(matzs)(),
  C2F(namstr)(),
  C2F(readchain)(),
  C2F(readmat)(),
  C2F(str2name)(),
  C2F(termf)(),
  C2F(tradsl)(),
  /* stack3.c */
  get_optionals();


void System2_contents( int x)
{
  if ( x== 1) 
    {
      C2F(bint2d)();
      C2F(bint3d)();
      C2F(atome)();
      C2F(badd)();
      C2F(basnms)();
      C2F(bfeval)();
      C2F(bgetx)();
      C2F(bgety)();
      C2F(bintg)();
      C2F(bj2)();
      C2F(bjac)();
      C2F(bjacd)();
      C2F(bjsolv)();
      C2F(boptim)();
      C2F(bresd)();
      C2F(bresid)();
      C2F(bschur)();
      C2F(bsolv)();
      C2F(bsurf)();
      C2F(bsurfd)();
      C2F(btof)();
      C2F(btofm)();
      C2F(bydot)();
      C2F(bydot2)();
      C2F(chkvar)();
      C2F(cmatptr)();
      C2F(cmplxt)();
      C2F(creadchain)();
      C2F(creadmat)();
      C2F(cvdm)();
      C2F(cvwm)();
      C2F(cwritechain)();
      C2F(cwritemat)();
      C2F(dfsub)();
      C2F(dgsub)();
      C2F(dguess)();
      C2F(dldsp)();
      C2F(expsum)();
      C2F(extlarg)();
      C2F(factf)();
      C2F(fmttyp)();
      C2F(fsub)();
      C2F(ftob)();
      C2F(getfun)();
      C2F(gsub)();
      C2F(intstr)();
      C2F(iseye)();
      C2F(isnum)();
      C2F(istrue)();
      C2F(matc)();
      C2F(matptr)();
      C2F(matz)();
      C2F(matzs)();
      C2F(namstr)();
      C2F(readchain)();
      C2F(readmat)();
      C2F(str2name)();
      C2F(termf)();
      C2F(tradsl)();
      /* stack3.c */
      get_optionals();
      C2F(formatnumber)();
    }
}

#endif






