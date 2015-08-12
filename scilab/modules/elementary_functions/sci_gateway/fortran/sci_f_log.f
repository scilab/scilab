c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intlog(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

      double precision sr,si
      integer iadr,sadr
      logical allpositive
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs .ne. 1) then
         call error(42)
         return
      endif

      il=iadr(lstk(top))

      il1=il
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).ne.1) then
         call funnam(ids(1,pt+1),'log',il1)
         fun=-1
         return
      endif

      if(istk(il).lt.0) then
c     argument is passed by reference
         ilr=il
         il=iadr(istk(il+1))
         mn=istk(il+1)*istk(il+2)
         it=istk(il+3)
         l=sadr(il+4)
         lr=sadr(ilr+4)
         err=lr+mn*(it+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il),1,istk(ilr),1)
         lstk(top+1)=lr+mn*(it+1)
      else
         mn=istk(il+1)*istk(il+2)
         it=istk(il+3)
         l=sadr(il+4)
         lr=l
         ilr=il
      endif
      
      if(mn.eq.0) return

      if(it.eq.0) then
c Argument is a real matrix
c Loop over the elements of the matrix.
c Set itr to 1 if one value is negative :
c that means that x < 0 so that log(x) is a complex number.
c If all values are >= 0, let itr = 0.
c The logical allpositive is : true if one value is zero
c and ieee=1. That allows to display only one error message,
c even if the matrix contains more than one entry.
c 
         itr=0
         allpositive = .true.
         do 10 i=0,mn-1
            if(stk(l+i).lt.0.0d+0) then
               itr=1
c               goto 20
            elseif(stk(l+i).eq.0.0d+0) then
               if(ieee.eq.0) then
                  call error(32)
                  return
               elseif(ieee.eq.1) then
                  if (allpositive) then
                    call msgs(64)
                    allpositive = .false.
                  endif
               endif
c               goto 20
            endif
 10      continue

 20      if(itr.eq.0) then
c     .     argument is a real positive matrix with entries >= 0
            do 193 i=0,mn-1
               stk(lr+i)=log(stk(l+i))
 193        continue
         else
c     .     argument is a real matrix with  at least one entry < 0     
            err=lr+2*mn-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            lstk(top+1)=lr+2*mn
            do 194 i=0,mn-1
               call wlog(stk(l+i),0.0d+0,stk(lr+i),stk(lr+mn+i))
 194        continue
            istk(ilr+3)=itr
         endif
      else
c     argument is a complex matrix
         do 195 i=0,mn-1
            sr=stk(l+i)
            si=stk(l+mn+i)
            if(sr.eq.0d0 .and. si.eq.0.0d+0) then  
               if(ieee.eq.0) then
                  call error(32)
                  return
               elseif(ieee.eq.1) then
                  call msgs(64)
               endif
            endif
            call wlog(sr,si,stk(lr+i),stk(lr+i+mn))
 195     continue
      endif
      return
      end

c     -------------------------------
