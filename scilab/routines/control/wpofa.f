      subroutine wpofa(ar,ai,lda,n,info)
c!
c     Copyright INRIA
      double precision ar(lda,*),ai(lda,*)
      double precision s,tr,ti,wdotcr,wdotci
      do 30 j = 1, n
         info = j
         s = 0.0d+0
         jm1 = j-1
         if (jm1 .lt. 1) go to 20
         do 10 k = 1, jm1
            tr = ar(k,j)-wdotcr(k-1,ar(1,k),ai(1,k),1,ar(1,j),ai(1,j),1)
            ti = ai(k,j)-wdotci(k-1,ar(1,k),ai(1,k),1,ar(1,j),ai(1,j),1)
            call wdiv(tr,ti,ar(k,k),ai(k,k),tr,ti)
            ar(k,j) = tr
            ai(k,j) = ti
            s = s + tr*tr + ti*ti
   10    continue
   20    continue
         s = ar(j,j) - s
         if (s.le.0.0d+0 .or. ai(j,j).ne.0.0d+0) go to 40
         ar(j,j) = sqrt(s)
   30 continue
      info = 0
   40 return
      end
