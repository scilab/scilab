      subroutine cstblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     output a vector of constants out(i)=rpar(i)
c     rpar(1:nrpar) : given constants
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,ny

c
      common /dbcos/ idb
c
      if(idb.eq.1) then
         write(6,'(''Const     t='',e10.3,'' flag='',i1)') t,flag
      endif
c
      call dcopy(nrpar,rpar,1,y,1)
      return
      end
