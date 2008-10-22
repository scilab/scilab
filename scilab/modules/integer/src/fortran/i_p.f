c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine i_p
      include 'stack.h'
      logical checkrhs,checklhs,crewimat
      integer iadr,topk
c     
      iadr(l)=l+l-1
c
      topk=top+1
      if(.not.checkrhs('%i_p',1,1)) return
      if(.not.checklhs('%i_p',1,1)) return

      lineln=lct(5)
      ndgt=lct(7)

      il=iadr(lstk(top))

      if(istk(il).ne.8) then
         call error(44)
         return
      endif
      m=istk(il+1)
      n=istk(il+2)
      is=istk(il+3)
      
      li=il+4
      if (.not.crewimat("print",topk,1,m*n+2*n,lw)) return
      call genmdsp(is,istk(li),m,m,n,lineln,wte,buf,istk(lw))
      istk(il)=0
      lstk(top+1)=lstk(top)+1
      return
      end
