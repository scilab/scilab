c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intcomp
      include 'stack.h'
      integer cmode,topk
      logical checkrhs,checklhs,getscalar
      integer iadr,sadr
      integer semi
      data semi/43/
c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c

      call ref2val
      if(rstk(pt).eq.901) goto 61
c
      rhs=max(0,rhs)
      if(.not.checklhs('comp',1,1)) return
      if(.not.checkrhs('comp',1,2)) return
      topk=top
c
      if(rhs.eq.2) then
c     .  get compilation option
         if(.not.getscalar('debug',topk,top,l)) return
         cmode=stk(l)
         if(cmode.ne.0.and.cmode.ne.1.and.cmode.ne.2) then
            err=2
            call error(42)
            return
         endif
         top=top-1
         rhs=1
      else
         cmode=0
      endif

      il=iadr(lstk(top))

c

      if(istk(il).eq.13) then
c     .  function is already compiled
         call msgs(80,0)
         goto 72
      endif
      if(istk(il).ne.11) then
         err=1
         call error(44)
         return
      endif
c
      l=il+1
      mlhs=istk(l)
      l=l+nsiz*mlhs+1
      mrhs=istk(l)
      l = l + nsiz*mrhs + 2
      pt=pt+1
      ids(1,pt)=l
      ids(2,pt)=sym
      ids(3,pt)=char1

      pstk(pt)=fin
      fin=lstk(top)
      sym=semi
      comp(1)=iadr(lstk(top+1))
      comp(2)=0
      comp(3)=cmode
      rstk(pt)=901
      icall=5
c     *call* parse  macro
      return
 61   l=ids(1,pt)
      sym=ids(2,pt)
      char1=ids(3,pt)
      pt=pt-1
      if(err1.ne.0) then
         comp(3)=0
         comp(2)=0
         comp(1)=0
         if (rstk(pt).eq.904) then
            top=toperr
            return
         endif
         il=iadr(lstk(top))
         istk(il)=0
         lhs=0
         err2=err1
         if(errct.eq.0) then
            err1=0
         else
            top=top-1
         endif
         return
      endif
      il=iadr(lstk(top))
      il1=iadr(lstk(top+1))
      n=comp(2)-il1
      comp(2)=0
      comp(3)=0
      call icopy(n,istk(il1),1,istk(l),1)
      istk(l-1)=n
      lstk(top+1)=sadr(l+n)
      istk(il)=13
 72   rhs=0
      call stackp(idstk(1,top),0)
      if(err.gt.0.or.err1.gt.0) goto 999
      top=top+1
      call objvide('comp',top)
      lhs=0
c     
 999  return
      end
      
