      subroutine sum2(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u1,nu1,u2,nu2,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     adds the inputs weighed by rpar
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u1(*),u2(*)
      double precision y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      common /dbcos/ idb
c
      if(idb.eq.1) then
         write(6,'(''Sum2     t='',e10.3,'' flag='',i1)') t,flag
      endif

      do 1 i=1,nu1
         y(i)=u1(i)*rpar(1)+u2(i)*rpar(2)
 1    continue
      return
      end
