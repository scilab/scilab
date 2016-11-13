c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1999 - INRIA - Serge Steer
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

      integer function find(lsize,alpha,beta,s,p)
      integer lsize
      double precision alpha,beta,s,p
c!purpose
c      this function checks if
c       the real root alpha/beta lies in the unit disc (if lsize=1)
c       the complex conjugate roots with sum s and product p lie
c       in the unit disc (if lsize=2)
c      if so, find=1, otherwise find=-1
c      in this function the parameter s is not referenced
c
c!calling sequence
c
c     integer function find(lsize,alpha,beta,s,p)
c     integer lsize
c     double precision alpha,beta,s,p
c!auxiliary routines
c     abs (fortran)
c!
      find=-1
      if(lsize.eq.2) go to 2
      if(abs(alpha).lt.abs(beta)) find=1
      return
  2   if(abs(p).lt.1.0d+0) find=1
      return
      end
