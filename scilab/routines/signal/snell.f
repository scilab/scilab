C/MEMBR ADD NAME=SNELL,SSI=0
      subroutine snell(dsn2,du, dk, dq)
c!purpose
c calculation of the jacobi's elliptic function sn(u,k)
c
c external calculation of the parameter necessary
c dk = k($k)
c dq = exp(-pi*k'/k) ... (jacobi's nome)
c!
c
      double precision dpi, domi
      double precision de, dz, dpi2, dq, dm, du, dk, dc, dqq, dh, dq1,
     *    dq2
c
      domi=2.0d+0*dlamch('p')
      dpi=4.0d+0*atan(1.0d+0)
c
      data de, dz /1.0d+0,2.0d+0/
c
      dpi2 = dpi/dz
      if (abs(dq).ge.de) go to 30
c
      dm = dpi2*du/dk
      dc = dz*dm
      dc =  cos(dc)
c
      dm =  sin(dm)*dk/dpi2
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
  20  dsn2 = dm
      return
c
  30  dsn2 = 0.0d+0
      return
      end
