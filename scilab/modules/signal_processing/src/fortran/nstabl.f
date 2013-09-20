c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

C/MEMBR ADD NAME=NSTABL,SSI=0
      subroutine nstabl(a,n,w,ist)
c     test de stabilite
      dimension a(*),w(*)
      double precision a,w,al
      ist=1
      n1=n+1
      do 1 i=1,n1
      w(i)=a(i)
      w(n1+i)=0.0d+0
   1  continue
      k=0
      if (n.eq.0) goto 99
  10  nk1=n-k+1
      do 11 j=1,nk1
  11  w(n1+j)=w(nk1-j+1)
      if(w(n1+nk1).eq.0.0d+0) return
      al=w(nk1)/w(n1+nk1)
      if(abs(al).ge.1.0d+0) return
      nk=n-k
      do 12 j=1,nk
  12  w(j)=w(j)-al*w(n1+j)  
      k=k+1
      if (k.lt.n) goto 10
  99  ist=0
      return
      end

