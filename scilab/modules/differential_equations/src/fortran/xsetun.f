C/MEMBR ADD NAME=XSETUN,SSI=0
      subroutine xsetun (lun)
c
c%purpose
c this routine resets the logical unit number for messages.
c!
      integer lun, mesflg, lunit
cDEC$ ATTRIBUTES DLLIMPORT:: /eh0001/
      common /eh0001/ mesflg, lunit
c
      if (lun .gt. 0) lunit = lun
      return
c----------------------- end of subroutine xsetun ----------------------
      end
