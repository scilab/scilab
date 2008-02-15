c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intclear(fname)
      include 'stack.h'
c     
      parameter (nz3=nsiz-3,nz2=nsiz-2)
      character*(*) fname
      integer top0,id(nsiz)
      logical getsmat,checkval,checklhs
      integer iadr, sadr
      integer a, blank,percen,sfin,helps(nsiz),scspal(nsiz)

      logical ok
      data a/10/,blank/40/,percen/56/
      data helps /353243448,673717273,nz2*673720360/
      data scspal/202509340,421796888,673715466,nz3*673720360/

      iadr(l)=l+l-1
c      sadr(l)=(l/2)+1
c
      if(.not.checklhs(fname,1,1)) return

      if(rhs.le.0) then
         if (macr.ne.0 .or. paus.ne.0) then
            if(rstk(pt-2).eq.909) then
c     .     clear within an exec'd macro
               goto 01
            endif
c     .     clear within a macro, an execstr, an exec or a pause
            k = lpt(1) - (13+nsiz)
            if(lin(k+7).ne.0.and.istk(lin(k+6)).eq.10) goto 02
c     .     clear within a macro, an exec or a pause
            bot = lin(k+5)
            goto 02
         endif

 01      continue
c     .  clear all variable

c     .  preserve %help and scicos_pal variables
         i1=bbot
         fin=-1
         call stackg(helps)
         if(err.gt.0) return
         if (fin.gt.0) i1=min(fin,i1)

         fin=-1
         call stackg(scspal)
         if(err.gt.0) return
         if (fin.gt.0) i1=min(fin,i1)
         bot = i1
         if(bot.eq.bbot) goto 02
         
         fin=0
         call stackg(helps)
         if(err.gt.0) return
         ih=fin
         fin=0
         call stackg(scspal)
         if(err.gt.0) return
         is=fin
         bot = bbot 
         if(is.eq.-1) then
            call stackp(scspal,0)
            if(err.gt.0) return
         endif
         if(ih.eq.-1) then
            call stackp(helps,0)
            if(err.gt.0) return
         endif
 02      top=top+1
         il = iadr(lstk(top))
         istk(il) = 0
         lstk(top+1) = lstk(top) + 1
         return
      endif
     
      top0=top
      do 10 k=1,rhs
         if(.not.getsmat(fname,top0,top,m,n,1,1,lr,nlr)) return
         if(.not.checkval(fname,m*n,1)) return
         if(nlr.eq.0) then
            top=top-1
            goto 10
         endif
c        . check for valid variable name
         do 05 i=0,nlr-1
            ic=abs(istk(lr+i))
            if((ic.gt.blank.and.(i.gt.0.and.ic.eq.percen)).or.
     $           (i.eq.0.and.ic.lt.a)) then
               err=rhs+1-k
               call error(248)
               return
            endif
 05      continue
         call namstr(id,istk(lr),nlr,0)
         il = iadr(lstk(top))
         istk(il) = 0
         lstk(top+1) = lstk(top) + 1
         rhs = 0
         call stackp(id,0)
         if (err .gt. 0.or.err1.gt.0) return
         fin = 1
 10   continue
      top=top+1
      il = iadr(lstk(top))
      istk(il) = 0
      lstk(top+1) = lstk(top) + 1
      end
      
