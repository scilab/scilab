      subroutine lookup(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     rpar(1:n)  =  u coordinate discretisation must be strictly increasing
c     rpar(n+1:2*n)  =  y coordinate discretisation
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      double precision du,dout
      integer i
c
      n=nrpar/2
      if(n.gt.2) then
         do 10 i=2,n-1
            if (u(1).le.rpar(i)) goto 20
 10      continue
      else
         if(n.eq.1) then
            y(1)=rpar(2)
            return
         endif
         i=2
      endif
 20   continue
      du=rpar(i)-rpar(i-1)
      dout=rpar(n+i)-rpar(n+i-1)
      y(1)=rpar(n+i)-(rpar(i)-u(1))*dout/du
      end
