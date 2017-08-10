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
      subroutine wmul(ar,ai,br,bi,cr,ci)
c
      double precision ar,ai,br,bi,cr,ci,t
c     c = a*b
      t = ar*bi + ai*br
      cr = ar*br - ai*bi
      ci = t
      return
      end
