C/MEMBR ADD NAME=MPDIAG,SSI=0
c     Copyright INRIA
      subroutine mpdiag(d,m,n,diag,dd,mr,nr)
      integer d(*),dd(*),diag
c
      if(n.le.0) goto 20
c
c extraction d'une diagonale dune matrice
c
      nr=1
      mn=min(m,n)
      k=-diag+1
      if(diag.ge.0) k=diag*m+1
      mr=diag+m
      if(diag+m.ge.mn) mr=mn
      if(diag.ge.n-mn) mr=n-diag
      l=0
      if(mr.le.0) goto 11
      do 10 kk=2,mr+1
      dd(kk)=k
      l=l+d(k+1)-d(k)
   10 k=k+m+1
   11 dd(1)=l
      return
c
c
c creation d'une matrice 'diagonale'
c
   20 mr=m
      nr=m
      l=0
      if(diag.ge.0) then
                         nr=nr+diag
                    else
                         mr=mr-diag
      endif
      do 21 k=2,mr*nr+1
   21 dd(k)=0
      kk=1-diag
      if(diag.ge.0) kk=diag*mr+1
      do 22 k=1,m
      dd(kk+1)=k
      kk=kk+mr+1
   22 l=d(k+1)-d(k)+l
      dd(1)=l+mr*nr-m
      return
      end
