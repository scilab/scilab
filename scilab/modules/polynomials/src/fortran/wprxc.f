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
C/MEMBR ADD NAME=WPRXC,SSI=0
c     Copyright INRIA
      subroutine wprxc(n,rootr,rooti,coeffr,coeffi)
c!but
c     wprxc calcule les coefficients d'un polynome defini par ses
c     racines (complexes) et dont le coefficient de degre maximum est 1
c
c!liste d'appel
c      subroutine wprxc(n,rootr,rooti,coeffr,coeffi)
c     double precision rootr(n),rooti(n),coeffr(n+1),coeffi(n+1)
c     integer n
c
c     n     : degre du polynome
c     root : tableau contenant les racines
c     coeff : tableau contenant les coefficients du polynome, ranges
c             par odre croissant
c!sous programmes appeles
c     dset waxpy (blas)
c!
      double precision rootr(n),rooti(n),coeffr(*),coeffi(*)
      integer n
c
      integer j,nj,ninf
      double precision dlamch
c
      ninf=0
c
      call dset (n,0.0d+0,coeffr,1)
      call dset (n+1,0.0d+0,coeffi,1)
      coeffr(n+1)=1.0d+0
c
      do 10 j=1,n
         if(abs(rootr(j)).gt.dlamch('o').or.
     $        abs(rooti(j)).gt.dlamch('o')) then
c     .    infinite roots gives zero high degree coeff
            ninf=ninf+1
         else
            nj=n+1-j
c            call waxpy(j,-rootr(j),-rooti(j),coeffr(nj+1),coeffi(nj+1),1
c     $           ,coeffr(nj),coeffi(nj),1)
            do k=nj,nj+j-1
               coeffr(k)=coeffr(k)-rootr(j)*coeffr(1+k)+
     $              rooti(j)*coeffi(1+k)
               coeffi(k)=coeffi(k)-rootr(j)*coeffi(1+k)-
     $              rooti(j)*coeffr(1+k)
            enddo
         endif
   10 continue

      if (ninf.gt.0) then
         call unsfdcopy(n-ninf+1,coeffr(ninf+1),1,coeffr(1),1)
         call dset(ninf,0.0d0,coeffr(n-ninf+2),1)
         call unsfdcopy(n-ninf+1,coeffi(ninf+1),1,coeffi(1),1)
         call dset(ninf,0.0d0,coeffi(n-ninf+2),1)

      endif
c
      return
      end
