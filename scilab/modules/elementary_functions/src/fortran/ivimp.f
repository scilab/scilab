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
      subroutine ivimp(i1,i2,pas,iv)
c     generate iv=i1:pas:i2
c
      integer i1,i2,pas,iv(*)
      k=0
      do 10 i=i1,i2,pas
         k=k+1
         iv(k)=i
 10   continue
      end
