      subroutine qzflr(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c
c     Gives quantized signal by floor method
c     rpar(i) quantization step used for i input

c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny


      integer i
c
      do 15 i=1,nu
         y(i)=rpar(i)*ANINT(u(i)/rpar(i)+0.5d0)
 15   continue
      return
      end
