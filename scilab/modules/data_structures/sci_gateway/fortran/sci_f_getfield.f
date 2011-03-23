c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intgetfield()
      include 'stack.h'
      integer vol2,vol1
      integer strpos
      external strpos
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (rhs .lt. 1) then
         call error(39)
         return
      endif
      lw=lstk(top+1)
c
c extraction
   10 continue
      if(rhs.ne.2) then
         call error(39)
         return
      endif
c     arg2(arg1)
c     get arg2
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).lt.15.or.istk(il2).gt.17) then
         err=2
         call error(56)
         return
      endif
      m2=istk(il2+1)
      id2=il2+3
      l2=sadr(il2+m2+3)
c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      ilist=0
      nlist=0
      if(istk(il1).eq.15) then
         ill=il1
         nlist=m1
         ll=sadr(ill+3+nlist)
         ilist=1
         il1=iadr(ll+istk(ill+1+ilist)-1)
         if(istk(il1).lt.0) il1=iadr(istk(il1+1))
         m1=istk(il1+1)
      endif
c
 15   continue
c
      if(istk(il1).ne.10)  goto 20
c     .  arg2(arg1) with arg1 vector of strings
      ilt=iadr(sadr(il2+istk(il2+1)+3))
      nt=istk(ilt+1)*istk(ilt+2)
      if(nt.ne.1) goto 17
c     .     Soft coded extraction
      buf='Soft coded field names not yet implemented'
      call error(999)
      return

 17   continue
c     arg2(arg1) arg1:string, hard coded case
      if(istk(il1+1).gt.1.and.istk(il1+2).gt.1) then
         call error(21)
         return
      endif
      n1=istk(il1+1)*istk(il1+2)
      ili=iadr(lw)
      lw=sadr(ili+n1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     .  look for corresponding index
      l=il1+5+n1
      mx=0
      do 18 i=1,n1
         nl=istk(il1+4+i)-istk(il1+3+i)
         n=strpos(istk(ilt+5),nt-1,istk(ilt+5+nt),istk(l),nl)
         l=l+nl
         if(n.le.0) then
            call error(21)
            return
         endif
         n=n+1
         mx=max(mx,n)
         istk(ili-1+i)=n
 18   continue
      n=n1
c     end of arg1=string case
      goto 22
c     begining of standard case
 20   if(m1.eq.-1) then
c     .  arg2(:)
         l=lstk(top)
         if(ilist.ne.nlist) then
            if(m2.ne.1) then
               call error(21)
               return
            endif
         endif
         if(m2.ne.lhs) then
            call error(41)
            return
         endif
         if(top+1+m2.ge.bot) then
            call error(18)
            return
         endif
         do  21 i=1,m2
            vol1=istk(il2+2+i)-istk(il2+1+i)
            if(vol1.eq.0) then
               err=i
               call error(117)
               return
            endif
            lstk(top+1)=lstk(top)+vol1
            top=top+1
 21      continue
         top=top-1
         call unsfdcopy(istk(il2+m2+2)-1,stk(l2),1,stk(l),1)
         goto 99
      else
c     .  arg2(arg1) standard case
         call indxg(il1,m2,ili,n,mx,lw,1)
         if(err.gt.0) return
      endif
c
 22   if(mx.gt.m2) then
         call error(21)
         return
      endif
c
      if(ilist.ge.nlist) goto 31
      if(n.gt.1) then
         call error(21)
         return
      endif
      lt2=sadr(il2+3+m2)+istk(il2+1+istk(ili))-1
      ll2=istk(il2+2+istk(ili))-istk(il2+1+istk(ili))
      il2=iadr(lt2)
 23   ilist=ilist+1
      il1=iadr(ll+istk(ill+1+ilist)-1)
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)

      if(istk(il2).eq.15.or.istk(il2).eq.16) goto 26
      call error(21)
      return

 26   continue
      m2=istk(il2+1)
      l2=sadr(il2+m2+3)
      goto 15
c

 31   if(n.ne.lhs) then
         call error(41)
         return
      endif
c
      if(top+n+1.ge.bot) then
         call error(18)
         return
      endif
      do  32 i=1,n
         k=istk(ili-1+i)
         vol2=istk(il2+2+k)-istk(il2+1+k)
         if(vol2.eq.0) then
            err=k
            call error(117)
            return
         endif
         lstk(top+1)=lstk(top)+vol2
         top=top+1
 32   continue
      top=top-1
c
      ill=iadr(max(lw,lstk(top+1)))
      lw=sadr(ill+n)
      err=sadr(ill+n)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      do 34 i=1,n
         k=istk(ili-1+i)
         istk(ill-1+i)=istk(il2+1+k)+l2-1
 34   continue
c
      do 35 i=1,n
         k=top-n+i
         call unsfdcopy(lstk(k+1)-lstk(k),stk(istk(ill-1+i)),1,
     $        stk(lstk(k)),1)
 35   continue
      goto 99

 99   return
      end
