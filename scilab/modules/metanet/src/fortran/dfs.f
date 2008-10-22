c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine dfs(i0,lp1,ls1,m,n,nn,num,pw)
      implicit integer (a-z)
      dimension lp1(*),ls1(m),pw(n),num(n),nn(n)
      if (i0 .lt. 0 .or. i0 .gt. n) then
         call erro('bad internal node number')
         return
      endif
      do 10 i=1,n
         pw(i)=0
         nn(i)=lp1(i+1)-lp1(i)
         num(i)=0
 10   continue
      pw(i0)=i0
      k=1
      num(i0)=k
      i=i0
 100  if(nn(i).eq.0) goto 200
      ll=lp1(i)+nn(i)-1
      j=ls1(ll)
      nn(i)=nn(i)-1
      if(pw(j).ne.0) goto 100
      pw(j)=i
      k=k+1
      num(j)=k
      i=j
      go to 100
 200  if(i.eq.i0) go to 1000
      i=pw(i)
      goto 100
 1000 return
      end
