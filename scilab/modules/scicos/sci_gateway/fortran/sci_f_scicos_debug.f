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
c     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
c     
c     See the file ./license.txt
c 

      subroutine scicosdebug(fname)
c
c
      include 'stack.h'
Cc (DLL Intel Fortran)     
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /cosdebug/
cDEC$ ENDIF      
      
      common /cosdebug/ cosd
      logical getrhsvar, createvar,checklhs,checkrhs
      integer cosd
      character*(*) fname
c
      rhs = max(0,rhs)
      if(.not.checklhs(fname,1,1)) return
      if(.not.checkrhs(fname,-1,1)) return
      if (rhs.eq.1) then
         if(.not.getrhsvar(1,'i',n,m,i)) return
         cosd=istk(i)
         lhsvar(1)=0
      else
         if(.not.createvar(1,'d',1,1,ipr1)) return
         stk(ipr1)=cosd
         lhsvar(1)=1 
      endif

      end
c     ********************
