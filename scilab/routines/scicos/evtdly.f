      subroutine evtdly(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA
c     Scicos block simulator
c     event delay
c     delay=rpar(1)
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      common /dbcos/ idb
c
      if(idb.eq.1) then
         write(6,'(''Evtdly     t='',e10.3,'' flag='',i1)') t,flag
      endif
c
      if(flag.eq.3) then
         tvec(1)=t+rpar(1)
      endif
      return
      end
