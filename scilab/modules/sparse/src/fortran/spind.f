c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine   spind(i,xadj,nb,nza)
c converts adjacency into sparse matrix
      integer*4 i(*),xadj(*)
      integer*4 nb,ii,io,nza,k
      io=0
      do 100 k=1,nb
      ii=xadj(k+1)-xadj(k)
      do 200 j=1,ii
      io = io + 1
      i(io) = k
 200  continue
 100  continue
      return
      end
