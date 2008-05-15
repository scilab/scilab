c
c  Copyright (C) 1997 Berwin A. Turlach <berwin@alphasun.anu.edu.au>
c
c  This program is free software; you can redistribute it and/or modify
c  it under the terms of the GNU General Public License as published by
c  the Free Software Foundation; either version 2 of the License, or
c  (at your option) any later version.
c
c  This program is distributed in the hope that it will be useful,
c  but WITHOUT ANY WARRANTY; without even the implied warranty of
c  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
c  GNU General Public License for more details.
c
c  You should have received a copy of the GNU General Public License
c  along with this program; if not, write to the Free Software
c  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
c  USA.

c
c  this routine checks whether Aind has valid entries, i.e., 
c    1) 1<= Aind(1,i) <= n for i=1,...,q (number of constraints)
c    2) 1<= Aind(j,i) <= n for j=2,...,Aind(1,i)+1, i=1,...,q
c
c  Aind is a m times q matrix constructed in Splus
c
      subroutine aind(ind,m,q,n,ok)
      implicit none
      integer m, ind(m,*), q, n, i, j
      logical ok
      ok = .FALSE.
      do i=1,q
         if( ind(1,i) .LT. 1 .OR. ind(1,i) .GT. n ) return
         do j=2,ind(1,i)+1
            if( ind(j,i) .LT. 1 .OR. ind(j,i) .GT. n ) return
         enddo
      enddo
      ok = .TRUE.
      return
      end
