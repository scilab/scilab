
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA/ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine ext7f(a,b)
c     example 7
c     creating vector c in scilab internal stack
c     -->link('ext7f.o','ext7f')
c     -->a=[1,2,3]; b=[2,3,4];
c     c does not exist (c made by the call to matz)
c     -->call('ext7f',a,1,'d',b,2,'d','out',1);
c     c now exists
c     -->c=a+2*b

      double precision a(3),b(3),c(3),w
      logical cwritemat,cwritechain
      do 1 k=1,3
         c(k)=a(k)+2.0d0*b(k)
 1    continue
c     sending c array values to c Scilab variable 
c     of size [1,3]
      if (.not.cwritemat('c'//char(0),1,3,c)) return
c     sending string 'test' (size 4) to Scilab variable d
      if (.not.cwritechain('d'//char(0),4,'test')) return
      return
      end
