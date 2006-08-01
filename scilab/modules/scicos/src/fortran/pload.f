      subroutine pload(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Preload function
c     if u(i).lt.0 then y(i)=-u(i)-rpar(i)
c     else y(i)=u(i)+rpar(i)
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
c     

 10   do 15 i=1,nu
         if (u(i).lt.0.0d0)then
            y(i)=u(i)-rpar(i)
         else if(u(i).gt.0.0d0)then
            y(i)=u(i)+rpar(i)
         else
            y(i)=0.0d0
         endif
 15   continue

      end
