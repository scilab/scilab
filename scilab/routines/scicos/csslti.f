      subroutine csslti(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA
c     Scicos block simulator
c     continuous state space linear system simulator
c     rpar(1:nx*nx)=A
c     rpar(nx*nx+1:nx*nx+nx*nu)=B
c     rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C
c     rpar(nx*nx+nx*nu+nx*ny+1:nx*nx+nx*nu+nx*ny+ny*nu)=D
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      la=1
      lb=nx*nx+la
      lc=lb+nx*nu

      if(flag.eq.1.or.flag.eq.6) then
c     y=c*x+d*u     
         ld=lc+nx*ny
         call dmmul(rpar(lc),ny,x,nx,y,ny,ny,nx,1)
         call dmmul1(rpar(ld),ny,u,nu,y,ny,ny,nu,1)
c         if(t.gt.64.0) write(6,'(e15.8,10(e10.3,x))') t,x(1),x(2),
c     $        u(1),y(1) 
      elseif(flag.eq.0) then
c     xd=a*x+b*u
         call dmmul(rpar(la),nx,x,nx,xd,nx,nx,nx,1)
         call dmmul1(rpar(lb),nx,u,nu,xd,nx,nx,nu,1)
c         if(t.gt.64.0) write(6,'(e15.8,10(e10.3,x))') t,x(1),x(2),
c     $        xd(1),xd(2),u(1)
      endif
      return
      end
