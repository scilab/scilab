C/MEMBR ADD NAME=WBALIN,SSI=0
c     Copyright INRIA
      subroutine wbalin(max , n , low , igh , scale , ear , eai)
c
c!purpose
c                - performs the inverse transformation of that
c                            done in subroutine cbal
c
c!calling sequence
c
c      subroutine wbalin(max , n , low , igh , scale , ear , eai)
c           s  max       - maximum row dimension of ea
c              n         - order of ea
c              low       - integer determined by balanc or balanx
c              igh       - integer determined by balanc or balanx
c              scale(n)  - contains information determining the
c                            permutations and scaling factors used
c                            by balanc or balanx
c              ea(max,n) - contains the matrix to be transformed
c
c!
c
c ****
      double precision s, ear,eai, scale
      dimension ear(max,n), eai(max,n),scale(n)
      if (igh.eq.low) go to 50
c ****
c     remove scaling from rows and columns
c ****
      do 20 i=low,igh
         s = scale(i)
         do 10 j=1,n
            ear(i,j) = s*ear(i,j)
            eai(i,j) = s*eai(i,j)
   10    continue
   20 continue
      do 40 j=low,igh
         s = 1.0d+0/scale(j)
         do 30 i=1,n
            ear(i,j) = ear(i,j)*s
            eai(i,j) = eai(i,j)*s
   30    continue
   40 continue
c ****
c     re-permute rows and columns
c ****
   50 do 80 ii=1,n
         i = ii
         if (i.ge.low .and. i.le.igh) go to 80
         if (i.lt.low) i = low - ii
         k = scale(i)
         if (k.eq.i) go to 80
         do 60 j=1,n
            s = ear(i,j)
            ear(i,j) = ear(k,j)
            ear(k,j) = s
            s = eai(i,j)
            eai(i,j) = eai(k,j)
            eai(k,j) = s
   60    continue
         do 70 j=1,n
            s = ear(j,i)
            ear(j,i) = ear(j,k)
            ear(j,k) = s
            s = eai(j,i)
            eai(j,i) = eai(j,k)
            eai(j,k) = s
   70    continue
   80 continue
      return
      end
