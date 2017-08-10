c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
c         Example for fsolve 
c         -----------------------------------------------------
c         subroutine myprog(n,x,fvec,iflag)
c         integer n,iflag
c         double precision x(n),fvec(n)
c         ----------
c         calculate the functions at x and
c         return this vector in fvec.
c         ---------
c         return
c         end
c
c         the value of iflag should not be changed by fcn unless
c         the user wants to terminate execution of hybrd.
c         in this case set iflag to a negative integer.
c
c       n is a positive integer input variable set to the number
c         of functions and variables.
c
c       x is an array of length n. on input x must contain
c         an initial estimate of the solution vector. on output x
c         contains the final estimate of the solution vector.
c
c       fvec is an output array of length n which contains
c         the functions evaluated at the output x.
c
      subroutine fsol1(n,x,fvec,iflag)
      integer n,iflag
      double precision x(n),fvec(n)
      double precision a,b(2)
      common / exfsol / a(2*2)
      data a / 1.0,2.0, 7.0, 8.0 /
      data b / 10.0 ,11.0 /
      do 10 i=1,2 
         fvec(i)= b(i)
         do 20 j=1,2
            fvec(i)= fvec(i)+a(i+2*(j-1))*x(j)
 20      continue
 10   continue
      return
      end

c         ----------
c         calculate the jacobian at x and
c         return the result in fjac
c         ---------

      subroutine fsolj1(n,x,fjac,iflag)
      integer n,iflag
      double precision x(n),fjac(n*n)
      double precision a
      common / exfsol / a(2*2)
      do 10 i=1,2 
         fjac(i)= 0.0
         do 20 j=1,2
            fjac(i+(j-1)*2)= a(i+2*(j-1))
 20      continue
 10   continue
      return
      end

