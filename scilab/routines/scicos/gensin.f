      subroutine gensin(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Sine genrator
c     ampl=rpar(1)
c     freq=rpar(2)
c     phase=rpar(3)
c
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
c
         y(1)=rpar(1)*sin(rpar(2)*t+rpar(3))
      end
