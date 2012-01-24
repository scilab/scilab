c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      subroutine orientandtype(orient,type)
      INCLUDE 'stack.h'
      integer orient,type,native
      parameter (native=0)
 
      if(rhs.eq.3) then
c     .  last argument must be "native" or "double" and previous must be
c     .  an orientation flag
         call getresulttype(top,type)
         if (type.lt.0) then
            err=3
            if (type.eq.-2) then
               call error(55)
            else
               call error(116)
            endif
            return
         endif
         top=top-1
         call  getorient(top,orient)
         if(err.gt.0) return
         top=top-1
      elseif(rhs.eq.2) then
c     .  last argument must be an orientation flag or "native" or "double" 
         call getresulttype(top,type)
         if (type.lt.0) then
c     .     orientation flag
            type=native
            call  getorient(top,orient)
            if(err.gt.0) return
         else
            orient=0
         endif
         top=top-1
      else
         type=native
         orient=0
      endif
      return
      end

      subroutine getorient(k,sel)
      INCLUDE 'stack.h'
c
      integer sel,row,col,star
      integer iadr,sadr
c
      data row/27/,col/12/,star/47/,mtlb/22/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      sel=-1
      il=iadr(lstk(k))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).eq.1) then
         if(istk(il+1)*istk(il+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         sel=stk(sadr(il+4))
         if(sel.lt.1) then
            err=2
            call error(44)
            return
         endif
      elseif (istk(il).eq.10) then
         if(istk(il+1)*istk(il+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         if(istk(il+6).eq.row) then
            sel=1
         elseif(istk(il+6).eq.col) then
            sel=2
         elseif(istk(il+6).eq.star) then
            sel=0
         elseif(istk(il+6).eq.mtlb) then
            sel=-1
         else
            err=2
            call error(44)
            return
         endif
      else
         err=2
         call error(44)
         return
      endif
      return
      end

c     -------------------------------
      subroutine getresulttype(k,type)
      INCLUDE 'stack.h'
      integer type
      character*7 temp
      integer iadr
c     
      iadr(l)=l+l-1

      il=iadr(lstk(k))
      if (istk(il).lt.0) il=iadr(istk(il+1))
      if (istk(il).ne.10.or.istk(il+1).ne.1.or.istk(il+2).ne.1) then
         type=-2
      else
         n=min(7,istk(il+5)-1)
         id=il+4
         l=id+2
         call codetoascii(min(n,7),istk(l),temp)
         if (temp(1:n).eq.'native') then
            type=0
         elseif(temp(1:n).eq.'double') then
            type=1
         else
            type=-1
         endif
      endif
      return
      end
