      subroutine lusat(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny,g,ng)
c     Copyright INRIA

c     Scicos block simulator
c     Lower-Upper saturation
c     Continous block, MIMO
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      double precision g(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny,ng

c
      if (flag.eq.9) then
         do 10 i=1,nu
            g(i)=u(i)-rpar(1)
            g(i+nu)=u(i)-rpar(2)
 10      continue
      endif

      if (flag.eq.1) then
         do 15 i=1,nu
            if (u(i).le.rpar(1))then
               y(i)=rpar(1)*rpar(3)
            else if(u(i).ge.rpar(2))then
               y(i)=rpar(2)*rpar(3)
            else
               y(i)=rpar(3)*u(i)
            endif
 15      continue
      endif
      end
