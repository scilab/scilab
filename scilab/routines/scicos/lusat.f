      subroutine lusat(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Lower-Upper saturation
c     Continous block, MIMO
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      common /dbcos/ idb
c
      if(idb.eq.1) then
         write(6,'(''lusat     t='',e10.3,'' flag='',i1)') t,flag
      endif

 10   do 15 i=1,nu
         if (u(i).le.rpar(1))then
            y(i)=rpar(1)*rpar(3)
         else if(u(i).ge.rpar(2))then
            y(i)=rpar(2)*rpar(3)
         else
            y(i)=rpar(3)*u(i)
         endif
 15   continue
      end
