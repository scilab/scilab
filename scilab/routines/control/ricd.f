C/MEMBR ADD NAME=RICD,SSI=0
      subroutine ricd(nf,nn,f,n,h,g,cond,x,z,nz,w,eps,ipvt,wrk1,wrk2,
     &                ierr)
C!purpose
C     this subroutine solves the discrete-time algebraic matrix
C     riccati equation
C
C                t        t               t      -1    t
C           x = f *x*f - f *x*g1*((g2 + g1 *x*g1)  )*g1 *x*f + h
C
C     by laub's variant of the hamiltonian-eigenvector approach.
C
C!method
C      laub, a.j., a schur method for solving algebraic riccati
C           equations, ieee trans. aut. contr., ac-24(1979), 913-921.
C
C     the matrix f is assumed to be nonsingular and the matrices g1 and
C     g2 are assumed to be combined into the square array g as follows:
C                                    -1   t
C                           g = g1*g2  *g1
C
C     in case f is singular, see: pappas, t., a.j. laub, and n.r.
C       sandell, on the numerical solution of the discrete-time
C       algebraic riccati equation, ieee trans. aut. contr., ac-25(1980
C       631-641.
C
C!calling sequence
C     subroutine ricd (nf,nn,f,n,h,g,cond,x,z,nz,w,eps
C    +                    ipvt,wrk1,wrk2,ierr   )
C
C     integer nf,ng,nh,nz,n,nn,itype(nn),ipvt(n),ierr
C     double precision f(nf,n),g(ng,n),h(nh,n),z(nz,nn),w(nz,nn),
C    +                 ,wrk1(nn),wrk2(nn),x(nf,n)
C     on input:
C        nf,nz      row dimensions of the arrays containing
C                         (f,g,h) and (z,w), respectively, as
C                         declared in the calling program dimension
C                         statement;
C
C        n                order of the matrices f,g,h;
C
C        nn               = 2*n = order of the internally generated
C                         matrices z and w;
C
C        f                a nonsingular n x n (real) matrix;
C
C        g,h              n x n symmetric, nonnegative definite
C                         (real) matrices.
C
C      eps      relative machine precision
C
C
C     on output:
C
C        x                n x n array containing txe unique positive
C                         (or nonnegative) definite solution of the
C                         riccati equation;
C
C
C        z,w              2*n x 2*n real scratch arrays used for
C                         computations involving the symplectic
C                         matrix associated with the riccati equation;
C
C        wrk1,wrk2       real scratch vectors of lengths  2*n
C
C      cond
C                         condition number estimate for the final nth
C                         order linear matrix equation solved;
C
C        ipvt       integer scratch vector of length 2*n
C
C        ierr             error code
C                         ierr=0 : ok
C                         ierr=-1 : singular linear system
C                         ierr=i : i th eigenvalue is badly calculated
C
C     ***note:  all scratch arrays must be declared and included
C               in the call.***
C
C!comments
C     it is assumed that:
C        (1)  f is nonsingular (can be relaxed; see ref. above   )
C        (2)  g and h are nonnegative definite
C        (3)  (f,g1) is stabilizable and (c,f) is detectable where
C              t
C             c *c = h (c of full rank = rank(h)).
C     under these assumptions the solution (returned in the array h) is
C     unique and nonnegative definite.
C
C!originator
C     written by alan j. laub (dep't. of elec. engrg. - systems, univ.
C     of southern calif., los angeles, ca 90007; ph.: (213) 743-5535),
C     sep. 1977.
C     most recent version: apr. 15, 1981.
C
C!auxiliary routines
C     hqror2,inva,fout,mulwoa,mulwob
C     dgeco,dgesl (linpack   )
C     balanc,balbak,orthes,ortran (eispack   )
C     ddot (blas)
C!
C
C     *****parameters:
      integer nf,nz,n,nn,ipvt(nn),ierr
      double precision f(nf,n),g(nf,n),h(nf,n),z(nz,nn),w(nz,nn),
     &                 wrk1(nn),wrk2(nn),x(nf,n)
      logical fail
      integer fout
      external fout
C
C     *****local variables:
      integer i,j,low,igh,nlow,npi,npj,nup
      double precision eps,t(1),cond,det(2),ddot
C
C
C     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
C
C     eps is a  machine dependent parameter
C     specifying the relative precision of realing point arithmetic.
C     for example, eps = 16.0d+0**(-13) for double precision arithmetic
C     on ibm s360/s370.
C
C
C                             ( f**-1            (f**-1)*g             )
C  set up symplectic matrix z=(                                        )
C                             ( h*(f**-1)        h*(f**-1)*g+trans(f)  )
C
C z11=f**-1
      fail = .false.
      do 20 j = 1,n
        do 10 i = 1,n
          z(i,j) = f(i,j)
 10     continue
 20   continue
      call dgeco(z,nz,n,ipvt,cond,wrk1)
      if ((cond+1.0d+0) .le. 1.0d+0) goto 200
      call dgedi(z,nz,n,ipvt,det,wrk1,1)
C z21=h*f**-1; z12=(f**-1)*g
      do 90 j = 1,n
        npj = n + j
        do 90 i = 1,n
          npi = n + i
          z(i,npj) = ddot(n,z(i,1),nz,g(1,j),1)
          z(npi,j) = ddot(n,h(i,1),nf,z(1,j),1)
 90   continue
C z22=transp(f)+h*(f**-1)*g
      do 140 j = 1,n
        npj = n + j
        do 130 i = 1,n
          npi = n + i
          z(npi,npj) = f(j,i) + ddot(n,z(npi,1),nz,g(1,j),1)
 130    continue
 140  continue
C
C     balance z
C
      call balanc(nz,nn,z,low,igh,wrk1)
C
C     reduce z to real schur form with eigenvalues outside the unit
C     disk in the upper left n x n upper quasi-triangular block
C
      nlow = 1
      nup = nn
      call orthes(nz,nn,nlow,nup,z,wrk2)
      call ortran(nz,nn,nlow,nup,z,wrk2,w)
      call hqror2(nz,nn,1,nn,z,t,t,w,ierr,11)
      if (ierr .ne. 0) goto 210
      call inva(nz,nn,z,w,fout,eps,ndim,fail,ipvt)
      if (fail) goto 220
      if (ndim .ne. n) goto 230
C
C     compute solution of the riccati equation from the orthogonal
C     matrix now in the array w.  store the result in the array h.
C
      call balbak(nz,nn,low,igh,wrk1,nn,w)
C resolution systeme lineaire
      call dgeco(w,nz,n,ipvt,cond,wrk1)
      if (cond+1.0d+0 .le. 1.0d+0) goto 200
      do 160 j = 1,n
        npj = n + j
        do 150 i = 1,n
          x(i,j) = w(npj,i)
 150    continue
 160  continue
      do 165 i = 1,n
 165  call dgesl(w,nz,n,ipvt,x(1,i),1)
      return
 200  continue
C systeme lineaire numeriquement singulier
      ierr = -1
      return
 210  continue
C   erreur dans hqror2
      ierr = i
      return
C
 220  continue
C      erreur dans inva
      return
C
 230  continue
C    la matrice symplectique n'a pas le
C    bon nombre de val. propres  de module
C    inferieur a 1.
      return
C
C     last line of ricd
C
      end

