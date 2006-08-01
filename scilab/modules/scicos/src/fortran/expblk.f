      subroutine expblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c     Outputs a^u(i)
c     a=rpar(1)
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      if(flag.eq.1) then
         do 15 i=1,nu
               y(i)=exp(log(rpar(1))*u(i))
 15      continue
      endif
      if(flag.ge.4) then
         do 20 i=1,nu
               y(i)=exp(log(rpar(1))*u(i))
 20      continue
      endif

      end
