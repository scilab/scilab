      subroutine dsmsp(nrb,ncb,nca,b,mrb,a,nela,inda,c,mrc)
c multiply a full matrix stored in b on right by a sparse
c  matrix stored in a,inda. Put result in c.
c*** input
c  nrb        actual number of rows in b.
c  ncb        actual number of columns in b and row of a 
c  nca        actual number of columns in a and c
c  b          a two-dimensional array containing all the
c               elements of the first matrix.
c  mrb        row-dimension of b in calling routine.
c  a            a one-dimensional array containing the non-zero elements
c                 of the first matrix,arranged row-wise, but not
c                 necessarily in order within rows.
c  nela        number of non-zero elements in a
c  inda(i)     1<=i<=nra number of non-zero elements in row i of a.
c  inda(nra+i) 1<=i<nela column index of i'th non-zero element of a.
c  mrc        row dimension of c in calling routine.
c***output
c  c          a two-dimensional array containing all the
c               elements of the product matrix.
c!
c     Copyright INRIA
      double precision  a(*), b(mrb,ncb), c(mrc,nca)
      integer inda(*)

c  nra,nca are number of rows,columns in a.
      nra = ncb
c  nrc,ncc are number of rows,columns in c.
      nrc = nrb
      ncc = nca

c  clear c to zero.
   10 do 30 i=1,nrc
        do 20 j=1,ncc
          c(i,j) = 0.0d0
   20   continue
   30 continue
c  n2 will be pointer to end of row k of a.
      n2 = 0
c k will be row-index for a.
      do 60 k=1,nra
c pick out number of non-zeros in row k of a.
        nir = inda(k)
        if (nir.eq.0) go to 60
c if no non-zeros skip processing of row k.
c n1 will point to start of row k in a,n2 to end of row.
        n1 = n2 + 1
        n2 = n2 + nir
c  process row k of a,i.e.
c form all products of b(i,k) with non-zero a(k,j),
c  add into c(i,j)
c l points to non-zero elements in row k of a.
        do 50 l=n1,n2
          j = inda(nra+l)
          do 40 i=1,nrb
            c(i,j) = c(i,j) + b(i,k)*a(l)
   40     continue
   50   continue
   60 continue
      return
      end
