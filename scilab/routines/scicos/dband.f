      subroutine dband(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Dead Band,
c     if u(i)<0 ,y(i)=min(0,u+DB(i)/2)
c     else       y(i)=max(0,u-DB(i)/2)
c     DB(i)=rpar(i)
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c     
 10   do 15 i=1,nu
         if (u(i).lt.0)then
            y(i)=min(0.0d0,u(i)+rpar(i)/2.0d0)
         else if(u(i).ge.0)then
            y(i)=max(0.0d0,u(i)-rpar(i)/2.0d0)
         endif
 15   continue
      end
