c     ********************
c     Copyright INRIA
c     ********************
      subroutine  intree2(fname)
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
      if(.not.getrhsvar(4,'i',ndep,mdep,ipdep)) return
c
      if(.not.createvar(5,'i',nvec*mvec,1,ipord)) return
      if(.not.createvar(6,'i',1,1,ipok)) return
c
      call ftree2(istk(ipvec),nvec*mvec,istk(ipdep),
     $     istk(ipoin),istk(ipoinr),istk(ipord),
     $     nord,istk(ipok))
      lhsvar(1)=5
      lhsvar(2)=6
c      nbrows(5)=nord
      istk(iadr(iwhere(5))+1)=nord
      end
c     ********************