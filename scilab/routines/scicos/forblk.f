      subroutine forblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     For block
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      double precision atol,rtol,ttol,deltat
      common /costol/ atol,rtol,ttol,deltat
c
c     
      if(flag.eq.3) then
         if(nevprt.eq.1) then
            z(2)=u(1)
            z(1)=1.d0
c     
            if(u(1).ge.1) then
               tvec(1)=t-1.d0
               tvec(2)=t+ttol/2.0d0
            else
               tvec(1)=t-1.d0
               tvec(2)=t-1.d0
            endif

         else
            z(1)=z(1)+1.d0
c     
            if(z(1).ge.z(2)) then
               tvec(1)=t+ttol/2.0d0
               tvec(2)=t-1.d0
            else
               tvec(1)=t-1.d0
               tvec(2)=t+ttol/2.0d0
            endif
         endif
      endif

      if(flag.eq.1.or.flag.eq.3) then
         y(1)=z(1)
      endif
      
      end
