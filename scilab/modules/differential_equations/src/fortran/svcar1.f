C/MEMBR ADD NAME=SVCAR1,SSI=0
       subroutine svcar1 (rsav, isav)
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
      double precision rls, rlsa,rlsr
      dimension rsav(*), isav(*)
      common /ls0001/ rls(219), ils(39)
      common /lsa001/ rlsa(22), ilsa(9)
      common /lsr001/ rlsr(5),ilsr(9)
      common /eh0001/ ieh(2)
      data lenrls/219/, lenils/39/, lenrla/22/, lenila/9/
      data lenrlr/5/,lenilr/9/
c
      l=1
      call dcopy(lenrls,rls,1,rsav(l),1)
      l=l+lenrls
      call dcopy(lenrla,rlsa,1,rsav(l),1)
      l=l+lenrla
      call dcopy(lenrlr,rlsr,1,rsav(l),1)
c
      l=0
      do 20 i = 1,lenils
        isav(l+i) =ils(i) 
 20   continue
      l=l+lenils
      do 25 i = 1,lenila
         isav(l+i)=ilsa(i)
 25   continue
      l=l+lenila
      do 26 i=1,lenilr
         isav(l+i)=ilsr(i)
 26   continue
      l=l+lenilr
c
      isav(l+1)=ieh(1)
      isav(l+2)=ieh(2)
      return
      end
