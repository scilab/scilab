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
      subroutine fmlag1(n,nr,a,z,w)
      implicit double precision (a-h,o-z)
      dimension a(*),z(n),w(n)
c
      if(nr.eq.n)return
      nr1=nr+1
      if(nr.ne.0) go to 20
      do 10 i=nr1,n
   10 w(i)=0.d0
      return
   20 nrr=n-nr
      nh1=nr*nr1/2
      nh=nh1+1
      do 30 j=nr1,n
      u=0.d0
      nj=nh
      do 40 i=1,nr
      u=u+a(nj)*z(i)
   40 nj=nj+nrr
      nh=nh+1
      w(j)=u
   30 continue
      return
      end
