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
      subroutine  intree4(fname)
c
c
!      include 'stack.h'
!      logical createvar,getrhsvar
!      character*(*) fname
!      integer iadr
!      
!      iadr(l)=l+l-1
!c
!      if(.not.getrhsvar(1,'i',nvec,mvec,ipvec)) return
!      if(.not.getrhsvar(2,'i',noin,moin,ipoin)) return
!      if(.not.getrhsvar(3,'i',noinr,moinr,ipoinr)) return
!      if(.not.getrhsvar(4,'i',nnd,mnd,ipnd)) return
!      if(.not.getrhsvar(5,'i',ntyp,mtyp,iptyp)) return
!c
!      if(.not.createvar(6,'i',1,nnd*mnd,ipr1)) return
!      if(.not.createvar(7,'i',1,nnd*mnd,ipr2)) return
!c
!      call ftree4(istk(ipvec),nvec*mvec,istk(ipnd),mnd,
!     $     istk(iptyp),istk(ipoin),istk(ipoinr),istk(ipr1),
!     $     istk(ipr2),nr)
!      lhsvar(1)=6
!      lhsvar(2)=7
!      istk(iadr(iwhere(6))+2)=nr
!      istk(iadr(iwhere(7))+2)=nr
      end
c     ********************

