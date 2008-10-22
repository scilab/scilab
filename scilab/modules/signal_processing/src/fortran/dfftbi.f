      subroutine dfftbi(a, b, nseg, n, nspn, isn,ierr,lout,lnow,
     +      lused,lmax,lbook,rstak,istak)
c!
c arrays a and b originally hold the real and imaginary
c      components of the data, and return the real and
c      imaginary components of the resulting fourier coefficients.
c multivariate data is indexed according to the fortran
c      array element successor function, without limit
c      on the number of implied multiple subscripts.
c      the subroutine is called once for each variate.
c      the calls for a multivariate transform may be in any order.
c
c n is the dimension of the current variable.
c nspn is the spacing of consecutive data values
c      while indexing the current variable.
c nseg*n*nspn is the total number of complex data values.
c the sign of isn determines the sign of the complex
c      exponential, and the magnitude of isn is normally one.
c      the magnitude of isn determines the indexing increment for a&b.
c
c if fft is called twice, with opposite signs on isn, an
c      identity transformation is done...calls can be in either order.
c      the results are scaled by 1/n when the sign of isn is positive.
c
c a tri-variate transform with a(n1,n2,n3), b(n1,n2,n3)
c is computed by
c        call fft(a,b,n2*n3,n1,1,-1)
c        call fft(a,b,n3,n2,n1,-1)
c        call fft(a,b,1,n3,n1*n2,-1)
c
c a single-variate transform of n complex data values is computed by
c        call fft(a,b,1,n,1,-1)
c
c the data may alternatively be stored in a single complex
c      array a, then the magnitude of isn changed to two to
c      give the correct indexing increment and a(2) used to
c      pass the initial address for the sequence of imaginary
c      values, e.g.
c        call fft(a,a(2),nseg,n,nspn,-2)
c
c array nfac is working storage for factoring n.  the smallest
c      number exceeding the 15 locations provided is 12,754,584.
c!
      implicit double precision (a-h,o-z)
      dimension a(*), b(*), rstak(*),istak(*),nfac(15)
c
c
      integer isize(5)
      data isize(1),isize(2),isize(3),isize(4),isize(5)/1,1,1,2,2/
c
      ierr=0
c
c determine the factors of n
c
      m = 0
      nf = abs(n)
      k = nf
      if (nf.eq.1) return
      nspan = abs(nf*nspn)
      ntot = abs(nspan*nseg)
      if (isn*ntot.ne.0) go to 20
      ierr=1
      return
c
  10  m = m + 1
      nfac(m) = 4
      k = k/16
  20  if (k-(k/16)*16.eq.0) go to 10
      j = 3
      jj = 9
      go to 40
  30  m = m + 1
      nfac(m) = j
      k = k/jj
  40  if (mod(k,jj).eq.0) go to 30
      j = j + 2
      jj = j**2
      if (jj.le.k) go to 40
      if (k.gt.4) go to 50
      kt = m
      nfac(m+1) = k
      if (k.ne.1) m = m + 1
      go to 90
  50  if (k-(k/4)*4.ne.0) go to 60
      m = m + 1
      nfac(m) = 2
      k = k/4
c all square factors out now, but k .ge. 5 still
  60  kt = m
      maxp = max(kt+kt+2,k-1)
      j = 2
  70  if (mod(k,j).ne.0) go to 80
      m = m + 1
      nfac(m) = j
      k = k/j
  80  j = ((j+1)/2)*2 + 1
      if (j.le.k) go to 70
  90  if (m.le.kt+1) maxp = m + kt + 1
      if (m+kt.gt.15) go to 120
      if (kt.eq.0) go to 110
      j = kt
 100  m = m + 1
      nfac(m) = nfac(j)
      j = j - 1
      if (j.ne.0) go to 100
c
 110  maxf = m - kt
      maxf = nfac(maxf)
      if (kt.gt.0) maxf = max(nfac(kt),maxf)
c    MAJOR MODIFICATION
      do 9999 kkk=1,m
      maxf=max(maxf,nfac(kkk))
 9999 continue
c      j = istkgt(maxf*4,3)
      nitems=maxf*4
c     following line modified FD & MG
      itype=4
c xxxxxxxxxxxxxxxxxxxxxxxxxxx
      istkgt = (lnow*isize(2)-1)/isize(itype) + 2
      i = ((istkgt-1+nitems)*isize(itype)-1)/isize(2) + 3
      if (i.gt.lmax) go to 1010
      istak(i-1) = itype
      istak(i) = lnow
      lout = lout + 1
      lnow = i
      lused = max(lused,lnow)
      goto 1300
c
 1010 continue
      ierr=-i
      return
 1300 continue
      j=istkgt
      jj = j + maxf
      j2 = jj + maxf
      j3 = j2 + maxf
c      k = istkgt(maxp,2)
      nitems=maxp
      itype=2
c     xxxxxxxxxxxxxxxxxxxxxxxxxx
      istkgt = (lnow*isize(2)-1)/isize(itype) + 2
      i = ((istkgt-1+nitems)*isize(itype)-1)/isize(2) + 3
      if (i.gt.lmax) go to 11
      istak(i-1) = itype
      istak(i) = lnow
      lout = lout + 1
      lnow = i
      lused = max(lused,lnow)
      goto 1400
c
  11  continue
      ierr=-i
      return
 1400 continue
      k=istkgt
c     la carte suivante est a supprimer si simple precision
c     next instruction commented by FD&MG (simulog residue?)
c    ********************************************
c      k=2*k-1
c    *********************************************
      call dfftmx(a, b, ntot, nf, nspan, isn, m, kt, rstak(j),
     *    rstak(jj), rstak(j2), rstak(j3), istak(k), nfac)
c      call istkrl(2)
      k=2
c  xxxxxxxxxxxxxxx
      in = 2
c
      if (lbook.le.lnow .and. lnow.le.lused .and. lused.le.lmax) go to
     *    13
      ierr = 3
      return
  13  if (in.le.0) goto 1500
      if (lbook.gt.istak(lnow) .or. istak(lnow).ge.lnow-1) go to 21
      lout = lout - 1
      lnow = istak(lnow)
      in = in - 1
      go to 13
c
  21  ierr = 4
      return
 1500 continue
      return
c
  120 continue
      ierr=2
      return
      end
