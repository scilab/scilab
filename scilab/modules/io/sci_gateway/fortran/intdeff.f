c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intdeff

      INCLUDE 'stack.h'
c     
      integer top0
      integer nocomp,profile

      logical cremat
      integer iadr,sadr
      character *8 caller

      save icomp
c     
      data nocomp/23/,profile/25/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c      
      icomp=1
      caller='deff'
      if(rhs.eq.3) then
         ilc=iadr(lstk(top))
         if(istk(ilc).eq.10) then
            if(istk(ilc+5+istk(ilc+1)*istk(ilc+2)).eq.nocomp) then
               icomp=0
            elseif(istk(ilc+5+istk(ilc+1)*istk(ilc+2)).eq.profile) then
               icomp=2
            endif
         else
            caller='function'
         endif
         rhs=rhs-1
         top=top-1
      endif
      if(rhs.ne.2) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif
      top0=top-rhs+1      
      il=iadr(lstk(top))
      nlines=1
      call getfun(0,nlines,caller)
      if(err.gt.0.or.err1.gt.0) goto 999
      if(icomp.ne.0) then
         call unsfdcopy(lstk(top+1)-lstk(top),stk(lstk(top)),1,
     $       stk(lstk(top0)),1) 
         lstk(top0+1)=lstk(top0)+lstk(top+1)-lstk(top)
         call putid(idstk(1,top0),idstk(1,top))
         top=top0
         if(icomp.eq.2) then
            top=top+1
            if (.not.cremat('getf',top,0,1,1,lr1,lc1)) return
            stk(lr1)=2
            rhs=2
         else
            rhs=1
         endif
         lhs=1
c     *call* comp
         fun=13
         fin=6
      else
         call stackp(idstk(1,top),0)
         top=top-1
         call objvide('deff',top)
         fun=0
      endif

 999  return
      end
