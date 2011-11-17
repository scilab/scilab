C/MEMBR ADD NAME=RSCOM1,SSI=0
      subroutine rscom1 (rsav, isav)
c%purpose
c this routine restores from rsav and isav the contents of common
c blocks ls0001 and eh0001, which are used internally in the lsode
c package.  this presumes that rsav and isav were loaded by means
c of subroutine svcom or the equivalent.
c!
      double precision isav
      integer ieh, ils
      integer i, lenils, lenrls
      double precision rsav,   rls
      dimension rsav(*), isav(*)
cDEC$ ATTRIBUTES DLLIMPORT:: /ls0001/
      common /ls0001/ rls(219), ils(39)
cDEC$ ATTRIBUTES DLLIMPORT:: /eh0001/
      common /eh0001/ ieh(2)
      data lenrls/219/, lenils/39/
c
      do 10 i = 1,lenrls
 10     rls(i) = rsav(i)
      do 20 i = 1,lenils
 20     ils(i) = int(isav(i))
      ieh(1) = int(isav(lenils+1))
      ieh(2) = int(isav(lenils+2))
      return
      end
