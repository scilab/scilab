c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine bezstp(p1,n1,p2,n2,a,na,u,nu,l,x,v,w,best,ipb,errr)
      double precision a(na,*),u(nu,*),x(na,*),v(nu,*),p1(*),p2(*)
      integer ipb(6)
      double precision w(*),best(*),errr
      double precision dt0,z,fact
      double precision c,s,errd,mm,eps,erri
      double precision dlamch,ddot
c
      eps=dlamch('p')
      n0=max(n1,n2)+1
      m1=max(n1-n2,0)
      m2=max(n2-n1,0)
      ll=2*l
      iuv=1
      ixy=iuv+ll
      iw1=ixy+ll
      iw=iw1+n0
      ifree=iw+2*n0
c     The matrix A is an upper triangular matrix catenated with a first full row
c     [A(1,1) ... A(1,n0-1) A(1,n0)
c      A(2,1) ... A(2,n0-1) A(2,n0)
c      A(3,1) ... A(3,n0-1)    0
c      A(4,1) ...    0         0
c      ...    ...   ...      ...   ]
c      
c     make it upper triangular by a sequence of givens rotations
      do 10 k=1,l
c     .  compute rotation that zeros a(k+1,n0+1-k)
         call giv(a(k,n0+1-k),a(k+1,n0+1-k),c,s)
c     .  apply it to the rows k and k+1 of a
         call drot(n0,a(k,1),na,a(k+1,1),na,c,s)
         a(k+1,n0+1-k)=0.0d+0
c     .  accumulate transformation in u
         call drot(ll,u(k,1),nu,u(k+1,1),nu,c,s)
         if(k.eq.1.and.l.lt.n0) then
c     .     store second row of a in x (that is right part of row 0 of a)
c     .     A=[[0 x]
c     .          At ]
            call dcopy(n0-1,a(2,1),na,x,na)
c     .     store second row of u in v (that is right part of row 0 of u)
            call dcopy(ll,u(2,1),nu,v,nu)
         endif
 10   continue
c
      call dcopy(ll,u(l,1),nu,w(iuv),1)
      call dcopy(ll,u(l+1,1),nu,w(ixy),1)

c
      if(l.le.abs(n1-n2)) then
c     .  The dimension of the image of the linear application is too
c     .  small, skip the errors computations
c     .  increase the space dimension
         goto 99
      endif
c     Get the highest degree coefficient of the CGD candidate
      fact=a(l,n0-l+1)

c     decrease the [u,v] degree using a linear combinaison with [x y]
      if(l.gt.1) then
         mm=w(ixy+2*m1)**2+w(ixy+1+2*m2)**2
         z=w(iuv+2*m1)*w(ixy+2*m1)+w(iuv+1+2*m2)*w(ixy+1+2*m2)
      else
         mm=w(ixy+2*m1)**2
         z=w(iuv+2*m1)*w(ixy+2*m1)
      endif
      if (mm.ne.0.0d+0) then
c     on abaisse le degre de [u,v]
         z=-z/mm
         call daxpy(ll,z,w(ixy),1,w(iuv),1)
      endif
c
      if (fact.eq.0.0d+0) then
c     .  the highest degree coefficient of the GCD candidate is zero. It is
c     .  possible to find a lower degree one, increase the space dimension
         goto 99
      endif
c     normalize the u and v componants of the unimodular matrix  to make
c     CGD candidate higher degree coefficient equal to 1
      call dscal(ll,1.0d+0/fact,w(iuv),1)
c     compute the determinant of the unimodular matrix (the determinant
c     of its degree 0 coefficient
      dt0=w(ixy+2*(l-1))*w(iuv+2*l-1)-w(ixy+2*l-1)*w(iuv+2*(l-1))
      if(dt0.eq.0.0d+0) then
c     .  the determinant of the unimodular matrix is 0,
c     .  increase the space dimension
         goto 99
      endif
c     normalize the x and y components of the unimodular matrix to make
c     it's determinant equal to 1
      call dscal(ll,1.0d+0/dt0,w(ixy),1)
      dt0=1.0d0
c
c     Estimate the forward error:
c     first compute ||p1*x-p2*y||
c     p1*x
      call dcopy(l-m1,w(ixy+2*m1),2,w(iw1),-1)
      call dpmul1(p1,n1,w(iw1),l-1-m1,w(iw))
      nw=n1+l-1-m1
c     p1*x+p2*y
      call dcopy(l-m2,w(ixy+1+2*m2),2,w(iw1),-1)
      call dpmul(p2,n2,w(iw1),l-1-m2,w(iw),nw)
      errd=ddot(nw+1,w(iw),1,w(iw),1)
c     now compute ||p1*u-p2*v-p||
c     p1*u
      if(l-1-m1.gt.0) then
         call dcopy(l-1-m1,w(iuv+2+2*m1),2,w(iw1),-1)
         call dpmul1(p1,n1,w(iw1),l-2-m1,w(iw))
         nw=n1+l-2-m1
      else
         call dpmul1(p1,n1,w(iuv+2*m1),0,w(iw))
         nw=n1
      endif
c     p1*u+p2*v
      if(l-1-m2.gt.0) then
         call dcopy(l-1-m2,w(iuv+3+2*m2),2,w(iw1),-1)
         call dpmul(p2,n2,w(iw1),l-2-m2,w(iw),nw)
      else
         call dpmul(p2,n2,w(iuv+1+2*m2),0,w(iw),nw)
      endif
c     p
      np=n0-l
      call dcopy(np+1,a(l,1),na,w(iw1),1)
      call daxpy(np,z,a(l+1,1),na,w(iw1),1)
      call dscal(np+1,1.0d+0/fact,w(iw1),1)
c     p1*u+p2*v-p
      call ddif(np+1,w(iw1),1,w(iw),1)
      errd=errd+ddot(nw+1,w(iw),1,w(iw),1)
c
c     Estimate the backward error
c     ------------------------------
c     first ||p*y+p1||
c     y
      call dcopy(n1-np+1,w(ixy+1+2*m2),2,w(iw),-1)
c     p*y+p1
      call dpmul1(w(iw1),np,w(iw),n1-np,w(iw))
      call dadd(n1+1,p1,1,w(iw),1)
      erri=ddot(n1+1,w(iw),1,w(iw),1)
c     now ||p*x-p2||
c     x
      call dcopy(n2-np+1,w(ixy+2*m1),2,w(iw),-1)
c     p*x
      call dpmul1(w(iw1),np,w(iw),n2-np,w(iw))
c     p*x-p2
      call ddif(n2+1,p2,1,w(iw),1)
      erri=erri+ddot(n2+1,w(iw),1,w(iw),1)

      if(max(erri,errd).lt.errr) then
c     .  A better solution found
         errr=max(erri,errd)
         nb=max(0,n0-l)
c     .  preserve the gcd and unimodular matrix candidates
c        best contains [gcd u,v,x,y],
c        the ipb array give info to split best.
         ipb(1)=1
c     .  store gcd candidate into best
         call dcopy(nb+1,a(l,1),na,best(ipb(1)),1)
         if(l.gt.1) then
            call daxpy(nb+1,z,a(l+1,1),na,best(ipb(1)),1)
         endif
         call dscal(nb+1,1.0d+0/fact,best(ipb(1)),1)
         ipb(2)=ipb(1)+nb+1

c     .  store the unimodular matrix candidate into best
         if(l.gt.1) then
            nn=max(n2-nb,1)
            call dcopy(nn,w(iuv+2*(l-nn)),2,best(ipb(2)),-1)
            ipb(3)=ipb(2)+nn
            nn=max(n1-nb,1)
            call dcopy(nn,w(iuv+1+2*(l-nn)),2,best(ipb(3)),-1)
            ipb(4)=ipb(3)+nn
         else
            best(ipb(2))=w(iuv)
            ipb(3)=ipb(2)+1
            best(ipb(3))=w(iuv+1)
            ipb(4)=ipb(3)+1
         endif
         nn=n2+1-nb
         call dcopy(nn,w(ixy+2*(l-nn)),2,best(ipb(4)),-1)
         ipb(5)=ipb(4)+nn
         nn=n1+1-nb
         call dcopy(nn,w(ixy+1+2*(l-nn)),2,best(ipb(5)),-1)
         ipb(6)=ipb(5)+nn
      endif
c
 99   continue
c     increase the dimension of the space
      return
      end
