c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intpredef
      include 'stack.h'
      logical checkrhs,checklhs,getscalar,cremat,getsmat,checkval
      integer bbots
      integer iadr
c
      iadr(l)=l+l-1
c
      rhs=max(0,rhs)

      if(.not.checklhs('predef',1,1)) return
      if(.not.checkrhs('predef',0,1)) return

      bbots=bbot
      if(rhs.le.0) then
c     .  get number of protected variables
         top=top+1
         if(.not.cremat('predef',top,0,1,1,l,lc)) return
         stk(l)=isiz-bbots
      else
         ityp=abs(istk(iadr(lstk(top))))
         if(ityp.eq.1) then
c     .     mark n oldest current variables as protected
            if(.not.getscalar('predef',top,top,l)) return
            is=isiz
            bbot=max(bot,min(isiz-nint(stk(l)),bot0))
         elseif(ityp.eq.10) then
            if(.not.getsmat('predef',top,top,m,n,1,1,l,n1)) return
            if(.not.checkval('predef',m*n,1) ) return
            if(abs(istk(l)).eq.10) then
c     .        mark all current variables as protected
               bbot=bot
            elseif(abs(istk(l)).eq.12) then
c     .        unmark all 
               bbot=bot0
            else
               buf='Unknown option'
               call error(901)
            endif
         endif
         if(.not.cremat('predef',top,0,1,2,l,lc)) return
         stk(l)=isiz-bbots
         stk(l+1)=isiz-bbot
      endif
      return
      end
