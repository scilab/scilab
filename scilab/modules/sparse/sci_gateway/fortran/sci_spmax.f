c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intspmax(fname,id)
      include 'stack.h'
      integer id(nsiz),top0
      character*(*) fname
      double precision tv
      integer iadr, sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      if (rhs .lt. 1) then
         call error(39)
         return
      endif

      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs.eq.1) then
         if (lhs .gt.2) then
            call error(41)
            return
         endif
         il1 = iadr(lstk(top))
         if (istk(il1) .ne. 5) then
            err=1
            call error(217)
            return
         endif
         m1=istk(il1+1)
         n1=istk(il1+2)
         it1=istk(il1+3)
         nel1=istk(il1+4)
         irc1=il1+5
         l1=sadr(irc1+m1+nel1)
         if(it1.ne.0) then
            err=1
            call error(52)
            return
         endif
         tv=0.0d0
         if(nel1.gt.0) then
            tv=stk(l1)
            im=0
            if(fin.eq.10) then
               do 60 i=0,nel1-1
                  if (stk(l1+i).gt.tv) then
                     tv=stk(l1+i)
                     im=i
                  endif
 60            continue
               if(tv.lt.0.0d0.and.nel1.lt.m1*n1) tv=0.0d0
            else
               do 61 i=0,nel1-1
                  if (stk(l1+i).lt.tv) then
                     tv=stk(l1+i)
                     im=i
                  endif
 61            continue
               if(tv.gt.0.0d0.and.nel1.lt.m1*n1) tv=0.0d0
            endif
         endif
c
         if(lhs.eq.2) then
            if (nel1.ne.0) then
               jm=istk(irc1+m1+im)
               im=im+1
               ii=0
               do 62 i=0,m1-1
                  ii=ii+istk(irc1+i)
                  if(ii.ge.im) goto 63
 62            continue
 63            im=i+1
            endif
         endif
c
         istk(il1)=1
         istk(il1+1)=1
         istk(il1+2)=1
         istk(il1+3)=0
         l=sadr(il1+4)
         stk(l)=tv
         lstk(top+1)=l+1
         if(lhs.eq.2) then
            top=top+1
            il2=iadr(lstk(top))
            err=lstk(top)+4-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            istk(il2)=1
            l=sadr(il2+4)
            istk(il2+3)=0
            if (nel1.ne.0) then
               istk(il2+1)=1
               istk(il2+2)=1
               stk(l)=im+(jm-1)*m1
               lstk(top+1)=l+1
            else
               istk(il2+1)=0
               istk(il2+2)=0
               lstk(top+1)=l+1
            endif
         endif
         return
      endif
c     
c     checking variable sp2 (number 2)
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      do 65 i=1,rhs-1
         il2 = iadr(lstk(top))
         if (istk(il2) .ne. 5) then
            err=2
            call error(217)
            return
         endif

         m2=istk(il2+1)
         n2=istk(il2+2)
         it2=istk(il2+3)
         nel2=istk(il2+4)
         irc2=il2+5
         if(it2.ne.0) then
            err=2
            call error(52)
            return
         endif
         l2=sadr(irc2+m2+nel2)
c     
c     checking variable sp1 (number 1)
c     
         top=top-1
         il1 = iadr(lstk(top))
         if (istk(il1) .ne. 5) then
            err=1
            call error(217)
            return
         endif
         m1=istk(il1+1)
         n1=istk(il1+2)
         it1=istk(il1+3)
         nel1=istk(il1+4)
         irc1=il1+5
         l1=sadr(irc1+m1+nel1)

         if(it1.ne.0) then
            err=1
            call error(52)
            return
         endif
         if(m1.ne.m2.or.n1.ne.n2) then
            call error(60)
            return
         endif
         irc=iadr(lw)
         nelmx=(iadr(lstk(bot))-irc-m1-10)/3
         lc=sadr(irc+m1+nelmx)
         lw=lc+nelmx
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif  
         nel=nelmx
         if(fname.eq.'max') then
            call dspmax(m1,n1,stk(l1),nel1,istk(irc1),stk(l2),nel2,
     $           istk(irc2),stk(lc),nel,istk(irc),ierr)
         else
            call dspmin(m1,n1,stk(l1),nel1,istk(irc1),stk(l2),nel2,
     $           istk(irc2),stk(lc),nel,istk(irc),ierr)
         endif
         if(ierr.ne.0) then
            call error(17)
            return
         endif
         istk(il1+3)=0
         istk(il1+4)=nel
         call icopy(m1+nel,istk(irc),1,istk(irc1),1)
         l1=sadr(irc1+m1+nel)
         call unsfdcopy(nel,stk(lc),1,stk(l1),1)
         lstk(top+1)=l1+nel
 65   continue
      return
      end
c			======================================
