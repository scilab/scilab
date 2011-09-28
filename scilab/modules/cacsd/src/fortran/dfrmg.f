      subroutine dfrmg(job,na,nb,nc,l,m,n,a,b,c,freqr,freqi,
     +     gr,gi,rcond,w,ipvt)
      integer na,nb,nc,l,m,n,ipvt(n)
      double precision a(na,n),b(nb,m),c(nc,n),freqr,freqi
      double precision w(*),gr(nc,m),gi(nc,m)
      double precision rcond,ddot
      integer job
c     
c     *** purpose:
c     sfrmg takes real matrices a (n x n), b (n x m), and c (l x n)
c     and forms the complex frequency response matrix
c     g(freq) := c * (((freq * i) - a)-inverse) * b
c     where  i = (n x n) identity matrix and  freq  is a complex
c     scalar parameter taking values along the imaginary axis for
c     continuous-time systems and on the unit circle for discrete-
c     time systems.
c     
c     on entry:
c     job    integer
c     set = 0.  for the first call of dfrmg whereupon
c     it is set to 1 for all subsequent calls;
c     na       integer
c     the leading or row dimension of the real array a
c     (and the complex array h) as declared in the main
c     calling program.
c     
c     nb       integer
c     the leading or row dimension of the real array b
c     (and the complex array ainvb) as declared in the main
c     calling program.
c     
c     nc       integer
c     the leading or row dimension of the real array c
c     (and the complex array g) as declared in the main
c     calling program.
c     
c     l        integer
c     the number of rows of c (the number of outputs).
c     
c     m        integer
c     the number of columns of b (the number of inputs).
c     
c     n        integer
c     the order of the matrix a (the number of states);
c     also = number of columns of c = number of rows of b.
c     
c     a        real(na,n)
c     a real n x n matrix (the system matrix); not needed as
c     input if job .eq. .false.
c     
c     b        real(nb,m)
c     a real n x m matrix (the input matrix); not needed as
c     input if job .eq. 1
c     
c     c        real(nc,n)
c     a real l x n matrix (the output matrix); not needed as
c     input if job .eq. 1
c     
c     freq     complex
c     a complex scalar (the frequency parameter).
c     on return:
c     
c     g        complex(nc,m)
c     the frequency response matrix g(freq).
c     
c     a,b,c    a is in upper hessenberg form while b and c have been
c     arrays are not further modified.
c     rcond    real
c     parameter of subroutine checo (checo may be consulted
c     for details); normal return is then
c     (1.0 + rcond) .gt. 1.0.
c     
c     w (2*(n*n)+2*n)    tableau de travail
c     
c     ipvt(n)       tableau de travail entier
c     this version dated june 1982.
c     alan j. laub, university of southern california.
c     
c     subroutines and functions called:
c     
c     balanc(eispack) ,checo,chefa,chesl,hqr(eispack),shetr
c     
c     internal variables:
c     
      integer i,igh,j,k,kk,kp,low
      double precision t
c     
c     fortran functions called:
c     
c     
      if(job.ne.0) goto 150
      call balanc (na,n,a,low,igh,w)
c     
c     adjust b and c matrices based on information in the vector
c     w which describes the balancing of a and is defined in the
c     subroutine balanc
c     
      do 40 k = 1,n
         kk = n-k+1
         if (kk .ge. low .and. kk .le. igh) go to 40
         if (kk .lt. low) kk = low-kk
         kp = int(w(kk))
         if (kp .eq. kk) go to 40
c     
c     permute rows of b
c     
         do 20 j = 1,m
            t = b(kk,j)
            b(kk,j) = b(kp,j)
            b(kp,j) = t
 20      continue
c     
c     permute columns of c
c     
         do 30 i = 1,l
            t = c(i,kk)
            c(i,kk) = c(i,kp)
            c(i,kp) = t
 30      continue
c     
 40   continue
      if (igh .eq. low) go to 80
      do 70 k = low,igh
         t = w(k)
c     
c     scale columns of permuted c
c     
         do 50 i = 1,l
            c(i,k) = c(i,k)*t
 50      continue
c     
c     scale rows of permuted b
c     
         do 60 j = 1,m
            b(k,j) = b(k,j)/t
 60      continue
c     
 70   continue
 80   continue
c     
c     reduce a to hessenberg form by orthogonal similarities and
c     accumulate the orthogonal transformations into b and c
c     
      call dhetr (na,nb,nc,l,m,n,low,igh,a,b,c,w)
c     
      job = 1
c     
c     update  h := (freq *i) - a  with appropriate value of freq
c     
 150  continue
      nn=n*n
      j1=1-n
      call dset(2*nn,0.0d+0,w,1)
      do 170 j=1,n
         j1=j1+n
         call dcopy(min(j+1,n),a(1,j),1,w(j1),1)
         w(j1+j-1)=w(j1+j-1)-freqr
 170  continue
      call dset(n,-freqi,w(1+nn),n+1)
c     
c     factor the complex hessenberg matrix and estimate its
c     condition
c     
      izr=nn+nn+1
      izi=izr+n
      call wgeco(w(1),w(nn+1),n,n,ipvt,rcond,w(izr),w(izi))
      t = 1.0d+0+rcond
c      if (t .eq. 1.0d+0) goto 250
c     
c     compute  c*(h-inverse)*b
c     
      do 220 j = 1,m
         call dcopy(n,b(1,j),1,w(izr),1)
         call dset(n,0.0d+0,w(izi),1)
         call wgesl(w(1),w(nn+1),n,n,ipvt,w(izr),w(izi),0)
         do 240 i=1,l
            gr(i,j)=-ddot(n,c(i,1),nc,w(izr),1)
            gi(i,j)=-ddot(n,c(i,1),nc,w(izi),1)
 240     continue
 220  continue

      end
