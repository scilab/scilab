      subroutine dollar(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Ouputs delayed input
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      do 5 j=1,nu
         if(flag.eq.1.or.flag.eq.6.or.flag.eq.4) then
            y(j)=z(j)
         elseif(flag.eq.2) then
            z(j)=u(j)
         endif
 5    continue
      end
