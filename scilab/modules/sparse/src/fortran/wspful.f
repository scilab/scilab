c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine wspful(ma,na,ar,ai,nela,inda,rr,ri)

      double precision ar(nela),ai(nela),rr(*),ri(*)
      integer ma,na,nela,inda(*)
c
      call dset(ma*na,0.0d0,rr,1)
      call dset(ma*na,0.0d0,ri,1)
      i0=0
      i1=i0
      i=1
      do 10 k=1,nela
 08      i0=i0+1
         if(i0-i1.gt.inda(i)) then
            i1=i0
            i=i+1
            goto 08
         endif
         j=inda(ma+k)
         rr(1+(i-1)+(j-1)*ma)=ar(k)
         ri(1+(i-1)+(j-1)*ma)=ai(k)
 10   continue
      end
