      subroutine selblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Selector block
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny
c
      common /dbcos/ idb
c
      if(idb.eq.1) then
         write(6,'(''Select     t='',e10.3,'' flag='',i1)') t,flag
      endif

c
      if(flag.eq.2.and.nevprt.gt.0) then
         ic=0
         nev=nevprt
 10      continue
         if(nev.ge.1) then
            ic=ic+1
            nev=nev/2
            goto 10
         endif
         z(1)=dble(ic)
      elseif(flag.eq.1.or.flag.eq.6) then
      y(1)=u(int(z(1)))
      endif
      end

