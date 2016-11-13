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
      subroutine fmc11z(a,n,nr,z,sig,w,ir,mk,eps)
      implicit double precision (a-h,o-z)
      dimension a(*),z(n),w(n)
c
      if(nr.eq.n) go to 45
      nr1=nr+1
      nh=nr*(nr1)/2+1
      if(nr.eq.0) go to 25
      do 20 i=1,nr
      do 10 j=nr1,n
      a(nh)=a(nh)+sig*z(i)*z(j)
   10 nh=nh+1
   20 continue
   25 do 40 j=nr1,n
      do 30 i=j,n
      a(nh)=a(nh)+sig*z(i)*z(j)
   30 nh=nh+1
   40 continue
      if(nr.eq.0) return
   45 call fmc11a(a,nr,z,sig,w,ir,mk,eps)
      return
      end
