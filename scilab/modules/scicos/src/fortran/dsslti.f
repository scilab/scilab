      subroutine dsslti(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     discrete state space linear system simulator
c     rpar(1:nx*nx)=A
c     rpar(nx*nx+1:nx*nx+nx*nu)=B
c     rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny
      double precision w(100)
c
c
      la=1
      lb=nz*nz+la
      lc=lb+nz*nu
      if(flag.eq.4) then
         if(nz.gt.100) then
            flag=-1
            return
         endif
      elseif(flag.eq.2) then
c     x+=a*x+b*u
         call dcopy(nz,z,1,w,1)
         call dmmul(rpar(la),nz,w,nz,z,nz,nz,nz,1)
         call dmmul1(rpar(lb),nz,u,nu,z,nz,nz,nu,1)
      elseif(flag.eq.1.or.flag.eq.6) then
c     y=c*x+d*u
         ld=lc+nz*ny
         call dmmul(rpar(lc),ny,z,nz,y,ny,ny,nz,1)
         call dmmul1(rpar(ld),ny,u,nu,y,ny,ny,nu,1)
      endif
      end

