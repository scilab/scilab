      subroutine sydsr(n,m,a,na,b,nb,c,nc,ierr)
c%But
c
c     this routine solves the discrete sylvester equation for the
c     case where the matrices  a and b  has been transformed to
c     quasi-triangular form.
c
c
c  warning   -this routine is intended to be called only from
c             slice  routine  sybad .

C  Copyright SLICOT
c%
      integer i,j,k,l,m,n,dk,dl,ii,ik,jj,kk,ll,l1,na,job,km1,
     x        ierr,info,ldim,nsys,ipvt(4)
      double precision a(na,n),c(nc,m),b(nb,m)
      double precision t(4,4),p(4)
      double precision ddot
c
      ierr = 0
      ldim = 4
      job = 0
      l = 1
   10    dl = 1
         if (l .eq. m) go to 20
         if (b(l+1,l) .ne. 0.0d+0) dl = 2
   20    ll = l + dl - 1
         k = 1
   30       km1 = k - 1
            dk = 1
            if (k .eq. n) go to 40
            if (a(k+1,k) .ne. 0.0d+0) dk = 2
   40       kk = k + dk - 1
            l1 = l - 1
            if (l1 .eq. 0) go to 70
            ii = 0
c
            do 50 i = k, kk
               ii = ii + 1
               jj = 0
c
                        do 50 j = l, ll
                                jj = jj + 1
                               t(ii,jj)=ddot(l1,c(i,1),nc,b(1,j),1)
   50       continue
c
        do 60 i = k, kk
                jj = 0
c
                do 60 j = l, ll
                        jj = jj + 1
                        c(i,j)=c(i,j)-ddot(kk-k+1,a(k,i),1,t(1,jj),1)
   60   continue
c
   70       if (km1 .eq. 0) go to 100
c
            do 90 j = l, ll
c
               do 90 i = 1, km1
c
                  do 90 ik = k, kk
                   c(ik,j)=c(ik,j)-a(i,ik)*ddot(ll,c(i,1),nc,b(1,j),1)
   90       continue
c
  100       if (dl .eq. 2) go to 120
            if (dk .eq. 2) go to 110
            t(1,1) = a(k,k) * b(l,l) - 1.0d+0
            if (t(1,1) .eq. 0.0d+0) go to 180
            c(k,l) = c(k,l) / t(1,1)
            go to 150
  110       t(1,1) = a(k,k) * b(l,l) - 1.0d+0
            t(1,2) = b(l,l) * a(kk,k)
            t(2,1) = b(l,l) * a(k,kk)
            t(2,2) = a(kk,kk) * b(l,l) - 1.0d+0
            p(1) = c(k,l)
            p(2) = c(kk,l)
            nsys = 2
            call dgefa(t,ldim,nsys,ipvt,info)
            if (info .ne. 0) go to 180
            call dgesl(t,ldim,nsys,ipvt,p,job)
            c(k,l) = p(1)
            c(kk,l) = p(2)
            go to 150
  120       if (dk .eq. 2) go to 140
            t(1,1) = b(l,l) * a(k,k) - 1.0d+0
            t(2,1) = b(l,ll) * a(k,k)
            t(1,2) = b(ll,l) * a(k,k)
            t(2,2) = b(ll,ll) * a(k,k) - 1.0d+0
            p(1) = c(k,l)
            p(2) = c(k,ll)
            nsys = 2
            call dgefa(t,ldim,nsys,ipvt,info)
            if (info .ne. 0) go to 180
            call dgesl(t,ldim,nsys,ipvt,p,job)
            c(k,l) = p(1)
            c(k,ll) = p(2)
            go to 150
  140       t(1,1) = b(l,l) * a(k,k) - 1.0d+0
            t(1,2) = b(l,l) * a(kk,k)
            t(1,3) = b(ll,l) * a(k,k)
            t(1,4) = b(ll,l) * a(kk,k)
            t(2,1) = b(l,l) * a(k,kk)
            t(2,2) = b(l,l) * a(kk,kk) - 1.0d+0
            t(2,3) = b(ll,l) * a(k,kk)
            t(2,4) = b(ll,l) * a(kk,kk)
            t(3,1) = b(l,ll) * a(k,k)
            t(3,2) = b(l,ll) * a(kk,k)
            t(3,3) = b(ll,ll) * a(k,k) - 1.0d+0
            t(3,4) = b(ll,ll) * a(kk,k)
            t(4,1) = b(l,ll) * a(k,kk)
            t(4,2) = b(l,ll) * a(kk,kk)
            t(4,3) = b(ll,ll) * a(k,kk)
            t(4,4) = b(ll,ll) * a(kk,kk) - 1.0d+0
            p(1) = c(k,l)
            p(2) = c(kk,l)
            p(3) = c(k,ll)
            p(4) = c(kk,ll)
            nsys = 4
            call dgefa(t,ldim,nsys,ipvt,info)
            if (info .ne. 0) go to 180
            call dgesl(t,ldim,nsys,ipvt,p,job)
            c(k,l) = p(1)
            c(kk,l) = p(2)
            c(k,ll) = p(3)
            c(kk,ll) = p(4)
c
  150    k = k + dk
         if (k .le. n) go to 30
      l = l + dl
      if (l .le. m) go to 10
      go to 190
  180 ierr = 1
  190 return
      end
