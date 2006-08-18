      subroutine hhdml(ktrans,nrowa,ncola,ioff,joff,nrowbl,ncolbl,
     1                 x,nx,qraux,a,na,mode,ierr)
c!purpose
c
c        to pre- or post-multiply a specified block of matrix a by the
c        orthogonal matrix q (or its transpose), where q is the
c        product of householder transformations which are stored as by
c        linpack routine dqrdc in arrays x and qraux.
c
c!method
c
c        the block of a to be transformed is the (nrowbl x ncolbl) one
c        with offset (ioff,joff), ie with first (top left) element
c        (ioff + 1,joff + 1).  this is operated on by the orthogonal
c        (ndimq x ndimq) q = h(1) * ... * h(ktrans) or its transpose,
c        where ndimq equals nrowbl for pre-multiplication and ncolbl
c        for post-multiplication.  each householder transformation
c        h(l) is completely described by the sub-vector stored in the
c        l-th element of qraux and the sub-diagonal part of the l-th
c        column of the (ndimq x ktrans) x.  note finally that ktrans
c        .le. ndimq.
c
c!reference
c
c        dongarra, j.j. et al
c        "linpack users' guide"
c        siam, 1979.  (chapter 9)
c
c!auxiliary routines
c
c        none
c
c! calling sequence
c
c        subroutine hhdml(ktrans,nrowa,ncola,ioff,joff,nrowbl,ncolbl,
c    1                    x,nx,qraux,a,na,mode,ierr)
c
c        integer ktrans,nrowa,ncola,ioff,joff,nrowbl,ncolbl,nx,na
c        integer mode,ierr
c
c        double precision x(nx,ktrans),qraux(ktrans),a(na,ncola)
c
c
c arguments in
c
c       ktrans   integer
c                -the number of householder transformations making up
c                q; declared first dimension of qraux and second
c                dimension of x
c
c       nrowa    integer
c                -the number of rows of matrix a
c
c       ncola    integer
c                -the number of columns of matrix a
c
c       ioff     integer
c                -the row offset of the specified block of a
c
c       joff     integer
c                -the column offset of the specified block of a
c
c       nrowbl   integer
c                -the number of rows of the specified block of a
c
c       ncolbl   integer
c                -the number of columns of the specified block of a
c
c       x        double precision(ndimq,ktrans)
c                -the matrix containing in its sub-diagonal part most
c                of the information necessary to construct q
c
c       nx       integer
c                -the declared first dimension of x.  note that
c                nx .ge. ndimq .ge. ktrans
c
c       qraux    double precision(ktrans)
c                -the remaining information necessary to construct q
c
c       a        double precision(nrowa,ncola)
c                -the matrix of which a specified block is to be
c                transformed.  note that this block is overwritten
c                here
c
c       na       integer
c                -the declared first dimension of a.  note that
c                na .ge. nrowa
c
c       mode     integer
c                -mode is a two-digit non-negative integer: its units
c                digit is 0 if q is to be applied and non-zero if
c                qtrans is, and its tens digit is 0 for post-multipli-
c                cation and non-zero for pre-multiplication
c
c arguments out
c
c       a        double precision(nrowa,ncola)
c                -the given matrix with specified block transformed
c
c       ierr     integer
c                -error indicator
c
c                ierr = 0        successful return
c
c                ierr = 1        nrowa .lt. (ioff + nrowbl)
c
c                ierr = 2        ncola .lt. (joff + ncolbl)
c
c                ierr = 3        ndimq does not lie in the interval
c                                ktrans, nx
c
c working space
c
c                none
c
c!originator
c
c                t.w.c.williams, control systems research group,
c                kingston polytechnic, march 16 1982
c
c!
c
      integer ktrans,nrowa,ncola,ioff,joff,nrowbl,ncolbl,nx,na
      integer mode,ierr
c
      double precision x(nx,ktrans),qraux(ktrans),a(na,ncola)
c
c     local variables:
      integer itrans,ipre,ndimq,iback,lstep,ia,ja,i,j,k,l
c
      double precision diag,temp
c
      double precision tau
c
c
      ierr = 0
c
      if ( (ioff + nrowbl) .le. nrowa) go to 10
c
      ierr = 1
      go to 150
c
 10   if ( (joff + ncolbl) .le. ncola) go to 20
c
      ierr = 2
      go to 150
c
c     itrans units digit of mode: 0 iff non-transposed q to be used
c
 20   itrans = mod(mode,10)
c
c     ipre 10 * (tens digit of mode): 0 iff post-multiplying ablk
c
      ipre = mode - itrans
c
      ndimq = ncolbl
      if (ipre .ne. 0) ndimq = nrowbl
      if ( (ktrans .le. ndimq) .and. (ndimq .le. nx) ) go to 30
c
      ierr = 3
      go to 150
c
c     iback 1 iff precisely one of itrans, ipre .ne. 0, ie iff the
c     householder transformations h(l) are applied in descending order
c
 30   iback = 0
      if (itrans .ne. 0) iback = 1
      if (ipre .ne. 0) iback = iback + 1
c
      if (iback .eq. 1) go to 40
c
c     initialization for h(l) applied in ascending order
c
      l = 1
      lstep = 1
      go to 50
c
c     initialization for h(l) applied in descending order
c
 40   l = ktrans
      lstep = -1
c
 50   if (ipre .eq. 0) go to 100
c
c     pre-multiply appropriate block of a by h(l) in correct order
c
      do 90 k = 1,ktrans
         diag = qraux(l)
         if (diag .eq. 0.0d+0) go to 90
         temp = x(l,l)
         x(l,l) = diag
c
c        operate on a one column at a time
c
         do 80 j = 1,ncolbl
            ja = joff + j
            tau = 0.0d+0
            do 60 i = l,nrowbl
               ia = ioff + i
 60            tau = tau + (x(i,l) * a(ia,ja) )
            tau = tau / diag
            do 70 i = l,nrowbl
               ia = ioff + i
 70            a(ia,ja) = a(ia,ja) - (tau * x(i,l) )
c
 80         continue
c
         x(l,l) = temp
 90      l = l + lstep
      go to 150
c
c     post-multiply appropriate block of a by h(l) in correct order
c
 100  continue
      do 140 k = 1,ktrans
         diag = qraux(l)
         if (diag .eq. 0.0d+0) go to 140
         temp = x(l,l)
         x(l,l) = diag
c
c        operate on a one row at a time
c
         do 130 i = 1,nrowbl
            ia = ioff + i
            tau = 0.0d+0
            do 110 j = l,ncolbl
               ja = joff + j
 110           tau = tau + (a(ia,ja) * x(j,l) )
            tau = tau / diag
            do 120 j = l,ncolbl
               ja = joff + j
 120           a(ia,ja) = a(ia,ja) - (tau * x(j,l) )
c
 130        continue
c
         x(l,l) = temp
 140     l = l + lstep
c
 150  continue
c
      return
      end
