      subroutine intpol(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     y=f(u) for f a tabulated function from R to R^ny
c
c     rpar(1:ny+1,1:np) : matrix of mesh point coordinates
c                       first row contains u coordinate mesh points
c                       next rows contains y coordinates mesh points
c                       (one row for each output)
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
c
      np=ipar(1)
      call intp(u,rpar(1),rpar(1+np),ny,np,y)

      end
     
