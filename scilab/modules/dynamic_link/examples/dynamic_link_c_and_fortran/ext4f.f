
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA/ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine ext4f(n,a,b,c)
c     example 4 (reading a chain)
c     -->link('ext4f.o','ext4f');
c     -->a=[1,2,3];b=[4,5,6];n=3;yes='yes'
c     -->c=call('ext4f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d')
c     -->c=sin(a)+cos(b)
c     -->yes='no'
c     -->c=a+b
c     -->clear yes  --> undefined variable : yes

      double precision a(*),b(*),c(*)
      integer chmax
      logical creadchain
      parameter (chmax=10)
      character ch*(chmax)

c     If chain named yes exists reads it in ch else return
      lch=chmax
      if(.not.creadchain('yes'//char(0),lch,ch)) return
c     *********************************     
      if(ch(1:lch).eq.'yes') then
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
      
