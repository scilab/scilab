      subroutine hltblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Notify simulation to stop  when called 
c     ipar(1) : stop reference
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      integer halt
      common /coshlt/ halt
c
      if(flag.eq.2) then
         halt=1
         if(nipar.gt.0) then
            z(1)=ipar(1)
         else
            z(1)=0.0d0
         endif
      endif
      end
