      subroutine rndblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator
c
c     ipar(1) 
c            0 : uniform
c            1 : normal
c     rpar(1:ny)=mean
c     rpar(ny+1:2*ny)=deviation
c     rpar(2*ny+1)=dt 
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      double precision urand,sr,si,t1

      integer halt
      common /coshlt/ halt
c     
      if(flag.eq.1.or.flag.eq.6) then
      do 10 i=1,ny
         y(i)=rpar(i)+rpar(ny+i)*z(i+1)
 10   continue
      elseif(flag.eq.2.or.flag.eq.4) then
c     uniform
         if(ipar(1).eq.0) then
            iy=int(z(1))
            do 20 i=1,nz-1
               z(i+1)=urand(iy)
 20         continue
         else
            iy=int(z(1))
c     normal
            do 30 i=1,nz-1
 75            sr=2.0d+0*urand(iy)-1.0d+0
               si=2.0d+0*urand(iy)-1.0d+0
               t1 = sr*sr+si*si
               if (t1 .gt. 1.0d+0) go to 75
               z(i+1) = sr*sqrt(-2.0d+0*log(t1)/t1)
 30         continue
         endif
         z(1)=iy
c         if(ntvec.eq.1) tvec(1)=t+rpar(2*(nz-1)+1)
      endif
      return
      end
