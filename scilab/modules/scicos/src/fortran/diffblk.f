      subroutine diffblk(flag,nevprt,t,res,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c
      double precision t,xd(*),x(*),z(*),res(*),tvec(*),rpar(*)
      double precision u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
c     
      if (flag.eq.0) then
         do 10 i=1,nu
            res(i)=x(i)-u(i)
 10      continue
      elseif(flag.eq.1) then
         do 11 i=1,nu
            y(i)=xd(i)
 11      continue
      elseif(flag.eq.6.or.flag.eq.7) then
         do 12 i=1,nu
            x(i)=u(i)
 12      continue
      endif
      end

