
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine matpow
c     
c     matrix/vector entrywize power

      include 'stack.h'
c     
      double precision ddot,dasum,sr,si
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)+1
c
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c
      itr=max(it1,it2)
c
      if(mn1.eq.0) then
         return
      endif
      if(mn2.eq.0) then
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)
         return
      endif
      if(mn1.eq.1) then
c     scalar^matrix treated as scalar.^matrix (see matxpow)
         err=lw+mn2*2-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         if(it2.eq.0) then
            if(it1.eq.0) then
               call ddpow1(mn2,stk(l1),0,stk(l2),1,
     $              stk(lw),stk(lw+mn2),1,err,itr)
            else
               call wdpow1(mn2,stk(l1),stk(l1+mn1),0,stk(l2),1,
     $              stk(lw),stk(lw+mn2),1,err)
            endif
         else
            if(it1.eq.0) then
               call dwpow1(mn2,stk(l1),0,stk(l2),stk(l2+mn2),1,
     &              stk(lw),stk(lw+mn2),1,err)
            else
               call wwpow1(mn2,stk(l1),stk(l1+mn1),0,stk(l2),stk(l2
     $              +mn2),1,stk(lw),stk(lw+mn2),1,err)
            endif
         endif
         if(err.eq.1) then
            call error(30)
            return
         endif
         if(err.eq.2) then
            if(ieee.eq.0) then
               call error(27)
               return
            elseif(ieee.eq.1) then
               call msgs(63)
            endif
            err=0
         endif
         istk(il1+1)=m2
         istk(il1+2)=n2
         istk(il1+3)=itr
         call unsfdcopy(mn2*(itr+1),stk(lw),1,stk(l1),1)
         lstk(top+1)=l1+mn2*(itr+1)
         return
      endif
      if(mn2.gt.1) goto 39
      if(m1.ne.n1) then
         if(mn2.eq.1.and.(m1.eq.1.or.n1.eq.1)) then
c     .     vect^scalar treated as  vect.^scalar (see matxpow)
            sr=stk(l2)
            si=stk(l2+1)
            if(it1.eq.0) then
               err=l1+mn1-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
            endif
            if(it2.eq.0) then
               if(it1.eq.0) then
                  call ddpow1(mn1,stk(l1),1,sr,0,stk(l1),stk(l1+mn1),1,
     $                 err,itr)
               else
                  call wdpow1(mn1,stk(l1),stk(l1+mn1),1,sr,0,
     $                 stk(l1),stk(l1+mn1),1,err)
               endif
            else
               if(it1.eq.0) then
                  call dwpow1(mn1,stk(l1),1,sr,si,0,
     &                 stk(l1),stk(l1+mn1),1,err)
               else
                  call wwpow1(mn1,stk(l1),stk(l1+mn1),1,sr,si,0,
     $                 stk(l1),stk(l1+mn1),1,err)
               endif
            endif
            if(err.eq.1) then
               call error(30)
               return
            endif
            if(err.eq.2) then
               if(ieee.eq.0) then
                  call error(27)
                  return
               elseif(ieee.eq.1) then
                  call msgs(63)
               endif
               err=0
            endif
            istk(il1+3)=itr
            lstk(top+1)=l1+mn1*(itr+1)
            return
         endif
         err=1
         call error(20)
         return
      endif
      nexp = nint(stk(l2))

      if (it2 .ne. 0) go to 39
      if (stk(l2) .ne. dble(nexp)) go to 39
      if (nexp.eq.1) return
      if (nexp.eq.0) then
         lw=l1+mn1*(it1+1)
         ipvt=iadr(lw+m1*(it1+1))
         err=sadr(ipvt+m1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         if (dasum(m1*n1*(it1+1),stk(l1),1).eq.0.0d0) then
            call error(30)
            return
         endif
         call dset(mn1,0.0d+0,stk(l1),1)
         call dset(m1,1.0d+0,stk(l1),m1+1)
         istk(il1+3)=0
         lstk(top+1)=l1+mn1
         return
      endif
c     
      if (nexp.le.0) then
         rhs=1
         call intinv('pow')
         call putlhsvar()
c         call matlu
         if(err.gt.0.or.err1.gt.0) return
         nexp=-nexp
      endif
      l2=l1+mn1*(it1+1)
c     
      l3=l2+mn1*(itr+1)
      err=l3+n1*(itr+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      lstk(top+1)=l1+mn1*(itr+1)
      istk(il1+3)=itr
c     
      call unsfdcopy(mn1*(itr+1),stk(l1),1,stk(l2),1)
      if(it1.eq.1) goto 35
c     la matrice est reelle
      do 34 kexp=2,nexp
         do 33 j=1,n1
            ls=l1+(j-1)*n1
            call unsfdcopy(n1,stk(ls),1,stk(l3),1)
            do 32 i=1,n1
               ls=l2+(i-1)
               ll=l1+(i-1)+(j-1)*n1
               stk(ll)=ddot(n1,stk(ls),n1,stk(l3),1)
 32         continue
 33      continue
 34   continue
      return
c     
 35   continue
c     la matrice est complexe
      do 38 kexp=2,nexp
         do 37 j=1,n1
            ls=l1+(j-1)*n1
            call unsfdcopy(n1,stk(ls),1,stk(l3),1)
            call unsfdcopy(n1,stk(ls+mn1),1,stk(l3+n1),1)
            do 36 i=1,n1
               ls=l2+(i-1)
               ll=l1+(i-1)+(j-1)*n1
               stk(ll)=ddot(n1,stk(ls),n1,stk(l3),1)-
     $              ddot(n1,stk(ls+mn1),n1,stk(l3+n1),1)
               stk(ll+mn1)=ddot(n1,stk(ls),n1,stk(l3+n1),1)+
     $              ddot(n1,stk(ls+mn1),n1,stk(l3),1)
 36         continue
 37      continue
 38   continue
      return
c     
c     puissance non entiere ou non positive
 39   fun = 6
      fin = 28
      rhs=2
      top=top+1
      return
      end
c			================================================
     
