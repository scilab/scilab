      subroutine fsv(flag,nevprt,t,xd,x,nx,z,nz,tvec,
     $        ntvec,rpar,nrpar,ipar,nipar
     $        ,u1,nu1,y1,ny1)
c
c
      double precision t,xd(*),x(*),z(*),tvec(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      double precision rpar(*),u1(*),y1(*)
c
c
      doubleprecision a,a0,b0,g
      a=u1(1)
      y1(1)=0.d0
      if(a.gt.1.0d0) then
         return
      endif
      g=1.4d0
      a0=2.0d0/g
      b0=(g+1)/g
      if(a.lt.0.528d0) then
         a=(2/(g+1.0d0))**(g/(g-1.0d0))  
      endif
      y1(1)=sqrt(2.0d0*g*(a**a0-a**b0)/(g-1.0d0))
   
 
c
c
      end
