c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


      subroutine compc(l,lp2,ls2,mma,n,ncomp,nn,pw)        
      implicit integer (a-z)
      dimension lp2(*),ls2(mma),pw(n),nn(n),ncomp(n)
      do 10 i=1,n
         pw(i)=0
         nn(i)=lp2(i+1)-lp2(i)
         ncomp(i)=0
 10   continue
      i0=1
      l=0
 100  l=l+1
      ncomp(i0)=l
      call dfs1(i0,l,lp2,ls2,mma,n,ncomp,nn,pw)
 200  do 210 ii=1,n
         if(ncomp(ii).gt.0) goto 210
         i0=ii
         goto 100
 210  continue
      end
