c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intisequal(id)
c     Interface for isequal:

      implicit none

      INCLUDE 'stack.h'

      integer id(nsiz)
      integer typ,m,n,l,il,il1,ilk,k,topk,top1,srhs,k1,it

c     EXTERNAL API FUNCTIONS
      logical  checkrhs, checklhs
      external checkrhs, checklhs
      character*7 fname
      integer iadr,sadr
      integer equal
      data equal/50/
c     TEXT
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      fname = 'isequal'

      topk=top
      top1=top-rhs+1
      rhs=max(0,rhs)

      if (.not.checkrhs(fname,2,2000000)) return
      if (.not.checklhs(fname,1,1)) return
c first check the types

      il1=iadr(lstk(top1))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      typ=istk(il1)

      do 10 k=1,rhs-1
         ilk=iadr(lstk(top1+k))
         if(istk(ilk).lt.0) ilk=iadr(istk(ilk+1))
         if (istk(ilk).ne.typ) goto 60
 10   continue
c
      if (typ.ge.15.and.typ.le.17) then
         call setfunnam(ids(1,pt+1),'%l_isequal',10)
         fun=-1
         return
      endif

      if(typ.gt.14) then
         call funnam(ids(1,pt+1),'isequal',il1)
         fun=-1
         return
      endif


c first check the dimensions
      il=iadr(lstk(top-rhs+1))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      m=istk(il+1)
      n=istk(il+2)
      do 20 k=2,rhs
         il=iadr(lstk(top-rhs+k))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(m.ne.istk(il+1).or.n.ne.istk(il+2)) goto 60
 20   continue

      if(typ.eq.1.or.typ.eq.2.or.typ.eq.8.or.typ.eq.5) then
c     checking sub_type
         il=iadr(lstk(top-rhs+1))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         it=istk(il+3)
         do 21 k=2,rhs
            il=iadr(lstk(top-rhs+k))
            if(istk(il).lt.0) il=iadr(istk(il+1))
            if(it.ne.istk(il+3)) goto 60
 21      continue
      endif


      srhs=rhs

 30   do 40 k=2,srhs
         call createref(iadr(lstk(top1)),top1,lstk(top1+1)-lstk(top1))
         topk=top1+k-1
         call createref(iadr(lstk(topk)),topk,lstk(topk+1)-lstk(topk))
         fin=equal
         rhs=2
         call allops()
         if(err.gt.0.or.err1.gt.0) return
         if(icall.ne.0) then
c     should not happen
            rhs=srhs
            top=top1-1+rhs
            call funnam(ids(1,pt+1),'isequal',iadr(lstk(top-rhs+1)))
            fun=-1
            return
         endif
c     
         il=iadr(lstk(top))
         do 35  k1=1,istk(il+1)*istk(il+2)
            if(istk(il+2+k1).eq.0) goto 60
 35      continue

         top=top-1
 40   continue
c variables are equal
      top=top1
      il=iadr(lstk(top))
      istk(il)=4
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=1
      lstk(top+1)=sadr(il+4)
      return
c variables are different
 60   continue
      top=top1
      il=iadr(lstk(top))
      istk(il)=4
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      lstk(top+1)=sadr(il+4)
      return


      end
c     -------------------------------
      
      
