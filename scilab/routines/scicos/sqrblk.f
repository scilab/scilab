      subroutine sqrblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
c     
      do 15 i=1,nu
         if(u(i).ge.0.0d0) then
            y(i)=sqrt(u(i))
         else
            flag=-2
            return
         endif
 15   continue

      end
