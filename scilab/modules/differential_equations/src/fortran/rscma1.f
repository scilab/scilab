C/MEMBR ADD NAME=RSCMA1,SSI=0
       subroutine rscma1 (rsav, isav)
c%purpose
c this routine restores from rsav and isav the contents of common
c blocks ls0001, lsa001, and eh0001, which are used internally in the
c lsoda package.  this presumes that rsav and isav were loaded by means
c of subroutine svcma or the equivalent.
c!
      double precision isav
      integer ieh, ils, ilsa
      integer i, lenrls, lenils, lenrla, lenila
      double precision rsav
      double precision rls, rlsa
      dimension rsav(*), isav(*)
cDEC$ ATTRIBUTES DLLIMPORT:: /ls0001/
      common /ls0001/ rls(219), ils(39)
cDEC$ ATTRIBUTES DLLIMPORT:: /lsa001/
      common /lsa001/ rlsa(22), ilsa(9)
cDEC$ ATTRIBUTES DLLIMPORT:: /eh0001/
      common /eh0001/ ieh(2)
      data lenrls/219/, lenils/39/, lenrla/22/, lenila/9/
c
      do 10 i = 1,lenrls
 10     rls(i) = rsav(i)
      do 15 i = 1,lenrla
 15     rlsa(i) = rsav(lenrls+i)
c
      do 20 i = 1,lenils
 20     ils(i) = int(isav(i))
      do 25 i = 1,lenila
 25     ilsa(i) = int(isav(lenils+i))
c
      ieh(1) = int(isav(lenils+lenila+1))
      ieh(2) = int(isav(lenils+lenila+2))
      return
      end
