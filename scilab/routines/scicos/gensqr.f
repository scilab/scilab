      subroutine gensqr(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Square wave generator
c     period=2*rpar(1)
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      common /dbcos/ idb
      double precision atol,rtol,ttol,deltat
      common /costol/ atol,rtol,ttol,deltat
c
      if(idb.eq.1) then
         write(6,'(''gensqr     t='',e10.3,'' flag='',i1)') t,flag
      endif
c
      if(flag.eq.3) then
         tvec(1)=t+max(rpar(1),ttol/2)
      elseif(flag.eq.2) then
         z(1)=-z(1)
      elseif(flag.eq.1.or.flag.eq.6) then
         y(1)=z(1)
      endif
      end

