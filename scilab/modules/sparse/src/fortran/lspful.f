c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine lspful(ma,na,nela,inda,r)
      integer r(*)
      integer ma,na,nela,inda(*)
c
      call iset(ma*na,0,r,1)
      i0=0
      i1=i0
      i=1
      do 10 k=1,nela
 08      i0=i0+1
         if(i0-i1.le.inda(i)) goto 09
         i1=i0
         i=i+1
         goto 08
 09      j=inda(ma+k)
         r(1+(i-1)+(j-1)*ma)=1
 10   continue
      end
