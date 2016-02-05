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
      subroutine fmani1 (mode,n,d,w,indi)
c
      implicit double precision (a-h,o-z)
      dimension d(n),w(n),indi(n)
c
      if(mode.eq.-1) go to 20
      do 10 i=1,n
   10 w(indi(i))=d(i)
      return
   20 do 30 i=1,n
   30 w(i)=d(indi(i))
      return
      end
