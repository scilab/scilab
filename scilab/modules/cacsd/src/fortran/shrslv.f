      subroutine shrslv(a,b,c,m,n,na,nb,nc,eps,cond,rmax,fail)
c
c!purpose
c   shrslv is a fortran iv subroutine to solve the real matrix
c   equation ax + xb = c, where a is in lower real schur form
c   and b is in upper real schur form,
c
c!calling sequence
c
c       subroutine shrslv(a,b,c,m,n,na,nb,nc,eps,cond,rmax,fail)
c   a      a doubly subscripted array containing the matrix a in
c          lower schur form
c
c   b      a doubly subscripted array containing tbe matrix b
c          in upper real schur form
c
c   c      a doubly subscripted array containing the matrix c.
c
c   m      the order of the matrix a
c
c   n      the order of the matrix b
c
c   na     the first dimension of the array a
c
c   nb     the first dimension of the array b
c
c   nc     the first dimension of the array c
c
c   eps    tolerance on a(k,k)+b(l,l)
c          if |a(k,k)+b(l,l)|<eps algorithm suppose that |a(k,k)+b(l,l)|=eps
c
c   cond    minimum allowed conditionnement for linear systems
c          if cond .le. 0 no estimation of conditionnement is done
c
c   rmax   maximum allowed size of any element of the transformation
c
c   fail   indicates if shrslv failed
c
c!auxiliary routines
c     ddot (blas)
c     dgeco dgefa dgesl (linpack)
c     dbas sqrt (fortran)
c!originator
c      Bartels and Stewart
c!
c
      integer m, n, na, nb, nc
      double precision a, b, c, rmax
      dimension a(na,m), b(nb,n), c(nc,n)
      logical fail
c internal variables
c
      integer k,km1,dk,kk,l,lm1,dl,ll,i,j,nsys,ipvt(4),info
      double precision t,p,zero,rcond,cond,const,z,ddot,eps
      dimension t(4,4),p(4),z(4)
      data zero /0.0d+0/
      if(cond.gt.zero) const = sqrt(sqrt(cond))
c
      info = 0
      fail = .true.
      l = 1
   10 lm1 = l - 1
      dl = 1
      if (l.eq.n) go to 20
      if (b(l+1,l).ne.zero) dl = 2
   20 ll = l + dl - 1
      if (l.eq.1) go to 60
      do 50 j=l,ll
         do 40 i=1,m
         c(i,j)=c(i,j)-ddot(lm1,c(i,1),nc,b(1,j),1)
   40    continue
   50 continue
   60 k = 1
   70 km1 = k - 1
      dk = 1
      if (k.eq.m) go to 80
      if (a(k,k+1).ne.zero) dk = 2
   80 kk = k + dk - 1
      if (k.eq.1) go to 120
      do 110 i=k,kk
         do 100 j=l,ll
         c(i,j) = c(i,j) - ddot(km1,a(i,1),na,c(1,j),1)
  100    continue
  110 continue
  120 continue
c      write(6,'(''dl='',i1,'' dk='',i1)') dl,dk
      if (dl.eq.2) go to 160
      if (dk.eq.2) go to 130
      t(1,1) = a(k,k) + b(l,l)
c      write(6,'(e10.3,3x,e10.3)') t(1,1),c(k,l)
      if (abs(t(1,1)).lt.eps) t(1,1)=sign(eps,t(1,1))
      c(k,l) = c(k,l)/t(1,1)
c      write(6,'(''c='',e10.3,'' rmax='',e10.3)') c(k,l),rmax

      if (abs(c(k,l)).ge.rmax) return
      go to 220
  130 t(1,1) = a(k,k) + b(l,l)
      t(1,2) = a(k,kk)
      t(2,1) = a(kk,k)
      t(2,2) = a(kk,kk) + b(l,l)
      p(1) = c(k,l)
      p(2) = c(kk,l)
c      write(6,'(e10.3,3x,e10.3,3x,e10.3)') t(1,1),t(1,2),p(1)
c      write(6,'(e10.3,3x,e10.3,3x,e10.3)') t(2,1),t(2,2),p(2)
      nsys = 2
      if (cond.gt.zero) go to 140
      call dgefa(t, 4, nsys, ipvt, info)
      if (info.gt.0) return
      go to 150
  140 continue
      call dgeco(t, 4, nsys, ipvt, rcond, z)
      if (rcond.lt.const) return
  150 continue
      call dgesl(t, 4, nsys, ipvt, p, 0)
c      write(6,'(''c='',e10.3,'' rmax='',e10.3)') c(k,l),rmax
c      write(6,'(''c='',e10.3,'' rmax='',e10.3)') c(kk,l),rmax
      c(k,l) = p(1)
      if (abs(c(k,l)).ge.rmax) return
      c(kk,l) = p(2)
      if (abs(c(kk,l)).ge.rmax) return
      go to 220
  160 if (dk.eq.2) go to 190
      t(1,1) = a(k,k) + b(l,l)
      t(1,2) = b(ll,l)
      t(2,1) = b(l,ll)
      t(2,2) = a(k,k) + b(ll,ll)
      p(1) = c(k,l)
      p(2) = c(k,ll)
c      write(6,'(e10.3,3x,e10.3,3x,e10.3)') t(1,1),t(1,2),p(1)
c      write(6,'(e10.3,3x,e10.3,3x,e10.3)') t(2,1),t(2,2),p(2)
      nsys = 2
      if (cond.gt.zero) go to 170
      call dgefa(t, 4, nsys, ipvt, info)
      if (info.gt.0) return
      go to 180
  170 continue
      call dgeco(t, 4, nsys, ipvt, rcond, z)
      if (rcond.lt.const) return
  180 continue
      call dgesl(t, 4, nsys, ipvt, p, 0)
c      write(6,'(''c='',e10.3,'' rmax='',e10.3)') c(k,l),rmax
c      write(6,'(''c='',e10.3,'' rmax='',e10.3)') c(kk,l),rmax
      c(k,l) = p(1)
      if (abs(c(k,l)).ge.rmax) return
      c(k,ll) = p(2)
      if (abs(c(k,ll)).ge.rmax) return
      go to 220
  190 t(1,1) = a(k,k) + b(l,l)
      t(1,2) = a(k,kk)
      t(1,3) = b(ll,l)
      t(1,4) = zero
      t(2,1) = a(kk,k)
      t(2,2) = a(kk,kk) + b(l,l)
      t(2,3) = zero
      t(2,4) = t(1,3)
      t(3,1) = b(l,ll)
      t(3,2) = zero
      t(3,3) = a(k,k) + b(ll,ll)
      t(3,4) = t(1,2)
      t(4,1) = zero
      t(4,2) = t(3,1)
      t(4,3) = t(2,1)
      t(4,4) = a(kk,kk) + b(ll,ll)
      p(1) = c(k,l)
      p(2) = c(kk,l)
      p(3) = c(k,ll)
      p(4) = c(kk,ll)
      do 191 j=1,4
c      write(6,'(5(e10.3,3x))') (t(j,i),i=1,4),p(j)
 191  continue
      nsys = 4
      if (cond.gt.zero) go to 200
      call dgefa(t, 4, nsys, ipvt, info)
      if (info.gt.0) return
      go to 210
  200 continue
      call dgeco(t, 4, nsys, ipvt, rcond, z)
      if (rcond.lt.const) return
  210 continue
      call dgesl(t, 4, nsys, ipvt, p, 0)
c      write(6,'(''c='',e10.3,'' rmax='',e10.3)') c(k,l),rmax
c      write(6,'(''c='',e10.3,'' rmax='',e10.3)') c(kk,l),rmax
c      write(6,'(''c='',e10.3,'' rmax='',e10.3)') c(k,ll),rmax
c      write(6,'(''c='',e10.3,'' rmax='',e10.3)') c(kk,ll),rmax
      c(k,l) = p(1)
      if (abs(c(k,l)).ge.rmax) return
      c(kk,l) = p(2)
      if (abs(c(kk,l)).ge.rmax) return
      c(k,ll) = p(3)
      if (abs(c(k,ll)).ge.rmax) return
      c(kk,ll) = p(4)
      if (abs(c(kk,ll)).ge.rmax) return
  220 k = k + dk
      if (k.le.m) go to 70
      l = l + dl
      if (l.le.n) go to 10
      fail = .false.
      return
      end
