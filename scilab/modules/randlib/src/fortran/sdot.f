      DOUBLE PRECISION FUNCTION sdot(n,sx,incx,sy,incy)
      DOUBLE PRECISION sx(1),sy(1),stemp
      INTEGER i,incx,incy,ix,iy,m,mp1,n

      stemp = 0.0E0
      sdot = 0.0E0
      IF (n.LE.0) RETURN
      IF (incx.EQ.1 .AND. incy.EQ.1) GO TO 20
      ix = 1
      iy = 1
      IF (incx.LT.0) ix = (-n+1)*incx + 1
      IF (incy.LT.0) iy = (-n+1)*incy + 1
      DO 10 i = 1,n
          stemp = stemp + sx(ix)*sy(iy)
          ix = ix + incx
          iy = iy + incy
   10 CONTINUE
      sdot = stemp
      RETURN

   20 m = mod(n,5)
      IF (m.EQ.0) GO TO 40
      DO 30 i = 1,m
          stemp = stemp + sx(i)*sy(i)
   30 CONTINUE
      IF (n.LT.5) GO TO 60
   40 mp1 = m + 1
      DO 50 i = mp1,n,5
          stemp = stemp + sx(i)*sy(i) + sx(i+1)*sy(i+1) +
     +            sx(i+2)*sy(i+2) + sx(i+3)*sy(i+3) + sx(i+4)*sy(i+4)
   50 CONTINUE
   60 sdot = stemp
      RETURN

      END
