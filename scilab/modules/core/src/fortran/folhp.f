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

      integer function folhp(ls,alpha,beta,s,p)
      integer ls
      double precision alpha,beta,s,p
c!purpose
c      this routine checks if
c    the real root alpha/beta lies in the open left half plane
c       (if ls=1)
c     the complex conjugate roots with sum s and product p lie
c     in the open left half plane (if ls=2)
c     if so, folhp=1, otherwise, folhp=-1
c      in this function the parameter p is not referenced
c
c!calling sequence
c
c     integer function folhp(ls,alpha,beta,s,p)
c     integer ls
c     double precision alpha,beta,s,p
c!auxiliary routines
c     none
c!
      folhp=-1
      if(ls.eq.2) go to 2
      if(alpha*beta.lt.0.0d+0) folhp=1
      return
 2    if(s.lt.0.0d+0) folhp=1
      return
      end
