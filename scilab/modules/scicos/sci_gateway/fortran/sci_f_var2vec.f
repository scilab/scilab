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
	subroutine intvar2vec(fname)
      
      include 'stack.h'
      integer iadr,sadr
      character*(*) fname
      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      il=iadr(lstk(top))
      il1=il
      l1=sadr(il1+4)
      n=lstk(top+1)-lstk(top)
      call unsfdcopy(n,stk(lstk(top)),-1,stk(l1),-1)
      istk(il1)=1
      istk(il1+1)=n
      istk(il1+2)=1
      istk(il1+3)=0
      lstk(top+1)=l1+n
      
      end
c     ********************

