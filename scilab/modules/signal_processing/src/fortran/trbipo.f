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

C/MEMBR ADD NAME=TRBIPO,SSI=0
      subroutine trbipo(nmaxi,maxdeg,nj,sfa,sm,spr,spi,zfa,zpr,zpi)
c!purpose
c  bilinear transformation of the poles
c!
      implicit double precision (a-h,o-z)
      double precision sm(maxdeg,*)
      double precision spr(*),spi(*),zpr(*),zpi(*)
      flmi=2.0d+0*dlamch('p')
      zfa = sfa*sm(nmaxi-1,1)
      do 20 i=1,nj
        qr = spr(i)
        q = 1.0d+0 - qr
        qi = spi(i)
        if (abs(qi).lt.flmi) go to 10
        qqr = qr*qr
        qqi = qi*qi
        zfa = zfa/(q-qr+qqr+qqi)
        q = 1.0d+0/(q*q+qqi)
        zpr(i) = (1.0d+0-qqr-qqi)*q
        zpi(i) = 2.0d+0*qi*q
        go to 20
  10    zpr(i) = (1.0d+0+qr)/q
        zpi(i) = 0.0d+0
        zfa = zfa/q
  20  continue
      return
      end
