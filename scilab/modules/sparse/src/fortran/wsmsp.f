      subroutine wsmsp(nrb,ncb,nca,br,bi,mrb,ar,ai,nela,inda,cr,ci,
c     Copyright INRIA
     $     mrc,itb,ita)
c multiply a full complex matrix stored in b on right by a sparse
c  complex matrix stored in a,inda. Put result in c.
c*** input
c  nrb        actual number of rows in b.
c  ncb        actual number of columns in b and row of a 
c  nca        actual number of columns in a and c
c  br,bi        a two-dimensional array containing all the
c               elements of the first matrix.
c  mrb        row-dimension of b in calling routine.
c  ar,ai          a one-dimensional array containing the non-zero elements
c                 of the first matrix,arranged row-wise, but not
c                 necessarily in order within rows.
c  nela        number of non-zero elements in a
c  inda(i)     1<=i<=nra number of non-zero elements in row i of a.
c  inda(nra+i) 1<=i<nela column index of i'th non-zero element of a.
c  mrc        row dimension of c in calling routine.
c  ita        real/complex a matrix type
c             set ita=0 if a is real
c  itb        real/complex b matrix type
c             set itb=0 if b is real
c***output
c  cr,ci        a two-dimensional array containing all the
c               elements of the product matrix.
c!
      double precision  ar(nela),ai(nela), br(mrb,ncb),bi(mrb,ncb)
      double precision  cr(mrc,nca),ci(mrc,nca)
      integer inda(*)

c  nra,nca are number of rows,columns in a.
      nra = ncb
c  nrc,ncc are number of rows,columns in c.
      nrc = nrb
      ncc = nca

c  clear c to zero.
   10 do 30 i=1,nrc
        do 20 j=1,ncc
          cr(i,j) = 0.0d0
          ci(i,j) = 0.0d0
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
          if(ita.eq.0) then
          do 40 i=1,nrb
            cr(i,j) = cr(i,j) + br(i,k)*ar(l)
            ci(i,j) = ci(i,j) + bi(i,k)*ar(l)
   40     continue
          elseif(itb.eq.0) then
          do 41 i=1,nrb
            cr(i,j) = cr(i,j) + br(i,k)*ar(l)
            ci(i,j) = ci(i,j) + br(i,k)*ai(l)
   41     continue
          else
          do 42 i=1,nrb
            cr(i,j) = cr(i,j) + br(i,k)*ar(l)-bi(i,k)*ai(l)
            ci(i,j) = ci(i,j) + bi(i,k)*ar(l)+br(i,k)*ai(l)
   42     continue
          endif
   50   continue
   60 continue
      return
      end
