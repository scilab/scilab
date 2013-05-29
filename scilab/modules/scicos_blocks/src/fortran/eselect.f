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

      subroutine eselect(flag,nevprt,ntvec,rpar,nrpar,ipar,nipar,u,nu)
c     Scicos block simulator
c     if-then-else block
c     if event input exits from then or else clock outputs based
c     on the sign of the unique input (if input>0 then  else )
c
c     Copyright INRIA
      double precision rpar(*),u(*)
      integer flag,nevprt,ntvec,nrpar,ipar(*)
      integer nipar,nu
c
      integer iu
c
      common /dbcos/ idb
c
      if(idb.eq.1) then
         write(6,'(''ifthel     t='',e10.3,'' flag='',i1)') t,flag
      endif
c
      iu=max(min(int(u(1)),ipar(1)),1)
      if(flag.eq.3) then
            ntvec=iu
      endif
      end
