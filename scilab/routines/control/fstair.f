      SUBROUTINE FSTAIR (A, E, Q, Z, M, N, ISTAIR, RANKE, TOL,
     *                   NBLCKS, IMUK, INUK, IMUK0, INUK0,
     *                   MNEI, WRK, IWRK,IERR)
C     PURPOSE:
C
C     Given a pencil sE-A where matrix E is in column echelon form the
C     subroutine FSTAIR computes according to the wishes of the user a
C     unitary transformed pencil Q(sE-A)Z which is more or less similar
C     to the generalized Schur form of the pencil sE-A.
C     The subroutine yields also part of the Kronecker structure of
C     the given pencil.
C
C     CONTRIBUTOR: Th.G.J. Beelen (Philips Glass Eindhoven).
C     Copyright SLICOT
C
C     REVISIONS: 1988, February 02.
C
C***********************************************************************
C
C      Philips Glass Eindhoven
C      5600 MD Eindhoven, Netherlands
C
C***********************************************************************
C          FSTAIR - SLICOT Library Routine Document
C
C 1 PURPOSE:
C
C   Given a pencil sE-A where matrix E is in column echelon form the
C   subroutine FSTAIR computes according to the wishes of the user a
C   unitary transformed pencil Q(sE-A)Z which is more or less similar
C   to the generalized Schur form of the pencil sE-A. The computed form
C   yields part of the Kronecker structure of the given pencil.
C
C 2 SPECIFICATION:
C
C   SUBROUTINE FSTAIR(A, LDA, E, Q, LDQ, Z, LDZ, M, N, ISTAIR, RANKE,
C                     NBLCKS, IMUK, INUK, IMUK0, INUK0, MNEI,
C                     WRK, IWRK, TOL, MODE, IERR)
C   INTEGER LDA, LDQ, LDZ, M, N, RANKE, NBLCKS, MODE, IERR
C   INTEGER ISTAIR(M), IMUK(N), INUK(M+1), IMUK0(N), INUK0(M+1),
C   INTEGER MNEI(4), IWRK(N)
C   DOUBLE PRECISION TOL
C   DOUBLE PRECISION WRK(N)
C   DOUBLE PRECISION A(LDA,N), E(LDA,N), Q(LDQ,M), Z(LDZ,N)
C
C 3 ARGUMENT LIST:
C
C   3.1 ARGUMENTS IN
C
C       A      - DOUBLE PRECISION array of DIMENSION (LDA,N).
C                The leading M x N part of this array contains the M x N
C                matrix A that has to be row compressed.
C                NOTE: this array is overwritten.
C
C       LDA    - INTEGER
C                LDA is the leading dimension of the arrays A and E.
C                (LDA >= M)
C
C       E      - DOUBLE PRECISION array of DIMENSION (LDA,N).
C                The leading M x N part of this array contains the M x N
C                matrix E which will be transformed equivalent to matrix
C                A.
C                On entry, matrix E has to be in column echelon form.
C                This may be accomplished by subroutine EREDUC.
C                NOTE: this array is overwritten.
C
C       Q      - DOUBLE PRECISION array of DIMENSION (LDQ,M).
C                The leading M x M part of this array contains an M x M
C                unitary row transformation matrix corresponding to the
C                row transformations of the matrices A and E which are
C                needed to transform an arbitrary pencil to a pencil
C                where E is in column echelon form.
C                NOTE: this array is overwritten.
C
C       LDQ    - INTEGER
C                LDQ is the leading dimension of the array Q.
C                (LDQ >= M)
C
C       Z      - DOUBLE PRECISION array of DIMENSION (LDZ,N).
C                The leading N x N part of this array contains an N x N
C                unitary column transformation matrix corresponding to
C                the column transformations of the matrices A and E
C                which are needed to transform an arbitrary pencil to
C                a pencil where E is in column echelon form.
C                NOTE: this array is overwritten.
C
C       LDZ    - INTEGER
C                LDZ is the leading dimension of the array Z.
C                (LDZ >= N)
C
C       M      - INTEGER
C      M is the number of rows of the matrices A, E and Q.
C
C       N      - INTEGER
C      N is the number of columns of the matrices A, E and Z.
C
C       ISTAIR - INTEGER array of DIMENSION (M).
C      ISTAIR contains the information on the column echelon
C      form of the input matrix E. This may be accomplished
C      by subroutine EREDUC.
C      ISTAIR(i) = + j   if the boundary element E(i,j) is a
C    corner point.
C        - j   if the boundary element E(i,j) is not
C    a corner point.
C      (i=1,...,M)
C      NOTE: this array is destroyed.
C
C       RANKE  - INTEGER
C      RANKE is the rank of the input matrix E being in column
C      echelon form.
C
C   3.2 ARGUMENTS OUT
C
C       A      - DOUBLE PRECISION array of DIMENSION (LDA,N).
C      The leading M x N part of this array contains the M x N
C      matrix A that has been row compressed while keeping E
C      in column echelon form.
C
C       E      - DOUBLE PRECISION array of DIMENSION (LDA,N).
C      The leading M x N part of this array contains the M x N
C      matrix E that has been transformed equivalent to matrix
C      A.
C
C       Q      - DOUBLE PRECISION array of DIMENSION (LDQ,M).
C      The leading M x M part of this array contains the M x M
C      unitary matrix Q which is the product of the input
C      matrix Q and the row transformation matrix which has
C      transformed the rows of the matrices A and E.
C
C       Z      - DOUBLE PRECISION array of DIMENSION (LDZ,N).
C      The leading N x N part of this array contains the N x N
C      unitary matrix Z which is the product of the input
C      matrix Z and the column transformation matrix which has
C      transformed the columns of the matrices A and E.
C
C       NBLCKS - INTEGER
C      NBLCKS is the number of submatrices having
C      full row rank >= 0  detected in matrix A.
C
C       IMUK   - INTEGER array of DIMENSION (N).
C      Array IMUK contains the column dimensions mu(k)
C      (k=1,...,NBLCKS) of the submatrices having full column
C      rank in the pencil sE(x)-A(x)
C      where  x = eps,inf  if MODE = 1 or 2
C       eps         MODE = 3 .
C
C       INUK   - INTEGER array of DIMENSION (M+1).
C      Array INUK contains the row dimensions nu(k)
C      (k=1,...,NBLCKS) of the submatrices having full row
C      rank in the pencil sE(x)-A(x)
C      where  x = eps,inf  if MODE = 1 or 2
C       eps         MODE = 3 .
C
C       IMUK0  - INTEGER array of DIMENSION (N).
C      Array IMUK0 contains the column dimensions mu(k)
C      (k=1,...,NBLCKS) of the submatrices having full column
C      rank in the pencil sE(eps,inf)-A(eps,inf).
C
C       INUK0  - INTEGER array of DIMENSION (M+1).
C      Array INUK0 contains the row dimensions nu(k)
C      (k=1,...,NBLCKS) of the submatrices having full row
C      rank in the pencil sE(eps,inf)-A(eps,inf).
C
C       MNEI   - INTEGER array of DIMENSION (4).
C      If MODE = 3 then
C      MNEI(1) = row    dimension of sE(eps)-A(eps)
C 2  = column dimension of sE(eps)-A(eps)
C 3  = row    dimension of sE(inf)-A(inf)
C 4  = column dimension of sE(inf)-A(inf)
C      If MODE = 1 or 2 then the array MNEI is empty.
C
C   3.3 WORK SPACE
C
C       WRK    - DOUBLE PRECISION array of DIMENSION (N).
C
C       IWRK   - INTEGER array of DIMENSION (N).
C
C   3.4 TOLERANCES
C
C       TOL    - DOUBLE PRECISION
C      TOL is the tolerance used when considering matrix
C      elements to be zero. TOL should be set to
C      TOL = RE * max( ||A|| , ||E|| ) + AE , where
C      ||.|| is the Frobenius norm. AE and RE are the absolute
C      and relative accuracy.
C      A recommanded choice is AE = EPS and RE = 100*EPS,
C      where EPS is the machine precision.
C
C   3.5 MODE PARAMETERS
C
C       MODE   - INTEGER
C      According to the value of MODE the subroutine FSTAIR
C      computes a generalized Schur form of the pencil sE-A,
C      where the structure of the generalized Schur form is
C      specified more the higher the value of MODE is.
C      (See also 6 DESCRIPTION).
C
C   3.6 ERROR INDICATORS
C
C       IERR   - INTEGER
C      On return, IERR contains 0 unless the subroutine
C      has failed.
C
C 4 ERROR INDICATORS and WARNINGS:
C
C   IERR = -1: the value of MODE is not 1, 2 or 3.
C   IERR =  0: succesfull completion.
C   IERR =  1: the algorithm has failed.
C
C 5 AUXILARY ROUTINES and COMMON BLOCKS:
C
C   BAE, SQUAEK, TRIRED from SLICOT.
C
C 6 DESCRIPTION:
C
C   On entry, matrix E is assumed to be in column echelon form.
C   Depending on the value of the parameter MODE, submatrices of A
C   and E will be reduced to a specific form. The higher the value of
C   MODE, the more the submatrices are transformed.
C
C   Step 1 of the algorithm.
C   If MODE = 1 then subroutine FSTAIR transforms the matrices A and
C   E to the following generalized Schur form by unitary transformations
C   Q1 and Z1, using subroutine BAE. (See also Algorithm 3.2.1 in [1]).
C
C                    | sE(eps,inf)-A(eps,inf) |      X     |
C       Q1(sE-A)Z1 = |------------------------|------------|
C                    |            O           | sE(r)-A(r) |
C
C   Here the pencil sE(eps,inf)-A(eps,inf) is in staircase form.
C   This pencil contains all Kronecker column indices and infinite
C   elementary divisors of the pencil sE-A.
C   The pencil sE(r)-A(r) contains all Kronecker row indices and
C   elementary divisors of sE-A.
C   NOTE: X is a pencil.
C
C   Step 2 of the algorithm.
C   If MODE = 2 then furthermore the submatrices having full row and
C   column rank in the pencil sE(eps,inf)-A(eps,inf) are triangularized
C   by applying unitary transformations Q2 and Z2 to Q1*(sE-A)*Z1. This
C   is done by subroutine TRIRED. (see also Algorithm 3.3.1 [1]).
C
C   Step 3 of the algorithm.
C   If MODE = 3 then moreover the pencils sE(eps)-A(eps) and
C   sE(inf)-A(inf) are separated in sE(eps,inf)-A(eps,inf) by applying
C   unitary transformations Q3 and Z3 to Q2*Q1*(sE-A)*Z1*Z2. This is
C   done by subroutine SQUAEK. (See also Algorithm 3.3.3 in [1]).
C   We then obtain
C
C              | sE(eps)-A(eps) |        X       |      X     |
C              |----------------|----------------|------------|
C              |        O       | sE(inf)-A(inf) |      X     |
C   Q(sE-A)Z = |=================================|============|  (1)
C              |             |            |
C              |                O                | sE(r)-A(r) |
C
C   where Q = Q3*Q2*Q1 and Z = Z1*Z2*Z3.
C   The accumulated row and column transformations are multiplied on
C   the left and right respectively with the contents of the arrays Q
C   and Z on entry and the results are stored in Q and Z, respectively.
C   NOTE: the pencil sE(r)-A(r) is not reduced furthermore.
C
C   Now let sE-A be an arbitrary pencil. This pencil has to be
C   transformed into a pencil with E in column echelon form before
C   calling FSTAIR. This may be accomplished by the subroutine EREDUC.
C   Let the therefore needed unitary row and column transformations be
C   Q0 and Z0, respectively.
C   Let, on entry, the arrays Q and Z contain Q0 and Z0, and let ISTAIR
C   contain the appropiate information. Then, on return with MODE = 3,
C   the contents of the arrays Q and Z are Q3*Q2*Q1*Q0 and Z0*Z1*Z2*Z3
C   which are the transformation matrices that transform the arbitrary
C   pencil sE-A into the form (1).
C
C 7 REFERENCES:
C
C   [1] Th.G.J. Beelen, New Algorithms for Computing the Kronecker
C       structure of a Pencil with Applications to Systems and Control
C       Theory, Ph.D.Thesis, Eindhoven University of Technology,
C       The Netherlands, 1987.
C
C 8 NUMERICAL ASPECTS:
C
C   It is shown in [1] that the algorithm is numerically backward
C   stable. The operations count is proportional to (max(M,N))**3 .
C
C 9 FURTHER REMARKS:
C
C   - The difference mu(k)-nu(k) = # Kronecker blocks of size kx(k+1).
C     The number of these blocks is given by NBLCKS.
C   - The difference nu(k)-mu(k+1) = # infinite elementary divisors of
C     degree k  (here mu(NBLCKS+1) := 0).
C   - MNEI(3) = MNEI(4) since pencil sE(inf)-A(inf) is regular.
C   - In the pencil sE(r)-A(r) the pencils sE(f)-A(f) and sE(eta)-A(eta)
C     can be separated by pertransposing the pencil sE(r)-A(r) and
C     using the last part of subroutine FSTAIR. The result has got to be
C     pertransposed again. (For more details see section 3.3.1 in [1]).
C
C***********************************************************************
C
C     .. Scalar arguments ..
C
      INTEGER LDA, LDQ, LDZ, M, N, RANKE, NBLCKS, MODE, IERR
      DOUBLE PRECISION TOL
C
C     .. Array arguments ..
C
      INTEGER ISTAIR(M), IMUK(N), INUK(M+1), IMUK0(N), INUK0(M+1),
     *        MNEI(4), IWRK(N)
      DOUBLE PRECISION A(M,N), E(M,N), Q(M,M), Z(N,N),
     *                 WRK(N)
C
C     EXTERNAL SUBROUTINES/FUNCTIONS:
C
C        BAE, SQUAEK, TRIRED from SLICOT.
C
C     Local variables.
C
      INTEGER MEI, NEI, IFIRA, IFICA, NRA, NCA, JK, RANKA,
     *        ISMUK, ISNUK, I, K
C
      LDA=M
      LDE=M
      LDQ=M
      LDZ=N
      MODE=3
      IERR = 0
C
C     A(k) is the submatrix in A that will be row compressed.
C
C     ISMUK= sum(i=1,..,k) MU(i), ISNUK= sum(i=1,...,k) NU(i),
C     IFIRA, IFICA: first row and first column index of A(k) in A.
C     NRA, NCA: number of rows and columns in A(k).
C
      IFIRA = 1
      IFICA = 1
      NRA = M
      NCA = N - RANKE
      ISNUK = 0
      ISMUK = 0
C
C     NBLCKS = # blocks detected in A with full row rank >= 0.
C
      NBLCKS = 0
      K = 0
C
C     Initialization of the arrays INUK and IMUK.
C
      DO 10 I = 1, M + 1
         INUK(I) = -1
   10 CONTINUE
C
C     Note: it is necessary that array INUK has dimension M+1 since it
C           is possible that M = 1 and NBLCKS = 2.
C           Example sE-A = (0 0 s -1).
C
      DO 20 I = 1, N
         IMUK(I) = -1
   20 CONTINUE
C
C     Compress the rows of A while keeping E in column echelon form.
C
C     REPEAT
C
   30    K = K + 1
         CALL BAE(A, LDA, E, Q, LDQ, Z, LDZ, M, N, ISTAIR, IFIRA,
     *            IFICA, NCA, RANKA, WRK, IWRK, TOL)
         IMUK(K) = NCA
         ISMUK = ISMUK + NCA

         INUK(K) = RANKA
         ISNUK = ISNUK + RANKA
         NBLCKS = NBLCKS + 1
C
C        If rank of A(k) = nrb then A has full row rank ;
C        JK = first column index (in A) after right most column of
C        matrix A(k+1).
C        (in case A(k+1) is empty, then JK = N+1).
C
         IFIRA = 1 + ISNUK
         IFICA = 1 + ISMUK
         IF (IFIRA .GT. M) THEN
            JK = N + 1
         ELSE
            JK = IABS(ISTAIR(IFIRA))
         END IF
         NRA = M - ISNUK
         NCA = JK - 1 - ISMUK
C
C        If NCA > 0 then there can be done some more row compression
C        of matrix A while keeping matrix E in column echelon form.
C
         IF (NCA .GT. 0) GOTO 30
C     UNTIL NCA <= 0
C
C     Matrix E(k+1) has full column rank since NCA = 0.
C     Reduce A and E by ignoring all rows and columns corresponding
C     to E(k+1).
C     Ignoring these columns in E changes the ranks of the
C     submatrices E(i), (i=1,...,k-1).
C
C     MEI and NEI are the dimensions of the pencil
C     sE(eps,inf)-A(eps,inf), i.e., the pencil that contains only
C     Kronecker column indices and infinity elementary divisors.
C
      MEI = ISNUK
      NEI = ISMUK
C
C     Save dimensions of the submatrices having full row or column rank
C     in pencil sE(eps,inf)-A(eps,inf), i.e., copy the arrays
C     IMUK and INUK to IMUK0 and INUK0, respectively.
C
      DO 40 I = 1, M + 1
         INUK0(I) = INUK(I)
   40 CONTINUE
C
      DO 50 I = 1, N
         IMUK0(I) = IMUK(I)
   50 CONTINUE
C
      IF (MODE .EQ. 1) RETURN
C
C     Triangularization of the submatrices in A and E.
C
      CALL TRIRED(A, LDA, E, Q, LDQ, Z, LDZ, M, N, NBLCKS, INUK, IMUK,
     *            IERR)
C
      IF (IERR .NE. 0) then
c      write(6,*) 'error: fstair failed!'
      return
      endif
C
      IF (MODE .EQ. 2) RETURN
C
C     Reduction to square submatrices E(k)'s in E.
C
      CALL SQUAEK(A, LDA, E, Q, LDQ, Z, LDZ, M, N, NBLCKS, INUK, IMUK,
     *            MNEI)
C
      RETURN
C *** Last line of FSTAIR *********************************************
      END
      SUBROUTINE SQUAEK(A, LDA, E, Q, LDQ, Z, LDZ, M, N, NBLCKS,
     *                  INUK, IMUK, MNEI)
C
C     PURPOSE:
C
C     On entry, it is assumed that the M by N matrices A and E have
C     been obtained after applying the Algorithms 3.2.1 and 3.3.1 to
C     the pencil s*E - A as described in [1], i.e.,
C
C                       | s*E(eps,inf)-A(eps,inf) |      X      |
C        Q(s*E - A)Z  = |-------------------------|-------------|
C                       |             0           | s*E(r)-A(r) |
C
C     Here the pencil s*E(eps,inf)-A(eps,inf) is in staircase form.
C     This pencil contains all Kronecker column indices and infinite
C     elementary divisors of the pencil s*E - A.
C     The pencil s*E(r)-A(r) contains all Kronecker row indices and
C     finite elementary divisors of s*E - A.
C     Furthermore, the submatrices having full row and column rank in
C     the pencil s*E(eps,inf)-A(eps,inf) are assumed to be triangu-
C     larized.
C     Subroutine SQUAEK separates the pencils s*E(eps)-A(eps) and
C     s*E(inf)-A(inf) in s*E(eps,inf)-A(eps,inf) using Algorithm 3.3.3
C     in [1]. The result then is
C
C    Q(s*E - A)Z =
C
C          | s*E(eps)-A(eps) |        X        |      X      |
C          |-----------------|-----------------|-------------|
C          |        0        | s*E(inf)-A(inf) |      X      |
C          |===================================|=============|
C          |               |             |
C          |                 0                 | s*E(r)-A(r) |
C
C     Note that the pencil s*E(r)-A(r) is not reduced furthermore.
C     REMARK: This routine is intended to be called only from the
C             SLICOT routine FSTAIR.
C
C     PARAMETERS:
C
C     A - DOUBLE PRECISION array of dimension (LDA,N).
C         On entry, it contains the matrix AA to be reduced.
C         On return, it contains the transformed matrix AA.
C     E - DOUBLE PRECISION array of dimension (LDA,N).
C         On entry, it contains the matrix EE to be reduced.
C         On return, it contains the transformed matrix EE.
C     Q - DOUBLE PRECISION array of dimension (LDQ,M).
C         On entry, Q contains the row transformations corresponding to
C         to the input matrices A and E.
C         On return, it contains the product of the input matrix Q and
C         the row transformation matrix that has transformed the rows
C         of the matrices A and E.
C     Z - DOUBLE PRECISION array of dimension (LDZ,N).
C         On entry, Z contains the column transformations corresponding
C         to the input matrices A and E.
C         On return, it contains the product of the input matrix Z and
C         the column transformation matrix that has transformed the
C         columns of the matrices A and E.
C     M - INTEGER.
C         Number of rows of A and E. 1 <= M <= LDA.
C     N - INTEGER.
C         Number of columns of A and E. N >= 1.
C     NBLCKS - INTEGER.
C         Number of submatrices having full row rank >=0 in A(eps,inf).
C     INUK - INTEGER array of dimension (NBLCKS).
C         On entry, INUK contains the row dimensions nu(k),
C         (k=1,..,NBLCKS) of the submatrices having full row rank in the
C         pencil s*E(eps,inf)-A(eps,inf).
C         On return, INUK contains the row dimensions nu(k),
C         (k=1,..,NBLCKS) of the submatrices having full row rank in the
C         pencil s*E(eps)-A(eps).
C     IMUK - INTEGER array of dimension (NBLCKS).
C         On entry, IMUK contains the column dimensions mu(k),
C         (k=1,..,NBLCKS) of the submatrices having full column rank in
C         the pencil s*E(eps,inf)-A(eps,inf).
C         On return, IMUK contains the column dimensions mnu(k),
C         (k=1,..,NBLCKS) of the submatrices having full column rank in
C         the pencil s*E(eps)-A(eps).
C     MNEI - INTEGER array of dimension (4).
C         MNEI(1) = MEPS = row    dimension of s*E(eps)-A(eps),
C              2  = NEPS = column dimension of s*E(eps)-A(eps),
C              3  = MINF = row    dimension of s*E(inf)-A(inf),
C              4  = NINF = column dimension of s*E(inf)-A(inf).
C
C     REFERENCES:
C
C     [1] Th.G.J. Beelen, New Algorithms for Computing the Kronecker
C         structure of a Pencil with Applications to Systems and
C         Control Theory, Ph.D.Thesis, Eindhoven University of
C         Technology, The Netherlands, 1987.
C
C     CONTRIBUTOR: Th.G.J. Beelen (Philips Glas Eindhoven)
C
C     REVISIONS: 1988, February 02.
C
C     Specification of the parameters.
C
C     .. Scalar arguments ..
C
      INTEGER LDA, LDQ, LDZ, M, N, NBLCKS
C
C     .. Array arguments ..
C
      DOUBLE PRECISION A(LDA,N), E(LDA,N), Q(LDQ,M), Z(LDZ,N)
      INTEGER INUK(NBLCKS), IMUK(NBLCKS), MNEI(4)
C
C     EXTERNAL SUBROUTINES:
C
C       DGIV, DROTI from SLICOT.
C
C     Local variables.
C
      DOUBLE PRECISION SC, SS
      INTEGER SK1P1, TK1P1, TP1, TP
      INTEGER ISMUK, ISNUK, MUKP1, MUK, NUK
      INTEGER IP, J, K, MUP, MUP1, NUP, NELM
      INTEGER MEPS, NEPS, MINF, NINF
      INTEGER RA, CA, RE, CE, RJE, CJE, CJA
C
C     Initialisation.
C
      ISMUK = 0
      ISNUK = 0
      DO 10 K = 1, NBLCKS
         ISMUK = ISMUK + IMUK(K)
         ISNUK = ISNUK + INUK(K)
   10 CONTINUE
C
C     MEPS, NEPS are the dimensions of the pencil s*E(eps)-A(eps).
C     MEPS = Sum(k=1,...,nblcks) NU(k),
C     NEPS = Sum(k=1,...,nblcks) MU(k).
C     MINF, NINF are the dimensions of the pencil s*E(inf)-A(inf).
C
      MEPS = ISNUK
      NEPS = ISMUK
      MINF = 0
      NINF = 0
C
C     MUKP1 = mu(k+1).  N.B. It is assumed that mu(NBLCKS + 1) = 0.
C
      MUKP1 = 0
C
      DO 60 K = NBLCKS, 1, -1
         NUK = INUK(K)
         MUK = IMUK(K)
C
C        Reduce submatrix E(k,k+1) to square matrix.
C        NOTE that always NU(k) >= MU(k+1) >= 0.
C
C        WHILE ( NU(k) >  MU(k+1) ) DO
   20    IF (NUK .GT. MUKP1) THEN
C
C           sk1p1 = sum(i=k+1,...,p-1) NU(i)
C           tk1p1 = sum(i=k+1,...,p-1) MU(i)
C           ismuk = sum(i=1,...,k) MU(i)
C           tp1 = sum(i=1,...,p-1) MU(i) = ismuk + tk1p1.
C
            SK1P1 = 0
            TK1P1 = 0
            DO 50 IP = K + 1, NBLCKS
C
C              Annihilate the elements originally present in the last
C              row of E(k,p+1) and A(k,p).
C              Start annihilating the first MU(p) - MU(p+1) elements by
C              applying column Givens rotations plus interchanging
C              elements.
C              Use original bottom diagonal element of A(k,k) as pivot.
C              Start position pivot in A = (ra,ca).
C
               TP1 = ISMUK + TK1P1
               RA = ISNUK + SK1P1
               CA = TP1
C
               MUP = IMUK(IP)
               MUP1 = INUK(IP)
               NUP = MUP1
C
               DO 30 J = 1, (MUP - NUP)
C
C                 CJA = current column index of pivot in A.
C
                  CJA = CA + J - 1
                  CALL DGIV(A(RA,CJA), A(RA,CJA+1), SC, SS)
C
C                 Apply transformations to A- and E-matrix.
C                 Interchange columns simultaneously.
C                 Update column transformation matrix Z.
C
                  NELM = RA
                  CALL DROTI(NELM, A(1,CJA), 1, A(1,CJA+1), 1, SC, SS)
                  A(RA,CJA) = 0.0D0
                  CALL DROTI(NELM, E(1,CJA), 1, E(1,CJA+1), 1, SC, SS)
                  CALL DROTI(N, Z(1,CJA), 1, Z(1,CJA+1), 1, SC, SS)
   30          CONTINUE
C
C              Annihilate the remaining elements originally present in
C              the last row of E(k,p+1) and A(k,p) by alternatingly
C              applying row and column rotations plus interchanging
C              elements.
C              Use diagonal elements of E(p,p+1) and original bottom
C              diagonal element of A(k,k) as pivots, respectively.
C              (re,ce) and (ra,ca) are the starting positions of the
C              pivots in E and A.
C
               RE = RA + 1
               TP = TP1 + MUP
               CE = 1 + TP
               CA = TP - MUP1
C
               DO 40 J = 1, MUP1
C
C                 (RJE,CJE) = current position pivot in E.
C
                  RJE = RE + J - 1
                  CJE = CE + J - 1
                  CJA = CA + J - 1
C
C                 Determine the row transformations.
C                 Apply these transformations to E- and A-matrix .
C                 Interchange the rows simultaneously.
C                 Update row transformation matrix Q.
C
                  CALL DGIV(E(RJE,CJE), E(RJE-1,CJE), SC, SS)
                  NELM = N - CJE + 1
                  CALL DROTI(NELM, E(RJE,CJE), LDA, E(RJE-1,CJE), LDA,
     *                       SC, SS)
                  E(RJE,CJE) = 0.0D0
                  NELM = N - CJA + 1
                  CALL DROTI(NELM, A(RJE,CJA), LDA, A(RJE-1,CJA), LDA,
     *                       SC, SS)
                  CALL DROTI(M, Q(RJE,1), LDQ, Q(RJE-1,1), LDQ, SC, SS)
C
C                 Determine the column transformations.
C                 Apply these transformations to A- and E-matrix.
C                 Interchange the columns simultaneously.
C                 Update column transformation matrix Z.
C
                  CALL DGIV(A(RJE,CJA), A(RJE,CJA+1), SC, SS)
                  NELM = RJE
                  CALL DROTI(NELM, A(1,CJA), 1, A(1,CJA+1), 1, SC, SS)
                  A(RJE,CJA) = 0.0D0
                  CALL DROTI(NELM, E(1,CJA), 1, E(1,CJA+1), 1, SC, SS)
                  CALL DROTI(N, Z(1,CJA), 1, Z(1,CJA+1), 1, SC, SS)
   40          CONTINUE
C
               SK1P1 = SK1P1 + NUP
               TK1P1 = TK1P1 + MUP
C
   50       CONTINUE
C
C           Reduce A=A(eps,inf) and E=E(eps,inf) by ignoring their last
C           row and right most column. The row and column ignored
C           belong to the pencil s*E(inf)-A(inf).
C           Redefine blocks in new A and E.
C
            MUK = MUK - 1
            NUK = NUK - 1
            IMUK(K) = MUK
            INUK(K) = NUK
            ISMUK = ISMUK - 1
            ISNUK = ISNUK - 1
            MEPS = MEPS - 1
            NEPS = NEPS - 1
            MINF = MINF + 1
            NINF = NINF + 1
C
            GOTO 20
         END IF
C        END WHILE 20
C
C        Now submatrix E(k,k+1) is square.
C
C        Consider next submatrix (k:=k-1).
C
         ISNUK = ISNUK - NUK
         ISMUK = ISMUK - MUK
         MUKP1 = MUK
   60 CONTINUE
C
C     If mu(NBLCKS) = 0, then the last submatrix counted in NBLCKS is
C     a 0 by 0 (empty) matrix. This "matrix" must be removed.
C
      IF (IMUK(NBLCKS) .EQ. 0) NBLCKS = NBLCKS - 1
C
C     Store dimensions of the pencils s*E(eps)-A(eps) and
C     s*E(inf)-A(inf) in array MNEI.
C
      MNEI(1) = MEPS
      MNEI(2) = NEPS
      MNEI(3) = MINF
      MNEI(4) = NINF
C
      RETURN
C *** Last line of SQUAEK *********************************************
      END
** END OF SQUAEKTEXT
      SUBROUTINE TRIAAK(A, LDA, E, Z, LDZ, N, NRA, NCA, IFIRA, IFICA)
C
C     PURPOSE:
C
C     Subroutine TRIAAK reduces a submatrix A(k) of A to upper triangu-
C     lar form by column Givens rotations only.
C     Here A(k) = A(IFIRA:ma,IFICA:na) where ma = IFIRA - 1 + NRA,
C     na = IFICA - 1 + NCA.
C     Matrix A(k) is assumed to have full row rank on entry. Hence, no
C     pivoting is done during the reduction process. See Algorithm 2.3.1
C     and Remark 2.3.4 in [1].
C     The constructed column transformations are also applied to matrix
C     E(k) = E(1:IFIRA-1,IFICA:na).
C     Note that in E columns are transformed with the same column
C     indices as in A, but with row indices different from those in A.
C     REMARK: This routine is intended to be called only from the
C             SLICOT auxiliary routine TRIRED.
C
C     PARAMETERS:
C
C     A - DOUBLE PRECISION array of dimension (LDA,N).
C         On entry, it contains the submatrix A(k) of full row rank
C         to be reduced to upper triangular form.
C         On return, it contains the transformed matrix A(k).
C     E - DOUBLE PRECISION array of dimension (LDA,N).
C         On entry, it contains the submatrix E(k).
C         On return, it contains the transformed matrix E(k).
C     Z - DOUBLE PRECISION array of dimension (LDZ,N).
C         On entry, Z contains the column transformations corresponding
C         to the input matrices A and E.
C         On return, it contains the product of the input matrix Z and
C         the column transformation matrix that has transformed the
C         columns of the matrices A and E.
C     N - INTEGER.
C         Number of columns of A and E. (N >= 1).
C     NRA - INTEGER.
C         Number of rows in A(k) to be transformed (1 <= NRA <= LDA).
C     NCA - INTEGER.
C         Number of columns in A(k) to be transformed (1 <= NCA <= N).
C     IFIRA - INTEGER.
C         Number of first row in A(k) to be transformed.
C     IFICA - INTEGER.
C         Number of first column in A(k) to be transformed.
C
C     REFERENCES:
C
C     [1] Th.G.J. Beelen, New Algorithms for Computing the Kronecker
C         structure of a Pencil with Applications to Systems and
C         Control Theory, Ph.D.Thesis, Eindhoven University of
C         Technology, The Netherlands, 1987.
C
C     CONTRIBUTOR: Th.G.J. Beelen (Philips Glas Eindhoven)
C
C     REVISIONS: 1988, January 29.
C
C     Specification of the parameters.
C
C     .. Scalar arguments ..
C
      INTEGER LDA, LDZ, N, NRA, NCA, IFIRA, IFICA
C
C     .. Array arguments ..
C
      DOUBLE PRECISION A(LDA,N), E(LDA,N), Z(LDZ,N)
C
C     EXTERNAL SUBROUTINES:
C
C       DROT from BLAS
C       DGIV from SLICOT.
C
C     Local variables.
C
      DOUBLE PRECISION SC, SS
      INTEGER I, II, J, JJ, JJPVT, IFICA1, IFIRA1, MNI, NELM
C
      IFIRA1 = IFIRA - 1
      IFICA1 = IFICA - 1
C
      DO 20 I = NRA, 1, -1
         II = IFIRA1 + I
         MNI = NCA - NRA + I
         JJPVT = IFICA1 + MNI
         NELM = IFIRA1 + I
         DO 10 J = MNI - 1, 1, -1
            JJ = IFICA1 + J
C
C           Determine the Givens transformation on columns jj and jjpvt.
C           Apply the transformation to these columns to annihilate
C           A(ii,jj) (from rows 1 up to ifira1+i).
C           Apply the transformation also to the E-matrix
C           (from rows 1 up to ifira1).
C           Update column transformation matrix Z.
C
            CALL DGIV(A(II,JJPVT), A(II,JJ), SC, SS)
            CALL DROT(NELM, A(1,JJPVT), 1, A(1,JJ), 1, SC, SS)
            A(II,JJ) = 0.0D0
            CALL DROT(IFIRA1, E(1,JJPVT), 1, E(1,JJ), 1, SC, SS)
            CALL DROT(N, Z(1,JJPVT), 1, Z(1,JJ), 1, SC, SS)
   10    CONTINUE
   20 CONTINUE
C
      RETURN
C *** Last line of TRIAAK *********************************************
      END
** END OF TRIAAKTEXT
*UPTODATE TRIAEKTEXT
      SUBROUTINE TRIAEK(A, LDA, E, Q, LDQ, M, N, NRE, NCE, IFIRE,
     *                  IFICE, IFICA)
C
C     PURPOSE:
C
C     Subroutine TRIAEK reduces a submatrix E(k) of E to upper triangu-
C     lar form by row Givens rotations only.
C     Here E(k) = E(IFIRE:me,IFICE:ne), where me = IFIRE - 1 + NRE,
C     ne = IFICE - 1 + NCE.
C     Matrix E(k) is assumed to have full column rank on entry. Hence,
C     no pivoting is done during the reduction process. See Algorithm
C     2.3.1 and Remark 2.3.4 in [1].
C     The constructed row transformations are also applied to matrix
C     A(k) = A(IFIRE:me,IFICA:N).
C     Note that in A(k) rows are transformed with the same row indices
C     as in E but with column indices different from those in E.
C     REMARK: This routine is intended to be called only from the
C             SLICOT auxiliary routine TRIRED.
C
C     PARAMETERS:
C
C     A - DOUBLE PRECISION array of dimension (LDA,N).
C         On entry, it contains the submatrix A(k).
C         On return, it contains the transformed matrix A(k).
C     E - DOUBLE PRECISION array of dimension (LDA,N).
C         On entry, it contains the submatrix E(k) of full column
C         rank to be reduced to upper triangular form.
C         On return, it contains the transformed matrix E(k).
C     Q - DOUBLE PRECISION array of dimension (LDQ,M).
C         On entry, Q contains the row transformations corresponding
C         to the input matrices A and E.
C         On return, it contains the product of the input matrix Q and
C         the row transformation matrix that has transformed the rows
C         of the matrices A and E.
C     M - INTEGER.
C         Number of rows of A and E. (1 <= M <= LDA).
C     N - INTEGER.
C         Number of columns of A and E. (N >= 1).
C     NRE - INTEGER
C         Number of rows in E to be transformed (1 <= NRE <= M).
C     NCE - INTEGER.
C         Number of columns in E to be transformed (1 <= NCE <= N).
C     IFIRE - INTEGER.
C         Index of first row in E to be transformed.
C     IFICE - INTEGER.
C         Index of first column in E to be transformed.
C     IFICA - INTEGER.
C         Index of first column in A to be transformed.
C
C     REFERENCES:
C
C     [1] Th.G.J. Beelen, New Algorithms for Computing the Kronecker
C         structure of a Pencil with Applications to Systems and
C         Control Theory, Ph.D.Thesis, Eindhoven University of
C         Technology, The Netherlands, 1987.
C
C     CONTRIBUTOR: Th.G.J. Beelen (Philips Glas Eindhoven)
C
C     REVISIONS: 1988, January 29.
C
C     Specification of the parameters.
C
C     .. Scalar arguments ..
C
      INTEGER LDA, LDQ, M, N, NRE, NCE, IFIRE, IFICE, IFICA
C
C     .. Array arguments ..
C
      DOUBLE PRECISION A(LDA,N), E(LDA,N), Q(LDQ,M)
C
C     EXTERNAL SUBROUTINES:
C
C       DROT from BLAS
C       DGIV from SLICOT.
C
C     Local variables.
C
      DOUBLE PRECISION SC, SS
      INTEGER I, II, IIPVT, J, JJ, IFICE1, IFIRE1, NELM
C
      IFIRE1 = IFIRE - 1
      IFICE1 = IFICE - 1
C
      DO 20 J = 1, NCE
         JJ = IFICE1 + J
         IIPVT = IFIRE1 + J
         DO 10 I = J + 1, NRE
            II = IFIRE1 + I
C
C           Determine the Givens transformation on rows ii and iipvt.
C           Apply the transformation to these rows (in whole E-matrix)
C           to annihilate E(ii,jj)  (from columns jj up to n).
C           Apply the transformations also to the A-matrix
C           (from columns ifica up to n).
C           Update the row transformation matrix Q.
C
            CALL DGIV(E(IIPVT,JJ), E(II,JJ), SC, SS)
            NELM = N - JJ + 1
            CALL DROT(NELM, E(IIPVT,JJ), LDA, E(II,JJ), LDA, SC, SS)
            E(II,JJ) = 0.0D0
            NELM = N - IFICA + 1
            CALL DROT(NELM, A(IIPVT,IFICA), LDA, A(II,IFICA), LDA,
     *                SC, SS)
            CALL DROT(M, Q(IIPVT,1), LDQ, Q(II,1), LDQ, SC, SS)
   10    CONTINUE
   20 CONTINUE
C
      RETURN
C *** Last line of TRIAEK *********************************************
      END
** END OF TRIAEKTEXT
*UPTODATE TRIREDTEXT
      SUBROUTINE TRIRED(A, LDA, E, Q, LDQ, Z, LDZ, M, N, NBLCKS,
     *                  INUK, IMUK, IERR)
C
C     PURPOSE:
C
C     On entry, it is assumed that the M by N matrices A and E have
C     been transformed to generalized Schur form by unitary trans-
C     formations (see Algorithm 3.2.1 in [1]), i.e.,
C
C                    | s*E(eps,inf)-A(eps,inf) |     X       |
C          s*E - A = |-------------------------|-------------| .
C                    |            0            | s*E(r)-A(r) |
C
C     Here the pencil s*E(eps,inf)-A(eps,inf) is in staircase form.
C     This pencil contains all Kronecker column indices and infinite
C     elementary divisors of the pencil s*E - A.
C     The pencil s*E(r)-A(r) contains all Kronecker row indices and
C     finite elementary divisors of s*E - A.
C     Subroutine TRIRED performs the triangularization of the sub-
C     matrices having full row and column rank in the pencil
C     s*E(eps,inf)-A(eps,inf) using Algorithm 3.3.1 in [1].
C     REMARK: This routine is intended to be called only from the
C             SLICOT routine FSTAIR.
C
C     PARAMETERS:
C
C     A - DOUBLE PRECISION array of dimension (LDA,N).
C         On entry, it contains the matrix A to be reduced.
C         On return, it contains the transformed matrix A.
C     E - DOUBLE PRECISION array of dimension (LDA,N).
C         On entry, it contains the matrix E to be reduced.
C         On return, it contains the transformed matrix E.
C     Q - DOUBLE PRECISION array of dimension (LDQ,M).
C         On entry, Q contains the row transformations corresponding
C         to the input matrices A and E.
C         On return, it contains the product of the input matrix Q and
C         the row transformation matrix that has transformed the rows
C         of the matrices A and E.
C     Z - DOUBLE PRECISION array of dimension (LDZ,N).
C         On entry, Z contains the column transformations corresponding
C         to the input matrices A and E.
C         On return, it contains the product of the input matrix Z and
C         the column transformation matrix that has transformed the
C         columns of the matrices A and E.
C     M - INTEGER.
C         Number of rows in A and E, 1 <= M <= LDA.
C     N - INTEGER.
C         Number of columns in A and E, N >= 1.
C     NBLCKS - INTEGER.
C         Number of submatrices having full row rank >=0 in A(eps,inf).
C     INUK - INTEGER array of dimension (NBLCKS).
C         Array containing the row dimensions nu(k) (k=1,..,NBLCKS)
C         of the submatrices having full row rank in the pencil
C         s*E(eps,inf)-A(eps,inf).
C     IMUK - INTEGER array of dimension (NBLCKS).
C         Array containing the column dimensions mu(k) (k=1,..,NBLCKS)
C         of the submatrices having full column rank in the pencil.
C     IERR - INTEGER.
C         IERR = 0, successful completion,
C                1, incorrect dimensions of a full row rank submatrix,
C                2, incorrect dimensions of a full column rank submatrix
C
C     REFERENCES:
C
C     [1] Th.G.J. Beelen, New Algorithms for Computing the Kronecker
C         structure of a Pencil with Applications to Systems and
C         Control Theory, Ph.D.Thesis, Eindhoven University of
C         Technology, The Netherlands, 1987.
C
C     CONTRIBUTOR: Th.G.J. Beelen (Philips Glas Eindhoven)
C
C     REVISIONS: 1988, January 29.
C
C     Specification of the parameters.
C
C     .. Scalar arguments ..
C
      INTEGER LDA, LDQ, LDZ, M, N, NBLCKS, IERR
C
C     .. Array arguments ..
C
      DOUBLE PRECISION A(LDA,N), E(LDA,N), Q(LDQ,M), Z(LDZ,N)
      INTEGER INUK(NBLCKS), IMUK(NBLCKS)
C
C     EXTERNAL SUBROUTINES:
C
C       TRIAAK, TRIAEK from SLICOT.
C
C     Local variables.
C
      INTEGER ISMUK, ISNUK1, IFIRA, IFICA, IFIRE, IFICE
      INTEGER I, K, MUK, MUKP1, NUK
C
C     ISMUK  = sum(i=1,...,k) MU(i),
C     ISNUK1 = sum(i=1,...,k-1) NU(i).
C
      ISMUK = 0
      ISNUK1 = 0
      DO 10 I = 1, NBLCKS
         ISMUK = ISMUK + IMUK(I)
         ISNUK1 = ISNUK1 + INUK(I)
   10 CONTINUE
C
C     NOTE:  ISNUK1 has not yet the correct value.
C
      IERR = 0
      MUKP1 = 0
      DO 20 K = NBLCKS, 1, -1
         MUK = IMUK(K)
         NUK = INUK(K)
         ISNUK1 = ISNUK1 - NUK
C
C        Determine left upper absolute coordinates of E(k) in E-matrix.
C
         IFIRE = 1 + ISNUK1
         IFICE = 1 + ISMUK
C
C        Determine left upper absolute coordinates of A(k) in A-matrix.
C
         IFIRA = IFIRE
         IFICA = IFICE - MUK
C
C        Reduce E(k) to upper triangular form using Givens
C        transformations on rows only. Apply the same transformations
C        to the rows of A(k).
C
         IF (MUKP1 .GT. NUK) THEN
            IERR = 1
            RETURN
         END IF
C
         CALL TRIAEK(A, LDA, E, Q, LDQ, M, N, NUK, MUKP1, IFIRE, IFICE,
     *               IFICA)
C
C        Reduce A(k) to upper triangular form using Givens
C        transformations on columns only. Apply the same transformations
C        to the columns in the E-matrix.
C
         IF (NUK .GT. MUK) THEN
            IERR = 2
            RETURN
         END IF
C
         CALL TRIAAK(A, LDA, E, Z, LDZ, N, NUK, MUK, IFIRA, IFICA)
C
         ISMUK = ISMUK - MUK
         MUKP1 = MUK
   20 CONTINUE
C
      RETURN
C *** Last line of TRIRED *********************************************
      END
      SUBROUTINE BAE(A, LDA, E, Q, LDQ, Z, LDZ, M, N, ISTAIR, IFIRA,
     *               IFICA, NCA, RANK, WRK, IWRK, TOL)
C
C     LIBRARY INDEX:
C
C
C
C     PURPOSE:
C
C     Let A and E be M x N matrices with E in column echelon form.
C     Let AA and EE be the following submatrices of A and E:
C       AA := A(IFIRA : M ; IFICA : N)
C       EE := E(IFIRA : M ; IFICA : N).
C     Let Aj and Ej be the following submatrices of AA and EE:
C       Aj := A(IFIRA : M ; IFICA : IFICA + NCA -1) and
C       Ej := E(IFIRA : M ; IFICA + NCA : N).
C
C     The subroutine BAE transforms (AA,EE) such that Aj is row
C     compressed while keeping matrix Ej in column echelon form
C     (which may be different from the form on entry).
C     In fact BAE performs the j-th step of Algorithm 3.2.1 in [1].
C     Furthermore, BAE determines the rank RANK of the submatrix Ej.
C     This is equal to the number of corner points in submatrix Ej.
C     REMARK: This routine is intended to be called only from the
C             SLICOT routine FSTAIR.
C
C     PARAMETERS:
C
C     A - DOUBLE PRECISION array of DIMENSION (LDA,N).
C         On entry, A(IFIRA : M ; IFICA : IFICA + NCA -1) contains the
C         matrix AA.
C         On return, it contains the matrix AA that has been row com-
C         pressed while keeping EE in column echelon form.
C     LDA - INTEGER.
C         LDA is the leading dimension of the arrays A and E. LDA >= M.
C     E - DOUBLE PRECISION array of DIMENSION (LDA,N).
C         On entry, E(IFIRA : M ; IFICA + NCA : N) contains the matrix
C         EE which is in column echelon form.
C         On return, it contains the transformed matrix EE which is kept
C         in column echelon form.
C     Q - DOUBLE PRECISION array of DIMENSION (LDQ,M).
C         On entry, the array Q contains the row transformations
C         corresponding to the input matrices A and E.
C         On return, it contains the M x M unitary matrix Q which is the
C         product of the input matrix Q and the row transformation
C         matrix that has transformed the rows of the matrices A and E.
C     LDQ - INTEGER.
C         LDQ is the leading dimension of the matrix Q. LDQ >= M.
C     Z - DOUBLE PRECISION array of DIMENSION (LDZ,N).
C         On entry, the array Z contains the column transformations
C         corresponding to the input matrices A and E.
C         On return, it contains the N x N unitary matrix Z which is the
C         product of the input matrix Z and the column transformation
C         matrix that has transformed the columns of A and E.
C     LDZ - INTEGER.
C         LDZ is the leading dimension of the matrix Z. LDZ >= N.
C     M - INTEGER.
C         M is the number of rows of the matrices A, E and Q. M >= 1.
C     N - INTEGER.
C         N is the number of columns of the matrices A, E and Z. N >= 1.
C     ISTAIR - INTEGER array of DIMENSION (M).
C         On entry, ISTAIR contains information on the column echelon
C         form of the input matrix E as follows:
C         ISTAIR(i) = + j: the boundary element E(i,j) is a corner point
C                     - j: the boundary element E(i,j) is not a corner
C                          point.
C         (i=1,...,M)
C         On return, ISTAIR contains the same information for the trans-
C         formed matrix E.
C     IFIRA - INTEGER.
C         IFIRA is the first row index of the submatrix Aj and Ej in
C         matrix A and E, respectively.
C     IFICA - INTEGER.
C         IFICA and IFICA + NCA are the first column index of the
C         submatrices Aj and Ej in the matrices A and E, respectively.
C     NCA - INTEGER.
C         NCA is the number of columns of the submatrix Aj in A.
C     RANK - INTEGER.
C         Numerical rank of the submatrix Ej in E (based on TOL).
C     WRK - DOUBLE PRECISION array of DIMENSION (N).
C         A real work space array.
C     IWRK - INTEGER array of DIMENSION (N).
C         An integer work space array.
C     TOL - DOUBLE PECISION.
C         TOL is the tolerance used when considering matrix elements to
C         be zero. TOL should be set to RE * max(||A||,||E||) + AE,
C         where ||.|| is the Frobenius norm. AE and RE are the absolute
C         and relative accuracy respectively.
C         A recommanded choice is AE = EPS and RE = 100*EPS, where EPS
C         is the machine precision.
C
C     REFERENCES:
C
C     [1] Th.G.J. Beelen, New Algorithms for Computing the Kronecker
C         structure of a Pencil with Applications to Systems and
C         Control Theory, Ph.D.Thesis, Eindhoven University of
C         Technology, The Netherlands, 1987.
C
C     CONTRIBUTOR: Th.G.J. Beelen (Philips Glass Eindhoven).
C
C     REVISIONS: 1988, January 29.
C
C     Specification of the parameters.
C
C     .. Scalar arguments ..
C
      INTEGER LDA, LDQ, LDZ, M, N, IFIRA, IFICA, NCA, RANK
      DOUBLE PRECISION TOL
C
C     .. Array arguments ..
C
      INTEGER ISTAIR(M), IWRK(N)
      DOUBLE PRECISION A(LDA,N), E(LDA,N), Q(LDQ,M), Z(LDZ,N), WRK(N)
C
C     EXTERNAL SUBROUTINES/FUNCTIONS:
C
C       IDAMAX, DROT, DSWAP from BLAS.
C       DGIV from SLICOT.
C
C     Local variables.
C
      INTEGER I, II, IMX, IP, IR, IST1, IST2, ISTPVT, ITYPE,
     *        IFIRA1, IFICA1, JPVT, JC1, JC2, NROWS,
     *        K, K1, KK, L, LSAV, LL, MK1, MXRANK, NELM, MJ, NJ
      DOUBLE PRECISION BMXNRM, BMX, SC, SS, EIJPVT
      LOGICAL LZERO
C
C     Initialisation.
C
C     NJ = number of columns in submatrix Aj,
C     MJ = number of rows in submatrices Aj and Ej.
C
      NJ = NCA
      MJ = M + 1 - IFIRA
      IFIRA1 = IFIRA - 1
      IFICA1 = IFICA - 1
      DO 10 I = 1, NJ
         IWRK(I) = I
   10 CONTINUE
      K = 1
      LZERO = .FALSE.
      RANK = MIN0(NJ,MJ)
      MXRANK = RANK
C
C     WHILE (K <= MXRANK) and (LZERO = FALSE) DO
   20 IF ((K .LE. MXRANK) .AND. (.NOT.LZERO)) THEN
C
C        Determine column in Aj with largest max-norm.
C
         BMXNRM = 0.0D0
         LSAV = K
         DO 30 L = K, NJ
C
C           IMX is relative index in column L of Aj where max el. is
C           found.
C           NOTE: the first el. in column L is in row K of matrix Aj.
C
            KK = IFIRA1 + K
            LL = IFICA1 + L
            IMX = IDAMAX(MJ - K + 1, A(KK,LL), 1)
            BMX = DABS(A(IMX + KK - 1, LL))
            IF (BMX .GT. BMXNRM) THEN
               BMXNRM = BMX
               LSAV = L
            END IF
   30    CONTINUE
C
         IF (BMXNRM .LT. TOL) THEN
C
C           Set submatrix of Aj to zero.
C
            DO 50 L = K, NJ
               LL = IFICA1 + L
               DO 40 I = K, MJ
                  II = IFIRA1 + I
                  A(II,LL) = 0.0D0
   40          CONTINUE
   50       CONTINUE
            LZERO = .TRUE.
            RANK = K - 1
         ELSE
C
C           Check whether columns have to be interchanged.
C
            IF (LSAV .NE. K) THEN
C
C              Interchange the columns in A which correspond to the
C              columns lsav and k in Aj. Store the permutation in IWRK.
C
               CALL DSWAP(M, A(1,IFICA1 + K), 1, A(1,IFICA1 + LSAV), 1)
               IP = IWRK(LSAV)
               IWRK(LSAV) = IWRK(K)
               IWRK(K) = IP
            END IF
C
            K1 = K + 1
            MK1 = NJ - K + 1 + (N - NCA - IFICA1)
            KK = IFICA1 + K
C
            DO 90 IR = K1, MJ
C
               I = MJ - IR + K1
C
C              II = absolute row number in A corresponding to row i in
C                   Aj.
C
               II = IFIRA1 + I
C
C              Construct Givens transformation to annihilate Aj(i,k).
C              Apply the row transformation to whole matrix A.
C              (NOT only to Aj).
C              Update row transformation matrix Q.
C
               CALL DGIV(A(II - 1,KK), A(II,KK), SC, SS)
               CALL DROT(MK1, A(II - 1,KK), LDA, A(II,KK), LDA, SC, SS)
               A(II,KK) = 0.0D0
               CALL DROT(M, Q(II - 1,1), LDQ, Q(II,1), LDQ, SC, SS)
C
C              Determine boundary type of matrix E at rows II-1 and II.
C
               IST1 = ISTAIR(II - 1)
               IST2 = ISTAIR(II)
               IF ((IST1 * IST2) .GT. 0) THEN
                  IF (IST1 .GT. 0) THEN
C
C                    boundary form = (* x)
C                                    (0 *)
C
                     ITYPE = 1
                  ELSE
C
C                    boundary form = (x x)
C                                    (x x)
C
                     ITYPE = 3
                  END IF
               ELSE
                  IF (IST1 .LT. 0) THEN
C
C                    boundary form = (x x)
C                                    (* x)
C
                     ITYPE=2
                  ELSE
C
C                    boundary form = (* x)
C                                    (0 x)
C
                     ITYPE = 4
                  END IF
               END IF
C
C              Apply row transformation also to matrix E.
C
C              JC1 = absolute number of the column in E in which stair
C                    element of row i-1 of Ej is present.
C              JC2 = absolute number of the column in E in which stair
C                    element of row i of Ej is present.
C
C              NOTE: JC1 < JC2   if ITYPE = 1.
C                    JC1 = JC2   if ITYPE = 2, 3 or 4.
C
               JC1 = IABS(IST1)
               JC2 = IABS(IST2)
               JPVT = MIN0(JC1,JC2)
               NELM = N - JPVT + 1
C
               CALL DROT(NELM, E(II-1,JPVT), LDA, E(II,JPVT), LDA,
     *                   SC, SS)
               EIJPVT = E(II,JPVT)
C
               GOTO (80, 60, 90, 70), ITYPE
C
   60          IF (DABS(EIJPVT) .LT. TOL) THEN
C                                                     (x x)    (* x)
C                 Boundary form has been changed from (* x) to (0 x)
C
                  ISTPVT = ISTAIR(II)
                  ISTAIR(II - 1) = ISTPVT
                  ISTAIR(II) = -(ISTPVT + 1)
                  E(II, JPVT) = 0.0D0
               END IF
               GOTO 90
C
   70          IF (DABS(EIJPVT) .GE. TOL) THEN
C
C                                                     (* x)    (x x)
C                 Boundary form has been changed from (0 x) to (* x)
C
                  ISTPVT = ISTAIR(II - 1)
                  ISTAIR(II - 1) = -ISTPVT
                  ISTAIR(II) = ISTPVT
               END IF
               GOTO 90
C
C              Construct column Givens transformation to annihilate
C              E(ii,jpvt).
C              Apply column Givens transformation to matrix E.
C              (NOT only to Ej).
C
   80          CALL DGIV(E(II,JPVT + 1), E(II,JPVT), SC, SS)
               CALL DROT(II, E(1,JPVT + 1), 1, E(1,JPVT), 1, SC, SS)
               E(II,JPVT) = 0.0D0
C
C              Apply this transformation also to matrix A.
C              (NOT only to Aj).
C              Update column transformation matrix Z.
C
               CALL DROT(M, A(1,JPVT + 1), 1, A(1,JPVT), 1, SC, SS)
               CALL DROT(N, Z(1,JPVT + 1), 1, Z(1,JPVT), 1, SC, SS)
C
   90       CONTINUE
C
            K = K + 1
         END IF
         GOTO 20
      END IF
C     END WHILE 20
C
C     Permute columns of Aj to original order.
C
      NROWS = IFIRA1 + RANK
      DO 120 I = 1, NROWS
         DO 100 K = 1, NJ
            KK = IFICA1 + K
            WRK(IWRK(K)) = A(I,KK)
  100    CONTINUE
         DO 110 K = 1, NJ
            KK = IFICA1 + K
            A(I,KK) = WRK(K)
  110    CONTINUE
  120 CONTINUE
C
      RETURN
C *** Last line of BAE ************************************************
      END
** END OF BAETEXT
*UPTODATE DGIVTEXT
      SUBROUTINE DGIV(DA, DB, DC, DS)
C
C     LIBRARY INDEX:
C
C     2.1.4  Decompositions and transformations.
C
C     PURPOSE:
C
C     This routine constructs the Givens transformation
C
C            ( DC  DS )
C        G = (        ),   DC**2 + DS**2 = 1.0D0 ,
C            (-DS  DC )
C                                 T                          T
C     such that the vector (DA,DB)  is transformed into (R,0), i.e.,
C
C            ( DC  DS ) ( DA )   ( R )
C            (        ) (    ) = (   )
C            (-DS  DC ) ( DB )   ( 0 ) .
C
C     This routine is a modification of the BLAS routine DROTG
C     (Algorithm 539) in order to leave the arguments DA and DB
C     unchanged. The value or R is not returned.
C
C     CONTRIBUTOR: P. Van Dooren (PRLB).
C
C     REVISIONS: 1987, November 24.
C
C     Specification of parameters.
C
C     .. Scalar Arguments ..
C
      DOUBLE PRECISION DA, DB, DC, DS
C
C     Local variables.
C
      DOUBLE PRECISION R, U, V
C
      IF (DABS(DA) .GT. DABS(DB)) THEN
         U = DA + DA
         V = DB/U
         R = DSQRT(0.25D0 + V**2) * U
         DC = DA/R
         DS = V * (DC + DC)
      ELSE
         IF (DB .NE. 0.0D0) THEN
            U = DB + DB
            V = DA/U
            R = DSQRT(0.25D0 + V**2) * U
            DS = DB/R
            DC = V * (DS + DS)
         ELSE
            DC = 1.0D0
            DS = 0.0D0
         END IF
      END IF
      RETURN
C *** Last line of DGIV ***********************************************
      END
** END OF DGIVTEXT
*UPTODATE DROTITEXT
      SUBROUTINE  DROTI (N, X, INCX, Y, INCY, C, S)
C
C     LIBRARY INDEX:
C
C     2.1.4 Decompositions and transfromations.
C
C     PURPOSE:
C
C     The subroutine DROTI performs the Givens transformation, defined
C     by C (cos) and S (sin), and interchanges the vectors involved,
C     i.e.,
C
C        |X(i)|    | 0   1 |   | C   S |   |X(i)|
C        |    | := |       | x |       | x |    |, i = 1,...N.
C        |Y(i)|    | 1   0 |   |-S   C |   |Y(i)|
C
C     REMARK. This routine is a modification of DROT from BLAS.
C
C     CONTRIBUTOR: Th.G.J. Beelen (Philips Glass Eindhoven)
C
C     REVISIONS: 1988, February 02.
C
C     Specification of the parameters.
C
C     .. Scalar argumants ..
C
      INTEGER INCX, INCY, N
      DOUBLE PRECISION C, S
C
C     .. Array arguments ..
C
      DOUBLE PRECISION X(*), Y(*)
C
C     Local variables.
C
      DOUBLE PRECISION DTEMP
      INTEGER I, IX, IY
C
      IF (N .LE. 0) RETURN
      IF ((INCX.NE.1) .OR. (INCY.NE.1)) THEN
C
C        Code for unequal increments or equal increments not equal to 1.
C
         IX = 1
         IY = 1
         IF (INCX.LT.0) IX = (-N+1) * INCX + 1
         IF (INCY.LT.0) IY = (-N+1) * INCY + 1
         DO 10 I = 1, N
            DTEMP  = C * Y(IY) - S * X(IX)
            Y(IY) = C * X(IX) + S * Y(IY)
            X(IX) = DTEMP
            IX = IX + INCX
            IY = IY + INCY
   10    CONTINUE
      ELSE
C
C        Code for both increments equal to 1.
C
         DO 20 I = 1, N
            DTEMP = C * Y(I) - S * X(I)
            Y(I) = C * X(I) + S * Y(I)
            X(I) = DTEMP
   20    CONTINUE
      END IF
      RETURN
C *** Last line if DROTI **********************************************
      END
