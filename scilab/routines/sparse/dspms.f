      subroutine dspms(nra,nca,ncb,a,nela,inda,b,mrb,c,mrc)
c**********************************************************
c multiply sparse matrix stored in a,inda on right by full
c   matrix stored in b. put result in c.
c*** input
c  nra         actual row dimension of a and c matrix
c  nca         actual column dimension of a and row dimension of b matrix
c  ncb         actual column dimension of a and c matrices
c  a            a one-dimensional array containing the non-zero elements
c                 of the first matrix,arranged row-wise, but not
c                 necessarily in order within rows.
c  nela        number of non-zero elements in a
c  inda(i)     1<=i<=nra number of non-zero elements in row i of a.
c  inda(nra+i) 1<=i<nela column index of i'th non-zero element of a.
c  b          a two-dimensional array containing all the
c               elements of the second matrix.
c  mrb        row-dimension of b in calling routine.
c  mrc        row-dimension of c in calling routine.
c*** output
c  c          a two-dimensional array containing all the
c               elements of the product matrix.
c!
c     Copyright INRIA
      double precision a(*), b(mrb,ncb), c(mrc,ncb)
      integer inda(*)
      double precision t
c
c  nrc,ncc are number of rows,columns in c.
      nrc = nra
      ncc = ncb
c clear c to zero.
   10 do 30 i=1,nrc
        do 20 j=1,ncc
          c(i,j) = 0.0d0
   20   continue
   30 continue
c  n2 will be pointer to end of row i of a.
      n2 = 0
c  i will be row-index for a.
      do 60 i=1,nra
c  pick out number of non-zero elements in row i.
        nir = inda(i)
c if no non-zeroes skip processing of row i of a.
        if (nir.eq.0) go to 60
c  n1 points to start of row i in a,n2 to end.
        n1 = n2 + 1
        n2 = n2 + nir
c process row i of a, i.e. form all products of non-zero a(i,l) with
c   b(l,j); put into c(i,j).
c k points to non-zero elements in row i of a.
        do 50 k=n1,n2
          l = inda(nra+k)
          t = a(k)
          do 40 j=1,ncb
            c(i,j) = c(i,j) + t*b(l,j)
   40     continue
   50   continue
   60 continue
      return
      end
