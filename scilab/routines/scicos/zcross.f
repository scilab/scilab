      subroutine zcross(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     zero crossing block
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny
c   
      common /dbcos/ idb
c
      if(idb.eq.1) then
         write(6,'(''Zcross     t='',e10.3,'' flag='',i1)') t,flag
      endif

      if (flag.eq.3) then
         l=nevprt*ntvec
         do 10 i=1,ntvec
            tvec(i)=rpar(l+i)+t  
 10      continue
      endif
      end
