      subroutine maxblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     outputs the maximum of all inputs
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      double precision ww
c
      common /dbcos/ idb
c
      if(idb.eq.1) then
         write(6,'(''Maxblk   t='',e10.3,'' flag='',i1)') t,flag
      endif
c     
      ww=u(1)
      do 15 i=1,nu
         ww=max(ww,u(i))
 15   continue
      y(1)=ww
      end
