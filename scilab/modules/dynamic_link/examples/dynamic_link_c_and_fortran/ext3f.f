
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA/ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine ext3f(ch,n,a,b,c)
c     ----------------------------
c     example 3 (passing a chain)
c     -->link('ext3f.o','ext3f');
c     -->a=[1,2,3];b=[4,5,6];n=3;
c     -->c=call('ext3f','yes',1,'c',n,2,'i',a,3,'d',b,4,'d','out',[1,3],5,'d')
c     -->c=sin(a)+cos(b)
c     -------------------------------------

      double precision a(*),b(*),c(*)
      character*(*) ch
      if(ch(1:3).eq.'yes') then
         do 1 k=1,n
            c(k)=sin(a(k))+cos(b(k))
 1       continue
      else
         do 2 k=1,n
            c(k)=a(k)+b(k)
 2       continue
      endif
      return
      end
