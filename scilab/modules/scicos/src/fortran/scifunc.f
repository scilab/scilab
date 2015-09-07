c     Scicos 
c 
c     Copyright (C) INRIA - METALAU Project <scicos@inria.fr> 
c 
c     This program is free software; you can redistribute it and/or modify 
c     it under the terms of the GNU General Public License as published by 
c     the Free Software Foundation; either version 2 of the License, or 
c     (at your option) any later version. 
c 
c     This program is distributed in the hope that it will be useful, 
c     but WITHOUT ANY WARRANTY; without even the implied warranty of 
c     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
c     GNU General Public License for more details. 
c 
c     You should have received a copy of the GNU General Public License 
c     along with this program; if not, write to the Free Software 
c     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. 
c 
c     See the file ./license.txt 
c 

      subroutine scifunc(mlhs,mrhs)
c     execute scilab function with mrhs input args and mlhs output
c     variables
c     input args are supposed to be stored in the top of the stack
c     at positions top-mrhs+1:top
c     function adress is given by the integer ptr in the common /scsptr/
c
c     Copyright INRIA
!      include "stack.h"
!      integer ptr
!      integer mlhs,mrhs
!C
!Cc (DLL Intel Fortran)     
!cDEC$ IF DEFINED (FORDLL)
!cDEC$ ATTRIBUTES DLLIMPORT:: /scsptr/
!cDEC$ ENDIF  
!      common /scsptr/ ptr   
!c
!
!C     macro execution 
!C     
!      pt = pt + 1
!      if (pt .gt. psiz) then
!        call error(26)
!        goto 9999
!      endif
!      ids(1,pt) = lhs
!      ids(2,pt) = rhs
!      rstk(pt) = 1001
!      lhs = mlhs
!      rhs = mrhs
!      niv = niv + 1
!      fun = 0
!      fin = ptr
!C     
!      icall = 5
!      krec = -1
!C      FIXME implement for Scilab 6
!C      include "callinter.h.f" (REMOVED)
! 200  lhs = ids(1,pt)
!      rhs = ids(2,pt)
!      pt = pt - 1
!      niv = niv - 1
!      return
!C+    
! 9999 continue
!      if(err1.gt.0) then
!         lhs=ids(1,pt)
!         rhs=ids(2,pt)
!         pt=pt-1
!         fun=0
!      endif
!      iero=1
!      niv=niv-1
      return
      end
