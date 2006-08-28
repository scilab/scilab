c  interface function 
c   ********************
       subroutine specfun
       include '../stack.h'
       rhs = max(0,rhs)
c
       goto (1,2,3,4,5,6,7,8) fin
       return
 1     call intsbesseli('besseli')
       return
 2     call intsbesselj('besselj')
       return
 3     call intsbesselk('besselk')
       return
 4     call intsbessely('bessely')
       return
 5     call intsgamma('gamma')
       return
 6     call intslgamma('lgamma')
       return
 7     call intsdlgamma('dlgamma')
       return
 8     call intscalerf('calerf')
       return
       end
