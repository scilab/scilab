      SUBROUTINE MB03UD( JOBQ, JOBP, N, A, LDA, Q, LDQ, SV, DWORK,
     $                   LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute all, or part, of the singular value decomposition of a
C     real upper triangular matrix.
C
C     The N-by-N upper triangular matrix A is factored as  A = Q*S*P',
C     where Q and P are N-by-N orthogonal matrices and S is an
C     N-by-N diagonal matrix with non-negative diagonal elements,
C     SV(1), SV(2), ..., SV(N), ordered such that
C
C        SV(1) >= SV(2) >= ... >= SV(N) >= 0.
C
C     The columns of Q are the left singular vectors of A, the diagonal
C     elements of S are the singular values of A and the columns of P
C     are the right singular vectors of A.
C
C     Either or both of Q and P' may be requested.
C     When P' is computed, it is returned in A.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     JOBQ    CHARACTER*1
C             Specifies whether the user wishes to compute the matrix Q
C             of left singular vectors as follows:
C             = 'V':  Left singular vectors are computed;
C             = 'N':  No left singular vectors are computed.
C
C     JOBP    CHARACTER*1
C             Specifies whether the user wishes to compute the matrix P'
C             of right singular vectors as follows:
C             = 'V':  Right singular vectors are computed;
C             = 'N':  No right singular vectors are computed.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrix A.  N >= 0.
C
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, the leading N-by-N upper triangular part of this
C             array must contain the upper triangular matrix A.
C             On exit, if JOBP = 'V', the leading N-by-N part of this 
C             array contains the N-by-N orthogonal matrix  P'; otherwise
C             the N-by-N upper triangular part of A is used as internal
C             workspace. The strictly lower triangular part of A is set 
C             internally to zero before the reduction to bidiagonal form
C             is performed.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,N).
C
C     Q       (output) DOUBLE PRECISION array, dimension (LDQ,N)
C             If JOBQ = 'V', the leading N-by-N part of this array 
C             contains the orthogonal matrix Q.
C             If JOBQ = 'N', Q is not referenced.
C
C     LDQ     INTEGER
C             The leading dimension of array Q.  
C             LDQ >= 1,  and when JOBQ = 'V',  LDQ >= MAX(1,N).
C
C     SV      (output) DOUBLE PRECISION array, dimension (N)
C             The N singular values of the matrix A, sorted in 
C             descending order.
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) returns the optimal LDWORK;
C             if INFO > 0, DWORK(2:N) contains the unconverged
C             superdiagonal elements of an upper bidiagonal matrix B
C             whose diagonal is in SV (not necessarily sorted).
C             B satisfies A = Q*B*P', so it has the same singular
C             values as A, and singular vectors related by Q and P'.
C
C     LDWORK  INTEGER
C             The length of the array DWORK. 
C             LDWORK >= MAX(1,5*N).
C             For optimum performance LDWORK should be larger.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             > 0:  the QR algorithm has failed to converge. In this 
C                   case INFO specifies how many superdiagonals did not 
C                   converge (see the description of DWORK).
C                   This failure is not likely to occur.
C
C     METHOD
C
C     The routine reduces A to bidiagonal form by means of elementary
C     reflectors and then uses the QR algorithm on the bidiagonal form.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute of Informatics, Bucharest, and
C     A. Varga, German Aerospace Center, DLR Oberpfaffenhofen,
C     March 1998. Based on the RASP routine DTRSVD.
C
C     REVISIONS
C
C     V. Sima, Feb. 2000.   
C
C     KEYWORDS
C
C     Bidiagonalization, orthogonal transformation, singular value
C     decomposition, singular values, triangular form.
C
C    ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ONE, ZERO
      PARAMETER         ( ONE = 1.0D0, ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         JOBP, JOBQ
      INTEGER           INFO, LDA, LDQ, LDWORK, N
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), DWORK(*), Q(LDQ,*), SV(*)
C     .. Local Scalars ..
      LOGICAL           WANTQ, WANTP
      INTEGER           I, IE, ISCL, ITAUP, ITAUQ, JWORK, MAXWRK,
     $                  MINWRK, NCOLP, NCOLQ
      DOUBLE PRECISION  ANRM, BIGNUM, EPS, SMLNUM
C     .. Local Arrays ..
      DOUBLE PRECISION  DUM(1)
C     .. External Functions ..
      LOGICAL           LSAME
      INTEGER           ILAENV
      DOUBLE PRECISION  DLAMCH, DLANTR
      EXTERNAL          DLAMCH, DLANTR, ILAENV, LSAME
C     .. External Subroutines ..
      EXTERNAL          DBDSQR, DGEBRD, DLACPY, DLASCL, DLASET, DORGBR,
     $                  XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         MAX, SQRT
C     .. Executable Statements ..
C
C     Check the input scalar arguments.
C
      INFO = 0
      WANTQ = LSAME( JOBQ, 'V' )
      WANTP = LSAME( JOBP, 'V' )
      MINWRK = 1
      IF( .NOT.WANTQ .AND. .NOT.LSAME( JOBQ, 'N' ) ) THEN
         INFO = -1
      ELSE IF( .NOT.WANTP .AND. .NOT.LSAME( JOBP, 'N' ) ) THEN
         INFO = -2
      ELSE IF( N.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -5
      ELSE IF( ( WANTQ .AND. LDQ.LT.MAX( 1, N ) ) .OR. 
     $    ( .NOT.WANTQ .AND. LDQ.LT.1 ) ) THEN
         INFO = -7
      END IF
C
C     Compute workspace
C     (Note: Comments in the code beginning "Workspace:" describe the
C     minimal amount of workspace needed at that point in the code,
C     as well as the preferred amount for good performance.
C     NB refers to the optimal block size for the immediately following 
C     subroutine, as returned by ILAENV.)
C
      IF( INFO.EQ.0 .AND. LDWORK.GE.1 .AND. N.GT.0 ) THEN
         MAXWRK = 3*N+2*N*ILAENV( 1, 'DGEBRD', ' ', N, N, -1, -1 )
         IF( WANTQ )
     $      MAXWRK = MAX( MAXWRK, 3*N+N*
     $                    ILAENV( 1, 'DORGBR', 'Q', N, N, N, -1 ) )
         IF( WANTP )
     $      MAXWRK = MAX( MAXWRK, 3*N+N*
     $                    ILAENV( 1, 'DORGBR', 'P', N, N, N, -1 ) )
         MINWRK = 5*N
         MAXWRK = MAX( MAXWRK, MINWRK )
         DWORK(1) = MAXWRK
      END IF
C
      IF( LDWORK.LT.MINWRK ) THEN
         INFO = -10
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'MB03UD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( N.EQ.0 ) THEN
         DWORK(1) = ONE
         RETURN
      END IF
C
C     Get machine constants.
C
      EPS = DLAMCH( 'P' )
      SMLNUM = SQRT( DLAMCH( 'S' ) ) / EPS
      BIGNUM = ONE / SMLNUM
C
C     Scale A if max entry outside range [SMLNUM,BIGNUM].
C
      ANRM = DLANTR( 'Max', 'Upper', 'Non-unit', N, N, A, LDA, DUM )
      ISCL = 0
      IF( ANRM.GT.ZERO .AND. ANRM.LT.SMLNUM ) THEN
         ISCL = 1
         CALL DLASCL( 'Upper', 0, 0, ANRM, SMLNUM, N, N, A, LDA, INFO )
      ELSE IF( ANRM.GT.BIGNUM ) THEN
         ISCL = 1
         CALL DLASCL( 'Upper', 0, 0, ANRM, BIGNUM, N, N, A, LDA, INFO )
      END IF
C
C     Zero out below.
C
      IF ( N.GT.1 )
     $   CALL DLASET( 'Lower', N-1, N-1, ZERO, ZERO, A(2,1), LDA )
C
C     Find the singular values and optionally the singular vectors
C     of the upper triangular matrix A.
C
      IE = 1
      ITAUQ = IE + N
      ITAUP = ITAUQ + N
      JWORK = ITAUP + N
C
C     First reduce the matrix to bidiagonal form. The diagonal
C     elements will be in SV and the superdiagonals in DWORK(IE).
C     (Workspace: need 4*N, prefer 3*N+2*N*NB)
C
      CALL DGEBRD( N, N, A, LDA, SV, DWORK(IE), DWORK(ITAUQ),
     $             DWORK(ITAUP), DWORK(JWORK), LDWORK-JWORK+1, INFO )
      IF( WANTQ ) THEN
C
C        Generate the transformation matrix Q corresponding to the
C        left singular vectors.
C        (Workspace: need 4*N, prefer 3*N+N*NB)
C
         NCOLQ = N
         CALL DLACPY( 'Lower', N, N, A, LDA, Q, LDQ )
         CALL DORGBR( 'Q', N, N, N, Q, LDQ, DWORK(ITAUQ), DWORK(JWORK),
     $                LDWORK-JWORK+1, INFO )
      ELSE
         NCOLQ = 0
      END IF
      IF( WANTP ) THEN
C
C        Generate the transformation matrix P' corresponding to the
C        right singular vectors.
C        (Workspace: need 4*N, prefer 3*N+N*NB)
C
         NCOLP = N
         CALL DORGBR( 'P', N, N, N, A, LDA, DWORK(ITAUP), DWORK(JWORK),
     $                LDWORK-JWORK+1, INFO )
      ELSE
         NCOLP = 0
      END IF
      JWORK = IE + N
C
C     Perform bidiagonal QR iteration, to obtain all or part of the
C     singular value decomposition of A.
C     (Workspace: need 5*N)
C
      CALL DBDSQR( 'U', N, NCOLP, NCOLQ, 0, SV, DWORK(IE), A, LDA,
     $             Q, LDQ, DUM, 1, DWORK(JWORK), INFO )
C
C     If DBDSQR failed to converge, copy unconverged superdiagonals
C     to DWORK(2:N).
C
      IF( INFO.NE.0 ) THEN
         DO 10 I = N - 1, 1, -1
            DWORK(I+1) = DWORK(I+IE-1)
   10    CONTINUE
      END IF
C
C     Undo scaling if necessary.
C
      IF( ISCL.EQ.1 ) THEN
         IF( ANRM.GT.BIGNUM )
     $      CALL DLASCL( 'G', 0, 0, BIGNUM, ANRM, N, 1, SV, N, INFO )                  
         IF( INFO.NE.0 .AND. ANRM.GT.BIGNUM )
     $      CALL DLASCL( 'G', 0, 0, BIGNUM, ANRM, N-1, 1, DWORK(2), N, 
     $                   INFO )
         IF( ANRM.LT.SMLNUM )
     $      CALL DLASCL( 'G', 0, 0, SMLNUM, ANRM, N, 1, SV, N, INFO )                  
         IF( INFO.NE.0 .AND. ANRM.LT.SMLNUM )
     $      CALL DLASCL( 'G', 0, 0, SMLNUM, ANRM, N-1, 1, DWORK(2), N, 
     $                   INFO )
      END IF
C
C     Return optimal workspace in DWORK(1).
C
      DWORK(1) = MAXWRK
C
      RETURN
C *** Last line of MB03UD ***
      END
