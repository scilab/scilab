c     ********************
c     Copyright INRIA
c     ********************
      subroutine  intree4(fname)
c
c
      include 'stack.h'
      logical createvar,getrhsvar
      character*(*) fname
c
      iadr(l)=l+l-1
      if(.not.getrhsvar(1,'i',nvec,mvec,ipvec)) return
      if(.not.getrhsvar(2,'i',noin,moin,ipoin)) return
      if(.not.getrhsvar(3,'i',noinr,moinr,ipoinr)) return
      if(.not.getrhsvar(4,'i',nnd,mnd,ipnd)) return
      if(.not.getrhsvar(5,'i',ntyp,mtyp,iptyp)) return
c
      if(.not.createvar(6,'i',1,nnd*mnd,ipr1)) return
      if(.not.createvar(7,'i',1,nnd*mnd,ipr2)) return
c
      call ftree4(istk(ipvec),nvec*mvec,istk(ipnd),mnd,
     $     istk(iptyp),istk(ipoin),istk(ipoinr),istk(ipr1),
     $     istk(ipr2),nr)
      lhsvar(1)=6
      lhsvar(2)=7
      istk(iadr(iwhere(6))+2)=nr
      istk(iadr(iwhere(7))+2)=nr
      end
c     ********************
