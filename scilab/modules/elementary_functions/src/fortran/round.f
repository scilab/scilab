      double precision function sciround(x1)
c!
c     Copyright INRIA
      double precision x1,x,y,z,e,h
      data h/1.0d+9/
c
      x=x1
      if (x.eq.0d0) then 
         sciround=x
         return
      endif
      if ((2.0d0*x).eq.dble(int(2.d0*x))) then
c     changing the signs gives round(0.5)=0 round(-0.5)=0
         if (x.gt.0.d0) x=x+1.d-10
         if (x.lt.0.d0) x=x-1.d-10
      endif
      z = abs(x)
c     -----testing Nans 
      if (isanan(x).eq.1) then 
         sciround=x
         return
      endif
      y = z + 1.0d+0
      if (y .eq. z) then 
         sciround=x
         return
      endif
      y = 0.0d+0
      e = h
 10   if (e .ge. z) go to 20
      e = 2.0d+0*e
      go to 10
 20   if (e .le. h) go to 30
      if (e .le. z) y = y + e
      if (e .le. z) z = z - e
      e = e/2.0d+0
      go to 20
 30   z = int(z + 0.50d+0)
      y = y + z
      if (x .lt. 0.0d+0) y = -y
      sciround = y
      return
      end
