      subroutine n1fc1(simul,prosca,n,xn,fn,g,dxmin,df1,epsf,zero,imp,
     &                 io,mode,iter,nsim,memax,iz,rz,dz,izs,rzs,dzs)
C          dimension iz=2*(memax+1)
C          dimension rz=5*n+(n+4)*memax
C          dimension dz=(memax+9)*memax+8
c     Copyright INRIA

      implicit double precision (a-h,o-z)
      external simul, prosca
      dimension iz(*), rz(*), dz(*), xn(n), g(n), izs(*), dzs(*)
      real rzs(*)
      dimension i5(1), d3(1), d4(1)
C
      if (n.gt.0 .and. df1.gt.0.d0 .and. epsf.ge.0.d0 .and. zero.ge.0.d0
     &    .and. iter.ge.0 .and. nsim.ge.0 .and. memax.ge.1 .and.
     &    dxmin.gt.0.d0) goto 10
      mode = 2
C     appel incoherent
      call n1fc1o(io,1,i1,i2,i3,i4,i5,d1,d2,d3,d4)
      goto 999
 10   ns = 1
      ngd = ns + n
      nx = ngd + n
      nsa = nx + n
      ngg = nsa + n
      nal = ngg + n
      naps = nal + memax
      nanc = naps + memax
      npoids = nanc + memax
      nq = npoids + memax
      njc = 1
      nic = njc + memax + 1
      nr = 1
      na = nr + (memax+1)*(memax+1)
      ne = na + memax + 1
      nrr = ne + memax + 1
      nxga = nrr + memax + 1
      ny = nxga + memax + 1
      nw1 = ny + memax + 1
      nw2 = nw1 + memax + 1
C
      niz = 2 * (memax+1)
      nrz = nq + n*memax - 1
      ndz = nw2 + memax
      if (imp .gt. 0) call n1fc1o(io,2,n,memax,niz,nrz,ndz,d1,d2,d3,d4)
      do 110 i = 1,niz
 110  iz(i) = 0
      do 120 i = 1,nrz
 120  rz(i) = 0.d0
      do 130 i = 1,ndz
 130  dz(i) = 0.d0
      call n1fc1a(simul,prosca,n,mode,xn,fn,g,df1,epsf,dxmin,imp,zero,
     &            io,ntot,iter,nsim,memax,rz(ns),rz(ngd),rz(nx),rz(nsa),
     &            rz(ngg),rz(nal),rz(naps),rz(nanc),rz(npoids),rz(nq),
     &            iz(njc),iz(nic),dz(nr),dz(na),dz(ne),dz(nrr),dz(nxga),
     &            dz(ny),dz(nw1),dz(nw2),izs,rzs,dzs)
      iz(1) = ntot
 999  return
      end
