c     ==================================================

c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

c     ==================================================
      subroutine intsbool2s
      include 'stack.h'

      logical ref
      integer sadr,iadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)
c

   10 if(rhs.ne.1) then
         call error(39)
         return
      endif
      if(lhs.ne.1) then
         call error(39)
         return
      endif

      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1
      mn1=istk(il1+1)*istk(il1+2)
      if (mn1.eq.0) return

      if(istk(il1).eq.4) then

c     argument is a full boolean matrix
         lr=sadr(ilr+4)
         err=lr+mn1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif

         do 13 k=mn1-1,0,-1
            stk(lr+k)=istk(il1+3+k)
 13      continue
         istk(ilr)=1
         istk(ilr+1)=istk(il1+1)
         istk(ilr+2)=istk(il1+2)
         istk(ilr+3)=0
         lstk(top+1)=lr+mn1
      elseif(istk(il1).eq.6) then
c     argument is a sparse boolean matrix
         m1=istk(il1+1)
         n1=istk(il1+2)
         nel1=istk(il1+4)
c   
         if(ref) then
            err=sadr(ilr+5+m1+nel1)+nel1-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(m1+nel1,istk(il1+5),1,istk(ilr+5),1)
         endif
         lj=sadr(ilr+5+m1+nel1)
         call dset(nel1,1.0d0,stk(lj),1)
         istk(ilr)=5
         istk(ilr+1)=istk(il1+1)
         istk(ilr+2)=istk(il1+2)
         istk(ilr+3)=0
         istk(ilr+4)=nel1
         lstk(top+1)=lj+nel1
      elseif(istk(il1).eq.1) then
         if(istk(il1+3).ne.0) then
            call putfunnam('bool2s',top)
            fun=-1
            return
         endif
         
         if(mn1.eq.0) then
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=lr
         else
            l1=sadr(il1+4)
            lr=sadr(ilr+4)
            if(ref) then
               err=lr+mn1-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
            endif
            do 20 k=mn1-1,0,-1
               if(stk(l1+k).ne.0.0d0) then
                  stk(lr+k)=1.0d0
               else
                  stk(lr+k)=0.0d0
               endif
 20         continue
            istk(ilr)=1
            istk(ilr+1)=istk(il1+1)
            istk(ilr+2)=istk(il1+2)
            istk(ilr+3)=0
            lstk(top+1)=lr+mn1
         endif
      elseif(istk(il1).eq.5) then
c     argument is a sparse matrix
         m1=istk(il1+1)
         n1=istk(il1+2)
         nel1=istk(il1+4)
         if(istk(il1+3).ne.0) then
            call putfunnam('bool2s',top)
            fun=-1
            return
         endif
c
         if(ref) then
            err=sadr(ilr+5+m1+nel1)+nel1-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(m1+nel1,istk(il1+5),1,istk(ilr+5),1)
         endif
         lj=sadr(ilr+5+m1+nel1)
         call dset(nel1,1.0d0,stk(lj),1)
         istk(ilr)=5
         istk(ilr+1)=istk(il1+1)
         istk(ilr+2)=istk(il1+2)
         istk(ilr+3)=0
         istk(ilr+4)=nel1
         lstk(top+1)=lj+nel1
      else
         call putfunnam('bool2s',top)
         fun=-1
         return
      endif
      end
c     ==================================================

      
