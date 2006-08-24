      subroutine zbeshg (x1r,x1i,alpha, kode, k,n, yr,yi, nz,wr,wi,ierr)
c     Author Serge Steer, Copyright INRIA, 2005
c     extends cbesi for the case where alpha is negative
      double precision alpha
      double precision x1r, x1i, yr(n), yi(n), wr(n),wi(n)
      integer kode,n,nz,ierr
c     
      double precision a,b,pi,inf,xr, xi,a1
      integer ier1,ier2
      double precision dlamch

      data pi /3.14159265358979324D0/
      inf=dlamch('o')*2.0d0   

      xr=x1r
      xi=x1i
      ier2=0
      if (xr.ne.xr.or.xi.ne.xi.or.alpha.ne.alpha) then
c     .  NaN case
         call dset(n,inf-inf,yr,1)
         call dset(n,inf-inf,yi,1)
         ierr=4
      elseif (alpha .ge. 0.0d0) then
         call zbesh(xr,xi,alpha,kode,k, n,yr,yi,nz,ierr)
         if(ierr.eq.1.or.ierr.eq.2.or.ierr.ge.4) then
            call dset(n,inf-inf,yr,1)
            call dset(n,inf-inf,yi,1)
         endif
      else if (alpha .eq. dint(alpha)) then
c     .  alpha <0 and integer,  
c     .  transform to positive value of alpha         
         if(alpha-1+n.ge.0) then 
c     .     0 is between alpha and alpha+n
            a1=0.0d0
            nn=min(n,int(-alpha))
         else
            a1=-(alpha-1+n)
            nn=n
         endif
         call zbesh(xr,xi,a1,kode,k,n,wr,wi,nz,ierr)
         if (ierr.eq.1.or.ierr.eq.2.or.ierr.ge.4) then
            call dset(n,inf-inf,yr,1)
            call dset(n,inf-inf,yi,1)
         else
            if(n.gt.nn) then
c     .        0 is between alpha and alpha+n
               call dcopy(n-nn,wr,1,yr(nn+1),1)
               call dcopy(n-nn,wi,1,yi(nn+1),1)
               call dcopy(nn,wr(2),-1,yr,1)
               call dcopy(nn,wi(2),-1,yi,1)
            else
c     .        alpha and alpha+n are negative
               call dcopy(n,wr,-1,yr,1)
               call dcopy(n,wi,-1,yi,1)
            endif
         endif
         i0=mod(int(abs(alpha))+1,2)
         call dscal((nn-i0+1)/2,-1.0d0,yr(1+i0),2)
         call dscal((nn-i0+1)/2,-1.0d0,yi(1+i0),2)
      else
c     .  first alpha is negative non integer,
c     .  transform to positive value of alpha       
         if(alpha-1.0d0+n.ge.0.0d0) then 
c     .     0 is between alpha and alpha+n
            nn=int(-alpha)+1
         else
            nn=n
         endif
c     .  compute for negative value of alpha+k, transform problem for
c     .  a1:a1+(nn-1) with a1 positive  a1+k =abs(alpha+nn-k)
         a1=-(alpha-1.0d0+nn)
         call zbesh(xr,xi,a1,kode,k,n,wr,wi,nz1,ierr)
         nz=max(nz1,nz2)
         if (ierr.eq.0) then
            a=cos(a1*pi)
            b=sin(a1*pi)
            if (k.eq.1) then
               call wscal(nn,a,b,wr,wi,1)
            else
               call wscal(nn,a,-b,wr,wi,1)
            endif
c     .     change sign to take into account that sin((a1+k)*pi) and cos((a1+k)*pi)
C     .     changes sign with k
            if (nn.ge.2) then
               call dscal(nn/2,-1.0d0,wr(2),2)
               call dscal(nn/2,-1.0d0,wi(2),2)
            endif
         elseif(ierr.eq.1.or.ierr.eq.2.or.ierr.ge.4) then
            call dset(nn,inf-inf,wr,1)
            call dset(nn,inf-inf,wi,1)
         endif
c     .  store the result in the correct order
         call dcopy(nn,wr,-1,yr,1)
         call dcopy(nn,wi,-1,yi,1)
c     .  compute for positive value of alpha+k is any 
         if (n.gt.nn) then
            a1=1.0d0-a1
            call zbesh(xr,xi,a1,kode,k,n-nn,yr(nn+1),yi(nn+1),nz,ier)
            if(ier.eq.1.or.ier.eq.2.or.ier.ge.4) then
               call dset(n-nn,inf-inf,yr(nn+1),1)
               call dset(n-nn,inf-inf,yi(nn+1),1)
            endif
            ierr=max(ierr,ier)
         endif
      endif
      end

      subroutine zbeshv (xr,xi,nx,alpha,na, kode,k,yr,yi,wr,wi,ierr)
c     Author Serge Steer, Copyright INRIA, 2005
c     compute besseli function for x and alpha given by vectors
c     w : working array of size 2*na (used only if nz>0 and alpha
C     contains negative
C     values
      double precision xr(nx),xi(nx),alpha(na),yr(*),yi(*),wr(*),wi(*)
      double precision e,dlamch,eps
      integer kode,nx,na,ier
      ierr=0
      eps=dlamch('p')
      if (na.lt.0) then
c     .  element wise case x and alpha are supposed to have the same
C     size
         do i=1,nx
            call  zbeshg (xr(i), xi(i), alpha(i), kode, k,1,yr(i),yi(i),
     $           nz,wr,wi,ier)
            ierr=max(ierr,ier)
         enddo
      elseif (na.eq.1) then
         do i=1,nx
            call  zbeshg (xr(i), xi(i), alpha(1), kode, k,1,yr(i),yi(i),
     $           nz,wr,wi,ier)
            ierr=max(ierr,ier)
         enddo
      else
c     .  compute besselh(x(i),y(j)), i=1,nx,j=1,na
         j0=1
 05      n=0
 10      n=n+1
         j=j0+n
         if (j.le.na.and.abs((1+alpha(j-1))-alpha(j)).le.eps) then
            goto 10
         endif
         do i=1,nx
            call zbeshg (xr(i), xi(i), alpha(j0) ,kode,k, n, wr, wi, nz,
     $           wr(na+1),wi(na+1),ier)
            ierr=max(ierr,ier)
            call dcopy(n,wr,1,yr(i+(j0-1)*nx),nx)
            call dcopy(n,wi,1,yi(i+(j0-1)*nx),nx)
         enddo
         j0=j
         if (j0.le.na) goto 05
      endif
      end


