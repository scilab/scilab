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
	subroutine  intree3(fname)
c
c			
!      include 'stack.h'
!      logical checklhs,checkrhs,putlhsvar
!      logical createvar,getrhsvar
!      
!      character*(*) fname
!      integer iadr
!      
!      iadr(l)=l+l-1
!      
!      if(.not.checklhs(fname,2,2)) return
!      if(.not.checkrhs(fname,7,7)) return
!      if(.not.getrhsvar(1,'i',nvec,mvec,ipvec)) return
!      if(.not.getrhsvar(2,'i',ndep,mdep,ipdep)) return
!      if(.not.getrhsvar(3,'i',ntyp,mtyp,iptyp)) return
!      if(.not.getrhsvar(4,'i',nbex,mbex,ipbex)) return
!      if(.not.getrhsvar(5,'i',nbop,mbop,ipbop)) return
!      if(.not.getrhsvar(6,'i',nbln,mbln,ipbln)) return
!      if(.not.getrhsvar(7,'i',nblr,mblr,ipblr)) return
!c
!      if(.not.createvar(8,'i',nvec*mvec,1,ipord)) return
!      if(.not.createvar(9,'i',1,1,ipok)) return
!      if(.not.createvar(10,'i',1,nvec*mvec,ipkk)) return
!c
!      call ftree3(istk(ipvec),nvec*mvec,istk(ipdep),
!     $     istk(iptyp),istk(ipbex),istk(ipbop),istk(ipbln),
!     $     istk(ipblr),istk(ipkk),istk(ipord),nord,
!     $     istk(ipok))
!      lhsvar(1)=8
!      lhsvar(2)=9
!      istk(iadr(iwhere(8))+1)=nord
!      if(.not.putlhsvar())return
      end
c     ********************
 