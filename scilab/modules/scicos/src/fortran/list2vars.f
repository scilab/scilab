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

      subroutine list2vars(n1,ltop)
c     given a list stored in position top in the stack
c     list2vars computes lstk(top:top-1+n1) pointers on the beginning of
c     each first n1 entries of the list.

c     Copyright INRIA
c     WARNING : if topk is the entry value of top, lstk(topk) is
c     incremented by sadr(3+n). Previous value is returned in ltop.
c
      include 'stack.h'
      integer iadr,sadr
c
      iadr(l) = l + l - 1
      sadr(l) = (l/2) + 1
c
      if (n1.eq.0) then
         top=top-1
         return
      endif
      il=iadr(lstk(top))
      if(istk(il).ne.15) then
         if(n1.le.1) return
         call error(98)
         return
      endif
      n=istk(il+1)
      if(n1.gt.n) then
         call error(98)
         return
      endif
      l=lstk(top)
      ltop=l
      l0=sadr(il+1+n+2)-1
      do 10 i=1,n1
         lstk(top)=l0+istk(il+1+i)
         top=top+1
 10   continue
      top=top-1
      lstk(top+1)=l0+istk(il+2+n1)
      return
      end
      subroutine ltopadj(ltop)
      include 'stack.h'
      lstk(top+1)=ltop
      return
      end
