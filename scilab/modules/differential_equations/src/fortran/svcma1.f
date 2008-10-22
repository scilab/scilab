C/MEMBR ADD NAME=SVCMA1,SSI=0
      subroutine svcma1 (rsav, isav)
c%purpose
c this routine stores in rsav and isav the contents of common blocks
c ls0001, lsa001, and eh0001, which are used internally in the
c lsoda package.
c
c%calling sequence
c rsav = real array of length 241 or more.
c isav = integer array of length 50 or more.
c!
      double precision isav
      integer ieh, ils, ilsa
      integer i, lenrls, lenils, lenrla, lenila
      double precision rsav
      double precision rls, rlsa
      dimension rsav(*), isav(*)
      common /ls0001/ rls(219), ils(39)
      common /lsa001/ rlsa(22), ilsa(9)
      common /eh0001/ ieh(2)
      data lenrls/219/, lenils/39/, lenrla/22/, lenila/9/
c
      do 10 i = 1,lenrls
 10     rsav(i) = rls(i)
      do 15 i = 1,lenrla
 15     rsav(lenrls+i) = rlsa(i)
c
      do 20 i = 1,lenils
 20     isav(i) = dble(ils(i))
      do 25 i = 1,lenila
 25     isav(lenils+i) = dble(ilsa(i))
c
      isav(lenils+lenila+1) = dble(ieh(1))
      isav(lenils+lenila+2) = dble(ieh(2))
      return
      end
