      integer function issymmetric(in)
c     return 1 if matrix at position in is symmetric
      include '../stack.h'
      double precision sr
c     
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      issymmetric=0
      nnn=top-rhs+in
      l=lstk(nnn)
      il=iadr(l)
      m=istk(il+1)
      n=istk(il+2)
      l=sadr(il+4)
      nn=m*n
      if (m.ne.n) return
      issymmetric =0
      if (istk(il+3).eq.0) then
         do 20 j = 1,n
            do 19 i = 1,j
               ls = l + (i-1) + (j-1)*n
               ll = l + (i-1)*n + (j-1)
               sr = abs(stk(ll)-stk(ls))
               if (stk(ll)+sr .gt. stk(ll)) goto 21
 19         continue
 20      continue
         issymmetric = 1
 21      continue
         return
      else
      if (n .eq. 1) goto 210
      do 200 j = 2,n
        j1 = j - 1
        do 200 i = 1,j1
          ls = l + (i-1) + j1*n
          ll = l + (i-1)*n + j1
          sr = abs(stk(ll)-stk(ls))
          si = abs(stk(ll+nn)+stk(ls+nn))
          if (stk(ll)+sr.gt.stk(ll) .or. stk(ll+nn)+si.gt.stk(ll+nn))
     &      goto 230
 200   continue
 210   do 220 j = 1,n
        ll = l + (j-1) + (j-1)*n
        if (stk(ll)+abs(stk(ll+nn)) .gt. stk(ll)) goto 230
 220  continue
      issymmetric = 1
 230  continue
         return
      endif
      end
