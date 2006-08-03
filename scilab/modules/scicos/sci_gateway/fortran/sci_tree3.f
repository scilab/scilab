c     ********************
c     Copyright INRIA
c     ********************
      subroutine intree3(fname)
c
c
      include 'stack.h'
      logical createvar,getrhsvar
      character*(*) fname
c
      iadr(l)=l+l-1
      if(.not.getrhsvar(1,'i',nvec,mvec,ipvec)) return
      if(.not.getrhsvar(2,'i',ndep,mdep,ipdep)) return
      if(.not.getrhsvar(3,'i',ntyp,mtyp,iptyp)) return
      if(.not.getrhsvar(4,'i',nbex,mbex,ipbex)) return
      if(.not.getrhsvar(5,'i',nbop,mbop,ipbop)) return
      if(.not.getrhsvar(6,'i',nbln,mbln,ipbln)) return
      if(.not.getrhsvar(7,'i',nblr,mblr,ipblr)) return
c
      if(.not.createvar(8,'i',nvec*mvec,1,ipord)) return
      if(.not.createvar(9,'i',1,1,ipok)) return
      if(.not.createvar(10,'i',1,nvec*mvec,ipkk)) return
c
      call ftree3(istk(ipvec),nvec*mvec,istk(ipdep),
     $     istk(iptyp),istk(ipbex),istk(ipbop),istk(ipbln),
     $     istk(ipblr),istk(ipkk),istk(ipord),nord,
     $     istk(ipok))
      lhsvar(1)=8
      lhsvar(2)=9
      istk(iadr(iwhere(8))+1)=nord
      end
c     ********************
