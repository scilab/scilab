
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine matrdiv
c     
c     matrix/vector right division

      include 'stack.h'
c     
      double precision sr,si
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
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
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c
      itr=max(it1,it2)
c
      if (mn1.eq.0.or.mn2.eq.0) then
c     .  a/[] or []/a
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
      if (mn2 .ne. 1) then
         if(m1.lt.0) then
            call error(14)
            return
         endif
         top = top+1
         rhs = 2        
         call intslash('slash')
         if (fin.ge.0) call putlhsvar()
c         if (m2 .eq. n2) fun = 1
c         if (m2 .ne. n2) fun = 4
c         fin = -1
      else
c     .  vector / cst
         if(m2.lt.0.and.mn1.ne.1) then 
            call error(14)
            return
         endif
         istk(il1+1)=m1
         istk(il1+2)=n1
         istk(il1+3)=itr
         lstk(top+1)=l1+mn1*(itr+1)
c     
         err=lstk(top+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     real / real
            call ddrdiv(stk(l1),1,stk(l2),0,stk(l1),1,mn1,ierr)
         elseif(it21.eq.1) then
c     .     real / complex
            sr=stk(l2)
            si=stk(l2+1)
            call dwrdiv(stk(l1),1,sr,si,0,stk(l1),stk(l1+mn1),1,
     $           mn1,ierr)
         elseif(it21.eq.2) then 
c     .     complex / real
            call wdrdiv(stk(l1),stk(l1+mn1),1,stk(l2),0,stk(l1)
     $           ,stk(l1+mn1),1,mn1,ierr)
         elseif(it21.eq.3) then  
c     .     complex / complex
            call wwrdiv(stk(l1),stk(l1+mn1),1,stk(l2),stk(l2+1)
     $           ,0,stk(l1),stk(l1+mn1),1,mn1,ierr)
         endif
         if(ierr.ne.0) then
            if(ieee.eq.0) then
               call error(27)
               return
            elseif(ieee.eq.1) then
               call msgs(63)
            endif
         endif
      endif
      return
      end
c			================================================
