      subroutine mfclck(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c
c     multifrequency clock
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      if(flag.eq.4) then
         z(1)=0.0d0
      elseif(flag.eq.2) then
         z(1)=z(1)+1.0d0
         if (z(1).eq.ipar(1)) z(1)=0.0d0
      elseif(flag.eq.3) then
         if (z(1).eq.ipar(1)-1) then
            tvec(1)=t-1.0d0
            tvec(2)=t+rpar(1)
         else
            tvec(1)=t+rpar(1)
            tvec(2)=t-1.0d0
         endif
      endif
      end

