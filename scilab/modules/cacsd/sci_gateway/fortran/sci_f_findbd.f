C FINDBD.F - Gateway function for computing the initial state and/or
C            the matrices B and D of a discrete-time linear system, 
C            given the system matrices A, C, and possibly B, D, and the
C            input and output trajectories, using SLICOT routine IB01CD.
C
C RELEASE 4.0, WGS COPYRIGHT 2000.
C
C scilab call:  
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
      INCLUDE 'stack.h'
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
C Determine the lengths of working arrays.
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
C Copy inputs from scilab workspace to locally allocated arrays.
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
C Copy output to scilab workspace.
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

c     =======================================================





