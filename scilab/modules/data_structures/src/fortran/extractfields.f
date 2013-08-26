c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine extractfields(illist,ind,n,lw)
c
c     given a list beginning at istk(illist), a vector of n indexes ind
c     extractfields creates n variables corresponding to the n fields
c
c     if n==-1 all fieds are extracted
c

      include 'stack.h'
      integer ind(*)
c
      integer vol2
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(n.eq.-1) then
c     .  L(:)
         n=istk(illist+1)
         llist=sadr(illist+3+n)
c     .  Check lhs
         if(lhs.le.1) then
            if(n.ne.lhs) then
               call forcerhs(n,ierr)
               if(ierr.ne.0) then
                  call error(41)
                  return
               endif
            endif
         else
            if(n.ne.lhs) then
               call error(41)
               return
            endif
         endif
c     .  check if it is possible to create n new variables
         if(top+1+n.ge.bot) then
            call error(18)
            return
         endif
c     .  create table of pointers on new variables
         do  31 i=1,n
            vol2=istk(illist+2+i)-istk(illist+1+i)
            if(vol2.eq.0) then
               err=i
               call error(117)
               return
            endif
            lstk(top+1)=lstk(top)+vol2
            top=top+1
 31      continue
         top=top-1
c     .  copy the fields
         l=lstk(top-n+1)
         call unsfdcopy(istk(illist+n+2)-1,stk(llist),1,stk(l),1)
      else
c     .  L([....])
         llist=sadr(illist+3+istk(illist+1))
         if(n.ne.lhs) then
            call forcerhs(n,ierr)
            if(ierr.ne.0) then
               call error(41)
               return
            endif
         endif
c     .  check if it is possible to create n new variables
         if(top+n+1.ge.bot) then
            call error(18)
            return
         endif
c     .  create table of pointers on new variables
         do  52 i=1,n
            k=ind(i)
            if(k.le.0.or.k.gt.istk(illist+1)) then
               call error(21)
               return
            endif
            vol2=istk(illist+2+k)-istk(illist+1+k)
            if(vol2.eq.0) then
               err=k
               call error(117)
               return
            endif
            lstk(top+1)=lstk(top)+vol2
            top=top+1
 52      continue
         top=top-1

c     .  preserve fields adress (newly created variables may
c     .  overwrite beginining of the given list)
         ill=iadr(max(lw,lstk(top+1)))
         lw=sadr(ill+n)
         err=sadr(ill+n)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif

         do 54 i=1,n
            k=ind(i)
            istk(ill-1+i)=istk(illist+1+k)+llist-1
 54      continue
c     .  copy the fields
         do 55 i=1,n
            k=top-n+i
            call unsfdcopy(lstk(k+1)-lstk(k),stk(istk(ill-1+i)),1,
     $           stk(lstk(k)),1)
 55      continue
      endif
      return
      end
