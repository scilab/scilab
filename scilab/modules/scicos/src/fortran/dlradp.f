      subroutine dlradp(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,u,nu,y,ny)
c     Copyright INRIA

c     Scicos block simulator

c     SISO, strictly proper adapted transfer function
c
c     u(1)    : main input
c     u(2)    : modes adaptation input
c
c     m = ipar(1) : degree of numerator
c     n = ipar(2) : degree of denominator n>m
c     npt = ipar(3) : number of mesh points
c     x = rpar(1:npt) : mesh points abscissae
c     rnr = rpar(npt+1:npt+m*npt) : rnr(i,k) i=1:m  is the real part of
c          the roots of the numerator at the kth mesh point
c     rni = rpar(npt+m*npt+1:npt+2*m*npt) : rni(i,k) i=1:m  is the 
c          imaginary part of the roots of the numerator at the kth 
c          mesh point
c     rdr = rpar(npt+2*m*np+1:npt+(2*m+n)*npt) : rdr(i,k) i=1:n  
c          is the real part of the roots of the denominator at the kth 
c          meshpoint 
c     rdi = rpar(npt+(2*m+n)*np+1:npt+2*(m+n)*npt) : rdi(i,k) i=1:n  
c          is the imaginary part of the roots of the denominator at 
c          the kth  meshpoint 
c     g   = rpar(npt+2*(m+n)*npt+1:npt+2*(m+n)*npt+npt) is the
c           gain values at the mesh points.
c!
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*),u(*),y(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu,ny

c
      double precision yyp,ddot
      double precision yy(201),num(51),den(51),ww(51)


      m=ipar(1)
      n=ipar(2)
      if(flag.eq.2) then
c     state
         m=ipar(1)
         n=ipar(2)
         mpn=m+n
         npt=ipar(3)
         call intp(u(2),rpar(1),rpar(1+npt),2*mpn+1,npt,yy)
         call wprxc(m,yy(1),yy(1+m),num,ww)
         call wprxc(n,yy(1+2*m),yy(1+2*m+n),den,ww)
         yyp=-ddot(n,den,1,z(m+1),1)+(ddot(m,num,1,z(1),1)+u(1))*
     $        yy(1+2*mpn)
         if(m.gt.0) then
            call unsfdcopy(m-1,z(2),-1,z(1),-1)
            z(m)=u(1)
         endif
         call unsfdcopy(n-1,z(m+2),-1,z(m+1),-1)
         z(mpn)=yyp
      elseif(flag.eq.4) then
c     init
         m=ipar(1)
         n=ipar(2) 
         if(m.gt.50.or.n.gt.50) then
            flag=-1
            return
         endif
      endif
c     y
      y(1)=z(m+n)

      end
