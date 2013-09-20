c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine sciislink(fname) 
C     ================================================================
C     [%t|%false,number]=c_link(name [,ilib]) 
C     checks if name is linked and optionaly linked form lib number ilib
C     ================================================================
      character*(*) fname
cc    implicit undefined (a-z)
      include 'stack.h'
      integer topk,iadr
c      integer m3,n3,lr3,nlr3,m2,n2,il2,ild2,m1,n1,il1,ild1
      integer ilib,iv ,l1
      logical checkrhs,checklhs,crebmat,getscalar,getsmat,cremat
      iadr(l)=l+l-1
      if (.not.checkrhs(fname,1,2)) return
      if (.not.checklhs(fname,1,2)) return
      topk=top
      if (rhs.eq.2) then 
         if(.not.getscalar(fname,topk,top,lr))return
         ilib = int(stk(lr))
         top=top-1
      else
         ilib=-1
      endif
      if(.not.getsmat(fname,topk,top,m3,n3,1,1,lr3,nlr3))return
      call cvstr(nlr3,istk(lr3),buf,1)
      buf(nlr3+1:nlr3+1)=char(0)
      call iislink(buf,ilib)
      if (.not.crebmat(fname,top,1,1,lr)) return
      if (ilib.eq.-1)  then 
         istk(lr) = 0
      else
         istk(lr) = 1
      endif
      if ( lhs.eq.2) then 
         top=top+1
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr)= ilib
      endif
      return
      end


