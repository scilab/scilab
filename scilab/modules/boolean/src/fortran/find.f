c     ==================================================
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     ==================================================
      subroutine intsfind(nmax)
c     find of a full standard or boolean matrix
      include 'stack.h'

c
      logical ref
      integer nmax
      integer sadr,iadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)
c

      
      il1=iadr(lstk(top))
      ilr=il1
      if(nmax.eq.0) then
         nt=nmax
         goto 17
      endif
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1

      if(istk(il1).eq.1) then
c     argument is a standard matrix
         m1=istk(il1+1)
         mn1=istk(il1+1)*istk(il1+2)
         it1=istk(il1+3)
         if(it1.ne.0) then
            call putfunnam('find',top)
            if(nmax.ne.-1) top=top+1
            fun=-1
            return
         endif
         l1=sadr(il1+4)
         if(ref) then
            err=sadr(ilr+4)+mn1-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(4,istk(il1),1,istk(ilr),1)
         endif
         lr=sadr(ilr+4)
         l=lr
         if(mn1.gt.0) then
            if (nmax.lt.0) then
c     .     get all the occurences
               do 11 k=0,mn1-1
                  if(stk(l1+k).ne.0.0d0) then
                     stk(l)=float(k+1)
                     l=l+1
                  endif
 11            continue
            else
c     .     get at most nmax occurences
               do 12 k=0,mn1-1
                  if(stk(l1+k).ne.0.0d0) then
                     stk(l)=float(k+1)
                     l=l+1
                     if(l-lr.ge.nmax) goto 13
                  endif
 12            continue
            endif
 13         nt=l-lr
         else
            nt=0
         endif
      elseif(istk(il1).eq.4) then
c     argument is a full boolean matrix
         m1=istk(il1+1)
         mn1=istk(il1+1)*istk(il1+2)
         if(.not.ref) then
            il=max(il1+3+mn1,iadr(lstk(top)+mn1*lhs)+8)
            err=sadr(il+mn1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(mn1,istk(il1+3),1,istk(il),1)
         else
            il=il1+3
         endif
         istk(ilr)=1
         lr=sadr(ilr+4)
         if(mn1.gt.0) then
            l=lr
            if(nmax.lt.0) then
c     .     get all occurrences
               do 14 k=0,mn1-1
                  if(istk(il+k).ne.1) goto 14
                  stk(l)=float(k+1)
                  l=l+1
 14            continue
            else
c     .     get at most nmax occurences
               do 15 k=0,mn1-1
                  if(istk(il+k).ne.1) goto 15
                  stk(l)=float(k+1)
                  l=l+1
                  if(l-lr.ge.nmax) goto 16
 15            continue
            endif
 16         nt=l-lr
         else
            nt=0
         endif
      endif
 17   istk(ilr)=1
      istk(ilr+1)=min(1,nt)
      istk(ilr+2)=nt
      istk(ilr+3)=0
      lstk(top+1)=lr+nt
      if(lhs.eq.1) goto 999
      top=top+1
      il2=iadr(lstk(top))
      istk(il2)=1
      istk(il2+1)=min(1,nt)
      istk(il2+2)=nt
      istk(il2+3)=0
      l2=sadr(il2+4)
      lstk(top+1)=l2+nt
      if(nt.eq.0) goto 999
      do 18 k=0,nt-1
         stk(l2+k)=float(int((stk(lr+k)-1.0d0)/m1)+1)
         stk(lr+k)=stk(lr+k)-(stk(l2+k)-1.0d+0)*m1
 18   continue
      goto 999
c
  999 return
      end

      subroutine intspfind(nmax)
      include 'stack.h'

      logical ref
      double precision temp
      integer sadr,iadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)


      il1=iadr(lstk(top))
      ilr=il1
      if(nmax.eq.0) then
         nt=nmax
         goto 17
      endif
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1

c     sparse matrix find
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      if(it1.ne.0) then
         call putfunnam('find',top)
         if(nmax.ne.-1) top=top+1
         fun=-1
         return
      endif
      nel1=istk(il1+4)
      if(nel1.eq.0) then
         nt=0
         lr=sadr(ilr+4)
         goto 17
      endif
c
      if(.not.ref) then
         lr=lw
      else
         lr=sadr(ilr+4)
      endif
      err=lr+nel1-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      li=il1+5
      lj=li+m1

      l=lr
      ip=lj

      do 10 i=0,m1-1
         ni=istk(li+i)
         if(ni.ne.0) then
            do 01 ii=0,ni-1
               stk(l+ii)=(i+1)+(istk(ip+ii)-1)*m1
 01         continue
            l=l+ni
            ip=ip+ni
         endif
 10   continue

c     order the index column wise
      call dsort(stk(lr),nel1,istk(iadr(lr+nel1)))
      do 11 i=1,int(nel1/2)
         temp=stk(lr-1+i)
         stk(lr-1+i)=stk(lr+nel1-i)
         stk(lr+nel1-i)=temp
 11   continue

      nt=nel1
      if(nmax.ge.0) nt=min(nel1,nmax)

      if(.not.ref) then
         l=sadr(il1+4)
         call dcopy(nt,stk(lr),1,stk(l),1)
         lr=l
      endif

      
 17   istk(ilr)=1
      istk(ilr+1)=min(1,nt)
      istk(ilr+2)=nt
      istk(ilr+3)=0
      lstk(top+1)=lr+nt
      if(lhs.eq.1) return
      top=top+1
      il2=iadr(lstk(top))
      istk(il2)=1
      istk(il2+1)=min(1,nt)
      istk(il2+2)=nt
      istk(il2+3)=0
      l2=sadr(il2+4)
      lstk(top+1)=l2+nt
      if(nt.eq.0) return
      do 18 k=0,nt-1
         stk(l2+k)=float(int((stk(lr+k)-1.0d0)/m1)+1)
         stk(lr+k)=stk(lr+k)-(stk(l2+k)-1.0d+0)*m1
 18   continue

      return
      end
c     ==================================================      
