      subroutine bezstp(p1,n1,p2,n2,a,na,u,nu,l,x,v,w,best,ipb,errr)
c     Copyright INRIA
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
c     
      do 10 k=1,l
         call giv(a(k,n0+1-k),a(k+1,n0+1-k),c,s)
         call drot(n0,a(k,1),na,a(k+1,1),na,c,s)
         a(k+1,n0+1-k)=0.0d+0
         call drot(ll,u(k,1),nu,u(k+1,1),nu,c,s)
         if(k.eq.1.and.l.lt.n0) then
            call dcopy(n0-1,a(2,1),na,x,na)
            call dcopy(ll,u(2,1),nu,v,nu)
         endif
 10   continue
c     
      call dcopy(ll,u(l,1),nu,w(iuv),1)
      call dcopy(ll,u(l+1,1),nu,w(ixy),1)

c     
      if(l.le.abs(n1-n2)) goto 99
      fact=a(l,n0-l+1)
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
c     normalisation pour que le terme de plus haut degre du pgcd soit 1 et
c       que le determinant soit 1
c
      if (fact.eq.0.0d+0) goto 99
      call dscal(ll,1.0d+0/fact,w(iuv),1)
      dt0=w(ixy+2*(l-1))*w(iuv+2*l-1)-w(ixy+2*l-1)*w(iuv+2*(l-1))
      if(dt0.eq.0.0d+0) goto 99
      call dscal(ll,1.0d+0/dt0,w(ixy),1)
      dt0=1
c     
c     estimation de l'erreur directe
c
c     p1*x
      call dcopy(l-m1,w(ixy+2*m1),2,w(iw1),-1)
      call dpmul1(p1,n1,w(iw1),l-1-m1,w(iw))
      nw=n1+l-1-m1
c     p1*x+p2*y
      call dcopy(l-m2,w(ixy+1+2*m2),2,w(iw1),-1)
      call dpmul(p2,n2,w(iw1),l-1-m2,w(iw),nw)
      errd=ddot(nw+1,w(iw),1,w(iw),1)
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
c     estimation de l'erreur inverse
c     ------------------------------
c     y
      call dcopy(n1-np+1,w(ixy+1+2*m2),2,w(iw),-1)
c     p*y+p1
      call dpmul1(w(iw1),np,w(iw),n1-np,w(iw))
      call dadd(n1+1,p1,1,w(iw),1)
      erri=ddot(n1+1,w(iw),1,w(iw),1)
c     x
      call dcopy(n2-np+1,w(ixy+2*m1),2,w(iw),-1)
c     p*x
      call dpmul1(w(iw1),np,w(iw),n2-np,w(iw))
c     p*x-p2
      call ddif(n2+1,p2,1,w(iw),1)
      erri=erri+ddot(n2+1,w(iw),1,w(iw),1)

c       write(6,*) np,errd,erri
c     
      if(max(erri,errd).lt.errr) then
         errr=max(erri,errd)
         nb=max(0,n0-l)
         ipb(1)=1
c     pgcd
         call dcopy(nb+1,a(l,1),na,best(ipb(1)),1)
         if(l.gt.1) then
            call daxpy(nb+1,z,a(l+1,1),na,best(ipb(1)),1)
         endif
         call dscal(nb+1,1.0d+0/fact,best(ipb(1)),1)
         ipb(2)=ipb(1)+nb+1
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
 99   return
      end
