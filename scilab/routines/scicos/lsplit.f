      subroutine lsplit(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     splitting signals
cc
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      common /dbcos/ idb
c
      if(idb.eq.1) then
         write(6,'(''Split     t='',e10.3,'' flag='',i1)') t,flag
      endif

      j=0
      do 2 i=1,ny/nu
         do 1 k=1,nu
            j=j+1
            y(j)=u(k)
 1       continue
 2    continue
      end
