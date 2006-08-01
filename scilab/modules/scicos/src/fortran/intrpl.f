      subroutine intrpl(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u1,nu1,y1,ny1)
c     Copyright INRIA

c     Scicos block simulator
c     ipar(1) : the number of input
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*)
      double precision y1,u1
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*),i
      integer nipar
c
c    
      do 100 i=2,nrpar/2
         if (u1.le.rpar(i)) goto 200 
 100  continue
      i=nrpar/2
 200  continue
      y1=rpar(nrpar/2+i-1) + ((rpar(nrpar/2+i)-rpar(nrpar/2+i-1))/
     $     (rpar(i)-rpar(i-1)))*(u1-rpar(i-1))
      return
      end


