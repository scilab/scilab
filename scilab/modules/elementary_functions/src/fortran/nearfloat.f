c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) Bruno Pincon
C Copyright (C) 2010 - DIGITEO - Michael Baudin
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

      double precision function nearfloat(x, dir)
*
*     PURPOSE
*        Compute the near (double) float from x in 
*        the direction dir
*        dir >= 0 => toward +oo
*        dir < 0  => toward -oo
*
*     AUTHOR
*        Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
*
*     REMARK
*        This code may be shorter if we assume that the
*        radix base of floating point numbers is 2 : one
*        could use the frexp C function to extract the
*        mantissa and exponent part in place of dealing
*        with a call to the log function with corrections
*        to avoid possible floating point error...
*
      implicit none

*     PARAMETERS
      double precision x, dir

*     EXTERNAL FUNCTIONS
      double precision dlamch
      external         dlamch
      integer          isanan
      external         isanan
*     LOCAL VARIABLES
      double precision sign_x, ep, xa, d, m
      integer          e, i, p

*     STATIC VARIABLES
 	logical          first, DENORM

      double precision RMAX, RMIN, ULP, BASE, LNB, TINY
      save             RMAX, RMIN, ULP, BASE, LNB, TINY
           save             first, DENORM
      data             first /.true./


*     TEXT
*     got f.p. parameters used by the algorithm
      if (first) then
          RMAX = dlamch('o')
          RMIN = dlamch('u')
          BASE = dlamch('b')
          p    = int(dlamch('n'))
          LNB  = log(BASE)

*         computation of 1 ulp : 1 ulp = base^(1-p)
*         p = number of digits for the mantissa = dlamch('n')
          ULP  = BASE**(1 - p)

*         query if denormalized numbers are used : if yes
*         compute TINY the smallest denormalized number > 0 :
*         TINY is also the increment between 2 neighbooring
*         denormalized numbers
          if (RMIN/BASE .ne. 0.d0) then
             DENORM = .true.
             TINY = RMIN
             do i = 1, p-1
                TINY = TINY / BASE
             enddo
          else
             DENORM = .false.
          endif
          first = .false.
      endif

      d = sign(1.d0, dir)
      sign_x = sign(1.d0, x)
      xa = abs(x)

      if (isanan(x) .eq. 1) then
*     nan
         nearfloat = x

      elseif (xa .gt. RMAX) then
*     +-inf
         if (d*sign_x .lt. 0.d0) then
            nearfloat = sign_x * RMAX
         else
            nearfloat = x
         endif

      elseif (xa .ge. RMIN) then
*     usual case : x is a normalized floating point number
*        1/ got the exponent e and the exponent part ep = base^e
         e = int( log(xa)/LNB )
         ep = BASE**e
*        in case of xa very near RMAX an error in e (of + 1)
*        result in an overflow in ep
         if ( ep .gt. RMAX ) then
            e = e - 1
            ep = BASE**e
         endif
*        also in case of xa very near RMIN and when denormalized
*        number are not used, an error in e (of -1) results in a
*        flush to 0 for ep.
         if ( ep .eq. 0.d0 ) then
            e = e + 1
            ep = BASE**e
         endif

*        2/ got the mantissa 
         m = xa/ep

*        3/ verify that 1 <= m < BASE
         if ( m .lt. 1.d0 ) then
*           multiply m by BASE
            do while ( m .lt. 1.d0 )
               m = m * BASE
               ep = ep / BASE
            enddo
         elseif ( m .ge. BASE ) then
*           divide m by BASE
            do while ( m .ge. 1.d0 )
               m = m / BASE
               ep = ep * BASE
            enddo
         endif

*        4/ now compute the near float
         if (d*sign_x .lt. 0.d0) then
*           retrieve one ULP to m but there is a special case
            if ( m .eq. 1.d0 .and. xa .ne. RMIN ) then
*              this is the special case : we must retrieve ULP / BASE
               nearfloat = sign_x*( m - (ULP/BASE) )*ep
            else
               nearfloat = sign_x*( m - ULP )*ep
            endif
         else
            nearfloat = sign_x*(m + ULP)*ep
         endif

      elseif ( x .eq. 0.d0 ) then
*     case x = 0  nearfloat depends if denormalized numbers are used
         if (DENORM) then
            nearfloat = d*TINY
         else
            nearfloat = d*RMIN
         endif

      else
*     x is a denormalized number 
         nearfloat = x + d*TINY

      endif

      end
