      subroutine wspms(nra,nca,ncb,ar,ai,nela,inda,br,bi,mrb,cr,ci,
c     Copyright INRIA
     $     mrc,ita,itb)
c**********************************************************
c multiply sparse matrix stored in a,inda on right by full
c   matrix stored in b. put result in c.
c*** input
c  nra         actual row dimension of a and c matrix
c  nca         actual column dimension of a and row dimension of b matrix
c  ncb         actual column dimension of a and c matrices
c  ar,ai          a one-dimensional array containing the non-zero elements
c                 of the first matrix,arranged row-wise, but not
c                 necessarily in order within rows.
c  nela        number of non-zero elements in a
c  inda(i)     1<=i<=nra number of non-zero elements in row i of a.
c  inda(nra+i) 1<=i<nela column index of i'th non-zero element of a.
c  br,bi        a two-dimensional array containing all the
c               elements of the second matrix.
c  mrb        row-dimension of b in calling routine.
c  mrc        row-dimension of c in calling routine.
c  ita        real/complex a matrix type
c             set ita=0 if a is real
c  itb        real/complex b matrix type
c             set itb=0 if b is real
c*** output
c  cr,ci        a two-dimensional array containing all the
c               elements of the product matrix.
c!
      double precision ar(nela),ai(nela), br(mrb,ncb),bi(mrb,ncb)
      double precision cr(mrc,ncb),ci(mrc,ncb)
      integer inda(*)
      double precision tr,ti
c
c  nrc,ncc are number of rows,columns in c.
      nrc = nra
      ncc = ncb
c clear c to zero.
   10 do 30 i=1,nrc
        do 20 j=1,ncc
          cr(i,j) = 0.0d0
          ci(i,j) = 0.0d0
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
          tr = ar(k)
          if(ita.eq.0) then
             ti=0.0d0
          else
             ti = ai(k)
          endif
          if(itb.eq.0) then
             do 40 j=1,ncb
                cr(i,j) = cr(i,j) + tr*br(l,j)
                ci(i,j) = ci(i,j) + ti*br(l,j)
 40          continue
          else
             do 41 j=1,ncb
                cr(i,j) = cr(i,j) + tr*br(l,j)-ti*bi(l,j)
                ci(i,j) = ci(i,j) + tr*bi(l,j)+ti*br(l,j)
 41          continue
          endif
   50   continue
   60 continue
      return
      end
