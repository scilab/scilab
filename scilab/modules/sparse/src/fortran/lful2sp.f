c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine lful2sp(ma,na,a,nel,indr)
c     converts a full matrix to a sparse representation

      integer a(ma,na)
      integer indr(*)
c
      nel=0
      do 5 i=1,ma
         ni=0
         do 4 j=1,na
            if(a(i,j).ne.0) then
               nel=nel+1
               indr(ma+nel)=j
               ni=ni+1
            endif
 4       continue
         indr(i)=ni
 5    continue
      end
