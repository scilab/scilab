C/MEMBR ADD NAME=RSCAR1,SSI=0
       subroutine rscar1 (rsav, isav)
c%purpose
c this routine restores from rsav and isav the contents of common
c blocks ls0001, lsa001, and eh0001, which are used internally in the
c lsodar package.  this presumes that rsav and isav were loaded by means
c of subroutine svcma or the equivalent.
c!
      double precision isav
      integer ieh, ils, ilsa
      integer i, lenrls, lenils, lenrla, lenila
      double precision rsav
      double precision rls, rlsa, rlsr
      dimension rsav(*), isav(*)
cDEC$ ATTRIBUTES DLLIMPORT:: /ls0001/
      common /ls0001/ rls(219), ils(39)
cDEC$ ATTRIBUTES DLLIMPORT:: /lsa001/
      common /lsa001/ rlsa(22), ilsa(9)
cDEC$ ATTRIBUTES DLLIMPORT:: /lsr001/
      common /lsr001/ rlsr(5),ilsr(9)
cDEC$ ATTRIBUTES DLLIMPORT:: /eh0001/
      common /eh0001/ ieh(2)
      data lenrls/219/, lenils/39/, lenrla/22/, lenila/9/
      data lenrlr/5/,lenilr/9/
c
      l=1
      call dcopy(lenrls,rsav(l),1,rls,1)
      l=l+lenrls
      call dcopy(lenrla,rsav(l),1,rlsa,1)
      l=l+lenrla
      call dcopy(lenrlr,rsav(l),1,rlsr,1)
c
      l=0
      do 20 i = 1,lenils
         ils(i) = int(isav(l+i))
 20   continue
      l=l+lenils
      do 25 i = 1,lenila
         ilsa(i) = int(isav(l+i))
 25   continue
      l=l+lenila
      do 26 i=1,lenilr
         ilsr(i) = int(isav(l+i))
 26   continue
      l=l+lenilr
c
      ieh(1) = int(isav(l+1))
      ieh(2) = int(isav(l+2))
      return
      end
