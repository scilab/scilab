c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2005 - INRIA - Serge STEER
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine zbeskg (x1r,x1i, alpha, kode, n, yr,yi, nz,ierr)
c     Author Serge Steer, 2005
c     extends cbesk for the case where alpha is negative
      double precision alpha
      double precision x1r, x1i, yr(n), yi(n)
      integer kode,n,nz,ierr
c
      double precision inf,xr, xi,a1,temp
      double precision dlamch
      inf=dlamch('o')*2.0d0   

      xr=x1r
      xi=x1i
      ierr=0
      if (xr.ne.xr.or.xi.ne.xi.or.alpha.ne.alpha) then
c     .  NaN case
         call dset(n,inf-inf,yr,1)
         call dset(n,inf-inf,yi,1)
         ierr=4
      elseif (alpha.ge.0.0d0) then
         call zbesk(xr,xi,abs(alpha),kode,n,yr,yi,nz,ierr)
         if (ierr.eq.2) then
            call dset(n,inf,yr,1)
            call dset(n,inf,yi,1)
         elseif(ierr.ge.4) then
            call dset(n,inf-inf,yr,1)
            call dset(n,inf-inf,yi,1)
         endif
      else
         if(alpha-1.0d0+n.ge.0.0d0) then 
c     .     0 is between alpha and alpha+n
            nn=int(-alpha)+1
         else
            nn=n
         endif
         a1=-(alpha-1.0d0+nn)
         call zbesk(xr,xi,a1,kode,nn,yr,yi,nz,ierr)
         if (ierr.eq.0) then
c     .      swap the result to have it in correct order
            if (nn.ge.2) then
               do i=1,nn/2
                  temp=yr(i)
                  yr(i)=yr(nn+1-i)
                  yr(nn+1-i)=temp
                  temp=yi(i)
                  yi(i)=yi(nn+1-i)
                  yi(nn+1-i)=temp
               enddo
            endif
         elseif(ierr.eq.2) then
            call dset(n,inf,yr,1)
            call dset(n,inf,yi,1)
         elseif(ierr.ge.4) then
            call dset(n,inf-inf,yr,1)
            call dset(n,inf-inf,yi,1)
         endif
         if (n.gt.nn) then
            a1=1.0d0-a1
            call zbesk(xr,xi,a1,kode,n-nn,yr(nn+1),yi(nn+1),nz,ier)
            if (ier.eq.2) then
               call dset(n-nn,inf,yr(nn+1),1)
               call dset(n-nn,inf,yi(nn+1),1)
            elseif(ierr.ge.4) then
               call dset(n-nn,inf-inf,yr(nn+1),1)
               call dset(n-nn,inf-inf,yi(nn+1),1)
            endif
            ierr=max(ierr,ier)
         endif
      endif

      end

      subroutine zbeskv (xr,xi,nx,alpha,na, kode,yr,yi,wr,wi,ierr)
c     Author Serge Steer, Copyright INRIA, 2005
c     compute besseli function for x and alpha given by vectors
c     w : working array of size 2*na (used only if nz>0 and alpha contains negative
C         values
      double precision xr(nx),xi(nx),alpha(na),yr(*),yi(*),wr(*),wi(*)
      double precision dlamch,eps
      integer kode,nx,na,ier
      eps=dlamch('p')
      ierr=0
      if (na.lt.0) then
c     .  element wise case x and alpha are supposed to have the same size
         do i=1,nx
            call  zbeskg (xr(i), xi(i), alpha(i), kode, 1, yr(i),yi(i),
     $            nz,ier)
            ierr=max(ierr,ier)
         enddo
      elseif (na.eq.1) then
        do i=1,nx
           call  zbeskg (xr(i), xi(i), alpha(1), kode, 1, yr(i),yi(i),
     $          nz,ier)
           ierr=max(ierr,ier)
        enddo
      else
c     .  compute besseli(x(i),y(j)), i=1,nx,j=1,na
         j0=1
 05      n=0
 10      n=n+1
         j=j0+n
         if (j.le.na.and.abs((1.0d0+alpha(j-1))-alpha(j)).le.eps) then
            goto 10
         endif

         do i=1,nx
            call zbeskg (xr(i),xi(i),alpha(j0),kode, n, wr, wi, nz,ier)
            ierr=max(ierr,ier)
            call dcopy(n,wr,1,yr(i+(j0-1)*nx),nx)
            call dcopy(n,wi,1,yi(i+(j0-1)*nx),nx)
         enddo
         j0=j
         if (j0.le.na) goto 05
      endif
      end


