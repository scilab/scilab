      subroutine bound(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
c     Copyright INRIA
      &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Scicos block simulator
c     Bound y(i)=rpar(nu+i) if u(i)>rpar(i) else y(i)=0
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      do 15 i=1,nu
         if (u(i).ge.rpar(i)) then
           y(i)=rpar(nu+i)
         else
           y(i)=0.0d0
         endif
 15   continue
      end
