      SUBROUTINE IB01CD( JOBX0, COMUSE, JOB, N, M, L, NSMP, A, LDA, B,
     $                   LDB, C, LDC, D, LDD, U, LDU, Y, LDY, X0, V,
     $                   LDV, TOL, IWORK, DWORK, LDWORK, IWARN, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To estimate the initial state and, optionally, the system matrices
C     B  and  D  of a linear time-invariant (LTI) discrete-time system,
C     given the system matrices  (A,B,C,D),  or (when  B  and  D  are
C     estimated) only the matrix pair  (A,C),  and the input and output
C     trajectories of the system. The model structure is :
C
C           x(k+1) = Ax(k) + Bu(k),   k >= 0,
C           y(k)   = Cx(k) + Du(k),
C
C     where  x(k)  is the  n-dimensional state vector (at time k),
C            u(k)  is the  m-dimensional input vector,
C            y(k)  is the  l-dimensional output vector,
C     and  A, B, C, and D  are real matrices of appropriate dimensions.
C     The input-output data can internally be processed sequentially.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     JOBX0   CHARACTER*1
C             Specifies whether or not the initial state should be 
C             computed, as follows:
C             = 'X':  compute the initial state x(0);
C             = 'N':  do not compute the initial state (possibly,
C                     because x(0) is known to be zero).
C
C     COMUSE  CHARACTER*1
C             Specifies whether the system matrices B and D should be
C             computed or used, as follows:
C             = 'C':  compute the system matrices B and D, as specified
C                     by JOB;
C             = 'U':  use the system matrices B and D, as specified by
C                     JOB;
C             = 'N':  do not compute/use the matrices B and D.
C             If  JOBX0 = 'N'  and  COMUSE <> 'N',  then  x(0)  is set
C             to zero.
C             If  JOBX0 = 'N'  and  COMUSE =  'N',  then  x(0)  is
C             neither computed nor set to zero.
C
C     JOB     CHARACTER*1
C             If  COMUSE = 'C'  or  'U',  specifies which of the system
C             matrices  B and D  should be computed or used, as follows:
C             = 'B':  compute/use the matrix B only (D is known to be
C                     zero);
C             = 'D':  compute/use the matrices B and D.
C             The value of  JOB  is irrelevant if  COMUSE = 'N'  or if
C             JOBX0 = 'N'  and  COMUSE = 'U'.
C             The combinations of options, the data used, and the
C             returned results, are given in the table below, where  
C             '*'  denotes an irrelevant value.
C
C              JOBX0   COMUSE    JOB     Data used    Returned results
C             ----------------------------------------------------------
C                X       C        B       A,C,u,y          x,B
C                X       C        D       A,C,u,y          x,B,D
C                N       C        B       A,C,u,y          x=0,B
C                N       C        D       A,C,u,y          x=0,B,D
C             ----------------------------------------------------------
C                X       U        B      A,B,C,u,y            x 
C                X       U        D      A,B,C,D,u,y          x 
C                N       U        *          -               x=0 
C             ----------------------------------------------------------
C                X       N        *        A,C,y              x 
C                N       N        *          -                - 
C             ----------------------------------------------------------
C
C             For  JOBX0 = 'N'  and  COMUSE = 'N',  the routine just
C             sets  DWORK(1)  to 2 and  DWORK(2)  to 1, and returns
C             (see the parameter DWORK).
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the system.  N >= 0.
C
C     M       (input) INTEGER
C             The number of system inputs.  M >= 0.
C
C     L       (input) INTEGER
C             The number of system outputs.  L > 0.
C
C     NSMP    (input) INTEGER
C             The number of rows of matrices  U  and  Y  (number of
C             samples,  t).
C             NSMP >= 0,            if  JOBX0 = 'N'  and  COMUSE <> 'C';
C             NSMP >= N,            if  JOBX0 = 'X'  and  COMUSE <> 'C';
C             NSMP >= N*M + a + e,  if  COMUSE = 'C',
C             where   a = 0,  if  JOBX0 = 'N';
C                     a = N,  if  JOBX0 = 'X';
C                     e = 0,  if  JOBX0 = 'X'  and  JOB = 'B';
C                     e = 1,  if  JOBX0 = 'N'  and  JOB = 'B';
C                     e = M,  if  JOB   = 'D'.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             If  JOBX0 = 'X'  or  COMUSE = 'C',  the leading N-by-N
C             part of this array must contain the system state matrix A.
C             If  N = 0,  or  JOBX0 = 'N'  and  COMUSE <> 'C',  this
C             array is not referenced.
C
C     LDA     INTEGER
C             The leading dimension of the array A.
C             LDA >= MAX(1,N),  if  JOBX0 = 'X'  or   COMUSE =  'C';
C             LDA >= 1,         if  JOBX0 = 'N'  and  COMUSE <> 'C'.
C
C     B       (input or output) DOUBLE PRECISION array, dimension
C             (LDB,M)
C             If  JOBX0 = 'X'  and  COMUSE = 'U',  B  is an input
C             parameter and, on entry, the leading N-by-M part of this
C             array must contain the system input matrix  B.
C             If  COMUSE = 'C',  B  is an output parameter and, on exit,
C             if  INFO = 0,  the leading N-by-M part of this array
C             contains the estimated system input matrix  B.
C             If  min(N,M) = 0,  or  JOBX0 = 'N'  and  COMUSE = 'U',
C             or  COMUSE = 'N',  this array is not referenced.
C
C     LDB     INTEGER
C             The leading dimension of the array B.
C             LDB >= MAX(1,N),  if  M > 0,  COMUSE = 'U',  JOBX0 = 'X',
C                               or  M > 0,  COMUSE = 'C';
C             LDB >= 1,         if  min(N,M) = 0,  or  COMUSE = 'N',
C                               or  JOBX0  = 'N'  and  COMUSE = 'U'.
C
C     C       (input) DOUBLE PRECISION array, dimension (LDC,N)
C             If  JOBX0 = 'X'  or  COMUSE = 'C',  the leading L-by-N
C             part of this array must contain the system output
C             matrix  C.
C             If  N = 0,  or  JOBX0 = 'N'  and  COMUSE <> 'C',  this
C             array is not referenced.
C
C     LDC     INTEGER
C             The leading dimension of the array C.
C             LDC >= L,  if  N > 0, and  JOBX0 = 'X'  or  COMUSE = 'C';
C             LDC >= 1,  if  N = 0, or  JOBX0 = 'N'  and  COMUSE <> 'C'.
C
C     D       (input or output) DOUBLE PRECISION array, dimension
C             (LDD,M)
C             If  JOBX0 = 'X',  COMUSE = 'U',  and  JOB = 'D',  D  is an
C             input parameter and, on entry, the leading L-by-M part of
C             this array must contain the system input-output matrix  D.
C             If  COMUSE = 'C'  and  JOB = 'D',  D  is an output
C             parameter and, on exit, if  INFO = 0,  the leading 
C             L-by-M part of this array contains the estimated system
C             input-output matrix  D.
C             If  M = 0,  or  JOBX0 = 'N'  and  COMUSE = 'U',  or
C             COMUSE = 'N',  or  JOB = 'B',  this array is not 
C             referenced.  
C
C     LDD     INTEGER
C             The leading dimension of the array D.
C             LDD >= L,  if  M > 0,   JOBX0 = 'X',  COMUSE = 'U',  and
C                                                   JOB = 'D',  or
C                        if  M > 0,  COMUSE = 'C',  and  JOB = 'D';
C             LDD >= 1,  if  M = 0,  or  JOBX0 = 'N'  and  COMUSE = 'U',
C                        or  COMUSE = 'N',  or  JOB = 'B'.
C
C     U       (input or input/output) DOUBLE PRECISION array, dimension
C             (LDU,M)
C             On entry, if  COMUSE = 'C',  or  JOBX0 = 'X'  and
C             COMUSE = 'U',  the leading NSMP-by-M part of this array
C             must contain the t-by-m input-data sequence matrix  U,
C             U = [u_1 u_2 ... u_m].  Column  j  of  U  contains the
C             NSMP  values of the j-th input component for consecutive
C             time increments.
C             On exit, if  COMUSE = 'C'  and  JOB = 'D',  the leading
C             NSMP-by-M part of this array contains details of the
C             QR factorization of the t-by-m matrix  U,  possibly 
C             computed sequentially (see METHOD).
C             If  COMUSE = 'C'  and  JOB = 'B',  or  COMUSE = 'U',  this
C             array is unchanged on exit.
C             If  M = 0,  or  JOBX0 = 'N'  and  COMUSE = 'U',  or
C             COMUSE = 'N',  this array is not referenced.
C
C     LDU     INTEGER
C             The leading dimension of the array U.
C             LDU >= MAX(1,NSMP),  if  M > 0    and  COMUSE = 'C'  or
C                                  JOBX0 = 'X'  and  COMUSE = 'U;
C             LDU >= 1,            if  M = 0,   or   COMUSE = 'N',  or
C                                  JOBX0 = 'N'  and  COMUSE = 'U'.
C
C     Y       (input) DOUBLE PRECISION array, dimension (LDY,L)
C             On entry, if  JOBX0 = 'X'  or  COMUSE = 'C',  the leading
C             NSMP-by-L part of this array must contain the t-by-l
C             output-data sequence matrix  Y,  Y = [y_1 y_2 ... y_l].
C             Column  j  of  Y  contains the  NSMP  values of the j-th
C             output component for consecutive time increments.
C             If  JOBX0 = 'N'  and  COMUSE <> 'C',  this array is not
C             referenced.
C
C     LDY     INTEGER
C             The leading dimension of the array Y.
C             LDY >= MAX(1,NSMP),  if  JOBX0 = 'X'  or   COMUSE = 'C;
C             LDY >= 1,            if  JOBX0 = 'N'  and  COMUSE <> 'C'.
C
C     X0      (output) DOUBLE PRECISION array, dimension (N)
C             If  INFO = 0  and  JOBX0 = 'X',  this array contains the
C             estimated initial state of the system,  x(0).
C             If  JOBX0 = 'N'  and  COMUSE = 'C',  this array is used as
C             workspace and finally it is set to zero.
C             If  JOBX0 = 'N'  and  COMUSE = 'U',  then  x(0)  is set to
C             zero without any calculations.
C             If  JOBX0 = 'N'  and  COMUSE = 'N',  this array is not
C             referenced.
C
C     V       (output) DOUBLE PRECISION array, dimension (LDV,N)
C             On exit, if  INFO = 0  or 2,  JOBX0 = 'X'  or
C             COMUSE = 'C',  the leading N-by-N part of this array
C             contains the orthogonal matrix V of a real Schur
C             factorization of the matrix  A.
C             If  JOBX0 = 'N'  and  COMUSE <> 'C',  this array is not
C             referenced.
C
C     LDV     INTEGER
C             The leading dimension of the array V.
C             LDV >= MAX(1,N),  if  JOBX0 = 'X'  or   COMUSE =  'C;
C             LDV >= 1,         if  JOBX0 = 'N'  and  COMUSE <> 'C'.
C
C     Tolerances
C
C     TOL     DOUBLE PRECISION
C             The tolerance to be used for estimating the rank of
C             matrices. If the user sets  TOL > 0,  then the given value
C             of  TOL  is used as a lower bound for the reciprocal
C             condition number;  a matrix whose estimated condition
C             number is less than  1/TOL  is considered to be of full  
C             rank.  If the user sets  TOL <= 0,  then  EPS  is used
C             instead, where  EPS  is the relative machine precision
C             (see LAPACK Library routine DLAMCH).  TOL <= 1.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (LIWORK), where
C             LIWORK >= 0,          if  JOBX0 = 'N'  and  COMUSE <> 'C';
C             LIWORK >= N,          if  JOBX0 = 'X'  and  COMUSE <> 'C';
C             LIWORK >= N*M + a,        if COMUSE = 'C' and JOB = 'B',
C             LIWORK >= max(N*M + a,M), if COMUSE = 'C' and JOB = 'D',
C             with  a = 0,  if  JOBX0 = 'N';
C                   a = N,  if  JOBX0 = 'X'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if  INFO = 0,  DWORK(1) returns the optimal value
C             of LDWORK;  DWORK(2)  contains the reciprocal condition
C             number of the triangular factor of the QR factorization of
C             the matrix  W2,  if  COMUSE = 'C',  or of the matrix
C             Gamma,  if  COMUSE = 'U'  (see METHOD); if  JOBX0 = 'N'  
C             and  COMUSE <> 'C',  DWORK(2)  is set to one;
C             if  COMUSE = 'C',  M > 0,  and  JOB = 'D',   DWORK(3)
C             contains the reciprocal condition number of the triangular
C             factor of the QR factorization of  U;  denoting
C                g = 2,  if  JOBX0  = 'X'  and  COMUSE <> 'C'  or
C                            COMUSE = 'C'  and  M = 0  or   JOB = 'B',
C                g = 3,  if  COMUSE = 'C'  and  M > 0  and  JOB = 'D',
C             then  DWORK(i), i = g+1:g+N*N,  
C                   DWORK(j), j = g+1+N*N:g+N*N+L*N,  and
C                   DWORK(k), k = g+1+N*N+L*N:g+N*N+L*N+N*M,   
C             contain the transformed system matrices  At, Ct, and Bt,
C             respectively, corresponding to the real Schur form of the
C             given system state matrix  A,  i.e.,
C                At = V'*A*V,  Bt = V'*B,  Ct = C*V.
C             The matrices  At, Ct, Bt  are not computed if  JOBX0 = 'N'
C             and  COMUSE <> 'C'.
C             On exit, if  INFO = -26,  DWORK(1)  returns the minimum
C             value of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= 2,  if  JOBX0 = 'N'  and  COMUSE <> 'C',  or
C                           if  max( N, M ) = 0.
C             Otherwise,
C             LDWORK >= LDW1 + N*( N + M + L ) + 
C                              max( 5*N, LDW1, min( LDW2, LDW3 ) ),
C             where, if  COMUSE = 'C',  then
C             LDW1 = 2,          if  M = 0  or   JOB = 'B',
C             LDW1 = 3,          if  M > 0  and  JOB = 'D',
C             LDWa = t*L*(r + 1) + max( N + max( d, f ), 6*r ),
C             LDW2 = LDWa,       if  M = 0  or  JOB = 'B',
C             LDW2 = max( LDWa, t*L*(r + 1) + 2*M*M + 6*M ),  
C                                if  M > 0  and JOB = 'D',
C             LDWb = (b + r)*(r + 1) + 
C                     max( q*(r + 1) + N*N*M + c + max( d, f ), 6*r ),
C             LDW3 = LDWb,       if  M = 0  or  JOB = 'B',
C             LDW3 = max( LDWb, (b + r)*(r + 1) + 2*M*M + 6*M ),  
C                                if  M > 0  and JOB = 'D',
C                r = N*M + a,
C                a = 0,                  if  JOBX0 = 'N',
C                a = N,                  if  JOBX0 = 'X';
C                b = 0,                  if  JOB   = 'B',
C                b = L*M,                if  JOB   = 'D';
C                c = 0,                  if  JOBX0 = 'N',
C                c = L*N,                if  JOBX0 = 'X';
C                d = 0,                  if  JOBX0 = 'N',
C                d = 2*N*N + N,          if  JOBX0 = 'X';
C                f = 2*r,                if  JOB   = 'B'   or  M = 0,
C                f = M + max( 2*r, M ),  if  JOB   = 'D'  and  M > 0;
C                q = b + r*L;
C             and, if  JOBX0 = 'X'  and  COMUSE <> 'C',  then
C             LDW1 = 2,
C             LDW2 = t*L*(N + 1) + 2*N + max( 2*N*N, 4*N ),
C             LDW3 = N*(N + 1) + 2*N + max( q*(N + 1) + 2*N*N + L*N,
C                                           4*N ),
C                q = N*L.
C             For good performance,  LDWORK  should be larger.
C             If  LDWORK >= LDW2,  or if  COMUSE = 'C'  and
C                 LDWORK >= t*L*(r + 1) + (b + r)*(r + 1) + N*N*M + c +
C                           max( d, f ),
C             then standard QR factorizations of the matrices  U  and/or
C             W2,  if  COMUSE = 'C',  or of the matrix  Gamma,  if  
C             JOBX0 = 'X'  and  COMUSE <> 'C'  (see METHOD), are used. 
C             Otherwise, the QR factorizations are computed sequentially
C             by performing  NCYCLE  cycles, each cycle (except possibly
C             the last one) processing  s < t  samples, where  s  is
C             chosen by equating  LDWORK  to the first term of  LDWb,
C             if  COMUSE = 'C',  or of  LDW3,  if  COMUSE <> 'C',  for
C             q  replaced by  s*L.  (s  is larger than or equal to the
C             minimum value of  NSMP.)  The computational effort may
C             increase and the accuracy may slightly decrease with the
C             decrease of  s.  Recommended value is  LDWORK = LDW2,
C             assuming a large enough cache size, to also accommodate
C             A,  (B,)  C,  (D,)  U,  and  Y.
C
C     Warning Indicator
C
C     IWARN   INTEGER
C             = 0:  no warning;
C             = 4:  the least squares problem to be solved has a
C                   rank-deficient coefficient matrix;
C             = 6:  the matrix  A  is unstable;  the estimated  x(0)
C                   and/or  B and D  could be inaccurate.
C             NOTE: the value 4 of  IWARN  has no significance for the
C                   identification problem. 
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  if the QR algorithm failed to compute all the
C                   eigenvalues of the matrix A (see LAPACK Library
C                   routine DGEES); the locations  DWORK(i),  for
C                   i = g+1:g+N*N,  contain the partially converged  
C                   Schur form;  
C             = 2:  the singular value decomposition (SVD) algorithm did
C                   not converge.
C
C     METHOD
C
C     Matrix  A  is initially reduced to a real Schur form, A = V*At*V',
C     and the given system matrices are transformed accordingly. For the
C     reduced system, an extension and refinement of the method in [1,2]
C     is used. Specifically, for  JOBX0 = 'X',  COMUSE = 'C',  and
C     JOB = 'D',  denoting
C
C           X = [ vec(D')' vec(B)' x0' ]',
C
C     where  vec(M)  is the vector obtained by stacking the columns of
C     the matrix  M,  then  X  is the least squares solution of the 
C     system  S*X = vec(Y),  with the matrix  S = [ diag(U)  W ], 
C     defined by 
C
C           ( U         |     | ... |     |     | ... |     |         )
C           (   U       |  11 | ... |  n1 |  12 | ... |  nm |         )
C       S = (     :     | y   | ... | y   | y   | ... | y   | P*Gamma ),
C           (       :   |     | ... |     |     | ... |     |         )
C           (         U |     | ... |     |     | ... |     |         )
C                                                                     ij
C     diag(U)  having  L  block rows and columns.  In this formula,  y 
C     are the outputs of the system for zero initial state computed 
C     using the following model, for j = 1:m, and for i = 1:n,
C            ij          ij                    ij
C           x  (k+1) = Ax  (k) + e_i u_j(k),  x  (0) = 0,
C
C            ij          ij
C           y  (k)   = Cx  (k),
C
C     where  e_i  is the i-th n-dimensional unit vector,  Gamma  is
C     given by
C
C                (     C     )
C                (    C*A    )
C        Gamma = (   C*A^2   ),
C                (     :     )
C                ( C*A^(t-1) )
C
C     and  P  is a permutation matrix that groups together the rows of
C     Gamma  depending on the same row of  C,  namely  
C     [ c_j;  c_j*A;  c_j*A^2; ...  c_j*A^(t-1) ],  for j = 1:L.
C     The first block column,  diag(U),  is not explicitly constructed,
C     but its structure is exploited. The last block column is evaluated
C     using powers of A with exponents 2^k. No interchanges are applied.
C     A special QR decomposition of the matrix  S  is computed. Let 
C     U = q*[ r' 0 ]'  be the QR decomposition of  U,  if  M > 0,  where
C     r  is  M-by-M.   Then,  diag(q')  is applied to  W  and  vec(Y).  
C     The block-rows of  S  and  vec(Y)  are implicitly permuted so that
C     matrix  S  becomes
C
C        ( diag(r)  W1 )
C        (    0     W2 ),
C        
C     where  W1  has L*M rows. Then, the QR decomposition of  W2 is 
C     computed (sequentially, if  M > 0) and used to obtain  B  and  x0.
C     The intermediate results and the QR decomposition of  U  are 
C     needed to find  D.  If a triangular factor is too ill conditioned,
C     then singular value decomposition (SVD) is employed. SVD is not 
C     generally needed if the input sequence is sufficiently
C     persistently exciting and  NSMP  is large enough. 
C     If the matrix  W  cannot be stored in the workspace (i.e.,  
C     LDWORK < LDW2),  the QR decompositions of  W2  and  U  are 
C     computed sequentially.
C     For  JOBX0 = 'N'  and  COMUSE = 'C',  or  JOB = 'B',  a simpler
C     problem is solved efficiently.
C
C     For  JOBX0 = 'X'  and  COMUSE <> 'C',  a simpler method is used.
C     Specifically, the output y0(k) of the system for zero initial
C     state is computed for k = 0, 1, ...,  t-1 using the given model. 
C     Then the following least squares problem is solved for x(0)
C
C                         (   y(0) - y0(0)   )
C                         (   y(1) - y0(1)   )
C        Gamma * x(0)  =  (        :         ).
C                         (        :         )
C                         ( y(t-1) - y0(t-1) )
C
C     The coefficient matrix  Gamma  is evaluated using powers of A with
C     exponents 2^k. The QR decomposition of this matrix is computed.
C     If its triangular factor  R  is too ill conditioned, then singular
C     value decomposition of  R  is used.
C     If the coefficient matrix cannot be stored in the workspace (i.e.,
C     LDWORK < LDW2),  the QR decomposition is computed sequentially.
C
C
C     REFERENCES
C
C     [1] Verhaegen M., and Varga, A.
C         Some Experience with the MOESP Class of Subspace Model
C         Identification Methods in Identifying the BO105 Helicopter.
C         Report TR R165-94, DLR Oberpfaffenhofen, 1994.
C
C     [2] Sima, V., and Varga, A.
C         RASP-IDENT : Subspace Model Identification Programs.
C         Deutsche Forschungsanstalt fur Luft- und Raumfahrt e. V., 
C         Report TR R888-94, DLR Oberpfaffenhofen, Oct. 1994.
C
C     NUMERICAL ASPECTS
C
C     The implemented method is numerically stable.
C
C     FURTHER COMMENTS
C
C     The algorithm for computing the system matrices  B  and  D  is 
C     less efficient than the MOESP or N4SID algorithms implemented in
C     SLICOT Library routines IB01BD/IB01PD, because a large least
C     squares problem has to be solved, but the accuracy is better, as 
C     the computed matrices  B  and  D  are fitted to the input and
C     output trajectories. However, if matrix  A  is unstable, the
C     computed matrices  B  and  D  could be inaccurate.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Apr. 2000.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Identification methods; least squares solutions; multivariable
C     systems; QR decomposition; singular value decomposition.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, TWO, THREE
      PARAMETER          ( ZERO  = 0.0D0, ONE = 1.0D0, TWO = 2.0D0, 
     $                     THREE = 3.0D0 )
C     .. Scalar Arguments ..
      DOUBLE PRECISION   TOL
      INTEGER            INFO, IWARN, L, LDA, LDB, LDC, LDD, LDU, LDV,
     $                   LDWORK, LDY, M, N, NSMP
      CHARACTER          COMUSE, JOB, JOBX0
C     .. Array Arguments ..
      DOUBLE PRECISION   A(LDA, *), B(LDB, *), C(LDC, *), D(LDD, *),
     $                   DWORK(*),  U(LDU, *), V(LDV, *), X0(*), 
     $                   Y(LDY, *)
      INTEGER            IWORK(*)
C     .. Local Scalars ..
      DOUBLE PRECISION   RCOND, RCONDU
      INTEGER            I, IA, IB, IC, IERR, IQ, ISIZE, ITAU, IWARNL, 
     $                   IWI, IWR, JWORK, LDW, LDW2, LDW3, LM, LN,
     $                   MAXWRK, MINSMP, MINWLS, MINWRK, MTMP, N2M,
     $                   NCOL, NCP1, NM, NN, NSMPL
      LOGICAL            COMPBD, USEBD, MAXDIA, MAXDIM, WITHB, WITHD, 
     $                   WITHX0
      CHARACTER          JOBD
C     .. Local Arrays ..
      DOUBLE PRECISION   DUM(1)
C     .. External Functions ..
      LOGICAL            LSAME
      INTEGER            ILAENV
      DOUBLE PRECISION   DLAMCH, DLAPY2
      EXTERNAL           DLAMCH, DLAPY2, ILAENV, LSAME
C     .. External Subroutines ..
      EXTERNAL           DCOPY, DGEMM, DGEMV, DLACPY, IB01QD, IB01RD, 
     $                   TB01WD, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          INT, MAX, MIN
C     .. Executable Statements ..
C
C     Check the input parameters.
C
      WITHX0 = LSAME( JOBX0,  'X' )
      COMPBD = LSAME( COMUSE, 'C' )
      USEBD  = LSAME( COMUSE, 'U' )
      WITHD  = LSAME( JOB,    'D' )
      WITHB  = LSAME( JOB,    'B' )   .OR. WITHD
      MAXDIM = ( WITHX0 .AND. USEBD ) .OR. COMPBD
      MAXDIA = WITHX0 .OR. COMPBD
C
      IWARN = 0
      INFO  = 0
      LDW   = MAX( 1, N )
      LM    = L*M
      LN    = L*N
      NN    = N*N
      NM    = N*M
      N2M   = N*NM
      IF( COMPBD ) THEN
         NCOL = NM
         IF( WITHX0 )
     $      NCOL = NCOL + N
         MINSMP = NCOL
         IF( WITHD ) THEN
            MINSMP = MINSMP + M
            IQ     = MINSMP
         ELSE IF ( .NOT.WITHX0 ) THEN
            IQ     = MINSMP
            MINSMP = MINSMP + 1
         ELSE
            IQ     = MINSMP
         END IF
      ELSE
         NCOL = N
         IF( WITHX0 ) THEN
            MINSMP = N
         ELSE
            MINSMP = 0
         END IF
         IQ = MINSMP
      END IF
C
      IF( .NOT.( WITHX0 .OR. LSAME( JOBX0, 'N' ) ) ) THEN
         INFO = -1
      ELSE IF( .NOT.( COMPBD .OR. USEBD .OR. LSAME( COMUSE, 'N' ) ) )
     $      THEN
         INFO = -2
      ELSE IF( .NOT.WITHB ) THEN
         INFO = -3
      ELSE IF( N.LT.0 ) THEN
         INFO = -4
      ELSE IF( M.LT.0 ) THEN
         INFO = -5
      ELSE IF( L.LE.0 ) THEN
         INFO = -6
      ELSE IF( NSMP.LT.MINSMP ) THEN
         INFO = -7
      ELSE IF( LDA.LT.1 .OR. ( MAXDIA .AND. LDA.LT.LDW ) ) THEN
         INFO = -9
      ELSE IF( LDB.LT.1 .OR. ( M.GT.0 .AND. MAXDIM .AND. LDB.LT.LDW ) )
     $      THEN
         INFO = -11
      ELSE IF( LDC.LT.1 .OR. ( N.GT.0 .AND. MAXDIA .AND. LDC.LT.L ) )
     $      THEN
         INFO = -13
      ELSE IF( LDD.LT.1 .OR. ( M.GT.0 .AND. MAXDIM .AND. WITHD .AND.
     $         LDD.LT.L ) ) THEN
         INFO = -15
      ELSE IF( LDU.LT.1 .OR. ( M.GT.0 .AND. MAXDIM .AND. LDU.LT.NSMP ) )
     $      THEN
         INFO = -17
      ELSE IF( LDY.LT.1 .OR. ( MAXDIA .AND. LDY.LT.NSMP ) ) THEN
         INFO = -19
      ELSE IF( LDV.LT.1 .OR. ( MAXDIA .AND. LDV.LT.LDW ) ) THEN
         INFO = -22
      ELSE IF( TOL.GT.ONE ) THEN
         INFO = -23
      END IF
C
C     Compute workspace.
C      (Note: Comments in the code beginning "Workspace:" describe the
C       minimal amount of workspace needed at that point in the code,
C       as well as the preferred amount for good performance.
C       NB refers to the optimal block size for the immediately
C       following subroutine, as returned by ILAENV.)
C
      IF ( .NOT.MAXDIA .OR. MAX( N, M ).EQ.0 ) THEN
         MINWRK = 2
      ELSE
         NSMPL = NSMP*L
         IQ    = IQ*L
         NCP1  = NCOL + 1
         ISIZE = NSMPL*NCP1
         IF ( COMPBD ) THEN
            IF ( N.GT.0 .AND. WITHX0 ) THEN
               IC = 2*NN + N
            ELSE
               IC = 0
            END IF
         ELSE
            IC = 2*NN
         END IF
         MINWLS = NCOL*NCP1
         IF ( COMPBD ) THEN
            IF ( WITHD ) 
     $         MINWLS = MINWLS + LM*NCP1
            IF ( M.GT.0 .AND. WITHD ) THEN
               IA = M + MAX( 2*NCOL, M )
            ELSE
               IA = 2*NCOL
            END IF
            ITAU = N2M + MAX( IC, IA )
            IF ( WITHX0 )
     $         ITAU = ITAU + LN
            LDW2 = ISIZE  + MAX( N + MAX( IC, IA ), 6*NCOL )
            LDW3 = MINWLS + MAX( IQ*NCP1 + ITAU, 6*NCOL )
            IF ( M.GT.0 .AND. WITHD ) THEN
               LDW2 = MAX( LDW2, ISIZE  + 2*M*M + 6*M )
               LDW3 = MAX( LDW3, MINWLS + 2*M*M + 6*M )
               IA   = 3
            ELSE
               IA = 2
            END IF
         ELSE
            ITAU = IC + LN
            LDW2 = ISIZE  + 2*N + MAX( IC, 4*N )
            LDW3 = MINWLS + 2*N + MAX( IQ*NCP1 + ITAU, 4*N )
            IA   = 2
         END IF
         MINWRK = IA + NN + NM + LN + MAX( 5*N, IA, MIN( LDW2, LDW3 ) )
C
         IF ( INFO.EQ.0 .AND. LDWORK.GE.MINWRK ) THEN
            MAXWRK = MAX( 5*N, IA ) 
            IF ( COMPBD ) THEN
               IF ( M.GT.0 .AND. WITHD ) THEN
                  MAXWRK = MAX( MAXWRK, ISIZE + N + M + 
     $                          MAX( M*ILAENV( 1, 'DGEQRF', ' ', NSMP,
     $                                         M, -1, -1 ),
     $                               NCOL + NCOL*ILAENV( 1, 'DGEQRF',
     $                                   ' ', NSMP-M, NCOL, -1, -1 ) ) )
                  MAXWRK = MAX( MAXWRK, ISIZE + N + M +
     $                          MAX( NCP1*ILAENV( 1, 'DORMQR', 'LT',
     $                                            NSMP, NCP1, M, -1 ),
     $                               NCOL + ILAENV( 1, 'DORMQR', 'LT',
     $                                         NSMP-M, 1, NCOL, -1 ) ) )
               ELSE
                  MAXWRK = MAX( MAXWRK, ISIZE + N + NCOL +
     $                                  MAX( NCOL*ILAENV( 1, 'DGEQRF',
     $                                       ' ', NSMPL, NCOL, -1, -1 ),
     $                                       ILAENV( 1, 'DORMQR', 'LT',
     $                                          NSMPL, 1, NCOL, -1 ) ) )
               END IF
            ELSE
               MAXWRK = MAX( MAXWRK, ISIZE + 2*N +
     $                               MAX( N*ILAENV( 1, 'DGEQRF', ' ',
     $                                              NSMPL, N, -1, -1 ),
     $                                    ILAENV( 1, 'DORMQR', 'LT',
     $                                            NSMPL, 1, N, -1 ) ) )
            END IF
            MAXWRK = IA + NN + NM + LN + MAXWRK
            MAXWRK = MAX( MAXWRK, MINWRK )
         END IF
      END IF
C
      IF ( INFO.EQ.0 .AND. LDWORK.LT.MINWRK ) THEN
         INFO = -26
         DWORK(1) = MINWRK
      END IF
C
C     Return if there are illegal arguments.
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'IB01CD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF ( .NOT.MAXDIA .OR. MAX( N, M ).EQ.0 ) THEN
         DWORK(2) = ONE
         IF ( COMPBD .AND. M.GT.0 .AND. WITHD ) THEN
            DWORK(1) = THREE
            DWORK(3) = ONE
         ELSE
            DWORK(1) = TWO
         END IF
         IF ( N.GT.0 .AND. USEBD ) THEN
            DUM(1) = ZERO
            CALL DCOPY( N, DUM, 0, X0, 1 )
         END IF
         RETURN
      END IF
C
C     Compute the Schur factorization of  A  and transform the other
C     given system matrices accordingly.
C     Workspace:  need   g + N*N + L*N + N*M + 5*N,  where
C                        g = 2,  if  M = 0, COMUSE = 'C', or  JOB = 'B',
C                        g = 3,  if  M > 0, COMUSE = 'C', and JOB = 'D',
C                        g = 2,  if  JOBX0 = 'X'  and  COMUSE <> 'C';
C                 prefer larger.
C
      IA = IA + 1
      IC = IA + NN
      IB = IC + LN
      CALL DLACPY( 'Full', N, N, A, LDA, DWORK(IA), LDW )
      CALL DLACPY( 'Full', L, N, C, LDC, DWORK(IC), L )
C
      IF ( USEBD ) THEN
         MTMP = M
         CALL DLACPY( 'Full', N, M, B, LDB, DWORK(IB), LDW )
      ELSE
         MTMP = 0
      END IF
      IWR   = IB  + NM
      IWI   = IWR + N
      JWORK = IWI + N
C
      CALL TB01WD( N, MTMP, L, DWORK(IA), LDW, DWORK(IB), LDW,
     $             DWORK(IC), L, V, LDV, DWORK(IWR), DWORK(IWI),
     $             DWORK(JWORK), LDWORK-JWORK+1, IERR )
      IF( IERR.GT.0 ) THEN
         INFO = 1
         RETURN
      END IF
      MAXWRK = MAX( MAXWRK, INT( DWORK( JWORK ) ) + JWORK - 1 )
C
      DO 10 I = IWR, IWI - 1
         IF( DLAPY2( DWORK(I), DWORK(I+N) ).GE.ONE )
     $      IWARN = 6
   10 CONTINUE
C
      JWORK = IWR
C
C     Estimate  x(0)  and/or the system matrices  B and D.
C     Workspace: need   g + N*N + L*N + N*M + 
C                           max( g, min( LDW2, LDW3 ) ) (see LDWORK);
C                prefer larger.
C
      IF ( COMPBD ) THEN
         CALL IB01QD( JOBX0, JOB, N, M, L, NSMP, DWORK(IA), LDW,
     $                DWORK(IC), L, U, LDU, Y, LDY, X0, DWORK(IB), LDW,
     $                D, LDD, TOL, IWORK, DWORK(JWORK), LDWORK-JWORK+1, 
     $                IWARNL, INFO )
C
         IF( INFO.EQ.0 ) THEN
            IF ( M.GT.0 .AND. WITHD )
     $         RCONDU = DWORK(JWORK+2)
C
C           Compute the system input matrix  B  corresponding to the
C           original system.  
C
            CALL DGEMM( 'NoTranspose', 'NoTranspose', N, M, N, ONE,
     $                  V, LDV, DWORK(IB), LDW, ZERO, B, LDB )
         END IF
      ELSE
         IF ( WITHD ) THEN
            JOBD = 'N' 
         ELSE
            JOBD = 'Z' 
         END IF
C
         CALL IB01RD( JOBD, N, MTMP, L, NSMP, DWORK(IA), LDW, DWORK(IB),
     $                LDW, DWORK(IC), L, D, LDD, U, LDU, Y, LDY, X0,
     $                TOL, IWORK, DWORK(JWORK), LDWORK-JWORK+1, IWARNL,
     $                INFO )
      END IF
      IWARN = MAX( IWARN, IWARNL )
C
      IF( INFO.EQ.0 ) THEN
         RCOND  = DWORK(JWORK+1)
         MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
         IF( WITHX0 ) THEN
C
C           Transform the initial state estimate to obtain the initial
C           state corresponding to the original system.
C           Workspace: need g + N*N + L*N + N*M + N.
C
            CALL DGEMV( 'NoTranspose', N, N, ONE, V, LDV, X0, 1, ZERO,
     $                  DWORK(JWORK), 1 )
            CALL DCOPY( N, DWORK(JWORK), 1, X0, 1 )
         END IF
C
         DWORK(1) = MAXWRK
         DWORK(2) = RCOND
         IF ( COMPBD .AND. M.GT.0 .AND. WITHD )
     $      DWORK(3) = RCONDU
      END IF
      RETURN
C
C *** End of IB01CD ***
      END
