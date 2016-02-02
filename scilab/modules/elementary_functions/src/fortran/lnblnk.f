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
      integer function lnblnk(str)
c
      character*(*) str
      n=len(str)+1
 10   continue
      n=n-1
      if (n.eq.0) then
         lnblnk=0
         return
      else
         if (str(n:n).ne.' ') then
            lnblnk=n
            return
         endif
      endif
      goto 10
      end
