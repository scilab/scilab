c     --------------------------
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     --------------------------
      subroutine scigspec(fname)
      INCLUDE 'stack.h'
c     
      character*(*) fname
      integer iadr,sadr
     
      sadr(l)=(l/2)+1
      iadr(l)=l+l-1

c     osolete
c     call macro for emulation
      call cvname(ids(1,pt+1),'gspec',0)
      fun=-1
      return

      end
c     --------------------------
      