      subroutine dmmul(a,na,b,nb,c,nc,l,m,n)
c!
c     
c     c=a*b .
c!
c
c     subroutine dmmul(a,na,b,nb,c,nc,l,m,n)
c     double precision a(na,m),b(nb,n),c(nc,n)
c     integer na,nb,nc,l,m,n
c
c     a            workspace of size na*m containing a
c     na           number of rows of array a in calling program
c     b,nb,c,nc    idem for b and c
c     l            # of rows of matrix  a and c
c     m            # of columns of a and rows of b
c     n            # of columns of b and c
c!
c     Copyright INRIA
      double precision a(*),b(*),c(*)
      double precision ddot
      integer na,nb,nc,l,m,n
      integer i,j,ib,ic
c
      ib=1
      ic=0
      do 30 j=1,n
         do 20 i=1,l
   20    c(ic+i)=ddot(m,a(i),na,b(ib),1)
      ic=ic+nc
      ib=ib+nb
   30 continue
      return
      end
