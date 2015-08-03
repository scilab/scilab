C/MEMBR ADD NAME=OUCH,SSI=0
      subroutine ouch
c!purpose
c   writes an error message when the algorithm fails to
c   converge.  there seem to be two conditions under which
c   the algorithm fails to converge: (1) the initial
c   guess for the extremal frequencies is so poor that
c   the exchange iteration cannot get started, or
c   (2) near the termination of a correct design,
c   the deviation decreases due to rounding errors
c   and the program stops.  in this latter case the
c   filter design is probably acceptable, but should
c   be checked by computing a frequency response.
c!
      common /oops/niter,iout
      character bufstr*(4096)

      write(bufstr, 1001)
      call basout(io ,wte ,bufstr(1:lnblnk(bufstr)))
1001  format('************ failure to converge **********')

      write(bufstr, 1002)
      call basout(io ,wte ,bufstr(1:lnblnk(bufstr)))
1002  format('probable cause is machine rounding error')

      write(bufstr, 1003) niter
      call basout(io ,wte ,bufstr(1:lnblnk(bufstr)))
1003  format( '- number of iterations =',i4)

      write(bufstr, 1004)
      call basout(io ,wte ,bufstr(1:lnblnk(bufstr)))
1004  format( 'if the number of iterations exceeds 3 the design')

      write(bufstr, 1005)
      call basout(io ,wte ,bufstr(1:lnblnk(bufstr)))
1005  format('may be correct, but should be verified with an fft')

      return
      end
