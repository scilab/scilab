c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ENPC
c ...
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
c     Example for colnew 
c     ------------------
c     See also Flist and tests/colnew.sci 
c
      subroutine cngu (x, z, dmval)
      implicit double precision  (a-h,o-z)
      dimension z(4) , dmval(2)
      double precision gamma, xt
      data gamma/1.1d0/, xt/1/
      xt = dsqrt(2.d0*(gamma-1.d0)/gamma)
      cons = gamma * x * (1.d0-.5d0*x*x)
      dcons = gamma * (1.d0 - 1.5d0*x*x)
      d2cons = -3.d0 * gamma * x
      if (x .gt. xt) go to 10
      z(1) = 2.d0 * x
      z(2) = 2.d0
      z(3) = -2.d0*x + cons
      z(4) = -2.d0 + dcons
      dmval(2) = d2cons
      go to 20
   10 z(1) = 0.d0
      z(2) = 0.d0
      z(3) = -cons
      z(4) = -dcons
      dmval(2) = -d2cons
   20 dmval(1) = 0.d0
      return
      end
c................................................................
      subroutine cnf (x, z, f)
      implicit double precision  (a-h,o-z)
      dimension z(4), f(2)
      double precision eps, dmu, eps4mu, gamma, xt
      data eps/.01d0/, dmu/.01d0/, eps4mu/1.0/, gamma/1.1d0/, xt/1/
      eps4mu =eps**4/dmu
      xt = dsqrt(2.d0*(gamma-1.d0)/gamma)
      f(1) = z(1)/x/x - z(2)/x + (z(1) - z(3)*(1.d0-z(1)/x) -
     .       gamma*x*(1.d0-x*x/2.)) / eps4mu
      f(2) = z(3)/x/x - z(4)/x + z(1)*(1.d0-z(1)/2.d0/x) / dmu
      return
      end
c................................................................
      subroutine cndf (x, z, df)
      implicit double precision  (a-h,o-z)
      dimension z(4), df(2,4)
      double precision eps, dmu, eps4mu, gamma, xt
      data eps/.01d0/, dmu/.01d0/, eps4mu/1/, gamma/1.1d0/, xt/1/
      eps4mu =eps**4/dmu
      xt = dsqrt(2.d0*(gamma-1.d0)/gamma)
      df(1,1) = 1.d0/x/x +(1.d0 + z(3)/x) / eps4mu
      df(1,2) = -1.d0/x
      df(1,3) = -(1.d0-z(1)/x) / eps4mu
      df(1,4) = 0.d0
      df(2,1) = (1.d0 - z(1)/x) / dmu
      df(2,2) = 0.d0
      df(2,3) = 1.d0/x/x
      df(2,4) = -1.d0/x
      return
      end
c................................................................
      subroutine cng (i, z, g)
      implicit double precision  (a-h,o-z)
      dimension z(4)
      go to (1, 2, 1, 3), i
    1 g = z(1)
      return
    2 g = z(3)
      return
    3 g = z(4) - .3d0*z(3) + .7d0
      return
      end
c................................................................
      subroutine cndg(i, z, dg)
      implicit double precision  (a-h,o-z)
      dimension z(4), dg(4)
      do 10 j=1,4
   10    dg(j) = 0. d0
      go to (1, 2, 1, 3), i
    1 dg(1) = 1.d0
      return
    2 dg(3) = 1.d0
      return
    3 dg(4) = 1.d0
      dg(3) = -.3d0
      return
      end
c
c----------------------------------------------------------------
c

