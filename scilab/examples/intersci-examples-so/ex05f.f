      subroutine foubare2(ch,a,ia,b,ib,c,mc,nc,d,w)
c     -----------------------------------------
c     -----------   EXAMPLE   -----------------
c     inputs:  ch, a,b and c; ia,ib and mc,nc
c     ch=character, a=integer, b=real and c=double 
c     ia,ib and [mc,nc] are the dimensions of a,b and c resp.
c     outputs: a,b,c,d
c     if ch='mul'   a,b and c = 2 * (a,b and c) 
c     and d of same dimensions as c with
c     d(i,j)=(i+j)*c(i,j)
c     if ch='add' a,b and c = 2 + (a,b and c)
c     d(i,j)=(i+j)+c(i,j)
c     w is a working array of size [mc,nc]
c     -------------------------------------------
      character*(*) ch
      integer a(*)
      real b(*)
      double precision c(mc,*),d(mc,*),w(mc,*)
      if(ch(1:3).eq.'mul') then
      do 1 k=1,ia
         a(k)=2*a(k)
 1    continue
      do 2 k=1,ib
         b(k)=2.0*b(k)
 2    continue
      do 3 i=1,mc
      do 3 j=1,nc
         c(i,j)=2.0d0*c(i,j)
 3    continue
      do 4 i=1,mc
      do 4 j=1,nc
       w(i,j)=dble(i+j)
       d(i,j)=w(i,j)*c(i,j)
 4    continue
      elseif(ch(1:3).eq.'add') then
      do 10 k=1,ia
         a(k)=2+a(k)
 10   continue
      do 20 k=1,ib
         b(k)=2.0+b(k)
 20   continue
      do 30 i=1,mc
      do 30 j=1,nc
         c(i,j)=2.0d0+c(i,j)
 30   continue
      do 40 i=1,mc
      do 40 j=1,nc
       w(i,j)=dble(i+j)
       d(i,j)=w(i,j)+c(i,j)
 40   continue
      endif
      end

