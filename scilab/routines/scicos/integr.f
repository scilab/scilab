      subroutine integr(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Integrator
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      common /dbcos/ idb
c
      if(idb.eq.1) then
         write(6,'(''Integr   t='',e10.3,'' flag='',i1)') t,flag
      endif
c
      if(flag.eq.1.or.flag.eq.6) then
         y(1)=x(1)
      elseif(flag.eq.0) then
         xd(1)=u(1)
      endif
      end

