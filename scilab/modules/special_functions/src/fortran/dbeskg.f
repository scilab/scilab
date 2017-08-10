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
      subroutine dbeskg (x1, alpha, kode, n, y, nz,ierr)
c     Author Serge Steer, 2005
c     extends dbesk for the case where alpha is negative
c     x is supposed to be positive (besselk,with x<0 is complex)
      double precision x1,alpha,y(n)
      integer kode,n,nz,ierr
c
      double precision inf,x,dlamch,a1,temp

      inf=dlamch('o')*2.0d0
      x=x1
      ierr=0
      if (x.ne.x.or.alpha.ne.alpha) then
c     .  NaN case
         call dset(n,inf-inf,y,1)
         ierr=4
      elseif (x .eq. 0.0d0) then
         call dset(n,-inf,y,1)
         ierr=2
      elseif (alpha.ge.0.0d0) then
         call dbesk(x,alpha,kode,n,y,nz,ierr)
         if (ierr.eq.2) call dset(n,inf,y,1)
      else
         if(alpha-1.0d0+n.ge.0.0d0) then 
c     .     0 is between alpha and alpha+n
            nn=int(-alpha)+1
         else
            nn=n
         endif
         a1=-(alpha-1.0d0+nn)
         call dbesk(x,a1,kode,nn,y,nz,ierr)
         if (ierr.eq.2) call dset(nn,inf,y,1)
c     .  swap the result to have it in correct order
         if (nn.ge.2) then
            do i=1,nn/2
               temp=y(i)
               y(i)=y(nn+1-i)
               y(nn+1-i)=temp
            enddo
         endif
         if (n.gt.nn) then
            call dbesk(x,1.0d0-a1,kode,n-nn,y(nn+1),nz,ier)
            if (ier.eq.2) call dset(n-nn,inf,y(nn+1),1)
            ierr=max(ierr,ier)
         endif
      endif
      end

      subroutine dbeskv (x,nx,alpha,na, kode,y,w,ierr)
c     Author Serge Steer, Copyright INRIA, 2005
c     compute besseli function for x and alpha given by vectors
c     w : working array of size 2*na (used only if nz>0 and alpha contains negative
C         values
      double precision x(nx),alpha(na),y(*),w(*)
      integer kode,nx,na,ier
      double precision dlamch,eps
      eps=dlamch('p')
      ierr=0
      if (na.lt.0) then
c     .  element wise case x and alpha are supposed to have the same size
         do i=1,nx
            call  dbeskg (abs(x(i)), alpha(i),kode,1,y(i), nz,ier)
            ierr=max(ierr,ier)
         enddo
      elseif (na.eq.1) then
         do i=1,nx
            call  dbeskg (abs(x(i)), alpha(1),kode,1,y(i), nz,ier)
            ierr=max(ierr,ier)
         enddo
      else
c     .  compute besseli(x(i),y(j)), i=1,nx,j=1,na
         j0=1
 05      n=0
 10      n=n+1
         j=j0+n
         if (j.le.na.and.abs((1+alpha(j-1))-alpha(j)).le.eps) then
            goto 10
         endif
         do i=1,nx
            call dbeskg(abs(x(i)),alpha(j0),kode,n, w, nz,ier)
            ierr=max(ierr,ier)
            call dcopy(n,w,1,y(i+(j0-1)*nx),nx)
         enddo
         j0=j
         if (j0.le.na) goto 05
      endif
      end


