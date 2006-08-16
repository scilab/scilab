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
      write(iout,1)niter
    1 format(1x,'************ failure to converge **********',/,
     1       1x,'probable cause is machine rounding error',/,
     2       1x,'- number of iterations =',i4,/,
     3       1x,'if the number of iterations exceeds 3 the design',/
     4       1x,'may be correct, but should be verified with an fft')
      return
      end
