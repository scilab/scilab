/* Copyright INRIA/ENPC */
#include "../machine.h"

/** only used to force the linker to load all system functions **/

#ifndef WIN32

extern void 
      C2F(intversion)(),
      C2F(scichdir)(),
      C2F(scigetcwd)(),
      C2F(ref2val)(),
      C2F(allops)(),
      C2F(banier)(),
      C2F(bexec)(),
      C2F(clause)(),
      C2F(cmdstr)(),
      C2F(comand)(),
      C2F(compcl)(),
      C2F(compil)(),
      C2F(copyvar)(),
      C2F(cvname)(),
      C2F(cvnamel)(),
      C2F(cvstr)(),
      C2F(defmat)(),
      C2F(diary)(),
      C2F(dspdsp)(),
      C2F(eqid)(),
      C2F(error)(),
      C2F(expr)(),
      C2F(fact)(),
      C2F(funnam)(),
      C2F(funs)(),
      C2F(fortrangetch)(),
      C2F(getlin)(),
      C2F(getnum)(),
      C2F(getstr)(),
      C2F(getsym)(),
      C2F(getval)(),
      C2F(crebmatvar)(),
      C2F(crematvar)(),
      C2F(inisci)(),
      C2F(isbrk)(),
      C2F(israt)(),
      C2F(logops)(),
      C2F(lspdsp)(),
      C2F(lst2vars)(),
      C2F(macro)(),
      C2F(majmin)(),
      C2F(mkindx)(),
      C2F(mklist)(),
      C2F(mname)(),
      C2F(mrknmd)(),
      C2F(msgs)(),
      C2F(nextj)(),
      C2F(parse)(),
      C2F(clrest)(),
      C2F(clsave)(),
      C2F(listtype)(),
      C2F(print)(),
      C2F(prntid)(),
      C2F(prompt)(),
      C2F(eptover)(),
      C2F(ptover)(),
      C2F(mvptr)(),
      C2F(ptrback)(),
      C2F(putid)(),
      C2F(run)(),
      /* C2F(sascii)(),*/
      C2F(savlod)(),
      C2F(scilines)(),
      C2F(scirun)(),
      C2F(seteol)(),
      C2F(setgetmode)(),
      C2F(setlnb)(),
      C2F(skpins)(),
      C2F(stackg)(),
      C2F(stackp)(),
      C2F(terme)(),
      C2F(typ2cod)(),
      C2F(whatln)(),
      C2F(wspdsp)(),
      /* C2F(xchar)(), */
  /* C2F(waitforinputend)(), */
      SearchForwardInHistory(),
      SearchBackwardInHistory(),
      AddHistory(),
      C2F(xerbla)(),
      C2F(intfromjava)(),
	  C2F(intcalendar)(),
	  C2F(intgetmemory)(),
	  C2F(intwhat)(),
	  C2F(intfromc)();


void System_contents( int x)
{
  if ( x== 1) 
    {
      C2F(intversion)();
      C2F(scichdir)();
      C2F(scigetcwd)();
      C2F(ref2val)();
      C2F(allops)();
      C2F(banier)();
      C2F(bexec)();
      C2F(clause)();
      C2F(cmdstr)();
      C2F(comand)();
      C2F(compcl)();
      C2F(compil)();
      C2F(copyvar)();
      C2F(cvname)();
      C2F(cvnamel)();
      C2F(cvstr)();
      C2F(defmat)();
      C2F(diary)();
      C2F(dspdsp)();
      C2F(eqid)();
      C2F(error)();
      C2F(expr)();
      C2F(fact)();
      C2F(funnam)();
      C2F(funs)();
      C2F(fortrangetch)();
      C2F(getlin)();
      C2F(getnum)();
      C2F(getstr)();
      C2F(getsym)();
      C2F(getval)();
      C2F(crebmatvar)();
      C2F(crematvar)();
      C2F(inisci)();
      C2F(isbrk)();
      C2F(israt)();
      C2F(logops)();
      C2F(lspdsp)();
      C2F(lst2vars)();
      C2F(macro)();
      C2F(majmin)();
      C2F(mkindx)();
      C2F(mklist)();
      C2F(mname)();
      C2F(mrknmd)();
      C2F(msgs)();
      C2F(nextj)();
      C2F(parse)();
      C2F(clrest)();
      C2F(clsave)();
      C2F(listtype)();
      C2F(print)();
      C2F(prntid)();
      C2F(prompt)();
      C2F(eptover)();
      C2F(ptover)();
      C2F(mvptr)();
      C2F(ptrback)();
      C2F(putid)();
      C2F(run)();
      /* C2F(sascii)();*/
      C2F(savlod)();
      C2F(scilines)();
      C2F(scirun)();
      C2F(seteol)();
      C2F(setgetmode)();
      C2F(setlnb)();
      C2F(skpins)();
      C2F(stackg)();
      C2F(stackp)();
      C2F(terme)();
      C2F(typ2cod)();
      C2F(whatln)();
      C2F(wspdsp)();
      /* C2F(xchar)(); */
      /* C2F(waitforinputend)(); */
      SearchForwardInHistory();
      SearchBackwardInHistory();
      AddHistory();
      C2F(xerbla)();
      C2F(intfromjava)();
	  C2F(intcalendar)();
	  C2F(intgetmemory)();
	  C2F(intwhat)();
	  C2F(intfromc)();
    }
} 

#endif
