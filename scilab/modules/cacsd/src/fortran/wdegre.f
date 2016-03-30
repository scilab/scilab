c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

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
