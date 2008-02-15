
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt



      subroutine ext13fi(fname)
      character*(*) fname
      logical getrhsvar, createvar,  putlhsvar
      include 'stack.h'
c
      nbvars=0
      minrhs=3
      maxrhs=3
      minlhs=1
      maxlhs=1
c
      if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
         call erro('wrong number of rhs arguments')
         return
      endif
      if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
         call erro('wrong number of lhs arguments')
         return
      endif
c     
c     when using fort the first parameter is the function name 
      if(.not.getrhsvar(1,'c',m1,n1,l1)) return
      if(.not.getrhsvar(2,'d',m2,n2,l2)) return
      if(.not.getrhsvar(3,'d',m3,n3,l3)) return
      if(m2*n2.ne.m3*n3) then 
         call erro('incompatible arguments ')
         return
      endif
c
      if(.not.createvar(4,'d',m2,n2,l4)) return
c
      call ext14f(n2*m2,stk(l2),stk(l3),stk(l4))
      lhsvar(1)=4
      if(.not.putlhsvar()) return
      end


      subroutine ext13f(n,a,b,c)
c     (very) simple example 1
c     -->link('ext1f.o','ext1f');
c     -->a=[1,2,3];b=[4,5,6];n=3;
c     -->c=fort('ext1f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d')
c     c=a+b
      double precision a(*),b(*),c(*)
      do 1 k=1,n
         c(k)=a(k)+b(k)
 1    continue
      return
      end
