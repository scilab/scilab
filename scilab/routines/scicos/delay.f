      subroutine delay(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Ouputs nx*dt delayed input
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      if(flag.eq.1.or.flag.eq.4.or.flag.eq.6) then
         y(1)=z(1)
      elseif(flag.eq.2) then
c     .  shift buffer   
         do 10 i=1,nz-1
            z(i)=z(i+1)
 10      continue
c     .  add new point to the buffer
         z(nz)=u(1)
      endif
      end
