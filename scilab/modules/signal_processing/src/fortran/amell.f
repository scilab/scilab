      subroutine amell(du, dk, dsn2 ,n)
c!purpose
c calculation of the jacobi's elliptic function am(u,k)
c
c external calculation of the parameter necessary
c ddk = k($dk)
c dq = exp(-pi*k'/k) ... (jacobi's nome)
c!calling sequence
c     subroutine amell(du, dk, dsn2 ,n)
c     double precision du(n),dsn2(n),dk
c     integer n
c!
c
      double precision dpi, domi,dsn2
      double precision de, dz, dpi2, dq, dm, du, dk, dc, dqq, dh, dq1,
     *    dq2 ,ddk ,dkprim,dsn22,du1
      double precision dlamch
      dimension du(n),dsn2(n)
      logical neg
c
      data de, dz /1.0d+0,2.0d+0/
c
      domi=2.0d+0*dlamch('p')
      dpi=4.0d+0*atan(1.0d+0)
      call compel(dk,ddk)
      call compel(sqrt(1.0d+0-dk*dk),dkprim)
      do 1 ijk=1,n
      neg=.false.
      du1=du(ijk)
      if(du1.lt.0.0d+0) then
      neg=.true.
      du1=-du1
      endif
      du1=mod(du1,4.0d+0*ddk)
      dq=exp(-dpi*dkprim/ddk)
      dpi2 = dpi/dz
      if (abs(dq).ge.de) go to 30
c
      dm = dpi2*du1/ddk
      dc = dz*dm
      dc =  cos(dc)
c
      dm =  sin(dm)*ddk/dpi2
      dqq = dq*dq
      dq1 = dq
      dq2 = dqq
c
      do 10 i=1,100
        dh = (de-dq1)/(de-dq2)
        dh = dh*dh
        dh = dh*(de-dz*dq2*dc+dq2*dq2)
        dh = dh/(de-dz*dq1*dc+dq1*dq1)
        dm = dm*dh
c
        dh = abs(de-dh)
        if (dh.lt.domi) go to 20
c
        dq1 = dq1*dqq
        dq2 = dq2*dqq
  10  continue
c
      go to 30
c
  20  if(dm.lt.-1.0d+0) dm=-1.0d+0
      if(dm.gt.1.0d+0) dm=1.0d+0
      dsn22 = asin(dm)
      if(dsn22.lt.0.0d+0) dsn22=dsn22+dpi*2.0d+0
      if(du1.ge.ddk.and.du1.le.2.0d+0*ddk) then
      dsn22=dpi-dsn22
      endif
      if(du1.ge.2.0d+0*ddk.and .du1.le.3.0d+0*ddk) then
      dsn22=3.0d+0*dpi-dsn22
      endif
      goto 2
c
  30  dsn22 = 0.0d+0
 2    continue
      if (neg) then
      dsn22=-dsn22
      endif
      dsn2(ijk)=dsn22
 1    continue
      return
      end
