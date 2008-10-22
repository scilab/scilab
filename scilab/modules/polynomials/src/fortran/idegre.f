c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine idegre(a,majo,nvrai)
c!but
c    calcul du degre d un polynome
c!liste d'appel
c    a=coeff par ordre croissant
c    majo=majorant
c    nvrai=degre calcule
c!
      dimension a(*)
      double precision a,test,dasum,an
c
      an=dasum(majo+1,a,1)
      if(an.eq.0.0d+0) goto 20
      if(majo.eq.0) goto 20
      do 10 k=1,majo+1
      kk=majo+2-k
      test=abs(a(kk))/an
      if(1.0d+0+test.ne.1.0d+0) then
      nvrai=kk-1
      return
      endif
  10  continue
  20  nvrai=0
      return
      end
