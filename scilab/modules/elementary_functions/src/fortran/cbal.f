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

      subroutine cbal(nm,n,ar,ai,low,igh,scale)
c
      integer i,j,k,l,m,n,jj,nm,igh,low,iexc
      double precision ar(nm,n),ai(nm,n),scale(n)
      double precision c,f,g,r,s,b2,radix
      logical noconv
c
c!purpose
c
c     this subroutine balances a complex matrix and isolates
c     eigenvalues whenever possible.
c
c!calling sequence
c     subroutine cbal(nm,n,ar,ai,low,igh,scale)
c
c     integer n,nm,igh,low
c     double precision ar(nm,n),ai(nm,n),scale(n)
c     logical noconv
c
c     on input:
c
c        nm must be set to the row dimension of two-dimensional
c          array parameters as declared in the calling program
c          dimension statement;
c
c        n is the order of the matrix;
c
c        ar and ai contain the real and imaginary parts,
c          respectively, of the complex matrix to be balanced.
c
c     on output:
c
c        ar and ai contain the real and imaginary parts,
c          respectively, of the balanced matrix;
c
c        low and igh are two integers such that ar(i,j) and ai(i,j)
c          are equal to zero if
c           (1) i is greater than j and
c           (2) j=1,...,low-1 or i=igh+1,...,n;
c
c        scale contains information determining the
c           permutations and scaling factors used.
c
c     suppose that the principal submatrix in rows low through igh
c     has been balanced, that p(j) denotes the index interchanged
c     with j during the permutation step, and that the elements
c     of the diagonal matrix used are denoted by d(i,j).  then
c        scale(j) = p(j),    for j = 1,...,low-1
c                 = d(j,j)       j = low,...,igh
c                 = p(j)         j = igh+1,...,n.
c     the order in which the interchanges are made is n to igh+1,
c     then 1 to low-1.
c
c     note that 1 is returned for igh if igh is zero formally.
c
c     the algol procedure exc contained in cbalance appears in
c     cbal  in line.  (note that the algol roles of identifiers
c     k,l have been reversed.)
c
c     arithmetic is real throughout.
c
c!originator
c     this subroutine is a translation of the algol procedure
c     cbalance, which is a complex version of balance,
c     num. math. 13, 293-304(1969) by parlett and reinsch.
c     handbook for auto. comp., vol.ii-linear algebra, 315-326(1971).
c     questions and comments should be directed to b. s. garbow,
c     applied mathematics division, argonne national laboratory
c
c!
c     ------------------------------------------------------------------
c
c     :::::::::: radix is a machine dependent parameter specifying
c                the base of the machine floating point representation.
c                radix = 16.0d+0 for long form arithmetic
c                on s360 ::::::::::
      data radix/2.0d+0/
c
      b2 = radix * radix
      k = 1
      l = n
      go to 100
c     :::::::::: in-line procedure for row and
c                column exchange ::::::::::
   20 scale(m) = j
      if (j .eq. m) go to 50
c
      do 30 i = 1, l
         f = ar(i,j)
         ar(i,j) = ar(i,m)
         ar(i,m) = f
         f = ai(i,j)
         ai(i,j) = ai(i,m)
         ai(i,m) = f
   30 continue
c
      do 40 i = k, n
         f = ar(j,i)
         ar(j,i) = ar(m,i)
         ar(m,i) = f
         f = ai(j,i)
         ai(j,i) = ai(m,i)
         ai(m,i) = f
   40 continue
c
   50 go to (80,130), iexc
c     :::::::::: search for rows isolating an eigenvalue
c                and push them down ::::::::::
   80 if (l .eq. 1) go to 280
      l = l - 1
c     :::::::::: for j=l step -1 until 1 do -- ::::::::::
  100 do 120 jj = 1, l
         j = l + 1 - jj
c
         do 110 i = 1, l
            if (i .eq. j) go to 110
            if (ar(j,i) .ne. 0.0d+0 .or. ai(j,i) .ne. 0.0d+0) go to 120
  110    continue
c
         m = l
         iexc = 1
         go to 20
  120 continue
c
      go to 140
c     :::::::::: search for columns isolating an eigenvalue
c                and push them left ::::::::::
  130 k = k + 1
c
  140 do 170 j = k, l
c
         do 150 i = k, l
            if (i .eq. j) go to 150
            if (ar(i,j) .ne. 0.0d+0 .or. ai(i,j) .ne. 0.0d+0) go to 170
  150    continue
c
         m = k
         iexc = 2
         go to 20
  170 continue
c     :::::::::: now balance the submatrix in rows k to l ::::::::::
      do 180 i = k, l
  180 scale(i) = 1.0d+0
c     :::::::::: iterative loop for norm reduction ::::::::::
  190 noconv = .false.
c
      do 270 i = k, l
         c = 0.0d+0
         r = 0.0d+0
c
         do 200 j = k, l
            if (j .eq. i) go to 200
            c = c + abs(ar(j,i)) + abs(ai(j,i))
            r = r + abs(ar(i,j)) + abs(ai(i,j))
  200    continue
c     :::::::::: guard against zero c or r due to underflow ::::::::::
         if (c .eq. 0.0d+0 .or. r .eq. 0.0d+0) go to 270
         g = r / radix
         f = 1.0d+0
         s = c + r
  210    if (c .ge. g) go to 220
         f = f * radix
         c = c * b2
         go to 210
  220    g = r * radix
  230    if (c .lt. g) go to 240
         f = f / radix
         c = c / b2
         go to 230
c     :::::::::: now balance ::::::::::
  240    if ((c + r) / f .ge. 0.950d+0 * s) go to 270
         g = 1.0d+0 / f
         scale(i) = scale(i) * f
         noconv = .true.
c
         do 250 j = k, n
            ar(i,j) = ar(i,j) * g
            ai(i,j) = ai(i,j) * g
  250    continue
c
         do 260 j = 1, l
            ar(j,i) = ar(j,i) * f
            ai(j,i) = ai(j,i) * f
  260    continue
c
  270 continue
c
      if (noconv) go to 190
c
  280 low = k
      igh = l
      return
c     :::::::::: last card of cbal ::::::::::
      end
