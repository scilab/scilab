      SUBROUTINE SB02RD( JOB, DICO, HINV, TRANA, UPLO, SCAL, SORT, FACT,
     $                   LYAPUN, N, A, LDA, T, LDT, V, LDV, G, LDG, Q,
     $                   LDQ, X, LDX, SEP, RCOND, FERR, WR, WI, S, LDS,
     $                   IWORK, DWORK, LDWORK, BWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve for X either the continuous-time algebraic Riccati
C     equation
C                                          -1
C        Q + op(A)'*X + X*op(A) - X*op(B)*R  op(B)'*X = 0,           (1)
C
C     or the discrete-time algebraic Riccati equation
C                                                                -1
C        X = op(A)'*X*op(A) - op(A)'*X*op(B)*(R + op(B)'*X*op(B))  *
C                             op(B)'*X*op(A) + Q,                    (2)
C
C     where op(M) = M or M' (M**T), A, op(B), Q, and R are N-by-N,
C     N-by-M, N-by-N, and M-by-M matrices respectively, with Q symmetric
C     and R symmetric nonsingular; X is an N-by-N symmetric matrix.
C                           -1
C     The matrix G = op(B)*R  *op(B)' must be provided on input, instead
C     of B and R, that is, the continuous-time equation
C
C        Q + op(A)'*X + X*op(A) - X*G*X = 0,                         (3)
C
C     or the discrete-time equation
C                                -1
C        Q + op(A)'*X*(I_n + G*X)  *op(A) - X = 0,                   (4)
C
C     are solved, where G is an N-by-N symmetric matrix. SLICOT Library
C     routine SB02MT should be used to compute G, given B and R. SB02MT
C     also enables to solve Riccati equations corresponding to optimal 
C     problems with coupling terms.
C
C     The routine also returns the computed values of the closed-loop
C     spectrum of the optimal system, i.e., the stable eigenvalues
C     lambda(1),...,lambda(N) of the corresponding Hamiltonian or
C     symplectic matrix associated to the optimal problem. It is assumed
C     that the matrices A, G, and Q are such that the associated
C     Hamiltonian or symplectic matrix has N stable eigenvalues, i.e.,
C     with negative real parts, in the continuous-time case, and with
C     moduli less than one, in the discrete-time case.
C
C     Optionally, estimates of the conditioning and error bound on the
C     solution of the Riccati equation (3) or (4) are returned.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     JOB     CHARACTER*1
C             Specifies the computation to be performed, as follows:
C             = 'X':  Compute the solution only;
C             = 'C':  Compute the reciprocal condition number only;
C             = 'E':  Compute the error bound only;
C             = 'A':  Compute all: the solution, reciprocal condition
C                     number, and the error bound.
C
C     DICO    CHARACTER*1
C             Specifies the type of Riccati equation to be solved or
C             analyzed, as follows:
C             = 'C':  Equation (3), continuous-time case;
C             = 'D':  Equation (4), discrete-time case.
C
C     HINV    CHARACTER*1
C             If DICO = 'D' and JOB = 'X' or JOB = 'A', specifies which
C             symplectic matrix is to be constructed, as follows:
C             = 'D':  The matrix H in (6) (see METHOD) is constructed;
C             = 'I':  The inverse of the matrix H in (6) is constructed.
C             HINV is not used if DICO = 'C', or JOB = 'C' or 'E'.
C
C     TRANA   CHARACTER*1
C             Specifies the form of op(A) to be used, as follows:
C             = 'N':  op(A) = A    (No transpose);
C             = 'T':  op(A) = A**T (Transpose);
C             = 'C':  op(A) = A**T (Conjugate transpose = Transpose).
C
C     UPLO    CHARACTER*1
C             Specifies which triangle of the matrices G and Q is
C             stored, as follows:
C             = 'U':  Upper triangle is stored;
C             = 'L':  Lower triangle is stored.
C
C     SCAL    CHARACTER*1
C             If JOB = 'X' or JOB = 'A', specifies whether or not a
C             scaling strategy should be used, as follows:
C             = 'G':  General scaling should be used;
C             = 'N':  No scaling should be used.
C             SCAL is not used if JOB = 'C' or 'E'.
C
C     SORT    CHARACTER*1
C             If JOB = 'X' or JOB = 'A', specifies which eigenvalues
C             should be obtained in the top of the Schur form, as
C             follows:
C             = 'S':  Stable   eigenvalues come first;
C             = 'U':  Unstable eigenvalues come first.
C             SORT is not used if JOB = 'C' or 'E'.
C
C     FACT    CHARACTER*1
C             If JOB <> 'X', specifies whether or not a real Schur
C             factorization of the closed-loop system matrix Ac is
C             supplied on entry, as follows:
C             = 'F':  On entry, T and V contain the factors from a real
C                     Schur factorization of the matrix Ac;
C             = 'N':  A Schur factorization of Ac will be computed
C                     and the factors will be stored in T and V.
C             For a continuous-time system, the matrix Ac is given by
C                Ac = A - G*X, if TRANA = 'N', or
C                Ac = A - X*G, if TRANA = 'T' or 'C',
C             and for a discrete-time system, the matrix Ac is given by
C                Ac = inv(I_n + G*X)*A, if TRANA = 'N', or
C                Ac = A*inv(I_n + X*G), if TRANA = 'T' or 'C'.
C             FACT is not used if JOB = 'X'.
C
C     LYAPUN  CHARACTER*1
C             If JOB <> 'X', specifies whether or not the original or
C             "reduced" Lyapunov equations should be solved for
C             estimating reciprocal condition number and/or the error
C             bound, as follows:
C             = 'O':  Solve the original Lyapunov equations, updating
C                     the right-hand sides and solutions with the
C                     matrix V, e.g., X <-- V'*X*V;
C             = 'R':  Solve reduced Lyapunov equations only, without
C                     updating the right-hand sides and solutions.
C                     This means that a real Schur form T of Ac appears
C                     in the equations, instead of Ac.
C             LYAPUN is not used if JOB = 'X'.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrices A, Q, G, and X.  N >= 0.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             If JOB = 'X' or JOB = 'A' or FACT = 'N' or LYAPUN = 'O',
C             the leading N-by-N part of this array must contain the
C             coefficient matrix A of the equation.
C             If JOB = 'C' or 'E' and FACT = 'F' and LYAPUN = 'R', A is
C             not referenced.
C
C     LDA     INTEGER
C             The leading dimension of the array A.
C             LDA >= MAX(1,N), if JOB  = 'X' or JOB = 'A' or
C                                 FACT = 'N' or LYAPUN = 'O'.
C             LDA >= 1,        otherwise.
C
C     T       (input or output) DOUBLE PRECISION array, dimension
C             (LDT,N)
C             If JOB <> 'X' and FACT = 'F', then T is an input argument
C             and on entry, the leading N-by-N upper Hessenberg part of
C             this array must contain the upper quasi-triangular matrix
C             T in Schur canonical form from a Schur factorization of Ac
C             (see argument FACT).
C             If JOB <> 'X' and FACT = 'N', then T is an output argument
C             and on exit, if INFO = 0 or INFO = 7, the leading N-by-N
C             upper Hessenberg part of this array contains the upper
C             quasi-triangular matrix T in Schur canonical form from a
C             Schur factorization of Ac (see argument FACT).
C             If JOB = 'X', the array T is not referenced.
C
C     LDT     INTEGER
C             The leading dimension of the array T.
C             LDT >= 1,        if JOB =  'X';
C             LDT >= MAX(1,N), if JOB <> 'X'.
C
C     V       (input or output) DOUBLE PRECISION array, dimension
C             (LDV,N)
C             If JOB <> 'X' and FACT = 'F', then V is an input argument
C             and on entry, the leading N-by-N part of this array must
C             contain the orthogonal matrix V from a real Schur
C             factorization of Ac (see argument FACT).
C             If JOB <> 'X' and FACT = 'N', then V is an output argument
C             and on exit, if INFO = 0 or INFO = 7, the leading N-by-N
C             part of this array contains the orthogonal N-by-N matrix
C             from a real Schur factorization of Ac (see argument FACT).
C             If JOB = 'X', the array V is not referenced.
C
C     LDV     INTEGER
C             The leading dimension of the array V.
C             LDV >= 1,        if JOB =  'X';
C             LDV >= MAX(1,N), if JOB <> 'X'.
C
C     G       (input/output) DOUBLE PRECISION array, dimension (LDG,N)
C             On entry, the leading N-by-N upper triangular part (if
C             UPLO = 'U') or lower triangular part (if UPLO = 'L') of
C             this array must contain the upper triangular part or lower
C             triangular part, respectively, of the symmetric matrix G.
C             On exit, if JOB = 'X' and DICO = 'D', or JOB <> 'X' and
C             LYAPUN = 'R', the leading N-by-N part of this array
C             contains the symmetric matrix G fully stored.
C             If JOB <> 'X' and LYAPUN = 'R', this array is modified
C             internally, but restored on exit.
C
C     LDG     INTEGER
C             The leading dimension of the array G.  LDG >= MAX(1,N).
C
C     Q       (input/output) DOUBLE PRECISION array, dimension (LDQ,N)
C             On entry, the leading N-by-N upper triangular part (if
C             UPLO = 'U') or lower triangular part (if UPLO = 'L') of
C             this array must contain the upper triangular part or lower
C             triangular part, respectively, of the symmetric matrix Q.
C             On exit, if JOB = 'X' and DICO = 'D', or JOB <> 'X' and
C             LYAPUN = 'R', the leading N-by-N part of this array
C             contains the symmetric matrix Q fully stored.
C             If JOB <> 'X' and LYAPUN = 'R', this array is modified
C             internally, but restored on exit.
C
C     LDQ     INTEGER
C             The leading dimension of the array Q.  LDQ >= MAX(1,N).
C
C     X       (input or output) DOUBLE PRECISION array, dimension
C             (LDX,N)
C             If JOB = 'C' or JOB = 'E', then X is an input argument
C             and on entry, the leading N-by-N part of this array must
C             contain the symmetric solution matrix of the algebraic
C             Riccati equation. If LYAPUN = 'R', this array is modified
C             internally, but restored on exit; however, it could differ
C             from the input matrix at the round-off error level.
C             If JOB = 'X' or JOB = 'A', then X is an output argument
C             and on exit, if INFO = 0 or INFO >= 6, the leading N-by-N
C             part of this array contains the symmetric solution matrix
C             X of the algebraic Riccati equation.
C
C     LDX     INTEGER
C             The leading dimension of the array X.  LDX >= MAX(1,N).
C
C     SEP     (output) DOUBLE PRECISION
C             If JOB = 'C' or JOB = 'A', and INFO = 0 or INFO = 7, the
C             estimated quantity
C                sep(op(Ac),-op(Ac)'), if DICO = 'C', or
C                sepd(op(Ac),op(Ac)'), if DICO = 'D'. (See METHOD.)
C             If N = 0, or X = 0, or JOB = 'X', or JOB = 'E', SEP is not
C             referenced.
C
C     RCOND   (output) DOUBLE PRECISION
C             If JOB = 'C' or JOB = 'A', and INFO = 0 or INFO = 7, an
C             estimate of the reciprocal condition number of the
C             algebraic Riccati equation.
C             If N = 0 or X = 0, RCOND is set to 1 or 0, respectively.
C             If JOB = 'X', or JOB = 'E', RCOND is not referenced.
C
C     FERR    (output) DOUBLE PRECISION
C             If JOB = 'E' or JOB = 'A', and INFO = 0 or INFO = 7, an
C             estimated forward error bound for the solution X. If XTRUE
C             is the true solution, FERR bounds the magnitude of the
C             largest entry in (X - XTRUE) divided by the magnitude of
C             the largest entry in X.
C             If N = 0 or X = 0, FERR is set to 0.
C             If JOB = 'X', or JOB = 'C', FERR is not referenced.
C
C     WR      (output) DOUBLE PRECISION array, dimension (2*N)
C     WI      (output) DOUBLE PRECISION array, dimension (2*N)
C             If JOB = 'X' or JOB = 'A', and INFO = 0 or INFO >= 5,
C             these arrays contain the real and imaginary parts,
C             respectively, of the eigenvalues of the 2N-by-2N matrix S,
C             ordered as specified by SORT (except for the case
C             HINV = 'D', when the order is opposite to that specified
C             by SORT). The leading N elements of these arrays contain
C             the closed-loop spectrum of the system matrix Ac (see
C             argument FACT). Specifically,
C                lambda(k) = WR(k) + j*WI(k), for k = 1,2,...,N.
C             If JOB = 'C' or JOB = 'E', these arrays are not
C             referenced.
C
C     S       (output) DOUBLE PRECISION array, dimension (LDS,2*N)
C             If JOB = 'X' or JOB = 'A', and INFO = 0 or INFO >= 5, the
C             leading 2N-by-2N part of this array contains the ordered
C             real Schur form S of the (scaled, if SCAL = 'G')
C             Hamiltonian or symplectic matrix H. That is,
C
C                    ( S    S   )
C                    (  11   12 )
C                S = (          ),
C                    ( 0    S   )
C                    (       22 )
C
C             where S  , S   and S   are N-by-N matrices.
C                    11   12      22
C             If JOB = 'C' or JOB = 'E', this array is not referenced.
C
C     LDS     INTEGER
C             The leading dimension of the array S.
C             LDS >= MAX(1,2*N), if JOB = 'X' or JOB = 'A';
C             LDS >= 1,          if JOB = 'C' or JOB = 'E'.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (LIWORK)
C             LIWORK >= 2*N,          if JOB = 'X';
C             LIWORK >= N*N,          if JOB = 'C' or JOB = 'E';
C             LIWORK >= MAX(2*N,N*N), if JOB = 'A'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, or INFO = 7, DWORK(1) returns the
C             optimal value of LDWORK. If INFO = 0, or INFO >= 5, and
C             JOB = 'X', or JOB = 'A', then DWORK(2) returns an estimate
C             RCONDU of the reciprocal of the condition number (in the
C             1-norm) of the N-th order system of algebraic equations
C             from which the solution matrix X is obtained, and DWORK(3)
C             returns the reciprocal pivot growth factor for the LU
C             factorization of the coefficient matrix of that system
C             (see SLICOT Library routine MB02PD); if DWORK(3) is much
C             less than 1, then the computed X and RCONDU could be
C             unreliable.
C             If DICO = 'D', and JOB = 'X', or JOB = 'A', then DWORK(4)
C             returns the reciprocal condition number RCONDA of the
C             given matrix A, and DWORK(5) returns the reciprocal pivot
C             growth factor for A or for its leading columns, if A is
C             singular (see SLICOT Library routine MB02PD); if DWORK(5)
C             is much less than 1, then the computed S and RCONDA could
C             be unreliable.
C             On exit, if INFO = 0, or INFO >= 4, and JOB = 'X', the
C             elements DWORK(6:5+4*N*N) contain the 2*N-by-2*N
C             transformation matrix  U  which reduced the Hamiltonian or
C             symplectic matrix  H  to the ordered real Schur form  S.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.  
C             LDWORK >= 5+MAX(1,4*N*N+8*N), if JOB = 'X' or JOB = 'A';
C             This may also be used for JOB = 'C' or JOB = 'E', but
C             exact bounds are as follows:
C             LDWORK >= 5 + MAX(1,LWS,LWE) + LWN, where
C             LWS = 0,       if FACT = 'F' or  LYAPUN = 'R';
C                 = 5*N,     if FACT = 'N' and LYAPUN = 'O' and
C                                              DICO = 'C' and JOB = 'C';
C                 = 5*N+N*N, if FACT = 'N' and LYAPUN = 'O' and
C                                              DICO = 'C' and JOB = 'E';
C                 = 5*N+N*N, if FACT = 'N' and LYAPUN = 'O' and
C                                              DICO = 'D';
C             LWE = 2*N*N,                if DICO = 'C' and JOB = 'C';
C                 = 4*N*N,                if DICO = 'C' and JOB = 'E';
C                 = MAX(3,2*N*N) + N*N,   if DICO = 'D' and JOB = 'C';
C                 = MAX(3,2*N*N) + 2*N*N, if DICO = 'D' and JOB = 'E';
C             LWN = 0,   if LYAPUN = 'O' or   JOB = 'C';
C                 = 2*N, if LYAPUN = 'R' and DICO = 'C' and JOB = 'E';
C                 = 3*N, if LYAPUN = 'R' and DICO = 'D' and JOB = 'E'.
C             For optimum performance LDWORK should sometimes be larger.
C
C     BWORK   LOGICAL array, dimension (LBWORK)
C             LBWORK >= 2*N,          if JOB = 'X' or JOB = 'A';
C             LBWORK >= 1,            if JOB = 'C' or JOB = 'E', and
C                                     FACT = 'N' and LYAPUN = 'R';
C             LBWORK >= 0,            otherwise.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  if matrix A is (numerically) singular in discrete-
C                   time case;
C             = 2:  if the Hamiltonian or symplectic matrix H cannot be
C                   reduced to real Schur form;
C             = 3:  if the real Schur form of the Hamiltonian or
C                   symplectic matrix H cannot be appropriately ordered;
C             = 4:  if the Hamiltonian or symplectic matrix H has less
C                   than N stable eigenvalues;
C             = 5:  if the N-th order system of linear algebraic
C                   equations, from which the solution matrix X would
C                   be obtained, is singular to working precision;
C             = 6:  if the QR algorithm failed to complete the reduction
C                   of the matrix Ac to Schur canonical form, T;
C             = 7:  if T and -T' have some almost equal eigenvalues, if
C                   DICO = 'C', or T has almost reciprocal eigenvalues,
C                   if DICO = 'D'; perturbed values were used to solve
C                   Lyapunov equations, but the matrix T, if given (for
C                   FACT = 'F'), is unchanged. (This is a warning
C                   indicator.)
C
C     METHOD
C
C     The method used is the Schur vector approach proposed by Laub [1],
C     but with an optional scaling, which enhances the numerical
C     stability [6]. It is assumed that [A,B] is a stabilizable pair
C     (where for (3) or (4), B is any matrix such that B*B' = G with
C     rank(B) = rank(G)), and [E,A] is a detectable pair, where E is any
C     matrix such that E*E' = Q with rank(E) = rank(Q). Under these
C     assumptions, any of the algebraic Riccati equations (1)-(4) is
C     known to have a unique non-negative definite solution. See [2].
C     Now consider the 2N-by-2N Hamiltonian or symplectic matrix
C
C                 ( op(A)   -G    )
C            H =  (               ),                                 (5)
C                 (  -Q   -op(A)' ),
C
C     for continuous-time equation, and
C                         -1              -1
C                 (  op(A)           op(A)  *G       )
C            H =  (        -1                   -1   ),              (6)
C                 ( Q*op(A)     op(A)' + Q*op(A)  *G )
C
C     for discrete-time equation, respectively, where
C                       -1
C            G = op(B)*R  *op(B)'.
C     The assumptions guarantee that H in (5) has no pure imaginary
C     eigenvalues, and H in (6) has no eigenvalues on the unit circle.
C     If Y is an N-by-N matrix then there exists an orthogonal matrix U
C     such that U'*Y*U is an upper quasi-triangular matrix. Moreover, U
C     can be chosen so that the 2-by-2 and 1-by-1 diagonal blocks
C     (corresponding to the complex conjugate eigenvalues and real
C     eigenvalues respectively) appear in any desired order. This is the
C     ordered real Schur form. Thus, we can find an orthogonal
C     similarity transformation U which puts (5) or (6) in ordered real
C     Schur form
C
C            U'*H*U = S = (S(1,1)  S(1,2))
C                         (  0     S(2,2))
C
C     where S(i,j) is an N-by-N matrix and the eigenvalues of S(1,1)
C     have negative real parts in case of (5), or moduli greater than
C     one in case of (6). If U is conformably partitioned into four
C     N-by-N blocks
C
C               U = (U(1,1)  U(1,2))
C                   (U(2,1)  U(2,2))
C
C     with respect to the assumptions we then have
C     (a) U(1,1) is invertible and X = U(2,1)*inv(U(1,1)) solves (1),
C         (2), (3), or (4) with X = X' and non-negative definite;
C     (b) the eigenvalues of S(1,1) (if DICO = 'C') or S(2,2) (if
C         DICO = 'D') are equal to the eigenvalues of optimal system
C         (the 'closed-loop' spectrum).
C
C     [A,B] is stabilizable if there exists a matrix F such that (A-BF)
C     is stable. [E,A] is detectable if [A',E'] is stabilizable.
C
C     The condition number of a Riccati equation is estimated as
C
C     cond = ( norm(Theta)*norm(A) + norm(inv(Omega))*norm(Q) +
C                 norm(Pi)*norm(G) ) / norm(X),
C
C     where Omega, Theta and Pi are linear operators defined by
C
C     Omega(W) = op(Ac)'*W + W*op(Ac),
C     Theta(W) = inv(Omega(op(W)'*X + X*op(W))),
C        Pi(W) = inv(Omega(X*W*X)),
C
C     in the continuous-time case, and
C
C     Omega(W) = op(Ac)'*W*op(Ac) - W,
C     Theta(W) = inv(Omega(op(W)'*X*op(Ac) + op(Ac)'X*op(W))),
C        Pi(W) = inv(Omega(op(Ac)'*X*W*X*op(Ac))),
C
C     in the discrete-time case, and Ac has been defined (see argument
C     FACT). Details are given in the comments of SLICOT Library
C     routines SB02QD and SB02SD.
C
C     The routine estimates the quantities
C
C     sep(op(Ac),-op(Ac)') = 1 / norm(inv(Omega)),
C     sepd(op(Ac),op(Ac)') = 1 / norm(inv(Omega)),
C
C     norm(Theta) and norm(Pi) using 1-norm condition estimator.
C
C     The forward error bound is estimated using a practical error bound
C     similar to the one proposed in [5].
C
C     REFERENCES
C
C     [1] Laub, A.J.
C         A Schur Method for Solving Algebraic Riccati equations.
C         IEEE Trans. Auto. Contr., AC-24, pp. 913-921, 1979.
C
C     [2] Wonham, W.M.
C         On a matrix Riccati equation of stochastic control.
C         SIAM J. Contr., 6, pp. 681-697, 1968.
C
C     [3] Sima, V.
C         Algorithms for Linear-Quadratic Optimization.
C         Pure and Applied Mathematics: A Series of Monographs and
C         Textbooks, vol. 200, Marcel Dekker, Inc., New York, 1996.
C
C     [4] Ghavimi, A.R. and Laub, A.J.
C         Backward error, sensitivity, and refinement of computed
C         solutions of algebraic Riccati equations.
C         Numerical Linear Algebra with Applications, vol. 2, pp. 29-49,
C         1995.
C
C     [5] Higham, N.J.
C         Perturbation theory and backward error for AX-XB=C.
C         BIT, vol. 33, pp. 124-136, 1993.
C
C     [6] Petkov, P.Hr., Konstantinov, M.M., and Mehrmann, V.
C         DGRSVX and DMSRIC: Fortran 77 subroutines for solving
C         continuous-time matrix algebraic Riccati equations with
C         condition and accuracy estimates.
C         Preprint SFB393/98-16, Fak. f. Mathematik, Tech. Univ.
C         Chemnitz, May 1998. 
C
C     NUMERICAL ASPECTS
C                               3
C     The algorithm requires 0(N ) operations. The solution accuracy
C     can be controlled by the output parameter FERR.
C
C     FURTHER COMMENTS
C
C     To obtain a stabilizing solution of the algebraic Riccati
C     equation for DICO = 'D', set SORT = 'U', if HINV = 'D', or set
C     SORT = 'S', if HINV = 'I'.
C
C     The routine can also compute the anti-stabilizing solutions of
C     the algebraic Riccati equations, by specifying 
C         SORT = 'U' if DICO = 'D' and HINV = 'I', or DICO = 'C', or 
C         SORT = 'S' if DICO = 'D' and HINV = 'D'.
C
C     Usually, the combinations HINV = 'D' and SORT = 'U', or HINV = 'I'
C     and SORT = 'U', for stabilizing and anti-stabilizing solutions,
C     respectively, will be faster then the other combinations [3].
C
C     The option LYAPUN = 'R' may produce slightly worse or better
C     estimates, and it is faster than the option 'O'.
C
C     This routine is a functionally extended and more accurate
C     version of the SLICOT Library routine SB02MD. Transposed problems
C     can be dealt with as well. Iterative refinement is used whenever
C     useful to solve linear algebraic systems. Condition numbers and
C     error bounds on the solutions are optionally provided.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Apr. 1999.
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
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, HALF, ONE
      PARAMETER         ( ZERO = 0.0D0, HALF = 0.5D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         DICO, FACT, HINV, JOB, LYAPUN, SCAL, SORT,
     $                  TRANA, UPLO
      INTEGER           INFO, LDA, LDG, LDQ, LDS, LDT, LDV, LDWORK, LDX,
     $                  N
      DOUBLE PRECISION  FERR, RCOND, SEP
C     .. Array Arguments ..
      LOGICAL           BWORK(*)
      INTEGER           IWORK(*)
      DOUBLE PRECISION  A(LDA,*), DWORK(*), G(LDG,*), Q(LDQ,*),
     $                  S(LDS,*), T(LDT,*), V(LDV,*), WI(*), WR(*),
     $                  X(LDX,*)
C     .. Local Scalars ..
      LOGICAL           COLEQU, DISCR, JBXA, JOBA, JOBC, JOBE, JOBX,
     $                  LHINV, LSCAL, LSCL, LSORT, LUPLO, NOFACT,
     $                  NOTRNA, ROWEQU, UPDATE
      CHARACTER         EQUED, JOBS, LOFACT, LOUP, TRANAT
      INTEGER           I, IERR, IU, IW, IWB, IWC, IWF, IWI, IWR, LDW,
     $                  LWE, LWN, LWS, N2, NN, NP1, NROT
      DOUBLE PRECISION  GNORM, QNORM, PIVOTA, PIVOTU, RCONDA, RCONDU,
     $                  WRKOPT
C     .. External Functions ..
      LOGICAL           LSAME, SB02MR, SB02MS, SB02MV, SB02MW
      DOUBLE PRECISION  DLAMCH, DLANGE, DLANSY
      EXTERNAL          DLAMCH, DLANGE, DLANSY, LSAME, SB02MR, SB02MS, 
     $                  SB02MV, SB02MW
C     .. External Subroutines ..
      EXTERNAL          DAXPY, DCOPY, DGEES, DGESV, DLACPY, DLASCL,
     $                  DLASET, DSCAL, DSWAP, DSYMM, MA02AD, MA02ED,
     $                  MB01RU, MB01SD, MB02PD, SB02QD, SB02RU, SB02SD,
     $                  XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         DBLE, MAX
C     .. Executable Statements ..
C
C     Decode the input parameters.
C
      N2  = N + N
      NN  = N*N
      NP1 = N + 1
      INFO = 0
      JOBA   = LSAME( JOB,    'A' )
      JOBC   = LSAME( JOB,    'C' )
      JOBE   = LSAME( JOB,    'E' )
      JOBX   = LSAME( JOB,    'X' )
      NOFACT = LSAME( FACT,   'N' )
      NOTRNA = LSAME( TRANA,  'N' )
      DISCR  = LSAME( DICO,   'D' )
      LUPLO  = LSAME( UPLO,   'U' )
      LSCAL  = LSAME( SCAL,   'G' )
      LSORT  = LSAME( SORT,   'S' )
      UPDATE = LSAME( LYAPUN, 'O' )
      JBXA   = JOBX .OR. JOBA
      LHINV  = .FALSE.
      IF ( DISCR .AND. JBXA )
     $   LHINV = LSAME( HINV, 'D' )
C
C     Test the input scalar arguments.
C
      IF( .NOT.( JBXA .OR. JOBC .OR. JOBE ) ) THEN
         INFO = -1           
      ELSE IF( .NOT.( DISCR .OR. LSAME( DICO, 'C' ) ) ) THEN
         INFO = -2
      ELSE IF( DISCR .AND. JBXA ) THEN
         IF( .NOT.( LHINV .OR. LSAME( HINV, 'I' ) ) )
     $      INFO = -3
      END IF
      IF( INFO.EQ.0 ) THEN
         IF( .NOT.( NOTRNA .OR. LSAME( TRANA, 'T' ) .OR.
     $                          LSAME( TRANA, 'C' ) ) ) THEN
            INFO = -4
         ELSE IF( .NOT.( LUPLO .OR. LSAME( UPLO, 'L' ) ) )
     $      THEN
            INFO = -5
         ELSE IF( JBXA ) THEN
            IF( .NOT.( LSCAL .OR. LSAME( SCAL, 'N' ) ) ) THEN
               INFO = -6
            ELSE IF( .NOT.( LSORT .OR. LSAME( SORT, 'U' ) ) ) THEN
               INFO = -7
            END IF
         END IF
         IF( INFO.EQ.0 .AND. .NOT.JOBX ) THEN
            IF( .NOT.( NOFACT .OR. LSAME( FACT, 'F' ) ) ) THEN
               INFO = -8
            ELSE IF( .NOT.( UPDATE .OR. LSAME( LYAPUN, 'R' ) ) ) THEN
               INFO = -9
            END IF
         END IF
         IF( INFO.EQ.0 ) THEN
            IF( N.LT.0 ) THEN
               INFO = -10
            ELSE IF( LDA.LT.1 .OR. ( ( JBXA .OR. NOFACT .OR. UPDATE )
     $         .AND. LDA.LT.N ) ) THEN
               INFO = -12
            ELSE IF( LDT.LT.1 .OR. ( .NOT. JOBX .AND. LDT.LT.N ) ) THEN
               INFO = -14
            ELSE IF( LDV.LT.1 .OR. ( .NOT. JOBX .AND. LDV.LT.N ) ) THEN
               INFO = -16
            ELSE IF( LDG.LT.MAX( 1, N ) ) THEN
               INFO = -18
            ELSE IF( LDQ.LT.MAX( 1, N ) ) THEN
               INFO = -20
            ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
               INFO = -22
            ELSE IF( LDS.LT.1 .OR. ( JBXA .AND. LDS.LT.N2 ) ) THEN
               INFO = -29
            ELSE
               IF( JBXA ) THEN
                  IF( LDWORK.LT.5 + MAX( 1, 4*NN + 8*N ) )
     $               INFO = -32
               ELSE
                  IF( NOFACT .AND. UPDATE ) THEN
                     IF( .NOT.DISCR .AND. JOBC ) THEN
                        LWS = 5*N
                     ELSE
                        LWS = 5*N + NN
                     END IF
                  ELSE
                     LWS = 0
                  END IF
                  IF( DISCR ) THEN
                     IF( JOBC ) THEN
                        LWE = MAX( 3, 2*NN) + NN
                     ELSE
                        LWE = MAX( 3, 2*NN) + 2*NN
                     END IF
                  ELSE
                     IF( JOBC ) THEN
                        LWE = 2*NN
                     ELSE
                        LWE = 4*NN
                     END IF
                  END IF
                  IF( UPDATE .OR. JOBC ) THEN
                     LWN = 0
                  ELSE
                     IF( DISCR ) THEN
                        LWN = 3*N
                     ELSE
                        LWN = 2*N
                     END IF
                  END IF
                  IF( LDWORK.LT.5 + MAX( 1, LWS, LWE ) + LWN )
     $               INFO = -32
               END IF
            END IF
         END IF
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'SB02RD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF ( N.EQ.0 ) THEN
         IF( JOBC .OR. JOBA )
     $      RCOND = ONE
         IF( JOBE .OR. JOBA )
     $      FERR  = ZERO
         DWORK(1) = ONE
         DWORK(2) = ONE
         DWORK(3) = ONE
         IF ( DISCR ) THEN
            DWORK(4) = ONE
            DWORK(5) = ONE
         END IF
         RETURN
      END IF
C
      IF ( JBXA ) THEN
C
C        Compute the solution matrix X.
C
C        Initialise the Hamiltonian or symplectic matrix associated with
C        the problem.
C        Workspace:  need   0    if DICO = 'C';
C                           6*N, if DICO = 'D'.
C
         CALL SB02RU( DICO, HINV, TRANA, UPLO, N, A, LDA, G, LDG, Q,
     $                LDQ, S, LDS, IWORK, DWORK, LDWORK, IERR )
C
         IF ( IERR.NE.0 ) THEN
            INFO = 1
            IF ( DISCR ) THEN
               DWORK(4) = DWORK(1)
               DWORK(5) = DWORK(2)
            END IF
            RETURN
         END IF
C
         IF ( DISCR ) THEN
            WRKOPT = 6*N
            RCONDA = DWORK(1)
            PIVOTA = DWORK(2)
         ELSE
            WRKOPT = 0
         END IF
C
         IF ( LSCAL ) THEN
C
C           Scale the Hamiltonian or symplectic matrix S, using the
C           square roots of the norms of the matrices Q and G.
C
            QNORM = SQRT( DLANSY( '1-norm', UPLO, N, Q, LDQ, DWORK ) )
            GNORM = SQRT( DLANSY( '1-norm', UPLO, N, G, LDG, DWORK ) )
C
            LSCL = QNORM.GT.GNORM .AND. GNORM.GT.ZERO
            IF( LSCL ) THEN
               CALL DLASCL( 'G', 0, 0, QNORM, GNORM, N, N, S(NP1,1),
     $                      LDS, IERR )
               CALL DLASCL( 'G', 0, 0, GNORM, QNORM, N, N, S(1,NP1),
     $                      LDS, IERR )
            END IF
         END IF
C
C        Find the ordered Schur factorization of S,  S = U*H*U'.
C        Workspace:  need   5 + 4*N*N + 6*N;
C                    prefer larger.
C
         IU  = 6
         IW  = IU + 4*NN
         LDW = LDWORK - IW + 1
         IF ( .NOT.DISCR ) THEN
            IF ( LSORT ) THEN
               CALL DGEES( 'Vectors', 'Sorted', SB02MV, N2, S, LDS,
     $                     NROT, WR, WI, DWORK(IU), N2, DWORK(IW), LDW,
     $                     BWORK, IERR )
            ELSE
               CALL DGEES( 'Vectors', 'Sorted', SB02MR, N2, S, LDS,
     $                     NROT, WR, WI, DWORK(IU), N2, DWORK(IW), LDW,
     $                     BWORK, IERR )
            END IF
         ELSE
            IF ( LSORT ) THEN
               CALL DGEES( 'Vectors', 'Sorted', SB02MW, N2, S, LDS,
     $                     NROT, WR, WI, DWORK(IU), N2, DWORK(IW), LDW,
     $                     BWORK, IERR )
            ELSE
               CALL DGEES( 'Vectors', 'Sorted', SB02MS, N2, S, LDS,
     $                     NROT, WR, WI, DWORK(IU), N2, DWORK(IW), LDW,
     $                     BWORK, IERR )
            END IF
            IF ( LHINV ) THEN
               CALL DSWAP( N, WR, 1, WR(NP1), 1 )
               CALL DSWAP( N, WI, 1, WI(NP1), 1 )
            END IF
         END IF
         IF ( IERR.GT.N2 ) THEN
            INFO = 3
         ELSE IF ( IERR.GT.0 ) THEN
            INFO = 2
         ELSE IF ( NROT.NE.N ) THEN
            INFO = 4
         END IF
         IF ( INFO.NE.0 ) THEN
            IF ( DISCR ) THEN
               DWORK(4) = RCONDA
               DWORK(5) = PIVOTA
            END IF
            RETURN
         END IF
C
         WRKOPT = MAX( WRKOPT, DWORK(IW) + DBLE( IW - 1 ) )
C
C        Compute the solution of X*U(1,1) = U(2,1) using
C        LU factorization and iterative refinement. The (2,1) block of S
C        is used as a workspace for factoring U(1,1).
C        Workspace:  need   5 + 4*N*N + 8*N.
C
C        First transpose U(2,1) in-situ.
C
         DO 20 I = 1, N - 1
            CALL DSWAP( N-I, DWORK(IU+N+I*(N2+1)-1), N2,
     $                  DWORK(IU+N+(I-1)*(N2+1)+1), 1 )
   20    CONTINUE
C
         IWR = IW
         IWC = IWR + N
         IWF = IWC + N
         IWB = IWF + N
         IW  = IWB + N
C
         CALL MB02PD( 'Equilibrate', 'Transpose', N, N, DWORK(IU), N2,
     $                S(NP1,1), LDS, IWORK, EQUED, DWORK(IWR),
     $                DWORK(IWC), DWORK(IU+N), N2, X, LDX, RCONDU,
     $                DWORK(IWF), DWORK(IWB), IWORK(NP1), DWORK(IW),
     $                IERR )
         IF( JOBX ) THEN
C
C           Restore U(2,1) back in-situ.
C
            DO 40 I = 1, N - 1
               CALL DSWAP( N-I, DWORK(IU+N+I*(N2+1)-1), N2,
     $                     DWORK(IU+N+(I-1)*(N2+1)+1), 1 )
   40       CONTINUE
C
            IF( .NOT.LSAME( EQUED, 'N' ) ) THEN
C
C              Undo the equilibration of U(1,1) and U(2,1).
C
               ROWEQU = LSAME( EQUED, 'R' ) .OR. LSAME( EQUED, 'B' )
               COLEQU = LSAME( EQUED, 'C' ) .OR. LSAME( EQUED, 'B' )
C
               IF( ROWEQU ) THEN
C
                  DO 60 I = 1, N
                     DWORK(IWR+I-1) = ONE / DWORK(IWR+I-1)
   60             CONTINUE
C
                  CALL MB01SD( 'Row scaling', N, N, DWORK(IU), N2,
     $                         DWORK(IWR), DWORK(IWC) )
               END IF
C
               IF( COLEQU ) THEN
C
                  DO 80 I = 1, N
                     DWORK(IWC+I-1) = ONE / DWORK(IWC+I-1)
   80             CONTINUE
C
                  CALL MB01SD( 'Column scaling', N, N, DWORK(IU), N2,
     $                         DWORK(IWR), DWORK(IWC) )
                  CALL MB01SD( 'Column scaling', N, N, DWORK(IU+N), N2,
     $                         DWORK(IWR), DWORK(IWC) )
               END IF
            END IF
C
C           Set S(2,1) to zero.
C
            CALL DLASET( 'Full', N, N, ZERO, ZERO, S(NP1,1), LDS )
         END IF
C
         PIVOTU = DWORK(IW)
C
         IF ( IERR.GT.0 ) THEN
C
C           Singular matrix. Set INFO and DWORK for error return.
C
            INFO = 5
            GO TO 160
         END IF
C
C        Make sure the solution matrix X is symmetric.
C
         DO 100 I = 1, N - 1
            CALL DAXPY( N-I, ONE, X(I,I+1), LDX, X(I+1,I), 1 )
            CALL DSCAL( N-I, HALF, X(I+1,I), 1 )
            CALL DCOPY( N-I, X(I+1,I), 1, X(I,I+1), LDX )
  100    CONTINUE
C
         IF( LSCAL ) THEN
C
C           Undo scaling for the solution matrix.
C
            IF( LSCL )
     $         CALL DLASCL( 'G', 0, 0, GNORM, QNORM, N, N, X, LDX,
     $                      IERR )
         END IF
      END IF
C
      IF ( .NOT.JOBX ) THEN
C
C        Estimate the conditioning and compute an error bound on the
C        solution of the algebraic Riccati equation.
C
         IW = 6
         LOFACT = FACT
         IF ( NOFACT .AND. .NOT.UPDATE ) THEN
C
C           Compute Ac and its Schur factorization.
C
            IF ( DISCR ) THEN
               CALL DLASET( 'Full', N, N, ZERO, ONE, DWORK(IW), N )
               CALL DSYMM(  'Left', UPLO, N, N, ONE, G, LDG, X, LDX,
     $                      ONE, DWORK(IW), N )
               IF ( NOTRNA ) THEN
C
C                 Compute Ac = inv(I_n + G*X)*A.
C
                  CALL DLACPY( 'Full', N, N, A, LDA, T, LDT )
                  CALL DGESV( N, N, DWORK(IW), N, IWORK, T, LDT, IERR )
               ELSE
C
C                 Compute Ac = A*inv(I_n + X*G).
C
                  CALL MA02AD( 'Full', N, N, A, LDA, T, LDT )
                  CALL DGESV( N, N, DWORK(IW), N, IWORK, T, LDT, IERR )
                  DO 120 I = 2, N
                     CALL DSWAP( I-1, T(1,I), 1, T(I,1), LDT )
  120             CONTINUE
               END IF
C
            ELSE
C
               CALL DLACPY( 'Full', N, N, A, LDA, T, LDT )
               IF ( NOTRNA ) THEN
C
C                 Compute Ac = A - G*X.
C
                  CALL DSYMM( 'Left', UPLO, N, N, -ONE, G, LDG, X, LDX,
     $                        ONE, T, LDT )
               ELSE
C
C                 Compute Ac = A - X*G.
C
                  CALL DSYMM( 'Right', UPLO, N, N, -ONE, G, LDG, X, LDX,
     $                        ONE, T, LDT )
               END IF
            END IF
C
C           Compute the Schur factorization of Ac, Ac = V*T*V'.
C           Workspace:  need   5 + 5*N.
C                       prefer larger.
C
            IWR = IW
            IWI = IWR + N
            IW  = IWI + N
            LDW = LDWORK - IW + 1
C
            CALL DGEES( 'Vectors', 'Not ordered', SB02MS, N, T, LDT,
     $                  NROT, DWORK(IWR), DWORK(IWI), V, LDV, DWORK(IW),
     $                  LDW, BWORK, IERR )
C
            IF( IERR.NE.0 ) THEN
               INFO = 6
               GO TO 160
            END IF
C
            WRKOPT = MAX( WRKOPT, DWORK(IW) + DBLE( IW - 1 ) )
            LOFACT = 'F'
            IW = 6
         END IF
C
         IF ( .NOT.UPDATE ) THEN
C
C           Update G, Q, and X using the orthogonal matrix V.
C
            TRANAT = 'T'
C
C           Save the diagonal elements of G and Q.
C
            CALL DCOPY( N, G, LDG+1, DWORK(IW), 1 )
            CALL DCOPY( N, Q, LDQ+1, DWORK(IW+N), 1 )
            IW = IW + N2
C
            IF ( JOBA )
     $         CALL DLACPY( 'Full', N, N, X, LDX, S(NP1,1), LDS )
            CALL MB01RU( UPLO, TRANAT, N, N, ZERO, ONE, X, LDX, V, LDV,
     $                   X, LDX, DWORK(IW), NN, IERR )
            CALL MA02ED( UPLO, N, X, LDX )
            IF( .NOT.DISCR ) THEN
               CALL MA02ED( UPLO, N, G, LDG )
               CALL MA02ED( UPLO, N, Q, LDQ )
            END IF
            CALL MB01RU( UPLO, TRANAT, N, N, ZERO, ONE, G, LDG, V, LDV,
     $                   G, LDG, DWORK(IW), NN, IERR )
            CALL MB01RU( UPLO, TRANAT, N, N, ZERO, ONE, Q, LDQ, V, LDV,
     $                   Q, LDQ, DWORK(IW), NN, IERR )
         END IF
C
C        Estimate the conditioning and/or the error bound.
C        Workspace: 5 + MAX(1,LWS,LWE) + LWN, where
C
C           LWS = 0,       if FACT = 'F' or  LYAPUN = 'R';
C               = 5*N,     if FACT = 'N' and LYAPUN = 'O' and DICO = 'C'
C                                                         and JOB = 'C';
C               = 5*N+N*N, if FACT = 'N' and LYAPUN = 'O' and DICO = 'C'
C                                          and (JOB = 'E' or JOB = 'A');
C               = 5*N+N*N, if FACT = 'N' and LYAPUN = 'O' and 
C                                                         DICO = 'D';
C           LWE = 2*N*N,                if DICO = 'C' and  JOB = 'C';
C               = 4*N*N,                if DICO = 'C' and (JOB = 'E' or
C                                                          JOB = 'A');
C               = MAX(3,2*N*N) + N*N,   if DICO = 'D' and  JOB = 'C';
C               = MAX(3,2*N*N) + 2*N*N, if DICO = 'D' and (JOB = 'E' or
C                                                          JOB = 'A');
C           LWN = 0,   if LYAPUN = 'O' or   JOB = 'C';
C               = 2*N, if LYAPUN = 'R' and DICO = 'C' and (JOB = 'E' or
C                                                          JOB = 'A');
C               = 3*N, if LYAPUN = 'R' and DICO = 'D' and (JOB = 'E' or
C                                                          JOB = 'A').
C
         LDW = LDWORK - IW + 1
         IF ( JOBA ) THEN
            JOBS = 'B'
         ELSE
            JOBS = JOB
         END IF
C
         IF ( DISCR ) THEN
            CALL SB02SD( JOBS, LOFACT, TRANA, UPLO, LYAPUN, N, A, LDA,
     $                   T, LDT, V, LDV, G, LDG, Q, LDQ, X, LDX, SEP,
     $                   RCOND, FERR, IWORK, DWORK(IW), LDW, IERR )
         ELSE
            CALL SB02QD( JOBS, LOFACT, TRANA, UPLO, LYAPUN, N, A, LDA,
     $                   T, LDT, V, LDV, G, LDG, Q, LDQ, X, LDX, SEP,
     $                   RCOND, FERR, IWORK, DWORK(IW), LDW, IERR )
         END IF
C
         WRKOPT = MAX( WRKOPT, DWORK(IW) + DBLE( IW - 1 ) )
         IF( IERR.EQ.NP1 ) THEN
            INFO = 7
         ELSE IF( IERR.GT.0 ) THEN
            INFO = 6
            GO TO 160
         END IF
C
         IF ( .NOT.UPDATE ) THEN
C
C           Restore X, G, and Q and set S(2,1) to zero, if needed.
C
            IF ( JOBA ) THEN
               CALL DLACPY( 'Full', N, N, S(NP1,1), LDS, X, LDX )
               CALL DLASET( 'Full', N, N, ZERO, ZERO, S(NP1,1), LDS )
            ELSE
               CALL MB01RU( UPLO, TRANA, N, N, ZERO, ONE, X, LDX, V,
     $                      LDV, X, LDX, DWORK(IW), NN, IERR )
               CALL MA02ED( UPLO, N, X, LDX )
            END IF
            IF ( LUPLO ) THEN
               LOUP = 'L'
            ELSE
               LOUP = 'U'
            END IF
C
            IW = 6
            CALL DCOPY( N, DWORK(IW), 1, G, LDG+1 )
            CALL MA02ED( LOUP, N, G, LDG )
            CALL DCOPY( N, DWORK(IW+N), 1, Q, LDQ+1 )
            CALL MA02ED( LOUP, N, Q, LDQ )
         END IF
C
      END IF
C
C     Set the optimal workspace and other details.
C
      DWORK(1) = WRKOPT
  160 CONTINUE
      IF( JBXA ) THEN
         DWORK(2) = RCONDU
         DWORK(3) = PIVOTU
         IF ( DISCR ) THEN
            DWORK(4) = RCONDA
            DWORK(5) = PIVOTA
         END IF
      END IF
C
      RETURN
C *** Last line of SB02RD ***
      END
