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

      integer function fout(lsize,alpha,beta,s,p)

      integer lsize
      double precision alpha,beta,s,p
c!purpose
c      this function checks if
c       the real root alpha/beta lies outside the unit disc
c       (if lsize=1)
c       the complex conjugate roots with sum s and product p lie
c       outside the unit disc (if lsize=2).
c      if so, fout=1, otherwise, fout=-1
c      in this function the parameter s is not referenced
c
c!calling sequence
c
c     integer function fout(lsize,alpha,beta,s,p)
c     integer lsize
c     double precision alpha,beta,s,p
c!auxiliary routines
c     abs (fortran)
c!
      fout=-1
      if(lsize.eq.2) go to 2
      if(abs(alpha).ge.abs(beta)) fout=1
      return
  2   if(abs(p).ge.1.) fout=1
      return
      end
