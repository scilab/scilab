c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine compfc(inf,lp1,ls1,m,n,nc,nfcomp,
     &     nn,num,pw,pile)
      implicit integer (a-z)
      dimension lp1(*),ls1(m),pw(n),nn(n),nfcomp(n),
     &     pile(n),num(n),inf(n)
      do 10 i=1,n
         pw(i)=0
         nn(i)=lp1(i+1)-lp1(i)
         num(i)=0
         nfcomp(i)=0
 10   continue
      i0=1
      nc=1
 100  call cfc(i0,inf,lp1,ls1,m,n,nc,nfcomp,
     &     nn,num,pw,pile)
      do 210 ii=1,n
         if(nfcomp(ii).gt.0) goto 210
         i0=ii
         goto 100
 210  continue
      nc=nc-1
      end
