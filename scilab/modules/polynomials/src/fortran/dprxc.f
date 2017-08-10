c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA - Serge STEER
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
C/MEMBR ADD NAME=DPRXC,SSI=0
c     Copyright INRIA
      subroutine dprxc(n,roots,coeff)
c!but
c     dpxrc calcule les coefficients d'un polynome defini par ses
c     racines (reelles) et dont le coefficient de degre maximum est 1
c
c!liste d'appel
c      subroutine dpxrc(n,roots,coeff)
c     double precision roots(n),coeff(n+1)
c     integer n
c
c     n     : degre du polynome
c     roots : tableau contenant les racines
c     coeff : tableau contenant les coefficients du polynome, ranges
c             par odre croissant
c!sous programmes appeles
c     dset daxpy (blas)
c!
      double precision roots(n),coeff(*)
      integer n
c
      integer j,nj, ninf
      double precision dlamch
c
      ninf=0
      call dset (n,0.0d+0,coeff,1)
      coeff(n+1)=1.0d+0
c
      do 10 j=1,n
         if(abs(roots(j)).gt.dlamch('o')) then
c     .    infinite roots gives zero high degree coeff
            ninf=ninf+1
         else
            nj=n+1-j
c            call daxpy(j,-roots(j),coeff(nj+1),1,coeff(nj),1)
            do k=nj,nj+j-1
                coeff(k)=coeff(k)-roots(j)*coeff(1+k)
            enddo
         endif
 10   continue
      if (ninf.gt.0) then
         call unsfdcopy(n-ninf+1,coeff(ninf+1),1,coeff(1),1)
         call dset(ninf,0.0d0,coeff(n-ninf+2),1)
      endif
c
      return
      end
