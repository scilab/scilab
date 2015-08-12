c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intpoly(id)

      INCLUDE 'stack.h'
      integer id(nsiz)
      integer iadr, sadr, w
      integer blank,racine,coeff
      logical roots,ref
      integer r(5)
      integer c(5)
      integer size
      character*256 tmpmsg
c
      data blank/40/,racine/27/,coeff/12/
      data r/27, 24, 24, 29, 28/
      data c/12, 24, 14, 15, 15/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)
      w = 0
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.lt.1.or.rhs.gt.3) then
         call error(42)
         return
      endif
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).ne.1) then
         fun=-1
         call funnam(ids(1,pt+1),'poly',il1)
         return
      endif
      ref=il1.ne.ilr
c
      roots=.true.

      if(rhs.eq.3) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         rhs=rhs-1
         top=top-1
         if(istk(il).ne.10) then
            err=rhs
            call error(55)
            return
         endif
         size = istk(il+5)-1
         if (size.ne.5 .and. size.ne.1) then
            w = 1
         endif
         il=il+5+istk(il+1)*istk(il+2)
         if(abs(istk(il)).ne.racine) then
            roots=.false.
            if(abs(istk(il)).ne.coeff) then
               err=rhs
               call error(36)
               return
            endif
         endif
         if(w.eq.0) then
            do 22 i=1,size
                if(c(i).ne.abs(istk(il+i-1)).and.
     $             r(i).ne.abs(istk(il+i-1))) then
                    w = 1
                endif
 22         continue
         endif
       else
        roots=.true.
      endif

      if(w.eq.1) then
        call msgstxt('Warning:')
        call msgstxt('This usage of 3rd argument of poly is obsolete.')
        call msgstxt('It will be more strict in scilab 6.0.0.')
        call msgstxt('Please use roots, coeff, r or c instead.')
      endif

c     formal variable
      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).ne.10) then
         err=2
         call error(55)
         return
      endif
      if(istk(il+1)*istk(il+2).ne.1) then
         err=2
         call error(89)
         return
      endif
      n=istk(il+5)-1
      if(n.gt.4) call msgs(115,0)
      if (abs(istk(il+5+1)).lt.10) then
         err=2
         call error(248)
         return
      endif
      do 12 i=1,4
         id(i)=blank
         if(i.le.n) then
            if (abs(istk(il+5+i)).ge.blank) then
               err=2
               call error(248)
               return
            endif
            id(i)=istk(il+5+i)
         else
c     .     fill with space
            id(i)=blank
         endif
 12   continue

c
c     first argument
      top=top-1
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
      itr=it1

      if(.not.roots) goto 17
      if(mn1.eq.1.or.m1.ne.n1) goto 14
c     polynome caracteristique,decomposition spectrale de la matrice

      if(ref) then
c     necessary while matdsr and matdsc cannot be called by reference
         lr=sadr(ilr+4)
         err=lr+mn1*(it1+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il1),1,istk(ilr),1)
         call unsfdcopy(mn1*(it1+1),stk(l1),1,stk(lr),1)
         lstk(top+1)=lr+mn1*(it1+1)
      endif
      rhs=1
      call intspec('spec')
      call putlhsvar
      if(err.gt.0) return
      il1=iadr(lstk(top))
      l1=sadr(il1+4)
      mn1=m1
      m1=1
      n1=mn1
      itr=min(it1,istk(il1+3))
      it1=istk(il1+3)
      lw=l1+mn1*(it1+1)
      ref=.false.
      ilr=il1
      goto 14
c
c     polynome defini par ses racines
 14   ild=ilr+8
      lc=sadr(ild+2)
      lr=lc+(mn1+1)*(it1+1)
      if(ref) then
         err=lr-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      else
         err=lr+mn1*(it1+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(mn1*(it1+1),stk(l1),1,stk(lr),1)
         l1=lr
      endif
      istk(ilr)=2
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=itr
      call icopy(4,id,1,istk(ilr+4),1)
      istk(ild)=1
      istk(ild+1)=mn1+2
      lstk(top+1)=lc+(mn1+1)*(itr+1)

      if(it1.eq.0) then
         call dprxc(mn1,stk(l1),stk(lc))
      else
         call wprxc(mn1,stk(l1),stk(l1+mn1),stk(lc),stk(lc+mn1+1))
         do 16 i=1,mn1+1
            l=lc-1+i
            if( stk(l)+stk(l+mn1+1).ne.stk(l)) goto 999
 16      continue
         istk(ilr+3)=0
         lstk(top+1)=lc+mn1+1
      endif
      goto 999
c
c     polynome defini par ses coefficients
 17   if(istk(il1).ne.1) then
         err=1
         call error(53)
         return
      endif
      if(mn1.eq.0) return

      if(n1.ne.1.and.m1.ne.1) then
         err=1
         call error(89)
         return
      endif

      lr=sadr(ilr+10)
      err=lr+mn1*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call unsfdcopy(mn1*(it1+1),stk(l1),-1,stk(lr),-1)
      istk(ilr)=2
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=it1
      call icopy(4,id,1,istk(ilr+4),1)
      istk(ilr+8)=1
      istk(ilr+9)=1+mn1
      lstk(top+1)=lr+mn1*(it1+1)
      goto 999
c
 999  return
      end
c			=======================================
