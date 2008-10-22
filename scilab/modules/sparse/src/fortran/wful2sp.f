c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine wful2sp(ma,na,ar,ai,nel,indr,rr,ri,zeror,zeroi)
c     converts a full matrix to a sparse representation
      double precision ar(ma,na),ai(ma,na),rr(*),ri(*)
      double precision zeror,zeroi
      integer indr(*)
c
      nel=0
      do 5 i=1,ma
         ni=0
         do 4 j=1,na
            if(ar(i,j).ne.zeror.or.ai(i,j).ne.zeroi) then
               nel=nel+1
               indr(ma+nel)=j
               rr(nel)=ar(i,j)
               ri(nel)=ai(i,j)
               ni=ni+1
            endif
 4       continue
         indr(i)=ni
 5    continue
      end
