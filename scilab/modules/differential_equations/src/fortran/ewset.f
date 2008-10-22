C/MEMBR ADD NAME=EWSET,SSI=0
      subroutine ewset (n, itol, rtol, atol, ycur, ewt)
clll. optimize
c-----------------------------------------------------------------------
c%purpose
c this subroutine sets the error weight vector ewt according to
c     ewt(i) = rtol(i)*abs(ycur(i)) + atol(i),  i = 1,...,n,
c with the subscript on rtol and/or atol possibly replaced by 1 above,
c depending on the value of itol.
c!
c-----------------------------------------------------------------------
      integer n, itol
      integer i
      double precision rtol, atol, ycur, ewt
      double precision atoli, rtoli
      dimension rtol(*), atol(*), ycur(n), ewt(n)
      rtoli = rtol(1)
      atoli = atol(1)
      do 10 i = 1,n
        if (itol .ge. 3) rtoli = rtol(i)
        if (itol .eq. 2 .or. itol .eq. 4) atoli = atol(i)
        ewt(i) = rtoli*abs(ycur(i)) + atoli
 10     continue
      return
c----------------------- end of subroutine ewset -----------------------
      end
