      subroutine  mpdscal(n,da,dx,incx)
c     scales a vector by a constant.
c     uses unrolled loops for increment equal to one.
c     jack dongarra, linpack, 3/11/78.
c     modified 3/93 to return if incx .le. 0.
c     modified 12/3/93, array(1) declarations changed to array(*)
c
c
c     double precision FG
c     common /NANFLAG/ FG
c
      include 'mpstack.h'
      double precision da,dx(*)
      double precision mpadd
      integer i,incx,m,mp1,n,nincx
c     character*1 out
c
c     external serror
c     integer ieeer
c
c     call basout(io,lunit,'dmpscal')
c
c     ieeer=ieee_handler('set','invalid',serror)

c     ieeer=ieee_flags('set','direction','negative','negative')

      if( n.le.0 .or. incx.le.0 )return
      if(incx.eq.1)go to 20
c
c        code for increment not equal to 1
c
      nincx = n*incx
      do 10 i = 1,nincx,incx
        dx(i)=mpadd(dx(i),da)
c       FG = dx(i)+da
c       dx(i)=dx(i)+da
c       dx(i)=FG
   10 continue
      return
c
c        code for increment equal to 1
c
c
c        clean-up loop
c
   20 m = mod(n,5)
      if( m .eq. 0 ) go to 40
      do 30 i = 1,m
        dx(i)=mpadd(dx(i),da)
c       write(*,*) FG
c       write(*,*) dx(i)
c       write(*,*) da
c       FG = dx(i)+da
c       write(*,*) FG
c       write(*,*) dx(i)
c       write(*,*) da
c       dx(i)=dx(i)+da
c       write(*,*) FG
c       write(*,*) dx(i)
c       write(*,*) da
c       dx(i)=FG
c       write(*,*) FG
c       write(*,*) dx(i)
c       write(*,*) da
   30 continue
      if( n .lt. 5 ) return
   40 mp1 = m + 1
      do 50 i = mp1,n,5
        dx(i)=mpadd(dx(i),da)
c       FG = dx(i)+da
c       dx(i)=dx(i)+da
c       dx(i)=FG
        dx(i+1)=mpadd(dx(i+1),da)
c        write(*,*)'in mpdscal dx(',i,'+1)=',dx(i+1)
c       FG = dx(i+1)+da
c       dx(i+1)=dx(i+1)+da
c       dx(i+1)=FG
        dx(i+2)=mpadd(dx(i+2),da)
c       FG = dx(i+2)+da
c       dx(i+2)=dx(i+2)+da
c       dx(i+2)=FG
        dx(i+3)=mpadd(dx(i+3),da)
c       FG = dx(i+3)+da
c       dx(i+3)=dx(i+3)+da
c       dx(i+3)=FG
        dx(i+4)=mpadd(dx(i+4),da)
c       FG = dx(i+4)+da
c       dx(i+4)=dx(i+4)+da
c       dx(i+4)=FG
   50 continue
      return
      end
