      subroutine inva(nmax,n,a,z,ftest,eps,ndim,fail,ind)
      integer nmax,n,ftest,ndim,ind(n)
      logical fail
      double precision a(nmax,n),z(nmax,n),eps
c!purpose
c  given the upper real schur matrix a
c  with 1x1 or 2x2 diagonal blocks, this routine reorders the diagonal
c  blocks along with their generalized eigenvalues by constructing equi-
c  valence transformation. the transformation is also
c  performed on the given (initial) transformation z (resulting from a
c  possible previous step or initialized with the identity matrix).
c  after reordering, the eigenvalues inside the region specified by the
c  function ftest appear at the top. if ndim is their number then the
c  ndim first columns of z span the requested subspace.
c!calling sequence
c
c     subroutine inva (nmax,n,a,z,ftest,eps,ndim,fail,ind)
c     integer nmax,n,ftest,ndim,ind(n)
c     logical fail
c     double precision a(nmax,n),z(nmax,n),eps
c
c     nmax     the first dimension of a and z
c     n        the order of a and z
c    *a        the matrix whose blocks are to be reordered.
c    *z        upon return this array is multiplied by the column
c              transformation z.
c     ftest(ls,alpha,beta,s,p) an integer function describing the
c              spectrum of the invariant subspace to be computed:
c              when ls=1 ftest checks if alpha/beta is in that spectrum
c              when ls=2 ftest checks if the two complex conjugate
c              roots with sum s and product p are in that spectrum
c              if the answer is positive, ftest=1, otherwise ftest=-1
c     eps      the required absolute accuracy of the result
c    *ndim     an integer giving the dimension of the computed
c              invariant subspace
c    *fail     a logical variable which is false on a normal return,
c              true otherwise (when exchng fails)
c    *ind      an integer working array of dimension at least n
c
c!auxiliary routines
c     exchng
c     ftest  (user defined)
c!
c Copyright SLICOT
      external ftest
      integer l,ls,ls1,ls2,l1,ll,num,is,l2i,l2k,i,k,ii,istep,ifirst
      double precision s,p,alpha,beta
      integer         iero
      common /ierinv/ iero
      
      iero=0
      fail=.false.
      ndim=0
      num=0
      l=0
      ls=1
c ** construct array ind(i) where :
c **      abs(ind(i)) is the size of the block i
c **     sign(ind(i)) indicates the location of its eigenvalues
c **                  (as determined by ftest).
c ** num is the number of elements in this array
      do 40 ll=1,n
        l=l+ls
        if(l.gt.n) go to 50
        l1=l+1
        if(l1.gt.n) go to 20
        if(a(l1,l).eq.0.0d+0) go to 20
c  here a 2x2  block is checked *
        ls=2
        s=a(l,l)+a(l1,l1)
        p=a(l,l)*a(l1,l1)-a(l,l1)*a(l1,l)
        is=ftest(ls,alpha,beta,s,p)
        if(iero.gt.0) return
        go to 30
c  here a 1x1  block is checked *
 20     ls=1
        is=ftest(ls,a(l,l),1.0d+0,s,p)
        if(iero.gt.0) return
 30     num=num+1
        if(is.eq.1) ndim=ndim+ls
 40     ind(num)=ls*is
c **  reorder blocks such that those with positive value
c **    of ind(.) appear first.
 50   l2i=1
      do 90 i=1,num
        if(ind(i).gt.0) go to 90
c  if a negative ind(i) is encountered, then search for the first
c  positive ind(k) following on it
        l2k=l2i
        do 60 k=i,num
          if(ind(k).lt.0) go to 60
          go to 70
 60       l2k=l2k-ind(k)
c  if there are no positive indices following on a negative one
c  then stop
        go to 100
c  if a positive ind(k) follows on a negative ind(i) then
c  interchange block k before block i by performing k-i swaps
 70     istep=k-i
        ls2=ind(k)
        l=l2k
        do 80 ii=1,istep
          ifirst=k-ii
          ls1=-ind(ifirst)
          l=l-ls1
c          call exchng(a,z,n,l,ls1,ls2,eps,fail,nmax,nmax)
            call exch(nmax,n,a,z,l,ls1,ls2)
          if (fail) return
 80       ind(ifirst+1)=ind(ifirst)
        ind(i)=ls2
 90     l2i=l2i+ind(i)
 100  fail=.false.
      return
      end
