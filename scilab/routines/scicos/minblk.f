      subroutine minblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     outputs the minimum of all inputs
cc
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny
c
      double precision ww
c      
      ww=u(1)
      do 15 i=1,nu
         ww=min(ww,u(i))
 15   continue
      y(1)=ww
      
      end
