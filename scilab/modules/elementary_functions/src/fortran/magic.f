c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
C/MEMBR ADD NAME=MAGIC,SSI=0
      subroutine magic(a,lda,n)
c!purpose
c     algorithms for magic squares taken from
c        mathematical recreations and essays, 12th ed.,
c        by w. w. rouse ball and h. s. m. coxeter
c!calling sequence
c     subroutine magic(a,lda,n)
c     double precision a(lda,n)
c     integer lda,n
c!
      double precision a(lda,n),t
c
      if (mod(n,4) .eq. 0) go to 100
      if (mod(n,2) .eq. 0) m = n/2
      if (mod(n,2) .ne. 0) m = n
c
c     odd order or upper corner of even order
c
      do 20 j = 1,m
         do 10 i = 1,m
            a(i,j) = 0
   10    continue
   20 continue
      i = 1
      j = (m+1)/2
      mm = m*m
      do 40 k = 1, mm
         a(i,j) = k
         i1 = i-1
         j1 = j+1
         if(i1.lt.1) i1 = m
         if(j1.gt.m) j1 = 1
         if(int(a(i1,j1)).eq.0) go to 30
            i1 = i+1
            j1 = j
   30    i = i1
         j = j1
   40 continue
      if (mod(n,2) .ne. 0) return
c
c     rest of even order
c
      t = m*m
      do 60 i = 1, m
         do 50 j = 1, m
            im = i+m
            jm = j+m
            a(i,jm) = a(i,j) + 2*t
            a(im,j) = a(i,j) + 3*t
            a(im,jm) = a(i,j) + t
   50    continue
   60 continue
      m1 = (m-1)/2
      if (m1.eq.0) return
      do 70 j = 1, m1
         call dswap(m,a(1,j),1,a(m+1,j),1)
   70 continue
      m1 = (m+1)/2
      m2 = m1 + m
      call dswap(1,a(m1,1),1,a(m2,1),1)
      call dswap(1,a(m1,m1),1,a(m2,m1),1)
      m1 = n+1-(m-3)/2
      if(m1.gt.n) return
      do 80 j = m1, n
         call dswap(m,a(1,j),1,a(m+1,j),1)
   80 continue
      return
c
c     double even order
c
  100 k = 1
      do 120 i = 1, n
         do 110 j = 1, n
            a(i,j) = k
            if (mod(i,4)/2 .eq. mod(j,4)/2) a(i,j) = n*n+1 - k
            k = k+1
  110    continue
  120 continue
      return
      end
