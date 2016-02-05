c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
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
