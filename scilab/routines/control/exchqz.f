        subroutine exchqz(nmax,n,a,b,z,l,ls1,ls2,eps,fail)
        integer nmax,n,l,ls1,ls2
        double precision a(nmax,n),b(nmax,n),z(nmax,n),eps
        logical fail
c!purpose
c  given the upper triangular matrix b and upper hessenberg matrix a
c  with consecutive ls1xls1 and ls2xls2 diagonal blocks (ls1,ls2.le.2)
c  starting at row/column l, exchqz produces equivalence transforma-
c  tions qt and zt that exchange the blocks along with their generalized
c  eigenvalues.
c
c!calling sequence
c
c     subroutine exchqz(nmax,n,a,b,z,l,ls1,ls2,eps,fail)
c     integer nmax,n,l,ls1,ls2
c     double precision a(nmax,n),b(nmax,n),z(nmax,n),eps
c     logical fail
c
c     nmax     the first dimension of a, b and z
c     n        the order of a, b and z
c    *a,*b     the matrix pair whose blocks are to be interchanged
c    *z        upon return this array is multiplied by the column
c              transformation zt.
c     l        the position of the blocks
c     ls1      the size of the first block
c     ls2      the size of the second block
c     eps      the required absolute accuracy of the result
c    *fail     a logical variable which is false on a normal return,
c              true otherwise.
c
c!auxiliary routines
c     drot (blas)
c     giv
c     max abs (fortran)
c!originator
c     VanDooren
c!
      integer i,j,l1,l2,l3,li,lj,ll,it1,it2
      double precision u(3,3),d,e,f,g,sa,sb,a11b11,a21b11,a12b22,
     1 b12b22,a22b22
     1,ammbmm,anmbmm,amnbnn,bmnbnn,annbnn
      logical altb
      fail=.false.
      l1=l+1
      ll=ls1+ls2
      if (ll.gt.2) go to 50
c ** interchange 1x1 and 1x1 blocks via an equivalence
c ** transformation       a:=q*a*z , b:=q*b*z
c ** where q and z are givens rotations
      f=max(abs(a(l1,l1)),abs(b(l1,l1)))
      altb=.true.
      if(abs(a(l1,l1)).ge.f) altb=.false.
      sa=a(l1,l1)/f
      sb=b(l1,l1)/f
      f=sa*b(l,l)-sb*a(l,l)
c construct the column transformation z
      g=sa*b(l,l1)-sb*a(l,l1)
      call giv(f,g,d,e)
      call drot(l1,a(1,l),1,a(1,l1),1,e,-d)
      call drot(l1,b(1,l),1,b(1,l1),1,e,-d)
      call drot(n,z(1,l),1,z(1,l1),1,e,-d)
c construct the row transformation q
      if (altb) call giv(b(l,l),b(l1,l),d,e)
      if (.not.altb) call giv(a(l,l),a(l1,l),d,e)
      call drot(n-l+1,a(l,l),nmax,a(l1,l),nmax,d,e)
      call drot(n-l+1,b(l,l),nmax,b(l1,l),nmax,d,e)
      a(l1,l)=0.0d+0
      b(l1,l)=0.0d+0
      return
c ** interchange 1x1 and 2x2 blocks via an equivalence
c ** transformation  a:=q2*q1*a*z1*z2 , b:=q2*q1*b*z1*z2
c ** where each qi and zi is a givens rotation
 50   l2=l+2
      if(ls1.eq.2) go to 100
      g=max(abs(a(l,l)),abs(b(l,l)))
      altb=.true.
      if(abs(a(l,l)).lt.g) go to 60
      altb=.false.
      call giv(a(l1,l1),a(l2,l1),d,e)
      call drot(n-l,a(l1,l1),nmax,a(l2,l1),nmax,d,e)
      call drot(n-l,b(l1,l1),nmax,b(l2,l1),nmax,d,e)
c *  evaluate the pencil at the eigenvalue corresponding
c *  to the 1x1 block
 60   sa=a(l,l)/g
      sb=b(l,l)/g
      do 80 j=1,2
           lj=l+j
           do 80 i=1,3
               li=l+i-1
 80            u(i,j)=sa*b(li,lj)-sb*a(li,lj)
      call giv(u(3,1),u(3,2),d,e)
      call drot(3,u(1,1),1,u(1,2),1,e,-d)
c perform the row transformation q1
      call giv(u(1,1),u(2,1),d,e)
      u(2,2)=-u(1,2)*e+u(2,2)*d
      call drot(n-l+1,a(l,l),nmax,a(l1,l),nmax,d,e)
      call drot(n-l+1,b(l,l),nmax,b(l1,l),nmax,d,e)
c perform the column transformation z1
      if (altb) call giv(b(l1,l),b(l1,l1),d,e)
      if (.not.altb) call giv(a(l1,l),a(l1,l1),d,e)
      call drot(l2,a(1,l),1,a(1,l1),1,e,-d)
      call drot(l2,b(1,l),1,b(1,l1),1,e,-d)
      call drot(n,z(1,l),1,z(1,l1),1,e,-d)
c perform the row transformation q2
      call giv(u(2,2),u(3,2),d,e)
      call drot(n-l+1,a(l1,l),nmax,a(l2,l),nmax,d,e)
      call drot(n-l+1,b(l1,l),nmax,b(l2,l),nmax,d,e)
c perform the column transformation z2
      if (altb) call giv (b(l2,l1),b(l2,l2),d,e)
      if (.not.altb) call giv(a(l2,l1),a(l2,l2),d,e)
      call drot(l2,a(1,l1),1,a(1,l2),1,e,-d)
      call drot(l2,b(1,l1),1,b(1,l2),1,e,-d)
      call drot(n,z(1,l1),1,z(1,l2),1,e,-d)
      if (altb) go to 90
      call giv(b(l,l),b(l1,l),d,e)
      call drot(n-l+1,a(l,l),nmax,a(l1,l),nmax,d,e)
      call drot(n-l+1,b(l,l),nmax,b(l1,l),nmax,d,e)
c  put the neglectable elements equal to zero
 90   a(l2,l)=0.0d+0
      a(l2,l1)=0.0d+0
      b(l1,l)=0.0d+0
      b(l2,l)=0.0d+0
      b(l2,l1)=0.0d+0
      return
c ** interchange 2x2 and 1x1 blocks via an equivalence
c ** transformation  a:=q2*q1*a*z1*z2 , b:=q2*q1*b*z1*z2
c ** where each qi and zi is a givens rotation
 100  if(ls2.eq.2) go to 150
      g=max(abs(a(l2,l2)),abs(b(l2,l2)))
      altb=.true.
      if(abs(a(l2,l2)).lt.g) go to 120
      altb=.false.
      call giv(a(l,l),a(l1,l),d,e)
      call drot(n-l+1,a(l,l),nmax,a(l1,l),nmax,d,e)
      call drot(n-l+1,b(l,l),nmax,b(l1,l),nmax,d,e)
c *  evaluate the pencil at the eigenvalue corresponding
c *  to the 1x1 block
 120  sa=a(l2,l2)/g
      sb=b(l2,l2)/g
      do 130 i=1,2
           li=l+i-1
           do 130 j=1,3
               lj=l+j-1
 130           u(i,j)=sa*b(li,lj)-sb*a(li,lj)
      call giv (u(1,1),u(2,1),d,e)
      call drot(3,u(1,1),3,u(2,1),3,d,e)
c perform the column transformation z1
      call giv (u(2,2),u(2,3),d,e)
      u(1,2)=u(1,2)*e-u(1,3)*d
      call drot(l2,a(1,l1),1,a(1,l2),1,e,-d)
      call drot(l2,b(1,l1),1,b(1,l2),1,e,-d)
      call drot(n,z(1,l1),1,z(1,l2),1,e,-d)
c perform the row transformation q1
      if (altb) call giv (b(l1,l1),b(l2,l1),d,e)
      if (.not.altb) call giv (a(l1,l1),a(l2,l1),d,e)
      call drot(n-l+1,a(l1,l),nmax,a(l2,l),nmax,d,e)
      call drot(n-l+1,b(l1,l),nmax,b(l2,l),nmax,d,e)
c perform the column transformation z2
      call giv (u(1,1),u(1,2),d,e)
      call drot(l2,a(1,l),1,a(1,l1),1,e,-d)
      call drot(l2,b(1,l),1,b(1,l1),1,e,-d)
      call drot(n,z(1,l),1,z(1,l1),1,e,-d)
c perform the row transformation q2
      if(altb) call giv(b(l,l),b(l1,l),d,e)
      if(.not.altb) call giv(a(l,l),a(l1,l),d,e)
      call drot(n-l+1,a(l,l),nmax,a(l1,l),nmax,d,e)
      call drot(n-l+1,b(l,l),nmax,b(l1,l),nmax,d,e)
      if(altb) go to 140
      call giv(b(l1,l1),b(l2,l1),d,e)
      call drot(n-l,a(l1,l1),nmax,a(l2,l1),nmax,d,e)
      call drot(n-l,b(l1,l1),nmax,b(l2,l1),nmax,d,e)
c  put the neglectable elements equal to zero
 140  a(l1,l)=0.0d+0
      a(l2,l)=0.0d+0
      b(l1,l)=0.0d+0
      b(l2,1)=0.0d+0
      b(l2,l1)=0.0d+0
      return
c ** interchange 2x2 and 2x2 blocks via a sequence of
c ** qz-steps realized by the equivalence transformations
c **          a:=q5*q4*q3*q2*q1*a*z1*z2*z3*z4*z5
c **          b:=q5*q4*q3*q2*q1*b*z1*z2*z3*z4*z5
c ** where each qi and zi is a givens rotation
 150  l3=l+3
c compute implicit shift
      ammbmm=a(l,l)/b(l,l)
      anmbmm=a(l1,l)/b(l,l)
      amnbnn=a(l,l1)/b(l1,l1)
      annbnn=a(l1,l1)/b(l1,l1)
      bmnbnn=b(l,l1)/b(l1,l1)
      do 180 it1=1,3
        u(1,1)=1.0d+0
        u(2,1)=1.0d+0
        u(3,1)=1.0d+0
        do 180 it2=1,10
c perform row transformations q1 and q2
          call giv(u(2,1),u(3,1),d,e)
          call drot(n-l+1,a(l1,l),nmax,a(l2,l),nmax,d,e)
          call drot(n-l,b(l1,l1),nmax,b(l2,l1),nmax,d,e)
          u(2,1)=d*u(2,1)+e*u(3,1)
          call giv(u(1,1),u(2,1),d,e)
          call drot(n-l+1,a(l,l),nmax,a(l1,l),nmax,d,e)
          call drot(n-l+1,b(l,l),nmax,b(l1,l),nmax,d,e)
c perform column transformations z1 and z2
          call giv(b(l2,l1),b(l2,l2),d,e)
          call drot(l3,a(1,l1),1,a(1,l2),1,e,-d)
          call drot(l2,b(1,l1),1,b(1,l2),1,e,-d)
          call drot(n,z(1,l1),1,z(1,l2),1,e,-d)
          call giv(b(l1,l),b(l1,l1),d,e)
          call drot(l3,a(1,l),1,a(1,l1),1,e,-d)
          call drot(l1,b(1,l),1,b(1,l1),1,e,-d)
          call drot(n,z(1,l),1,z(1,l1),1,e,-d)
c perform transformations q3,z3,q4,z4,q5 and z5 in
c order to reduce the pencil to hessenberg form
          call giv(a(l2,l),a(l3,l),d,e)
          call drot(n-l+1,a(l2,l),nmax,a(l3,l),nmax,d,e)
          call drot(n-l1,b(l2,l2),nmax,b(l3,l2),nmax,d,e)
          call giv(b(l3,l2),b(l3,l3),d,e)
          call drot(l3,a(1,l2),1,a(1,l3),1,e,-d)
          call drot(l3,b(1,l2),1,b(1,l3),1,e,-d)
          call drot(n,z(1,l2),1,z(1,l3),1,e,-d)
          call giv(a(l1,l),a(l2,l),d,e)
          call drot(n-l+1,a(l1,l),nmax,a(l2,l),nmax,d,e)
          call drot(n-l,b(l1,l1),nmax,b(l2,l1),nmax,d,e)
          call giv(b(l2,l1),b(l2,l2),d,e)
          call drot(l3,a(1,l1),1,a(1,l2),1,e,-d)
          call drot(l2,b(1,l1),1,b(1,l2),1,e,-d)
          call drot(n,z(1,l1),1,z(1,l2),1,e,-d)
          call giv(a(l2,l1),a(l3,l1),d,e)
          call drot(n-l,a(l2,l1),nmax,a(l3,l1),nmax,d,e)
          call drot(n-l1,b(l2,l2),nmax,b(l3,l2),nmax,d,e)
          call giv(b(l3,l2),b(l3,l3),d,e)
          call drot(l3,a(1,l2),1,a(1,l3),1,e,-d)
          call drot(l3,b(1,l2),1,b(1,l3),1,e,-d)
          call drot(n,z(1,l2),1,z(1,l3),1,e,-d)
c test of convergence on the element separating the blocks
          if(abs(a(l2,l1)).le.eps) go to 190
c compute a new shift in case of no convergence
          a11b11=a(l,l)/b(l,l)
          a12b22=a(l,l1)/b(l1,l1)
          a21b11=a(l1,l)/b(l,l)
          a22b22=a(l1,l1)/b(l1,l1)
          b12b22=b(l,l1)/b(l1,l1)
          u(1,1)=((ammbmm-a11b11)*(annbnn-a11b11)-amnbnn*anmbmm
     1         +anmbmm*bmnbnn*a11b11)/a21b11+a12b22-a11b11*b12b22
          u(2,1)=(a22b22-a11b11)-a21b11*b12b22-(ammbmm-a11b11)
     1         -(annbnn-a11b11)+anmbmm*bmnbnn
 180      u(3,1)=a(l2,l1)/b(l1,l1)
      fail=.true.
      return
c  put the neglectable elements equal to zero in
c  case of convergence
 190  a(l2,l)=0.0d+0
      a(l2,l1)=0.0d+0
      a(l3,l)=0.0d+0
      a(l3,l1)=0.0d+0
      b(l1,l)=0.0d+0
      b(l2,l)=0.0d+0
      b(l2,l1)=0.0d+0
      b(l3,l)=0.0d+0
      b(l3,l1)=0.0d+0
      b(l3,l2)=0.0d+0
      return
      end
