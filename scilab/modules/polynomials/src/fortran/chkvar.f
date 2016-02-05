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

      logical function chkvar(v1,v2)
c     compare polynomials formal names
      integer v1(4),v2(4)
      logical test
      
      if(v1(1).eq.0.or.v2(1).eq.0) then
         test=.true.
      else
         test=.false.
         do 10 i=1,4
            if(v1(i).ne.v2(i)) goto 20
 10      continue
         test=.true.
      endif
 20   chkvar=test
      return
      end
      
     
      
