      subroutine logblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA


c     Scicos block simulator
c     y=log(u)/log(rpar(1))
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      if(flag.eq.1) then
         do 15 i=1,nu
            if(u(i).gt.0.0d0) then
               y(i)=log(u(i))/log(rpar(1))
            else
               flag=-2
               return
            endif
 15      continue
      endif
      if(flag.eq.6) then
         do 20 i=1,nu
            if(u(i).gt.0.0d0) then
               y(i)=log(u(i))/log(rpar(1))
            endif
 20      continue
      endif

      end
