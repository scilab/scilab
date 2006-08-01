      subroutine invblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Outputs the inverse of the input
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny


      double precision ww
c
      if(flag.eq.6) then
         do 10 i=1,nu
            ww=u(i)
            if(ww.ne.0.0d0) then
               y(i)=1.0d0/ww
            endif
 10         continue
      endif
c
      if(flag.eq.1) then
         do 15 i=1,nu
            ww=u(i)
            if(ww.ne.0.0d0) then
               y(i)=1.0d0/ww
            else
               flag=-2
               return
            endif
 15      continue
      endif
      end
