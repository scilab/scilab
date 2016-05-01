C/MEMBR ADD NAME=XSETF,SSI=0
      subroutine xsetf (mflag)
c
c%purpose
c this routine resets the print control flag mflag.
c!
      integer mflag, mesflg, lunit
cDEC$ ATTRIBUTES DLLIMPORT:: /eh0001/
      common /eh0001/ mesflg, lunit
c
      if (mflag .eq. 0 .or. mflag .eq. 1) mesflg = mflag
      return
c----------------------- end of subroutine xsetf -----------------------
      end
