      subroutine constraint(flag,nevprt,t,res,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c
      double precision t,xd(*),x(*),z(*),res(*),tvec(*),rpar(*)
      double precision u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      common /dbcos/ idb
c     
      if (flag.eq.0) then
         do 12 i=1,nu
            res(i)=xd(i)-u(i)
            res(i+nu)=xd(i)
 12      continue
      elseif(flag.eq.1) then
         do 14 i=1,ny
            y(i)=xd(i+nu)
 14      continue
c      elseif(flag.eq.6.or.flag.eq.7) then
c         do 12 i=1,nu
c 12      continue
      endif
      end

