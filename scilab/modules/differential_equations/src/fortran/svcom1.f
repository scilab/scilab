C/MEMBR ADD NAME=SVCOM1,SSI=0
      subroutine svcom1(rsav, isav)
c%purpose
c this routine stores in rsav and isav the contents of common blocks
c ls0001 and eh0001, which are used internally in the lsode package.
c
c%calling sequence
c rsav = real array of length 219 or more.
c isav = integer array of length 41 or more.
c!
      double precision isav
      integer ieh, ils
      integer i, lenils, lenrls
      double precision rsav,   rls
      dimension rsav(*), isav(*)
      common /ls0001/ rls(219), ils(39)
      common /eh0001/ ieh(2)
      data lenrls/219/, lenils/39/
c
      do 10 i = 1,lenrls
 10     rsav(i) = rls(i)
      do 20 i = 1,lenils
 20     isav(i) = dble(ils(i))
      isav(lenils+1) =dble(ieh(1))
      isav(lenils+2) =dble(ieh(2))
      return
      end
