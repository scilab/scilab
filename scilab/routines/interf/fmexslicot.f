C FINDBD.F - Gateway function for computing the initial state and/or
C            the matrices B and D of a discrete-time linear system, 
C            given the system matrices A, C, and possibly B, D, and the
C            input and output trajectories, using SLICOT routine IB01CD.
C
C RELEASE 4.0, WGS COPYRIGHT 2000.
C
C Matlab call:  
C   [(x0)(,B(,D))(,V)(,rcnd)] = findBD(jobx0,comuse(,job),A(,B),C(,D),Y
C                                      (,U,tol,printw,ldwork))
C
C     [x0,B,V,rcnd] = findBD(1,1,1,A,C,Y,U)
C   [x0,B,D,V,rcnd] = findBD(1,1,2,A,C,Y,U)
C        [B,V,rcnd] = findBD(0,1,1,A,C,Y,U)
C      [B,D,V,rcnd] = findBD(0,1,2,A,C,Y,U)
C       [x0,V,rcnd] = findBD(1,2,1,A,B,C,Y,U)
C       [x0,V,rcnd] = findBD(1,2,2,A,B,C,D,Y,U)
C         [x0,rcnd] = findBD(0,2)                  (x0 = 0, rcnd = 1)
C       [x0,V,rcnd] = findBD(1,3,A,C,Y)
C  
C Purpose:
C   To estimate the initial state and/or the matrices B and D of a
C   discrete-time system, given the system matrices A, C, and possibly
C   B, D, and the input and output trajectories of the system.
C
C Input parameters: 
C   jobx0 - integer option to specify whether or not the initial state 
C           should be computed:
C           = 1 : compute the initial state x(0);
C           = 0 : do not compute the initial state (possibly, because
C                 x(0) is known to be zero).
C   comuse- integer option to specify whether the system matrices B
C           and D should be computed or used:
C           = 1 : compute the matrices B and D, as specified by job;
C           = 2 : use the matrices B and D, as specified by job;
C           = 3 : do not compute/use the matrices B and D.
C   job   - integer option to determine which of the system matrices
C           B and D should be computed or used:
C           = 1 : compute/use the matrix B only (D is known to be zero);
C           = 2 : compute/use the matrices B and D.
C           job must not be specified if jobx0 = 0 and comuse = 2, or
C           if comuse = 3.
C   A     - the n-by-n system state matrix A.
C   B     - the n-by-m system input matrix B (input when jobx0 = 1 and
C           comuse = 2).
C   C     - the l-by-n system output matrix C.
C   D     - the l-by-m system matrix D (input when jobx0 = 1,
C        comuse = 2 
C           and job = 2).
C   Y     - the t-by-l output-data sequence matrix.  Column  j  of  Y 
C           contains the  t  values of the j-th output component for
C           consecutive time increments.
C   U     - the t-by-m input-data sequence matrix (input when jobx0 = 1 
C           and comuse = 2, or comuse = 1).  Column  j  of  U 
C           contains the  t  values of the j-th input component for
C           consecutive time increments.
C   tol   - (optional) tolerance used for estimating the rank of
C           matrices. If  tol > 0,  then the given value of  tol  is
C           used as a lower bound for the reciprocal condition number;
C           an m-by-n matrix whose estimated condition number is less
C           than  1/tol  is considered to be of full rank.
C           Default: m*n*epsilon_machine where epsilon_machine is the
C           relative machine precision.
C   printw- (optional) switch for printing the warning messages.
C           = 1:  print warning messages;
C           = 0:  do not print warning messages.
C           Default:    printw = 0.
C   ldwork- (optional) the workspace size.
C           Default : computed by the formula
C           LDWORK = MAX( minimum workspace size needed, 2*CSIZE/3,
C                        CSIZE - ( m + l )*t - 2*n*( n + m + l ) - l*m )
C           where CSIZE is the cache size in double precision words.
C
C Output parameters:
C   x0    - the n-vector of estimated initial state x(0) (output when
C           jobx0 = 1, and set to 0 when jobx0 = 0 and comuse = 2).
C   B     - the n-by-m system input matrix B (output when comuse = 1).
C   D     - the l-by-m system matrix D (output when comuse = 1 and 
C           job = 2).
C   V     - the n-by-n orthogonal matrix which reduces A to a real Schur
C           form (output when jobx0 = 1 or comuse = 1).
C   rcnd  - (optional) vector of length 1 (or 2, if comuse = 1 and 
C           job = 2), containing the reciprocal condition numbers of
C           the matrices involved in rank decisions.
C
C Contributor:
C   V. Sima, Research Institute for Informatics, Bucharest, April 2000.
C
C Revisions:
C   V. Sima, July 2000, February 2001.
C
C **********************************************************************
C
      SUBROUTINE findbd( NLHS, PLHS, NRHS, PRHS )
C     ADDED FOR SCILAB ==========================
      INCLUDE '../stack.h'
      LOGICAL CREATEVAR
C     ===========================================
C
C For Windows only!
C This is the library needed to set the floating point 
C exception word and it needs to be here in the code.  
C  
C     USE DFLIB
C
C .. Parameters ..
      DOUBLE PRECISION ZERO 
      PARAMETER (ZERO =0.0D0)
C
C .. Mex-file interface parameters ..
      INTEGER PLHS (*),PRHS (*)
      INTEGER NLHS ,NRHS 
C
C .. Mex-file integer functions ..
      INTEGER MXCREATEFULL ,MXGETPR 
      INTEGER MXGETM ,MXGETN ,MXISNUMERIC ,MXISCOMPLEX 
C
C .. Scalar parameters used by SLICOT subroutines ..
      DOUBLE PRECISION TOL 
      INTEGER INFO ,IWARN ,L ,LDA ,LDB ,LDC ,LDD ,LDU ,LDV ,LDWORK ,
     $   LDY ,M ,N ,NSMP 
      CHARACTER COMUSE ,JOB ,JOBX0 
C
C .. Allocatable arrays ..
C !Fortran 90/95 (Fixed dimensions should be used with Fortran 77.)
      INTEGER IWORK
      INTEGER A, B, C, D, DWORK, U, V, X0, Y
C
C .. Local variables and constant dimension arrays ..
      DOUBLE PRECISION TEMP 
      CHARACTER *120TEXT 
      INTEGER CSIZE ,CUSE ,IC ,IJOB ,INI ,IP ,IPS ,IQ ,ISIZE ,ITMP ,
     $   LDW1 ,LDW2 ,LDW3 ,LDWMIN ,LIWORK ,MA ,MINSMP ,MINWLS ,NA ,
     $   NCOL ,NCP1 ,TASK 
      LOGICAL PRINTW 
C
C .. Cache size in double precision words ..
      PARAMETER (CSIZE =64000)
C
C .. External subroutines ..
      EXTERNAL IB01CD 
C
C ..Intrinsic functions..
      INTRINSIC INT ,MAX ,MIN 
C
C For Windows only!
C This resets the floating point exception to allow divide by zero,
C overflow and invalid numbers. 
C
C     INTEGER(2)        CONTROL
C
C     CALL GETCONTROLFPQQ( CONTROL )
C     CONTROL = CONTROL .OR. FPCW$ZERODIVIDE
C     CONTROL = CONTROL .OR. FPCW$INVALID
C     CONTROL = CONTROL .OR. FPCW$OVERFLOW
C     CALL SETCONTROLFPQQ( CONTROL )
C
C Check for proper number of arguments.
C
      IF (NRHS.LT.2 )THEN 
         CALL MEXERRMSGTXT (
     $      'FINDBD REQUIRES AT LEAST 2 INPUT ARGUMENTS')
      ELSE IF (NLHS.LT.1 )THEN 
         CALL MEXERRMSGTXT (
     $      'FINDBD REQUIRES AT LEAST 1 OUTPUT ARGUMENTS')
      END IF 
C
C Check dimensions of input parameters and read/set scalar parameters.
C
C   jobx0
C
      IF (MXGETM (PRHS (1)).NE.1.OR. MXGETN (PRHS (1)).NE.1)THEN 
         CALL MEXERRMSGTXT ('JOBX0 MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (1)).EQ.0.OR.MXISCOMPLEX (PRHS (1)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('JOBX0 MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (1)),TEMP ,1)
      TASK =TEMP 
      IF (TASK.LT.0 .OR.TASK.GT.1 )THEN 
         CALL MEXERRMSGTXT (
     $      'JOBX0 HAS 0 OR 1 THE ONLY ADMISSIBLE VALUES')
      END IF 
C
      IF (TASK.EQ.1 )THEN 
         JOBX0 ='X'
      ELSE 
         JOBX0 ='N'
      END IF 
C
C   comuse
C
      IF (MXGETM (PRHS (2)).NE.1.OR. MXGETN (PRHS (2)).NE.1)THEN 
         CALL MEXERRMSGTXT ('COMUSE MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (2)).EQ.0.OR.MXISCOMPLEX (PRHS (2)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('COMUSE MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (2)),TEMP ,1)
      CUSE =TEMP 
      IF (CUSE.LT.1 .OR.CUSE.GT.3 )THEN 
         CALL MEXERRMSGTXT ('
     $      COMUSE HAS 1, 2, OR 3 THE ONLY ADMISSIBLE VALUES')
      END IF 
C
      IF (CUSE.EQ.1 )THEN 
         COMUSE ='C'
      ELSE IF (CUSE.EQ.2 )THEN 
         COMUSE ='U'
      ELSE 
         COMUSE ='N'
      END IF 
C
      IF (TASK.EQ.1 .AND.CUSE.EQ.3 )THEN 
         IF (NRHS.LT.5 )THEN 
            CALL MEXERRMSGTXT (
     $         'FINDBD REQUIRES AT LEAST 5 INPUT ARGUMENTS')
         END IF 
      ELSE IF (CUSE.EQ.1 )THEN 
         IF (NRHS.LT.6 )THEN 
            CALL MEXERRMSGTXT (
     $         'FINDBD REQUIRES AT LEAST 6 INPUT ARGUMENTS')
         END IF 
      END IF 
C
C   job
C
      IF ((TASK.EQ.1 .AND.CUSE.EQ.2 ).OR.CUSE.EQ.1 )THEN 
         IF (MXGETM (PRHS (3)).NE.1.OR. MXGETN (PRHS (3)).NE.1)THEN 
            CALL MEXERRMSGTXT ('JOB MUST BE A SCALAR')
         END IF 
         IF (MXISNUMERIC (PRHS (3)).EQ.0.OR.MXISCOMPLEX (PRHS (3)).EQ.1)
     $      THEN 
            CALL MEXERRMSGTXT ('JOB MUST BE AN INTEGER SCALAR')
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (3)),TEMP ,1)
         IJOB =TEMP 
         IF (IJOB.LT.1 .OR.IJOB.GT.2 )THEN 
            CALL MEXERRMSGTXT (
     $         'JOB HAS 1 OR 2 THE ONLY ADMISSIBLE VALUES')
         END IF 
         IP =4
C
         IF (IJOB.EQ.1 )THEN 
            JOB ='B'
         ELSE 
            JOB ='D'
         END IF 
      ELSE 
         IP =3
         JOB ='B'
         IJOB =1
      END IF 
      IF (TASK.EQ.1 .AND.CUSE.EQ.2 )THEN 
         IF (IJOB.EQ.1 .AND.NRHS.LT.7 )THEN 
            CALL MEXERRMSGTXT (
     $         'FINDBD REQUIRES AT LEAST 7 INPUT ARGUMENTS')
         ELSE IF (IJOB.EQ.2 .AND.NRHS.LT.8 )THEN 
            CALL MEXERRMSGTXT (
     $         'FINDBD REQUIRES AT LEAST 8 INPUT ARGUMENTS')
         END IF 
      END IF 
c
      IPS =IP 
      N =0
      M =0
      L =1
      NSMP =1
C
C   A(n,n)
C
      IF (NRHS.GE.IP )THEN 
         N =MXGETM (PRHS (IP ))
         NA =MXGETN (PRHS (IP ))
         IF (NA.NE.N )THEN 
            WRITE (TEXT ,'(''A MUST HAVE '',I6,'' ROWS AND COLUMNS'')')
     $         N 
            CALL MEXERRMSGTXT (TEXT )
         END IF 
         IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP ))
     $      .EQ.1)THEN 
            CALL MEXERRMSGTXT ('A MUST BE A REAL MATRIX')
         END IF 
         IP =IP +1
      END IF 
C
C   B(n,m)
C
      IF (TASK.EQ.1 .AND.CUSE.EQ.2 .AND.NRHS.GE.IP )THEN 
         NA =MXGETM (PRHS (IP ))
         M =MXGETN (PRHS (IP ))
         IF (NA.NE.N )THEN 
            WRITE (TEXT ,'(''B MUST HAVE '',I6,'' ROWS'')')N 
            CALL MEXERRMSGTXT (TEXT )
         END IF 
         IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP ))
     $      .EQ.1)THEN 
            CALL MEXERRMSGTXT ('B MUST BE A REAL MATRIX')
         END IF 
         IP =IP +1
      END IF 
C
C   C(l,n)
C
      IF (NRHS.GE.IP )THEN 
         L =MXGETM (PRHS (IP ))
         NA =MXGETN (PRHS (IP ))
         IF (NA.NE.N )THEN 
            WRITE (TEXT ,'(''C MUST HAVE '',I6,'' COLUMNS'')')N 
            CALL MEXERRMSGTXT (TEXT )
         END IF 
         IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP ))
     $      .EQ.1)THEN 
            CALL MEXERRMSGTXT ('C MUST BE A REAL MATRIX')
         END IF 
         IP =IP +1
C
         IF (L.LE.0 )THEN 
            CALL MEXERRMSGTXT ('THE SYSTEM HAS NO OUTPUTS')
         END IF 
      END IF 
C
C   D(l,m)
C
      IF (TASK.EQ.1 .AND.CUSE.EQ.2 .AND.IJOB.EQ.2 .AND.NRHS.GE.IP )THEN 
         NA =MXGETM (PRHS (IP ))
         MA =MXGETN (PRHS (IP ))
         IF (NA.NE.L )THEN 
            WRITE (TEXT ,'(''D MUST HAVE '',I6,'' ROWS'')')L 
            CALL MEXERRMSGTXT (TEXT )
         END IF 
         IF (MA.NE.M )THEN 
            WRITE (TEXT ,'(''D MUST HAVE '',I6,'' COLUMNS'')')M 
            CALL MEXERRMSGTXT (TEXT )
         END IF 
         IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP ))
     $      .EQ.1)THEN 
            CALL MEXERRMSGTXT ('D MUST BE A REAL MATRIX')
         END IF 
         IP =IP +1
      END IF 
C
C  Y(txp), U(txm)
C
      IF (NRHS.GE.IP )THEN 
         NSMP =MXGETM (PRHS (IP ))
         IF (MXGETN (PRHS (IP )).NE.L )THEN 
            WRITE (TEXT ,'('' Y MUST HAVE '',I6,'' COLUMNS'')')L 
            CALL MEXERRMSGTXT (TEXT )
         END IF 
         IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP ))
     $      .EQ.1)THEN 
            CALL MEXERRMSGTXT ('Y MUST BE A REAL MATRIX')
         END IF 
         IP =IP +1
      END IF 
C
      IF (((TASK.EQ.1 .AND.CUSE.EQ.2 ).OR.CUSE.EQ.1 ).AND.NRHS.GE.IP )
     $   THEN 
         MA =MXGETN (PRHS (IP ))
         IF (CUSE.EQ.2 .AND.MA.NE.M )THEN 
            WRITE (TEXT ,'(''U MUST HAVE '',I6,'' COLUMNS'')')M 
            CALL MEXERRMSGTXT (TEXT )
         ELSE 
            M =MA 
         END IF 
C
         IF (M.GT.0 )THEN 
            IF (MXGETM (PRHS (IP )).NE.NSMP )THEN 
               CALL MEXERRMSGTXT (
     $            'U MUST HAVE THE SAME ROW DIMENSION AS Y')
            END IF 
            IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP )
     $         ).EQ.1)THEN 
               CALL MEXERRMSGTXT ('U MUST BE A REAL MATRIX')
            END IF 
         END IF 
         IP =IP +1
      END IF 
C
      IF (CUSE.EQ.1 )THEN 
         NCOL =N *M 
         IF (TASK.EQ.1 )NCOL =NCOL +N 
         MINSMP =NCOL 
         IF (IJOB.EQ.2 )THEN 
            MINSMP =MINSMP +M 
            IQ =MINSMP 
         ELSE IF (TASK.EQ.0 )THEN 
            IQ =MINSMP 
            MINSMP =MINSMP +1
         ELSE 
            IQ =MINSMP 
         END IF 
      ELSE 
         NCOL =N 
         IF (TASK.EQ.1 )THEN 
            MINSMP =N 
         ELSE 
            MINSMP =0
         END IF 
         IQ =MINSMP 
      END IF 
C
      IF (NSMP.LT.MINSMP )THEN 
         WRITE (TEXT ,'
     $      ('' THE NUMBER OF SAMPLES SHOULD BE AT LEAST '',I10)'
     $      )MINSMP 
         CALL MEXERRMSGTXT (TEXT )
      END IF 
C
C     tol
C   
      TOL =ZERO 
      IF (NRHS.GE.IP )THEN 
         IF (MXGETM (PRHS (IP )).NE.1.OR. MXGETN (PRHS (IP )).NE.1)THEN 
            CALL MEXERRMSGTXT ('TOL MUST BE A SCALAR')
         END IF 
         IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP ))
     $      .EQ.1)THEN 
              CALL MEXERRMSGTXT ('TOL MUST BE A REAL SCALAR')
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),TOL ,1)
         IP =IP +1
      END IF 
C
C     printw
C   
      PRINTW =.FALSE.
      IF (NRHS.GE.IP )THEN 
         IF (MXGETM (PRHS (IP )).NE.1.OR.MXGETN (PRHS (IP )).NE.1)THEN 
            CALL MEXERRMSGTXT ('PRINTW MUST BE A SCALAR')
         END IF 
         IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP ))
     $      .EQ.1)THEN 
            CALL MEXERRMSGTXT ('PRINTW MUST BE AN INTEGER SCALAR')
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),TEMP ,1)
         ITMP =TEMP 
         IF (ITMP.LT.0 .OR.ITMP.GT.1 )THEN 
            CALL MEXERRMSGTXT (
     $         'PRINTW HAS 0 OR 1 THE ONLY ADMISSIBLE VALUES')
         END IF 
         PRINTW =ITMP.EQ.1 
         IP =IP +1
      END IF 
C
C Determine the lenghts of working arrays.
C The default value for LDWORK is computed using the formula
C      LDWORK = MAX( minimum value needed, 2*CSIZE/3,
C     $              CSIZE - ( M + L )*NSMP - 2*N*( N + M + L ) - L*M )
C where CSIZE is the cache size in double precision words.
C If LDWORK is specified,
C        but it is less than the minimum workspace size 
C needed, that minimum value is used instead.
C
      LDA =MAX (1,N )
      LDB =LDA 
      LDC =L 
      LDD =LDC 
      LDV =LDA 
      LDY =MAX (1,NSMP )
      IF (M.GT.0 .AND.((TASK.EQ.1 .AND.CUSE.EQ.2 ).OR.CUSE.EQ.1 ))THEN 
         LDU =LDY 
      ELSE 
         LDU =1
      END IF 
C
      LIWORK =NCOL 
      IF ((TASK.EQ.0 .AND.CUSE.NE.1 ).OR.MAX (N ,M ).EQ.0)THEN 
         LDWORK =2
      ELSE 
         IF (IJOB.EQ.2 )LIWORK =MAX (LIWORK ,M )
         IQ =IQ *L 
         NCP1 =NCOL +1
         ISIZE =NSMP *L *NCP1 
         IF (N.GT.0 .AND.TASK.EQ.1 )THEN 
            IF (CUSE.EQ.1 )THEN 
               IC =2*N *N +N 
            ELSE 
               IC =2*N *N 
            END IF 
         ELSE 
            IC =0
         END IF 
         MINWLS =NCOL *NCP1 
         IF (CUSE.EQ.1 )THEN 
            IF (IJOB.EQ.2 )MINWLS =MINWLS +L *M *NCP1 
            IF (M.GT.0 .AND.IJOB.EQ.2 )THEN 
               IA =M +MAX (2*NCOL ,M )
            ELSE 
               IA =2*NCOL 
            END IF 
            LDW1 =N *N *M +MAX (IC ,IA )
            IF (TASK.EQ.1 )LDW1 =LDW1 +L *N 
            LDW2 =ISIZE +MAX (N +MAX (IC ,IA ),6*NCOL )
            LDW3 =MINWLS +MAX (IQ *NCP1 +LDW1 ,6*NCOL )
            IF (M.GT.0 .AND.IJOB.EQ.2 )THEN 
               LDW2 =MAX (LDW2 ,ISIZE +2*M *M +6*M )
               LDW3 =MAX (LDW3 ,MINWLS +2*M *M +6*M )
               INI =3
            ELSE 
               INI =2
            END IF 
         ELSE 
            ITAU =IC +L *N 
            LDW2 =ISIZE +2*N +MAX (IC ,4*N )
            LDW3 =MINWLS +2*N +MAX (IQ *NCP1 +ITAU ,4*N )
            INI =2
         END IF 
         LDWMIN =INI +N *(N +M +L )+MAX (5*N ,INI ,MIN (LDW2 ,LDW3 ))
         LDWORK =MAX (LDWMIN ,2*CSIZE /3,CSIZE -(M +L )*NSMP -2*N *(N +
     $      M +L )-L *M )
      END IF 
C
C   ldwork
C   
      IF (NRHS.GE.IP )THEN 
         IF (MXGETM (PRHS (IP )).NE.1.OR.MXGETN (PRHS (IP )).NE.1)THEN 
            CALL MEXERRMSGTXT ('LDWORK MUST BE A SCALAR')
         END IF 
         IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP ))
     $      .EQ.1)THEN 
              CALL MEXERRMSGTXT ('LDWORK MUST BE A REAL SCALAR')
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),TEMP ,1)
         ITMP =INT (TEMP )
         IF (ITMP.LT.LDWMIN )THEN 
            LDWORK =LDWMIN 
         ELSE 
            LDWORK =ITMP 
         END IF 
      END IF 
C
C Allocate variable dimension local arrays.
C !Fortran 90/95
C
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDA,N,A)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDB,M,B)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDC,N,C)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDD,M,D)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDWORK,1,DWORK)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'i',LIWORK,1,IWORK)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDU,M,U)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDV,N,V)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',N,1,X0)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDY,L,Y)) RETURN
C
C Copy inputs from MATLAB workspace to locally allocated arrays.
C      
      IP =IPS 
      IF (NRHS.GE.IP )THEN 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),stk(A),LDA *N )
         IP =IP +1
         IF (TASK.EQ.1 .AND.CUSE.EQ.2 )THEN 
            IF (N.GT.0 )THEN 
               CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),stk(B),
     $            LDB *M )
            END IF 
            IP =IP +1
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),stk(C),LDC *N )
         IP =IP +1
         IF (TASK.EQ.1 .AND.CUSE.EQ.2 .AND.IJOB.EQ.2 )THEN 
            CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),stk(D),LDD *M )
            IP =IP +1
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),stk(Y),NSMP *L )
         IP =IP +1
         IF (CUSE.EQ.1 .OR.(TASK.EQ.1 .AND.CUSE.EQ.2 ))THEN 
            CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),stk(U),NSMP *M )
         END IF 
      END IF 
C
C Do the actual computations.
C
      CALL IB01CD (JOBX0 ,COMUSE ,JOB ,N ,M ,L ,NSMP ,stk(A),LDA ,stk(B)
     $   ,LDB ,stk(C),LDC ,stk(D),LDD ,stk(U),LDU ,stk(Y),LDY ,stk(X0),
     $   stk(V),LDV ,TOL ,istk(IWORK),stk(DWORK),LDWORK ,IWARN ,INFO )
      IF (IWARN.NE.0 .AND.PRINTW )THEN 
         WRITE (TEXT ,'(''  IWARN = '',I4,'' ON EXIT FROM IB01CD'')')
     $      IWARN 
      END IF 
      IF (INFO.NE.0 )THEN 
         WRITE (TEXT ,'(''INFO = '',I4,'' ON EXIT FROM IB01CD'')')INFO 
      ELSE 
C
C Copy output to MATLAB workspace.
C
         IP =1
         IF (TASK.EQ.1 .OR.(TASK.EQ.0 .AND.CUSE.EQ.2 ))THEN 
            PLHS (IP )=MXCREATEFULL (N ,1,0)
            CALL MXCOPYREAL8TOPTR (stk(X0),MXGETPR (PLHS (IP )),N )
            IP =IP +1
         END IF 
C         
         IF (CUSE.EQ.1 .AND.NLHS.GE.IP )THEN 
            PLHS (IP )=MXCREATEFULL (N ,M ,0)
            CALL MXCOPYREAL8TOPTR (stk(B),MXGETPR (PLHS (IP )),N *M )
            IP =IP +1
            IF (NLHS.GE.IP .AND.IJOB.EQ.2 )THEN 
               PLHS (IP )=MXCREATEFULL (L ,M ,0)
               CALL MXCOPYREAL8TOPTR (stk(D),MXGETPR (PLHS (IP )),L *M )
               IP =IP +1
            END IF 
         END IF 
C
         IF ((TASK.EQ.1 .OR.CUSE.EQ.1 ).AND.NLHS.GE.IP )THEN 
            PLHS (IP )=MXCREATEFULL (N ,N ,0)
            CALL MXCOPYREAL8TOPTR (stk(V),MXGETPR (PLHS (IP )),N *N )
            IP =IP +1
         END IF 
C         
         IF (NLHS.GE.IP )THEN 
            NO =1
            IF (CUSE.EQ.1 .AND.M.GT.0 .AND.IJOB.EQ.2 )NO =2
            PLHS (IP )=MXCREATEFULL (NO ,1,0)
            CALL MXCOPYREAL8TOPTR (stk(DWORK+2-1),MXGETPR (PLHS (IP )),
     $         NO )
         END IF 
      END IF 
C
C Deallocate local arrays.
C !Fortran 90/95
C
C     DEALLOCATE( A, B, C, D, V, U, Y, IWORK, DWORK, X0 )
C
C Error and warning handling.
C
      IF (IWARN.NE.0 .AND.PRINTW )THEN 
         CALL MEXPRINTF (TEXT )
      END IF 
C
      IF (INFO.NE.0 )THEN 
         CALL MEXERRMSGTXT (TEXT )
      END IF 
C
      RETURN 
C *** Last line of findBD ***
      END 

C ORDER.F  - Gateway function for computation of the order of a
C            discrete-time system using SLICOT routine IB01AD.
C
C RELEASE 4.0, WGS COPYRIGHT 2000.
C
C Matlab call:
C   [R(,n,sval,rcnd)] = order(meth,alg,jobd,batch,conct,s,Y(,U,tol,
C                             printw,ldwork,R))
C
C Purpose:
C   To preprocess the input-output data for estimating the matrices 
C   of a linear time-invariant dynamical system, using Cholesky or
C   QR factorization and subspace identification techniques (MOESP 
C   and N4SID), and to find an estimate of the system order. The
C   input-output data can, optionally, be processed sequentially.
C
C Input parameters:
C   meth  - integer option to determine the method to use:
C           = 1 : MOESP method with past inputs and outputs;
C           = 2 : N4SID method.
C   alg   - integer option to determine the algorithm for computing the
C           triangular factor of the concatenated block-Hankel matrices
C           built from the input-output data:
C           = 1 : Cholesky algorithm on the correlation matrix;
C           = 2 : fast QR algorithm;
C           = 3 : standard QR algorithm.
C   jobd  - integer option to specify if the matrices B and D should
C           later be computed using the MOESP approach:
C           = 1 : the matrices B and D should later be computed using
C                 the MOESP approach;
C           = 2 : the matrices B and D should not be computed using
C                 the MOESP approach.
C           This parameter is not relevant for meth = 2.
C   batch - integer option to specify whether or not sequential data
C           processing is to be used, and, for sequential processing,
C           whether or not the current data block is the first block,
C           an intermediate block, or the last block, as follows:
C           = 1:  the first block in sequential data processing;
C           = 2:  an intermediate block in sequential data processing;
C           = 3:  the last block in sequential data processing;
C           = 4:  one block only (non-sequential data processing).
C   conct - integer option to specify whether or not the successive data
C           blocks in sequential data processing belong to a single
C           experiment, as follows:
C           = 1:  the current data block is a continuation of the
C                 previous data block and/or it will be continued by the
C                 next data block;
C           = 2:  there is no connection between the current data block
C                 and the previous and/or the next ones.
C           This parameter is not used if batch = 4.
C   s     - the number of block rows in the input and output block
C           Hankel matrices to be processed.  s > 0.
C   Y     - the t-by-l output-data sequence matrix.  Column j of  Y
C           contains the  t  values of the j-th output component for
C           consecutive time increments.
C   U     - (optional) the t-by-m input-data sequence matrix.  Column j
C           of  U  contains the  t  values of the j-th input component
C           for consecutive time increments.
C           Default:    U = [].
C   tol   - (optional) vector of length 2 containing tolerances:
C           tol(1) - tolerance used for estimating the rank of matrices.
C           If  tol(1) > 0,  then the given value of  tol(1)  is used
C           as a lower bound for the reciprocal condition number;
C           an m-by-n matrix whose estimated condition number is less
C           than  1/tol(1)  is considered to be of full rank.
C           If  tol(1) <= 0,  then a default value m*n*epsilon_machine
C           is used, where epsilon_machine is the relative machine
C           precision.
C           tol(2) - tolerance used for determining an estimate of the
C           system order. If  tol(2) >= 0,  the estimate is indicated
C           by the index of the last singular value greater than or
C           equal to  tol(2).  (Singular values less than  tol(2) are
C           considered as zero.) When  tol(2) = 0,  an internally
C           computed default value,  tol(2) = s*epsilon_machine*sval(1),
C           is used, where  sval(1)  is the maximal singular value, and
C           epsilon_machine the relative machine precision.
C           When  tol(2) < 0,  the estimate is indicated by the index of
C           the singular value that has the largest logarithmic gap to
C           its successor.
C           Default:    tol(1:2) = [0,-1].
C   printw- (optional) switch for printing the warning messages.
C           = 1:  print warning messages;
C           = 0:  do not print warning messages.
C           Default:    printw = 0.
C   ldwork- (optional) the workspace size.
C           Default : computed by the formulas
C           nr = 2*( m + l )*s
C           LDWORK = ( t - 2*s + 3 + 64 )*nr
C           if ( CSIZE > MAX( nr*nr + t*( m + l ) + 16, 2*nr ) ) then
C              LDWORK = MIN( LDWORK, CSIZE - nr*nr - t*( m + l ) - 16 )
C           else
C              LDWORK = MIN( LDWORK, MAX( 2*nr, CSIZE/2 ) )
C           end if
C           LDWORK = MAX( minimum workspace size needed, LDWORK )
C           where CSIZE is the cache size in double precision words.
C           If LDWORK is specified less than the minimum workspace size 
C           needed, that minimum value is used instead.
C   R     - (optional) if batch = 2 or 3, the 2*(m+l)*s-by-2*(m+l)*s 
C           (upper triangular, if alg <> 2) part of  R  must contain the
C           (upper triangular) matrix  R  computed at the previous call
C           of this mexfile in sequential data processing. If conct = 1,
C           R  has an additional column, also set at the previous call. 
C           If alg = 2,  R  has m+l+1 additional columns, set at the
C           previous call. 
C           This parameter is not used for batch = 1 or batch = 4.
C
C Output parameter:
C   R     - if batch = 3 or 4, the 2*(m+l)*s-by-2*(m+l)*s part of  R 
C           contains the processed upper triangular factor  R  from the
C           QR factorization of the concatenated block-Hankel matrices,
C           and further details needed for computing system matrices.
C           If batch = 1 or 2, then  R  contains intermediate results
C           needed at the next call of this mexfile. If batch = 1 or 2
C           and conct = 1,  R  has an additional column, also set before
C           return. If batch = 1 or 2 and alg = 2,  R  has m+l+1 
C           additional columns, set before return.
C   n     - the order of the system.
C   sval  - (optional) the singular values used for estimating the order
C           of the system.
C   rcnd  - (optional) if meth = 2, vector of length 2 containing the
C           reciprocal condition numbers of the matrices involved in
C           rank decisions or least squares solutions.
C
C Contributor:
C   V. Sima, Research Institute for Informatics, Bucharest, Oct. 1999.
C
C Revisions:
C   V. Sima, May 2000, July 2000.
C
C **********************************************************************
C
      SUBROUTINE sorder( NLHS, PLHS, NRHS, PRHS )
C     ADDED FOR SCILAB ==========================
      INCLUDE '../stack.h'
      LOGICAL CREATEVAR
C     ===========================================
C
C For Windows only!
C This is the library needed to set the floating point 
C exception word and it needs to be here in the code.  
C  
C     USE DFLIB
C
C .. Parameters ..
      DOUBLE PRECISION ZERO ,ONE 
      PARAMETER (ZERO =0.0D0,ONE =1.0D0)
C
C .. Mex-file interface parameters ..
      INTEGER PLHS (*),PRHS (*)
      INTEGER NLHS ,NRHS 
C
C .. Mex-file integer functions ..
      INTEGER MXCREATEFULL ,MXGETPR 
      INTEGER MXGETM ,MXGETN ,MXISNUMERIC ,MXISCOMPLEX 
C
C .. Scalar parameters used by SLICOT subroutines ..
      DOUBLE PRECISION TOL1 ,TOL2 
      INTEGER INFO ,IWARN ,L ,LDR ,LDU ,LDWORK ,LDY ,M ,N ,NOBR ,NSMP 
      CHARACTER ALG ,BATCH ,CONCT ,CTRL ,JOBD ,METH 
C
C .. Allocatable arrays ..
C !Fortran 90/95 (Fixed dimensions should be used with Fortran 77.)
      INTEGER IWORK
      INTEGER DWORK, R, SV, U, Y
C
C .. Local variables and constant dimension arrays ..
      DOUBLE PRECISION TEMP ,TOL (2)
      INTEGER CSIZE ,IALG ,IBCH ,ICNC ,IJOB ,IMTH ,IP ,ISIZE ,ITMP ,
     $   LDWMIN ,LIWORK ,NCOL ,NR ,NRSAVE ,NSMPMN 
      CHARACTER *120TEXT 
      LOGICAL PRINTW 
C
C .. Cache size in double precision words ..
      PARAMETER (CSIZE =64000)
C
C .. External subroutines ..
      EXTERNAL DCOPY ,DLACPY ,IB01AD 
C
C ..Intrinsic functions..
      INTRINSIC INT ,MAX ,MIN 
C
C For Windows only!
C This resets the floating point exception to allow divide by zero,
C overflow and invalid numbers. 
C
C     INTEGER(2)        CONTROL
C
C     CALL GETCONTROLFPQQ( CONTROL )
C     CONTROL = CONTROL .OR. FPCW$ZERODIVIDE
C     CONTROL = CONTROL .OR. FPCW$INVALID
C     CONTROL = CONTROL .OR. FPCW$OVERFLOW
C     CALL SETCONTROLFPQQ( CONTROL )
C
C Check for proper number of arguments.
C
      IF (NRHS.LT.7 )THEN 
         CALL MEXERRMSGTXT ('ORDER REQUIRES AT LEAST 7 INPUT ARGUMENTS')
      ELSE IF (NLHS.LT.1 )THEN 
         CALL MEXERRMSGTXT (
     $      'ORDER REQUIRES AT LEAST 1 OUTPUT ARGUMENTS')
      END IF 
C
C Check dimensions of input parameters and read/set scalar parameters.
C
C   meth
C
      IF (MXGETM (PRHS (1)).NE.1.OR. MXGETN (PRHS (1)).NE.1)THEN 
         CALL MEXERRMSGTXT ('METH MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (1)).EQ.0.OR.MXISCOMPLEX (PRHS (1)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('METH MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (1)),TEMP ,1)
      IMTH =TEMP 
      IF (IMTH.LT.1 .OR.IMTH.GT.2 )THEN 
         CALL MEXERRMSGTXT (
     $      'METH HAS 1 OR 2 THE ONLY ADMISSIBLE VALUES')
      END IF 
C
      IF (IMTH.EQ.1 )THEN 
         METH ='M'
      ELSE 
         METH ='N'
      END IF 
C
C   alg
C
      IF (MXGETM (PRHS (2)).NE.1.OR. MXGETN (PRHS (2)).NE.1)THEN 
         CALL MEXERRMSGTXT ('ALG MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (2)).EQ.0.OR.MXISCOMPLEX (PRHS (2)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('ALG MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (2)),TEMP ,1)
      IALG =TEMP 
      IF (IALG.LT.1 .OR.IALG.GT.3 )THEN 
         CALL MEXERRMSGTXT ('
     $      ALG HAS 1, 2 OR 3 THE ONLY ADMISSIBLE VALUES')
      END IF 
C
      IF (IALG.EQ.1 )THEN 
         ALG ='C'
      ELSE IF (IALG.EQ.2 )THEN 
         ALG ='F'
      ELSE 
         ALG ='Q'
      END IF 
C
C   jobd
C
      IF (MXGETM (PRHS (3)).NE.1.OR. MXGETN (PRHS (3)).NE.1)THEN 
         CALL MEXERRMSGTXT ('JOBD MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (3)).EQ.0.OR.MXISCOMPLEX (PRHS (3)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('JOBD MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (3)),TEMP ,1)
      IJOB =TEMP 
      IF (IJOB.LT.1 .OR.IJOB.GT.2 )THEN 
         CALL MEXERRMSGTXT (
     $      'JOBD HAS 1 OR 2 THE ONLY ADMISSIBLE VALUES')
      END IF 
C
      IF (IJOB.EQ.1 )THEN 
         JOBD ='M'
      ELSE 
         JOBD ='N'
      END IF 
C
C   batch
C
      IF (MXGETM (PRHS (4)).NE.1.OR. MXGETN (PRHS (4)).NE.1)THEN 
         CALL MEXERRMSGTXT ('BATCH MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (4)).EQ.0.OR.MXISCOMPLEX (PRHS (4)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('BATCH MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (4)),TEMP ,1)
      IBCH =TEMP 
      IF (IBCH.LT.1 .OR.IBCH.GT.4 )THEN 
         CALL MEXERRMSGTXT ('
     $      BATCH HAS 1, 2, 3 OR 4 THE ONLY ADMISSIBLE VALUES')
      END IF 
C
      IF (IBCH.EQ.1 )THEN 
         BATCH ='F'
      ELSE IF (IBCH.EQ.2 )THEN 
         BATCH ='I'
      ELSE IF (IBCH.EQ.3 )THEN 
         BATCH ='L'
      ELSE 
         BATCH ='O'
      END IF 
C
C   conct
C
      IF (MXGETM (PRHS (5)).NE.1.OR. MXGETN (PRHS (5)).NE.1)THEN 
         CALL MEXERRMSGTXT ('CONCT MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (5)).EQ.0.OR.MXISCOMPLEX (PRHS (5)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('CONCT MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (5)),TEMP ,1)
      ICNC =TEMP 
      IF (ICNC.LT.1 .OR.ICNC.GT.2 )THEN 
         CALL MEXERRMSGTXT (
     $      'CONCT HAS 1 OR 2 THE ONLY ADMISSIBLE VALUES')
      END IF 
C
      IF (ICNC.EQ.1 )THEN 
         CONCT ='C'
      ELSE 
         CONCT ='N'
      END IF 
C
C   ctrl is set to 'N'
C
      CTRL ='N'
C
C   s
C
      IF (MXGETM (PRHS (6)).NE.1.OR. MXGETN (PRHS (6)).NE.1)THEN 
         CALL MEXERRMSGTXT ('S MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (6)).EQ.0.OR.MXISCOMPLEX (PRHS (6)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('S MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (6)),TEMP ,1)
      NOBR =TEMP 
      IF (NOBR.LT.1 )THEN 
         CALL MEXERRMSGTXT ('S MUST BE A POSITIVE INTEGER')
      END IF 
C
C   Y(txp), (U(txm))
C
      NSMP =MXGETM (PRHS (7))
      L =MXGETN (PRHS (7))
C
      IF (L.LE.0 )THEN 
         CALL MEXERRMSGTXT ('THE SYSTEM HAS NO OUTPUTS')
      END IF 
C
      IF (NRHS.GE.8 )THEN 
         M =MXGETN (PRHS (8))
      ELSE 
         M =0
      END IF 
      NR =2*(M +L )*NOBR 
      IF (IBCH.LE.2 )THEN 
         NSMPMN =2*NOBR 
      ELSE 
         NSMPMN =NR +2*NOBR -1
      END IF 
C
      IF (NSMP.LT.NSMPMN )THEN 
         WRITE (TEXT ,'
     $      ('' THE NUMBER OF SAMPLES SHOULD BE AT LEAST '',I10)'
     $      )NSMPMN 
         CALL MEXERRMSGTXT (TEXT )
      END IF 
C
      IF (MXISNUMERIC (PRHS (7)).EQ.0.OR.MXISCOMPLEX (PRHS (7)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('Y MUST BE A REAL MATRIX')
      END IF 
C
      IF (M.GT.0 )THEN 
         IF (MXISNUMERIC (PRHS (8)).EQ.0.OR.MXISCOMPLEX (PRHS (8)).EQ.1)
     $      THEN 
            CALL MEXERRMSGTXT ('U MUST BE A REAL MATRIX')
         END IF 
         IF (MXGETM (PRHS (8)).NE.NSMP )THEN 
            CALL MEXERRMSGTXT (
     $         'U MUST HAVE THE SAME ROW DIMENSION AS Y')
         END IF 
      END IF 
C
C     tol
C   
      TOL1 =ZERO 
      TOL2 =-ONE 
      IF (NRHS.GE.9 )THEN 
         ISIZE =MXGETM (PRHS (9))*MXGETN (PRHS (9))
         IF (ISIZE.GT.2 )CALL MEXERRMSGTXT (
     $      'TOL MUST BE A VECTOR WITH AT MOST 2 ELEMENTS')
         IF (MXISNUMERIC (PRHS (9)).EQ.0.OR.MXISCOMPLEX (PRHS (9)).EQ.1)
     $      THEN 
              CALL MEXERRMSGTXT ('TOL MUST BE A REAL VECTOR')
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (9)),TOL ,ISIZE )
         IF (ISIZE.GT.0 )TOL1 =TOL (1)
         IF (ISIZE.GT.1 )TOL2 =TOL (2)
      END IF 
C
      PRINTW =.FALSE.
      IP =10
      NCOL =NR 
      IF (IBCH.LT.4 )THEN 
         IF (ICNC.EQ.1 )NCOL =NCOL +1
         IF (IALG.EQ.2 )NCOL =NCOL +M +L +1
      END IF 
      NRSAVE =(NCOL -NR )*NR 
C
C     printw
C   
      IF (NRHS.GE.IP )THEN 
         IF (MXGETM (PRHS (IP )).NE.1.OR.MXGETN (PRHS (IP )).NE.1)THEN 
            CALL MEXERRMSGTXT ('PRINTW MUST BE A SCALAR')
         END IF 
         IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP ))
     $      .EQ.1)THEN 
            CALL MEXERRMSGTXT ('PRINTW MUST BE AN INTEGER SCALAR')
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),TEMP ,1)
         ITMP =TEMP 
         IF (ITMP.LT.0 .OR.ITMP.GT.1 )THEN 
            CALL MEXERRMSGTXT (
     $         'PRINTW HAS 0 OR 1 THE ONLY ADMISSIBLE VALUES')
         END IF 
         PRINTW =ITMP.EQ.1 
         IP =IP +1
      END IF 
C
C Determine the lenghts of working arrays.
C The quasi-optimal value for LDWORK (assumed block-size 64) is possibly
C modified, so that the sum of the lenghts of DWORK and other needed
C arrays does not exceed the cache size. Specifically, the default value
C for LDWORK is computed using the formulas
C           nr = 2*( m + l )*s
C           LDWORK = ( t - 2*s + 3 + 64 )*nr
C           if ( CSIZE > MAX( nr*nr + t*( m + l ) + 16, 2*nr ) ) then
C              LDWORK = MIN( LDWORK, CSIZE - nr*nr - t*( m + l ) - 16 )
C           else
C              LDWORK = MIN( LDWORK, MAX( 2*nr, CSIZE/2 ) )
C           end if
C           LDWORK = MAX( minimum workspace size needed, LDWORK )
C where CSIZE is the cache size in double precision words.
C If LDWORK is specified,
C        but it is less than the minimum workspace size 
C needed, that minimum value is used instead.
C
      LDY =NSMP 
      IF (M.GT.0 )THEN 
         LDU =LDY 
      ELSE 
         LDU =1
      END IF 
      LDR =NR 
      IF (IMTH.EQ.1 .AND.IJOB.EQ.1 )LDR =MAX (NR ,3*M *NOBR )
      IF (IMTH.EQ.2 )THEN 
         LIWORK =(M +L )*NOBR 
      ELSE IF (IALG.EQ.2 )THEN 
         LIWORK =M +L 
      ELSE 
         LIWORK =1
      END IF 
C
C     The next statement is included in order to allow switching
C     from Cholesky or fast QR to QR algorithm.
C
      LDWORK =(NSMP -2*NOBR +3+64)*NR 
      IF (CSIZE .GT.MAX (NR *NR +NSMP *(M +L )+16,2*NR ))THEN 
         LDWORK =MIN (LDWORK ,CSIZE -NR *NR -NSMP *(M +L )-16)
      ELSE 
         LDWORK =MIN (LDWORK ,MAX (2*NR ,CSIZE /2))
      END IF 
      LDWMIN =2*NR *(NOBR +1)
      IF (IALG.EQ.2 )LDWMIN =MAX (LDWMIN ,2*NR *(M +L +1)+NR ,NR *(M +
     $   L +3))
      LDWORK =MAX (LDWORK ,LDWMIN )
C
C   ldwork
C   
      IF (NRHS.GE.IP )THEN 
         IF (MXGETM (PRHS (IP )).NE.1.OR.MXGETN (PRHS (IP )).NE.1)THEN 
            CALL MEXERRMSGTXT ('LDWORK MUST BE A SCALAR')
         END IF 
         IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP ))
     $      .EQ.1)THEN 
              CALL MEXERRMSGTXT ('LDWORK MUST BE A REAL SCALAR')
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),TEMP ,1)
         ITMP =INT (TEMP )
         IF (ITMP.LT.LDWMIN )THEN 
            LDWORK =LDWMIN 
         ELSE 
            LDWORK =ITMP 
         END IF 
         IP =IP +1
C
C     R(2*(m+p)*s,2*(m+p)*s(+c)), where
C     c = 1,       if conct = 1 and batch < 4;
C     c = 0,       if conct = 2 or  batch = 4;
C     c = c+m+l+1, if  alg  = 2 and batch < 4.
C   
         IF (NRHS.GE.IP .AND.(IBCH.EQ.2 .OR.IBCH.EQ.3 ))THEN 
            IF (MXGETM (PRHS (IP )).NE.NR )THEN 
               WRITE (TEXT ,'(''R MUST HAVE '',I5,'' ROWS'')')NR 
               CALL MEXERRMSGTXT (TEXT )
            END IF 
            IF (MXGETN (PRHS (IP )).NE.NCOL )THEN 
               WRITE (TEXT ,'(''R MUST HAVE '',I5,'' COLUMNS'')')NCOL 
               CALL MEXERRMSGTXT (TEXT )
            END IF 
            IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP )
     $         ).EQ.1)THEN 
               CALL MEXERRMSGTXT ('R MUST BE A REAL MATRIX')
            END IF 
         END IF 
      END IF 
C
C Allocate variable dimension local arrays.
C !Fortran 90/95
C
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDR,NCOL,R)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',L*NOBR,1,SV)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDU,M,U)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDY,L,Y)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'i',LIWORK,1,IWORK)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDWORK,1,DWORK)) RETURN
      CALL DSET(LDR*NCOL,0.0d0,stk(R),1)
C
C Copy inputs from MATLAB workspace to locally allocated arrays.
C      
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (7)),stk(Y),NSMP *L )

      IF (M.GT.0 )THEN 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (8)),stk(U),NSMP *M )
      END IF 
      IF (NRHS.GE.IP .AND.(IBCH.EQ.2 .OR.IBCH.EQ.3 ))THEN 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),stk(R),NR *NCOL )
         IF (IALG.EQ.2 .OR.ICNC.EQ.1 )THEN 
            IF (LDR.EQ.NR )THEN 
               CALL DCOPY (NRSAVE ,stk(R+1-1 +(NR+1-1)*LDR),1,stk(DWORK)
     $            ,1)
            ELSE 
               CALL DCOPY (NRSAVE ,stk(R+NR*NR+1-1 +(1-1)*LDR),1,stk(
     $            DWORK),1)
            END IF 
         END IF 
      END IF 

C
C Do the actual computations.
C
      CALL IB01AD (METH ,ALG ,JOBD ,BATCH ,CONCT ,CTRL ,NOBR ,M ,L ,
     $   NSMP ,stk(U),LDU ,stk(Y),LDY ,N ,stk(R),LDR ,stk(SV),TOL1 ,
     $   TOL2 ,istk(IWORK),stk(DWORK),LDWORK ,IWARN ,INFO )
      IF (IWARN.NE.0 .AND.PRINTW )THEN 
         WRITE (TEXT ,'(''  IWARN = '',I4,'' ON EXIT FROM IB01AD'')')
     $      IWARN 
      END IF 
      IF (INFO.NE.0 )THEN 
         WRITE (TEXT ,'(''INFO = '',I4,'' ON EXIT FROM IB01AD'')')INFO 
         GO TO 20
      END IF 
C
C Copy output to MATLAB workspace.
C
      IF (LDR.GT.NR )CALL DLACPY ('FULL',NR ,NR ,stk(R),LDR ,stk(R),NR )
      IF ((IALG.EQ.2 .OR.ICNC.EQ.1 ).AND.IBCH.LE.2 )THEN 
         IF (LDR.EQ.NR )THEN 
            CALL DCOPY (NRSAVE ,stk(DWORK),1,stk(R+1-1 +(NR+1-1)*LDR),1)
         ELSE 
            CALL DCOPY (NRSAVE ,stk(DWORK),1,stk(R+NR*NR+1-1 +(1-1)*LDR)
     $         ,1)
         END IF 
      END IF 
      PLHS (1)=MXCREATEFULL (NR ,NCOL ,0)
      CALL MXCOPYREAL8TOPTR (stk(R),MXGETPR (PLHS (1)),NR *NCOL )
      IF (IBCH.GT.2 )THEN 
         IF (NLHS.GT.1 )THEN 
            PLHS (2)=MXCREATEFULL (1,1,0)
            TEMP =N 
            CALL MXCOPYREAL8TOPTR (TEMP ,MXGETPR (PLHS (2)),1)
            IF (NLHS.GT.2 )THEN 
               PLHS (3)=MXCREATEFULL (L *NOBR ,1,0)
               CALL MXCOPYREAL8TOPTR (stk(SV),MXGETPR (PLHS (3)),
     $            L *NOBR )
            END IF 
            IF (IMTH.EQ.2 .AND.NLHS.GT.3 )THEN 
               PLHS (4)=MXCREATEFULL (2,1,0)
               CALL MXCOPYREAL8TOPTR (stk(DWORK+2-1),MXGETPR (PLHS (4)),
     $            2)
            END IF 
         END IF 
      END IF 
C
C Deallocate local arrays.
C !Fortran 90/95
C
   20 CONTINUE 
C 
C     DEALLOCATE( R, SV, U, Y, IWORK, DWORK )
C
C Error and warning handling.
C
      IF (IWARN.NE.0 .AND.PRINTW )THEN 
         CALL MEXPRINTF (TEXT )
      END IF 
C
      IF (INFO.NE.0 )THEN 
         CALL MEXERRMSGTXT (TEXT )
      END IF 
C
      RETURN 
C *** Last line of ORDER ***
      END 

C SIDENT.F - Gateway function for computation of a discrete-time
C            state-space realization (A,B,C,D) and Kalman gain
C            using SLICOT routine IB01BD.
C
C RELEASE 4.0, WGS COPYRIGHT 2000.
C
C Matlab call:  
C   [(A,C)(,B(,D))(,K,Q,Ry,S,rcnd)] = sident(meth,job,s,n,l,R(,tol,t,A,
C                                            C,printw))
C
C Purpose:
C   To compute a state-space realization (A,B,C,D) and the Kalman
C   predictor gain K of a discrete-time system, given the system
C   order and the relevant part of the R factor of the concatenated 
C   block-Hankel matrices, using subspace identification techniques 
C   (MOESP and N4SID).
C
C Input parameters: 
C   meth  - integer option to determine the method to use:
C           = 1 : MOESP method with past inputs and outputs;
C           = 2 : N4SID method;
C           = 3 : combined method: A and C via MOESP, B and D via N4SID.
C   job   - integer option to determine the calculation to be performed:
C           = 1 : compute all system matrices, A, B, C, D;
C           = 2 : compute the matrices A and C only;
C           = 3 : compute the matrix B only;
C           = 4 : compute the matrices B and D only.
C   s     - the number of block rows in the processed input and output
C           block Hankel matrices.  s > 0.
C   n     - the order of the system.
C   l     - the number of system outputs.
C   R     - the 2*(m+l)*s-by-2*(m+l)*s part of  R  contains the
C           processed upper triangular factor  R  from the
C           QR factorization of the concatenated block-Hankel matrices,
C           and further details needed for computing system matrices.
C           (Above, m denotes the number of system inputs, determined
C           by s, l, and the size of R.) 
C   tol   - (optional) tolerance used for estimating the rank of
C           matrices. If  tol > 0,  then the given value of  tol  is
C           used as a lower bound for the reciprocal condition number;
C           an m-by-n matrix whose estimated condition number is less
C           than  1/tol  is considered to be of full rank.
C           Default:    m*n*epsilon_machine where epsilon_machine is
C           the relative machine precision.
C   t     - (optional) the total number of samples used for calculating
C           the covariance matrices.  Either  t = 0, or  t >= 2*(m+l)*s.
C           This parameter is not needed if the covariance matrices
C           and/or the Kalman predictor gain matrix are not desired.
C           If t = 0, then K, Q, Ry, and S are not computed.
C           Default:    t = 0.
C   A     - (optional) the n-by-n system state matrix A.
C           This parameter is needed if meth >= 2 and job >= 3.
C   C     - (optional) the l-by-n system output matrix C.
C           This parameter is needed if meth >= 2 and job >= 3.
C   printw- (optional) switch for printing the warning messages.
C           = 1:  print warning messages;
C           = 0:  do not print warning messages.
C           Default:    printw = 0.
C
C Output parameters:
C   A     - if job <= 2, the n-by-n system state matrix A.
C   C     - if job <= 2, the l-by-n system output matrix C.
C   B     - if job <> 2, the n-by-m system input matrix B.
C   D     - if job = 1 or 4, the l-by-m system matrix D.
C   K     - (optional) the n-by-l Kalman predictor gain matrix K.
C   Q     - (optional) the n-by-n positive semidefinite state covariance
C           matrix used as state weighting matrix when computing the
C           Kalman gain.
C   Ry    - (optional) the l-by-l positive (semi)definite output
C           covariance matrix used as output weighting matrix when
C           computing the Kalman gain.
C   S     - (optional) the n-by-l state-output cross-covariance matrix
C           used as cross-weighting matrix when computing the Kalman
C           gain.
C   rcnd  - (optional) vector of length lr, containing estimates of the
C           reciprocal condition numbers of the matrices involved in
C           rank decisions, least squares or Riccati equation solutions,
C           where lr = 4,  if Kalman gain matrix K is not required, and
C                 lr = 12, if Kalman gain matrix K is required.
C
C Contributor:
C   V. Sima, Research Institute for Informatics, Bucharest, Oct. 1999.
C
C Revisions:
C   V. Sima, May 2000, July 2000.
C
C **********************************************************************
C
      SUBROUTINE sident  ( NLHS, PLHS, NRHS, PRHS )
C     ADDED FOR SCILAB ==========================
      INCLUDE '../stack.h'
      LOGICAL CREATEVAR
C     ===========================================
C
C For Windows only!
C This is the library needed to set the floating point 
C exception word and it needs to be here in the code.  
C  
C     USE DFLIB
C
C .. Parameters ..
      DOUBLE PRECISION ZERO 
      PARAMETER (ZERO =0.0D0)
C
C .. Mex-file interface parameters ..
      INTEGER PLHS (*),PRHS (*)
      INTEGER NLHS ,NRHS 
C
C .. Mex-file integer functions ..
      INTEGER MXCREATEFULL ,MXGETPR 
      INTEGER MXGETM ,MXGETN ,MXISNUMERIC ,MXISCOMPLEX 
C
C .. Scalar parameters used by SLICOT subroutines ..
      DOUBLE PRECISION TOL 
      INTEGER INFO ,IWARN ,L ,LDA ,LDB ,LDC ,LDD ,LDK ,LDO ,LDQ ,LDR ,
     $   LDRY ,LDS ,LDWORK ,M ,N ,NOBR ,NSMPL 
      CHARACTER JOB ,JOBCK ,METH 
C
C .. Allocatable arrays ..
C !Fortran 90/95 (Fixed dimensions should be used with Fortran 77.)
      INTEGER IWORK
      INTEGER BWORK
      INTEGER A, B, C, D, DWORK, K, Q, R, RY, S
C
C .. Local variables and constant dimension arrays ..
      DOUBLE PRECISION TEMP 
      CHARACTER *120TEXT 
      INTEGER ID ,IJOB ,IP ,ITMP ,LBWORK ,LDUNN ,LIWORK ,LL ,LNOBR ,MA ,
     $   MNOBR ,MNOBRN ,N2 ,NA ,NCOL ,NL ,NN ,NPL ,NR ,NRC ,TASK 
      LOGICAL PRINTW 
C
C .. External subroutines ..
      EXTERNAL IB01BD 
C
C ..Intrinsic functions..
      INTRINSIC MAX 
C
C For Windows only!
C This resets the floating point exception to allow divide by zero,
C overflow and invalid numbers. 
C
C     INTEGER(2)        CONTROL
C
C     CALL GETCONTROLFPQQ( CONTROL )
C     CONTROL = CONTROL .OR. FPCW$ZERODIVIDE
C     CONTROL = CONTROL .OR. FPCW$INVALID
C     CONTROL = CONTROL .OR. FPCW$OVERFLOW
C     CALL SETCONTROLFPQQ( CONTROL )
C
C Check for proper number of arguments.
C
      IF (NRHS.LT.6 )THEN 
         CALL MEXERRMSGTXT (
     $      'SIDENT REQUIRES AT LEAST 6 INPUT ARGUMENTS')
      ELSE IF (NLHS.LT.1 )THEN 
         CALL MEXERRMSGTXT (
     $      'SIDENT REQUIRES AT LEAST 1 OUTPUT ARGUMENTS')
      END IF 
C
C Check dimensions of input parameters and read/set scalar parameters.
C
C   meth
C
      IF (MXGETM (PRHS (1)).NE.1.OR. MXGETN (PRHS (1)).NE.1)THEN 
         CALL MEXERRMSGTXT ('METH MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (1)).EQ.0.OR.MXISCOMPLEX (PRHS (1)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('METH MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (1)),TEMP ,1)
      TASK =TEMP 
      IF (TASK.LT.1 .OR.TASK.GT.3 )THEN 
         CALL MEXERRMSGTXT ('
     $      METH HAS 1, 2, OR 3 THE ONLY ADMISSIBLE VALUES')
      END IF 
C
      IF (TASK.EQ.1 )THEN 
         METH ='M'
      ELSE IF (TASK.EQ.2 )THEN 
         METH ='N'
      ELSE 
         METH ='C'
      END IF 
C
C   job
C
      IF (MXGETM (PRHS (2)).NE.1.OR. MXGETN (PRHS (2)).NE.1)THEN 
         CALL MEXERRMSGTXT ('JOB MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (2)).EQ.0.OR.MXISCOMPLEX (PRHS (2)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('JOB MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (2)),TEMP ,1)
      IJOB =TEMP 
      IF (IJOB.LT.1 .OR.IJOB.GT.4 )THEN 
         CALL MEXERRMSGTXT ('
     $      JOB HAS 1, 2, 3 OR 4 THE ONLY ADMISSIBLE VALUES')
      END IF 
C
      IF (IJOB.EQ.1 )THEN 
         JOB ='A'
      ELSE IF (IJOB.EQ.2 )THEN 
         JOB ='C'
      ELSE IF (IJOB.EQ.3 )THEN 
         JOB ='B'
      ELSE 
         JOB ='D'
      END IF 
C         
C   s
C
      IF (MXGETM (PRHS (3)).NE.1.OR. MXGETN (PRHS (3)).NE.1)THEN 
         CALL MEXERRMSGTXT ('S MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (3)).EQ.0.OR.MXISCOMPLEX (PRHS (3)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('S MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (3)),TEMP ,1)
      NOBR =TEMP 
      IF (NOBR.LT.1 )THEN 
         CALL MEXERRMSGTXT ('S MUST BE A POSITIVE INTEGER')
      END IF 
C         
C   n
C
      IF (MXGETM (PRHS (4)).NE.1.OR. MXGETN (PRHS (4)).NE.1)THEN 
         CALL MEXERRMSGTXT ('N MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (4)).EQ.0.OR.MXISCOMPLEX (PRHS (4)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('N MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (4)),TEMP ,1)
      N =TEMP 
      IF (N.LT.1 )THEN 
         CALL MEXERRMSGTXT ('N MUST BE A POSITIVE INTEGER')
      END IF 
      IF (N.GE.NOBR )THEN 
         WRITE (TEXT ,'('' THE ORDER SHOULD BE AT MOST '', I6)')NOBR -1
         CALL MEXERRMSGTXT (TEXT )
      END IF 
C         
C   l
C
      IF (MXGETM (PRHS (5)).NE.1.OR. MXGETN (PRHS (5)).NE.1)THEN 
         CALL MEXERRMSGTXT ('L MUST BE A SCALAR')
      END IF 
      IF (MXISNUMERIC (PRHS (5)).EQ.0.OR.MXISCOMPLEX (PRHS (5)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('L MUST BE AN INTEGER SCALAR')
      END IF 
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (5)),TEMP ,1)
      L =TEMP 
      IF (L.LT.1 )THEN 
         CALL MEXERRMSGTXT ('THE SYSTEM HAS NO OUTPUTS')
      END IF 
C
C   R(nr,nr)
C
      NR =MXGETM (PRHS (6))
      NCOL =MXGETN (PRHS (6))
      IF (NR.LT.2 *L )THEN 
         WRITE (TEXT ,'(''R MUST HAVE AT LEAST '',I5,'' ROWS'')')2*L 
         CALL MEXERRMSGTXT (TEXT )
      END IF 
      IF (NCOL.LT.NR )THEN 
         WRITE (TEXT ,'(''R MUST HAVE AT LEAST '',I5,'' COLUMNS'')')
     $      NCOL 
         CALL MEXERRMSGTXT (TEXT )
      END IF 
      IF (MXISNUMERIC (PRHS (6)).EQ.0.OR.MXISCOMPLEX (PRHS (6)).EQ.1)
     $   THEN 
         CALL MEXERRMSGTXT ('R MUST BE A REAL MATRIX')
      END IF 
C
C   m
C
      M =NR /(2*NOBR )-L 
C
C   tol
C   
      TOL =ZERO 
      IF (NRHS.GT.6 )THEN 
         IF (MXGETM (PRHS (7)).NE.1.OR. MXGETN (PRHS (7)).NE.1)THEN 
            CALL MEXERRMSGTXT ('TOL MUST BE A SCALAR')
         END IF 
         IF (MXISNUMERIC (PRHS (7)).EQ.0.OR.MXISCOMPLEX (PRHS (7)).EQ.1)
     $      THEN 
              CALL MEXERRMSGTXT ('TOL MUST BE A REAL SCALAR')
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (7)),TOL ,1)
      END IF 
C
C   t
C   
      NSMPL =0
      JOBCK ='N'
      IF (NRHS.GT.7 )THEN 
         JOBCK ='K'
         IF (MXGETM (PRHS (8)).NE.1.OR. MXGETN (PRHS (8)).NE.1)THEN 
            CALL MEXERRMSGTXT ('T MUST BE A SCALAR')
         END IF 
         IF (MXISNUMERIC (PRHS (8)).EQ.0.OR.MXISCOMPLEX (PRHS (8)).EQ.1)
     $      THEN 
              CALL MEXERRMSGTXT ('T MUST BE A REAL SCALAR')
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (8)),TEMP ,1)
         NSMPL =TEMP 
C
         IF (NSMPL.NE.0 .AND.NSMPL.LT.NR )THEN 
            WRITE (TEXT ,'
     $         ('' THE NUMBER OF SAMPLES SHOULD BE AT LEAST '',I10)')NR 
            CALL MEXERRMSGTXT (TEXT )
         ELSE IF (NSMPL.EQ.0 )THEN 
            JOBCK ='N'
         END IF 
      END IF 
C
C   A(n,n)
C
      IF (TASK.GE.2 .AND.IJOB.GE.3 )THEN 
         MA =MXGETM (PRHS (9))
         NA =MXGETN (PRHS (9))
         IF (MA.NE.N .OR.NA.NE.N )THEN 
            WRITE (TEXT ,'(''A MUST HAVE '',I5,'' ROWS AND COLUMNS'')')
     $         N 
            CALL MEXERRMSGTXT (TEXT )
         END IF 
         IF (MXISNUMERIC (PRHS (9)).EQ.0.OR.MXISCOMPLEX (PRHS (9)).EQ.1)
     $      THEN 
            CALL MEXERRMSGTXT ('A MUST BE A REAL MATRIX')
         END IF 
C
C   C(l,n)
C
         MA =MXGETM (PRHS (10))
         NA =MXGETN (PRHS (10))
         IF (MA.NE.L )THEN 
            WRITE (TEXT ,'(''C MUST HAVE '',I5,'' ROWS'')')L 
            CALL MEXERRMSGTXT (TEXT )
         END IF 
         IF (NA.NE.N )THEN 
            WRITE (TEXT ,'(''C MUST HAVE '',I5,'' COLUMNS'')')N 
            CALL MEXERRMSGTXT (TEXT )
         END IF 
         IF (MXISNUMERIC (PRHS (10)).EQ.0.OR.MXISCOMPLEX (PRHS (10))
     $      .EQ.1)THEN 
            CALL MEXERRMSGTXT ('C MUST BE A REAL MATRIX')
         END IF 
      END IF 
C
C     printw
C   
      PRINTW =.FALSE.
      IP =11
      IF (NRHS.GE.IP )THEN 
         IF (MXGETM (PRHS (IP )).NE.1.OR.MXGETN (PRHS (IP )).NE.1)THEN 
            CALL MEXERRMSGTXT ('PRINTW MUST BE A SCALAR')
         END IF 
         IF (MXISNUMERIC (PRHS (IP )).EQ.0.OR.MXISCOMPLEX (PRHS (IP ))
     $      .EQ.1)THEN 
            CALL MEXERRMSGTXT ('PRINTW MUST BE AN INTEGER SCALAR')
         END IF 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (IP )),TEMP ,1)
         ITMP =TEMP 
         IF (ITMP.LT.0 .OR.ITMP.GT.1 )THEN 
            CALL MEXERRMSGTXT (
     $         'PRINTW HAS 0 OR 1 THE ONLY ADMISSIBLE VALUES')
         END IF 
         PRINTW =ITMP.EQ.1 
      END IF 
C
C Determine the lenghts of working arrays.
C The value for LDWORK is the minimum value needed by IB01BD for each
C method and algorithm implemented.  Using a larger value could
C increase the efficiency.
C
      MNOBR =M *NOBR 
      LNOBR =L *NOBR 
      MNOBRN =MNOBR +N 
      LDUNN =(LNOBR -L )*N 
      NPL =N +L 
      N2 =N +N 
      NN =N *N 
      NL =N *L 
      LL =L *L 
C
      LDA =MAX (1,N )
      LDB =LDA 
      LDC =MAX (1,L )
      LDD =LDC 
      LDO =LNOBR 
      LDR =NR 
      IF (NSMPL.NE.0 )THEN 
         LDK =LDA 
         LDQ =LDA 
         LDS =LDA 
         LDRY =LDC 
         LBWORK =N2 
      ELSE 
         LDK =1
         LDQ =1
         LDS =1
         LDRY =1
         LBWORK =1
      END IF 
C
      LIWORK =MNOBR +N 
      IF (TASK.EQ.1 )THEN 
         LIWORK =MAX (LIWORK ,LNOBR )
      ELSE IF (TASK.EQ.2 )THEN 
         LIWORK =MAX (LIWORK ,M *NPL )
      ELSE 
         LIWORK =MAX (LIWORK ,LNOBR ,M *NPL )
      END IF 
      IF (NSMPL.GT.0 )LIWORK =MAX (LIWORK ,NN )
C
      IAW =0
      LDWORK =LDUNN +4*N 
      IF (TASK.EQ.1 )THEN 
         ID =0
      ELSE 
         ID =N 
      END IF 
C
      IF (TASK.NE.2 )THEN 
         IF (IJOB.LE.2 )THEN 
            LDWORK =MAX (LDWORK ,2*LDUNN +N2 ,LDUNN +NN +7*N )
         END IF 
      END IF 
C
      IF ((M.GT.0 .AND.IJOB.NE.2 ).OR.TASK.GE.2 )THEN 
         LDWORK =MAX (LDWORK ,2*LDUNN +NN +ID +7*N )
         IF (TASK.EQ.1 )LDWORK =MAX (LDWORK ,LDUNN +N +6*MNOBR ,LDUNN +
     $      N +MAX (L +MNOBR ,LNOBR +MAX (3*LNOBR ,M )))
      ELSE 
         IF (TASK.NE.2 )THENIAW =N +NN 
      END IF 
C
      IF (TASK.NE.1 .OR.NSMPL.GT.0 )THEN 
         LDWORK =MAX (LDWORK ,LDUNN +IAW +N2 +MAX (5*N ,LNOBR +2*MNOBR +
     $      L ),ID +4*MNOBRN ,ID +MNOBRN +NPL )
         IF (TASK.NE.1 .AND.M.GT.0 .AND.IJOB.NE.2 )LDWORK =MAX (LDWORK ,
     $      MNOBR *NPL *(M *NPL +1)+MAX (NPL **2,4*M *NPL +1))
         LDWORK =LNOBR *N +LDWORK 
      END IF 
C
      IF (NSMPL.GT.0 )LDWORK =MAX (LDWORK ,4*NN +2*NL +LL +MAX (3*L ,
     $   NL ),14*NN +12*N +5)
C
C Allocate variable dimension local arrays.
C !Fortran 90/95
C
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDA,N,A)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDB,M,B)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDC,N,C)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDD,M,D)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDWORK,1,DWORK)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'i',LIWORK,1,IWORK)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDQ,N,Q)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDR,NCOL,R)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDRY,L,RY)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDS,L,S)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'b',LBWORK,1,BWORK)) RETURN
      IF(.NOT.CREATEVAR(NBVARS+1,'d',LDK,L,K)) RETURN
C
C Copy inputs from MATLAB workspace to locally allocated arrays.
C      
      CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (6)),stk(R),LDR *NCOL )
      IF (TASK.GE.2 .AND.IJOB.GE.3 )THEN 
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (9)),stk(A),LDA *N )
         CALL MXCOPYPTRTOREAL8 (MXGETPR (PRHS (10)),stk(C),LDC *N )
      END IF 
C
C Do the actual computations.
C
      CALL IB01BD (METH ,JOB ,JOBCK ,NOBR ,N ,M ,L ,NSMPL ,stk(R),LDR ,
     $   stk(A),LDA ,stk(C),LDC ,stk(B),LDB ,stk(D),LDD ,stk(Q),LDQ ,
     $   stk(RY),LDRY ,stk(S),LDS ,stk(K),LDK ,TOL ,istk(IWORK),stk(
     $   DWORK),LDWORK ,istk(BWORK),IWARN ,INFO )
      IF (IWARN.NE.0 .AND.PRINTW )THEN 
         WRITE (TEXT ,'(''  IWARN = '',I4,'' ON EXIT FROM IB01BD'')')
     $      IWARN 
      END IF 
      IF (INFO.NE.0 )THEN 
         WRITE (TEXT ,'(''INFO = '',I4,'' ON EXIT FROM IB01BD'')')INFO 
      ELSE 
C
C Copy output to MATLAB workspace.
C
         IF (IJOB.LE.2 )THEN 
            PLHS (1)=MXCREATEFULL (N ,N ,0)
            CALL MXCOPYREAL8TOPTR (stk(A),MXGETPR (PLHS (1)),NN )
            IP =1
            IF (NLHS.GT.1 )THEN 
               PLHS (2)=MXCREATEFULL (L ,N ,0)
               CALL MXCOPYREAL8TOPTR (stk(C),MXGETPR (PLHS (2)),NL )
               IP =2
            END IF 
         ELSE 
            IP =0
         END IF 
C         
         IF (NLHS.GT.IP )THEN 
            IF (IJOB.EQ.1 .OR.IJOB.GE.3 )THEN 
               IP =IP +1
               PLHS (IP )=MXCREATEFULL (N ,M ,0)
               CALL MXCOPYREAL8TOPTR (stk(B),MXGETPR (PLHS (IP )),N *M )
            END IF 
            IF (NLHS.GT.IP )THEN 
               IF (IJOB.EQ.1 .OR.IJOB.EQ.4 )THEN 
                  IP =IP +1
                  PLHS (IP )=MXCREATEFULL (L ,M ,0)
                  CALL MXCOPYREAL8TOPTR (stk(D),MXGETPR (PLHS (IP )),
     $               L *M )
               END IF 
            END IF 
         END IF 
C
         IF (NSMPL.GT.0 .AND.NLHS.GT.IP )THEN 
            IP =IP +1
            PLHS (IP )=MXCREATEFULL (N ,L ,0)
            CALL MXCOPYREAL8TOPTR (stk(K),MXGETPR (PLHS (IP )),NL )
         END IF 
C
         IF (NSMPL.GT.0 .AND.NLHS.GT.IP )THEN 
            IP =IP +1
            PLHS (IP )=MXCREATEFULL (N ,N ,0)
            CALL MXCOPYREAL8TOPTR (stk(Q),MXGETPR (PLHS (IP )),NN )
            IP =IP +1
            PLHS (IP )=MXCREATEFULL (L ,L ,0)
            CALL MXCOPYREAL8TOPTR (stk(RY),MXGETPR (PLHS (IP )),LL )
            IP =IP +1
            PLHS (IP )=MXCREATEFULL (N ,L ,0)
            CALL MXCOPYREAL8TOPTR (stk(S),MXGETPR (PLHS (IP )),NL )
         END IF 
C
         IF (NLHS.GT.IP )THEN 
            IP =IP +1
            IF (NSMPL.EQ.0 )THEN 
               NRC =4
            ELSE 
               NRC =12
            END IF 
            PLHS (IP )=MXCREATEFULL (NRC ,1,0)
            CALL MXCOPYREAL8TOPTR (stk(DWORK+2-1),MXGETPR (PLHS (IP )),
     $         NRC )
         END IF 
      END IF 
C
C Deallocate local arrays.
C !Fortran 90/95
C
C     DEALLOCATE( A, B, C, D, R, Q, RY, S, IWORK, DWORK )
C     DEALLOCATE( K, BWORK )
C
C Error and warning handling.
C
      IF (IWARN.NE.0 .AND.PRINTW )THEN 
         CALL MEXPRINTF (TEXT )
      END IF 
C
      IF (INFO.NE.0 )THEN 
         CALL MEXERRMSGTXT (TEXT )
      END IF 
C
      RETURN 
C *** Last line of SIDENT ***
      END 

