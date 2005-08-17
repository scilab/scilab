/* Copyright INRIA/ENPC */

#include "../machine.h"

/** only used to force the linker to load all calelm functions **/

#ifndef WIN32

extern int C2F(dcabs1)(),C2F(arcosh)(),C2F(calerf)(),C2F(ccopy)(),
  C2F(coshin)(),C2F(cupro)(),C2F(cuproi)(),C2F(cusum)(),C2F(dad)(),
C2F(dadd)(),C2F(ddif)(),C2F(ddpow)(),C2F(ddpow1)(),C2F(ddpowe)(),
C2F(ddrdiv)(),C2F(dipow)(),C2F(dipowe)(),C2F(dlblks)(),C2F(dlgama)(),
C2F(dmcopy)(),C2F(dmmul)(),C2F(dmmul1)(),C2F(dmprod)(),C2F(dmsum)(),
C2F(drdiv)(),C2F(dset)(),C2F(dsort)(),C2F(dsum)(),C2F(dtild)(),
C2F(dvmul)(),C2F(dwdiv)(),C2F(dwpow)(),C2F(dwpow1)(),C2F(dwpowe)(),
C2F(dwrdiv)(),C2F(entier)(),C2F(franck)(),C2F(gamma)(),C2F(gdcp2i)(),
C2F(hilber)(),C2F(icopy)(),C2F(imcopy)(),C2F(int2db)(),C2F(intp)(),
C2F(iset)(),C2F(isort)(),C2F(isova0)(),C2F(isoval)(),C2F(ivimp)(),
C2F(iwamax)(),C2F(kronc)(),C2F(kronr)(),C2F(lnblnk)(),C2F(magic)(),
C2F(mtran)(),C2F(psi)(),C2F(pythag)(),C2F(rat)(),C2F(rcopy)(),
C2F(rcsort)(),C2F(rea2db)(),C2F(ribesl)(),C2F(rjbesl)(),C2F(rkbesl)(),
C2F(round)(),C2F(rybesl)(),C2F(simple)(),C2F(urand)(),C2F(vpythag)(),
C2F(wasum)(),C2F(watan)(),C2F(waxpy)(),C2F(wcopy)(),C2F(wddiv)(),
C2F(wdiv)(),C2F(wdotci)(),C2F(wdotcr)(),C2F(wdpow)(),C2F(wdpow1)(),
C2F(wdpowe)(),C2F(wdrdiv)(),C2F(wipow)(),C2F(wipowe)(),C2F(wlog)(),
C2F(wmmul)(),C2F(wmprod)(),C2F(wmsum)(),C2F(wmul)(),C2F(wnrm2)(),
C2F(wrscal)(),C2F(wscal)(),C2F(wsign)(),C2F(wsort)(),C2F(wsqrt)(),
C2F(wswap)(),C2F(wvmul)(),C2F(wwdiv)(),C2F(wwpow)(),C2F(wwpow1)(),
C2F(wwpowe)(),C2F(wwrdiv)(),C2F(unsfdcopy)();


int Calelm_contents(x) 
     int x;
{
  if ( x== 1) 
    {
	C2F(dcabs1)();
	C2F(arcosh)();
	C2F(calerf)();
	C2F(ccopy)();
	C2F(coshin)();
	C2F(cupro)();
	C2F(cuproi)();
	C2F(cusum)();
	C2F(dad)();
	C2F(dadd)();
	C2F(ddif)();
	C2F(ddpow)();
	C2F(ddpow1)();
	C2F(ddpowe)();
	C2F(ddrdiv)();
	C2F(dipow)();
	C2F(dipowe)();
	C2F(dlblks)();
	C2F(dlgama)();
	C2F(dmcopy)();
	C2F(dmmul)();
	C2F(dmmul1)();
	C2F(dmprod)();
	C2F(dmsum)();
	C2F(drdiv)();
	C2F(dset)();
	C2F(dsort)();
	C2F(dsum)();
	C2F(dtild)();
	C2F(dvmul)();
	C2F(dwdiv)();
	C2F(dwpow)();
	C2F(dwpow1)();
	C2F(dwpowe)();
	C2F(dwrdiv)();
	C2F(entier)();
	C2F(franck)();
	C2F(gamma)();
	C2F(gdcp2i)();
	C2F(hilber)();
	C2F(icopy)();
	C2F(imcopy)();
	C2F(int2db)();
	C2F(intp)();
	C2F(iset)();
	C2F(isort)();
	C2F(isova0)();
	C2F(isoval)();
	C2F(ivimp)();
	C2F(iwamax)();
	C2F(kronc)();
	C2F(kronr)();
	C2F(lnblnk)();
	C2F(magic)();
	C2F(mtran)();
	C2F(psi)();
	C2F(pythag)();
	C2F(rat)();
	C2F(rcopy)();
	C2F(rcsort)();
	C2F(rea2db)();
	C2F(ribesl)();
	C2F(rjbesl)();
	C2F(rkbesl)();
	C2F(sciround)();
	C2F(rybesl)();
	C2F(simple)();
	C2F(unsfdcopy)();
	C2F(urand)();
	C2F(vpythag)();
	C2F(wasum)();
	C2F(watan)();
	C2F(waxpy)();
	C2F(wcopy)();
	C2F(wddiv)();
	C2F(wdiv)();
	C2F(wdotci)();
	C2F(wdotcr)();
	C2F(wdpow)();
	C2F(wdpow1)();
	C2F(wdpowe)();
	C2F(wdrdiv)();
	C2F(wipow)();
	C2F(wipowe)();
	C2F(wlog)();
	C2F(wmmul)();
	C2F(wmprod)();
	C2F(wmsum)();
	C2F(wmul)();
	C2F(wnrm2)();
	C2F(wrscal)();
	C2F(wscal)();
	C2F(wsign)();
	C2F(wsort)();
	C2F(wsqrt)();
	C2F(wswap)();
	C2F(wvmul)();
	C2F(wwdiv)();
	C2F(wwpow)();
	C2F(wwpow1)();
	C2F(wwpowe)();
	C2F(wwrdiv)();
    }
  return 0;
}
#endif
