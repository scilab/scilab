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
      subroutine dbesjg (x1, alpha, n, y, nz,w,ierr)
c     Author Serge Steer, 2005
c     extends dbesj for the case where alpha is negative
c     if alpha is negative and non integer and x1 is negative:error
      double precision x1,alpha,y(n),w(n)
      integer n,nz,ierr
c
      double precision a,b,pi,inf,x,a1
      integer ier2
      double precision dlamch
      data pi /3.14159265358979324D0/

      inf=dlamch('o')*2.0d0
      x=x1
      ierr=0
      ier2=0
      if (x.ne.x.or.alpha.ne.alpha) then
c     .  NaN case
         call dset(n,inf-inf,y,1)
         ierr=4
      elseif (alpha .ge. 0.0d0) then
c     .  alpha <0 and x>=0 or x<0
         call dbesj(abs(x),alpha,n,y,nz,ierr)
         if (ierr.eq.2) call dset(n,inf,y,1)
         if(x.lt.0.0d0) then
            i0=mod(int(alpha)+1,2)
            call dscal((n-i0+1)/2,-1.0d0,y(1+i0),2)
         endif
      else if (alpha .eq. dint(alpha)) then
c     .  alpha <0 and integer,   CHECKED
c     .  transform to positive value of alpha         
         if(alpha-1+n.ge.0) then 
c     .     0 is between alpha and alpha+n
            a1=0.0d0
            nn=min(n,int(-alpha))
         else
            a1=-(alpha-1+n)
            nn=n
         endif
         call dbesj(abs(x),a1,n,w,nz,ierr)
         if (ierr.eq.2) then
            call dset(n,inf,y,1)
         else
           if(n.gt.nn) then
c     .        0 is between alpha and alpha+n
               call dcopy(n-nn,w,1,y(nn+1),1)
               call dcopy(nn,w(2),-1,y,1)
            else
c     .        alpha and alpha+n are negative
               call dcopy(nn,w,-1,y,1)
            endif
         endif
c     .  apply parity
         i0=mod(int(abs(alpha))+1,2)
         if(x.gt.0.0d0) then
            call dscal((nn-i0+1)/2,-1.0d0,y(1+i0),2)
         else
            call dscal((n-nn)/2,-1.0d0,y(nn+2),2)
         endif
      else if (x .eq. 0.0d0) then
c     .  alpha <0 and x==0 CHECKED
         if(alpha-1.0d0+n.ge.0.0d0) then 
c     .     0 is between alpha and alpha+n
            nn=int(-alpha)+1
         else
            nn=n
         endif
         ierr=2
         call dset(nn,-inf,y,1)
         if (n.gt.nn) call dset(n-nn,0.0d0,y(nn+1),1)
      else
c     .  first alpha is negative non integer, x should be positive (with
C     .  x negative the result is complex. CHECKED
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
         call dbesj(x,a1,nn,y,nz1,ierr)
         call dbesy(x,a1,nn,w,ier)
         ierr=max(ierr,ier) 
         if (ierr.eq.0) then
            a=cos(a1*pi)
            b=-sin(a1*pi)
c     .     to avoid numerical errors if a is near 0 (or b near 0)  
c     .     and y is big (or w is big)
            if(abs(abs(a)-1.0d0).lt.eps)  b=0.0d0
            if(abs(abs(b)-1.0d0).lt.eps)  a=0.0d0
            call dscal(nn,b,w,1)
            call daxpy(nn,a,y,1,w,1)
         elseif (ierr.eq.2) then
            call dset(nn,inf,w,1)
         elseif (ierr.eq.4) then
            call dset(nn,inf-inf,w,1)
         endif
c     .  change sign to take into account that cos((a1+k)*pi) and
C     .  sin((a1+k)*pi) changes sign with k
        if (nn.ge.2) call dscal(nn/2,-1.0d0,w(2),2)

c     .  store the result in the correct order
         call dcopy(nn,w,-1,y,1)
      
c     .  compute for positive value of alpha+k is any (note that x>0)
         if (n.gt.nn) then
c     .     this code is taken from the alpha>0 case above
            call dbesj(abs(x),1.0d0-a1,n-nn,y(nn+1),nz1,ier)
            if (ier.eq.2) call dset(n-nn,inf,y(nn+1),1)
            ierr=max(ierr,ier)
         endif
      endif
      end


      subroutine dbesjv (x,nx,alpha,na,kode,y,w,ierr)
c     Author Serge Steer, Copyright INRIA, 2005
c     compute besselj function for x and alpha given by vectors
c     w : working array of size 2*na (used only if nz>0 and alpha contains negative
C         values
      double precision x(nx),alpha(na),y(*),w(*)
      integer kode,nx,na,ier
      double precision dlamch,w1,eps
      eps=dlamch('p')
      ierr=0
      if (na.lt.0) then
c     .  element wise case x and alpha are supposed to have the same size
         do i=1,nx
            call  dbesjg (x(i), alpha(i),1,y(i), nz, w1,ier)
            ierr=max(ierr,ier)
         enddo
      elseif (na.eq.1) then
c     .  element wise case x and alpha are supposed to have the same size
         do i=1,nx
            call  dbesjg (x(i), alpha(1),1,y(i), nz, w1,ier)
            ierr=max(ierr,ier)
         enddo
      else
c     .  compute besselj(x(i),y(j)), i=1,nx,j=1,na
         j0=1
 05      n=0
 10      n=n+1
         j=j0+n
         if (j.le.na.and.abs((1+alpha(j-1))-alpha(j)).le.eps) then
            goto 10
         endif
         do i=1,nx
            call dbesjg(x(i),alpha(j0),n, w, nz, w(na+1),ier)
            ierr=max(ierr,ier)
            call dcopy(n,w,1,y(i+(j0-1)*nx),nx)
         enddo
         j0=j
         if (j0.le.na) goto 05
      endif
      end


