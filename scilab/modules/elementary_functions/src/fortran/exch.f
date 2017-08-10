c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA - Serge STEER
c Copyright (C) INRIA - Francois DELEBECQUE
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
        subroutine exch(nmax,n,a,z,l,ls1,ls2)
c
      integer nmax,n,l,ls1,ls2
        double precision a(nmax,n),z(nmax,n)
c!purpose
c  given  upper hessenberg matrix a
c  with consecutive ls1xls1 and ls2xls2 diagonal blocks (ls1,ls2.le.2)
c  starting at row/column l, exch produces equivalence transforma-
c  tion zt that exchange the blocks along with their
c  eigenvalues.
c
c!calling sequence
c
c     subroutine exch(nmax,n,a,z,l,ls1,ls2)
c     integer nmax,n,l,ls1,ls2
c     double precision a(nmax,n),z(nmax,n)
c
c     nmax     the first dimension of a, b and z
c     n        the order of a, and z
c    *a        the matrix whose blocks are to be interchanged
c    *z        upon return this array is multiplied by the column
c              transformation zt.
c     l        the position of the blocks
c     ls1      the size of the first block
c     ls2      the size of the second block
c
c!auxiliary routines
c     drot (blas)
c     giv
c     max abs (fortran)
c!originator
c     Delebecque f. and Steer s. INRIA adapted from exchqz (VanDooren)
c!
      integer i,j,l1,l2,l3,li,lj,ll
      double precision u(3,4),d,e,f,g,sa,sb
      l1=l+1
      ll=ls1+ls2
      if (ll.gt.2) go to 50
c ** interchange 1x1 and 1x1 blocks via an equivalence
c ** transformation       a:=z'*a*z ,
c ** where z is givens rotation
      f=max(abs(a(l1,l1)),1.0d+0)
      sa=a(l1,l1)/f
      sb=1.0d+0/f
      f=sa-sb*a(l,l)
c construct the column transformation z
      g=-sb*a(l,l1)
      call giv(f,g,d,e)
      call drot(l1,a(1,l),1,a(1,l1),1,e,-d)
      call drot(n,z(1,l),1,z(1,l1),1,e,-d)
c construct the row transformation q
      call drot(n-l+1,a(l,l),nmax,a(l1,l),nmax,e,-d)
      a(l1,l)=0.0d+0
      return
c ** interchange 1x1 and 2x2 blocks via an equivalence
c ** transformation  a:=z2'*z1'*a*z1*z2 ,
c ** where each zi is a givens rotation
 50   l2=l+2
      if(ls1.eq.2) go to 100
      g=max(abs(a(l,l)),1.0d+0)
c *  evaluate the pencil at the eigenvalue corresponding
c *  to the 1x1 block
 60   sa=a(l,l)/g
      sb=1.0d+0/g
      do 80 j=1,2
           lj=l+j
           do 80 i=1,3
               li=l+i-1
               u(i,j)=-sb*a(li,lj)
   80          if(li.eq.lj) u(i,j)=u(i,j)+sa
      call giv(u(3,1),u(3,2),d,e)
      call drot(3,u(1,1),1,u(1,2),1,e,-d)
c perform the row transformation z1'
      call giv(u(1,1),u(2,1),d,e)
      u(2,2)=-u(1,2)*e+u(2,2)*d
      call drot(n-l+1,a(l,l),nmax,a(l1,l),nmax,d,e)
c perform the column transformation z1
      call drot(l2,a(1,l),1,a(1,l1),1,d,e)
      call drot(n,z(1,l),1,z(1,l1),1,d,e)
c perform the row transformation z2'
      call giv(u(2,2),u(3,2),d,e)
      call drot(n-l+1,a(l1,l),nmax,a(l2,l),nmax,d,e)
c perform the column transformation z2
      call drot(l2,a(1,l1),1,a(1,l2),1,d,e)
      call drot(n,z(1,l1),1,z(1,l2),1,d,e)
c  put the neglectable elements equal to zero
      a(l2,l)=0.0d+0
      a(l2,l1)=0.0d+0
      return
c ** interchange 2x2 and 1x1 blocks via an equivalence
c ** transformation  a:=z2'*z1'*a*z1*z2 ,
c ** where each zi is a givens rotation
 100  if(ls2.eq.2) go to 150
      g=max(abs(a(l2,l2)),1.0d+0)
c *  evaluate the pencil at the eigenvalue corresponding
c *  to the 1x1 block
 120  sa=a(l2,l2)/g
      sb=1.0d+0/g
      do 130 i=1,2
           li=l+i-1
           do 130 j=1,3
               lj=l+j-1
               u(i,j)=-sb*a(li,lj)
  130    if(i.eq.j) u(i,j)=u(i,j)+sa
      call giv (u(1,1),u(2,1),d,e)
      call drot(3,u(1,1),3,u(2,1),3,d,e)
c perform the column transformation z1
      call giv (u(2,2),u(2,3),d,e)
      u(1,2)=u(1,2)*e-u(1,3)*d
      call drot(l2,a(1,l1),1,a(1,l2),1,e,-d)
      call drot(n,z(1,l1),1,z(1,l2),1,e,-d)
c perform the row transformation z1'
      call drot(n-l+1,a(l1,l),nmax,a(l2,l),nmax,e,-d)
c perform the column transformation z2
      call giv (u(1,1),u(1,2),d,e)
      call drot(l2,a(1,l),1,a(1,l1),1,e,-d)
      call drot(n,z(1,l),1,z(1,l1),1,e,-d)
c perform the row transformation z2'
      call drot(n-l+1,a(l,l),nmax,a(l1,l),nmax,e,-d)
c  put the neglectable elements equal to zero
 140  a(l1,l)=0.0d+0
      a(l2,l)=0.0d+0
      return
c ** interchange 2x2 and 2x2 blocks via a sequence of
c **  equivalence transformations
c **          a:=z5'*z4'*z3'*z2'*z1'*a*z1*z2*z3*z4*z5
c ** where each zi is a givens rotation
 150  l3=l+3
      d=a(l2,l2)*a(l3,l3)-a(l2,l3)*a(l3,l2)
      e=a(l2,l2)+a(l3,l3)
      call dmmul(a(l,l),nmax,a(l,l),nmax,u,3,2,4,4)
      do 20 i=1,2
      u(i,i)=u(i,i)+d
      do 10 j=1,4
      u(i,j)=u(i,j)-e*a(l-1+i,l-1+j)
   10 continue
   20 continue
c g0
      call giv(u(1,1),u(2,1),d,e)
      call drot(4,u(1,1),3,u(2,1),3,d,e)
c z1
      call giv(u(2,4),u(2,3),d,e)
      call drot(2,u(1,4),1,u(1,3),1,d,e)
      call drot(l3,a(1,l3),1,a(1,l2),1,d,e)
      call drot(n,z(1,l3),1,z(1,l2),1,d,e)
c z1'
      call drot(n-l+1,a(l3,l),nmax,a(l2,l),nmax,d,e)
c z2
      call giv(u(2,4),u(2,2),d,e)
      call drot(2,u(1,4),1,u(1,2),1,d,e)
      call drot(l3,a(1,l3),1,a(1,l1),1,d,e)
      call drot(n,z(1,l3),1,z(1,l1),1,d,e)
c z2'
      u(2,4)=d*u(2,4)
      call drot(n-l+1,a(l3,l),nmax,a(l1,l),nmax,d,e)
c z3
      call giv(u(1,3),u(1,2),d,e)
      call drot(1,u(1,3),1,u(1,2),1,d,e)
      call drot(l3,a(1,l2),1,a(1,l1),1,d,e)
      call drot(n,z(1,l2),1,z(1,l1),1,d,e)
c z3'
      u(2,4)=d*u(2,4)
      call drot(n-l+1,a(l2,l),nmax,a(l1,l),nmax,d,e)
c z4
      call giv(u(1,3),u(1,1),d,e)
      call drot(l3,a(1,l2),1,a(1,l),1,d,e)
      call drot(n,z(1,l2),1,z(1,l),1,d,e)
c z4'
      call drot(n-l+1,a(l2,l),nmax,a(l,l),nmax,d,e)
c zeroes negligible elements
      a(l2,l)=0.0d+0
      a(l3,l)=0.0d+0
      a(l2,l1)=0.0d+0
      a(l3,l1)=0.0d+0
      return
      end
