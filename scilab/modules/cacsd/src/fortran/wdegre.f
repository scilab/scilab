c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

C/MEMBR ADD NAME=WDEGRE,SSI=0

      subroutine wdegre(ar,ai,majo,nvrai)
c    calcul du degre d un polynome a coefficients complexes
c    a=ar+i*ai=coeffs par ordre croissant
c    majo=majorant du degre
c    nvrai=degre calcule
      dimension ar(*),ai(*)
      double precision ar,ai,test
      if(majo.eq.0) goto 20
      do 10 k=1,majo+1
      kk=majo+2-k
      test=abs(ar(kk))+abs(ai(kk))
      if(1.0d+0+test.ne.1.0d+0) then
      nvrai=kk-1
      return
      endif
  10  continue
  20  nvrai=0
      return
      end
