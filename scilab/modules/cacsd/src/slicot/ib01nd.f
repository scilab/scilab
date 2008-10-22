      SUBROUTINE IB01ND( METH, JOBD, NOBR, M, L, R, LDR, SV, TOL, IWORK, 
     $                   DWORK, LDWORK, IWARN, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To find the singular value decomposition (SVD) giving the system
C     order, using the triangular factor of the concatenated block
C     Hankel matrices. Related preliminary calculations needed for
C     computing the system matrices are also performed.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     METH    CHARACTER*1
C             Specifies the subspace identification method to be used,
C             as follows:
C             = 'M':  MOESP  algorithm with past inputs and outputs;
C             = 'N':  N4SID  algorithm.
C
C     JOBD    CHARACTER*1
C             Specifies whether or not the matrices B and D should later
C             be computed using the MOESP approach, as follows:
C             = 'M':  the matrices B and D should later be computed
C                     using the MOESP approach;
C             = 'N':  the matrices B and D should not be computed using
C                     the MOESP approach.
C             This parameter is not relevant for METH = 'N'.
C
C     Input/Output Parameters
C
C     NOBR    (input) INTEGER
C             The number of block rows,  s,  in the input and output
C             block Hankel matrices.  NOBR > 0.
C
C     M       (input) INTEGER
C             The number of system inputs.  M >= 0.
C
C     L       (input) INTEGER
C             The number of system outputs.  L > 0.
C
C     R       (input/output) DOUBLE PRECISION array, dimension
C             ( LDR,2*(M+L)*NOBR )
C             On entry, the leading 2*(M+L)*NOBR-by-2*(M+L)*NOBR upper
C             triangular part of this array must contain the upper
C             triangular factor R from the QR factorization of the
C             concatenated block Hankel matrices. Denote  R_ij, 
C             i,j = 1:4,  the ij submatrix of  R,  partitioned by 
C             M*NOBR,  M*NOBR,  L*NOBR,  and  L*NOBR  rows and columns.
C             On exit, if INFO = 0, the leading
C             2*(M+L)*NOBR-by-2*(M+L)*NOBR upper triangular part of this
C             array contains the matrix S, the processed upper
C             triangular factor R, as required by other subroutines.
C             Specifically, let  S_ij, i,j = 1:4,  be the ij submatrix
C             of  S,  partitioned by  M*NOBR,  L*NOBR,  M*NOBR,  and 
C             L*NOBR  rows and columns. The submatrix  S_22  contains
C             the matrix of left singular vectors needed subsequently.
C             Useful information is stored in  S_11  and in the
C             block-column  S_14 : S_44.  For METH = 'M' and JOBD = 'M',
C             the upper triangular part of  S_31  contains the upper
C             triangular factor in the QR factorization of the matrix
C             R_1c = [ R_12'  R_22'  R_11' ]',  and  S_12  contains the
C             corresponding leading part of the transformed matrix
C             R_2c = [ R_13'  R_23'  R_14' ]'.  For  METH = 'N',  the
C             subarray  S_41 : S_43  contains the transpose of the
C             matrix contained in  S_14 : S_34. 
C
C     LDR     INTEGER
C             The leading dimension of the array  R.
C             LDR >= MAX( 2*(M+L)*NOBR, 3*M*NOBR ),
C                                  for METH = 'M' and JOBD = 'M';
C             LDR >= 2*(M+L)*NOBR, for METH = 'M' and JOBD = 'N' or
C                                  for METH = 'N'.
C
C     SV      (output) DOUBLE PRECISION array, dimension ( L*NOBR )
C             The singular values of the relevant part of the triangular
C             factor from the QR factorization of the concatenated block
C             Hankel matrices.
C
C     Tolerances
C
C     TOL     DOUBLE PRECISION
C             The tolerance to be used for estimating the rank of
C             matrices. If the user sets  TOL > 0,  then the given value
C             of  TOL  is used as a lower bound for the reciprocal
C             condition number;  an m-by-n matrix whose estimated
C             condition number is less than  1/TOL  is considered to  
C             be of full rank.  If the user sets  TOL <= 0,  then an 
C             implicitly computed, default tolerance, defined by 
C             TOLDEF = m*n*EPS,  is used instead, where  EPS  is the 
C             relative machine precision (see LAPACK Library routine 
C             DLAMCH).
C             This parameter is not used for  METH = 'M'.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension ((M+L)*NOBR)
C             This parameter is not referenced for METH = 'M'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if  INFO = 0,  DWORK(1) returns the optimal value
C             of LDWORK,  and, for  METH = 'N',  DWORK(2)  and  DWORK(3)
C             contain the reciprocal condition numbers of the 
C             triangular factors of the matrices  U_f  and  r_1  [6].
C             On exit, if  INFO = -12,  DWORK(1)  returns the minimum
C             value of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= max( (2*M-1)*NOBR, (M+L)*NOBR, 5*L*NOBR ),
C                                       if METH = 'M' and JOBD = 'M';
C             LDWORK >=  5*L*NOBR,      if METH = 'M' and JOBD = 'N';
C             LDWORK >=  5*(M+L)*NOBR,  if METH = 'N'.
C             For good performance,  LDWORK  should be larger.
C
C     Warning Indicator
C
C     IWARN   INTEGER
C             = 0:  no warning;
C             = 4:  the least squares problems with coefficient matrix
C                   U_f,  used for computing the weighted oblique
C                   projection (for METH = 'N'), have a rank-deficient
C                   coefficient matrix;
C             = 5:  the least squares problem with coefficient matrix
C                   r_1  [6], used for computing the weighted oblique
C                   projection (for METH = 'N'), has a rank-deficient
C                   coefficient matrix.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 2:  the singular value decomposition (SVD) algorithm did
C                   not converge.
C
C     METHOD
C
C     A singular value decomposition (SVD) of a certain matrix is 
C     computed, which reveals the order  n  of the system as the number
C     of "non-zero" singular values. For the MOESP approach, this matrix
C     is  [ R_24'  R_34' ]' := R(ms+1:(2m+l)s,(2m+l)s+1:2(m+l)s),  
C     where  R  is the upper triangular factor  R  constructed by SLICOT
C     Library routine  IB01MD.  For the N4SID approach, a weighted
C     oblique projection is computed from the upper triangular factor  R
C     and its SVD is then found.
C
C     REFERENCES
C
C     [1] Verhaegen M., and Dewilde, P.
C         Subspace Model Identification. Part 1: The output-error 
C         state-space model identification class of algorithms.
C         Int. J. Control, 56, pp. 1187-1210, 1992.
C
C     [2] Verhaegen M.
C         Subspace Model Identification. Part 3: Analysis of the
C         ordinary output-error state-space model identification
C         algorithm.
C         Int. J. Control, 58, pp. 555-586, 1993.
C
C     [3] Verhaegen M.
C         Identification of the deterministic part of MIMO state space
C         models given in innovations form from input-output data.
C         Automatica, Vol.30, No.1, pp.61-74, 1994.
C
C     [4] Van Overschee, P., and De Moor, B.
C         N4SID: Subspace Algorithms for the Identification of
C         Combined Deterministic-Stochastic Systems.
C         Automatica, Vol.30, No.1, pp. 75-93, 1994.
C
C     [5] Van Overschee, P., and De Moor, B.
C         Subspace Identification for Linear Systems: Theory -
C         Implementation - Applications.
C         Kluwer Academic Publishers, Boston/London/Dordrecht, 1996.
C
C     [6] Sima, V.
C         Subspace-based Algorithms for Multivariable System 
C         Identification.
C         Studies in Informatics and Control, 5, pp. 335-344, 1996.
C
C     NUMERICAL ASPECTS
C
C     The implemented method is numerically stable.
C                                      3
C     The algorithm requires 0(((m+l)s) ) floating point operations.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Aug. 1999.
C
C     REVISIONS
C
C     Feb. 2000, Feb. 2001.
C
C     KEYWORDS
C
C     Identification methods, multivariable systems, QR decomposition,
C     singular value decomposition.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO, THREE
      PARAMETER          ( ZERO  = 0.0D0, ONE = 1.0D0, TWO = 2.0D0, 
     $                     THREE = 3.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION   TOL
      INTEGER            INFO, IWARN, L, LDR, LDWORK, M, NOBR
      CHARACTER          JOBD, METH
C     .. Array Arguments ..
      DOUBLE PRECISION   DWORK(*), R(LDR, *), SV(*)
      INTEGER            IWORK(*)
C     .. Local Scalars ..
      DOUBLE PRECISION   EPS, RCOND1, RCOND2, SVLMAX, THRESH, TOLL
      INTEGER            I, IERR, ITAU, ITAU2, ITAU3, JWORK, LLMNOB,
     $                   LLNOBR, LMMNOB, LMNOBR, LNOBR, MAXWRK, MINWRK,
     $                   MMNOBR, MNOBR, NR, NR2, NR3, NR4, NRSAVE, RANK,
     $                   RANK1
      LOGICAL            JOBDM, MOESP, N4SID 
C     .. Local Arrays ..
      DOUBLE PRECISION   DUM(1), SVAL(3)
C     .. External Functions ..
      LOGICAL            LSAME 
      INTEGER            ILAENV
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH, ILAENV, LSAME
C     .. External Subroutines ..
      EXTERNAL           DCOPY, DGEQRF, DLACPY, DLASET, DORMQR, DSWAP,
     $                   DTRCON, MA02AD, MB03OD, MB03UD, MB04ID, MB04IY,
     $                   MB04OD, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          INT, MAX
C     ..
C     .. Executable Statements ..
C
C     Decode the scalar input parameters.
C
      MOESP  = LSAME( METH, 'M' )
      N4SID  = LSAME( METH, 'N' )
      JOBDM  = LSAME( JOBD, 'M' )
      MNOBR  = M*NOBR
      LNOBR  = L*NOBR
      LLNOBR = LNOBR + LNOBR
      LMNOBR = LNOBR + MNOBR
      MMNOBR = MNOBR + MNOBR
      LMMNOB = MMNOBR + LNOBR
      NR     = LMNOBR + LMNOBR
      IWARN  = 0
      INFO   = 0
C
C     Check the scalar input parameters.
C
      IF( .NOT.( MOESP .OR. N4SID ) ) THEN
         INFO = -1
      ELSE IF( MOESP .AND. .NOT.( JOBDM .OR. LSAME( JOBD, 'N' ) ) ) THEN
         INFO = -2
      ELSE IF( NOBR.LE.0 ) THEN
         INFO = -3
      ELSE IF( M.LT.0 ) THEN
         INFO = -4
      ELSE IF( L.LE.0 ) THEN
         INFO = -5
      ELSE IF( LDR.LT.NR .OR. ( MOESP .AND. JOBDM .AND. 
     $         LDR.LT.3*MNOBR ) ) THEN
         INFO = -7
      ELSE
C
C        Compute workspace.
C        (Note: Comments in the code beginning "Workspace:" describe the
C         minimal amount of workspace needed at that point in the code,
C         as well as the preferred amount for good performance.
C         NB refers to the optimal block size for the immediately
C         following subroutine, as returned by ILAENV.)
C
         MINWRK = 1
         IF ( LDWORK.GE.1 ) THEN
            IF ( MOESP ) THEN
               MINWRK = 5*LNOBR
               IF ( JOBDM )
     $            MINWRK = MAX( MMNOBR - NOBR, LMNOBR, MINWRK ) 
               MAXWRK = LNOBR + LNOBR*ILAENV( 1, 'DGEQRF', ' ', LMNOBR,
     $                                        LNOBR, -1, -1 )
            ELSE
C
               MINWRK = MAX( MINWRK, 5*LMNOBR )
               MAXWRK = MAX( MNOBR + MNOBR*ILAENV( 1, 'DGEQRF', ' ', 
     $                                 MMNOBR, MNOBR, -1, -1 ),
     $                       MNOBR + LLNOBR*ILAENV( 1, 'DORMQR', 'LT', 
     $                                 MMNOBR, LLNOBR, MNOBR, -1 ) )
               MAXWRK = MAX( MAXWRK, MNOBR + LNOBR*ILAENV( 1, 'DORMQR',
     $                                 'LN', MMNOBR, LNOBR, MNOBR,
     $                                 -1 ) )
               MAXWRK = MAX( MAXWRK, LNOBR + LNOBR*ILAENV( 1, 'DGEQRF',
     $                                  ' ', LMMNOB, LNOBR, -1, -1 ) )
            END IF
            MAXWRK = MAX( MINWRK, MAXWRK )
         END IF
C
         IF( LDWORK.LT.MINWRK ) THEN 
            INFO = -12
            DWORK( 1 ) = MINWRK
         END IF
      END IF       
C
C     Return if there are illegal arguments.
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'IB01ND', -INFO )
         RETURN
      END IF
C
C     Compute pointers to the needed blocks of  R.
C
      NR2   = MNOBR  + 1
      NR3   = MMNOBR + 1
      NR4   = LMMNOB + 1
      ITAU  = 1
      JWORK = ITAU + MNOBR
C
      IF( MOESP ) THEN
C
C        MOESP approach.
C
         IF( M.GT.0 .AND. JOBDM ) THEN
C
C           Rearrange the blocks of  R:
C           Copy the (1,1) block into the position (3,2) and
C           copy the (1,4) block into (3,3).
C
            CALL DLACPY( 'Upper', MNOBR, MNOBR, R, LDR, R(NR3,NR2),
     $                   LDR )
            CALL DLACPY( 'Full',  MNOBR, LNOBR, R(1,NR4), LDR,
     $                   R(NR3,NR3), LDR )
C
C           Using structure, triangularize the matrix
C              R_1c = [ R_12'  R_22'  R_11' ]'
C           and then apply the transformations to the matrix  
c              R_2c = [ R_13'  R_23'  R_14' ]'.
C           Workspace: need M*NOBR + MAX(M-1,L)*NOBR.
C
            CALL MB04OD( 'Upper', MNOBR, LNOBR, MNOBR, R(NR2,NR2), LDR,
     $                   R(NR3,NR2), LDR, R(NR2,NR3), LDR, R(NR3,NR3),
     $                   LDR, DWORK(ITAU), DWORK(JWORK) )
            CALL MB04ID( MMNOBR, MNOBR, MNOBR-1, LNOBR, R(1,NR2), LDR,
     $                   R(1,NR3), LDR, DWORK(ITAU), DWORK(JWORK),
     $                   LDWORK-JWORK+1, IERR )
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
C           Copy the leading  M*NOBR x M*NOBR  and  M*NOBR x L*NOBR
C           submatrices of  R_1c  and  R_2c,  respectively, into their
C           final positions, required by SLICOT Library routine  IB01PD.
C
            CALL DLACPY( 'Upper', MNOBR, MNOBR, R(1,NR2), LDR, 
     $                   R(LMNOBR+1,1), LDR )
            CALL DLACPY( 'Full',  MNOBR, LNOBR, R(1,NR3), LDR, R(1,NR2),
     $                   LDR )
         END IF
C
C        Copy [ R_24'  R_34' ]'  in  [ R_22'  R_32' ]'.
C
         CALL DLACPY( 'Full', LMNOBR, LNOBR, R(NR2,NR4), LDR,
     $                R(NR2,NR2), LDR )
C
C        Triangularize the matrix in  [ R_22'  R_32' ]'.
C        Workspace: need 2*L*NOBR; prefer L*NOBR + L*NOBR*NB.
C
         JWORK = ITAU + LNOBR
         CALL DGEQRF( LMNOBR, LNOBR, R(NR2,NR2), LDR, DWORK(ITAU),
     $                DWORK(JWORK), LDWORK-JWORK+1, IERR )
C
      ELSE
C
C        N4SID approach.
C
         DUM(1) = ZERO
         LLMNOB = LLNOBR + MNOBR
C
C        Set the precision parameters. A threshold value  EPS**(2/3)  is
C        used for deciding to use pivoting or not, where  EPS  is the 
C        relative machine precision (see LAPACK Library routine DLAMCH).
C
         TOLL   = TOL
         EPS    = DLAMCH( 'Precision' )
         THRESH = EPS**( TWO/THREE )
C
         IF( M.GT.0 ) THEN
C
C           For efficiency of later calculations, interchange the first
C           two block-columns. The corresponding submatrices are
C           redefined according to their new position.
C
            DO 10 I = 1, MNOBR
               CALL DSWAP( I, R(1,I), 1, R(1,MNOBR+I), 1 )
               CALL DCOPY( MNOBR, R(I+1,MNOBR+I), 1, R(I+1,I), 1 )
               CALL DCOPY( MMNOBR-I, DUM, 0, R(I+1,MNOBR+I), 1 )
   10       CONTINUE
C
C           Now,
C
C           U_f = [ R_11'  R_21'    0      0   ]',
C           U_p = [ R_12'    0      0      0   ]',
C           Y_p = [ R_13'  R_23'  R_33'    0   ]',  and
C           Y_f = [ R_14'  R_24'  R_34'  R_44' ]',
C
C           where  R_21,  R_12,  R_33,  and  R_44  are upper triangular.
C           Define  W_p := [ U_p  Y_p ].
C
C           Prepare the computation of residuals of the two least
C           squares problems giving the weighted oblique projection P:
C
C           r_1 = W_p - U_f X_1,   X_1 = arg min || U_f X - W_p ||,
C           r_2 = Y_f - U_f X_2,   X_2 = arg min || U_f X - Y_f ||,
C
C           P = (arg min || r_1 X - r_2 ||)' r_1'.                   (1)
C
C           Alternately,  P'  is given by the projection  
C              P' = Q_1 (Q_1)' r_2,
C           where  Q_1  contains the first  k  columns of the orthogonal
C           matrix in the  QR  factorization of  r_1,  k := rank(r_1).
C
C           Triangularize the matrix  U_f = q r  (using structure), and
C           apply the transformation  q'  to the corresponding part of
C           the matrices  W_p,  and  Y_f.
C           Workspace: need 2*(M+L)*NOBR.
C
            CALL MB04ID( MMNOBR, MNOBR, MNOBR-1, LLMNOB, R, LDR,
     $                   R(1,NR2), LDR, DWORK(ITAU), DWORK(JWORK),
     $                   LDWORK-JWORK+1, IERR )
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
C           Save updated  Y_f  (transposed) in the last block-row of  R.
C
            CALL MA02AD( 'Full', LMMNOB, LNOBR, R(1,NR4), LDR, R(NR4,1),
     $                   LDR )
C
C           Check the condition of the triangular factor  r  and decide
C           to use pivoting or not.
C           Workspace: need 4*M*NOBR.
C
            CALL DTRCON( '1-norm', 'Upper', 'NonUnit', MNOBR, R, LDR,
     $                   RCOND1, DWORK(JWORK), IWORK, IERR )
C
            IF( TOLL.LE.ZERO )
     $         TOLL = MNOBR*MNOBR*EPS
            IF ( RCOND1.GT.MAX( TOLL, THRESH ) ) THEN
C
C              U_f is considered full rank and no pivoting is used.
C
               CALL DLASET( 'Full', MNOBR, LLMNOB, ZERO, ZERO, R(1,NR2),
     $                      LDR )
            ELSE
C
C              Save information about  q  in the (2,1) block of  R.
C              Use QR factorization with column pivoting,  r P = Q R.
C              Information on  Q  is stored in the strict lower triangle
C              of R_11  and in  DWORK(ITAU2).
C
               DO 20 I = 1, MNOBR - 1
                  CALL DCOPY( MNOBR, R(I+1,I), -1, R(NR2,I), -1 )
                  CALL DCOPY( MNOBR-I, DUM, 0, R(I+1,I), 1 )
                  IWORK(I) = 0
   20          CONTINUE
C
               IWORK(MNOBR) = 0
C
C              Workspace: need   5*M*NOBR.
C
               ITAU2  = JWORK
               JWORK  = ITAU2 + MNOBR
               SVLMAX = ZERO
               CALL MB03OD( 'QR', MNOBR, MNOBR, R, LDR, IWORK, TOLL, 
     $                      SVLMAX, DWORK(ITAU2), RANK, SVAL,
     $                      DWORK(JWORK), IERR )
C
C              Workspace: need   2*M*NOBR + (M+2*L)*NOBR;
C                         prefer 2*M*NOBR + (M+2*L)*NOBR*NB. 
C
               CALL DORMQR( 'Left', 'Transpose', MNOBR, LLMNOB, MNOBR,
     $                      R, LDR, DWORK(ITAU2), R(1,NR2), LDR, 
     $                      DWORK(JWORK), LDWORK-JWORK+1, IERR )
               MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
               IF ( RANK.LT.MNOBR ) THEN
C
C                 The least squares problem is rank-deficient.
C
                  IWARN = 4
               END IF
C
C              Determine residuals r_1 and r_2: premultiply by  Q  and
C              then by  q.
C              Workspace: need   2*M*NOBR + (M+2*L)*NOBR);
C                         prefer 2*M*NOBR + (M+2*L)*NOBR*NB.
C
               CALL DLASET( 'Full', RANK, LLMNOB, ZERO, ZERO, R(1,NR2), 
     $                      LDR )
               CALL DORMQR( 'Left', 'NoTranspose', MNOBR, LLMNOB, MNOBR,
     $                      R, LDR, DWORK(ITAU2), R(1,NR2), LDR,
     $                      DWORK(JWORK), LDWORK-JWORK+1, IERR )
               MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
               JWORK  = ITAU2
C
C              Restore the transformation  q.
C
               DO 30 I = 1, MNOBR - 1
                  CALL DCOPY( MNOBR, R(NR2,I), 1, R(I+1,I), 1 )
   30          CONTINUE
C
            END IF
C
C           Premultiply by the transformation  q  (apply transformations
C           in backward order).
C           Workspace: need   M*NOBR + (M+2*L)*NOBR;
C                      prefer larger.
C
            CALL MB04IY( 'Left', 'NoTranspose', MMNOBR, LLMNOB, MNOBR,
     $                   MNOBR-1, R, LDR, DWORK(ITAU), R(1,NR2), LDR,
     $                   DWORK(JWORK), LDWORK-JWORK+1, IERR )
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
         ELSE
C
C           Save  Y_f  (transposed) in the last block-row of  R.
C
            CALL MA02AD( 'Full', LMMNOB, LNOBR, R(1,NR4), LDR, R(NR4,1),
     $                   LDR )
            RCOND1 = ONE
         END IF
C
C        Triangularize the matrix  r_1  for determining the oblique
C        projection  P  in least squares problem in (1).  Exploit the
C        fact that the third block-row of r_1  has the structure  
C        [ 0  T ],  where  T  is an upper triangular matrix.  Then apply
C        the corresponding transformations  Q'  to the matrix  r_2.
C        Workspace: need   2*M*NOBR;
C                   prefer   M*NOBR + M*NOBR*NB.
C
         CALL DGEQRF( MMNOBR, MNOBR, R(1,NR2), LDR, DWORK(ITAU),
     $                DWORK(JWORK), LDWORK-JWORK+1, IERR )
C
C        Workspace: need   M*NOBR + 2*L*NOBR;
C                   prefer M*NOBR + 2*L*NOBR*NB.
C
         CALL DORMQR( 'Left', 'Transpose', MMNOBR, LLNOBR, MNOBR,
     $                R(1,NR2), LDR, DWORK(ITAU), R(1,NR3), LDR,
     $                DWORK(JWORK), LDWORK-JWORK+1, IERR )
         NRSAVE = NR2
C
         ITAU2 = JWORK
         JWORK = ITAU2 + LNOBR
         CALL MB04ID( LMNOBR, LNOBR, LNOBR-1, LNOBR, R(NR2,NR3), LDR,
     $                R(NR2,NR4), LDR, DWORK(ITAU2), DWORK(JWORK),
     $                LDWORK-JWORK+1, IERR )
         MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
C        Check the condition of the triangular matrix of order  (m+l)*s
C        just determined, and decide to use pivoting or not.
C        Workspace: need 4*(M+L)*NOBR.
C
         CALL DTRCON( '1-norm', 'Upper', 'NonUnit', LMNOBR, R(1,NR2),
     $                LDR, RCOND2, DWORK(JWORK), IWORK, IERR )
C
         IF( TOL.LE.ZERO )
     $      TOLL = LMNOBR*LMNOBR*EPS
         IF ( RCOND2.LE.MAX( TOLL, THRESH ) ) THEN
            IF ( M.GT.0 ) THEN
C
C              Save information about  Q  in  R_11  (in the strict lower
C              triangle),  R_21  and  R_31  (transposed information).
C
               CALL DLACPY( 'Lower', MMNOBR-1, MNOBR, R(2,NR2), LDR,
     $                      R(2,1), LDR )
               NRSAVE = 1
C
               DO 40 I = NR2, LMNOBR
                  CALL DCOPY( MNOBR, R(I+1,MNOBR+I), 1, R(MNOBR+I,1), 
     $                        LDR )
   40          CONTINUE
C
            END IF
C
            CALL DLASET( 'Lower', LMNOBR-1, LMNOBR-1, ZERO, ZERO,
     $                   R(2,NR2), LDR )
C
C           Use QR factorization with column pivoting.
C           Workspace: need 5*(M+L)*NOBR.
C
            DO 50 I = 1, LMNOBR
               IWORK(I) = 0
   50       CONTINUE
C
            ITAU3  = JWORK
            JWORK  = ITAU3 + LMNOBR
            SVLMAX = ZERO
            CALL MB03OD( 'QR', LMNOBR, LMNOBR, R(1,NR2), LDR, IWORK,
     $                   TOLL, SVLMAX, DWORK(ITAU3), RANK1, SVAL,
     $                   DWORK(JWORK), IERR )
C
C           Workspace: need   2*(M+L)*NOBR + L*NOBR;
C                      prefer 2*(M+L)*NOBR + L*NOBR*NB.
C
            CALL DORMQR( 'Left', 'Transpose', LMNOBR, LNOBR, LMNOBR,
     $                   R(1,NR2), LDR, DWORK(ITAU3), R(1,NR4), LDR,
     $                   DWORK(JWORK), LDWORK-JWORK+1, IERR )
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
            IF ( RANK1.LT.LMNOBR ) THEN
C
C              The least squares problem is rank-deficient.
C
               IWARN = 5
            END IF
C
C           Apply the orthogonal transformations, in backward order, to
C           [r_2(1:rank(r_1),:)' 0]',  to obtain  P'.
C           Workspace: need   2*(M+L)*NOBR + L*NOBR;
C                      prefer 2*(M+L)*NOBR + L*NOBR*NB.
C
            CALL DLASET( 'Full', LMNOBR-RANK1, LNOBR, ZERO, ZERO,
     $                   R(RANK1+1,NR4), LDR )
            CALL DORMQR( 'Left', 'NoTranspose', LMNOBR, LNOBR, LMNOBR,
     $                   R(1,NR2), LDR, DWORK(ITAU3), R(1,NR4), LDR,
     $                   DWORK(JWORK), LDWORK-JWORK+1, IERR )
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
            JWORK = ITAU3
C
            IF ( M.GT.0 ) THEN
C
C              Restore the saved transpose matrix from  R_31.
C
               DO 60 I = NR2, LMNOBR
                  CALL DCOPY( MNOBR, R(MNOBR+I,1), LDR, R(I+1,MNOBR+I), 
     $                        1 )
   60          CONTINUE
C
            END IF
C
         END IF
C
C        Workspace: need   M*NOBR + L*NOBR;
C                   prefer larger.
C
         CALL MB04IY( 'Left', 'NoTranspose', LMNOBR, LNOBR, LNOBR,
     $                LNOBR-1, R(NR2,NR3), LDR, DWORK(ITAU2),
     $                R(NR2,NR4), LDR, DWORK(JWORK), LDWORK-JWORK+1,
     $                IERR )
         MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
C        Workspace: need   M*NOBR + L*NOBR;
C                   prefer M*NOBR + L*NOBR*NB.
C
         JWORK = ITAU2
         CALL DORMQR( 'Left', 'NoTranspose', MMNOBR, LNOBR, MNOBR,
     $                R(1,NRSAVE), LDR, DWORK(ITAU), R(1,NR4), LDR,
     $                DWORK(JWORK), LDWORK-JWORK+1, IERR )
C
C        Now, the matrix  P'  is available in  R_14 : R_34.
C        Triangularize the matrix  P'.
C        Workspace: need   2*L*NOBR;
C                   prefer   L*NOBR + L*NOBR*NB.
C
         JWORK = ITAU + LNOBR
         CALL DGEQRF( LMMNOB, LNOBR, R(1,NR4), LDR, DWORK(ITAU),
     $                DWORK(JWORK), LDWORK-JWORK+1, IERR )
C
C        Copy the triangular factor to its final position,  R_22.
C
         CALL DLACPY( 'Upper', LNOBR, LNOBR, R(1,NR4), LDR, R(NR2,NR2), 
     $                LDR )
C
C        Restore  Y_f.
C
         CALL MA02AD( 'Full', LNOBR, LMMNOB, R(NR4,1), LDR, R(1,NR4),
     $                LDR )
      END IF
C
C     Find the singular value decomposition of  R_22.
C     Workspace: need 5*L*NOBR.
C
      CALL MB03UD( 'NoVectors', 'Vectors', LNOBR, R(NR2,NR2), LDR,
     $             DUM, 1, SV, DWORK, LDWORK, IERR )
      IF ( IERR.NE.0 ) THEN
         INFO = 2
         RETURN
      END IF
      MAXWRK = MAX( MAXWRK, INT( DWORK(1) ) )
C
C     Transpose  R(m*s+1:(m+L)*s,m*s+1:(m+L)*s)  in-situ; its
C     columns will then be the singular vectors needed subsequently.
C
      DO 70 I = NR2+1, LMNOBR
         CALL DSWAP( LMNOBR-I+1, R(I,I-1), 1, R(I-1,I), LDR )
   70 CONTINUE
C
C     Return optimal workspace in  DWORK(1)  and reciprocal condition
C     numbers, if  METH = 'N'.
C
      DWORK(1) = MAXWRK
      IF ( N4SID ) THEN
         DWORK(2) = RCOND1
         DWORK(3) = RCOND2
      END IF   
      RETURN
C
C *** Last line of IB01ND ***
      END
