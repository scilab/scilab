      SUBROUTINE SB02OY( TYPE, DICO, JOBB, FACT, UPLO, JOBL, JOBE, N, M,
     $                   P, A, LDA, B, LDB, Q, LDQ, R, LDR, L, LDL, E,
     $                   LDE, AF, LDAF, BF, LDBF, TOL, IWORK, DWORK, 
     $                   LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To construct the extended matrix pairs for the computation of the
C     solution of the algebraic matrix Riccati equations arising in the
C     problems of optimal control, both discrete and continuous-time,
C     and of spectral factorization, both discrete and continuous-time.
C     These matrix pairs, of dimension 2N + M, are given by
C
C           discrete-time                   continuous-time
C
C     |A   0   B|     |E   0   0|    |A   0   B|     |E   0   0| 
C     |Q  -E'  L| - z |0  -A'  0|,   |Q   A'  L| - s |0  -E'  0|.   (1)
C     |L'  0   R|     |0  -B'  0|    |L'  B'  R|     |0   0   0| 
C
C     After construction, these pencils are compressed to a form 
C     (see [1])
C
C        lambda x A  - B ,
C                  f    f
C
C     where A  and B  are 2N-by-2N matrices. 
C            f      f
C                              -1
C     Optionally, matrix G = BR  B' may be given instead of B and R;
C     then, for L = 0, 2N-by-2N matrix pairs are directly constructed as
C
C         discrete-time            continuous-time
C
C     |A   0 |     |E   G |    |A  -G |     |E   0 | 
C     |      | - z |      |,   |      | - s |      |.               (2) 
C     |Q  -E'|     |0  -A'|    |Q   A'|     |0  -E'| 
C
C     Similar pairs are obtained for non-zero L, if SLICOT Library
C     routine SB02MT is called before SB02OY.
C     Other options include the case with E identity matrix, L a zero
C     matrix, or Q and/or R given in a factored form, Q = C'C, R = D'D. 
C     For spectral factorization problems, there are minor differences
C     (e.g., B is replaced by C'). 
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     TYPE    CHARACTER*1
C             Specifies the type of problem to be addressed as follows:
C             = 'O':  Optimal control problem;
C             = 'S':  Spectral factorization problem.
C
C     DICO    CHARACTER*1
C             Specifies the type of linear system considered as follows:
C             = 'C':  Continuous-time system;
C             = 'D':  Discrete-time system.
C
C     JOBB    CHARACTER*1
C             Specifies whether or not the matrix G is given, instead
C             of the matrices B and R, as follows:
C             = 'B':  B and R are given;
C             = 'G':  G is given.
C             For JOBB = 'G', a 2N-by-2N matrix pair is directly
C             obtained assuming L = 0 (see the description of JOBL).
C
C     FACT    CHARACTER*1
C             Specifies whether or not the matrices Q and/or R (if 
C             JOBB = 'B') are factored, as follows:
C             = 'N':  Not factored, Q and R are given;
C             = 'C':  C is given, and Q = C'C;
C             = 'D':  D is given, and R = D'D (if TYPE = 'O'), or 
C                     R = D + D' (if TYPE = 'S');
C             = 'B':  Both factors C and D are given, Q = C'C, R = D'D 
C                     (or R = D + D').
C
C     UPLO    CHARACTER*1
C             If JOBB = 'G', or FACT = 'N', specifies which triangle of
C             the matrices G, or Q and R, is stored, as follows:
C             = 'U':  Upper triangle is stored;
C             = 'L':  Lower triangle is stored.
C
C     JOBL    CHARACTER*1
C             Specifies whether or not the matrix L is zero, as follows:
C             = 'Z':  L is zero;
C             = 'N':  L is nonzero.
C             JOBL is not used if JOBB = 'G' and JOBL = 'Z' is assumed.
C             Using SLICOT Library routine SB02MT to compute the
C             corresponding A and Q in this case, before calling SB02OY,
C             enables to obtain 2N-by-2N matrix pairs directly.
C
C     JOBE    CHARACTER*1
C             Specifies whether or not the matrix E is identity, as
C             follows:
C             = 'I':  E is the identity matrix;
C             = 'N':  E is a general matrix.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrices A, Q, and E, and the number
C             of rows of the matrices B and L.  N >= 0.
C
C     M       (input) INTEGER
C             If JOBB = 'B', M is the order of the matrix R, and the
C             number of columns of the matrix B.  M >= 0.
C             M is not used if JOBB = 'G'.
C
C     P       (input) INTEGER
C             If FACT = 'C' or 'D' or 'B', or if TYPE = 'S', P is the 
C             number of rows of the matrix C and/or D, respectively. 
C             P >= 0, and if JOBB = 'B' and TYPE = 'S', then P = M. 
C             Otherwise, P is not used.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             The leading N-by-N part of this array must contain the
C             state matrix A of the system.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,N).
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,*)
C             If JOBB = 'B', the leading N-by-M part of this array must
C             contain the input matrix B of the system.
C             If JOBB = 'G', the leading N-by-N upper triangular part
C             (if UPLO = 'U') or lower triangular part (if UPLO = 'L') 
C             of this array must contain the upper triangular part or
C             lower triangular part, respectively, of the matrix
C                   -1
C             G = BR  B'. The stricly lower triangular part (if 
C             UPLO = 'U') or stricly upper triangular part (if 
C             UPLO = 'L') is not referenced.
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,N).
C
C     Q       (input) DOUBLE PRECISION array, dimension (LDQ,N)
C             If FACT = 'N' or 'D', the leading N-by-N upper triangular
C             part (if UPLO = 'U') or lower triangular part (if UPLO =
C             'L') of this array must contain the upper triangular part
C             or lower triangular part, respectively, of the symmetric
C             output weighting matrix Q. The stricly lower triangular 
C             part (if UPLO = 'U') or stricly upper triangular part (if 
C             UPLO = 'L') is not referenced.
C             If FACT = 'C' or 'B', the leading P-by-N part of this
C             array must contain the output matrix C of the system.
C
C     LDQ     INTEGER
C             The leading dimension of array Q.
C             LDQ >= MAX(1,N) if FACT = 'N' or 'D',
C             LDQ >= MAX(1,P) if FACT = 'C' or 'B'.
C
C     R       (input) DOUBLE PRECISION array, dimension (LDR,M)
C             If FACT = 'N' or 'C', the leading M-by-M upper triangular
C             part (if UPLO = 'U') or lower triangular part (if UPLO =
C             'L') of this array must contain the upper triangular part
C             or lower triangular part, respectively, of the symmetric
C             input weighting matrix R. The stricly lower triangular 
C             part (if UPLO = 'U') or stricly upper triangular part (if 
C             UPLO = 'L') is not referenced.
C             If FACT = 'D' or 'B', the leading P-by-M part of this
C             array must contain the direct transmission matrix D of the
C             system.
C             If JOBB = 'G', this array is not referenced.
C             
C     LDR     INTEGER
C             The leading dimension of array R.
C             LDR >= MAX(1,M) if JOBB = 'B' and FACT = 'N' or 'C';
C             LDR >= MAX(1,P) if JOBB = 'B' and FACT = 'D' or 'B';
C             LDR >= 1        if JOBB = 'G'.
C
C     L       (input) DOUBLE PRECISION array, dimension (LDL,M)
C             If JOBL = 'N' (and JOBB = 'B'), the leading N-by-M part of
C             this array must contain the cross weighting matrix L.
C             If JOBL = 'Z' or JOBB = 'G', this array is not referenced.
C
C     LDL     INTEGER
C             The leading dimension of array L.
C             LDL >= MAX(1,N) if JOBL = 'N';
C             LDL >= 1        if JOBL = 'Z' or JOBB = 'G'.
C
C     E       (input) DOUBLE PRECISION array, dimension (LDE,N)
C             If JOBE = 'N', the leading N-by-N part of this array must
C             contain the matrix E of the descriptor system.
C             If JOBE = 'I', E is taken as identity and this array is
C             not referenced.
C
C     LDE     INTEGER
C             The leading dimension of array E.
C             LDE >= MAX(1,N) if JOBE = 'N';
C             LDE >= 1        if JOBE = 'I'.
C
C     AF      (output) DOUBLE PRECISION array, dimension (LDAF,*)
C             The leading 2N-by-2N part of this array contains the
C             matrix A  in the matrix pencil.
C                     f
C             Array AF must have 2*N+M columns if JOBB = 'B', and 2*N
C             columns, otherwise.
C
C     LDAF    INTEGER
C             The leading dimension of array AF.
C             LDAF >= MAX(1,2*N+M) if JOBB = 'B',
C             LDAF >= MAX(1,2*N)   if JOBB = 'G'.
C
C     BF      (output) DOUBLE PRECISION array, dimension (LDBF,2*N)
C             The leading 2N-by-2N part of this array contains the
C             matrix B  in the matrix pencil.
C                     f
C             The last M zero columns are never constucted. 
C
C     LDBF    INTEGER
C             The leading dimension of array BF.
C             LDBF >= MAX(1,2*N+M) if JOBB = 'B',
C             LDBF >= MAX(1,2*N)   if JOBB = 'G'.
C
C     Tolerances
C
C     TOL     DOUBLE PRECISION
C             The tolerance to be used to test for near singularity of
C             the original matrix pencil, specifically of the triangular
C             factor obtained during the reduction process. If the user
C             sets TOL > 0, then the given value of TOL is used as a
C             lower bound for the reciprocal condition number of that
C             matrix; a matrix whose estimated condition number is less
C             than 1/TOL is considered to be nonsingular. If the user
C             sets TOL <= 0, then a default tolerance, defined by
C             TOLDEF = EPS, is used instead, where EPS is the machine
C             precision (see LAPACK Library routine DLAMCH).
C             This parameter is not referenced if JOBB = 'G'.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (LIWORK)
C             LIWORK >= M if JOBB = 'B',
C             LIWORK >= 1 if JOBB = 'G'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) returns the optimal value
C             of LDWORK. If JOBB = 'B', DWORK(2) returns the reciprocal
C             of the condition number of the M-by-M lower triangular
C             matrix obtained after compression.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.  
C             LDWORK >= 1                  if JOBB = 'G',
C             LDWORK >= MAX(1,2*N + M,3*M) if JOBB = 'B'.
C             For optimum performance LDWORK should be larger.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  if the computed extended matrix pencil is singular,
C                   possibly due to rounding errors.
C
C     METHOD
C
C     The extended matrix pairs are constructed, taking various options 
C     into account. If JOBB = 'B', the problem order is reduced from
C     2N+M to 2N (see [1]).
C
C     REFERENCES
C
C     [1] Van Dooren, P.
C         A Generalized Eigenvalue Approach for Solving Riccati
C         Equations.
C         SIAM J. Sci. Stat. Comp., 2, pp. 121-135, 1981.
C
C     [2] Mehrmann, V.
C         The Autonomous Linear Quadratic Control Problem. Theory and 
C         Numerical Solution.
C         Lect. Notes in Control and Information Sciences, vol. 163,
C         Springer-Verlag, Berlin, 1991.
C
C     [3] Sima, V.
C         Algorithms for Linear-Quadratic Optimization.
C         Pure and Applied Mathematics: A Series of Monographs and
C         Textbooks, vol. 200, Marcel Dekker, Inc., New York, 1996.
C
C     NUMERICAL ASPECTS
C
C     The algorithm is backward stable.
C
C     CONTRIBUTORS
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Sep. 1997.
C     Supersedes Release 2.0 routine SB02CY by T.G.J. Beelen, Philips,
C     Eindhoven, Holland, M. Vanbegin, and P. Van Dooren, Philips
C     Research Laboratory, Brussels, Belgium.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Algebraic Riccati equation, closed loop system, continuous-time
C     system, discrete-time system, optimal regulator, Schur form.
C     
C     ******************************************************************
C
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         DICO, FACT, JOBB, JOBE, JOBL, TYPE, UPLO
      INTEGER           INFO, LDA, LDAF, LDB, LDBF, LDE, LDL, LDQ, LDR,
     $                  LDWORK, M, N, P
      DOUBLE PRECISION  TOL
C     .. Array Arguments ..
      INTEGER           IWORK(*)
      DOUBLE PRECISION  A(LDA,*), AF(LDAF,*), B(LDB,*), BF(LDBF,*),
     $                  DWORK(*), E(LDE,*), L(LDL,*), Q(LDQ,*), R(LDR,*)
C     .. Local Scalars ..
      LOGICAL           DISCR, LFACB, LFACN, LFACQ, LFACR, LJOBB, LJOBE,
     $                  LJOBL, LUPLO, OPTC
      INTEGER           I, ITAU, J, JWORK, N2, N2P1, NM, NNM, NP1, 
     $                  WRKOPT
      DOUBLE PRECISION  RCOND, TOLDEF
C     .. External Functions ..
      LOGICAL           LSAME
      DOUBLE PRECISION  DLAMCH  
      EXTERNAL          DLAMCH, LSAME
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DGEQLF, DLACPY, DLASET, DORMQL, DSYRK, 
     $                  DTRCON, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         INT, MAX
C     .. Executable Statements ..
C
      INFO  = 0
      OPTC  = LSAME( TYPE, 'O' )
      DISCR = LSAME( DICO, 'D' )
      LJOBB = LSAME( JOBB, 'B' )      
      LFACN = LSAME( FACT, 'N' )
      LFACQ = LSAME( FACT, 'C' )
      LFACR = LSAME( FACT, 'D' )
      LFACB = LSAME( FACT, 'B' )
      LUPLO = LSAME( UPLO, 'U' )
      LJOBE = LSAME( JOBE, 'I' )
      N2 = N + N
      IF ( LJOBB ) THEN
         LJOBL = LSAME( JOBL, 'Z' )
         NM  = N + M 
         NNM = N2 + M
      ELSE
         NM = N 
         NNM = N2
      END IF
      NP1  = N + 1
      N2P1 = N2 + 1
C
C     Test the input scalar arguments.
C
      IF( .NOT.OPTC .AND. .NOT.LSAME( TYPE, 'S' ) ) THEN
         INFO = -1
      ELSE IF( .NOT.DISCR .AND. .NOT.LSAME( DICO, 'C' ) ) THEN
         INFO = -2
      ELSE IF( .NOT.LJOBB .AND. .NOT.LSAME( JOBB, 'G' ) ) THEN
         INFO = -3
      ELSE IF( .NOT.LFACQ .AND. .NOT.LFACR .AND. .NOT.LFACB
     $                                     .AND. .NOT.LFACN ) THEN
         INFO = -4
      ELSE IF( .NOT.LJOBB .OR. LFACN ) THEN
         IF( .NOT.LUPLO .AND. .NOT.LSAME( UPLO, 'L' ) ) 
     $      INFO = -5
      ELSE IF( LJOBB ) THEN
         IF( .NOT.LJOBL .AND. .NOT.LSAME( JOBL, 'N' ) )
     $      INFO = -6
      ELSE IF( .NOT.LJOBE .AND. .NOT.LSAME( JOBE, 'N' ) ) THEN
         INFO = -7
      ELSE IF( N.LT.0 ) THEN
         INFO = -8
      ELSE IF( LJOBB ) THEN
         IF( M.LT.0 ) 
     $      INFO = -9
      ELSE IF( .NOT.LFACN .OR. .NOT.OPTC ) THEN
         IF( P.LT.0 ) THEN
            INFO = -10
         ELSE IF( LJOBB ) THEN
            IF( .NOT.OPTC .AND. P.NE.M )
     $         INFO = -10
         END IF
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -12
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -14
      ELSE IF( ( ( LFACN.OR.LFACR ) .AND. LDQ.LT.MAX( 1, N ) ) .OR.
     $         ( ( LFACQ.OR.LFACB ) .AND. LDQ.LT.MAX( 1, P ) ) ) THEN
         INFO = -16
      ELSE IF( LDR.LT.1 ) THEN
         INFO = -18
      ELSE IF( LJOBB ) THEN
         IF ( ( LFACN.OR.LFACQ ) .AND. LDR.LT.M .OR.
     $        ( LFACR.OR.LFACB ) .AND. LDR.LT.P ) THEN
            INFO = -18
         ELSE IF( ( .NOT.LJOBL .AND. LDL.LT.MAX( 1, N ) ) .OR.
     $            (      LJOBL .AND. LDL.LT.1 ) ) THEN
            INFO = -20
         END IF
      END IF
      IF( ( .NOT.LJOBE .AND. LDE.LT.MAX( 1, N ) ) .OR.
     $    (      LJOBE .AND. LDE.LT.1 ) ) THEN
         INFO = -22
      ELSE IF( LDAF.LT.MAX( 1, NNM ) ) THEN
         INFO = -24
      ELSE IF( LDBF.LT.MAX( 1, NNM ) ) THEN
         INFO = -26
      ELSE IF( ( LJOBB .AND. LDWORK.LT.MAX( NNM, 3*M ) ) .OR. 
     $                       LDWORK.LT.1 ) THEN
         INFO = -30
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'SB02OY', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      DWORK(1) = ONE
      IF ( N.EQ.0 ) 
     $   RETURN
C
C     Construct the extended matrices in AF and BF, by block-columns.
C
      CALL DLACPY( 'Full', N, N, A, LDA, AF, LDAF )
C
      IF ( .NOT.LFACQ .AND. .NOT.LFACB ) THEN
         CALL DLACPY( UPLO, N, N, Q, LDQ, AF(NP1,1), LDAF )
         IF ( LUPLO ) THEN
C        
C           Construct the lower triangle of Q.
C           
            DO 20 J = 1, N - 1
               CALL DCOPY( N-J, Q(J,J+1), LDQ, AF(NP1+J,J), 1 )
   20       CONTINUE
C        
         ELSE
C        
C           Construct the upper triangle of Q.
C           
            DO 40 J = 2, N
               CALL DCOPY( J-1, Q(J,1), LDQ, AF(NP1,J), 1 )
   40       CONTINUE
C
         END IF
      ELSE
         CALL DSYRK( 'Upper', 'Transpose', N, P, ONE, Q, LDQ, ZERO,
     $               AF(NP1,1), LDAF )
C
         DO 60 J = 2, N
            CALL DCOPY( J-1, AF(NP1,J), 1, AF(N+J,1), LDAF )
   60    CONTINUE
C
      END IF
C
      IF ( LJOBB ) THEN
         IF ( LJOBL ) THEN
            CALL DLASET( 'Full', M, N, ZERO, ZERO, AF(N2P1,1), LDAF )
         ELSE
C
            DO 80 I = 1, N
               CALL DCOPY( M, L(I,1), LDL, AF(N2P1,I), 1 )
   80       CONTINUE
C
         END IF
      END IF
C
      IF ( DISCR.OR.LJOBB ) THEN
         CALL DLASET( 'Full', N, N, ZERO, ZERO, AF(1,NP1), LDAF )
      ELSE
         IF ( LUPLO ) THEN
C        
C           Construct (1,2) block of AF using the upper triangle of G.
C           
            DO 140 J = 1, N
C
               DO 100 I = 1, J
                  AF(I,N+J)= -B(I,J)
  100          CONTINUE
C
               DO 120 I = J + 1, N
                  AF(I,N+J)= -B(J,I)
  120          CONTINUE
C
  140       CONTINUE
C        
         ELSE
C        
C           Construct (1,2) block of AF using the lower triangle of G.
C           
            DO 200 J = 1, N
C
               DO 160 I = 1, J - 1
                  AF(I,N+J)= -B(J,I)
  160          CONTINUE
C
               DO 180 I = J, N
                  AF(I,N+J)= -B(I,J)
  180          CONTINUE
C
  200       CONTINUE
C        
         END IF
      END IF
C
      IF ( DISCR ) THEN
         IF ( LJOBE ) THEN
            CALL DLASET( 'Full', NM, N, ZERO, -ONE, AF(NP1,NP1), LDAF )
         ELSE
C
            DO 240 J = 1, N
C           
               DO 220 I = 1, N
                  AF(N+I,N+J)= -E(J,I)
  220          CONTINUE
C           
  240       CONTINUE
C
            IF ( LJOBB ) 
     $         CALL DLASET( 'Full', M, N, ZERO, ZERO, AF(N2P1,NP1),
     $                      LDAF )
         END IF
      ELSE
C
         DO 280 J = 1, N
C        
            DO 260 I = 1, N
               AF(N+I,N+J)= A(J,I)
  260       CONTINUE
C        
  280    CONTINUE
C
         IF ( LJOBB ) THEN
            IF ( OPTC ) THEN
C
               DO 300 J = 1, N
                  CALL DCOPY ( M, B(J,1), LDB, AF(N2P1,N+J), 1 ) 
  300          CONTINUE
C
            ELSE
               CALL DLACPY( 'Full', P, N, Q, LDQ, AF(N2P1,NP1), LDAF )
            END IF
         END IF
      END IF
C
      IF ( LJOBB ) THEN
C
         IF ( OPTC ) THEN
            CALL DLACPY( 'Full', N, M, B, LDB, AF(1,N2P1), LDAF )
         ELSE
C
            DO 320 I = 1, P
               CALL DCOPY( N, Q(I,1), LDQ, AF(1,N2+I), 1 )
  320       CONTINUE
C
         END IF
C
         IF ( LJOBL ) THEN
            CALL DLASET( 'Full', N, M, ZERO, ZERO, AF(NP1,N2P1), LDAF )
         ELSE
            CALL DLACPY( 'Full', N, M, L, LDL, AF(NP1,N2P1), LDAF )
         END IF
C
         IF ( .NOT.LFACR .AND. .NOT.LFACB ) THEN
            CALL DLACPY( UPLO, M, M, R, LDR, AF(N2P1,N2P1), LDAF )
            IF ( LUPLO ) THEN
C           
C              Construct the lower triangle of R.
C              
               DO 340 J = 1, M - 1
                  CALL DCOPY( M-J, R(J,J+1), LDR, AF(N2P1+J,N2+J), 1 )
  340          CONTINUE
C           
            ELSE
C           
C              Construct the upper triangle of R.
C              
               DO 360 J = 2, M
                  CALL DCOPY( J-1, R(J,1), LDR, AF(N2P1,N2+J), 1 )
  360          CONTINUE
C           
            END IF
         ELSE IF ( OPTC ) THEN
            CALL DSYRK( 'Upper', 'Transpose', M, P, ONE, R, LDR, ZERO,
     $                  AF(N2P1,N2P1), LDAF )
C
            DO 380 J = 2, M
               CALL DCOPY( J-1, AF(N2P1,N2+J), 1, AF(N2+J,N2P1), LDAF )
  380       CONTINUE
C
         ELSE
C
            DO 420 J = 1, M
C           
               DO 400 I = 1, P
                  AF(N2+I,N2+J) = R(I,J) + R(J,I)
  400          CONTINUE
C           
  420       CONTINUE
C
         END IF
      END IF
C
C     Construct the first two block columns of BF.
C
      IF ( LJOBE ) THEN
         CALL DLASET( 'Full', N+NM, N, ZERO, ONE, BF, LDBF )
      ELSE
         CALL DLACPY( 'Full', N, N, E, LDE, BF, LDBF )
         CALL DLASET( 'Full', NM, N, ZERO, ZERO, BF(NP1,1), LDBF )
      END IF
C
      IF ( .NOT.DISCR.OR.LJOBB ) THEN
         CALL DLASET( 'Full', N, N, ZERO, ZERO, BF(1,NP1), LDBF )
      ELSE
         IF ( LUPLO ) THEN
C        
C           Construct (1,2) block of BF using the upper triangle of G.
C           
            DO 480 J = 1, N
C
               DO 440 I = 1, J
                  BF(I,N+J)= B(I,J)
  440          CONTINUE
C
               DO 460 I = J + 1, N
                  BF(I,N+J)= B(J,I)
  460          CONTINUE
C
  480       CONTINUE
C        
         ELSE
C        
C           Construct (1,2) block of BF using the lower triangle of G.
C           
            DO 540 J = 1, N
C
               DO 500 I = 1, J - 1
                  BF(I,N+J)= B(J,I)
  500          CONTINUE
C
               DO 520 I = J, N
                  BF(I,N+J)= B(I,J)
  520          CONTINUE
C
  540       CONTINUE
C        
         END IF
      END IF
C
      IF ( DISCR ) THEN
C     
         DO 580 J = 1, N
C        
            DO 560 I = 1, N
               BF(N+I,N+J)= -A(J,I)
  560       CONTINUE
C           
  580    CONTINUE
C
         IF ( LJOBB ) THEN
C        
            IF ( OPTC ) THEN
C        
               DO 620 J = 1, N
C              
                  DO 600 I = 1, M
                     BF(N2+I,N+J)= -B(J,I)
  600             CONTINUE
C              
  620          CONTINUE
C        
            ELSE
C        
               DO 660 J = 1, N
C              
                  DO 640 I = 1, P
                     BF(N2+I,N+J) = -Q(I,J)
  640             CONTINUE
C        
  660          CONTINUE
C              
            END IF
         END IF
C
      ELSE
         IF ( LJOBE ) THEN
            CALL DLASET( 'Full', NM, N, ZERO, -ONE, BF(NP1,NP1), LDBF )
         ELSE
C        
            DO 700 J = 1, N
C           
               DO 680 I = 1, N
                  BF(N+I,N+J)= -E(J,I)
  680          CONTINUE
C        
  700       CONTINUE
C        
            IF ( LJOBB ) 
     $         CALL DLASET( 'Full', M, N, ZERO, ZERO, BF(N2P1,NP1), 
     $                      LDBF )
         END IF
      END IF
C
      IF ( .NOT.LJOBB ) 
     $   RETURN
C
C     Compress the pencil lambda x BF - AF, using QL factorization.
C     (Note: Comments in the code beginning "Workspace:" describe the
C     minimal amount of real workspace needed at that point in the
C     code, as well as the preferred amount for good performance.
C     NB refers to the optimal block size for the immediately
C     following subroutine, as returned by ILAENV.)
C     
C     Workspace: need 2*M;  prefer M + M*NB.
C
      ITAU  = 1 
      JWORK = ITAU + M 
      CALL DGEQLF( NNM, M, AF(1,N2P1), LDAF, DWORK(ITAU), DWORK(JWORK), 
     $             LDWORK-JWORK+1, INFO )
      WRKOPT = DWORK(JWORK)
C
C     Workspace: need 2*N+M;  prefer M + 2*N*NB.
C
      CALL DORMQL( 'Left', 'Transpose', NNM, N2, M, AF(1,N2P1), LDAF,
     $             DWORK(ITAU), AF, LDAF, DWORK(JWORK), LDWORK-JWORK+1, 
     $             INFO )
      WRKOPT = MAX( WRKOPT, INT( DWORK(JWORK) )+JWORK-1 )
C
      CALL DORMQL( 'Left', 'Transpose', NNM, N2, M, AF(1,N2P1), LDAF, 
     $             DWORK(ITAU), BF, LDBF, DWORK(JWORK), LDWORK-JWORK+1,
     $             INFO )
C
C     Check the singularity of the L factor in the QL factorization: 
C     if singular, then the extended matrix pencil is also singular.
C     Workspace 3*M.
C       
      TOLDEF = TOL
      IF ( TOLDEF.LE.ZERO )
     $   TOLDEF = DLAMCH( 'Epsilon' )
C
      CALL DTRCON( '1-norm', 'Lower', 'Non unit', M, AF(N2P1,N2P1), 
     $             LDAF, RCOND, DWORK, IWORK, INFO )
      WRKOPT = MAX( WRKOPT, 3*M )
C     
      IF ( RCOND.LE.TOLDEF ) 
     $   INFO = 1
C
      DWORK(1) = WRKOPT
      DWORK(2) = RCOND
C
      RETURN
C *** Last line of SB02OY ***
      END
