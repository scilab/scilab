      SUBROUTINE DGGES( JOBVSL, JOBVSR, SORT, DELCTG, N, A, LDA, B, LDB,
     $                  SDIM, ALPHAR, ALPHAI, BETA, VSL, LDVSL, VSR,
     $                  LDVSR, WORK, LWORK, BWORK, INFO )
*
*  -- LAPACK driver routine (version 2.1) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 15, 1996
*
*     .. Scalar Arguments ..
      CHARACTER          JOBVSL, JOBVSR, SORT
      INTEGER            INFO, LDA, LDB, LDVSL, LDVSR, LWORK, N, SDIM
*     ..
*     .. Array Arguments ..
      LOGICAL            BWORK( * )
      DOUBLE PRECISION   A( LDA, * ), ALPHAI( * ), ALPHAR( * ),
     $                   B( LDB, * ), BETA( * ), VSL( LDVSL, * ),
     $                   VSR( LDVSR, * ), WORK( * )
*     ..
*     .. Function Arguments ..
      LOGICAL            DELCTG
      EXTERNAL           DELCTG
*     ..
*
*  Purpose
*  =======
*
*  DGGES computes for a pair of N-by-N real nonsymmetric matrices (A,B),
*  the generalized eigenvalues, the generalized real Schur form (S,T),
*  optionally, the left and/or right matrices of Schur vectors (VSL and
*  VSR). This gives the generalized Schur factorization
*
*           (A,B) = ( (VSL)*S*(VSR)**T, (VSL)*T*(VSR)**T )
*
*  Optionally, it also orders the eigenvalues so that a selected cluster
*  of eigenvalues appears in the leading diagonal blocks of the upper
*  quasi-triangular matrix S and the upper triangular matrix T.The
*  leading columns of VSL and VSR then form an orthonormal basis for the
*  corresponding left and right eigenspaces (deflating subspaces).
*
*  (If only the generalized eigenvalues are needed, use the driver
*  DGGEV instead, which is faster.)
*
*  A generalized eigenvalue for a pair of matrices (A,B) is a scalar w
*  or a ratio alpha/beta = w, such that  A - w*B is singular.  It is
*  usually represented as the pair (alpha,beta), as there is a
*  reasonable interpretation for beta=0 or both being zero.
*
*  A pair of matrices (S,T) is in generalized real Schur form if T is
*  upper triangular with non-negative diagonal and S is block upper
*  triangular with 1-by-1 and 2-by-2 blocks.  1-by-1 blocks correspond
*  to real generalized eigenvalues, while 2-by-2 blocks of S will be
*  "standardized" by making the corresponding elements of T have the
*  form:
*          [  a  0  ]
*          [  0  b  ]
*
*  and the pair of corresponding 2-by-2 blocks in S and T will have a
*  complex conjugate pair of generalized eigenvalues.
*
*
*  Arguments
*  =========
*
*  JOBVSL  (input) CHARACTER*1
*          = 'N':  do not compute the left Schur vectors;
*          = 'V':  compute the left Schur vectors.
*
*  JOBVSR  (input) CHARACTER*1
*          = 'N':  do not compute the right Schur vectors;
*          = 'V':  compute the right Schur vectors.
*
*  SORT    (input) CHARACTER*1
*          Specifies whether or not to order the eigenvalues on the
*          diagonal of the generalized Schur form.
*          = 'N':  Eigenvalues are not ordered;
*          = 'S':  Eigenvalues are ordered (see DELZTG);
*
*  DELZTG  (input) LOGICAL FUNCTION of three DOUBLE PRECISION arguments
*          DELZTG must be declared EXTERNAL in the calling subroutine.
*          If SORT = 'N', DELZTG is not referenced.
*          If SORT = 'S', DELZTG is used to select eigenvalues to sort
*          to the top left of the Schur form.
*          An eigenvalue (ALPHAR(j)+ALPHAI(j))/BETA(j) is selected if
*          DELZTG(ALPHAR(j),ALPHAI(j),BETA(j)) is true; i.e. if either
*          one of a complex conjugate pair of eigenvalues is selected,
*          then both complex eigenvalues are selected.
*
*          Note that in the ill-conditioned case, a selected complex
*          eigenvalue may no longer satisfy DELZTG(ALPHAR(j),ALPHAI(j),
*          BETA(j)) = .TRUE. after ordering. INFO is to be set to N+2
*          in this case.
*
*  N       (input) INTEGER
*          The order of the matrices A, B, VSL, and VSR.  N >= 0.
*
*  A       (input/output) DOUBLE PRECISION array, dimension (LDA, N)
*          On entry, the first of the pair of matrices.
*          On exit, A has been overwritten by its generalized Schur
*          form S.
*
*  LDA     (input) INTEGER
*          The leading dimension of A.  LDA >= max(1,N).
*
*  B       (input/output) DOUBLE PRECISION array, dimension (LDB, N)
*          On entry, the second of the pair of matrices.
*          On exit, B has been overwritten by its generalized Schur
*          form T.
*
*  LDB     (input) INTEGER
*          The leading dimension of B.  LDB >= max(1,N).
*
*  SDIM    (output) INTEGER
*          If SORT = 'N', SDIM = 0.
*          If SORT = 'S', SDIM = number of eigenvalues (after sorting)
*          for which DELZTG is true.  (Complex conjugate pairs for which
*          DELZTG is true for either eigenvalue count as 2.)
*
*  ALPHAR  (output) DOUBLE PRECISION array, dimension (N)
*  ALPHAI  (output) DOUBLE PRECISION array, dimension (N)
*  BETA    (output) DOUBLE PRECISION array, dimension (N)
*          On exit, (ALPHAR(j) + ALPHAI(j)*i)/BETA(j), j=1,...,N, will
*          be the generalized eigenvalues.  ALPHAR(j) + ALPHAI(j)*i,
*          and  BETA(j),j=1,...,N are the diagonals of the complex Schur
*          form (S,T) that would result if the 2-by-2 diagonal blocks of
*          the real Schur form of (A,B) were further reduced to
*          triangular form using 2-by-2 complex unitary transformations.
*          If ALPHAI(j) is zero, then the j-th eigenvalue is real; if
*          positive, then the j-th and (j+1)-st eigenvalues are a
*          complex conjugate pair, with ALPHAI(j+1) negative.
*
*          Note: the quotients ALPHAR(j)/BETA(j) and ALPHAI(j)/BETA(j)
*          may easily over- or underflow, and BETA(j) may even be zero.
*          Thus, the user should avoid naively computing the ratio.
*          However, ALPHAR and ALPHAI will be always less than and
*          usually comparable with norm(A) in magnitude, and BETA always
*          less than and usually comparable with norm(B).
*
*  VSL     (output) DOUBLE PRECISION array, dimension (LDVSL,N)
*          If JOBVSL = 'V', VSL will contain the left Schur vectors.
*          Not referenced if JOBVSL = 'N'.
*
*  LDVSL   (input) INTEGER
*          The leading dimension of the matrix VSL. LDVSL >=1, and
*          if JOBVSL = 'V', LDVSL >= N.
*
*  VSR     (output) DOUBLE PRECISION array, dimension (LDVSR,N)
*          If JOBVSR = 'V', VSR will contain the right Schur vectors.
*          Not referenced if JOBVSR = 'N'.
*
*  LDVSR   (input) INTEGER
*          The leading dimension of the matrix VSR. LDVSR >= 1, and
*          if JOBVSR = 'V', LDVSR >= N.
*
*  WORK    (workspace/output) DOUBLE PRECISION array, dimension (LWORK)
*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK.  LWORK >= 8*N+16 must be 7*(N+1)+16
*
*  BWORK   (workspace) LOGICAL array, dimension (N)
*          Not referenced if SORT = 'N'.
*
*  INFO    (output) INTEGER
*          = 0:  successful exit
*          < 0:  if INFO = -i, the i-th argument had an illegal value.
*          = 1,...,N:
*                The QZ iteration failed.  (A,B) are not in Schur
*                form, but ALPHAR(j), ALPHAI(j), and BETA(j) should
*                be correct for j=INFO+1,...,N.
*          > N:  =N+1: other than QZ iteration failed in DHGEQZ.
*                =N+2: after reordering, roundoff changed values of
*                      some complex eigenvalues so that leading
*                      eigenvalues in the Generalized Schur form no
*                      longer satisfy DELZTG=.TRUE.  This could also
*                      be caused due to scaling.
*                =N+3: reordering failed in DTGSEN.
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            CURSL, ILASCL, ILBSCL, ILVSL, ILVSR, LASTSL,
     $                   LST2SL, WANTST
      INTEGER            I, ICOLS, IERR, IHI, IJOBVL, IJOBVR, ILEFT,
     $                   ILO, IP, IRIGHT, IROWS, ITAU, IWRK, MAXWRK,
     $                   MINWRK
      DOUBLE PRECISION   ANRM, ANRMTO, BIGNUM, BNRM, BNRMTO, EPS, PVSL,
     $                   PVSR, SAFMAX, SAFMIN, SMLNUM
*     ..
*     .. Local Arrays ..
      INTEGER            IDUM( 1 )
      DOUBLE PRECISION   DIF( 2 )
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGEQRF, DGGBAK, DGGBAL, DGGHRD, DHGEQZ, DLABAD,
     $                   DLACPY, DLASCL, DLASET, DORGQR, DORMQR, DTGSEN,
     $                   XERBLA
*     ..
*     .. External Functions ..
      character*120 text
      LOGICAL            LSAME
      INTEGER            ILAENV
      DOUBLE PRECISION   DLAMCH, DLANGE
      EXTERNAL           LSAME, ILAENV, DLAMCH, DLANGE
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          MAX, SQRT
*     ..
*     .. Executable Statements ..
*
*     Decode the input arguments
*
	IF( LSAME( JOBVSL, 'N' ) ) THEN
         IJOBVL = 1
         ILVSL = .FALSE.
      ELSE IF( LSAME( JOBVSL, 'V' ) ) THEN
         IJOBVL = 2
         ILVSL = .TRUE.
      ELSE
         IJOBVL = -1
         ILVSL = .FALSE.
      END IF
*
      IF( LSAME( JOBVSR, 'N' ) ) THEN
         IJOBVR = 1
         ILVSR = .FALSE.
      ELSE IF( LSAME( JOBVSR, 'V' ) ) THEN
         IJOBVR = 2
         ILVSR = .TRUE.
      ELSE
         IJOBVR = -1
         ILVSR = .FALSE.
      END IF
*
      WANTST = LSAME( SORT, 'S' )
*
*     Test the input arguments
*
      INFO = 0
      IF( IJOBVL.LE.0 ) THEN
         INFO = -1
      ELSE IF( IJOBVR.LE.0 ) THEN
         INFO = -2
      ELSE IF( ( .NOT.WANTST ) .AND. ( .NOT.LSAME( SORT, 'N' ) ) ) THEN
         INFO = -3
      ELSE IF( N.LT.0 ) THEN
         INFO = -5
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDVSL.LT.1 .OR. ( ILVSL .AND. LDVSL.LT.N ) ) THEN
         INFO = -15
      ELSE IF( LDVSR.LT.1 .OR. ( ILVSR .AND. LDVSR.LT.N ) ) THEN
         INFO = -17
      END IF
*
*     Compute workspace
*      (Note: Comments in the code beginning "Workspace:" describe the
*       minimal amount of workspace needed at that point in the code,
*       as well as the preferred amount for good performance.
*       NB refers to the optimal block size for the immediately
*       following subroutine, as returned by ILAENV.)
*
      MINWRK = 1
      IF( INFO.EQ.0 .AND. LWORK.GE.1 ) THEN
         MINWRK = 7*( N+1 ) + 16
         MAXWRK = 7*( N+1 ) + N*ILAENV( 1, 'DGEQRF', ' ', N, 1, N, 0 ) +
     $            16
         IF( ILVSL ) THEN
            MAXWRK = MAX( MAXWRK, 7*( N+1 )+N*
     $               ILAENV( 1, 'DORGQR', ' ', N, 1, N, -1 ) )
         END IF
         WORK( 1 ) = MAXWRK
      END IF
*
      IF( LWORK.LT.MINWRK )
     $   INFO = -19
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'DGGES ', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.EQ.0 ) THEN
         SDIM = 0
         RETURN
      END IF
*
*     Get machine constants
*
      EPS = DLAMCH( 'P' )
      SAFMIN = DLAMCH( 'S' )
      SAFMAX = ONE / SAFMIN
      CALL DLABAD( SAFMIN, SAFMAX )
      SMLNUM = SQRT( SAFMIN ) / EPS
      BIGNUM = ONE / SMLNUM
*
*     Scale A if max element outside range [SMLNUM,BIGNUM]
*
      ANRM = DLANGE( 'M', N, N, A, LDA, WORK )
      ILASCL = .FALSE.
      IF( ANRM.GT.ZERO .AND. ANRM.LT.SMLNUM ) THEN
         ANRMTO = SMLNUM
         ILASCL = .TRUE.
      ELSE IF( ANRM.GT.BIGNUM ) THEN
         ANRMTO = BIGNUM
         ILASCL = .TRUE.
      END IF
      IF( ILASCL )
     $   CALL DLASCL( 'G', 0, 0, ANRM, ANRMTO, N, N, A, LDA, IERR )
*
*     Scale B if max element outside range [SMLNUM,BIGNUM]
*
      BNRM = DLANGE( 'M', N, N, B, LDB, WORK )
      ILBSCL = .FALSE.
      IF( BNRM.GT.ZERO .AND. BNRM.LT.SMLNUM ) THEN
         BNRMTO = SMLNUM
         ILBSCL = .TRUE.
      ELSE IF( BNRM.GT.BIGNUM ) THEN
         BNRMTO = BIGNUM
         ILBSCL = .TRUE.
      END IF
      IF( ILBSCL )
     $   CALL DLASCL( 'G', 0, 0, BNRM, BNRMTO, N, N, B, LDB, IERR )
*
*     Permute the matrix to make it more nearly triangular
*     (Workspace: need 6*N + 2*N space for storing balancing factors)
*
      ILEFT = 1
      IRIGHT = N + 1
      IWRK = IRIGHT + N
      CALL DGGBAL( 'P', N, A, LDA, B, LDB, ILO, IHI, WORK( ILEFT ),
     $             WORK( IRIGHT ), WORK( IWRK ), IERR ) 
*
*     Reduce B to triangular form (QR decomposition of B)
*     (Workspace: need N, prefer N*NB)
*
      IROWS = IHI + 1 - ILO
      ICOLS = N + 1 - ILO
      ITAU = IWRK
      IWRK = ITAU + IROWS
      CALL DGEQRF( IROWS, ICOLS, B( ILO, ILO ), LDB, WORK( ITAU ),
     $             WORK( IWRK ), LWORK+1-IWRK, IERR )
*
*     Apply the orthogonal transformation to matrix A
*     (Workspace: need N, prefer N*NB)
*
      CALL DORMQR( 'L', 'T', IROWS, ICOLS, IROWS, B( ILO, ILO ), LDB,
     $             WORK( ITAU ), A( ILO, ILO ), LDA, WORK( IWRK ),
     $             LWORK+1-IWRK, IERR )
*
*     Initialize VSL
*     (Workspace: need N, prefer N*NB)
*
      IF( ILVSL ) THEN
         CALL DLASET( 'Full', N, N, ZERO, ONE, VSL, LDVSL )
         CALL DLACPY( 'L', IROWS-1, IROWS-1, B( ILO+1, ILO ), LDB,
     $                VSL( ILO+1, ILO ), LDVSL )
         CALL DORGQR( IROWS, IROWS, IROWS, VSL( ILO, ILO ), LDVSL,
     $                WORK( ITAU ), WORK( IWRK ), LWORK+1-IWRK, IERR )
      END IF
*
*     Initialize VSR
*
      IF( ILVSR )
     $   CALL DLASET( 'Full', N, N, ZERO, ONE, VSR, LDVSR )
*
*     Reduce to generalized Hessenberg form
*     (Workspace: none needed)
*
      CALL DGGHRD( JOBVSL, JOBVSR, N, ILO, IHI, A, LDA, B, LDB, VSL,
     $             LDVSL, VSR, LDVSR, IERR )
*
*     Perform QZ algorithm, computing Schur vectors if desired
*     (Workspace: need N)
*
      IWRK = ITAU
      CALL DHGEQZ( 'S', JOBVSL, JOBVSR, N, ILO, IHI, A, LDA, B, LDB,
     $             ALPHAR, ALPHAI, BETA, VSL, LDVSL, VSR, LDVSR,
     $             WORK( IWRK ), LWORK+1-IWRK, IERR )
      IF( IERR.NE.0 ) THEN
         IF( IERR.GT.0 .AND. IERR.LE.N ) THEN
            INFO = IERR
         ELSE IF( IERR.GT.N .AND. IERR.LE.2*N ) THEN
            INFO = IERR - N
         ELSE
            INFO = N + 1
         END IF
         GO TO 40
      END IF
*
*     Sort eigenvalues ALPHA/BETA if desired
*     (Workspace: need 4*N+16 )
*
      SDIM = 0
      IF( WANTST ) THEN
*
*        Undo scaling on eigenvalues before DELZTGing
*
         IF( ILASCL ) THEN
            CALL DLASCL( 'G', 0, 0, ANRMTO, ANRM, N, 1, ALPHAR, N,
     $                   IERR )
            CALL DLASCL( 'G', 0, 0, ANRMTO, ANRM, N, 1, ALPHAI, N,
     $                   IERR )
         END IF
         IF( ILBSCL )
     $      CALL DLASCL( 'G', 0, 0, BNRMTO, BNRM, N, 1, BETA, N, IERR )
*
*        Select eigenvalues
*
         DO 10 I = 1, N
            BWORK( I ) = DELCTG( ALPHAR( I ), ALPHAI( I ), BETA( I ) )
   10    CONTINUE
         CALL DTGSEN( 0, ILVSL, ILVSR, BWORK, N, A, LDA, B, LDB, ALPHAR,
     $                ALPHAI, BETA, VSL, LDVSL, VSR, LDVSR, SDIM, PVSL,
     $                PVSR, DIF, WORK( IWRK ), LWORK-IWRK+1, IDUM, 1,
     $                IERR )
          IF( IERR.EQ.1 )
     $      INFO = N + 3
*
      END IF
*
*     Apply back-permutation to VSL and VSR
*     (Workspace: none needed)
*
      IF( ILVSL )
     $   CALL DGGBAK( 'P', 'L', N, ILO, IHI, WORK( ILEFT ),
     $                WORK( IRIGHT ), N, VSL, LDVSL, IERR )
*
      IF( ILVSR )
     $   CALL DGGBAK( 'P', 'R', N, ILO, IHI, WORK( ILEFT ),
     $                WORK( IRIGHT ), N, VSR, LDVSR, IERR )
*
*     Check if unscaling would cause over/underflow, if so, rescale
*     (ALPHAR(I),ALPHAI(I),BETA(I)) so BETA(I) is on the order of
*     B(I,I) and ALPHAR(I) and ALPHAI(I) are on the order of A(I,I)
*
      DO 50 I = 1, N
         IF( ALPHAI( I ).NE.ZERO ) THEN
            IF( ( BETA( I )/SAFMAX ).GT.( BNRMTO/BNRM ) .OR.
     $          ( SAFMIN/BETA( I ) ).GT.( BNRM/BNRMTO ) ) THEN
               WORK( 1 ) = ABS(B( I, I )/BETA( I ))
               BETA( I ) = BETA( I )*WORK( 1 )
               ALPHAR( I ) = ALPHAR( I )*WORK( 1 )
               ALPHAI( I ) = ALPHAI( I )*WORK( 1 )
            ELSE IF( ( ALPHAR( I )/SAFMAX ).GT.( ANRMTO/ANRM ) .OR.
     $          ( SAFMIN/ALPHAR( I ) ).GT.( ANRM/ANRMTO ) ) THEN
               WORK( 1 ) = ABS( A( I, I )/ALPHAR( I ) )
               BETA( I ) = BETA( I )*WORK( 1 )
               ALPHAR( I ) = ALPHAR( I )*WORK( 1 )
               ALPHAI( I ) = ALPHAI( I )*WORK( 1 )
            ELSE IF( ( ALPHAI( I )/SAFMAX ).GT.( ANRMTO/ANRM ) .OR.
     $          ( SAFMIN/ALPHAI( I ) ).GT.( ANRM/ANRMTO ) ) THEN
               WORK( 1 ) = ABS( A( I, I+1 )/ALPHAI( I ) )
               BETA( I ) = BETA( I )*WORK( 1 )
               ALPHAR( I ) = ALPHAR( I )*WORK( 1 )
               ALPHAI( I ) = ALPHAI( I )*WORK( 1 )
            END IF
         END IF
   50 CONTINUE
*
*     Undo scaling
*
      IF( ILASCL ) THEN
         CALL DLASCL( 'H', 0, 0, ANRMTO, ANRM, N, N, A, LDA, IERR )
         CALL DLASCL( 'G', 0, 0, ANRMTO, ANRM, N, 1, ALPHAR, N, IERR )
         CALL DLASCL( 'G', 0, 0, ANRMTO, ANRM, N, 1, ALPHAI, N, IERR )
      END IF
*
      IF( ILBSCL ) THEN
         CALL DLASCL( 'U', 0, 0, BNRMTO, BNRM, N, N, B, LDB, IERR )
         CALL DLASCL( 'G', 0, 0, BNRMTO, BNRM, N, 1, BETA, N, IERR )
      END IF
*
   20 CONTINUE
*
      IF( WANTST ) THEN
*
*        Check if reordering is correct
*
         LASTSL = .TRUE.
         LST2SL = .TRUE.
         SDIM = 0
         IP = 0
         DO 30 I = 1, N
            CURSL = DELCTG( ALPHAR( I ), ALPHAI( I ), BETA( I ) )
            IF( ALPHAI( I ).EQ.ZERO ) THEN
               IF( CURSL )
     $            SDIM = SDIM + 1
               IP = 0
               IF( CURSL .AND. .NOT.LASTSL )
     $            INFO = N + 2
            ELSE
               IF( IP.EQ.1 ) THEN
*
*                 Last eigenvalue of conjugate pair
*
                  CURSL = CURSL .OR. LASTSL
                  LASTSL = CURSL
                  IF( CURSL )
     $               SDIM = SDIM + 2
                  IP = -1
                  IF( CURSL .AND. .NOT.LST2SL )
     $               INFO = N + 2
               ELSE
*
*                 First eigenvalue of conjugate pair
*
                  IP = 1
               END IF
            END IF
            LST2SL = LASTSL
            LASTSL = CURSL
   30    CONTINUE
*
      END IF
*
   40 CONTINUE
*
      WORK( 1 ) = MAXWRK
*
      RETURN
*
*     End of DGGES
*
      END

      SUBROUTINE DLAGS2( UPPER, A1, A2, A3, B1, B2, B3, CSU, SNU, CSV,
     $                   SNV, CSQ, SNQ )
*
*  -- LAPACK auxiliary routine (version 2.1) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     September 30, 1994
*
*     .. Scalar Arguments ..
      LOGICAL            UPPER
      DOUBLE PRECISION   A1, A2, A3, B1, B2, B3, CSQ, CSU, CSV, SNQ,
     $                   SNU, SNV
*     ..
*
*  Purpose
*  =======
*
*  DLAGS2 computes 2-by-2 orthogonal matrices U, V and Q, such
*  that if ( UPPER ) then
*
*            U'*A*Q = U'*( A1 A2 )*Q = ( x  0  )
*                        ( 0  A3 )     ( x  x  )
*  and
*            V'*B*Q = V'*( B1 B2 )*Q = ( x  0  )
*                        ( 0  B3 )     ( x  x  )
*
*  or if ( .NOT.UPPER ) then
*
*            U'*A*Q = U'*( A1 0  )*Q = ( x  x  )
*                        ( A2 A3 )     ( 0  x  )
*  and
*            V'*B*Q = V'*( B1 0  )*Q = ( x  x  )
*                        ( B2 B3 )     ( 0  x  )
*
*  The rows of the transformed A and B are parallel, where
*
*    U = (  CSU  SNU ), V = (  CSV SNV ), Q = (  CSQ   SNQ )
*        ( -SNU  CSU )      ( -SNV CSV )      ( -SNQ   CSQ )
*
*  Z' denotes the transpose of Z.
*
*
*  Arguments
*  =========
*
*  UPPER   (input) LOGICAL
*          = .TRUE.: the input matrices A and B are upper triangular.
*          = .FALSE.: the input matrices A and B are lower triangular.
*
*  A1      (input) DOUBLE PRECISION
*  A2      (input) DOUBLE PRECISION
*  A3      (input) DOUBLE PRECISION
*          On entry, A1, A2 and A3 are elements of the input 2-by-2
*          upper (lower) triangular matrix A.
*
*  B1      (input) DOUBLE PRECISION
*  B2      (input) DOUBLE PRECISION
*  B3      (input) DOUBLE PRECISION
*          On entry, B1, B2 and B3 are elements of the input 2-by-2
*          upper (lower) triangular matrix B.
*
*  CSU     (output) DOUBLE PRECISION
*  SNU     (output) DOUBLE PRECISION
*          The desired orthogonal matrix U.
*
*  CSV     (output) DOUBLE PRECISION
*  SNV     (output) DOUBLE PRECISION
*          The desired orthogonal matrix V.
*
*  CSQ     (output) DOUBLE PRECISION
*  SNQ     (output) DOUBLE PRECISION
*          The desired orthogonal matrix Q.
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO
      PARAMETER          ( ZERO = 0.0D+0 )
*     ..
*     .. Local Scalars ..
      DOUBLE PRECISION   A, AUA11, AUA12, AUA21, AUA22, AVB12, AVB11, 
     $                   AVB21, AVB22, B, C, CSL, CSR, D, R, S1, S2, 
     $                   SNL, SNR, UA11, UA11R, UA12, UA21, UA22, 
     $                   UA22R, VB11, VB11R, VB12, VB21, VB22, VB22R
*     ..
*     .. External Subroutines ..
      EXTERNAL           DLARTG, DLASV2
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS
*     ..
*     .. Executable Statements ..
*
      IF( UPPER ) THEN
*
*        Input matrices A and B are upper triangular matrices
*
*        Form matrix C = A*adj(B) = ( a b )
*                                   ( 0 d )
*
         A = A1*B3
         D = A3*B1
         B = A2*B1 - A1*B2
*
*        The SVD of real 2-by-2 triangular C
*
*         ( CSL -SNL )*( A B )*(  CSR  SNR ) = ( R 0 )
*         ( SNL  CSL ) ( 0 D ) ( -SNR  CSR )   ( 0 T )
*
         CALL DLASV2( A, B, D, S1, S2, SNR, CSR, SNL, CSL )
*
         IF( ABS( CSL ).GE.ABS( SNL ) .OR. ABS( CSR ).GE.ABS( SNR ) )
     $        THEN
*
*           Compute the (1,1) and (1,2) elements of U'*A and V'*B,
*           and (1,2) element of |U|'*|A| and |V|'*|B|.
*
            UA11R = CSL*A1
            UA12 = CSL*A2 + SNL*A3
*
            VB11R = CSR*B1
            VB12 = CSR*B2 + SNR*B3
*
            AUA12 = ABS( CSL )*ABS( A2 ) + ABS( SNL )*ABS( A3 )
            AVB12 = ABS( CSR )*ABS( B2 ) + ABS( SNR )*ABS( B3 )
*
*           zero (1,2) elements of U'*A and V'*B
*
            IF( ( ABS( UA11R )+ABS( UA12 ) ).NE.ZERO ) THEN
               IF( AUA12 / ( ABS( UA11R )+ABS( UA12 ) ).LE.AVB12 /
     $             ( ABS( VB11R )+ABS( VB12 ) ) ) THEN
                  CALL DLARTG( -UA11R, UA12, CSQ, SNQ, R )
               ELSE
                  CALL DLARTG( -VB11R, VB12, CSQ, SNQ, R )
               END IF
            ELSE
               CALL DLARTG( -VB11R, VB12, CSQ, SNQ, R )
            END IF
*
            CSU = CSL
            SNU = -SNL
            CSV = CSR
            SNV = -SNR
*
         ELSE
*
*           Compute the (2,1) and (2,2) elements of U'*A and V'*B,
*           and (2,2) element of |U|'*|A| and |V|'*|B|.
*
            UA21 = -SNL*A1
            UA22 = -SNL*A2 + CSL*A3
*
            VB21 = -SNR*B1
            VB22 = -SNR*B2 + CSR*B3
*
            AUA22 = ABS( SNL )*ABS( A2 ) + ABS( CSL )*ABS( A3 )
            AVB22 = ABS( SNR )*ABS( B2 ) + ABS( CSR )*ABS( B3 )
*
*           zero (2,2) elements of U'*A and V'*B, and then swap.
*
            IF( ( ABS( UA21 )+ABS( UA22 ) ).NE.ZERO ) THEN
               IF( AUA22 / ( ABS( UA21 )+ABS( UA22 ) ).LE.AVB22 /
     $             ( ABS( VB21 )+ABS( VB22 ) ) ) THEN
                  CALL DLARTG( -UA21, UA22, CSQ, SNQ, R )
               ELSE
                  CALL DLARTG( -VB21, VB22, CSQ, SNQ, R )
               END IF
            ELSE
               CALL DLARTG( -VB21, VB22, CSQ, SNQ, R )
            END IF
*
            CSU = SNL
            SNU = CSL
            CSV = SNR
            SNV = CSR
*
         END IF
*
      ELSE
*
*        Input matrices A and B are lower triangular matrices
*
*        Form matrix C = A*adj(B) = ( a 0 )
*                                   ( c d )
*
         A = A1*B3
         D = A3*B1
         C = A2*B3 - A3*B2
*
*        The SVD of real 2-by-2 triangular C
*
*         ( CSL -SNL )*( A 0 )*(  CSR  SNR ) = ( R 0 )
*         ( SNL  CSL ) ( C D ) ( -SNR  CSR )   ( 0 T )
*
         CALL DLASV2( A, C, D, S1, S2, SNR, CSR, SNL, CSL )
*
         IF( ABS( CSR ).GE.ABS( SNR ) .OR. ABS( CSL ).GE.ABS( SNL ) )
     $        THEN
*
*           Compute the (2,1) and (2,2) elements of U'*A and V'*B,
*           and (2,1) element of |U|'*|A| and |V|'*|B|.
*
            UA21 = -SNR*A1 + CSR*A2
            UA22R = CSR*A3
*
            VB21 = -SNL*B1 + CSL*B2
            VB22R = CSL*B3
*
            AUA21 = ABS( SNR )*ABS( A1 ) + ABS( CSR )*ABS( A2 )
            AVB21 = ABS( SNL )*ABS( B1 ) + ABS( CSL )*ABS( B2 )
*
*           zero (2,1) elements of U'*A and V'*B.
*
            IF( ( ABS( UA21 )+ABS( UA22R ) ).NE.ZERO ) THEN
               IF( AUA21 / ( ABS( UA21 )+ABS( UA22R ) ).LE.AVB21 /
     $             ( ABS( VB21 )+ABS( VB22R ) ) ) THEN
                  CALL DLARTG( UA22R, UA21, CSQ, SNQ, R )
               ELSE
                  CALL DLARTG( VB22R, VB21, CSQ, SNQ, R )
               END IF
            ELSE
               CALL DLARTG( VB22R, VB21, CSQ, SNQ, R )
            END IF
*
            CSU = CSR
            SNU = -SNR
            CSV = CSL
            SNV = -SNL
*
         ELSE
*
*           Compute the (1,1) and (1,2) elements of U'*A and V'*B,
*           and (1,1) element of |U|'*|A| and |V|'*|B|.
*
            UA11 = CSR*A1 + SNR*A2
            UA12 = SNR*A3
*
            VB11 = CSL*B1 + SNL*B2
            VB12 = SNL*B3
*
            AUA11 = ABS( CSR )*ABS( A1 ) + ABS( SNR )*ABS( A2 )
            AVB11 = ABS( CSL )*ABS( B1 ) + ABS( SNL )*ABS( B2 )
*
*           zero (1,1) elements of U'*A and V'*B, and then swap.
*
            IF( ( ABS( UA11 )+ABS( UA12 ) ).NE.ZERO ) THEN
               IF( AUA11 / ( ABS( UA11 )+ABS( UA12 ) ).LE.AVB11 /
     $             ( ABS( VB11 )+ABS( VB12 ) ) ) THEN
                  CALL DLARTG( UA12, UA11, CSQ, SNQ, R )
               ELSE
                  CALL DLARTG( VB12, VB11, CSQ, SNQ, R )
               END IF
            ELSE
               CALL DLARTG( VB12, VB11, CSQ, SNQ, R )
            END IF
*
            CSU = SNR
            SNU = CSR
            CSV = SNL
            SNV = CSL
*
         END IF
*
      END IF
*
      RETURN
*
*     End of DLAGS2
*
      END
      SUBROUTINE DLAGV2( A, LDA, B, LDB, ALPHAR, ALPHAI, BETA, CSL, SNL,
     $                   CSR, SNR )
*
*  -- LAPACK auxiliary routine (version 2.1) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 15, 1996
*
*     .. Scalar Arguments ..
      INTEGER            LDA, LDB
      DOUBLE PRECISION   CSL, CSR, SNL, SNR
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION   A( LDA, * ), ALPHAI( 2 ), ALPHAR( 2 ),
     $                   B( LDB, * ), BETA( 2 )
*     ..
*
*  Purpose
*  =======
*
*  DLAGV2 computes the Generalized Schur factorization of a real 2-by-2
*  matrix pencil (A,B) where B is upper triangular. This routine
*  computes orthogonal (rotation) matrices given by CSL, SNL and CSR,
*  SNR such that
*
*  1) if the pencil (A,B) has two real eigenvalues (include 0/0 or 1/0
*     types), then
*
*     [ a11 a12 ] := [  CSL  SNL ] [ a11 a12 ] [  CSR -SNR ]
*     [  0  a22 ]    [ -SNL  CSL ] [ a21 a22 ] [  SNR  CSR ]
*
*     [ b11 b12 ] := [  CSL  SNL ] [ b11 b12 ] [  CSR -SNR ]
*     [  0  b22 ]    [ -SNL  CSL ] [  0  b22 ] [  SNR  CSR ],
*
*  2) if the pencil (A,B) has a pair of complex conjugate eigenvalues,
*     then
*
*     [ a11 a12 ] := [  CSL  SNL ] [ a11 a12 ] [  CSR -SNR ]
*     [ a21 a22 ]    [ -SNL  CSL ] [ a21 a22 ] [  SNR  CSR ]
*
*     [ b11  0  ] := [  CSL  SNL ] [ b11 b12 ] [  CSR -SNR ]
*     [  0  b22 ]    [ -SNL  CSL ] [  0  b22 ] [  SNR  CSR ]
*
*     where b11 >= b22 > 0.
*
*
*  Arguments
*  =========
*
*  A       (input/output) DOUBLE PRECISION array, dimension (LDA, 2)
*          On entry, the 2 x 2 matrix A.
*          On exit, A is overwritten by the ``A-part'' of the
*          generalized Schur form.
*
*  LDA     (input) INTEGER
*          THe leading dimension of the array A.  LDA >= 2.
*
*  B       (input/output) DOUBLE PRECISION array, dimension (LDB, 2)
*          On entry, the upper triangular 2 x 2 matrix B.
*          On exit, B is overwritten by the ``B-part'' of the
*          generalized Schur form.
*
*  LDB     (input) INTEGER
*          THe leading dimension of the array B.  LDB >= 2.
*
*  ALPHAR  (output) DOUBLE PRECISION array, dimension (2)
*  ALPHAI  (output) DOUBLE PRECISION array, dimension (2)
*  BETA    (output) DOUBLE PRECISION array, dimension (2)
*          (ALPHAR(k)+i*ALPHAI(k))/BETA(k) are the eigenvalues of the
*          pencil (A,B), k=1,2, i = sqrt(-1).  Note that BETA(k) may
*          be zero.
*
*  CSL     (output) DOUBLE PRECISION
*          The cosine of the left rotation matrix.
*
*  SNL     (output) DOUBLE PRECISION
*          The sine of the left rotation matrix.
*
*  CSR     (output) DOUBLE PRECISION
*          The cosine of the right rotation matrix.
*
*  SNR     (output) DOUBLE PRECISION
*          The sine of the right rotation matrix.
*
*  Further Details
*  ===============
*
*  Based on contributions by
*     Mark Fahey, Department of Mathematics, Univ. of Kentucky, USA
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      DOUBLE PRECISION   ANORM, ASCALE, BNORM, BSCALE, H1, H2, H3, QQ,
     $                   R, RR, SAFMIN, SCALE1, SCALE2, T, ULP, WI, WR1,
     $                   WR2
*     ..
*     .. External Subroutines ..
      EXTERNAL           DLAG2, DLARTG, DLASV2, DROT
*     ..
*     .. External Functions ..
      DOUBLE PRECISION   DLAMCH, DLAPY2
      EXTERNAL           DLAMCH, DLAPY2
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX
*     ..
*     .. Executable Statements ..
*
      SAFMIN = DLAMCH( 'S' )
      ULP = DLAMCH( 'P' )
*
*     Scale A
*
      ANORM = MAX( ABS( A( 1, 1 ) )+ABS( A( 2, 1 ) ),
     $        ABS( A( 1, 2 ) )+ABS( A( 2, 2 ) ), SAFMIN )
      ASCALE = ONE / ANORM
      A( 1, 1 ) = ASCALE*A( 1, 1 )
      A( 1, 2 ) = ASCALE*A( 1, 2 )
      A( 2, 1 ) = ASCALE*A( 2, 1 )
      A( 2, 2 ) = ASCALE*A( 2, 2 )
*
*     Scale B
*
      BNORM = MAX( ABS( B( 1, 1 ) ), ABS( B( 1, 2 ) )+ABS( B( 2, 2 ) ),
     $        SAFMIN )
      BSCALE = ONE / BNORM
      B( 1, 1 ) = BSCALE*B( 1, 1 )
      B( 1, 2 ) = BSCALE*B( 1, 2 )
      B( 2, 2 ) = BSCALE*B( 2, 2 )
*
*     Check if A can be deflated
*
      IF( ABS( A( 2, 1 ) ).LE.ULP ) THEN
         CSL = ONE
         SNL = ZERO
         CSR = ONE
         SNR = ZERO
         A( 2, 1 ) = ZERO
         B( 2, 1 ) = ZERO
*
*     Check if B is singular
*
      ELSE IF( ABS( B( 1, 1 ) ).LE.ULP ) THEN
         CALL DLARTG( A( 1, 1 ), A( 2, 1 ), CSL, SNL, R )
         CSR = ONE
         SNR = ZERO
         CALL DROT( 2, A( 1, 1 ), LDA, A( 2, 1 ), LDA, CSL, SNL )
         CALL DROT( 2, B( 1, 1 ), LDB, B( 2, 1 ), LDB, CSL, SNL )
         A( 2, 1 ) = ZERO
         B( 1, 1 ) = ZERO
         B( 2, 1 ) = ZERO
*
      ELSE IF( ABS( B( 2, 2 ) ).LE.ULP ) THEN
         CALL DLARTG( A( 2, 2 ), A( 2, 1 ), CSR, SNR, T )
         SNR = -SNR
         CALL DROT( 2, A( 1, 1 ), 1, A( 1, 2 ), 1, CSR, SNR )
         CALL DROT( 2, B( 1, 1 ), 1, B( 1, 2 ), 1, CSR, SNR )
         CSL = ONE
         SNL = ZERO
         A( 2, 1 ) = ZERO
         B( 2, 1 ) = ZERO
         B( 2, 2 ) = ZERO
*
      ELSE
*
*        B is nonsingular, first compute the eigenvalues of (A,B)
*
         CALL DLAG2( A, LDA, B, LDB, SAFMIN, SCALE1, SCALE2, WR1, WR2,
     $               WI )
*
         IF( WI.EQ.ZERO ) THEN
*
*           two real eigenvalues, compute s*A-w*B
*
            H1 = SCALE1*A( 1, 1 ) - WR1*B( 1, 1 )
            H2 = SCALE1*A( 1, 2 ) - WR1*B( 1, 2 )
            H3 = SCALE1*A( 2, 2 ) - WR1*B( 2, 2 )
*
            RR = DLAPY2( H1, H2 )
            QQ = DLAPY2( SCALE1*A( 2, 1 ), H3 )
*
            IF( RR.GT.QQ ) THEN
*
*              find right rotation matrix to zero 1,1 element of
*              (sA - wB)
*
               CALL DLARTG( H2, H1, CSR, SNR, T )
*
            ELSE
*
*              find right rotation matrix to zero 2,1 element of
*              (sA - wB)
*
               CALL DLARTG( H3, SCALE1*A( 2, 1 ), CSR, SNR, T )
*
            END IF
*
            SNR = -SNR
            CALL DROT( 2, A( 1, 1 ), 1, A( 1, 2 ), 1, CSR, SNR )
            CALL DROT( 2, B( 1, 1 ), 1, B( 1, 2 ), 1, CSR, SNR )
*
*           compute inf norms of A and B
*
            H1 = MAX( ABS( A( 1, 1 ) )+ABS( A( 1, 2 ) ),
     $           ABS( A( 2, 1 ) )+ABS( A( 2, 2 ) ) )
            H2 = MAX( ABS( B( 1, 1 ) )+ABS( B( 1, 2 ) ),
     $           ABS( B( 2, 1 ) )+ABS( B( 2, 2 ) ) )
*
            IF( ( SCALE1*H1 ).GE.ABS( WR1 )*H2 ) THEN
*
*              find left rotation matrix Q to zero out B(2,1)
*
               CALL DLARTG( B( 1, 1 ), B( 2, 1 ), CSL, SNL, R )
*
            ELSE
*
*              find left rotation matrix Q to zero out A(2,1)
*
               CALL DLARTG( A( 1, 1 ), A( 2, 1 ), CSL, SNL, R )
*
            END IF
*
            CALL DROT( 2, A( 1, 1 ), LDA, A( 2, 1 ), LDA, CSL, SNL )
            CALL DROT( 2, B( 1, 1 ), LDB, B( 2, 1 ), LDB, CSL, SNL )
*
            A( 2, 1 ) = ZERO
            B( 2, 1 ) = ZERO
*
         ELSE
*
*           a pair of complex conjugate eigenvalues
*           first compute the SVD of the matrix B
*
            CALL DLASV2( B( 1, 1 ), B( 1, 2 ), B( 2, 2 ), R, T, SNR,
     $                   CSR, SNL, CSL )
*
*           Form (A,B) := Q(A,B)Z' where Q is left rotation matrix and
*           Z is right rotation matrix computed from DLASV2
*
            CALL DROT( 2, A( 1, 1 ), LDA, A( 2, 1 ), LDA, CSL, SNL )
            CALL DROT( 2, B( 1, 1 ), LDB, B( 2, 1 ), LDB, CSL, SNL )
            CALL DROT( 2, A( 1, 1 ), 1, A( 1, 2 ), 1, CSR, SNR )
            CALL DROT( 2, B( 1, 1 ), 1, B( 1, 2 ), 1, CSR, SNR )
*
            B( 2, 1 ) = ZERO
            B( 1, 2 ) = ZERO
*
         END IF
*
      END IF
*
*     Unscaling
*
      A( 1, 1 ) = ANORM*A( 1, 1 )
      A( 2, 1 ) = ANORM*A( 2, 1 )
      A( 1, 2 ) = ANORM*A( 1, 2 )
      A( 2, 2 ) = ANORM*A( 2, 2 )
      B( 1, 1 ) = BNORM*B( 1, 1 )
      B( 2, 1 ) = BNORM*B( 2, 1 )
      B( 1, 2 ) = BNORM*B( 1, 2 )
      B( 2, 2 ) = BNORM*B( 2, 2 )
*
      IF( WI.EQ.ZERO ) THEN
         ALPHAR( 1 ) = A( 1, 1 )
         ALPHAR( 2 ) = A( 2, 2 )
         ALPHAI( 1 ) = ZERO
         ALPHAI( 2 ) = ZERO
         BETA( 1 ) = B( 1, 1 )
         BETA( 2 ) = B( 2, 2 )
      ELSE
         ALPHAR( 1 ) = ANORM*WR1 / SCALE1 / BNORM
         ALPHAI( 1 ) = ANORM*WI / SCALE1 / BNORM
         ALPHAR( 2 ) = ALPHAR( 1 )
         ALPHAI( 2 ) = -ALPHAI( 1 )
         BETA( 1 ) = ONE
         BETA( 2 ) = ONE
      END IF
*
   10 CONTINUE
*
      RETURN
*
*     End of DLAGV2
*
      END
      SUBROUTINE DLATDF( IJOB, N, Z, LDZ, RHS, RDSUM, RDSCAL, IPIV,
     $                   JPIV )
*
*  -- LAPACK auxiliary routine (version 2.1) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 15, 1996
*
*     .. Scalar Arguments ..
      INTEGER            IJOB, LDZ, N
      DOUBLE PRECISION   RDSCAL, RDSUM
*     ..
*     .. Array Arguments ..
      INTEGER            IPIV( * ), JPIV( * )
      DOUBLE PRECISION   RHS( * ), Z( LDZ, * )
*     ..
*
*  Purpose
*  =======
*
*  DLATDF uses the LU factorization of the n-by-n matrix Z computed by
*  DGETC2 and computes a contribution to the reciprocal Dif-estimate
*  by solving Z * x = b for x, and choosing the r.h.s. b such that
*  the norm of x is as large as possible. On entry RHS = b holds the
*  contribution from earlier solved sub-systems, and on return RHS = x.
*
*  The factorization of Z returned by DGETC2 has the form Z = P*L*U*Q,
*  where P and Q are permutation matrices. L is lower triangular with
*  unit diagonal elements and U is upper triangular.
*
*  Arguments
*  =========
*
*  IJOB    (input) INTEGER
*          IJOB = 2: First compute an approximative null-vector e
*              of Z using DGECON, e is normalized and solve for
*              Zx = +-e - f with the sign giving the greater value
*              of 2-norm(x). About 5 times as expensive as Default.
*          IJOB .ne. 2: Local look ahead strategy where all entries of
*              the r.h.s. b is choosen as either +1 or -1 (Default).
*
*  N       (input) INTEGER
*          The number of columns of the matrix Z.
*
*  Z       (input) DOUBLE PRECISION array, dimension (LDZ, N)
*          On entry, the LU part of the factorization of the n-by-n
*          matrix Z computed by DGETC2:  Z = P * L * U * Q
*

*  LDZ     (input) INTEGER
*          The leading dimension of the array Z.  LDA >= max(1, N).
*
*  RHS     (input/output) DOUBLE PRECISION array, dimension N.
*          On entry, RHS contains contributions from other subsystems.
*          On exit, RHS contains the solution of the subsystem with
*          entries acoording to the value of IJOB (see above).
*
*  RDSUM   (input/output) DOUBLE PRECISION
*          On entry, the sum of squares of computed contributions to
*          the Dif-estimate under computation by DTGSYL, where the
*          scaling factor RDSCAL (see below) has been factored out.
*          On exit, the corresponding sum of squares updated with the
*          contributions from the current sub-system.
*          If TRANS = 'T' RDSUM is not touched.
*          NOTE: RDSUM only makes sense when DTGSY2 is called by DTGSYL.
*
*  RDSCAL  (input/output) DOUBLE PRECISION
*          On entry, scaling factor used to prevent overflow in RDSUM.
*          On exit, RDSCAL is updated w.r.t. the current contributions
*          in RDSUM.
*          If TRANS = 'T', RDSCAL is not touched.
*          NOTE: RDSCAL only makes sense when DTGSY2 is called by
*                DTGSYL.
*
*  IPIV    (input) INTEGER array, dimension (N).
*          The pivot indices; for 1 <= i <= N, row i of the
*          matrix has been interchanged with row IPIV(i).
*
*  JPIV    (input) INTEGER array, dimension (N).
*          The pivot indices; for 1 <= j <= N, column j of the
*          matrix has been interchanged with column JPIV(j).
*
*  Further Details
*  ===============
*
*  Based on contributions by
*     Bo Kagstrom and Peter Poromaa, Department of Computing Science,
*     Umea University, S-901 87 Umea, Sweden.
*
*  This routine is a further developed implementation of algorithm
*  BSOLVE in [1] using complete pivoting in the LU factorization.
*
*  [1] Bo Kagstrom and Lars Westin,
*      Generalized Schur Methods with Condition Estimators for
*      Solving the Generalized Sylvester Equation, IEEE Transactions
*      on Automatic Control, Vol. 34, No. 7, July 1989, pp 745-751.
*
*  [2] Peter Poromaa,
*      On Efficient and Robust Estimators for the Separation
*      between two Regular Matrix Pairs with Applications in
*      Condition Estimation. Report IMINF-95.05, Departement of
*      Computing Science, Umea University, S-901 87 Umea, Sweden, 1995.
*
*  =====================================================================
*
*     .. Parameters ..
      INTEGER            MAXDIM
      PARAMETER          ( MAXDIM = 8 )
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      INTEGER            I, INFO, J, K
      DOUBLE PRECISION   BM, BP, PMONE, SMINU, SPLUS, TEMP
*     ..
*     .. Local Arrays ..
      INTEGER            IWORK( MAXDIM )
      DOUBLE PRECISION   WORK( 4*MAXDIM ), XM( MAXDIM ), XP( MAXDIM )
*     ..
*     .. External Subroutines ..
      EXTERNAL           DAXPY, DCOPY, DGECON, DGESC2, DLASSQ, DLASWP,
     $                   DSCAL
*     ..
*     .. External Functions ..
      DOUBLE PRECISION   DASUM, DDOT
      EXTERNAL           DASUM, DDOT
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, SQRT
*     ..
*     .. Executable Statements ..
*
      IF( IJOB.NE.2 ) THEN
*
*        Apply permutations IPIV to RHS
*
         CALL DLASWP( 1, RHS, LDZ, 1, N-1, IPIV, 1 )
*
*        Solve for L-part choosing RHS either to +1 or -1.
*
         IF( RDSCAL.EQ.ZERO )
     $      PMONE = -ONE
*
         DO 10 J = 1, N - 1
            BP = RHS( J ) + ONE
            BM = RHS( J ) - ONE
            SPLUS = ONE
*
*           Look-ahead for L-part RHS(1:N-1) = + or -1, SPLUS and
*           SMIN computed more efficiently than in BSOLVE [1].
*
            SPLUS = SPLUS + DDOT( N-J, Z( J+1, J ), 1, Z( J+1, J ), 1 )
            SMINU = DDOT( N-J, Z( J+1, J ), 1, RHS( J+1 ), 1 )
            SPLUS = SPLUS*RHS( J )
            IF( SPLUS.GT.SMINU ) THEN
               RHS( J ) = BP
            ELSE IF( SMINU.GT.SPLUS ) THEN
               RHS( J ) = BM
            ELSE
*
*              In this case the updating sums are equal and we can
*              choose RHS(J) +1 or -1. The first time this happens
*              we choose -1, thereafter +1. This is a simple way to
*              get good estimates of matrices like Byers well-known
*              example (see [1]). (Not done in BSOLVE.)
*
               RHS( J ) = RHS( J ) + PMONE
               PMONE = ONE
            END IF
*
*           Compute the remaining r.h.s.
*
            TEMP = -RHS( J )
            CALL DAXPY( N-J, TEMP, Z( J+1, J ), 1, RHS( J+1 ), 1 )
*
   10    CONTINUE
*
*        Solve for U-part, look-ahead for RHS(N) = +-1. This is not done
*        in BSOLVE and will hopefully give us a better estimate because
*        any ill-conditioning of the original matrix is transfered to U
*        and not to L. U(N, N) is an approximation to sigma_min(LU).
*
         CALL DCOPY( N-1, RHS, 1, XP, 1 )
         XP( N ) = RHS( N ) + ONE
         RHS( N ) = RHS( N ) - ONE
         SPLUS = ZERO
         SMINU = ZERO
         DO 30 I = N, 1, -1
            TEMP = ONE / Z( I, I )
            XP( I ) = XP( I )*TEMP
            RHS( I ) = RHS( I )*TEMP
            DO 20 K = I + 1, N
               XP( I ) = XP( I ) - XP( K )*( Z( I, K )*TEMP )
               RHS( I ) = RHS( I ) - RHS( K )*( Z( I, K )*TEMP )
   20       CONTINUE
            SPLUS = SPLUS + ABS( XP( I ) )
            SMINU = SMINU + ABS( RHS( I ) )
   30    CONTINUE
         IF( SPLUS.GT.SMINU )
     $      CALL DCOPY( N, XP, 1, RHS, 1 )
*
*        Apply the permutations JPIV to the computed solution (RHS)
*
         CALL DLASWP( 1, RHS, LDZ, 1, N-1, JPIV, -1 )
*
*        Compute the sum of squares
*
         CALL DLASSQ( N, RHS, 1, RDSCAL, RDSUM )
*
      ELSE
*
*        IJOB = 2, Compute approximate nullvector XM of Z
*
         CALL DGECON( 'I', N, Z, LDZ, ONE, TEMP, WORK, IWORK, INFO )
         CALL DCOPY( N, WORK( N+1 ), 1, XM, 1 )
*
*        Compute RHS
*
         CALL DLASWP( 1, XM, LDZ, 1, N-1, IPIV, -1 )
         TEMP = ONE / SQRT( DDOT( N, XM, 1, XM, 1 ) )
         CALL DSCAL( N, TEMP, XM, 1 )
         CALL DCOPY( N, XM, 1, XP, 1 )
         CALL DAXPY( N, ONE, RHS, 1, XP, 1 )
         CALL DAXPY( N, -ONE, XM, 1, RHS, 1 )
         CALL DGESC2( N, Z, LDZ, RHS, IPIV, JPIV, TEMP )
         CALL DGESC2( N, Z, LDZ, XP, IPIV, JPIV, TEMP )
         IF( DASUM( N, XP, 1 ).GT.DASUM( N, RHS, 1 ) )
     $      CALL DCOPY( N, XP, 1, RHS, 1 )
*
*        Compute the sum of squares
*
         CALL DLASSQ( N, RHS, 1, RDSCAL, RDSUM )
*
      END IF
*
      RETURN
*
*     End of DLATDF
*
      END

      SUBROUTINE DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ, Z,
     $                   LDZ, J1, N1, N2, WORK, LWORK, INFO )
*
*  -- LAPACK auxiliary routine (version 2.1) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 15, 1996
*
*     .. Scalar Arguments ..
      LOGICAL            WANTQ, WANTZ
      INTEGER            INFO, J1, LDA, LDB, LDQ, LDZ, LWORK, N, N1, N2
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION   A( LDA, * ), B( LDB, * ), Q( LDQ, * ),
     $                   WORK( * ), Z( LDZ, * )
*     ..
*
*  Purpose
*  =======
*
*  DTGEX2 swaps adjacent diagonal blocks (A11, B11) and (A22, B22)
*  of size 1-by-1 or 2-by-2 in an upper (quasi) triangular matrix pair
*  (A, B) by an orthogonal equivalence transformation.
*
*  (A, B) must be in generalized real Schur canonical form (as returned
*  by DGGES), i.e. A is block upper triangular with 1-by-1 and 2-by-2
*  diagonal blocks. B is upper triangular.
*
*  Optionally, the matrices Q and Z of generalized Schur vectors are
*  updated.
*
*         Q(in) * A(in) * Z(in)' = Q(out) * A(out) * Z(out)'
*         Q(in) * B(in) * Z(in)' = Q(out) * B(out) * Z(out)'
*
*
*  Arguments
*  =========
*
*  WANTQ   (input) LOGICAL
*          .TRUE. : update the left transformation matrix Q;
*          .FALSE.: do not update Q.
*
*  WANTZ   (input) LOGICAL
*          .TRUE. : update the right transformation matrix Z;
*          .FALSE.: do not update Z.
*
*  N       (input) INTEGER
*          The order of the matrices A and B. N >= 0.
*
*  A      (input/output) DOUBLE PRECISION arrays, dimensions (LDA,N)
*          On entry, the matrix A in the pair (A, B).
*          On exit, the updated matrix A.
*
*  LDA     (input)  INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  B      (input/output) DOUBLE PRECISION arrays, dimensions (LDB,N)
*          On entry, the matrix B in the pair (A, B).
*          On exit, the updated matrix B.
*
*  LDB     (input)  INTEGER
*          The leading dimension of the array B. LDB >= max(1,N).
*
*  Q       (input/output) DOUBLE PRECISION array, dimension (LDZ,N)
*          On entry, if WANTQ = .TRUE., the orthogonal matrix Q.
*          On exit, the updated matrix Q.
*          Not referenced if WANTQ = .FALSE..
*
*  LDQ     (input) INTEGER
*          The leading dimension of the array Q. LDQ >= 1.
*          If WANTQ = .TRUE., LDQ >= N.
*
*  Z       (input/output) DOUBLE PRECISION array, dimension (LDZ,N)
*          On entry, if WANTZ =.TRUE., the orthogonal matrix Z.
*          On exit, the updated matrix Z.
*          Not referenced if WANTZ = .FALSE..
*
*  LDZ     (input) INTEGER
*          The leading dimension of the array Z. LDZ >= 1.
*          If WANTZ = .TRUE., LDZ >= N.
*
*  J1      (input) INTEGER
*          The index to the first block (A11, B11). 1 <= J1 <= N.
*
*  N1      (input) INTEGER
*          The order of the first block (A11, B11). N1 = 0, 1 or 2.
*
*  N2      (input) INTEGER
*          The order of the second block (A22, B22). N2 = 0, 1 or 2.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK).
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK.
*          LWORK >=  MAX( N*(N2+N1), (N2+N1)*(N2+N1)*2 )
*
*  INFO    (output) INTEGER
*            =0: Successful exit
*            >0: If INFO = 1, the transformed matrix (A, B) would be
*                too far from generalized Schur form; the blocks are
*                not swapped and (A, B) and (Q, Z) are unchanged.
*                The problem of swapping is too ill-conditioned.
*            <0: If INFO = -16: LWORK is too small. Appropriate value
*                for LWORK is returned in WORK(1).
*
*  Further Details
*  ===============
*
*  Based on contributions by
*     Bo Kagstrom and Peter Poromaa, Department of Computing Science,
*     Umea University, S-901 87 Umea, Sweden.
*
*  In the current code both weak and strong stability tests are
*  performed. The user can omit the strong stability test by changing
*  the internal logical parameter WANDS to .FALSE.. See ref. [2] for
*  details.
*
*  [1] B. Kagstrom; A Direct Method for Reordering Eigenvalues in the
*      Generalized Real Schur Form of a Regular Matrix Pair (A, B), in
*      M.S. Moonen et al (eds), Linear Algebra for Large Scale and
*      Real-Time Applications, Kluwer Academic Publ. 1993, pp 195-218.
*
*  [2] B. Kagstrom and P. Poromaa; Computing Eigenspaces with Specified
*      Eigenvalues of a Regular Matrix Pair (A, B) and Condition
*      Estimation: Theory, Algorithms and Software,
*      Report UMINF - 94.04, Department of Computing Science, Umea
*      University, S-901 87 Umea, Sweden, 1994. Also as LAPACK Working
*      Note 87. To appear in Numerical Algorithms, 1996.
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
      DOUBLE PRECISION   TEN
      PARAMETER          ( TEN = 1.0D+01 )
      INTEGER            LDST
      PARAMETER          ( LDST = 4 )
      LOGICAL            WANDS
      PARAMETER          ( WANDS = .TRUE. )
*     ..
*     .. Local Scalars ..
      LOGICAL            DTRONG, WEAK
      INTEGER            I, IDUM, LINFO, M
      DOUBLE PRECISION   BQRA21, BRQA21, DDUM, DNORM, DSCALE, DSUM, EPS,
     $                   F, G, SA, SB, SCALE, SMLNUM, SS, THRESH, WS
*     ..
*     .. Local Arrays ..
      INTEGER            IWORK( LDST )
      DOUBLE PRECISION   AI( 2 ), AR( 2 ), BE( 2 ), IR( LDST, LDST ),
     $                   IRCOP( LDST, LDST ), LI( LDST, LDST ),
     $                   LICOP( LDST, LDST ), S( LDST, LDST ),
     $                   SCPY( LDST, LDST ), T( LDST, LDST ),
     $                   TAUL( LDST ), TAUR( LDST ), TCPY( LDST, LDST )
*     ..
*     .. External Functions ..
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH
*     ..
*     .. External Subroutines ..
      EXTERNAL           DCOPY, DGEMM, DGEQR2, DGERQ2, DLACPY, DLAGV2,
     $                   DLARTG, DLASSQ, DORG2R, DORGR2, DORM2R, DORMR2,
     $                   DROT, DSCAL, DTGSY2
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX, SQRT
*     ..
*     .. Executable Statements ..
*
      INFO = 0
*
*     Quick return if possible
*
      IF( N.LE.1 .OR. N1.LE.0 .OR. N2.LE.0 )
     $   RETURN
      IF( N1.GT.N .OR. ( J1+N1 ).GT.N )
     $   RETURN
      M = N1 + N2
      IF( LWORK.LT.MAX( N*M, M*M*2 ) ) THEN
         INFO = -16
         WORK( 1 ) = MAX( N*M, M*M*2 )
         RETURN
      END IF
*
      WEAK = .FALSE.
      DTRONG = .FALSE.
*
*     Make a local copy of selected block
*
      CALL DCOPY( LDST*LDST, ZERO, 0, LI, 1 )
      CALL DCOPY( LDST*LDST, ZERO, 0, IR, 1 )
      CALL DLACPY( 'Full', M, M, A( J1, J1 ), LDA, S, LDST )
      CALL DLACPY( 'Full', M, M, B( J1, J1 ), LDB, T, LDST )
*
*     Compute threshold for testing acceptance of swapping.
*
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' ) / EPS
      DSCALE = ZERO
      DSUM = ONE
      CALL DLACPY( 'Full', M, M, S, LDST, WORK, M )
      CALL DLASSQ( M*M, WORK, 1, DSCALE, DSUM )
      CALL DLACPY( 'Full', M, M, T, LDST, WORK, M )
      CALL DLASSQ( M*M, WORK, 1, DSCALE, DSUM )
      DNORM = DSCALE*SQRT( DSUM )
      THRESH = MAX( TEN*EPS*DNORM, SMLNUM )
*
      IF( M.EQ.2 ) THEN
*
*        CASE 1: Swap 1-by-1 and 1-by-1 blocks.
*
*        Compute orthogonal QL and RQ that swap 1-by-1 and 1-by-1 blocks
*        using Givens rotations and perform the swap tentatively.
*
         F = S( 2, 2 )*T( 1, 1 ) - T( 2, 2 )*S( 1, 1 )
         G = S( 2, 2 )*T( 1, 2 ) - T( 2, 2 )*S( 1, 2 )
         SB = ABS( T( 2, 2 ) )
         SA = ABS( S( 2, 2 ) )
         CALL DLARTG( F, G, IR( 1, 2 ), IR( 1, 1 ), DDUM )
         IR( 2, 1 ) = -IR( 1, 2 )
         IR( 2, 2 ) = IR( 1, 1 )
         CALL DROT( 2, S( 1, 1 ), 1, S( 1, 2 ), 1, IR( 1, 1 ),
     $              IR( 2, 1 ) )
         CALL DROT( 2, T( 1, 1 ), 1, T( 1, 2 ), 1, IR( 1, 1 ),
     $              IR( 2, 1 ) )
         IF( SA.GE.SB ) THEN
            CALL DLARTG( S( 1, 1 ), S( 2, 1 ), LI( 1, 1 ), LI( 2, 1 ),
     $                   DDUM )
         ELSE
            CALL DLARTG( T( 1, 1 ), T( 2, 1 ), LI( 1, 1 ), LI( 2, 1 ),
     $                   DDUM )
         END IF
         CALL DROT( 2, S( 1, 1 ), LDST, S( 2, 1 ), LDST, LI( 1, 1 ),
     $              LI( 2, 1 ) )
         CALL DROT( 2, T( 1, 1 ), LDST, T( 2, 1 ), LDST, LI( 1, 1 ),
     $              LI( 2, 1 ) )
         LI( 2, 2 ) = LI( 1, 1 )
         LI( 1, 2 ) = -LI( 2, 1 )
*
*        Weak stability test:
*           |S21| + |T21| <= O(EPS * F-norm((S, T)))
*
         WS = ABS( S( 2, 1 ) ) + ABS( T( 2, 1 ) )
         WEAK = WS.LE.THRESH
         IF( .NOT.WEAK )
     $      GO TO 70
*
         IF( WANDS ) THEN
*
*           Strong stability test:
*             F-norm((A-QL'*S*QR, B-QL'*T*QR)) <= O(EPS*F-norm((A,B)))
*
            CALL DLACPY( 'Full', M, M, A( J1, J1 ), LDA, WORK( M*M+1 ),
     $                   M )
            CALL DGEMM( 'N', 'N', M, M, M, ONE, LI, LDST, S, LDST, ZERO,
     $                  WORK, M )
            CALL DGEMM( 'N', 'T', M, M, M, -ONE, WORK, M, IR, LDST, ONE,
     $                  WORK( M*M+1 ), M )
            DSCALE = ZERO
            DSUM = ONE
            CALL DLASSQ( M*M, WORK( M*M+1 ), 1, DSCALE, DSUM )
*
            CALL DLACPY( 'Full', M, M, B( J1, J1 ), LDB, WORK( M*M+1 ),
     $                   M )
            CALL DGEMM( 'N', 'N', M, M, M, ONE, LI, LDST, T, LDST, ZERO,
     $                  WORK, M )
            CALL DGEMM( 'N', 'T', M, M, M, -ONE, WORK, M, IR, LDST, ONE,
     $                  WORK( M*M+1 ), M )
            CALL DLASSQ( M*M, WORK( M*M+1 ), 1, DSCALE, DSUM )
            SS = DSCALE*SQRT( DSUM )
            DTRONG = SS.LE.THRESH
            IF( .NOT.DTRONG )
     $         GO TO 70
         END IF
*
*        Update (A(J1:J1+M-1, M+J1:N), B(J1:J1+M-1, M+J1:N)) and
*               (A(1:J1-1, J1:J1+M), B(1:J1-1, J1:J1+M)).
*
         CALL DROT( J1+1, A( 1, J1 ), 1, A( 1, J1+1 ), 1, IR( 1, 1 ),
     $              IR( 2, 1 ) )
         CALL DROT( J1+1, B( 1, J1 ), 1, B( 1, J1+1 ), 1, IR( 1, 1 ),
     $              IR( 2, 1 ) )
         CALL DROT( N-J1+1, A( J1, J1 ), LDA, A( J1+1, J1 ), LDA,
     $              LI( 1, 1 ), LI( 2, 1 ) )
         CALL DROT( N-J1+1, B( J1, J1 ), LDB, B( J1+1, J1 ), LDB,
     $              LI( 1, 1 ), LI( 2, 1 ) )
*
*        Set  N1-by-N2 (2,1) - blocks to ZERO.
*
         A( J1+1, J1 ) = ZERO
         B( J1+1, J1 ) = ZERO
*
*        Accumulate transformations into Q and Z if requested.
*
         IF( WANTZ )
     $      CALL DROT( N, Z( 1, J1 ), 1, Z( 1, J1+1 ), 1, IR( 1, 1 ),
     $                 IR( 2, 1 ) )
         IF( WANTQ )
     $      CALL DROT( N, Q( 1, J1 ), 1, Q( 1, J1+1 ), 1, LI( 1, 1 ),
     $                 LI( 2, 1 ) )
*
*        Exit with INFO = 0 if swap was successfully performed.
*
         RETURN
*
      ELSE
*
*        CASE 2: Swap 1-by-1 and 2-by-2 blocks, or 2-by-2
*                and 2-by-2 blocks.
*
*        Solve the generalized Sylvester equation
*                 S11 * R - L * S22 = SCALE * S12
*                 T11 * R - L * T22 = SCALE * T12
*        for R and L. Solutions in LI and IR.
*
         CALL DLACPY( 'Full', N1, N2, T( 1, N1+1 ), LDST, LI, LDST )
         CALL DLACPY( 'Full', N1, N2, S( 1, N1+1 ), LDST,
     $                IR( N2+1, N1+1 ), LDST )
         CALL DTGSY2( 'N', 0, N1, N2, S, LDST, S( N1+1, N1+1 ), LDST,
     $                IR( N2+1, N1+1 ), LDST, T, LDST, T( N1+1, N1+1 ),
     $                LDST, LI, LDST, SCALE, DSUM, DSCALE, IWORK, IDUM,
     $                LINFO )
*
*        Compute orthogonal matrix QL:
*
*                    QL' * LI = [ TL ]
*                               [ 0  ]
*        where
*                    LI =  [      -L              ]
*                          [ SCALE * identity(N2) ]
*
         DO 10 I = 1, N2
            CALL DSCAL( N1, -ONE, LI( 1, I ), 1 )
            LI( N1+I, I ) = SCALE
   10    CONTINUE
         CALL DGEQR2( M, N2, LI, LDST, TAUL, WORK, LINFO )
         IF( LINFO.NE.0 )
     $      GO TO 70
         CALL DORG2R( M, M, N2, LI, LDST, TAUL, WORK, LINFO )
         IF( LINFO.NE.0 )
     $      GO TO 70
*
*        Compute orthogonal matrix RQ:
*
*                    IR * RQ' =   [ 0  TR],
*
*         where IR = [ SCALE * identity(N1), R ]
*
         DO 20 I = 1, N1
            IR( N2+I, I ) = SCALE
   20    CONTINUE
         CALL DGERQ2( N1, M, IR( N2+1, 1 ), LDST, TAUR, WORK, LINFO )
         IF( LINFO.NE.0 )
     $      GO TO 70
         CALL DORGR2( M, M, N1, IR, LDST, TAUR, WORK, LINFO )
         IF( LINFO.NE.0 )
     $      GO TO 70
*
*        Perform the swapping tentatively:
*
         CALL DGEMM( 'T', 'N', M, M, M, ONE, LI, LDST, S, LDST, ZERO,
     $               WORK, M )
         CALL DGEMM( 'N', 'T', M, M, M, ONE, WORK, M, IR, LDST, ZERO, S,
     $               LDST )
         CALL DGEMM( 'T', 'N', M, M, M, ONE, LI, LDST, T, LDST, ZERO,
     $               WORK, M )
         CALL DGEMM( 'N', 'T', M, M, M, ONE, WORK, M, IR, LDST, ZERO, T,
     $               LDST )
         CALL DLACPY( 'F', M, M, S, LDST, SCPY, LDST )
         CALL DLACPY( 'F', M, M, T, LDST, TCPY, LDST )
         CALL DLACPY( 'F', M, M, IR, LDST, IRCOP, LDST )
         CALL DLACPY( 'F', M, M, LI, LDST, LICOP, LDST )
*
*        Triangularize the B-part by an RQ factorization.
*        Apply transformation (from left) to A-part, giving S.
*
         CALL DGERQ2( M, M, T, LDST, TAUR, WORK, LINFO )
         IF( LINFO.NE.0 )
     $      GO TO 70
         CALL DORMR2( 'R', 'T', M, M, M, T, LDST, TAUR, S, LDST, WORK,
     $                LINFO )
         IF( LINFO.NE.0 )
     $      GO TO 70
         CALL DORMR2( 'L', 'N', M, M, M, T, LDST, TAUR, IR, LDST, WORK,
     $                LINFO )
         IF( LINFO.NE.0 )
     $      GO TO 70
*
*        Compute F-norm(S21) in BRQA21. (T21 is 0.)
*
         DSCALE = ZERO
         DSUM = ONE
         DO 30 I = 1, N2
            CALL DLASSQ( N1, S( N2+1, I ), 1, DSCALE, DSUM )
   30    CONTINUE
         BRQA21 = DSCALE*SQRT( DSUM )
*
*        Triangularize the B-part by a QR factorization.
*        Apply transformation (from right) to A-part, giving S.
*
         CALL DGEQR2( M, M, TCPY, LDST, TAUL, WORK, LINFO )
         IF( LINFO.NE.0 )
     $      GO TO 70
         CALL DORM2R( 'L', 'T', M, M, M, TCPY, LDST, TAUL, SCPY, LDST,
     $                WORK, INFO )
         CALL DORM2R( 'R', 'N', M, M, M, TCPY, LDST, TAUL, LICOP, LDST,
     $                WORK, INFO )
         IF( LINFO.NE.0 )
     $      GO TO 70
*
*        Compute F-norm(S21) in BQRA21. (T21 is 0.)
*
         DSCALE = ZERO
         DSUM = ONE
         DO 40 I = 1, N2
            CALL DLASSQ( N1, SCPY( N2+1, I ), 1, DSCALE, DSUM )
   40    CONTINUE
         BQRA21 = DSCALE*SQRT( DSUM )
*
*        Decide which method to use.
*          Weak stability test:
*             F-norm(S21) <= O(EPS * F-norm((S, T)))
*
         IF( BQRA21.LE.BRQA21 .AND. BQRA21.LE.THRESH ) THEN
            CALL DLACPY( 'F', M, M, SCPY, LDST, S, LDST )
            CALL DLACPY( 'F', M, M, TCPY, LDST, T, LDST )
            CALL DLACPY( 'F', M, M, IRCOP, LDST, IR, LDST )
            CALL DLACPY( 'F', M, M, LICOP, LDST, LI, LDST )
         ELSE IF( BRQA21.GE.THRESH ) THEN
            GO TO 70
         END IF
*
*        Set lower triangle of B-part to zero
*
         DO 50 I = 2, M
            CALL DCOPY( M-I+1, ZERO, 0, T( I, I-1 ), 1 )
   50    CONTINUE
*
         IF( WANDS ) THEN
*
*           Strong stability test:
*              F-norm((A-QL*S*QR', B-QL*T*QR')) <= O(EPS*F-norm((A,B)))
*
            CALL DLACPY( 'Full', M, M, A( J1, J1 ), LDA, WORK( M*M+1 ),
     $                   M )
            CALL DGEMM( 'N', 'N', M, M, M, ONE, LI, LDST, S, LDST, ZERO,
     $                  WORK, M )
            CALL DGEMM( 'N', 'N', M, M, M, -ONE, WORK, M, IR, LDST, ONE,
     $                  WORK( M*M+1 ), M )
            DSCALE = ZERO
            DSUM = ONE
            CALL DLASSQ( M*M, WORK( M*M+1 ), 1, DSCALE, DSUM )
*
            CALL DLACPY( 'Full', M, M, B( J1, J1 ), LDB, WORK( M*M+1 ),
     $                   M )
            CALL DGEMM( 'N', 'N', M, M, M, ONE, LI, LDST, T, LDST, ZERO,
     $                  WORK, M )
            CALL DGEMM( 'N', 'N', M, M, M, -ONE, WORK, M, IR, LDST, ONE,
     $                  WORK( M*M+1 ), M )
            CALL DLASSQ( M*M, WORK( M*M+1 ), 1, DSCALE, DSUM )
            SS = DSCALE*SQRT( DSUM )
            DTRONG = ( SS.LE.THRESH )
            IF( .NOT.DTRONG )
     $         GO TO 70
*
         END IF
*
*        If the swap is accepted ("weakly" and "strongly"), apply the
*        transformations and set N1-by-N2 (2,1)-block to zero.
*
         DO 60 I = 1, N2
            CALL DCOPY( N1, ZERO, 0, S( N2+1, I ), 1 )
   60    CONTINUE
*
*        copy back M-by-M diagonal block starting at index J1 of (A, B)
*
         CALL DLACPY( 'F', M, M, S, LDST, A( J1, J1 ), LDA )
         CALL DLACPY( 'F', M, M, T, LDST, B( J1, J1 ), LDB )
         CALL DCOPY( LDST*LDST, ZERO, 0, T, 1 )
*
*        Standardize existing 2-by-2 blocks.
*
         CALL DCOPY( M*M, ZERO, 0, WORK, 1 )
         WORK( 1 ) = ONE
         T( 1, 1 ) = ONE
         IDUM = LWORK - M*M - 2
         IF( N2.GT.1 ) THEN
            CALL DLAGV2( A( J1, J1 ), LDA, B( J1, J1 ), LDB, AR, AI, BE,
     $                   WORK( 1 ), WORK( 2 ), T( 1, 1 ), T( 2, 1 ) )
            WORK( M+1 ) = -WORK( 2 )
            WORK( M+2 ) = WORK( 1 )
            T( N2, N2 ) = T( 1, 1 )
            T( 1, 2 ) = -T( 2, 1 )
         END IF
         WORK( M*M ) = ONE
         T( M, M ) = ONE
*
         IF( N1.GT.1 ) THEN
            CALL DLAGV2( A( J1+N2, J1+N2 ), LDA, B( J1+N2, J1+N2 ), LDB,
     $                   TAUR, TAUL, WORK( M*M+1 ), WORK( N2*M+N2+1 ),
     $                   WORK( N2*M+N2+2 ), T( N2+1, N2+1 ),
     $                   T( M, M-1 ) )
            WORK( M*M ) = WORK( N2*M+N2+1 )
            WORK( M*M-1 ) = -WORK( N2*M+N2+2 )
            T( M, M ) = T( N2+1, N2+1 )
            T( M-1, M ) = -T( M, M-1 )
         END IF
         CALL DGEMM( 'T', 'N', N2, N1, N2, ONE, WORK, M, A( J1, J1+N2 ),
     $               LDA, ZERO, WORK( M*M+1 ), N2 )
         CALL DLACPY( 'Full', N2, N1, WORK( M*M+1 ), N2, A( J1, J1+N2 ),
     $                LDA )
         CALL DGEMM( 'T', 'N', N2, N1, N2, ONE, WORK, M, B( J1, J1+N2 ),
     $               LDB, ZERO, WORK( M*M+1 ), N2 )
         CALL DLACPY( 'Full', N2, N1, WORK( M*M+1 ), N2, B( J1, J1+N2 ),
     $                LDB )
         CALL DGEMM( 'N', 'N', M, M, M, ONE, LI, LDST, WORK, M, ZERO,
     $               WORK( M*M+1 ), M )
         CALL DLACPY( 'Full', M, M, WORK( M*M+1 ), M, LI, LDST )
         CALL DGEMM( 'N', 'N', N2, N1, N1, ONE, A( J1, J1+N2 ), LDA,
     $               T( N2+1, N2+1 ), LDST, ZERO, WORK, N2 )
         CALL DLACPY( 'Full', N2, N1, WORK, N2, A( J1, J1+N2 ), LDA )
         CALL DGEMM( 'N', 'N', N2, N1, N1, ONE, B( J1, J1+N2 ), LDA,
     $               T( N2+1, N2+1 ), LDST, ZERO, WORK, N2 )
         CALL DLACPY( 'Full', N2, N1, WORK, N2, B( J1, J1+N2 ), LDB )
         CALL DGEMM( 'T', 'N', M, M, M, ONE, IR, LDST, T, LDST, ZERO,
     $               WORK, M )
         CALL DLACPY( 'Full', M, M, WORK, M, IR, LDST )
*
*        Accumulate transformations into Q and Z if requested.
*
         IF( WANTQ ) THEN
            CALL DGEMM( 'N', 'N', N, M, M, ONE, Q( 1, J1 ), LDQ, LI,
     $                  LDST, ZERO, WORK, N )
            CALL DLACPY( 'Full', N, M, WORK, N, Q( 1, J1 ), LDQ )
*
         END IF
*
         IF( WANTZ ) THEN
            CALL DGEMM( 'N', 'N', N, M, M, ONE, Z( 1, J1 ), LDZ, IR,
     $                  LDST, ZERO, WORK, N )
            CALL DLACPY( 'Full', N, M, WORK, N, Z( 1, J1 ), LDZ )
*
         END IF
*
*        Update (A(J1:J1+M-1, M+J1:N), B(J1:J1+M-1, M+J1:N)) and
*                (A(1:J1-1, J1:J1+M), B(1:J1-1, J1:J1+M)).
*
         I = J1 + M
         IF( I.LE.N ) THEN
            CALL DGEMM( 'T', 'N', M, N-I+1, M, ONE, LI, LDST,
     $                  A( J1, I ), LDA, ZERO, WORK, M )
            CALL DLACPY( 'Full', M, N-I+1, WORK, M, A( J1, I ), LDA )
            CALL DGEMM( 'T', 'N', M, N-I+1, M, ONE, LI, LDST,
     $                  B( J1, I ), LDA, ZERO, WORK, M )
            CALL DLACPY( 'Full', M, N-I+1, WORK, M, B( J1, I ), LDA )
         END IF
         I = J1 - 1
         IF( I.GT.0 ) THEN
            CALL DGEMM( 'N', 'N', I, M, M, ONE, A( 1, J1 ), LDA, IR,
     $                  LDST, ZERO, WORK, I )
            CALL DLACPY( 'Full', I, M, WORK, I, A( 1, J1 ), LDA )
            CALL DGEMM( 'N', 'N', I, M, M, ONE, B( 1, J1 ), LDB, IR,
     $                  LDST, ZERO, WORK, I )
            CALL DLACPY( 'Full', I, M, WORK, I, B( 1, J1 ), LDB )
         END IF
*
*        Exit with INFO = 0 if swap was successfully performed.
*
         RETURN
*
      END IF
*
*     Exit with INFO = 1 if swap was rejected.
*
   70 CONTINUE
*
      INFO = 1
      RETURN
*
*     End of DTGEX2
*
      END
      SUBROUTINE DTGEXC( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ, Z,
     $                   LDZ, IFST, ILST, WORK, LWORK, INFO )
*
*  -- LAPACK routine (version 2.1) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 15, 1996
*
*     .. Scalar Arguments ..
      LOGICAL            WANTQ, WANTZ
      INTEGER            IFST, ILST, INFO, LDA, LDB, LDQ, LDZ, LWORK, N
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION   A( LDA, * ), B( LDB, * ), Q( LDQ, * ),
     $                   WORK( * ), Z( LDZ, * )
*     ..
*
*  Purpose
*  =======
*
*  DTGEXC reorders the generalized real Schur decomposition of a real
*  matrix pair (A,B) using an orthogonal equivalence transformation
*
*                 (A, B) = Q * (A, B) * Z',
*
*  so that the diagonal block of (A, B) with row index IFST is moved
*  to row ILST.
*
*  (A, B) must be in generalized real Schur canonical form (as returned
*  by DGGES), i.e. A is block upper triangular with 1-by-1 and 2-by-2
*  diagonal blocks. B is upper triangular.
*
*  Optionally, the matrices Q and Z of generalized Schur vectors are
*  updated.
*
*         Q(in) * A(in) * Z(in)' = Q(out) * A(out) * Z(out)'
*         Q(in) * B(in) * Z(in)' = Q(out) * B(out) * Z(out)'
*
*
*  Arguments
*  =========
*
*  WANTQ   (input) LOGICAL
*          .TRUE. : update the left transformation matrix Q;
*          .FALSE.: do not update Q.
*
*  WANTZ   (input) LOGICAL
*          .TRUE. : update the right transformation matrix Z;
*          .FALSE.: do not update Z.
*
*  N       (input) INTEGER
*          The order of the matrices A and B. N >= 0.
*
*  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
*          On entry, the matrix A in generalized real Schur canonical
*          form.
*          On exit, the updated matrix A, again in generalized
*          real Schur canonical form.
*
*  LDA     (input)  INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  B       (input/output) DOUBLE PRECISION array, dimension (LDB,N)
*          On entry, the matrix B in generalized real Schur canonical
*          form (A,B).
*          On exit, the updated matrix B, again in generalized
*          real Schur canonical form (A,B).
*
*  LDB     (input)  INTEGER
*          The leading dimension of the array B. LDB >= max(1,N).
*
*  Q       (input/output) DOUBLE PRECISION array, dimension (LDZ,N)
*          On entry, if WANTQ = .TRUE., the orthogonal matrix Q.
*          On exit, the updated matrix Q.
*          If WANTQ = .FALSE., Q is not referenced.
*
*  LDQ     (input) INTEGER
*          The leading dimension of the array Q. LDQ >= 1.
*          If WANTQ = .TRUE., LDQ >= N.
*
*  Z       (input/output) DOUBLE PRECISION array, dimension (LDZ,N)
*          On entry, if WANTZ = .TRUE., the orthogonal matrix Z.
*          On exit, the updated matrix Z.
*          If WANTZ = .FALSE., Z is not referenced.
*
*  LDZ     (input) INTEGER
*          The leading dimension of the array Z. LDZ >= 1.
*          If WANTZ = .TRUE., LDZ >= N.
*
*  IFST    (input/output) INTEGER
*  ILST    (input/output) INTEGER
*          Specify the reordering of the diagonal blocks of (A, B).
*          The block with row index IFST is moved to row ILST, by a
*          sequence of swapping between adjacent blocks.
*          On exit, if IFST pointed on entry to the second row of
*          a 2-by-2 block, it is changed to point to the first row;
*          ILST always points to the first row of the block in its
*          final position (which may differ from its input value by
*          +1 or -1). 1 <= IFST, ILST <= N.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK. LWORK >= 4*N + 16.
*
*  INFO    (output) INTEGER
*           =0:  successful exit.
*           <0:  if INFO = -i, the i-th argument had an illegal value.
*           =1:  The transformed matrix pair (A, B) would be too far
*                from generalized Schur form; the problem is ill-
*                conditioned. (A, B) may have been partially reordered,
*                and ILST points to the first row of the current
*                position of the block being moved.
*
*  Further Details
*  ===============
*
*  Based on contributions by
*     Bo Kagstrom and Peter Poromaa, Department of Computing Science,
*     Umea University, S-901 87 Umea, Sweden.
*
*  [1] B. Kagstrom; A Direct Method for Reordering Eigenvalues in the
*      Generalized Real Schur Form of a Regular Matrix Pair (A, B), in
*      M.S. Moonen et al (eds), Linear Algebra for Large Scale and
*      Real-Time Applications, Kluwer Academic Publ. 1993, pp 195-218.
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO
      PARAMETER          ( ZERO = 0.0D+0 )
*     ..
*     .. Local Scalars ..
      INTEGER            HERE, NBF, NBL, NBNEXT
*     ..
*     .. External Subroutines ..
      EXTERNAL           DTGEX2, XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          MAX
*     ..
*     .. Executable Statements ..
*
*     Decode and test input arguments.
*
      INFO = 0
      IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -5
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDQ.LT.1 .OR. WANTQ .AND. ( LDQ.LT.MAX( 1, N ) ) ) THEN
         INFO = -9
      ELSE IF( LDZ.LT.1 .OR. WANTZ .AND. ( LDZ.LT.MAX( 1, N ) ) ) THEN
         INFO = -11
      ELSE IF( IFST.LT.1 .OR. IFST.GT.N ) THEN
         INFO = -12
      ELSE IF( ILST.LT.1 .OR. ILST.GT.N ) THEN
         INFO = -13
      ELSE IF( LWORK.LT.N*4+16 ) THEN
         WORK( 1 ) = 4*N + 16
         INFO = -15
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'DTGEXC', -INFO )
         RETURN
      END IF
*
*     Quick return if possible
*
      IF( N.LE.1 )
     $   RETURN
*
*     Determine the first row of the specified block and find out
*     if it is 1-by-1 or 2-by-2.
*
      IF( IFST.GT.1 ) THEN
         IF( A( IFST, IFST-1 ).NE.ZERO )
     $      IFST = IFST - 1
      END IF
      NBF = 1
      IF( IFST.LT.N ) THEN
         IF( A( IFST+1, IFST ).NE.ZERO )
     $      NBF = 2
      END IF
*
*     Determine the first row of the final block
*     and find out if it is 1-by-1 or 2-by-2.
*
      IF( ILST.GT.1 ) THEN
         IF( A( ILST, ILST-1 ).NE.ZERO )
     $      ILST = ILST - 1
      END IF
      NBL = 1
      IF( ILST.LT.N ) THEN
         IF( A( ILST+1, ILST ).NE.ZERO )
     $      NBL = 2
      END IF
      IF( IFST.EQ.ILST )
     $   RETURN
*
      IF( IFST.LT.ILST ) THEN
*
*        Update ILST.
*
         IF( NBF.EQ.2 .AND. NBL.EQ.1 )
     $      ILST = ILST - 1
         IF( NBF.EQ.1 .AND. NBL.EQ.2 )
     $      ILST = ILST + 1
*
         HERE = IFST
*
   10    CONTINUE
*
*        Swap with next one below.
*
         IF( NBF.EQ.1 .OR. NBF.EQ.2 ) THEN
*
*           Current block either 1-by-1 or 2-by-2.
*
            NBNEXT = 1
            IF( HERE+NBF+1.LE.N ) THEN
               IF( A( HERE+NBF+1, HERE+NBF ).NE.ZERO )
     $            NBNEXT = 2
            END IF
            CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ, Z,
     $                   LDZ, HERE, NBF, NBNEXT, WORK, LWORK, INFO )
            IF( INFO.NE.0 ) THEN
               ILST = HERE
               RETURN
            END IF
            HERE = HERE + NBNEXT
*
*           Test if 2-by-2 block breaks into two 1-by-1 blocks.
*
            IF( NBF.EQ.2 ) THEN
               IF( A( HERE+1, HERE ).EQ.ZERO )
     $            NBF = 3
            END IF
*
         ELSE
*
*           Current block consists of two 1-by-1 blocks, each of which
*           must be swapped individually.
*
            NBNEXT = 1
            IF( HERE+3.LE.N ) THEN
               IF( A( HERE+3, HERE+2 ).NE.ZERO )
     $            NBNEXT = 2
            END IF
            CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ, Z,
     $                   LDZ, HERE+1, 1, NBNEXT, WORK, LWORK, INFO )
            IF( INFO.NE.0 ) THEN
               ILST = HERE
               RETURN
            END IF
            IF( NBNEXT.EQ.1 ) THEN
*
*              Swap two 1-by-1 blocks.
*
               CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ, Z,
     $                      LDZ, HERE, 1, 1, WORK, LWORK, INFO )
               IF( INFO.NE.0 ) THEN
                  ILST = HERE
                  RETURN
               END IF
               HERE = HERE + 1
*
            ELSE
*
*              Recompute NBNEXT in case of 2-by-2 split.
*
               IF( A( HERE+2, HERE+1 ).EQ.ZERO )
     $            NBNEXT = 1
               IF( NBNEXT.EQ.2 ) THEN
*
*                 2-by-2 block did not split.
*
                  CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ,
     $                         Z, LDZ, HERE, 1, NBNEXT, WORK, LWORK,
     $                         INFO )
                  IF( INFO.NE.0 ) THEN
                     ILST = HERE
                     RETURN
                  END IF
                  HERE = HERE + 2
               ELSE
*
*                 2-by-2 block did split.
*
                  CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ,
     $                         Z, LDZ, HERE, 1, 1, WORK, LWORK, INFO )
                  IF( INFO.NE.0 ) THEN
                     ILST = HERE
                     RETURN
                  END IF
                  HERE = HERE + 1
                  CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ,
     $                         Z, LDZ, HERE, 1, 1, WORK, LWORK, INFO )
                  IF( INFO.NE.0 ) THEN
                     ILST = HERE
                     RETURN
                  END IF
                  HERE = HERE + 1
               END IF
*
            END IF
         END IF
         IF( HERE.LT.ILST )
     $      GO TO 10
      ELSE
         HERE = IFST
*
   20    CONTINUE
*
*        Swap with next one below.
*
         IF( NBF.EQ.1 .OR. NBF.EQ.2 ) THEN
*
*           Current block either 1-by-1 or 2-by-2.
*
            NBNEXT = 1
            IF( HERE.GE.3 ) THEN
               IF( A( HERE-1, HERE-2 ).NE.ZERO )
     $            NBNEXT = 2
            END IF
            CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ, Z,
     $                   LDZ, HERE-NBNEXT, NBNEXT, NBF, WORK, LWORK,
     $                   INFO )
            IF( INFO.NE.0 ) THEN
               ILST = HERE
               RETURN
            END IF
            HERE = HERE - NBNEXT
*
*           Test if 2-by-2 block breaks into two 1-by-1 blocks.
*
            IF( NBF.EQ.2 ) THEN
               IF( A( HERE+1, HERE ).EQ.ZERO )
     $            NBF = 3
            END IF
*
         ELSE
*
*           Current block consists of two 1-by-1 blocks, each of which
*           must be swapped individually.
*
            NBNEXT = 1
            IF( HERE.GE.3 ) THEN
               IF( A( HERE-1, HERE-2 ).NE.ZERO )
     $            NBNEXT = 2
            END IF
            CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ, Z,
     $                   LDZ, HERE-NBNEXT, NBNEXT, 1, WORK, LWORK,
     $                   INFO )
            IF( INFO.NE.0 ) THEN
               ILST = HERE
               RETURN
            END IF
            IF( NBNEXT.EQ.1 ) THEN
*
*              Swap two 1-by-1 blocks.
*
               CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ, Z,
     $                      LDZ, HERE, NBNEXT, 1, WORK, LWORK, INFO )
               IF( INFO.NE.0 ) THEN
                  ILST = HERE
                  RETURN
               END IF
               HERE = HERE - 1
            ELSE
*
*             Recompute NBNEXT in case of 2-by-2 split.
*
               IF( A( HERE, HERE-1 ).EQ.ZERO )
     $            NBNEXT = 1
               IF( NBNEXT.EQ.2 ) THEN
*
*                 2-by-2 block did not split.
*
                  CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ,
     $                         Z, LDZ, HERE-1, 2, 1, WORK, LWORK, INFO )
                  IF( INFO.NE.0 ) THEN
                     ILST = HERE
                     RETURN
                  END IF
                  HERE = HERE - 2
               ELSE
*
*                 2-by-2 block did split.
*
                  CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ,
     $                         Z, LDZ, HERE, 1, 1, WORK, LWORK, INFO )
                  IF( INFO.NE.0 ) THEN
                     ILST = HERE
                     RETURN
                  END IF
                  HERE = HERE - 1
                  CALL DTGEX2( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ,
     $                         Z, LDZ, HERE, 1, 1, WORK, LWORK, INFO )
                  IF( INFO.NE.0 ) THEN
                     ILST = HERE
                     RETURN
                  END IF
                  HERE = HERE - 1
               END IF
            END IF
         END IF
         IF( HERE.GT.ILST )
     $      GO TO 20
      END IF
      ILST = HERE
      RETURN
*
*     End of DTGEXC
*
      END
      SUBROUTINE DTGSY2( TRANS, IJOB, M, N, A, LDA, B, LDB, C, LDC, D,
     $                   LDD, E, LDE, F, LDF, SCALE, RDSUM, RDSCAL,
     $                   IWORK, PQ, INFO )
*
*  -- LAPACK auxiliary routine (version 2.1) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 15, 1996
*
*     .. Scalar Arguments ..
      CHARACTER          TRANS
      INTEGER            IJOB, INFO, LDA, LDB, LDC, LDD, LDE, LDF, M, N,
     $                   PQ
      DOUBLE PRECISION   RDSCAL, RDSUM, SCALE
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), B( LDB, * ), C( LDC, * ),
     $                   D( LDD, * ), E( LDE, * ), F( LDF, * )
*     ..
*
*  Purpose
*  =======
*
*  DTGSY2 solves the generalized Sylvester equation:
*
*              A * R - L * B = scale * C                (1)
*              D * R - L * E = scale * F,
*
*  using Level 1 and 2 BLAS. where R and L are unknown M-by-N matrices,
*  (A, D), (B, E) and (C, F) are given matrix pairs of size M-by-M,
*  N-by-N and M-by-N, respectively, with real entries. (A, D) and (B, E)
*  must be in generalized Schur canonical form, i.e. A, B are upper
*  quasi triangular and D, E are upper triangular. The solution (R, L)
*  overwrites (C, F). 0 <= SCALE <= 1 is an output scaling factor
*  chosen to avoid overflow.
*
*  In matrix notation solving equation (1) corresponds to solve
*  Z*x = scale*b, where Z is defined as
*
*         Z = [ kron(In, A)  -kron(B', Im) ]             (2)
*             [ kron(In, D)  -kron(E', Im) ],
*
*  Ik is the identity matrix of size k and X' is the transpose of X.
*  kron(X, Y) is the Kronecker product between the matrices X and Y.
*  In the process of solving (1), we solve a number of such systems
*  where Dim(In), Dim(In) = 1 or 2.
*
*  If TRANS = 'T', solve the transposed system Z'*y = scale*b for y,
*  which is equivalent to solve for R and L in
*
*              A' * R  + D' * L   = scale *  C           (3)
*              R  * B' + L  * E'  = scale * -F
*
*  This case is used to compute an estimate of Dif[(A, D), (B, E)] =
*  sigma_min(Z) using reverse communicaton with DLACON.
*
*  DTGSY2 also (IJOB >= 1) contributes to the computation in DTGSYL
*  of an upper bound on the separation between to matrix pairs. Then
*  the input (A, D), (B, E) are sub-pencils of the matrix pair in
*  DTGSYL. See DTGSYL for details.
*
*  Arguments
*  =========
*
*  TRANS   (input) CHARACTER
*          = 'N', solve the generalized Sylvester equation (1).
*          = 'T': solve the 'transposed' system (3).
*
*  IJOB    (input) INTEGER
*          Specifies what kind of functionality to be performed.
*          = 0: solve (1) only.
*          = 1: A contribution from this subsystem to a Frobenius
*               norm-based estimate of the separation between two matrix
*               pairs is computed. (look ahead strategy is used).
*          = 2: A contribution from this subsystem to a Frobenius
*               norm-based estimate of the separation between two matrix
*               pairs is computed. (DGECON on sub-systems is used.)
*          Not referenced if TRANS = 'T'.
*
*  M       (input) INTEGER
*          On entry, M specifies the order of A and D, and the row
*          dimension of C, F, R and L.
*
*  N       (input) INTEGER
*          On entry, N specifies the order of B and E, and the column
*          dimension of C, F, R and L.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA, M)
*          On entry, A contains an upper quasi triangular matrix.
*
*  LDA     (input) INTEGER
*          The leading dimension of the matrix A. LDA >= max(1, M).
*
*  B       (input) DOUBLE PRECISION array, dimension (LDB, N)
*          On entry, B contains an upper quasi triangular matrix.
*
*  LDB     (input) INTEGER
*          The leading dimension of the matrix B. LDB >= max(1, N).
*
*  C       (input/ output) DOUBLE PRECISION array, dimension (LDC, N)
*          On entry, C contains the right-hand-side of the first matrix
*          equation in (1).
*          On exit, if IJOB = 0, C has been overwritten by the
*          solution R.
*
*  LDC     (input) INTEGER
*          The leading dimension of the matrix C. LDC >= max(1, M).
*
*  D       (input) DOUBLE PRECISION array, dimension (LDD, M)
*          On entry, D contains an upper triangular matrix.
*
*  LDD     (input) INTEGER
*          The leading dimension of the matrix D. LDD >= max(1, M).
*
*  E       (input) DOUBLE PRECISION array, dimension (LDE, N)
*          On entry, E contains an upper triangular matrix.
*
*  LDE     (input) INTEGER
*          The leading dimension of the matrix E. LDE >= max(1, N).
*
*  F       (input/ output) DOUBLE PRECISION array, dimension (LDF, N)
*          On entry, F contains the right-hand-side of the second matrix
*          equation in (1).
*          On exit, if IJOB = 0, F has been overwritten by the
*          solution L.
*
*  LDF     (input) INTEGER
*          The leading dimension of the matrix F. LDF >= max(1, M).
*
*  SCALE   (output) DOUBLE PRECISION
*          On exit, 0 <= SCALE <= 1. If 0 < SCALE < 1, the solutions
*          R and L (C and F on entry) will hold the solutions to a
*          slightly perturbed system but the input matrices A, B, D and
*          E have not been changed. If SCALE = 0, R and L will hold the
*          solutions to the homogeneous system with C = F = 0. Normally,
*          SCALE = 1.
*
*  RDSUM   (input/output) DOUBLE PRECISION
*          On entry, the sum of squares of computed contributions to
*          the Dif-estimate under computation by DTGSYL, where the
*          scaling factor RDSCAL (see below) has been factored out.
*          On exit, the corresponding sum of squares updated with the
*          contributions from the current sub-system.
*          If TRANS = 'T' RDSUM is not touched.
*          NOTE: RDSUM only makes sense when DTGSY2 is called by DTGSYL.
*
*  RDSCAL  (input/output) DOUBLE PRECISION
*          On entry, scaling factor used to prevent overflow in RDSUM.
*          On exit, RDSCAL is updated w.r.t. the current contributions
*          in RDSUM.
*          If TRANS = 'T', RDSCAL is not touched.
*          NOTE: RDSCAL only makes sense when DTGSY2 is called by
*                DTGSYL.
*
*  IWORK   (workspace) INTEGER array, dimension (M+N+2)
*
*  PQ      (output) INTEGER
*          On exit, the number of subsystems (of size 2-by-2, 4-by-4 and
*          8-by-8) solved by this routine.
*
*  INFO    (output) INTEGER
*          On exit, if INFO is set to
*            =0: Successful exit
*            <0: If INFO = -i, the i-th argument had an illegal value.
*            >0: The matrix pairs (A, D) and (B, E) have common or very
*                close eigenvalues.
*
*  Further Details
*  ===============
*
*  Based on contributions by
*     Bo Kagstrom and Peter Poromaa, Department of Computing Science,
*     Umea University, S-901 87 Umea, Sweden.
*
*  =====================================================================
*
*     .. Parameters ..
      INTEGER            LDZ
      PARAMETER          ( LDZ = 8 )
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            NOTRAN
      INTEGER            I, IE, IERR, II, IS, ISP1, J, JE, JJ, JS, JSP1,
     $                   K, MB, NB, P, Q, ZDIM
      DOUBLE PRECISION   ALPHA, SCALOC
*     ..
*     .. Local Arrays ..
      INTEGER            IPIV( LDZ ), JPIV( LDZ )
      DOUBLE PRECISION   RHS( LDZ ), Z( LDZ, LDZ )
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      EXTERNAL           LSAME
*     ..
*     .. External Subroutines ..
      EXTERNAL           DAXPY, DCOPY, DGEMM, DGEMV, DGER, DGESC2,
     $                   DGETC2, DLATDF, DSCAL, XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          MAX
*     ..
*     .. Executable Statements ..
*
*     Decode and test input parameters
*
      INFO = 0
      IERR = 0
      NOTRAN = LSAME( TRANS, 'N' )
      IF( .NOT.NOTRAN .AND. .NOT.LSAME( TRANS, 'T' ) ) THEN
         INFO = -1
      ELSE IF( ( IJOB.LT.0 ) .OR. ( IJOB.GT.2 ) ) THEN
         INFO = -2
      ELSE IF( M.LE.0 ) THEN
         INFO = -3
      ELSE IF( N.LE.0 ) THEN
         INFO = -4
      ELSE IF( LDA.LT.MAX( 1, M ) ) THEN
         INFO = -5
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -8
      ELSE IF( LDC.LT.MAX( 1, M ) ) THEN
         INFO = -10
      ELSE IF( LDD.LT.MAX( 1, M ) ) THEN
         INFO = -12
      ELSE IF( LDE.LT.MAX( 1, N ) ) THEN
         INFO = -14
      ELSE IF( LDF.LT.MAX( 1, M ) ) THEN
         INFO = -16
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'DTGSY2', -INFO )
         RETURN
      END IF
*
*     Determine block structure of A
*
      PQ = 0
      P = 0
      I = 1
   10 CONTINUE
      IF( I.GT.M )
     $   GO TO 20
      P = P + 1
      IWORK( P ) = I
      IF( I.EQ.M )
     $   GO TO 20
      IF( A( I+1, I ).NE.ZERO ) THEN
         I = I + 2
      ELSE
         I = I + 1
      END IF
      GO TO 10
   20 CONTINUE
      IWORK( P+1 ) = M + 1
*
*     Determine block structure of B
*
      Q = P + 1
      J = 1
   30 CONTINUE
      IF( J.GT.N )
     $   GO TO 40
      Q = Q + 1
      IWORK( Q ) = J
      IF( J.EQ.N )
     $   GO TO 40
      IF( B( J+1, J ).NE.ZERO ) THEN
         J = J + 2
      ELSE
         J = J + 1
      END IF
      GO TO 30
   40 CONTINUE
      IWORK( Q+1 ) = N + 1
      PQ = P*( Q-P-1 )
*
      IF( NOTRAN ) THEN
*
*        Solve (I, J) - subsystem
*           A(I, I) * R(I, J) - L(I, J) * B(J, J) = C(I, J)
*           D(I, I) * R(I, J) - L(I, J) * E(J, J) = F(I, J)
*        for I = P, P - 1, ..., 1; J = 1, 2, ..., Q
*
         SCALE = ONE
         SCALOC = ONE
         DO 120 J = P + 2, Q
            JS = IWORK( J )
            JSP1 = JS + 1
            JE = IWORK( J+1 ) - 1
            NB = JE - JS + 1
            DO 110 I = P, 1, -1
*
               IS = IWORK( I )
               ISP1 = IS + 1
               IE = IWORK( I+1 ) - 1
               MB = IE - IS + 1
               ZDIM = MB*NB*2
*
               IF( ( MB.EQ.1 ) .AND. ( NB.EQ.1 ) ) THEN
*
*                 Build a 2-by-2 system Z * x = RHS
*
                  Z( 1, 1 ) = A( IS, IS )
                  Z( 2, 1 ) = D( IS, IS )
                  Z( 1, 2 ) = -B( JS, JS )
                  Z( 2, 2 ) = -E( JS, JS )
*
*                 Set up right hand side(s)
*
                  RHS( 1 ) = C( IS, JS )
                  RHS( 2 ) = F( IS, JS )
*
*                 Solve Z * x = RHS
*
                  CALL DGETC2( ZDIM, Z, LDZ, IPIV, JPIV, IERR )
                  IF( IERR.GT.0 )
     $               INFO = IERR
*
                  IF( IJOB.EQ.0 ) THEN
                     CALL DGESC2( ZDIM, Z, LDZ, RHS, IPIV, JPIV,
     $                            SCALOC )
                     IF( SCALOC.NE.ONE ) THEN
                        DO 50 K = 1, N
                           CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                           CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
   50                   CONTINUE
                        SCALE = SCALE*SCALOC
                     END IF
                  ELSE
                     CALL DLATDF( IJOB, ZDIM, Z, LDZ, RHS, RDSUM,
     $                            RDSCAL, IPIV, JPIV )
                  END IF
*
*                 Unpack solution vector(s)
*
                  C( IS, JS ) = RHS( 1 )
                  F( IS, JS ) = RHS( 2 )
*
*                 Substitute R(I, J) and L(I, J) into remaining
*                 equation.
*
                  IF( I.GT.1 ) THEN
                     ALPHA = -RHS( 1 )
                     CALL DAXPY( IS-1, ALPHA, A( 1, IS ), 1, C( 1, JS ),
     $                           1 )
                     CALL DAXPY( IS-1, ALPHA, D( 1, IS ), 1, F( 1, JS ),
     $                           1 )
                  END IF
                  IF( J.LT.Q ) THEN
                     CALL DAXPY( N-JE, RHS( 2 ), B( JS, JE+1 ), LDB,
     $                           C( IS, JE+1 ), LDC )
                     CALL DAXPY( N-JE, RHS( 2 ), E( JS, JE+1 ), LDE,
     $                           F( IS, JE+1 ), LDF )
                  END IF
*
               ELSE IF( ( MB.EQ.1 ) .AND. ( NB.EQ.2 ) ) THEN
*
*                 Build a 4-by-4 system Z * x = RHS
*
                  Z( 1, 1 ) = A( IS, IS )
                  Z( 2, 1 ) = ZERO
                  Z( 3, 1 ) = D( IS, IS )
                  Z( 4, 1 ) = ZERO
*
                  Z( 1, 2 ) = ZERO
                  Z( 2, 2 ) = A( IS, IS )
                  Z( 3, 2 ) = ZERO
                  Z( 4, 2 ) = D( IS, IS )
*
                  Z( 1, 3 ) = -B( JS, JS )
                  Z( 2, 3 ) = -B( JS, JSP1 )
                  Z( 3, 3 ) = -E( JS, JS )
                  Z( 4, 3 ) = -E( JS, JSP1 )
*
                  Z( 1, 4 ) = -B( JSP1, JS )
                  Z( 2, 4 ) = -B( JSP1, JSP1 )
                  Z( 3, 4 ) = ZERO
                  Z( 4, 4 ) = -E( JSP1, JSP1 )
*
*                 Set up right hand side(s)
*
                  RHS( 1 ) = C( IS, JS )
                  RHS( 2 ) = C( IS, JSP1 )
                  RHS( 3 ) = F( IS, JS )
                  RHS( 4 ) = F( IS, JSP1 )
*
*                 Solve Z * x = RHS
*
                  CALL DGETC2( ZDIM, Z, LDZ, IPIV, JPIV, IERR )
                  IF( IERR.GT.0 )
     $               INFO = IERR
*
                  IF( IJOB.EQ.0 ) THEN
                     CALL DGESC2( ZDIM, Z, LDZ, RHS, IPIV, JPIV,
     $                            SCALOC )
                     IF( SCALOC.NE.ONE ) THEN
                        DO 60 K = 1, N
                           CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                           CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
   60                   CONTINUE
                        SCALE = SCALE*SCALOC
                     END IF
                  ELSE
                     CALL DLATDF( IJOB, ZDIM, Z, LDZ, RHS, RDSUM,
     $                            RDSCAL, IPIV, JPIV )
                  END IF
*
*                 Unpack solution vector(s)
*
                  C( IS, JS ) = RHS( 1 )
                  C( IS, JSP1 ) = RHS( 2 )
                  F( IS, JS ) = RHS( 3 )
                  F( IS, JSP1 ) = RHS( 4 )
*
*                 Substitute R(I, J) and L(I, J) into remaining
*                 equation.
*
                  IF( I.GT.1 ) THEN
                     CALL DGER( IS-1, NB, -ONE, A( 1, IS ), 1, RHS( 1 ),
     $                          1, C( 1, JS ), LDC )
                     CALL DGER( IS-1, NB, -ONE, D( 1, IS ), 1, RHS( 1 ),
     $                          1, F( 1, JS ), LDF )
                  END IF
                  IF( J.LT.Q ) THEN
                     CALL DAXPY( N-JE, RHS( 3 ), B( JS, JE+1 ), LDB,
     $                           C( IS, JE+1 ), LDC )
                     CALL DAXPY( N-JE, RHS( 3 ), E( JS, JE+1 ), LDE,
     $                           F( IS, JE+1 ), LDF )
                     CALL DAXPY( N-JE, RHS( 4 ), B( JSP1, JE+1 ), LDB,
     $                           C( IS, JE+1 ), LDC )
                     CALL DAXPY( N-JE, RHS( 4 ), E( JSP1, JE+1 ), LDE,
     $                           F( IS, JE+1 ), LDF )
                  END IF
*
               ELSE IF( ( MB.EQ.2 ) .AND. ( NB.EQ.1 ) ) THEN
*
*                 Build a 4-by-4 system Z * x = RHS
*
                  Z( 1, 1 ) = A( IS, IS )
                  Z( 2, 1 ) = A( ISP1, IS )
                  Z( 3, 1 ) = D( IS, IS )
                  Z( 4, 1 ) = ZERO
*
                  Z( 1, 2 ) = A( IS, ISP1 )
                  Z( 2, 2 ) = A( ISP1, ISP1 )
                  Z( 3, 2 ) = D( IS, ISP1 )
                  Z( 4, 2 ) = D( ISP1, ISP1 )
*
                  Z( 1, 3 ) = -B( JS, JS )
                  Z( 2, 3 ) = ZERO
                  Z( 3, 3 ) = -E( JS, JS )
                  Z( 4, 3 ) = ZERO
*
                  Z( 1, 4 ) = ZERO
                  Z( 2, 4 ) = -B( JS, JS )
                  Z( 3, 4 ) = ZERO
                  Z( 4, 4 ) = -E( JS, JS )
*
*                 Set up right hand side(s)
*
                  RHS( 1 ) = C( IS, JS )
                  RHS( 2 ) = C( ISP1, JS )
                  RHS( 3 ) = F( IS, JS )
                  RHS( 4 ) = F( ISP1, JS )
*
*                 Solve Z * x = RHS
*
                  CALL DGETC2( ZDIM, Z, LDZ, IPIV, JPIV, IERR )
                  IF( IERR.GT.0 )
     $               INFO = IERR
                  IF( IJOB.EQ.0 ) THEN
                     CALL DGESC2( ZDIM, Z, LDZ, RHS, IPIV, JPIV,
     $                            SCALOC )
                     IF( SCALOC.NE.ONE ) THEN
                        DO 70 K = 1, N
                           CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                           CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
   70                   CONTINUE
                        SCALE = SCALE*SCALOC
                     END IF
                  ELSE
                     CALL DLATDF( IJOB, ZDIM, Z, LDZ, RHS, RDSUM,
     $                            RDSCAL, IPIV, JPIV )
                  END IF
*
*                 Unpack solution vector(s)
*
                  C( IS, JS ) = RHS( 1 )
                  C( ISP1, JS ) = RHS( 2 )
                  F( IS, JS ) = RHS( 3 )
                  F( ISP1, JS ) = RHS( 4 )
*
*                 Substitute R(I, J) and L(I, J) into remaining
*                 equation.
*
                  IF( I.GT.1 ) THEN
                     CALL DGEMV( 'N', IS-1, MB, -ONE, A( 1, IS ), LDA,
     $                           RHS( 1 ), 1, ONE, C( 1, JS ), 1 )
                     CALL DGEMV( 'N', IS-1, MB, -ONE, D( 1, IS ), LDD,
     $                           RHS( 1 ), 1, ONE, F( 1, JS ), 1 )
                  END IF
                  IF( J.LT.Q ) THEN
                     CALL DGER( MB, N-JE, ONE, RHS( 3 ), 1,
     $                          B( JS, JE+1 ), LDB, C( IS, JE+1 ), LDC )
                     CALL DGER( MB, N-JE, ONE, RHS( 3 ), 1,
     $                          E( JS, JE+1 ), LDB, F( IS, JE+1 ), LDC )
                  END IF
*
               ELSE IF( ( MB.EQ.2 ) .AND. ( NB.EQ.2 ) ) THEN
*
*                 Build an 8-by-8 system Z * x = RHS
*
                  CALL DCOPY( LDZ*LDZ, ZERO, 0, Z, 1 )
*
                  Z( 1, 1 ) = A( IS, IS )
                  Z( 2, 1 ) = A( ISP1, IS )
                  Z( 5, 1 ) = D( IS, IS )
*
                  Z( 1, 2 ) = A( IS, ISP1 )
                  Z( 2, 2 ) = A( ISP1, ISP1 )
                  Z( 5, 2 ) = D( IS, ISP1 )
                  Z( 6, 2 ) = D( ISP1, ISP1 )
*
                  Z( 3, 3 ) = A( IS, IS )
                  Z( 4, 3 ) = A( ISP1, IS )
                  Z( 7, 3 ) = D( IS, IS )
*
                  Z( 3, 4 ) = A( IS, ISP1 )
                  Z( 4, 4 ) = A( ISP1, ISP1 )
                  Z( 7, 4 ) = D( IS, ISP1 )
                  Z( 8, 4 ) = D( ISP1, ISP1 )
*
                  Z( 1, 5 ) = -B( JS, JS )
                  Z( 3, 5 ) = -B( JS, JSP1 )
                  Z( 5, 5 ) = -E( JS, JS )
                  Z( 7, 5 ) = -E( JS, JSP1 )
*
                  Z( 2, 6 ) = -B( JS, JS )
                  Z( 4, 6 ) = -B( JS, JSP1 )
                  Z( 6, 6 ) = -E( JS, JS )
                  Z( 8, 6 ) = -E( JS, JSP1 )
*
                  Z( 1, 7 ) = -B( JSP1, JS )
                  Z( 3, 7 ) = -B( JSP1, JSP1 )
                  Z( 7, 7 ) = -E( JSP1, JSP1 )
*
                  Z( 2, 8 ) = -B( JSP1, JS )
                  Z( 4, 8 ) = -B( JSP1, JSP1 )
                  Z( 8, 8 ) = -E( JSP1, JSP1 )
*
*                 Set up right hand side(s)
*
                  K = 1
                  II = MB*NB + 1
                  DO 80 JJ = 0, NB - 1
                     CALL DCOPY( MB, C( IS, JS+JJ ), 1, RHS( K ), 1 )
                     CALL DCOPY( MB, F( IS, JS+JJ ), 1, RHS( II ), 1 )
                     K = K + MB
                     II = II + MB
   80             CONTINUE
*
*                 Solve Z * x = RHS
*
                  CALL DGETC2( ZDIM, Z, LDZ, IPIV, JPIV, IERR )
                  IF( IERR.GT.0 )
     $               INFO = IERR
                  IF( IJOB.EQ.0 ) THEN
                     CALL DGESC2( ZDIM, Z, LDZ, RHS, IPIV, JPIV,
     $                            SCALOC )
                     IF( SCALOC.NE.ONE ) THEN
                        DO 90 K = 1, N
                           CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                           CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
   90                   CONTINUE
                        SCALE = SCALE*SCALOC
                     END IF
                  ELSE
                     CALL DLATDF( IJOB, ZDIM, Z, LDZ, RHS, RDSUM,
     $                            RDSCAL, IPIV, JPIV )
                  END IF
*
*                 Unpack solution vector(s)
*
                  K = 1
                  II = MB*NB + 1
                  DO 100 JJ = 0, NB - 1
                     CALL DCOPY( MB, RHS( K ), 1, C( IS, JS+JJ ), 1 )
                     CALL DCOPY( MB, RHS( II ), 1, F( IS, JS+JJ ), 1 )
                     K = K + MB
                     II = II + MB
  100             CONTINUE
*
*                 Substitute R(I, J) and L(I, J) into remaining
*                 equation.
*
                  IF( I.GT.1 ) THEN
                     CALL DGEMM( 'N', 'N', IS-1, NB, MB, -ONE,
     $                           A( 1, IS ), LDA, RHS( 1 ), MB, ONE,
     $                           C( 1, JS ), LDC )
                     CALL DGEMM( 'N', 'N', IS-1, NB, MB, -ONE,
     $                           D( 1, IS ), LDD, RHS( 1 ), MB, ONE,
     $                           F( 1, JS ), LDF )
                  END IF
                  IF( J.LT.Q ) THEN
                     K = MB*NB + 1
                     CALL DGEMM( 'N', 'N', MB, N-JE, NB, ONE, RHS( K ),
     $                           MB, B( JS, JE+1 ), LDB, ONE,
     $                           C( IS, JE+1 ), LDC )
                     CALL DGEMM( 'N', 'N', MB, N-JE, NB, ONE, RHS( K ),
     $                           MB, E( JS, JE+1 ), LDE, ONE,
     $                           F( IS, JE+1 ), LDF )
                  END IF
*
               END IF
*
  110       CONTINUE
  120    CONTINUE
      ELSE
*
*        Solve (I, J) - subsystem
*             A(I, I)' * R(I, J) + D(I, I)' * L(J, J)  =  C(I, J)
*             R(I, I)  * B(J, J) + L(I, J)  * E(J, J)  = -F(I, J)
*        for I = 1, 2, ..., P, J = Q, Q - 1, ..., 1
*
         SCALE = ONE
         SCALOC = ONE
         DO 200 I = 1, P
*
            IS = IWORK( I )
            ISP1 = IS + 1
            IE = IWORK( I+1 ) - 1
            MB = IE - IS + 1
            DO 190 J = Q, P + 2, -1
*
               JS = IWORK( J )
               JSP1 = JS + 1
               JE = IWORK( J+1 ) - 1
               NB = JE - JS + 1
               ZDIM = MB*NB*2
               IF( ( MB.EQ.1 ) .AND. ( NB.EQ.1 ) ) THEN
*
*                 Build a 2-by-2 system Z' * x = RHS
*
                  Z( 1, 1 ) = A( IS, IS )
                  Z( 2, 1 ) = -B( JS, JS )
                  Z( 1, 2 ) = D( IS, IS )
                  Z( 2, 2 ) = -E( JS, JS )
*
*                 Set up right hand side(s)
*
                  RHS( 1 ) = C( IS, JS )
                  RHS( 2 ) = F( IS, JS )
*
*                 Solve Z' * x = RHS
*
                  CALL DGETC2( ZDIM, Z, LDZ, IPIV, JPIV, IERR )
                  IF( IERR.GT.0 )
     $               INFO = IERR
*
                  CALL DGESC2( ZDIM, Z, LDZ, RHS, IPIV, JPIV, SCALOC )
                  IF( SCALOC.NE.ONE ) THEN
                     DO 130 K = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                        CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
  130                CONTINUE
                     SCALE = SCALE*SCALOC
                  END IF
*
*                 Unpack solution vector(s)
*
                  C( IS, JS ) = RHS( 1 )
                  F( IS, JS ) = RHS( 2 )
*
*                 Substitute R(I, J) and L(I, J) into remaining
*                 equation.
*
                  IF( J.GT.P+2 ) THEN
                     ALPHA = RHS( 1 )
                     CALL DAXPY( JS-1, ALPHA, B( 1, JS ), 1, F( IS, 1 ),
     $                           LDF )
                     ALPHA = RHS( 2 )
                     CALL DAXPY( JS-1, ALPHA, E( 1, JS ), 1, F( IS, 1 ),
     $                           LDF )
                  END IF
                  IF( I.LT.P ) THEN
                     ALPHA = -RHS( 1 )
                     CALL DAXPY( M-IE, ALPHA, A( IS, IE+1 ), LDA,
     $                           C( IE+1, JS ), 1 )
                     ALPHA = -RHS( 2 )
                     CALL DAXPY( M-IE, ALPHA, D( IS, IE+1 ), LDD,
     $                           C( IE+1, JS ), 1 )
                  END IF
*
               ELSE IF( ( MB.EQ.1 ) .AND. ( NB.EQ.2 ) ) THEN
*
*                 Build a 4-by-4 system Z' * x = RHS
*
                  Z( 1, 1 ) = A( IS, IS )
                  Z( 2, 1 ) = ZERO
                  Z( 3, 1 ) = -B( JS, JS )
                  Z( 4, 1 ) = -B( JSP1, JS )
*
                  Z( 1, 2 ) = ZERO
                  Z( 2, 2 ) = A( IS, IS )
                  Z( 3, 2 ) = -B( JS, JSP1 )
                  Z( 4, 2 ) = -B( JSP1, JSP1 )
*
                  Z( 1, 3 ) = D( IS, IS )
                  Z( 2, 3 ) = ZERO
                  Z( 3, 3 ) = -E( JS, JS )
                  Z( 4, 3 ) = ZERO
*
                  Z( 1, 4 ) = ZERO
                  Z( 2, 4 ) = D( IS, IS )
                  Z( 3, 4 ) = -E( JS, JSP1 )
                  Z( 4, 4 ) = -E( JSP1, JSP1 )
*
*                 Set up right hand side(s)
*
                  RHS( 1 ) = C( IS, JS )
                  RHS( 2 ) = C( IS, JSP1 )
                  RHS( 3 ) = F( IS, JS )
                  RHS( 4 ) = F( IS, JSP1 )
*
*                 Solve Z' * x = RHS
*
                  CALL DGETC2( ZDIM, Z, LDZ, IPIV, JPIV, IERR )
                  IF( IERR.GT.0 )
     $               INFO = IERR
                  CALL DGESC2( ZDIM, Z, LDZ, RHS, IPIV, JPIV, SCALOC )
                  IF( SCALOC.NE.ONE ) THEN
                     DO 140 K = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                        CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
  140                CONTINUE
                     SCALE = SCALE*SCALOC
                  END IF
*
*                 Unpack solution vector(s)
*
                  C( IS, JS ) = RHS( 1 )
                  C( IS, JSP1 ) = RHS( 2 )
                  F( IS, JS ) = RHS( 3 )
                  F( IS, JSP1 ) = RHS( 4 )
*
*                 Substitute R(I, J) and L(I, J) into remaining
*                 equation.
*
                  IF( J.GT.P+2 ) THEN
                     CALL DAXPY( JS-1, RHS( 1 ), B( 1, JS ), 1,
     $                           F( IS, 1 ), LDF )
                     CALL DAXPY( JS-1, RHS( 2 ), B( 1, JSP1 ), 1,
     $                           F( IS, 1 ), LDF )
                     CALL DAXPY( JS-1, RHS( 3 ), E( 1, JS ), 1,
     $                           F( IS, 1 ), LDF )
                     CALL DAXPY( JS-1, RHS( 4 ), E( 1, JSP1 ), 1,
     $                           F( IS, 1 ), LDF )
                  END IF
                  IF( I.LT.P ) THEN
                     CALL DGER( M-IE, NB, -ONE, A( IS, IE+1 ), LDA,
     $                          RHS( 1 ), 1, C( IE+1, JS ), LDC )
                     CALL DGER( M-IE, NB, -ONE, D( IS, IE+1 ), LDD,
     $                          RHS( 3 ), 1, C( IE+1, JS ), LDC )
                  END IF
*
               ELSE IF( ( MB.EQ.2 ) .AND. ( NB.EQ.1 ) ) THEN
*
*                 Build a 4-by-4 system Z' * x = RHS
*
                  Z( 1, 1 ) = A( IS, IS )
                  Z( 2, 1 ) = A( IS, ISP1 )
                  Z( 3, 1 ) = -B( JS, JS )
                  Z( 4, 1 ) = ZERO
*
                  Z( 1, 2 ) = A( ISP1, IS )
                  Z( 2, 2 ) = A( ISP1, ISP1 )
                  Z( 3, 2 ) = ZERO
                  Z( 4, 2 ) = -B( JS, JS )
*
                  Z( 1, 3 ) = D( IS, IS )
                  Z( 2, 3 ) = D( IS, ISP1 )
                  Z( 3, 3 ) = -E( JS, JS )
                  Z( 4, 3 ) = ZERO
*
                  Z( 1, 4 ) = ZERO
                  Z( 2, 4 ) = D( ISP1, ISP1 )
                  Z( 3, 4 ) = ZERO
                  Z( 4, 4 ) = -E( JS, JS )
*
*                 Set up right hand side(s)
*
                  RHS( 1 ) = C( IS, JS )
                  RHS( 2 ) = C( ISP1, JS )
                  RHS( 3 ) = F( IS, JS )
                  RHS( 4 ) = F( ISP1, JS )
*
*                 Solve Z' * x = RHS
*
                  CALL DGETC2( ZDIM, Z, LDZ, IPIV, JPIV, IERR )
                  IF( IERR.GT.0 )
     $               INFO = IERR
*
                  CALL DGESC2( ZDIM, Z, LDZ, RHS, IPIV, JPIV, SCALOC )
                  IF( SCALOC.NE.ONE ) THEN
                     DO 150 K = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                        CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
  150                CONTINUE
                     SCALE = SCALE*SCALOC
                  END IF
*
*                 Unpack solution vector(s)
*
                  C( IS, JS ) = RHS( 1 )
                  C( ISP1, JS ) = RHS( 2 )
                  F( IS, JS ) = RHS( 3 )
                  F( ISP1, JS ) = RHS( 4 )
*
*                 Substitute R(I, J) and L(I, J) into remaining
*                 equation.
*
                  IF( J.GT.P+2 ) THEN
                     CALL DGER( MB, JS-1, ONE, RHS( 1 ), 1, B( 1, JS ),
     $                          1, F( IS, 1 ), LDF )
                     CALL DGER( MB, JS-1, ONE, RHS( 3 ), 1, E( 1, JS ),
     $                          1, F( IS, 1 ), LDF )
                  END IF
                  IF( I.LT.P ) THEN
                     CALL DGEMV( 'T', MB, M-IE, -ONE, A( IS, IE+1 ),
     $                           LDA, RHS( 1 ), 1, ONE, C( IE+1, JS ),
     $                           1 )
                     CALL DGEMV( 'T', MB, M-IE, -ONE, D( IS, IE+1 ),
     $                           LDD, RHS( 3 ), 1, ONE, C( IE+1, JS ),
     $                           1 )
                  END IF
*
               ELSE IF( ( MB.EQ.2 ) .AND. ( NB.EQ.2 ) ) THEN
*
*                 Build an 8-by-8 system Z' * x = RHS
*
                  CALL DCOPY( LDZ*LDZ, ZERO, 0, Z, 1 )
*
                  Z( 1, 1 ) = A( IS, IS )
                  Z( 2, 1 ) = A( IS, ISP1 )
                  Z( 5, 1 ) = -B( JS, JS )
                  Z( 7, 1 ) = -B( JSP1, JS )
*
                  Z( 1, 2 ) = A( ISP1, IS )
                  Z( 2, 2 ) = A( ISP1, ISP1 )
                  Z( 6, 2 ) = -B( JS, JS )
                  Z( 8, 2 ) = -B( JSP1, JS )
*
                  Z( 3, 3 ) = A( IS, IS )
                  Z( 4, 3 ) = A( IS, ISP1 )
                  Z( 5, 3 ) = -B( JS, JSP1 )
                  Z( 7, 3 ) = -B( JSP1, JSP1 )
*
                  Z( 3, 4 ) = A( ISP1, IS )
                  Z( 4, 4 ) = A( ISP1, ISP1 )
                  Z( 6, 4 ) = -B( JS, JSP1 )
                  Z( 8, 4 ) = -B( JSP1, JSP1 )
*
                  Z( 1, 5 ) = D( IS, IS )
                  Z( 2, 5 ) = D( IS, ISP1 )
                  Z( 5, 5 ) = -E( JS, JS )
*
                  Z( 2, 6 ) = D( ISP1, ISP1 )
                  Z( 6, 6 ) = -E( JS, JS )
*
                  Z( 3, 7 ) = D( IS, IS )
                  Z( 4, 7 ) = D( IS, ISP1 )
                  Z( 5, 7 ) = -E( JS, JSP1 )
                  Z( 7, 7 ) = -E( JSP1, JSP1 )
*
                  Z( 4, 8 ) = D( ISP1, ISP1 )
                  Z( 6, 8 ) = -E( JS, JSP1 )
                  Z( 8, 8 ) = -E( JSP1, JSP1 )
*
*                 Set up right hand side(s)
*
                  K = 1
                  II = MB*NB + 1
                  DO 160 JJ = 0, NB - 1
                     CALL DCOPY( MB, C( IS, JS+JJ ), 1, RHS( K ), 1 )
                     CALL DCOPY( MB, F( IS, JS+JJ ), 1, RHS( II ), 1 )
                     K = K + MB
                     II = II + MB
  160             CONTINUE
*
*
*                 Solve Z' * x = RHS
*
                  CALL DGETC2( ZDIM, Z, LDZ, IPIV, JPIV, IERR )
                  IF( IERR.GT.0 )
     $               INFO = IERR
*
                  CALL DGESC2( ZDIM, Z, LDZ, RHS, IPIV, JPIV, SCALOC )
                  IF( SCALOC.NE.ONE ) THEN
                     DO 170 K = 1, N
                        CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                        CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
  170                CONTINUE
                     SCALE = SCALE*SCALOC
                  END IF
*
*                 Unpack solution vector(s)
*
                  K = 1
                  II = MB*NB + 1
                  DO 180 JJ = 0, NB - 1
                     CALL DCOPY( MB, RHS( K ), 1, C( IS, JS+JJ ), 1 )
                     CALL DCOPY( MB, RHS( II ), 1, F( IS, JS+JJ ), 1 )
                     K = K + MB
                     II = II + MB
  180             CONTINUE
*
*                 Substitute R(I, J) and L(I, J) into remaining
*                 equation.
*
                  IF( J.GT.P+2 ) THEN
                     CALL DGEMM( 'N', 'T', MB, JS-1, NB, ONE,
     $                           C( IS, JS ), LDC, B( 1, JS ), LDB, ONE,
     $                           F( IS, 1 ), LDF )
                     CALL DGEMM( 'N', 'T', MB, JS-1, NB, ONE,
     $                           F( IS, JS ), LDF, E( 1, JS ), LDE, ONE,
     $                           F( IS, 1 ), LDF )
                  END IF
                  IF( I.LT.P ) THEN
                     CALL DGEMM( 'T', 'N', M-IE, NB, MB, -ONE,
     $                           A( IS, IE+1 ), LDA, C( IS, JS ), LDC,
     $                           ONE, C( IE+1, JS ), LDC )
                     CALL DGEMM( 'T', 'N', M-IE, NB, MB, -ONE,
     $                           D( IS, IE+1 ), LDD, F( IS, JS ), LDF,
     $                           ONE, C( IE+1, JS ), LDC )
                  END IF
*
               END IF
*
  190       CONTINUE
  200    CONTINUE
*
      END IF
      RETURN
*
*     End of DTGSY2
*
      END
      SUBROUTINE DTGSYL( TRANS, IJOB, M, N, A, LDA, B, LDB, C, LDC, D,
     $                   LDD, E, LDE, F, LDF, SCALE, DIF, WORK, LWORK,
     $                   IWORK, INFO )
*
*  -- LAPACK routine (version 2.1) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 15, 1996
*
*     .. Scalar Arguments ..
      CHARACTER          TRANS
      INTEGER            IJOB, INFO, LDA, LDB, LDC, LDD, LDE, LDF,
     $                   LWORK, M, N
      DOUBLE PRECISION   DIF, SCALE
*     ..
*     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), B( LDB, * ), C( LDC, * ),
     $                   D( LDD, * ), E( LDE, * ), F( LDF, * ),
     $                   WORK( * )
*     ..
*
*  Purpose
*  =======
*
*  DTGSYL solves the generalized Sylvester equation:
*
*              A * R - L * B = scale * C                 (1)
*              D * R - L * E = scale * F
*
*  where R and L are unknown m-by-n matrices, (A, D), (B, E) and
*  (C, F) are given matrix pairs of size m-by-m, n-by-n and m-by-n,
*  respectively, with real entries. (A, D) and (B, E) must be in
*  generalized (real) Schur canonical form, i.e. A, B are upper quasi
*  triangular and D, E are upper triangular.
*
*  The solution (R, L) overwrites (C, F). 0 <= SCALE <= 1 is an output
*  scaling factor chosen to avoid overflow.
*
*  In matrix notation (1) is equivalent to solve  Zx = scale b, where
*  Z is defined as
*
*             Z = [ kron(In, A)  -kron(B', Im) ]         (2)
*                 [ kron(In, D)  -kron(E', Im) ].
*
*  Here Ik is the identity matrix of size k and X' is the transpose of
*  X. kron(X, Y) is the Kronecker product between the matrices X and Y.
*
*  If TRANS = 'T', DTGSYL solves the transposed system Z'*y = scale*b,
*  which is equivalent to solve for R and L in
*
*              A' * R  + D' * L   = scale *  C           (3)
*              R  * B' + L  * E'  = scale * (-F)
*
*  This case (TRANS = 'T') is used to compute an one-norm-based estimate
*  of Dif[(A,D), (B,E)], the separation between the matrix pairs (A,D)
*  and (B,E), using DLACON.
*
*  If IJOB >= 1, DTGSYL computes a Frobenius norm-based estimate
*  of Dif[(A,D),(B,E)]. That is, the reciprocal of a lower bound on the
*  reciprocal of the smallest singular value of Z. See [1-2] for more
*  information.
*
*  This is a level 3 BLAS algorithm.
*
*  Arguments
*  =========
*
*  TRANS   (input) CHARACTER*1
*          = 'N', solve the generalized Sylvester equation (1).
*          = 'T', solve the 'transposed' system (3).
*
*  IJOB    (input) INTEGER
*          Specifies what kind of functionality to be performed.
*           =0: solve (1) only.
*           =1: The functionality of 0 and 3.
*           =2: The functionality of 0 and 4.
*           =3: Only an estimate of Dif[(A,D), (B,E)] is computed.
*               (look ahead strategy IJOB  = 1 is used).
*           =4: Only an estimate of Dif[(A,D), (B,E)] is computed.
*               ( DGECON on sub-systems is used ).
*          Not referenced if TRANS = 'T'.
*
*  M       (input) INTEGER
*          The order of the matrices A and D, and the row dimension of
*          the matrices C, F, R and L.
*
*  N       (input) INTEGER
*          The order of the matrices B and E, and the column dimension
*          of the matrices C, F, R and L.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA, M)
*          The upper quasi triangular matrix A.
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1, M).
*
*  B       (input) DOUBLE PRECISION array, dimension (LDB, N)
*          The upper quasi triangular matrix B.
*
*  LDB     (input) INTEGER
*          The leading dimension of the array B. LDB >= max(1, N).
*
*  C       (input/output) DOUBLE PRECISION array, dimension (LDC, N)
*          On entry, C contains the right-hand-side of the first matrix
*          equation in (1) or (3).
*          On exit, if IJOB = 0, 1 or 2, C has been overwritten by
*          the solution R. If IJOB = 3 or 4 and TRANS = 'N', C holds R,
*          the solution achieved during the computation of the
*          Dif-estimate.
*
*  LDC     (input) INTEGER
*          The leading dimension of the array C. LDC >= max(1, M).
*
*  D       (input) DOUBLE PRECISION array, dimension (LDD, M)
*          The upper triangular matrix D.
*
*  LDD     (input) INTEGER
*          The leading dimension of the array D. LDD >= max(1, M).
*
*  E       (input) DOUBLE PRECISION array, dimension (LDE, N)
*          The upper triangular matrix E.
*
*  LDE     (input) INTEGER
*          The leading dimension of the array E. LDE >= max(1, N).
*
*  F       (input/output) DOUBLE PRECISION array, dimension (LDF, N)
*          On entry, F contains the right-hand-side of the second matrix
*          equation in (1) or (3).
*          On exit, if IJOB = 0, 1 or 2, F has been overwritten by
*          the solution L. If IJOB = 3 or 4 and TRANS = 'N', F holds L,
*          the solution achieved during the computation of the
*          Dif-estimate.
*
*  LDF     (input) INTEGER
*          The leading dimension of the array F. LDF >= max(1, M).
*
*  DIF     (output) DOUBLE PRECISION
*          On exit DIF is the reciprocal of a lower bound of the
*          reciprocal of the Dif-function, i.e. DIF is an upper bound of
*          Dif[(A,D), (B,E)] = sigma_min(Z), where Z as in (2).
*          IF IJOB = 0 or TRANS = 'T', DIF is not touched.
*
*  SCALE   (output) DOUBLE PRECISION
*          On exit SCALE is the scaling factor in (1) or (3).
*          If 0 < SCALE < 1, C and F hold the solutions R and L, resp.,
*          to a slightly perturbed system but the input matrices A, B, D
*          and E have not been changed. If SCALE = 0, C and F hold the
*          solutions R and L, respectively, to the homogeneous system
*          with C = F = 0. Normally, SCALE = 1.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          If IJOB = 0, WORK is not referenced.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK. LWORK > = 1.
*          If IJOB = 1 or 2 and TRANS = 'N', LWORK >= 2*M*N.
*
*  IWORK   (workspace) INTEGER array, dimension (M+N+6)
*
*  INFO    (output) INTEGER
*            =0: successful exit
*            <0: If INFO = -i, the i-th argument had an illegal value.
*            >0: (A, D) and (B, E) have common or close eigenvalues.
*
*  Further Details
*  ===============
*
*  Based on contributions by
*     Bo Kagstrom and Peter Poromaa, Department of Computing Science,
*     Umea University, S-901 87 Umea, Sweden.
*
*  [1] B. Kagstrom and P. Poromaa, LAPACK-Style Algorithms and Software
*      for Solving the Generalized Sylvester Equation and Estimating the
*      Separation between Regular Matrix Pairs, Report UMINF - 93.23,
*      Department of Computing Science, Umea University, S-901 87 Umea,
*      Sweden, December 1993, Revised April 1994, Also as LAPACK Working
*      Note 75.  To appear in ACM Trans. on Math. Software, Vol 22,
*      No 1, 1996.
*
*  [2] B. Kagstrom, A Perturbation Analysis of the Generalized Sylvester
*      Equation (AR - LB, DR - LE ) = (C, F), SIAM J. Matrix Anal.
*      Appl., 15(4):1045-1060, 1994
*
*  [3] B. Kagstrom and L. Westin, Generalized Schur Methods with
*      Condition Estimators for Solving the Generalized Sylvester
*      Equation, IEEE Transactions on Automatic Control, Vol. 34, No. 7,
*      July 1989, pp 745-751.
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            NOTRAN
      INTEGER            I, IE, IFUNC, IROUND, IS, ISOLVE, J, JE, JS, K,
     $                   LINFO, MB, NB, P, PPQQ, PQ, Q
      DOUBLE PRECISION   DSCALE, DSUM, SCALE2, SCALOC
*     ..
*     .. External Functions ..
      LOGICAL            LSAME
      INTEGER            ILAENV
      EXTERNAL           LSAME, ILAENV
*     ..
*     .. External Subroutines ..
      EXTERNAL           DCOPY, DGEMM, DLACPY, DSCAL, DTGSY2, XERBLA
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          DBLE, MAX, SQRT
*     ..
*     .. Executable Statements ..
*
*     Decode and test input parameters
*
      INFO = 0
      NOTRAN = LSAME( TRANS, 'N' )
      IF( .NOT.NOTRAN .AND. .NOT.LSAME( TRANS, 'T' ) ) THEN
         INFO = -1
      ELSE IF( ( IJOB.LT.0 ) .OR. ( IJOB.GT.4 ) ) THEN
         INFO = -2
      ELSE IF( M.LE.0 ) THEN
         INFO = -3
      ELSE IF( N.LE.0 ) THEN
         INFO = -4
      ELSE IF( LDA.LT.MAX( 1, M ) ) THEN
         INFO = -6
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -8
      ELSE IF( LDC.LT.MAX( 1, M ) ) THEN
         INFO = -10
      ELSE IF( LDD.LT.MAX( 1, M ) ) THEN
         INFO = -12
      ELSE IF( LDE.LT.MAX( 1, N ) ) THEN
         INFO = -14
      ELSE IF( LDF.LT.MAX( 1, M ) ) THEN
         INFO = -16
      ELSE IF( IJOB.EQ.1 .AND. NOTRAN .AND. LWORK.LT.2*M*N ) THEN
         INFO = -20
      ELSE IF( IJOB.EQ.2 .AND. NOTRAN .AND. LWORK.LT.2*M*N ) THEN
         INFO = -20
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'DTGSYL', -INFO )
         RETURN
      END IF
*
*     Determine optimal block sizes MB and NB
*
      MB = ILAENV( 2, 'DTGSYL', TRANS, M, N, -1, -1 )
      NB = ILAENV( 5, 'DTGSYL', TRANS, M, N, -1, -1 )
*
      ISOLVE = 1
      IFUNC = 0
      IF( IJOB.GE.3 .AND. NOTRAN ) THEN
         IFUNC = IJOB - 2
         DO 10 J = 1, N
            CALL DCOPY( M, ZERO, 0, C( 1, J ), 1 )
            CALL DCOPY( M, ZERO, 0, F( 1, J ), 1 )
   10    CONTINUE
      ELSE IF( IJOB.GE.1 .AND. NOTRAN ) THEN
         ISOLVE = 2
      END IF
*
      IF( ( MB.LE.1 .AND. NB.LE.1 ) .OR. ( MB.GE.M .AND. NB.GE.N ) )
     $     THEN
*
         DO 30 IROUND = 1, ISOLVE
*
*           Use unblocked Level 2 solver
*
            DSCALE = ZERO
            DSUM = ONE
            PQ = 0
            CALL DTGSY2( TRANS, IFUNC, M, N, A, LDA, B, LDB, C, LDC, D,
     $                   LDD, E, LDE, F, LDF, SCALE, DSUM, DSCALE,
     $                   IWORK, PQ, INFO )
            IF( DSCALE.NE.ZERO ) THEN
               IF( IJOB.EQ.1 .OR. IJOB.EQ.3 ) THEN
                  DIF = SQRT( DBLE( 2*M*N ) ) / ( DSCALE*SQRT( DSUM ) )
               ELSE
                  DIF = SQRT( DBLE( PQ ) ) / ( DSCALE*SQRT( DSUM ) )
               END IF
            END IF
*
            IF( ISOLVE.EQ.2 .AND. IROUND.EQ.1 ) THEN
               IFUNC = IJOB
               SCALE2 = SCALE
               CALL DLACPY( 'F', M, N, C, LDC, WORK, M )
               CALL DLACPY( 'F', M, N, F, LDF, WORK( M*N+1 ), M )
               DO 20 J = 1, N
                  CALL DCOPY( M, ZERO, 0, C( 1, J ), 1 )
                  CALL DCOPY( M, ZERO, 0, F( 1, J ), 1 )
   20          CONTINUE
            ELSE IF( ISOLVE.EQ.2 .AND. IROUND.EQ.2 ) THEN
               CALL DLACPY( 'F', M, N, WORK, M, C, LDC )
               CALL DLACPY( 'F', M, N, WORK( M*N+1 ), M, F, LDF )
               SCALE = SCALE2
            END IF
   30    CONTINUE
*
         RETURN
      END IF
*
*     Determine block structure of A
*
      P = 0
      I = 1
   40 CONTINUE
      IF( I.GT.M )
     $   GO TO 50
      P = P + 1
      IWORK( P ) = I
      I = I + MB
      IF( I.GE.M )
     $   GO TO 50
      IF( A( I, I-1 ).NE.ZERO )
     $   I = I + 1
      GO TO 40
   50 CONTINUE
*
      IWORK( P+1 ) = M + 1
      IF( IWORK( P ).EQ.IWORK( P+1 ) )
     $   P = P - 1
*
*     Determine block structure of B
*
      Q = P + 1
      J = 1
   60 CONTINUE
      IF( J.GT.N )
     $   GO TO 70
      Q = Q + 1
      IWORK( Q ) = J
      J = J + NB
      IF( J.GE.N )
     $   GO TO 70
      IF( B( J, J-1 ).NE.ZERO )
     $   J = J + 1
      GO TO 60
   70 CONTINUE
*
      IWORK( Q+1 ) = N + 1
      IF( IWORK( Q ).EQ.IWORK( Q+1 ) )
     $   Q = Q - 1
*
      IF( NOTRAN ) THEN
*
         DO 150 IROUND = 1, ISOLVE
*
*           Solve (I, J)-subsystem
*               A(I, I) * R(I, J) - L(I, J) * B(J, J) = C(I, J)
*               D(I, I) * R(I, J) - L(I, J) * E(J, J) = F(I, J)
*           for I = P, P - 1,..., 1; J = 1, 2,..., Q
*
            DSCALE = ZERO
            DSUM = ONE
            PQ = 0
            SCALE = ONE
            DO 130 J = P + 2, Q
               JS = IWORK( J )
               JE = IWORK( J+1 ) - 1
               NB = JE - JS + 1
               DO 120 I = P, 1, -1
                  IS = IWORK( I )
                  IE = IWORK( I+1 ) - 1
                  MB = IE - IS + 1
                  PPQQ = 0
                  CALL DTGSY2( TRANS, IFUNC, MB, NB, A( IS, IS ), LDA,
     $                         B( JS, JS ), LDB, C( IS, JS ), LDC,
     $                         D( IS, IS ), LDD, E( JS, JS ), LDE,
     $                         F( IS, JS ), LDF, SCALOC, DSUM, DSCALE,
     $                         IWORK( Q+2 ), PPQQ, LINFO )
                  IF( LINFO.GT.0 )
     $               INFO = LINFO
*
                  PQ = PQ + PPQQ
                  IF( SCALOC.NE.ONE ) THEN
                     DO 80 K = 1, JS - 1
                        CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                        CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
   80                CONTINUE
                     DO 90 K = JS, JE
                        CALL DSCAL( IS-1, SCALOC, C( 1, K ), 1 )
                        CALL DSCAL( IS-1, SCALOC, F( 1, K ), 1 )
   90                CONTINUE
                     DO 100 K = JS, JE
                        CALL DSCAL( M-IE, SCALOC, C( IE+1, K ), 1 )
                        CALL DSCAL( M-IE, SCALOC, F( IE+1, K ), 1 )
  100                CONTINUE
                     DO 110 K = JE + 1, N
                        CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                        CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
  110                CONTINUE
                     SCALE = SCALE*SCALOC
                  END IF
*
*                 Substitute R(I, J) and L(I, J) into remaining
*                 equation.
*
                  IF( I.GT.1 ) THEN
                     CALL DGEMM( 'N', 'N', IS-1, NB, MB, -ONE,
     $                           A( 1, IS ), LDA, C( IS, JS ), LDC, ONE,
     $                           C( 1, JS ), LDC )
                     CALL DGEMM( 'N', 'N', IS-1, NB, MB, -ONE,
     $                           D( 1, IS ), LDD, C( IS, JS ), LDC, ONE,
     $                           F( 1, JS ), LDF )
                  END IF
                  IF( J.LT.Q ) THEN
                     CALL DGEMM( 'N', 'N', MB, N-JE, NB, ONE,
     $                           F( IS, JS ), LDF, B( JS, JE+1 ), LDB,
     $                           ONE, C( IS, JE+1 ), LDC )
                     CALL DGEMM( 'N', 'N', MB, N-JE, NB, ONE,
     $                           F( IS, JS ), LDF, E( JS, JE+1 ), LDE,
     $                           ONE, F( IS, JE+1 ), LDF )
                  END IF
  120          CONTINUE
  130       CONTINUE
            IF( DSCALE.NE.ZERO ) THEN
               IF( IJOB.EQ.1 .OR. IJOB.EQ.3 ) THEN
                  DIF = SQRT( DBLE( 2*M*N ) ) / ( DSCALE*SQRT( DSUM ) )
               ELSE
                  DIF = SQRT( DBLE( PQ ) ) / ( DSCALE*SQRT( DSUM ) )
               END IF
            END IF
            IF( ISOLVE.EQ.2 .AND. IROUND.EQ.1 ) THEN
               IFUNC = IJOB
               SCALE2 = SCALE
               CALL DLACPY( 'F', M, N, C, LDC, WORK, M )
               CALL DLACPY( 'F', M, N, F, LDF, WORK( M*N+1 ), M )
               DO 140 J = 1, N
                  CALL DCOPY( M, ZERO, 0, C( 1, J ), 1 )
                  CALL DCOPY( M, ZERO, 0, F( 1, J ), 1 )
  140          CONTINUE
            ELSE IF( ISOLVE.EQ.2 .AND. IROUND.EQ.2 ) THEN
               CALL DLACPY( 'F', M, N, WORK, M, C, LDC )
               CALL DLACPY( 'F', M, N, WORK( M*N+1 ), M, F, LDF )
               SCALE = SCALE2
            END IF
  150    CONTINUE
*
      ELSE
*
*        Solve transposed (I, J)-subsystem
*             A(I, I)' * R(I, J)  + D(I, I)' * L(I, J)  =  C(I, J)
*             R(I, J)  * B(J, J)' + L(I, J)  * E(J, J)' = -F(I, J)
*        for I = 1,2,..., P; J = Q, Q-1,..., 1
*
         SCALE = ONE
         DO 210 I = 1, P
            IS = IWORK( I )
            IE = IWORK( I+1 ) - 1
            MB = IE - IS + 1
            DO 200 J = Q, P + 2, -1
               JS = IWORK( J )
               JE = IWORK( J+1 ) - 1
               NB = JE - JS + 1
               CALL DTGSY2( TRANS, IFUNC, MB, NB, A( IS, IS ), LDA,
     $                      B( JS, JS ), LDB, C( IS, JS ), LDC,
     $                      D( IS, IS ), LDD, E( JS, JS ), LDE,
     $                      F( IS, JS ), LDF, SCALOC, DSUM, DSCALE,
     $                      IWORK( Q+2 ), PPQQ, LINFO )
               IF( LINFO.GT.0 )
     $            INFO = LINFO
               IF( SCALOC.NE.ONE ) THEN
                  DO 160 K = 1, JS - 1
                     CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                     CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
  160             CONTINUE
                  DO 170 K = JS, JE
                     CALL DSCAL( IS-1, SCALOC, C( 1, K ), 1 )
                     CALL DSCAL( IS-1, SCALOC, F( 1, K ), 1 )
  170             CONTINUE
                  DO 180 K = JS, JE
                     CALL DSCAL( M-IE, SCALOC, C( IE+1, K ), 1 )
                     CALL DSCAL( M-IE, SCALOC, F( IE+1, K ), 1 )
  180             CONTINUE
                  DO 190 K = JE + 1, N
                     CALL DSCAL( M, SCALOC, C( 1, K ), 1 )
                     CALL DSCAL( M, SCALOC, F( 1, K ), 1 )
  190             CONTINUE
                  SCALE = SCALE*SCALOC
               END IF
*
*              Substitute R(I, J) and L(I, J) into remaining equation.
*
               IF( J.GT.P+2 ) THEN
                  CALL DGEMM( 'N', 'T', MB, JS-1, NB, ONE, C( IS, JS ),
     $                        LDC, B( 1, JS ), LDB, ONE, F( IS, 1 ),
     $                        LDF )
                  CALL DGEMM( 'N', 'T', MB, JS-1, NB, ONE, F( IS, JS ),
     $                        LDF, E( 1, JS ), LDE, ONE, F( IS, 1 ),
     $                        LDF )
               END IF
               IF( I.LT.P ) THEN
                  CALL DGEMM( 'T', 'N', M-IE, NB, MB, -ONE,
     $                        A( IS, IE+1 ), LDA, C( IS, JS ), LDC, ONE,
     $                        C( IE+1, JS ), LDC )
                  CALL DGEMM( 'T', 'N', M-IE, NB, MB, -ONE,
     $                        D( IS, IE+1 ), LDD, F( IS, JS ), LDF, ONE,
     $                        C( IE+1, JS ), LDC )
               END IF
  200       CONTINUE
  210    CONTINUE
*
      END IF
*
      RETURN
*
*     End of DTGSYL
*
      END 
      SUBROUTINE DGESC2( N, A, LDA, RHS, IPIV, JPIV, SCALE )
*
*  -- LAPACK auxiliary routine (version 2.1) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 15, 1996
*
*     .. Scalar Arguments ..
      INTEGER            LDA, N
      DOUBLE PRECISION   SCALE
*     ..
*     .. Array Arguments ..
      INTEGER            IPIV( * ), JPIV( * )
      DOUBLE PRECISION   A( LDA, * ), RHS( * )
*     ..
*
*  Purpose
*  =======
*
*  DGESC2 solves a system of linear equations
*
*            A * X = scale* RHS
*
*  with a general N-by-N matrix A using the LU factorization with
*  complete pivoting computed by DGETC2.
*
*  Arguments
*  =========
*
*  N       (input) INTEGER
*          The order of the matrix A.
*
*  A       (input) DOUBLE PRECISION array, dimension (LDA,N)
*          On entry, the  LU part of the factorization of the n-by-n
*          matrix A computed by DGETC2:  A = P * L * U * Q
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A.  LDA >= max(1, N).
*
*  RHS     (input/output) DOUBLE PRECISION array, dimension (N).
*          On entry, the right hand side vector b.
*          On exit, the solution vector X.
*
*  IPIV    (iput) INTEGER array, dimension (N).
*          The pivot indices; for 1 <= i <= N, row i of the
*          matrix has been interchanged with row IPIV(i).
*
*  JPIV    (iput) INTEGER array, dimension (N).
*          The pivot indices; for 1 <= j <= N, column j of the
*          matrix has been interchanged with column JPIV(j).
*
*  SCALE    (output) DOUBLE PRECISION
*           On exit, SCALE contains the scale factor. SCALE is chosen
*           0 <= SCALE <= 1 to prevent owerflow in the solution.
*
*  Further Details
*  ===============
*
*  Based on contributions by
*     Bo Kagstrom and Peter Poromaa, Department of Computing Science,
*     Umea University, S-901 87 Umea, Sweden.
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ONE, TWO
      PARAMETER          ( ONE = 1.0D+0, TWO = 2.0D+0 )
*     ..
*     .. Local Scalars ..
      INTEGER            I, J
      DOUBLE PRECISION   BIGNUM, EPS, SMLNUM, TEMP
*     ..
*     .. External Subroutines ..
      EXTERNAL           DLABAD, DLASWP, DSCAL
*     ..
*     .. External Functions ..
      INTEGER            IDAMAX
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           IDAMAX, DLAMCH
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS
*     ..
*     .. Executable Statements ..
*
*      Set constant to control owerflow
*
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' ) / EPS
      BIGNUM = ONE / SMLNUM
      CALL DLABAD( SMLNUM, BIGNUM )
*
*     Apply permutations IPIV to RHS
*
      CALL DLASWP( 1, RHS, LDA, 1, N-1, IPIV, 1 )
*
*     Solve for L part
*
      DO 20 I = 1, N - 1
         DO 10 J = I + 1, N
            RHS( J ) = RHS( J ) - A( J, I )*RHS( I )
   10    CONTINUE
   20 CONTINUE
*
*     Solve for U part
*
      SCALE = ONE
*
*     Check for scaling
*
      I = IDAMAX( N, RHS, 1 )
      IF( TWO*SMLNUM*ABS( RHS( I ) ).GT.ABS( A( N, N ) ) ) THEN
         TEMP = ( ONE / TWO ) / ABS( RHS( I ) )
         CALL DSCAL( N, TEMP, RHS( 1 ), 1 )
         SCALE = SCALE*TEMP
      END IF
*
      DO 40 I = N, 1, -1
         TEMP = ONE / A( I, I )
         RHS( I ) = RHS( I )*TEMP
         DO 30 J = I + 1, N
            RHS( I ) = RHS( I ) - RHS( J )*( A( I, J )*TEMP )
   30    CONTINUE
   40 CONTINUE
*
*     Apply permutations JPIV to the solution (RHS)
*
      CALL DLASWP( 1, RHS, LDA, 1, N-1, JPIV, -1 )
      RETURN
*
*     End of DGESC2
*
      END
      SUBROUTINE DGETC2( N, A, LDA, IPIV, JPIV, INFO )
*
*  -- LAPACK auxiliary routine (version 2.1) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 15, 1996
*
*     .. Scalar Arguments ..
      INTEGER            INFO, LDA, N
*     ..
*     .. Array Arguments ..
      INTEGER            IPIV( * ), JPIV( * )
      DOUBLE PRECISION   A( LDA, * )
*     ..
*
*  Purpose
*  =======
*
*  DGETC2 computes an LU factorization with complete pivoting of the
*  n-by-n matrix A. The factorization has the form A = P * L * U * Q,
*  where P and Q are permutation matrices, L is lower triangular with
*  unit diagonal elements and U is upper triangular.
*
*  This is the Level 2 BLAS algorithm.
*
*  Arguments
*  =========
*
*  N       (input) INTEGER
*          The order of the matrix A. N >= 0.
*
*  A       (input/output) DOUBLE PRECISION array, dimension (LDA, N)
*          On entry, the n-by-n matrix A to be factored.
*          On exit, the factors L and U from the factorization
*          A = P*L*U*Q; the unit diagonal elements of L are not stored.
*          If U(k, k) appears to be less than SMIN, U(k, k) is given the
*          value of SMIN, i.e., giving a nonsingular perturbed system.
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A.  LDA >= max(1,N).
*
*  IPIV    (output) INTEGER array, dimension(N).
*          The pivot indices; for 1 <= i <= N, row i of the
*          matrix has been interchanged with row IPIV(i).
*
*  JPIV    (output) INTEGER array, dimension(N).
*          The pivot indices; for 1 <= j <= N, column j of the
*          matrix has been interchanged with column JPIV(j).
*
*  INFO    (output) INTEGER
*           = 0: successful exit
*           > 0: if INFO = k, U(k, k) is likely to produce owerflow if
*                we try to solve for x in Ax = b. So U is perturbed to
*                avoid the overflow.
*
*  Further Details
*  ===============
*
*  Based on contributions by
*     Bo Kagstrom and Peter Poromaa, Department of Computing Science,
*     Umea University, S-901 87 Umea, Sweden.
*
*  =====================================================================
*
*     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      INTEGER            I, IP, IPV, J, JP, JPV
      DOUBLE PRECISION   BIGNUM, EPS, SMIN, SMLNUM, XMAX
*     ..
*     .. External Subroutines ..
      EXTERNAL           DGER, DLABAD, DSWAP
*     ..
*     .. External Functions ..
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          ABS, MAX
*     ..
*     .. Executable Statements ..
*
*     Set constants to control overflow
*
      INFO = 0
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' ) / EPS
      BIGNUM = ONE / SMLNUM
      CALL DLABAD( SMLNUM, BIGNUM )
*
*     Factorize A using complete pivoting.
*     Set pivots less than SMIN to SMIN.
*
      DO 40 I = 1, N - 1
*
*        Find max element in matrix A
*
         XMAX = ZERO
         DO 20 IP = I, N
            DO 10 JP = I, N
               IF( ABS( A( IP, JP ) ).GE.XMAX ) THEN
                  XMAX = ABS( A( IP, JP ) )
                  IPV = IP
                  JPV = JP
               END IF
   10       CONTINUE
   20    CONTINUE
         IF( I.EQ.1 )
     $      SMIN = MAX( EPS*XMAX, SMLNUM )
*
*        Swap rows
*
         IF( IPV.NE.I )
     $      CALL DSWAP( N, A( IPV, 1 ), LDA, A( I, 1 ), LDA )
         IPIV( I ) = IPV
*
*        Swap columns
*
         IF( JPV.NE.I )
     $      CALL DSWAP( N, A( 1, JPV ), 1, A( 1, I ), 1 )
         JPIV( I ) = JPV
*
*        Check for singularity
*
         IF( ABS( A( I, I ) ).LT.SMIN ) THEN
            INFO = I
            A( I, I ) = SMIN
         END IF
         DO 30 J = I + 1, N
            A( J, I ) = A( J, I ) / A( I, I )
   30    CONTINUE
         CALL DGER( N-I, N-I, -ONE, A( I+1, I ), 1, A( I, I+1 ), LDA,
     $              A( I+1, I+1 ), LDA )
   40 CONTINUE
*
      IF( ABS( A( N, N ) ).LT.SMIN ) THEN
         INFO = N
         A( N, N ) = SMIN
      END IF
*
      RETURN
*
*     End of DGETC2
*
      END

      SUBROUTINE DTGSEN( IJOB, WANTQ, WANTZ, SELECT, N, A, LDA, B, LDB,
     $                   ALPHAR, ALPHAI, BETA, Q, LDQ, Z, LDZ, M, PL,
     $                   PR, DIF, WORK, LWORK, IWORK, LIWORK, INFO )
*
*  -- LAPACK routine (version 2.1) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     June 15, 1996
*
*     .. Scalar Arguments ..
      LOGICAL            WANTQ, WANTZ
      INTEGER            IJOB, INFO, LDA, LDB, LDQ, LDZ, LIWORK, LWORK,
     $                   M, N
      DOUBLE PRECISION   PL, PR
*     ..
*     .. Array Arguments ..
      LOGICAL            SELECT( * )
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), ALPHAI( * ), ALPHAR( * ),
     $                   B( LDB, * ), BETA( * ), DIF( * ), Q( LDQ, * ),
     $                   WORK( * ), Z( LDZ, * )
*     ..
*
*  Purpose
*  =======
*
*  DTGSEN reorders the generalized real Schur decomposition of a real
*  matrix pair (A, B) (in terms of an orthonormal equivalence trans-
*  formation Q' * (A, B) * Z), so that a selected cluster of eigenvalues
*  appears in the leading diagonal blocks of the upper quasi-triangular
*  matrix A and the upper triangular B. The leading columns of Q and
*  Z form orthonormal bases of the corresponding left and right eigen-
*  spaces (deflating subspaces). (A, B) must be in generalized real
*  Schur canonical form (as returned by DGGES), i.e. A is block upper
*  triangular with 1-by-1 and 2-by-2 diagonal blocks. B is upper
*  triangular.
*
*  DTGSEN also computes the generalized eigenvalues
*
*              w(j) = (ALPHAR(j) + i*ALPHAI(j))/BETA(j)
*
*  of the reordered matrix pair (A, B).
*
*  Optionally, DTGSEN computes the estimates of reciprocal condition
*  numbers for eigenvalues and eigenspaces. These are Difu[(A11,B11),
*  (A22,B22)] and Difl[(A11,B11), (A22,B22)], i.e. the separation(s)
*  between the matrix pairs (A11, B11) and (A22,B22) that correspond to
*  the selected cluster and the eigenvalues outside the cluster, resp.,
*  and norms of "projections" onto left and right eigenspaces w.r.t.
*  the selected cluster in the (1,1)-block.
*
*  Arguments
*  =========
*
*  IJOB    (input) INTEGER
*          Specifies whether condition numbers are required for the
*          cluster of eigenvalues (PL and PR) or the deflating subspaces
*          (Difu and Difl):
*           =0: Only reorder w.r.t. SELECT. No extras.
*           =1: Reciprocal of norms of "projections" onto left and right
*               eigenspaces w.r.t. the selected cluster (PL and PR).
*           =2: Upper bounds on Difu and Difl. F-norm-based estimate
*               (DIF(1:2)).
*           =3: Estimate of Difu and Difl. 1-norm-based estimate
*               (DIF(1:2)).
*               About 5 times as expensive as IJOB = 2.
*           =4: Compute PL, PR and DIF (i.e. 0, 1 and 2 above): Economic
*               version to get it all.
*           =5: Compute PL, PR and DIF (i.e. 0, 1 and 3 above)
*
*  WANTQ   (input) LOGICAL
*          .TRUE. : update the left transformation matrix Q;
*          .FALSE.: do not update Q.
*
*  WANTZ   (input) LOGICAL
*          .TRUE. : update the right transformation matrix Z;
*          .FALSE.: do not update Z.
*
*  SELECT  (input) LOGICAL array, dimension (N)
*          SELECT specifies the eigenvalues in the selected cluster.
*          To select a real eigenvalue w(j), SELECT(j) must be set to
*          .TRUE.. To select a complex conjugate pair of eigenvalues
*          w(j) and w(j+1), corresponding to a 2-by-2 diagonal block,
*          either SELECT(j) or SELECT(j+1) or both must be set to
*          .TRUE.; a complex conjugate pair of eigenvalues must be
*          either both included in the cluster or both excluded.
*
*  N       (input) INTEGER
*          The order of the matrices A and B. N >= 0.
*
*  A       (input/output) DOUBLE PRECISION array, dimension(LDA,N)
*          On entry, the upper quasi-triangular matrix A, with (A, B) in
*          generalized real Schur canonical form.
*          On exit, A is overwritten by the reordered matrix A.
*
*  LDA     (input) INTEGER
*          The leading dimension of the array A. LDA >= max(1,N).
*
*  B       (input/output) DOUBLE PRECISION array, dimension(LDB,N)
*          On entry, the upper triangular matrix B, with (A, B) in
*          generalized real Schur canonical form.
*          On exit, B is overwritten by the reordered matrix B.
*
*  LDB     (input) INTEGER
*          The leading dimension of the array B. LDB >= max(1,N).
*
*  ALPHAR  (output) DOUBLE PRECISION array, dimension (N)
*  ALPHAI  (output) DOUBLE PRECISION array, dimension (N)
*  BETA    (output) DOUBLE PRECISION array, dimension (N)
*          On exit, (ALPHAR(j) + ALPHAI(j)*i)/BETA(j), j=1,...,N, will
*          be the generalized eigenvalues.  ALPHAR(j) + ALPHAI(j)*i
*          and BETA(j),j=1,...,N  are the diagonals of the complex Schur
*          form (S,T) that would result if the 2-by-2 diagonal blocks of
*          the real generalized Schur form of (A,B) were further reduced
*          to triangular form using complex unitary transformations.
*          If ALPHAI(j) is zero, then the j-th eigenvalue is real; if
*          positive, then the j-th and (j+1)-st eigenvalues are a
*          complex conjugate pair, with ALPHAI(j+1) negative.
*
*  Q       (input/output) DOUBLE PRECISION array, dimension (LDQ,N)
*          On entry, if WANTQ = .TRUE., Q is an N-by-N matrix.
*          On exit, Q has been postmultiplied by the left orthogonal
*          transformation matrix which reorder (A, B); The leading M
*          columns of Q form orthonormal bases for the specified pair of
*          left eigenspaces (deflating subspaces).
*          If WANTQ = .FALSE., Q is not referenced.
*
*  LDQ     (input) INTEGER
*          The leading dimension of the array Q.  LDQ >= 1;
*          and if WANTQ = .TRUE., LDQ >= N.
*
*  Z       (input/output) DOUBLE PRECISION array, dimension (LDZ,N)
*          On entry, if WANTZ = .TRUE., Z is an N-by-N matrix.
*          On exit, Z has been postmultiplied by the left orthogonal
*          transformation matrix which reorder (A, B); The leading M
*          columns of Z form orthonormal bases for the specified pair of
*          left eigenspaces (deflating subspaces).
*          If WANTZ = .FALSE., Z is not referenced.
*
*  LDZ     (input) INTEGER
*          The leading dimension of the array Z. LDZ >= 1;
*          If WANTZ = .TRUE., LDZ >= N.
*
*  M       (output) INTEGER
*          The dimension of the specified pair of left and right eigen-
*          spaces (deflating subspaces). 0 <= M <= N.
*
*  PL, PR  (output) DOUBLE PRECISION
*          If IJOB = 1, 4 or 5, PL, PR are lower bounds on the
*          reciprocal of the norm of "projections" onto left and right
*          eigenspaces with respect to the selected cluster.
*          0 < PL, PR <= 1.
*          If M = 0 or M = N, PL = PR  = 1.
*          If IJOB = 0, 2 or 3, PL and PR are not referenced.
*
*  DIF     (output) DOUBLE PRECISION array, dimension (2).
*          If IJOB >= 2, DIF(1:2) store the estimates of Difu and Difl.
*          If IJOB = 2 or 4, DIF(1:2) are F-norm-based upper bounds on
*          Difu and Difl. If IJOB = 3 or 5, DIF(1:2) are 1-norm-based
*          estimates of Difu and Difl.
*          If M = 0 or N, DIF(1:2) = F-norm([A, B]).
*          If IJOB = 0 or 1, DIF is not referenced.
*
*  WORK    (workspace) DOUBLE PRECISION array, dimension (LWORK)
*          IF IJOB = 0, WORK is not referenced.
*
*  LWORK   (input) INTEGER
*          The dimension of the array WORK. LWORK >=  4*N+16.
*          If IJOB = 1, 2 or 4, LWORK >= MAX(4*N+16, 2*M*(N-M)).
*          If IJOB = 3 or 5, LWORK >= MAX(4*N+16, 4*M*(N-M)).
*
*  IWORK   (workspace) INTEGER array, dimension (LIWORK)
*          IF IJOB = 0, IWORK is not referenced.
*
*  LIWORK  (input) INTEGER
*          The dimension of the array IWORK. LIWORK >= 1.
*          If IJOB = 1, 2 or 4, LIWORK >=  N+6.
*          If IJOB = 3 or 5, LIWORK >= MAX(2*M*(N-M), N+6).
*
*  INFO    (output) INTEGER
*            =0: Successful exit.
*            <0: If INFO = -i, the i-th argument had an illegal value.
*            =1: Reordering of (A, B) failed because the transformed
*                matrix pair (A, B) would be too far from generalized
*                Schur form; the problem is very ill-conditioned.
*                (A, B) may have been partially reordered.
*                If requested, 0 is returned in DIF(*), PL and PR.
*
*  Further Details
*  ===============
*
*  DTGSEN first collects the selected eigenvalues by computing
*  orthogonal U and W that move them to the top left corner of (A, B).
*  In other words, the selected eigenvalues are the eigenvalues of
*  (A11, B11) in:
*
*                U'*(A, B)*W = (A11 A12) (B11 B12) n1
*                              ( 0  A22),( 0  B22) n2
*                                n1  n2    n1  n2
*
*  where N = n1+n2 and U' means the transpose of U. The first n1 columns
*  of U and W span the specified pair of left and right eigenspaces
*  (deflating subspaces) of (A, B).
*
*  If (A, B) has been obtained from the generalized real Schur
*  decomposition of a matrix pair (C, D) = Q*(A, B)*Z', then the
*  reordered generalized real Schur form of (C, D) is given by
*
*           (C, D) = (Q*U)*(U'*(A, B)*W)*(Z*W)',
*
*  and the first n1 columns of Q*U and Z*W span the corresponding
*  deflating subspaces of (C, D) (Q and Z store Q*U and Z*W, resp.).
*
*  Note that if the selected eigenvalue is sufficiently ill-conditioned,
*  then its value may differ significantly from its value before
*  reordering.
*
*  The reciprocal condition numbers of the left and right eigenspaces
*  spanned by the first n1 columns of U and W (or Q*U and Z*W) may
*  be returned in DIF(1:2), corresponding to Difu and Difl, resp.
*
*  The Difu and Difl are defined as:
*
*       Difu[(A11, B11), (A22, B22)] = sigma-min( Zu )
*  and
*       Difl[(A11, B11), (A22, B22)] = Difu[(A22, B22), (A11, B11)],
*
*  where sigma-min(Zu) is the smallest singular value of the
*  (2*n1*n2)-by-(2*n1*n2) matrix
*
*       Zu = [ kron(In2, A11)  -kron(A22', In1) ]
*            [ kron(In2, B11)  -kron(B22', In1) ].
*
*  Here, Inx is the identity matrix of size nx and A22' is the
*  transpose of A22. kron(X, Y) is the Kronecker product between
*  the matrices X and Y.
*
*  When DIF(2) is small, small changes in (A, B) can cause large changes
*  in the deflating subspace. An approximate (asymptotic) bound on the
*  maximum angular error in the computed deflating subspaces is
*
*       EPS * norm((A, B)) / DIF(2),
*
*  where EPS is the machine precision.
*
*  The reciprocal norm of the projectors on the left and right
*  eigenspaces associated with (A11, B11) may be returned in PL and PR.
*  They are computed as follows. First we compute L and R so that
*  P*(A, B)*Q is block diagonal, where
*
*       P = ( I -L ) n1           Q = ( I R ) n1
*           ( 0  I ) n2    and        ( 0 I ) n2
*             n1 n2                    n1 n2
*
*  and (L, R) is the solution to the generalized Sylvester equation
*
*       A11*R - L*A22 = -A12
*       B11*R - L*B22 = -B12
*
*  Then PL = (F-norm(L)**2+1)**(-1/2) and PR = (F-norm(R)**2+1)**(-1/2).
*  An approximate (asymptotic) bound on the average absolute error of
*  the selected eigenvalues is
*
*       EPS * norm((A, B)) / PL.
*
*  There are also global error bounds which valid for perturbations up
*  to a certain restriction:  A lower bound (x) on the smallest
*  F-norm(E,F) for which an eigenvalue of (A11, B11) may move and
*  coalesce with an eigenvalue of (A22, B22) under perturbation (E,F),
*  (i.e. (A + E, B + F), is
*
*   x = min(Difu,Difl)/((1/(PL*PL)+1/(PR*PR))**(1/2)+2*max(1/PL,1/PR)).
*
*  An approximate bound on x can be computed from DIF(1:2), PL and PR.
*
*  If y = ( F-norm(E,F) / x) <= 1, the angles between the perturbed
*  (L', R') and unperturbed (L, R) left and right deflating subspaces
*  associated with the selected cluster in the (1,1)-blocks can be
*  bounded as
*
*   max-angle(L, L') <= arctan( y * PL / (1 - y * (1 - PL * PL)**(1/2))
*   max-angle(R, R') <= arctan( y * PR / (1 - y * (1 - PR * PR)**(1/2))
*
*  See LAPACK User's Guide section 4.11 or the following references
*  for more information.
*
*  Note that if the default method for computing the Frobenius-norm-
*  based estimate DIF is not wanted (see DLATDF), then the parameter
*  IDIFJB (see below) should be changed from 3 to 4 (routine DLATDF
*  (IJOB = 2 will be used)). See DTGSYL for more details.
*
*  Based on contributions by
*     Bo Kagstrom and Peter Poromaa, Department of Computing Science,
*     Umea University, S-901 87 Umea, Sweden.
*
*  References
*  ==========
*
*  [1] B. Kagstrom; A Direct Method for Reordering Eigenvalues in the
*      Generalized Real Schur Form of a Regular Matrix Pair (A, B), in
*      M.S. Moonen et al (eds), Linear Algebra for Large Scale and
*      Real-Time Applications, Kluwer Academic Publ. 1993, pp 195-218.
*
*  [2] B. Kagstrom and P. Poromaa; Computing Eigenspaces with Specified
*      Eigenvalues of a Regular Matrix Pair (A, B) and Condition
*      Estimation: Theory, Algorithms and Software,
*      Report UMINF - 94.04, Department of Computing Science, Umea
*      University, S-901 87 Umea, Sweden, 1994. Also as LAPACK Working
*      Note 87. To appear in Numerical Algorithms, 1996.
*
*  [3] B. Kagstrom and P. Poromaa, LAPACK-Style Algorithms and Software
*      for Solving the Generalized Sylvester Equation and Estimating the
*      Separation between Regular Matrix Pairs, Report UMINF - 93.23,
*      Department of Computing Science, Umea University, S-901 87 Umea,
*      Sweden, December 1993, Revised April 1994, Also as LAPACK Working
*      Note 75. To appear in ACM Trans. on Math. Software, Vol 22, No 1,
*      1996.
*
*  =====================================================================
*
*     .. Parameters ..
      INTEGER            IDIFJB
      PARAMETER          ( IDIFJB = 3 )
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
*     ..
*     .. Local Scalars ..
      LOGICAL            PAIR, SWAP, WANTD, WANTD1, WANTD2, WANTP
      INTEGER            I, IERR, IJB, K, KASE, KK, KS, MN2, N1, N2
      DOUBLE PRECISION   DSCALE, DSUM, EPS, RDSCAL, SMLNUM
*     ..
*     .. External Subroutines ..
      EXTERNAL           DLACON, DLACPY, DLAG2, DLASSQ, DTGEXC, DTGSYL,
     $                   XERBLA
*     ..
*     .. External Functions ..
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC          MAX, SIGN, SQRT
*     ..
*     .. Executable Statements ..
*
*     Decode and test the input parameters
*
*
*     Get machine constants
*
      EPS = DLAMCH( 'P' )
      SMLNUM = DLAMCH( 'S' ) / EPS
      INFO = 0
      IERR = 0
      IF( IJOB.LT.0 .OR. IJOB.GT.5 ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -5
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -9
      ELSE IF( LDQ.LT.1 .OR. ( WANTQ .AND. LDQ.LT.N ) ) THEN
         INFO = -14
      ELSE IF( LDZ.LT.1 .OR. ( WANTZ .AND. LDZ.LT.N ) ) THEN
         INFO = -16
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'DTGSEN', -INFO )
         RETURN
      END IF
*
      WANTP = IJOB.EQ.1 .OR. IJOB.GE.4
      WANTD1 = IJOB.EQ.2 .OR. IJOB.EQ.4
      WANTD2 = IJOB.EQ.3 .OR. IJOB.EQ.5
      WANTD = WANTD1 .OR. WANTD2
*
*     Set M to the dimension of the specified pair of deflating
*     subspaces.
*
      M = 0
      PAIR = .FALSE.
      DO 10 K = 1, N
         IF( PAIR ) THEN
            PAIR = .FALSE.
         ELSE
            IF( K.LT.N ) THEN
               IF( A( K+1, K ).EQ.ZERO ) THEN
                  IF( SELECT( K ) )
     $               M = M + 1
               ELSE
                  PAIR = .TRUE.
                  IF( SELECT( K ) .OR. SELECT( K+1 ) )
     $               M = M + 2
               END IF
            ELSE
               IF( SELECT( N ) )
     $            M = M + 1
            END IF
         END IF
   10 CONTINUE
*
      IF( IJOB.EQ.0 .AND. LWORK.LT.4*N+16 ) THEN
         INFO = -22
*
*  modified: Varga
*  originally:    ELSE IF( .NOT.WANTD2 .AND. 
*
      ELSE IF( .NOT.WANTD2 .AND. IJOB.NE.0 .AND. 
     $   LWORK.LT.MAX( 4*N+16, 2*M*( N-M ) ) )  THEN
         INFO = -22
      ELSE IF( WANTD2 .AND. LWORK.LT.MAX( 4*N+16, 4*M*( N-M ) ) ) THEN
         INFO = -22
      ELSE IF( IJOB.EQ.0 .AND. LIWORK.LT.1 ) THEN
         INFO = -24
      ELSE IF( .NOT.WANTD2 .AND. IJOB.NE.0 .AND. LIWORK.LT.N+6 ) THEN
         INFO = -24
      ELSE IF( WANTD2 .AND. LIWORK.LT.MAX( 2*M*( N-M ), N+6 ) ) THEN
         INFO = -24
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'DTGSEN', -INFO )
         RETURN
      END IF
*
*     Quick return if possible.
*
      IF( M.EQ.N .OR. M.EQ.0 ) THEN
         IF( WANTP ) THEN
            PL = ONE
            PR = ONE
         END IF
         IF( WANTD ) THEN
            DSCALE = ZERO
            DSUM = ONE
            DO 20 I = 1, N
               CALL DLASSQ( N, A( 1, I ), 1, DSCALE, DSUM )
               CALL DLASSQ( N, B( 1, I ), 1, DSCALE, DSUM )
   20       CONTINUE
            DIF( 1 ) = DSCALE*SQRT( DSUM )
            DIF( 2 ) = DIF( 1 )
         END IF
         GO TO 60
      END IF
*
*     Collect the selected blocks at the top-left corner of (A, B).
*
      KS = 0
      PAIR = .FALSE.
      DO 30 K = 1, N
         IF( PAIR ) THEN
            PAIR = .FALSE.
         ELSE
*
            SWAP = SELECT( K )
            IF( K.LT.N ) THEN
               IF( A( K+1, K ).NE.ZERO ) THEN
                  PAIR = .TRUE.
                  SWAP = SWAP .OR. SELECT( K+1 )
               END IF
            END IF
*
            IF( SWAP ) THEN
               KS = KS + 1
*
*              Swap the K-th block to position KS.
*              Perform the reordering of diagonal blocks in (A, B)
*              by orthogonal transformation matrices and update
*              Q and Z accordingly (if requested):
*
               KK = K
               IF( K.NE.KS )
     $            CALL DTGEXC( WANTQ, WANTZ, N, A, LDA, B, LDB, Q, LDQ,
     $                         Z, LDZ, KK, KS, WORK, LWORK, IERR )
*
               IF( IERR.GT.0 ) THEN
*
*                 Swap is rejected: exit.
*
                  INFO = 1
                  IF( WANTP ) THEN
                     PL = ZERO
                     PR = ZERO
                  END IF
                  IF( WANTD ) THEN
                     DIF( 1 ) = ZERO
                     DIF( 2 ) = ZERO
                  END IF
                  GO TO 60
               END IF
*
               IF( PAIR )
     $            KS = KS + 1
            END IF
         END IF
   30 CONTINUE
      IF( WANTP ) THEN
*
*        Solve generalized Sylvester equation for R and L
*        and compute PL and PR.
*
         N1 = M
         N2 = N - M
         I = N1 + 1
         IJB = 0
         CALL DLACPY( 'Full', N1, N2, A( 1, I ), LDA, WORK, N1 )
         CALL DLACPY( 'Full', N1, N2, B( 1, I ), LDB, WORK( N1*N2+1 ),
     $                N1 )
         CALL DTGSYL( 'N', IJB, N1, N2, A, LDA, A( I, I ), LDA, WORK,
     $                N1, B, LDB, B( I, I ), LDB, WORK( N1*N2+1 ), N1,
     $                DSCALE, DIF( 1 ), WORK( N1*N2*2+1 ),
     $                LWORK-2*N1*N2, IWORK, IERR )
*
*        Estimate the reciprocal of norms of "projections" onto left
*        and right eigenspaces.
*
         RDSCAL = ZERO
         DSUM = ONE
         CALL DLASSQ( N1*N2, WORK, 1, RDSCAL, DSUM )
         PL = RDSCAL*SQRT( DSUM )
         IF( PL.EQ.ZERO ) THEN
            PL = ONE
         ELSE
            PL = DSCALE / ( SQRT( DSCALE*DSCALE / PL+PL )*SQRT( PL ) )
         END IF
         RDSCAL = ZERO
         DSUM = ONE
         CALL DLASSQ( N1*N2, WORK( N1*N2+1 ), 1, RDSCAL, DSUM )
         PR = RDSCAL*SQRT( DSUM )
         IF( PR.EQ.ZERO ) THEN
            PR = ONE
         ELSE
            PR = DSCALE / ( SQRT( DSCALE*DSCALE / PR+PR )*SQRT( PR ) )
         END IF
      END IF
*
      IF( WANTD ) THEN
*
*        Compute estimates of Difu and Difl.
*
         IF( WANTD1 ) THEN
            N1 = M
            N2 = N - M
            I = N1 + 1
            IJB = IDIFJB
*
*           Frobenius norm-based Difu-estimate.
*
            CALL DTGSYL( 'N', IJB, N1, N2, A, LDA, A( I, I ), LDA, WORK,
     $                   N1, B, LDB, B( I, I ), LDB, WORK( N1*N2+1 ),
     $                   N1, DSCALE, DIF( 1 ), WORK( 2*N1*N2+1 ),
     $                   LWORK-2*N1*N2, IWORK, IERR )
*
*           Frobenius norm-based Difl-estimate.
*
            CALL DTGSYL( 'N', IJB, N2, N1, A( I, I ), LDA, A, LDA, WORK,
     $                   N2, B( I, I ), LDB, B, LDB, WORK( N1*N2+1 ),
     $                   N2, DSCALE, DIF( 2 ), WORK( 2*N1*N2+1 ),
     $                   LWORK-2*N1*N2, IWORK, IERR )
         ELSE
*
*
*           Compute 1-norm-based estimates of Difu and Difl using
*           reversed communication with DLACON. In each step a
*           generalized Sylvester equation or a transposed variant
*           is solved.
*
            KASE = 0
            N1 = M
            N2 = N - M
            I = N1 + 1
            IJB = 0
            MN2 = 2*N1*N2
*
*           1-norm-based estimate of Difu.
*
   40       CONTINUE
            CALL DLACON( MN2, WORK( MN2+1 ), WORK, IWORK, DIF( 1 ),
     $                   KASE )
            IF( KASE.NE.0 ) THEN
               IF( KASE.EQ.1 ) THEN
*
*                 Solve generalized Sylvester equation.
*
                  CALL DTGSYL( 'N', IJB, N1, N2, A, LDA, A( I, I ), LDA,
     $                         WORK, N1, B, LDB, B( I, I ), LDB,
     $                         WORK( N1*N2+1 ), N1, DSCALE, DIF( 1 ),
     $                         WORK( 2*N1*N2+1 ), LWORK-2*N1*N2, IWORK,
     $                         IERR )
               ELSE
*
*                 Solve the transposed variant.
*
                  CALL DTGSYL( 'T', IJB, N1, N2, A, LDA, A( I, I ), LDA,
     $                         WORK, N1, B, LDB, B( I, I ), LDB,
     $                         WORK( N1*N2+1 ), N1, DSCALE, DIF( 1 ),
     $                         WORK( 2*N1*N2+1 ), LWORK-2*N1*N2, IWORK,
     $                         IERR )
               END IF
               GO TO 40
            END IF
            DIF( 1 ) = DSCALE / DIF( 1 )
*
*           1-norm-based estimate of Difl.
*
   50       CONTINUE
            CALL DLACON( MN2, WORK( MN2+1 ), WORK, IWORK, DIF( 2 ),
     $                   KASE )
            IF( KASE.NE.0 ) THEN
               IF( KASE.EQ.1 ) THEN
*
*                 Solve generalized Sylvester equation.
*
                  CALL DTGSYL( 'N', IJB, N2, N1, A( I, I ), LDA, A, LDA,
     $                         WORK, N2, B( I, I ), LDB, B, LDB,
     $                         WORK( N1*N2+1 ), N2, DSCALE, DIF( 2 ),
     $                         WORK( 2*N1*N2+1 ), LWORK-2*N1*N2, IWORK,
     $                         IERR )
               ELSE
*
*                 Solve the transposed variant.
*
                  CALL DTGSYL( 'T', IJB, N2, N1, A( I, I ), LDA, A, LDA,
     $                         WORK, N2, B( I, I ), LDB, B, LDB,
     $                         WORK( N1*N2+1 ), N2, DSCALE, DIF( 2 ),
     $                         WORK( 2*N1*N2+1 ), LWORK-2*N1*N2, IWORK,
     $                         IERR )
               END IF
               GO TO 50
            END IF
            DIF( 2 ) = DSCALE / DIF( 2 )
*
         END IF
      END IF
*
   60 CONTINUE
*
*     Compute generalized eigenvalues of reordered pair (A, B) and
*     normalize the generalized Schur form.
*
      PAIR = .FALSE.
      DO 80 K = 1, N
         IF( PAIR ) THEN
            PAIR = .FALSE.
         ELSE
*
            IF( K.LT.N ) THEN
               IF( A( K+1, K ).NE.ZERO ) THEN
                  PAIR = .TRUE.
               END IF
            END IF
*
            IF( PAIR ) THEN
*
*             Compute the eigenvalue(s) at position K.
*
               WORK( 1 ) = A( K, K )
               WORK( 2 ) = A( K+1, K )
               WORK( 3 ) = A( K, K+1 )
               WORK( 4 ) = A( K+1, K+1 )
               WORK( 5 ) = B( K, K )
               WORK( 6 ) = B( K+1, K )
               WORK( 7 ) = B( K, K+1 )
               WORK( 8 ) = B( K+1, K+1 )
               CALL DLAG2( WORK, 2, WORK( 5 ), 2, SMLNUM*EPS, BETA( K ),
     $                     BETA( K+1 ), ALPHAR( K ), ALPHAR( K+1 ),
     $                     ALPHAI( K ) )
               ALPHAI( K+1 ) = -ALPHAI( K )
*
            ELSE
*
               IF( SIGN( ONE, B( K, K ) ).LT.ZERO ) THEN
*
*                 If B(K,K) is negative, make it positive
*
                  DO 70 I = 1, N
                     A( K, I ) = -A( K, I )
                     B( K, I ) = -B( K, I )
                     Q( I, K ) = -Q( I, K )
   70             CONTINUE
               END IF
*
               ALPHAR( K ) = A( K, K )
               ALPHAI( K ) = ZERO
               BETA( K ) = B( K, K )
*
            END IF
         END IF
   80 CONTINUE
      RETURN
*
*     End of DTGSEN
*
      END
