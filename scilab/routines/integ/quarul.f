C/MEMBR ADD NAME=QUARUL,SSI=0
      subroutine quarul(f, a, b, result, abserr, resabs, resasc)
c
c     based on quadpack routine quarul
c     ******************************************************
c
c           purpose
c              to compute i = integral of f over (a,b), with error
c                             estimate
c                         j = integral of abs(f) over (a,b)
c
c           calling sequence
c              call quarul (f,a,b,result,abserr,resabs,resasc)
c
c           parameters
c              f      - function subprogram defining the integrand
c                       function f(x). the actual name for f needs
c                       to be declared e x t e r n a l in the
c                       calling program
c
c              a      - lower limit of integration
c
c              b      - upper limit of integration
c
c              result - approximation to the integral i.
c                       result is calculated by applying
c                       the 21-point gauss-kronrod rule
c                       (resk), obtained by optimal
c                       addition of abscissae to the
c                       10-point gauss  rule (resg).
c
c              abserr - estimate of the modulus of the
c                       absolute error, which should not
c                       exceed abs(i-result)
c              resabs - approximation to the integral j
c
c              resasc - approximation to the integral of
c                       abs(f-i/(b-a)) over (a,b)
c
c     ******************************************************
c     .. scalar arguments ..
      double precision a, abserr, b, resabs, resasc, result
c     .. function arguments ..
      double precision f
      external f
c recuperation d'erreur
      integer       iero
      common/ierajf/iero
c     ..
c     .. local scalars ..
      double precision absc, centre, dhlgth, epmach, fc, fsum, fval1,
     * fval2,hlgth, resg, resk, reskh, uflow
      integer j
c     .. local arrays ..
      double precision fv1(10), fv2(10), wg(10), wgk(11), xgk(11)
c     .. function references ..
      double precision dlamch
c     ..
c
c            the abscissae and weights are given for the
c            interval (-1,1) . because of symmetry only the
c            positive abscissae and their corresponding
c            weights are given.
c            xgk    - abscissae of the 21-point gauss-kronrod rule
c                     xgk(2), xgk(4), .... abscissae of the 10-point
c                     gauss rule
c                     xgk(1), xgk(3), .... abscissae which
c                     are optimally added to the 10-point
c                     gauss rule
c            wgk    - weights of the 21-point gauss-kronrod rule
c            wg     - weights of the 10-point gauss rule,
c                     corresponding to the abscissae xgk(2),
c                     xgk(4), ... wg(1), wg(3), ... are set
c                     to zero.
c
      data xgk(1), xgk(2), xgk(3), xgk(4), xgk(5), xgk(6), xgk(7),xgk(8)
     *, xgk(9), xgk(10), xgk(11) /0.99565716302580808073552728070d+00,
     *0.97390652851717172007796401210d+00,
     *0.93015749135570822600120718010d+00,
     *0.86506336668898451073209668840d+00,
     *0.78081772658641689706371757830d+00,
     *0.67940956829902440623432736510d+00,
     *0.56275713466860468333900009930d+00,
     *0.43339539412924719079926594320d+00,
     *0.29439286270146019813112660310d+00,
     *0.14887433898163121088482600110d+00,0.0d+0/
      data wgk(1), wgk(2), wgk(3), wgk(4), wgk(5), wgk(6), wgk(7),wgk(8)
     *, wgk(9), wgk(10), wgk(11) /0.11694638867371874278064396060d-01,
     *0.32558162307964727478818972460d-01,
     *0.54755896574351996031381300240d-01,
     *0.75039674810919952767043140920d-01,
     *0.93125454583697605535065465080d-01,
     *0.10938715880229764189921059030d+00,
     *0.12349197626206585107795810980d+00,
     *0.13470921731147332592805400180d+00,
     *0.14277593857706008079709427310d+00,
     *0.14773910490133849137484151600d+00,
     *0.14944555400291690566493646840d+00/
      data wg(1), wg(2), wg(3), wg(4), wg(5), wg(6), wg(7), wg(8),wg(9),
     * wg(10) /0.0d+0,0.66671344308688137593568809890d-01,0.0d+0,
     *0.14945134915058059314577633970d+00,0.0d+0,
     *0.21908636251598204399553493420d+00,0.0d+0,
     *0.26926671930999635509122692160d+00,0.0d+0,
     *0.29552422471475287017389299470d+00/
      epmach=dlamch('p')
      uflow=dlamch('u')
c
c           list of major variables
c           ----------------------
c           centre - mid point of the interval
c           hlgth  - half length of the interval
c           absc   - abscissa
c           fval*  - function value
c           resg   - 10-point gauss formula
c           resk   - 21-point gauss-kronrod formula
c           reskh  - approximation to mean value of f over
c                    (a,b), i.e. to i/(b-a)
c
      centre = 0.50d+00*(a+b)
      hlgth = 0.50d+00*(b-a)
      dhlgth = abs(hlgth)
c
c           compute the 21-point gauss-kronrod approximation to
c           the integral, and estimate the absolute error
c
      resg = 0.0d+00
      fc = f(centre)
      if(iero.ne.0) return
      resk = wgk(11)*fc
      resabs = abs(resk)
      do 20 j=1,10
         absc = hlgth*xgk(j)
         fval1 = f(centre-absc)
         if(iero.ne.0) return
         fval2 = f(centre+absc)
         if(iero.ne.0) return
         fv1(j) = fval1
         fv2(j) = fval2
         fsum = fval1 + fval2
         resg = resg + wg(j)*fsum
         resk = resk + wgk(j)*fsum
         resabs = resabs + wgk(j)*(abs(fval1)+abs(fval2))
   20 continue
      reskh = resk*0.50d+00
      resasc = wgk(11)*abs(fc-reskh)
      do 40 j=1,10
         resasc = resasc + wgk(j)*(abs(fv1(j)-reskh)+abs(fv2(j)-reskh)
     *   )
   40 continue
      result = resk*hlgth
      resabs = resabs*dhlgth
      resasc = resasc*dhlgth
      abserr = abs((resk-resg)*hlgth)
      if (resasc.ne.0.0d+00 .and. abserr.ne.0.0d+00) abserr =resasc*
     *min(0.10d+01,(0.20d+03*abserr/resasc)**1.50d+0)
      if (resabs.gt.uflow/(0.50d+02*epmach)) abserr =max(epmach*resabs*
     *0.50d+02,abserr)
      return
      end
