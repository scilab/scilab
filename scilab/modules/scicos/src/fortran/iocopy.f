      subroutine iocopy(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA
      include 'stack.h'

c     Scicos block simulator
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      common /dbcos/ idb
c
      if(idb.eq.1) then
c				 replaces write(6 ...) by basout bug 2598                  
c        write(6,'(''iocopy     t='',e10.3,'' flag='',i1)') t,flag
        write(buf,'(''iocopy     t='',e10.3,'' flag='',i1)') t,flag
        call basout(io,wte,buf)
      endif
c     
      do 15 i=1,nu
         y(i)=u(i)
 15   continue
      
      end
