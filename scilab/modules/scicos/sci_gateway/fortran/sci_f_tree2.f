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
	subroutine  intree2(fname)
c
!      include 'stack.h'
!      logical checklhs,checkrhs,putlhsvar
!      logical createvar,getrhsvar
!      character*(*) fname
!      
!      integer iadr
!      
!      iadr(l)=l+l-1
!
!c	
!	if(.not.checklhs(fname,2,2)) return
!      if(.not.checkrhs(fname,4,4)) return
!      if(.not.getrhsvar(1,'i',nvec,mvec,ipvec)) return
!      if(.not.getrhsvar(2,'i',noin,moin,ipoin)) return
!      if(.not.getrhsvar(3,'i',noinr,moinr,ipoinr)) return
!      if(.not.getrhsvar(4,'i',ndep,mdep,ipdep)) return
!c
!      if(.not.createvar(5,'i',nvec*mvec,1,ipord)) return
!      if(.not.createvar(6,'i',1,1,ipok)) return
!c
!      call ftree2(istk(ipvec),nvec*mvec,istk(ipdep),
!     $     istk(ipoin),istk(ipoinr),istk(ipord),
!     $     nord,istk(ipok))
!      lhsvar(1)=5
!      lhsvar(2)=6
!c      nbrows(5)=nord
!      istk(iadr(iwhere(5))+1)=nord
!      if(.not.putlhsvar())return
      end
c     ********************
			