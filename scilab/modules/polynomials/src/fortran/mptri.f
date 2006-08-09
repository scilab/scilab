C/MEMBR ADD NAME=MPTRI,SSI=0
c     Copyright INRIA
      subroutine mptri(d,m,n,diag,dd,job)
c
      integer d(*),m,n,dd(*),diag,job
c
      do 10 i=2,m*n+1
   10 dd(i)=i-1
c
      if(job.eq.0) goto 15
c triangle superieur
      if(diag.le.0) goto 11
      call iset(m*diag,0,dd(2),1)
      ls=m*diag+2
      nn=n-diag
      ll=m-1
      goto 12
   11 ls=2-diag
      nn=n
      ll=m-1+diag
   12 do 13 j=1,nn
      if(ll.le.0) goto 20
      call iset(ll,0,dd(ls+1),1)
      ll=ll-1
      ls=ls+m+1
   13 continue
      goto 20
c
c triangle inferieur
   15 nn=n
      if(diag.lt.0) goto 16
      ls=m*(diag+1)+1
      nn=n-diag-1
      ll=1
      goto 17
   16 ls=1
      ll=-diag
      nn=n
   17 do 19 j=1,nn
      if(ll.gt.m) ll=m
      call iset(ll,0,dd(ls+1),1)
      ls=ls+m
      ll=ll+1
   19 continue
c
c calcul du volume
   20 l=0
      do 21 i=2,m*n+1
      if(dd(i).eq.0) then
                          l=l+1
                     else
                          l=l+d(dd(i)+1)-d(dd(i))
      endif
   21 continue
      dd(1)=l
      return
      end
