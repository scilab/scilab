      double precision function dsn2(du, dk, dq)
c!purpose
c  function:     dsn2
c  calculation of the jacobi's elliptic function sn(u,k) 
c  
c  external calculation of the parameter necessary 
c  dk = k($k)
c  dq = exp(-pi*k'/k) ... (jacobi's nome)
c!
  
      implicit double precision (a-h,o-z)
  
      data de, dz /1.d00,2.d00/ 
  
      dpi=4.0d0*atan(1.0d00)
      domi=2.0d0*dlamch('p')
     
      dpi2 = dpi/dz 
      if (abs(dq).ge.de) go to 30
  
      dm = dpi2*du/dk 
      dc = dz*dm
      dc = cos(dc) 
  
      dm = sin(dm)*dk/dpi2 
      dqq = dq*dq 
      dq1 = dq
      dq2 = dqq 
  
      do 10 i=1,100 
        dh = (de-dq1)/(de-dq2)
        dh = dh*dh
        dh = dh*(de-dz*dq2*dc+dq2*dq2)
        dh = dh/(de-dz*dq1*dc+dq1*dq1)
        dm = dm*dh
  
        dh = abs(de-dh)
        if (dh.lt.domi) go to 20
  
        dq1 = dq1*dqq 
        dq2 = dq2*dqq 
  10  continue
  
      go to 30
  
  20  dsn2 = dm 
      return
  
  30  dsn2 = 0.d00
      return
      end 
