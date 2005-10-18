      subroutine mpdspmas(nr,nc,a,nela,inda,b,c,mpzero,d,indd,ao)
c
c                    INRIA (LGPL license) 
c
c--------------------------------------------------------------------
c   Maxplus multiply sparse (CSR) matrix stored in a, inda on right by
c   full vector stored in b. Put result in c. In d is coded where the 
c   maximum are reached.
c
c---------------------------------------------------------------------
c                               input
c---------------------------------------------------------------------
c
c  nr          actual row dimension of a and c matrix.
c  nc          actual column dimension of a.
c  a           one-dimensional array containing the non-zero elements
c                 of the first matrix, arranged row-wise, but not
c                 necessarily in order within rows.
c  nela        number of non-zero elements in a
c  inda(i)     1<=i<=nr number of non-zero elements in row i of a.
c  inda(nra+i) 1<=i<nel  column index of i'th non-zero element of a.
c  b           vector of size nc.
c  mpzero      -inf
c
c---------------------------------------------------------------------
c                                output
c---------------------------------------------------------------------
c  c          a two-dimensional array containing all the
c               elements of the matrix product.
c  d          nra size vector of 1.
c  indd(i)    =1 for 1<=i<=nra . 
c  indd(nra+i) 1<=i<nra column index of i'th non-zero element of d.
c                coding the first element where the maximum is reached
c                in the maxplus scalar poduct of b and the i'th row of a.
c ao          a nr-vector of the entries of a at which the optimums are
c             reached .
c
c---------------------------------------------------------------------
c
      double precision a(*),d(*),b(nc),c(nr),ao(nr)
      double precision mpzero,t,cc,aop
      integer inda(*),indd(*),nir,nrpi,id
c
c      write(*,*)'mpdspams',mpzero,nr,nc,nela,a(1),b(1),inda(1)
c
c clear c to zero.
c
      do 20 i=1,nr
          c(i) = mpzero
 20    continue
c
c  initialisation of n2 which will be a pointer to end of row i of a.
c
      n2 = 0
c
c  i will be row-index for a.
c
      do 60 i=1,nr
c
c  pick out number of non-zero elements in row i.
c
        nir = inda(i)
        nrpi=nr+i
c
c if no non-zeroes skip processing of row i of a.
c
        if (nir.eq. 0) go to 55
c
c  n1 points to start of row i in a, n2 to end.
c
        n1 = n2 + 1
        n2 = n2 + nir
        l=inda(nr+n1)
c
c current maximum
c
        cc=a(n1)+b(l)
        aop=a(n1)
c
c id entry number of the current maximum
c
        id=l
        n1=n1+1
c
c process row i of a, i.e. form all products of non-zero a(i,l) with
c b(l); put into c(i).
c k points to non-zero elements in row i of a.
c
        do 50 k=n1,n2
          l = inda(nr+k)
          t = a(k)+b(l)
          if (cc .ge. t) goto 50
          cc=t
          id=l
          aop=a(k)
 50    continue
       c(i)=cc
       indd(nrpi)=id
       ao(i)=aop
       goto 60
 55    continue
       c(i)=mpzero
       indd(nrpi)=1
       d(i)=mpzero
 60   continue

c
c complete the sparse (CSR) matrix d which has only one nonzero entry
c in each row equal to 1.
c
      do 70 k=1,nr
         d(k)=1.d0
         indd(k)=1
 70   continue
c
c      write(*,*)c(1),d(1),indd(1),indd(2),ao(1)
c
      return
      end
