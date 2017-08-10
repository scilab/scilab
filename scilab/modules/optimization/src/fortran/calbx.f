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
c
      subroutine calbx(n,index,indic,nt,np,y,s,ys,z,zs,x,diag,bx)
c
c     fonction : {bx}=[b]*{x}.
c                [b] est definie par les vecteurs
c                ({y}(i),{s}(i),{z}(i), i=1,np) et {diag}
c
      implicit double precision (a-h,o-z)
      dimension y(nt,n),s(nt,n),z(nt,n),ys(nt),zs(nt)
      dimension diag(n),bx(n),x(n)
      integer indic(n),index(nt)
c
      do 100 i=1,n
         if(indic(i).gt.0) go to 100
         bx(i)=diag(i)*x(i)
100   continue
c
      do 110 i=1,np
         ii=index(i)
c
         yx=0
         zx=0
         do 120 j=1,n
            if(indic(j).gt.0) go to 120
            yx=yx+y(ii,j)*x(j)
            zx=zx+z(ii,j)*x(j)
120      continue
c
         do 130 j=1,n
            if(indic(j).gt.0) go to 130
            bx(j)=bx(j)+yx*y(ii,j)/ys(ii)-zx*z(ii,j)/zs(ii)
130      continue
110   continue
c
      return
      end
