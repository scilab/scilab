      subroutine sawtth(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      common /dbcos/ idb
c
c
      if(idb.eq.1) then
         write(6,'(''sawtth     t='',e10.3,'' flag='',i1)') t,flag
      endif
c
      if (flag.eq.1) then
         y(1)=t-z(1)

      elseif (flag.eq.2.and.nevprt.eq.1) then
         z(1)=t
      elseif (flag.eq.4) then
         z(1)=0.0d0
      endif
      return
      end
