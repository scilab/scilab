c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intformat

      include 'stack.h'
      logical checkrhs,checklhs,getscalar,getsmat,cremat
      integer topk,fe,fv
      integer iadr
c
      data fe/14/,fv/31/
c
      iadr(l)=l+l-1
c

      rhs=max(0,rhs)
      topk=top
c
      if(.not.checklhs('format',1,1)) return
      if(.not.checkrhs('format',0,2)) return
 
      if(rhs.eq.0) then
c     .  return current format definition
         top=top+1
         if(.not.cremat('format',top,0,1,2,l,lc)) return
         stk(l)=lct(6)
         stk(l+1)=lct(7)
         return
      endif

 160  il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))

      if(istk(il).eq.1) then
c     .  nbre de digits
         if(.not.getscalar('format',topk,top,l)) return
         nd=min(max(2,int(stk(l))),25)
         if(lct(6).eq.0) nd=max(nd,8)
         lct(7)=nd
      elseif(istk(il).eq.10) then
c     .  type du format
         if(.not.getsmat('format',topk,top,m,n,1,1,l,n1)) return
         if(abs(istk(l)).eq.fe) then
            lct(6)=0
            lct(7)=max(8,lct(7))
         else
            if(abs(istk(l)).ne.fv) then
               err=1
               call error(36)
               return
            endif
            lct(6)=1
         endif
      else
         err=1
         call error(55)
         return
      endif

      if(rhs.eq.2) then
         top=top-1
         rhs=rhs-1
         goto 160
      endif
      call objvide('format',top)

      return
      end
