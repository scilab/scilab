      SUBROUTINE DCUTET(FUNSUB,NUMFUN,VER,NUMTET,MINPTS,MAXPTS,EPSABS,
     +                  EPSREL,LENVER,NW,RESTAR,RESULT,ABSERR,NEVAL,
     +                  IFAIL,WORK,IWORK)
C***BEGIN PROLOGUE DCUTET
C***DATE WRITTEN   900220   (YYMMDD)
C***REVISION DATE  900307   (YYMMDD)
C***CATEGORY NO. H2B2A1/H2B2A2
C***KEYWORDS QUADRATURE, THREE DIMENSIONAL, ADAPTIVE, CUBATURE
C***AUTHORS
C            Jarle Berntsen, The Computing Centre,
C            University of Bergen, Thormohlens gt. 55,
C            N-5008 Bergen, Norway
C            Phone..  47-5-544055
C            Email..  jarle@eik.ii.uib.no
C
C            Ronald Cools, Dept. of Computer Science,
C            Katholieke Universiteit Leuven, Celestijnenlaan 200A,
C            B-3030 Heverlee, Belgium
C            Phone..  32-16-201015 (3562)
C            Email..  ronald@cs.kuleuven.ac.be
C
C            Terje O. Espelid, Department of Informatics,
C            University of Bergen, Thormohlens gt. 55,
C            N-5008 Bergen, Norway
C            Phone..  47-5-544180
C            Email..  terje@eik.ii.uib.no
C
C***PURPOSE  To compute the three-dimensional integral over a region
C            consisting of NUMTET tetrahedrons.
C***DESCRIPTION
C
C            The routine calculates an approximation to a given
C            vector of definite integrals
C
C            I  I  I (F ,F ,...,F )      DX(3)DX(2)DX(1),
C                      1  2      NUMFUN
C
C            where the region of integration is a collection of
C            NUMTET tetrahedrons and
C            where F = F (X(1),X(2),X(3)), J = 1,2,...,NUMFUN.
C                   J   J
C
C
C            A globally adaptive strategy is applied in order to
C            compute approximations RESULT(K)
C            hopefully satisfying, for each component of I, the
C            following claim for accuracy:
C            ABS(I(K)-RESULT(K)).LE.MAX(EPSABS,EPSREL*ABS(I(K)))
C
C            DCUTET is a driver for the integration routine
C            DADTET.
C
C            DADTET repeatedly
C            subdivides the tetrahedrons with greatest estimated  errors
C            and estimates the integrals and the
C            errors over the new subtetrahedrons
C            until the error request
C            is met or MAXPTS function evaluations have been used.
C
C            A 43 point integration rule
C            with all evaluation points inside the tetrahedron
C            is applied. The rule has polynomial degree 8.
C
C            If the values of the input parameters EPSABS
C            or EPSREL are selected great enough,
C            an integration rule is applied over each tetrahedron and
C            the results are added up to give the approximations
C            RESULT(K). No further subdivision
C            of the tetrahedrons will then be applied.
C
C            When DCUTET computes estimates to a vector of
C            integrals, all components of the vector are given
C            the same treatment. That is, I(F ) and I(F ) for
C                                            J         K
C            J not equal to K, are estimated with the same
C            subdivision of the region of integration.
C            For integrals with enough similarity, we may save
C            time by applying DCUTET to all integrands in one call.
C            For integrals that varies continuously as functions of
C            some parameter, the estimates produced by DCUTET will
C            also vary continuously when the same subdivision is
C            applied to all components. This will generally not be
C            the case when the different components are given
C            separate treatment.
C
C            On the other hand this feature should be used with
C            caution when the different components of the integrals
C            require clearly different subdivisions.
C
C   ON ENTRY
C
C     FUNSUB Externally declared subroutine for computing
C            all components of the integrand at the given
C            evaluation point.
C            It must have parameters (X,NUMFUN,FUNVLS)
C            Input parameters:
C              X(1)   The x-coordinate of the evaluation point.
C              X(2)   The y-coordinate of the evaluation point.
C              X(3)   The z-coordinate of the evaluation point.
C              NUMFUN Integer that defines the number of
C                     components of I.
C            Output parameter:
C              FUNVLS Real array of dimension NUMFUN
C                     that defines NUMFUN components of the integrand.
C
C     NUMFUN Integer.
C            Number of components of the integral.
C     VER    Real array of dimension (3,4,LENVER).
C            VER(1,K,L), VER(2,K,L) and VER(3,K,L) are the x, y and z
C            coordinates respectively of vertex K in tetrahedron L.
C            On entry VER(*,*,L) must contain the vertices of the
C            NUMTET user specified tetrahedrons for L = 1,2,...,NUMTET.
C     NUMTET Integer.
C            The number of tetrahedrons.
C     MINPTS Integer.
C            Minimum number of function evaluations.
C     MAXPTS Integer.
C            Maximum number of function evaluations.
C            The number of function evaluations over each subregion
C            is 43.
C
C            MAXPTS >= 43*NUMTET and MAXPTS >= MINPTS
C
C     EPSABS Real.
C            Requested absolute error.
C     EPSREL Real.
C            Requested relative error.
C     LENVER Integer.
C            Defines the length of the array VER.
C
C            We let
C            MAXSUB denote the maximum allowed number of subregions
C            for the given value of MAXPTS.
C            MAXSUB = 7*(MAXPTS-43*NUMTET)/(8*43) + NUMTET
C            LENVER should be greater or equal to MAXSUB.
C
C     NW     Integer.
C            Defines the length of the working array WORK.
C
C            We let
C            MAXSUB denote the maximum allowed number of subregions
C            for the given values of MAXPTS.
C            MAXSUB = 7*(MAXPTS-43*NUMTET)/(8*43) + NUMTET
C            NW should then be greater or equal to
C            MAXSUB*(2*NUMFUN+1) + MAX(8*6*MDIV,6*NUMTET)*NUMFUN + 1
C            where MDIV is the number of tetrahedrons that are divided
C            in each subdivision step.
C            MDIV is default set internally in DCUTET equal to 1.
C            For efficient execution on parallel computers
C            with NPROC processors MDIV should be set equal to
C            the smallest integer such that MOD(8*MDIV,NPROC) = 0.
C
C     RESTAR Integer.
C            If RESTAR = 0, this is the first attempt to compute
C            the integral.
C            If RESTAR = 1,
C            then we restart a previous attempt.
C            In this case the only parameters for DCUTET that may
C            be changed (with respect to the previous call of DCUTET)
C            are MINPTS, MAXPTS, EPSABS, EPSREL and RESTAR.
C
C   ON RETURN
C
C     RESULT Real array of dimension NUMFUN.
C            Approximations to all components of the integral.
C     ABSERR Real array of dimension NUMFUN.
C            Estimates of absolute errors.
C     NEVAL  Integer.
C            Number of function evaluations used by DCUTET.
C     IFAIL  Integer.
C            IFAIL = 0 for normal exit.
C
C              ABSERR(K) <=  EPSABS or
C              ABSERR(K) <=  ABS(RESULT(K))*EPSREL with MAXPTS or less
C              function evaluations for all values of K,
C              1 <= K <= NUMFUN .
C
C            IFAIL = 1 if MAXPTS was too small for DCUTET
C              to obtain the required accuracy. In this case DCUTET
C              returns values of RESULT with estimated absolute
C              errors ABSERR.
C            IFAIL = 2 if NUMFUN is less than 1.
C            IFAIL = 3 if the volume of one of the initially given
C                      tetrahedrons is zero.
C            IFAIL = 4 if MAXPTS is less than 43*NUMTET.
C            IFAIL = 5 if MAXPTS is less than MINPTS.
C            IFAIL = 6 if EPSABS <= 0 and EPSREL <= 0.
C            IFAIL = 7 if LENVER is less than MAXSUB.
C            IFAIL = 8 if NW is less than MAXSUB*(2*NUMFUN+1) +
C                      NUMFUN*MAX(8*6*MDIV,6*NUMTET) + 1.
C            IFAIL = 9 if unlegal RESTAR.
C     VER    Real array of dimension (3,4,LENVER).
C            On exit VER
C            contains the vertices of the tetrahedrons used to produce
C            the approximations to the integrals.
C     WORK   Real array of dimension NW.
C            Used as working storage.
C            WORK(NW) = NSUB, the number of subregions in the data
C            structure.
C            Let WRKSUB=(NW-1-NUMFUN*7*MAX(8*MDIV,NUMTET))/
C                        (2*NUMFUN+1).
C            WORK(1),...,WORK(NUMFUN*WRKSUB) contain
C              the estimated components of the integrals over the
C              subregions.
C            WORK(NUMFUN*WRKSUB+1),...,WORK(2*NUMFUN*WRKSUB) contain
C              the estimated errors over the subregions.
C            WORK(2*NUMFUN*WRKSUB+1),...,WORK(2*NUMFUN*
C              WRKSUB+WRKSUB) contain the greatest errors
C              in each subregion.
C            WORK((2*NUMFUN+1)*WRKSUB),...,WORK(NW - 1) is used as
C              temporary storage in DADTET.
C     IWORK  Integer array of dimension LENVER + MDIV.
C            Used as working storage.
C
C           SAMPLE PROGRAM
C   The following program will approximate the integral of
C                     exp(x*x+y*y+z*z)
C   over the tetrahedron (0.,0.,0.),(1.,0.,0.),(0.,1.,0.),(0.,0.,1.)
C   and print out the values of the estimated integral, the estimated
C   error, the number of function evaluations and IFAIL.
C     PROGRAM DTEST1
C     DOUBLE PRECISION VER(3,4,50),EPSABS,EPSREL,RESULT(1),ABSERR(1),
C    +                 WORK(1000)
C     INTEGER NUMFUN,NUMTET,MINPTS,MAXPTS,LENVER,NW,RESTAR,NEVAL,
C    +        IFAIL,IWORK(51)
C     EXTERNAL F
C     VER(1,1,1) = 0.D0
C     VER(1,2,1) = 1.D0
C     VER(1,3,1) = 0.D0
C     VER(1,4,1) = 0.D0
C     VER(2,1,1) = 0.D0
C     VER(2,2,1) = 0.D0
C     VER(2,3,1) = 1.D0
C     VER(2,4,1) = 0.D0
C     VER(3,1,1) = 0.D0
C     VER(3,2,1) = 0.D0
C     VER(3,3,1) = 0.D0
C     VER(3,4,1) = 1.D0
C     EPSABS = 0.D0
C     EPSREL = 1.D-5
C     NUMFUN = 1
C     NUMTET = 1
C     MINPTS = 0
C     MAXPTS = 1000
C     LENVER = 50
C     NW = 1000
C     RESTAR = 0
C     CALL DCUTET(F,NUMFUN,VER,NUMTET,MINPTS,MAXPTS,EPSABS,
C    +            EPSREL,LENVER,NW,RESTAR,RESULT,ABSERR,NEVAL,
C    +            IFAIL,WORK,IWORK)
C     WRITE(*,*)'RESULT=',RESULT(1)
C     WRITE(*,*)'ERROR =',ABSERR(1)
C     WRITE(*,*)'NEVAL =',NEVAL
C     WRITE(*,*)'IFAIL =',IFAIL
C     END
C     SUBROUTINE F(X,NUMFUN,FUNVLS)
C     INTEGER NUMFUN
C     DOUBLE PRECISION X(3),FUNVLS(NUMFUN)
C     FUNVLS(1) = EXP(X(1)*X(1)+X(2)*X(2)+X(3)*X(3))
C     RETURN
C     END
C
C   Output produced on a SUN 3/50
C
C       RESULT=   0.22779999698986
C       ERROR =    1.9752829160298D-06
C       NEVAL =  43
C       IFAIL =  0
C
C***LONG DESCRIPTION
C
C   The information for each tetrahedron is contained in the
C   data structures VER,  WORK and IWORK.
C   VER contains the coordinates of the tetrahedrons.
C   When passed on to DADTET, WORK is split into four
C   arrays VALUES, ERRORS, GREATE and WORK2.
C   VALUES contains the estimated values of the integrals.
C   ERRORS contains the estimated errors.
C   GREATE contains the greatest estimated error for each tetrahedron.
C   The data structures for the tetrahedrons are in DADTET organized
C   as a heap and the size of GREATE(I) defines the position of
C   tetrahedron I in the heap. The heap is maintained by the program
C   DTRTET and we use a partially ordered list of pointers, saved in
C   IWORK. When passed on to DADTET, IWORK is split into two
C   arrays LIST and VACANT. LIST is a partially ordered list
C   such that GREATE(LIST(1)) is the maximum error estimate
C   for all subtetrahedrons in our datastructure.
C   VACANT is a work space needed in th updating process of the list.
C
C   The subroutine DADTET is written for efficient execution on shared
C   memory parallel computers. On a computer with NPROC processors we
C   will in each subdivision step divide MDIV tetrahedrons, where MDIV
C   is chosen such that MOD(8*MDIV,NPROC) = 0, in totally 8*MDIV new
C   tetrahedrons.  Each processor will then compute estimates of the
C   integrals and errors over 8*MDIV/NPROC tetrahedrons in each
C   subdivision step.
C   The subroutine for estimating the integral and the error over
C   each subregion, DRLTET, uses WORK2 as a work array.
C   We must make sure that each processor writes its results to
C   separate parts of the memory, and therefore the sizes of WORK and
C   WORK2 are functions of MDIV.
C   In order to achieve parallel processing of tetrahedrons, compiler
C   directives should be placed in front of the DO 20 and the DO 200
C   loops in DADTET on machines like Alliant and CRAY.
C
C***REFERENCES
C   J.Berntsen, R. Cools and T.O.Espelid, An Algorithm for Adaptive
C   Cubature Over a Collection of Tetrahedrons, to be published.
C
C***ROUTINES CALLED DCHTET,DADTET
C***END PROLOGUE DCUTET
C
C   Parameters
C
C   MDIV   Integer.
C          MDIV is the number of tetrahedrons that are divided in
C          each subdivision step in DADTET.
C          MDIV is chosen default to 1.
C          For efficient execution on parallel computers
C          with NPROC processors MDIV should be set equal to
C          the smallest integer such that MOD(8*MDIV,NPROC) = 0.
C
      INTEGER MDIV
      PARAMETER (MDIV=1)

C
C   Global variables.
C
      EXTERNAL FUNSUB
      common/ierdcu/iero
      INTEGER NUMFUN,NUMTET,MINPTS,MAXPTS,LENVER,NW,RESTAR,NEVAL,IFAIL,
     +        IWORK(LENVER+MDIV)
      DOUBLE PRECISION VER(3,4,LENVER),EPSABS,EPSREL,RESULT(NUMFUN),
     +                 ABSERR(NUMFUN),WORK(NW)
C
C   Local variables.
C
C   MAXSUB Integer.
C          The maximum allowed number of subdivisions
C          for the given values of MAXPTS.
C   MINSUB Integer.
C          The minimum allowed number of subregions for the given
C          values of MINPTS.
C   WRKSUB Integer.
C          The maximum allowed number of subregions as a function
C          of NW, NUMFUN and MDIV. This determines the length
C          of the main work arrays.
C
      INTEGER MAXSUB,MINSUB,NSUB,LENW
      INTEGER WRKSUB,I1,I2,I3,I4,I5
C
C***FIRST EXECUTABLE STATEMENT DCUTET
C
C   Call DCHTET to compute MAXSUB and MINSUB,
C   and to check the input parameters.
C
      iero=0
      CALL DCHTET(NUMFUN,MDIV,VER,NUMTET,MINPTS,MAXPTS,EPSABS,EPSREL,
     +            LENVER,NW,RESTAR,MAXSUB,MINSUB,IFAIL)
C
C   All tests on input data passed?
C
      IF (IFAIL.NE.0) THEN
          RETURN
      END IF
C
C   Split up the work space.
C
      WRKSUB = (NW-1-NUMFUN*7*MAX(8*MDIV,NUMTET))/ (2*NUMFUN+1)
      I1 = 1
      I2 = I1 + WRKSUB*NUMFUN
      I3 = I2 + WRKSUB*NUMFUN
      I4 = I3 + WRKSUB
      I5 = I4 + 6*MAX(8*MDIV,NUMTET)*NUMFUN
C
C   On restart runs the number of subregions from the
C   previous call is assigned to NSUB.
C
      IF (RESTAR.EQ.1) THEN
          NSUB = WORK(NW)
      END IF
C
C   Compute the size of the temporary work space needed in DADTET.
C
      LENW = MAX(8*MDIV,NUMTET)*NUMFUN
      CALL DADTET(NUMFUN,MDIV,VER,NUMTET,MINSUB,MAXSUB,FUNSUB,EPSABS,
     +            EPSREL,LENVER,RESTAR,LENW,RESULT,ABSERR,NEVAL,NSUB,
     +            IFAIL,WORK(I1),WORK(I2),WORK(I3),WORK(I4),WORK(I5),
     +            IWORK(1),IWORK(1+LENVER))
      WORK(NW) = NSUB
      RETURN
C
C***END DCUTET
C
      END

      SUBROUTINE DCHTET(NUMFUN,MDIV,VER,NUMTET,MINPTS,MAXPTS,EPSABS,
     +                  EPSREL,LENVER,NW,RESTAR,MAXSUB,MINSUB,IFAIL)
C***BEGIN PROLOGUE DCHTET
C***REFER TO DCUTET
C***PURPOSE  DCHTET checks the validity of the
C            input parameters to DCUTET.
C***DESCRIPTION
C            DCHTET computes MAXSUB, MINSUB and IFAIL as
C            functions of the input parameters to DCUTET,
C            and checks the validity of the input parameters to DCUTET.
C
C   ON ENTRY
C
C     NUMFUN Integer.
C            Number of components of the integral.
C     MDIV   Integer.
C            MDIV is the number of tetrahedrons that are divided in
C            each subdivision step in DADTET.
C            MDIV is chosen default to 1.
C            For efficient execution on parallel computers
C            with NPROC processors MDIV should be set equal to
C            the smallest integer such that MOD(8*MDIV,NPROC) = 0.
C     VER    Real array of dimension (3,4,LENVER).
C            VER(1,K,L), VER(2,K,L) and VER(3,K,L) are the x, y and z
C            coordinates respectively of vertex K in tetrahedron L.
C            On entry VER(*,*,L) must contain the vertices of the
C            NUMTET user specified tetrahedrons for L = 1,2,...,NUMTET.
C     NUMTET Integer.
C            The number of tetrahedrons.
C     MINPTS Integer.
C            Minimum number of function evaluations.
C     MAXPTS Integer.
C            Maximum number of function evaluations.
C            The number of function evaluations over each subregion
C            is NUM.
C            MAXPTS >= NUM*NUMTET and MAXPTS >= MINPTS
C
C     EPSABS Real.
C            Requested absolute error.
C     EPSREL Real.
C            Requested relative error.
C     LENVER Integer.
C            Defines the length of the array VER.
C
C            We let
C            MAXSUB denote the maximum allowed number of subregions
C            for the given values of MAXPTS.
C            MAXSUB = 7*(MAXPTS-NUM*NUMTET)/(8*NUM) + NUMTET
C            LENVER should then be greater or equal to MAXSUB.
C
C     NW     Integer.
C            Defines the length of the working array WORK.
C
C            We let
C            MAXSUB denote the maximum allowed number of subregions
C            for the given values of MAXPTS.
C            MAXSUB = 7*(MAXPTS-NUM*NUMTET)/(8*NUM) + NUMTET
C            NW should then be greater or equal to
C            MAXSUB*(2*NUMFUN+1) + 6*MAX(8*MDIV,NUMTET)*NUMFUN + 1
C
C     RESTAR Integer.
C            If RESTAR = 0, this is the first attempt to compute
C            the integral.
C            If RESTAR = 1,
C            then we restart a previous attempt.
C            In this case the only parameters for DCUTET that may
C            be changed (with respect to the previous call of DCUTET)
C            are MINPTS, MAXPTS, EPSABS, EPSREL and RESTAR.
C
C
C   ON RETURN
C
C     MAXSUB Integer.
C            The maximum allowed number of subregions for the
C            given values of MAXPTS.
C     MINSUB Integer.
C            The minimum allowed number of subregions for the given
C            values of MINPTS.
C     IFAIL  Integer.
C            IFAIL = 0 for normal exit.
C            IFAIL = 2 if NUMFUN is less than 1.
C            IFAIL = 3 if the volume of one of the initially given
C                      tetrahedrons is zero.
C            IFAIL = 4 if MAXPTS is less than NUM*NUMTET.
C            IFAIL = 5 if MAXPTS is less than MINPTS.
C            IFAIL = 6 if EPSABS <= 0 and EPSREL <= 0.
C            IFAIL = 7 if LENVER is less than MAXSUB.
C            IFAIL = 8 if NW is less than MAXSUB*(2*NUMFUN+1) +
C                      NUMFUN*6*MAX(8*MDIV,NUMTET) + 1.
C            IFAIL = 9 if illegal value of RESTAR.
C
C***ROUTINES CALLED-NONE
C***END PROLOGUE DCHTET
C
C   Global variables.
C
      INTEGER NUMFUN,MDIV,NUMTET,MINPTS,MAXPTS,NW,MAXSUB,MINSUB,RESTAR,
     +        LENVER,IFAIL
      DOUBLE PRECISION VER(3,4,NUMTET),EPSABS,EPSREL

C
C   Local variables. 
C   NUM is set equal to the number of evaluation points used
C   by the cubature rule over each tetrahedron.
C
      INTEGER LIMIT,J,NUM
      DOUBLE PRECISION VOLUME
C
C***FIRST EXECUTABLE STATEMENT DCHTET
C
      IFAIL = 0
      NUM = 43
C
C   Compute MAXSUB and MINSUB.
C
      MAXSUB = 7* (MAXPTS-NUM*NUMTET)/ (8*NUM) + NUMTET
      MINSUB = 7* (MINPTS-NUM*NUMTET)/ (8*NUM) + NUMTET
      IF (MOD(7* (MINPTS-NUM*NUMTET),8*NUM).GT.0) THEN
          MINSUB = MINSUB + 1
      END IF
      MINSUB = MAX(NUMTET,MINSUB)
C
C   Check on positive NUMFUN.
C
      IF (NUMFUN.LT.1) THEN
          IFAIL = 2
          RETURN
      END IF
C
C   Check on legal volume of each subregion of integration.
C
      DO 10 J = 1,NUMTET
           VOLUME = (VER(1,2,J)-VER(1,1,J))*
     +     ((VER(2,3,J)-VER(2,1,J))*(VER(3,4,J)-VER(3,1,J)) -
     +      (VER(2,4,J)-VER(2,1,J))*(VER(3,3,J)-VER(3,1,J))) -
     +              (VER(2,2,J)-VER(2,1,J))*
     +     ((VER(1,3,J)-VER(1,1,J))*(VER(3,4,J)-VER(3,1,J)) - 
     +      (VER(1,4,J)-VER(1,1,J))*(VER(3,3,J)-VER(3,1,J))) +
     +              (VER(3,2,J)-VER(3,1,J))* 
     +     ((VER(1,3,J)-VER(1,1,J))*(VER(2,4,J)-VER(2,1,J)) -  
     +      (VER(1,4,J)-VER(1,1,J))*(VER(2,3,J)-VER(2,1,J)))
           VOLUME = VOLUME/6
          IF (VOLUME.EQ.0) THEN
              IFAIL = 3
              RETURN
          END IF
   10 CONTINUE
C
C   Check on MAXPTS >= NUM*NUMTET.
C
      IF (MAXPTS.LT.NUM*NUMTET) THEN
          IFAIL = 4
          RETURN
      END IF
C
C   Check on MAXPTS >= MINPTS.
C
      IF (MAXPTS.LT.MINPTS) THEN
          IFAIL = 5
          RETURN
      END IF
C
C   Check on legal accuracy requests.
C
      IF (EPSABS.LE.0 .AND. EPSREL.LE.0) THEN
          IFAIL = 6
          RETURN
      END IF
C
C   Check on big enough LENVER.
C
      IF (LENVER.LT.MAXSUB) THEN
          IFAIL = 7
          RETURN
      END IF
C
C   Check on big enough NW.
C
      LIMIT = MAXSUB* (2*NUMFUN+1) + 7*MAX(8*MDIV,NUMTET)*NUMFUN + 1
      IF (NW.LT.LIMIT) THEN
          IFAIL = 8
          RETURN
      END IF
C
C    Check on legal RESTAR.
C
      IF (RESTAR.NE.0 .AND. RESTAR.NE.1) THEN
          IFAIL = 9
      END IF
      RETURN
C
C***END DCHTET
C
      END

      SUBROUTINE DADTET(NUMFUN,MDIV,VER,NUMTET,MINSUB,MAXSUB,FUNSUB,
     +                  EPSABS,EPSREL,LENVER,RESTAR,LENW,RESULT,ABSERR,
     +                  NEVAL,NSUB,IFAIL,VALUES,ERRORS,GREATE,WORK2,
     +                  WORK3,LIST,VACANT)
C***BEGIN PROLOGUE DADTET
C***REFER TO DCUTET
C***DESCRIPTION Computation of integrals over regions consisting of
C            a collection of tetrahedrons.
C
C            DADTET repeatedly
C            subdivides the tetrahedrons with greatest estimated  errors
C            and estimates the integrals and the errors over the new
C            new sub-tetrahedrons until the error request is met
C            or MAXPTS function evaluations have been used.
C
C            A 43 point integration rule
C            with all evaluation points inside the tetrahedron
C            is applied. The rule has polynomial degree 8.
C
C   ON ENTRY
C
C     NUMFUN Integer.
C            Number of components of the integral.
C     MDIV   Integer.
C            MDIV is the number of tetrahedrons that are divided in
C            each subdivision step in DADTET.
C            MDIV is chosen default to 1.
C            For efficient execution on parallel computers
C            with NPROC processors MDIV should be set equal to
C            the smallest integer such that MOD(8*MDIV,NPROC) = 0.
C     VER    Real array of dimension (3,4,LENVER).
C            VER(1,K,L), VER(2,K,L) and VER(3,K,L) are the x, y and z
C            coordinates respectively of vertex K in tetrahedron L.
C            On entry VER(*,*,L) must contain the vertices of the
C            NUMTET user specified tetrahedrons for L = 1,2,...,NUMTET.
C     NUMTET Integer.
C            The number of tetrahedrons.
C     MINSUB Integer.
C            The minimum allowed number of subregions.
C     MAXSUB Integer.
C            The maximum allowed number of subregions.
C     FUNSUB Externally declared subroutine for computing
C            all components of the integrand at the given
C            evaluation point.
C            It must have parameters (X,NUMFUN,FUNVLS)
C            Input parameters:
C              X(1)   The x-coordinate of the evaluation point.
C              X(2)   The y-coordinate of the evaluation point.
C              X(3)   The z-coordinate of the evaluation point.
C              NUMFUN Integer that defines the number of
C                     components of I.
C            Output parameter:
C              FUNVLS Real array of dimension NUMFUN
C                     that defines NUMFUN components of the integrand.
C
C     EPSABS Real.
C            Requested absolute error.
C     EPSREL Real.
C            Requested relative error.
C
C     LENVER Integer.
C            Defines the length of the array VER.
C
C            We let
C            MAXSUB denote the maximum allowed number of subregions
C            for the given values of MAXPTS.
C            MAXSUB = 7*(MAXPTS-43*NUMTET)/(8*43) + NUMTET
C            LENVER should then be greater or equal to MAXSUB.
C
C     RESTAR Integer.
C            If RESTAR = 0, this is the first attempt to compute
C            the integral.
C            If RESTAR = 1,
C            then we restart a previous attempt.
C            In this case the only parameters for DCUTET that may
C            be changed (with respect to the previous call of DCUTET)
C            are MINPTS, MAXPTS, EPSABS, EPSREL and RESTAR.
C     LENW   Integer.
C            Length of the workspace WORK2.
C
C   ON RETURN
C
C     RESULT Real array of dimension NUMFUN.
C            Approximations to all components of the integral.
C     ABSERR Real array of dimension NUMFUN.
C            Estimates of absolute errors.
C     NEVAL  Integer.
C            Number of function evaluations used by DCUTET.
C     NSUB   Integer.
C            The number of tetrahedrons in the data structure.
C     IFAIL  Integer.
C            IFAIL = 0 for normal exit.
C
C              ABSERR(K) <=  EPSABS or
C              ABSERR(K) <=  ABS(RESULT(K))*EPSREL with MAXPTS or less
C              function evaluations for all values of K,
C              1 <= K <= NUMFUN .
C
C            IFAIL = 1 if MAXSUB was too small for DADTET
C              to obtain the required accuracy. In this case DADTET
C              returns values of RESULT with estimated absolute
C              errors ABSERR.
C     VALUES Real array of dimension (NUMFUN,MAXSUB).
C            The estimated components of the integrals over the
C            subregions.
C     ERRORS Real array of dimension (NUMFUN,MAXSUB).
C            The estimated errors over the subregions.
C     GREATE Real array of dimension MAXSUB.
C            The greatest errors in each subregion.
C     WORK2  Real array of dimension 6*LENW.
C            Work array used in DRLTET.
C     WORK3  Real array of dimension LENW.
C            Work array used in DRLTET.
C     LIST   Integer array used in DTRTET of dimension LENVER.
C            Is a partially sorted list, where LIST(1) is the top
C            element in a heap of subregions.
C     VACANT Integer array of dimension MDIV
C            Used as intermediate storage of pointers.
C***ROUTINES CALLED DTRTET,DRLTET
C***END PROLOGUE DADTET
C
C   Global variables.
C
      EXTERNAL FUNSUB
      common/ierdcu/iero

      INTEGER NUMFUN,MDIV,NUMTET,MINSUB,MAXSUB,LENW,RESTAR,LENVER,NEVAL,
     +        NSUB,IFAIL,LIST(LENVER),VACANT(MDIV)
      DOUBLE PRECISION VER(3,4,LENVER),EPSABS,EPSREL,RESULT(NUMFUN),
     +                 ABSERR(NUMFUN),VALUES(NUMFUN,MAXSUB),
     +                 ERRORS(NUMFUN,MAXSUB),GREATE(MAXSUB),WORK3(LENW),
     +                 WORK2(6*LENW)

C
C   Local variables.
C
C   SBRGNS is the number of stored subregions.
C   NDIV   The number of subregions to be divided in each main step.
C   POINTR Pointer to the position in the data structure where
C          the new subregions are to be stored.
C   G      is the homogeneous coordinates of the integration rule.
C   W      is the weights of the integration rule and the null rules.
C   TOP    is a pointer to the top element in the heap of subregions.
C   COUNT  is the number of times the termination criterium is
C          satisfied.
      INTEGER I,J,K,L,TOP,SIZE
      INTEGER SBRGNS,I1,I2,I3,I4,I5,I6,I7,I8
      INTEGER L1,K1
      INTEGER COUNT,GROUP,LEFT,NDIV,POINTR,INDEX,NUM
      DOUBLE PRECISION VEROLD(3,4),VERNEW(3,6),SUMVAL(2),SUMERR(2)
C
C***FIRST EXECUTABLE STATEMENT DADTET
C
      COUNT = 0
      NUM = 43
      IF (RESTAR.EQ.1) THEN
          SBRGNS = NSUB
          GO TO 110
      END IF
C
C   Initiate RESULT and ABSERR.
C
      DO 10 J = 1,NUMFUN
          RESULT(J) = 0
          ABSERR(J) = 0
   10 CONTINUE
C
C   Apply DRLTET over the NUMTET tetrahedrons.
C   This loop may be run in parallel.
C
      DO 20 I = 1,NUMTET
          L1 = 1 + (I-1)*6*NUMFUN
          K1 = 1 + (I-1)*NUMFUN
          CALL DRLTET(VER(1,1,I),NUMFUN,FUNSUB,WORK2(L1),VALUES(1,I),
     +                ERRORS(1,I),GREATE(I),WORK3(K1))
          if(iero.ne.0) return
   20 CONTINUE
      NEVAL = NUM*NUMTET
      SBRGNS = NUMTET
C
C   Add the computed values to RESULT and ABSERR.
C
      DO 40 I = 1,NUMTET
          DO 30 J = 1,NUMFUN
              RESULT(J) = RESULT(J) + VALUES(J,I)
              ABSERR(J) = ABSERR(J) + ERRORS(J,I)
   30     CONTINUE
   40 CONTINUE
C
C   Store results in heap.
C
      DO 50 I = 1,NUMTET
          INDEX = I
          CALL DTRTET(2,INDEX,GREATE,LIST,(INDEX))
   50 CONTINUE

C
C   We check for termination.
C
      IF (SBRGNS.LT.MINSUB) THEN
          GO TO 110
      END IF
      DO 60 J = 1,NUMFUN
          IF (ABSERR(J).GT.EPSREL*ABS(RESULT(J)) .AND.
     +        ABSERR(J).GT.EPSABS) THEN
              GO TO 110
          END IF
   60 CONTINUE
      IFAIL = 0
      COUNT = COUNT + 1
      GO TO 499
C
C***End initiation.
C
C***Begin loop while the error is too great,
C   and SBRGNS+7 is less than MAXSUB.
C
  110 IF (SBRGNS+7.LE.MAXSUB) THEN
C
C   If we are allowed to divide further,
C   prepare to apply basic rule over  the tetrahedrons produced
C   by dividing the
C   NDIV sub-tetrahedrons with greatest errors.
C   If MAXSUB and SBRGNS are great enough, NDIV = MDIV.
C
          NDIV = MAXSUB - SBRGNS
          NDIV = MIN(NDIV,MDIV,SBRGNS)
C
C   Divide each of the NDIV sub-tetrahedrons in eight new
C   sub-tetrahedrons, and compute integral and error over each.
C   When we pick a tetrahedron to divide in eight one of the
C   new sub-tetrahedron is stored in the original tetrahedrons'
C   position in the datastructure. Thus we get 7*NDIV new
C   elements in the datastructure after the subdivision. The size
C   of the datastructure before the sub-division
C   is stored in the variable SIZE, while SBRGNS is the size of
C   heap at any time.
          SIZE = SBRGNS
          DO 150 I = 1,NDIV
              POINTR = SIZE + 7* (NDIV+1-I)
C
C   Adjust RESULT and ABSERR. TOP is a pointer to the top of the heap.
C
              TOP = LIST(1)
              VACANT(I) = TOP
              DO 115 J = 1,NUMFUN
                  RESULT(J) = RESULT(J) - VALUES(J,TOP)
                  ABSERR(J) = ABSERR(J) - ERRORS(J,TOP)
  115         CONTINUE
C
C   Save the vertices.
C
              DO 130 L = 1,3
                  DO 135 K = 1,4
                      VEROLD(L,K) = VER(L,K,TOP)
  135             CONTINUE
  130         CONTINUE
C
C   Adjust the heap.
C
              CALL DTRTET(1,SBRGNS,GREATE,LIST,K)
C
C   Compute the eight new tetrahedrons. Store 7 in consecutive
C   positions and 1 in the vacant position.
C
              I1 = TOP
              I2 = POINTR - 6
              I3 = POINTR - 5
              I4 = POINTR - 4
              I5 = POINTR - 3
              I6 = POINTR - 2
              I7 = POINTR - 1
              I8 = POINTR
C
C   Compute the 6 division points.
C
              DO 140 L = 1,3
                  VERNEW(L,1) = (VEROLD(L,1)+VEROLD(L,2))/2
                  VERNEW(L,2) = (VEROLD(L,2)+VEROLD(L,3))/2
                  VERNEW(L,3) = (VEROLD(L,1)+VEROLD(L,3))/2
                  VERNEW(L,4) = (VEROLD(L,1)+VEROLD(L,4))/2
                  VERNEW(L,5) = (VEROLD(L,2)+VEROLD(L,4))/2
                  VERNEW(L,6) = (VEROLD(L,3)+VEROLD(L,4))/2
  140         CONTINUE
C
              DO 145 L = 1,3
                  VER(L,1,I1) = VEROLD(L,1)
                  VER(L,2,I1) = VERNEW(L,1)
                  VER(L,3,I1) = VERNEW(L,3)
                  VER(L,4,I1) = VERNEW(L,4)
                  VER(L,1,I2) = VEROLD(L,2)
                  VER(L,2,I2) = VERNEW(L,1)
                  VER(L,3,I2) = VERNEW(L,2)
                  VER(L,4,I2) = VERNEW(L,5)
                  VER(L,1,I3) = VEROLD(L,3)
                  VER(L,2,I3) = VERNEW(L,2)
                  VER(L,3,I3) = VERNEW(L,3)
                  VER(L,4,I3) = VERNEW(L,6)
                  VER(L,1,I4) = VEROLD(L,4)
                  VER(L,2,I4) = VERNEW(L,4)
                  VER(L,3,I4) = VERNEW(L,5)
                  VER(L,4,I4) = VERNEW(L,6)
                  VER(L,1,I5) = VERNEW(L,1)
                  VER(L,2,I5) = VERNEW(L,3)
                  VER(L,3,I5) = VERNEW(L,4)
                  VER(L,4,I5) = VERNEW(L,5)
                  VER(L,1,I6) = VERNEW(L,1)
                  VER(L,2,I6) = VERNEW(L,2)
                  VER(L,3,I6) = VERNEW(L,3)
                  VER(L,4,I6) = VERNEW(L,5)
                  VER(L,1,I7) = VERNEW(L,2)
                  VER(L,2,I7) = VERNEW(L,3)
                  VER(L,3,I7) = VERNEW(L,5)
                  VER(L,4,I7) = VERNEW(L,6)
                  VER(L,1,I8) = VERNEW(L,3)
                  VER(L,2,I8) = VERNEW(L,4)
                  VER(L,3,I8) = VERNEW(L,5)
                  VER(L,4,I8) = VERNEW(L,6)
  145         CONTINUE
  150     CONTINUE
C
C   Apply basic rule.
C   This loop may be run in parallel.
C
          DO 200 I = 1,8*NDIV
              IF (I.LE.NDIV) THEN
                  INDEX = VACANT(I)
              ELSE
                  INDEX = SBRGNS + I
              END IF
              L1 = 1 + (I-1)*6*NUMFUN
              K1 = 1 + (I-1)*NUMFUN
              CALL DRLTET(VER(1,1,INDEX),NUMFUN,FUNSUB,WORK2(L1),
     +                    VALUES(1,INDEX),ERRORS(1,INDEX),GREATE(INDEX),
     +                    WORK3(K1))
              if(iero.ne.0) return
  200     CONTINUE
          NEVAL = NEVAL + 8*NDIV*NUM
C
C   Add new contributions to RESULT and ABSERR.
C
          DO 220 I = 1,8*NDIV
              IF (I.LE.NDIV) THEN
                  INDEX = VACANT(I)
              ELSE
                  INDEX = SBRGNS + I
              END IF
              DO 210 J = 1,NUMFUN
                  RESULT(J) = RESULT(J) + VALUES(J,INDEX)
                  ABSERR(J) = ABSERR(J) + ERRORS(J,INDEX)
  210         CONTINUE
  220     CONTINUE
C
C   Store results in heap.
C
          DO 250 I = 1,8*NDIV
              IF (I.LE.NDIV) THEN
                  INDEX = VACANT(I)
              ELSE
                  INDEX = SBRGNS + I
              END IF
              J = SBRGNS + I
              CALL DTRTET(2,J,GREATE,LIST,INDEX)
  250     CONTINUE
          SBRGNS = SBRGNS + 8*NDIV
C
C   Check for termination.
C
          IF (SBRGNS.LT.MINSUB) THEN
              GO TO 110
          END IF
          DO 255 J = 1,NUMFUN
              IF (ABSERR(J).GT.EPSREL*ABS(RESULT(J)) .AND.
     +            ABSERR(J).GT.EPSABS) THEN
                  GO TO 110
              END IF
  255     CONTINUE
          IFAIL = 0
          COUNT = COUNT + 1
C
C   Else we did not succeed with the
C   given value of MAXSUB.
C
      ELSE
          IFAIL = 1
      END IF
C
C   Compute more accurate values of RESULT and ABSERR. This is done
C   to reduce the effect of roundoff on final results. Large
C   intermediate sums in the computation may course large, unnecessary
C   roundoff errors. Thus recomputing the sums of errors and estimates
C   and in addition grouping the sums in three groups should remove this
C   problem.
C
  499 CONTINUE
      DO 550 J = 1,NUMFUN
          RESULT(J) = 0
          ABSERR(J) = 0
          GROUP = SBRGNS** (1.d0/3.d0)
          IF (GROUP**3.NE.SBRGNS) THEN
              GROUP = GROUP + 1
          END IF
          LEFT = SBRGNS
          DO 520 I2 = 0,SBRGNS - 1,GROUP**2
              SUMVAL(2) = 0
              SUMERR(2) = 0
              DO 510 I1 = 0,MIN(LEFT,GROUP**2) - 1,GROUP
                  SUMVAL(1) = 0
                  SUMERR(1) = 0
                  DO 500 I = 1 + I1 + I2,MIN(LEFT,GROUP) + I1 + I2
                      SUMVAL(1) = SUMVAL(1) + VALUES(J,I)
                      SUMERR(1) = SUMERR(1) + ERRORS(J,I)
  500             CONTINUE
                  LEFT = LEFT - MIN(LEFT,GROUP)
                  SUMVAL(2) = SUMVAL(2) + SUMVAL(1)
                  SUMERR(2) = SUMERR(2) + SUMERR(1)
  510         CONTINUE
              RESULT(J) = RESULT(J) + SUMVAL(2)
              ABSERR(J) = ABSERR(J) + SUMERR(2)
  520     CONTINUE
  550 CONTINUE
C
C   Check again for termination
C
      IF (IFAIL.EQ.0) THEN
          IF (COUNT.LE.1) THEN
              DO 560 J = 1,NUMFUN
                  IF (ABSERR(J).GT.EPSREL*ABS(RESULT(J)) .AND.
     +                ABSERR(J).GT.EPSABS) THEN
                      GO TO 110
                  END IF
  560         CONTINUE
          END IF
      END IF
C
      NSUB = SBRGNS
      RETURN
C
C***END DADTET
C
      END

      SUBROUTINE DTRTET(DVFLAG,SBRGNS,GREATE,LIST,NEW)
C***BEGIN PROLOGUE DTRTET
C***REFER TO DCUTET
C***PURPOSE  DTRTET maintains a heap of subregions.
C***DESCRIPTION DTRTET maintains a heap of subregions.
C            The subregions are stored in a partially sorted
C            binary tree, ordered according to the size of the
C            greatest error estimates of each subregion(GREATE).
C            The subregion with greatest error estimate is in the
C            first position of the heap.
C
C   PARAMETERS
C
C     DVFLAG Integer.
C            If DVFLAG = 1, we remove the subregion with
C            greatest error from the heap.
C            If DVFLAG = 2, we insert a new subregion in the heap.
C     SBRGNS Integer.
C            Number of subregions in the heap.
C     GREATE Real array of dimension SBRGNS.
C            Used to store the greatest estimated errors in
C            all subregions.
C     LIST   Integer array of dimension SBRGNS.
C            Used as a partially ordered list of pointers to the
C            different subregions. This list is a heap where the
C            element on top of the list is the subregion with the
C            greatest error estimate.
C     NEW    Integer.
C            Index to the new region to be inserted in the heap.
C***ROUTINES CALLED-NONE
C***END PROLOGUE DTRTET
C
C   Global variables.
C
      INTEGER DVFLAG,NEW,SBRGNS,LIST(*)
      DOUBLE PRECISION GREATE(*)
C
C   Local variables.
C
C   GREAT  is used as intermediate storage for the greatest error of a
C          subregion.
C   SUBRGN Position of child/parent subregion in the heap.
C   SUBTMP Position of parent/child subregion in the heap.
      INTEGER SUBRGN,SUBTMP
      DOUBLE PRECISION GREAT
C
C***FIRST EXECUTABLE STATEMENT DTRTET
C
C    If DVFLAG = 1, we will reduce the partial ordered list by the
C    element with greatest estimated error. Thus the element in
C    in the heap with index LIST(1) is vacant and can be used later.
C    Reducing the heap by one element implies that the last element
C    should be re-positioned.
C
      IF (DVFLAG.EQ.1) THEN
          GREAT = GREATE(LIST(SBRGNS))
          SBRGNS = SBRGNS - 1
          SUBRGN = 1
   20     SUBTMP = 2*SUBRGN
          IF (SUBTMP.LE.SBRGNS) THEN
              IF (SUBTMP.NE.SBRGNS) THEN
C
C   Find max. of left and right child.
C
                  IF (GREATE(LIST(SUBTMP)).LT.
     +                GREATE(LIST(SUBTMP+1))) THEN
                      SUBTMP = SUBTMP + 1
                  END IF
              END IF
C
C   Compare max.child with parent.
C   If parent is max., then done.
C
              IF (GREAT.LT.GREATE(LIST(SUBTMP))) THEN
C
C   Move the pointer at position subtmp up the heap.
C
                  LIST(SUBRGN) = LIST(SUBTMP)
                  SUBRGN = SUBTMP
                  GO TO 20
              END IF
          END IF
C
C   Update the pointer.
C
          IF (SBRGNS.GT.0) THEN
              LIST(SUBRGN) = LIST(SBRGNS+1)
          END IF
      ELSE IF (DVFLAG.EQ.2) THEN
C
C   If DVFLAG = 2, find the position for the NEW region in the heap.
C
          GREAT = GREATE(NEW)
          SUBRGN = SBRGNS
   40     SUBTMP = SUBRGN/2
          IF (SUBTMP.GE.1) THEN
C
C   Compare max.child with parent.
C   If parent is max, then done.
C
              IF (GREAT.GT.GREATE(LIST(SUBTMP))) THEN
C
C   Move the pointer at position subtmp down the heap.
C
                  LIST(SUBRGN) = LIST(SUBTMP)
                  SUBRGN = SUBTMP
                  GO TO 40
              END IF
          END IF
C
C    Set the pointer to the new region in the heap.
C
          LIST(SUBRGN) = NEW
      END IF
C
C***END DTRTET
C
      RETURN
      END

      SUBROUTINE DRLTET(VER,NUMFUN,FUNSUB,NULL,BASVAL,RGNERR,GREATE,
     +                  SUMVAL)
C***BEGIN PROLOGUE DRLTET
C***REFER TO DCUTET
C***PURPOSE  To compute basic integration rule values and
C            corresponding error estimates.
C***DESCRIPTION DRLTET computes basic integration rule values
C            for a vector of integrands over a tetrahedron.
C            DRLTET also computes estimates for the errors by
C            using several null rule approximations.
C   ON ENTRY
C
C   VER    Real array of dimension (3,4).
C          The coordinates of the vertices of the tetrahedron.
C          vertex i -> ( ver(1,i),ver(2,i),ver(3,i) )
C   NUMFUN Integer.
C          Number of components of the vector integrand.
C   FUNSUB Externally declared subroutine for computing
C            all components of the integrand at the given
C            evaluation point.
C            It must have parameters (X,NUMFUN,FUNVLS)
C            Input parameters:
C              X(1)   The x-coordinate of the evaluation point.
C              X(2)   The y-coordinate of the evaluation point.
C              X(3)   The z-coordinate of the evaluation point.
C              NUMFUN Integer that defines the number of
C                     components of the vector integrand.
C            Output parameter:
C              FUNVLS Real array of dimension NUMFUN
C                     that defines NUMFUN components of the integrand.
C
C   NULL   Real array of dimension (NUMFUN,6).
C          A work array.
C   SUMVAL Real array of dimension (NUMFUN).
C          A work array
C
C   ON RETURN
C
C   BASVAL Real array of dimension NUMFUN.
C          The values for the basic rule for each component
C          of the integrand.
C   RGNERR Real array of dimension NUMFUN.
C          The error estimates for each component of the integrand.
C   GREATE Real.
C          The greatest error component of the integrand.
C
C***REFERENCES Beckers M., A. Haegemans,
C              The construction of cubature formula for the tetrahedron
C              Report TW128, K.U. Leuven (1990).
C***ROUTINES CALLED
C                   DLAMCH,DORTET,FUNSUB
C***END PROLOGUE DRLTET
C
C   Parameters
C
C   ORBITS Integer
C          The number of orbits of the cubature formula and null rules
C   CRIVAL Real
C          The decision to choose the optimistic part of the error
C          estimator is based on CRIVAL
C   FACOPT Real
C          FACOPT is the safety coefficient used in the optimistic part
C          of the error estimator.
C   FACMED Real
C          FACMED is the safety coefficient used in the non-optimistic
C          part of the error estimator. FACMED is related to CRIVAL 
C          and FACOPT.
C
      INTEGER ORBITS
      DOUBLE PRECISION CRIVAL,FACOPT,FACMED
      PARAMETER (ORBITS=7)
      PARAMETER (CRIVAL=0.5,FACMED=5,FACOPT=FACMED/CRIVAL)
C
C   Global variables.
C
      INTEGER NUMFUN
      DOUBLE PRECISION VER(3,4),BASVAL(NUMFUN),RGNERR(NUMFUN),GREATE,
     +                 SUMVAL(NUMFUN),NULL(NUMFUN,6)
      EXTERNAL FUNSUB
      common/ierdcu/iero
C
C   Local variables.
C
C   K      Integer array of dimension (0:3) that contains the structure
C          parameters. K(I) = number of orbits of type I.
C   TYPE1  Real array of dimension (K(1)).
C          Contains the first homogeneous coordinate of the generators
C          of type 1
C   TYPE2  Real array of dimension (K(2)).
C          Contains the first homogeneous coordinate of the generators
C          of type 2
C   TYPE3  Real array of dimension (2,K(2)).
C          Contains the first two homogeneous coordinates of
C          the generators of type 3.
C   WEIGHT Real array of dimension (9,ORBITS).
C          The weights of the cubature formula and the null rules.
C          WEIGHT(1,1) ,..., WEIGHT(1,ORBITS) are the weights of the
C                cubature formula
C          WEIGHT(I,1) ,..., WEIGHT(I,ORBITS) for I > 1, are the weights
C                of the null rules
C
C
      INTEGER TYPE,NR,K(0:3),I,J,P
      DOUBLE PRECISION Z(3),TYPE1(3),TYPE2(1),TYPE3(2,2),NOISE,DLAMCH,
     +                 WEIGHT(7,ORBITS),VOLUME,DEG4,DEG3,DEG1,R2,R1,R,
     +                 TRES
C
C  Cubature formula of degree 8 with 43 points
C
C  Structure parameters
C
      DATA (K(I),I=0,3)/1,3,1,2/
C
C  Information for the generators
C
      DATA (TYPE1(I),I=1,3)/0.379510205167980387748057300876D+00,
     +     0.753689235068359830728182577696D+00,
     +     0.982654148484406008240470085259D+00/
      DATA (TYPE2(I),I=1,1)/0.449467259981105775574375471447D+00/
      DATA ((TYPE3(I,J),I=1,2),J=1,2)/
     +     0.506227344977843677082264893876D+00,
     +     0.356395827885340437169173969841D-01,
     +     0.736298458958971696943019005441D+00,
     +     0.190486041934633455699433285302D+00/
C
C  Weights of the cubature formula
C
      DATA (WEIGHT(1,I),I=1,ORBITS)/-
     +     0.123001131951839495043519102752D+00,
     +     0.855018349372014074906384482699D-01,
     +     0.118021998788034059253768205083D-01,
     +     0.101900465455732427902646736855D-02,
     +     0.274781029468036908044610867719D-01,
     +     0.342269148520915110408153517904D-01,
     +     0.128431148469725555789001180031D-01/
C
C  Weights of the null rule of degree 5
C
      DATA (WEIGHT(2,I),I=1,ORBITS)/0.211921237628032658308230999090D+00
     +     ,-0.660207516445726284649283745987D-01,
     +     0.225058824086711710443385047042D-01,
     +     -0.375962972067425589765730699401D-03,
     +     0.710066020561055159657284834784D-02,
     +     0.156515256061747694921427149028D-02,
     +     -0.814530839643584660306807872526D-02/
C
C  Weights of null rule of degree 4
C
      DATA (WEIGHT(3,I),I=1,ORBITS)/-
     +     0.508105488137100551376844924797D-01,
     +     0.104596681151665328209751420525D-01,
     +     0.927471438532788763594989973184D-01,
     +     0.210489990008917994323967321174D-02,
     +     0.379184172251962722213408547663D-01,
     +     -0.111747242913563605790923001557D-01,
     +     -0.386541758762774673113423570465D-01/
C
C  Weights of first null rule of degree 3
C
      DATA (WEIGHT(4,I),I=1,ORBITS)/-
     +     0.775992773232808462404390159802D-01,
     +     -0.527453289659022924847298408064D-01,
     +     0.145876238555932704488677626554D-01,
     +     0.739374873393616192857532718429D-02,
     +     -0.374618791364332892611678523428D-01,
     +     0.538502846550653076078817013885D-01,
     +     -0.183980865177843057548322735665D-01/
C
C  Weights of second null rule of degree 3
C
      DATA (WEIGHT(5,I),I=1,ORBITS)/0.181767621501470154602720474731D-01
     +     ,0.179938831310058580533178529022D-01,
     +     0.713210362750414891598257378898D-01,
     +     -0.443935688958258805893448212636D-01,
     +     -0.657639036547720234169662790056D-01,
     +     -0.101551807522541414699808460583D-01,
     +     0.265486188970540796821750584204D-01/
C
C  Weights of null rule of degree 2
C
      DATA (WEIGHT(6,I),I=1,ORBITS)/-
     +     0.867629853722843888927184699428D-01,
     +     -0.715881271235661902772072127812D-01,
     +     0.886720767790426261677273459523D-02,
     +     -0.577885573028655167063092577589D-01,
     +     0.430310167581202031805055255554D-01,
     +     -0.606467834856775537069463817445D-02,
     +     0.319492443333738343104163265406D-01/
C
C   Weights of null rule of degree 1
C
      DATA (WEIGHT(7,I),I=1,ORBITS)/0.510374015624925451319499382594D-01
     +     ,0.463998830432033721597269299429D-01,
     +     -0.191086148397852799983451475821D-01,
     +     -0.973768821003670776204287367278D-01,
     +     0.180352562073914141268335496511D-01,
     +     0.277129527093489643801598303110D-01,
     +     -0.176218263109360550515567818653D-01/
C
C  The number of points used by the cubature formula is
C     NUM    = K(0) + 4*K(1) + 6*K(2) + 12*K(3) = 43
C
C***FIRST EXECUTABLE STATEMENT DRLTRI
      TRES = 50*DLAMCH('p')
C
C  Compute the volume of the tetrahedron
C
      VOLUME = (VER(1,2)-VER(1,1))* ((VER(2,3)-VER(2,1))*
     +         (VER(3,4)-VER(3,1))- (VER(2,4)-VER(2,1))*
     +         (VER(3,3)-VER(3,1))) - (VER(2,2)-VER(2,1))*
     +         ((VER(1,3)-VER(1,1))* (VER(3,4)-VER(3,1))-
     +         (VER(1,4)-VER(1,1))* (VER(3,3)-VER(3,1))) +
     +         (VER(3,2)-VER(3,1))* ((VER(1,3)-VER(1,1))*
     +         (VER(2,4)-VER(2,1))- (VER(1,4)-VER(1,1))*
     +         (VER(2,3)-VER(2,1)))
      VOLUME = ABS(VOLUME)/6
C
C  Initialise BASVAL and NULL
C
      DO 10 J = 1,NUMFUN
          BASVAL(J) = 0.d0
          DO 20 I = 1,6
              NULL(J,I) = 0.d0
   20     CONTINUE
   10 CONTINUE
      P = 1
C
C  Compute contributions from orbits with 1, 4, 6 and 12  points
C
      DO 4 TYPE = 0,3
          DO 3 NR = 1,K(TYPE)
              IF (TYPE.EQ.1) THEN
C                 Generator ( z(1) , z(2), z(2) ; z(2) )
                  Z(1) = TYPE1(NR)
                  Z(2) = (1-Z(1))/3
              ELSE IF (TYPE.EQ.2) THEN
C                 Generator ( z(1) , z(1), z(2) ; z(2) )
                  Z(1) = TYPE2(NR)
                  Z(2) = (1-2*Z(1))/2
              ELSE
C                 Generator ( z(1) , z(2), z(3) ; z(3) )
                  Z(1) = TYPE3(1,NR)
                  Z(2) = TYPE3(2,NR)
                  Z(3) = (1-Z(1)-Z(2))/2
              END IF
C                                RGNERR is work array for DORTHE
              CALL DORTET(TYPE,Z,VER,NUMFUN,FUNSUB,SUMVAL,RGNERR)
              if(iero.ne.0) return
              DO 2 J = 1,NUMFUN
                  BASVAL(J) = BASVAL(J) + WEIGHT(1,P)*SUMVAL(J)
                  DO 1 I = 1,6
                      NULL(J,I) = NULL(J,I) + WEIGHT(I+1,P)*SUMVAL(J)
    1             CONTINUE
    2         CONTINUE
              P = P + 1
    3     CONTINUE
    4 CONTINUE
C
C  Compute error estimates
C
      GREATE = 0
      DO 30 J = 1,NUMFUN
          NOISE = ABS(BASVAL(J))*TRES
          DEG4 = SQRT(NULL(J,1)**2+NULL(J,2)**2)
          DEG3 = SQRT(NULL(J,3)**2+NULL(J,4)**2)
          IF (DEG4.LE.NOISE) THEN
              RGNERR(J) = NOISE
          ELSE
              DEG1 = SQRT(NULL(J,5)**2+NULL(J,6)**2)
              IF (DEG3.NE.0) THEN
                  R1 = (DEG4/DEG3)**2
              ELSE
                  R1 = 1
              END IF
              IF (DEG1.NE.0) THEN
                  R2 = DEG3/DEG1
              ELSE
                  R2 = 1
              END IF
              R = MAX(R1,R2)
              IF (R.GE.CRIVAL) THEN
                  RGNERR(J) = FACMED*R*DEG4
              ELSE
                  RGNERR(J) = FACOPT* (R**2)*DEG4
              END IF
              RGNERR(J) = MAX(NOISE,RGNERR(J))
          END IF
          RGNERR(J) = VOLUME*RGNERR(J)
          BASVAL(J) = VOLUME*BASVAL(J)
          GREATE = MAX(GREATE,RGNERR(J))
   30 CONTINUE
C
C***END DRLTET
C
      RETURN
      END

      SUBROUTINE DORTET(TYPE,GENER,VER,NUMFUN,FUNSUB,SUMVAL,WORK)
C***BEGIN PROLOGUE DORTET
C***PURPOSE  To compute the sum of function values over all points
C            of an orbit.
C***DESCRIPTION
C   ON ENTRY
C
C   TYPE   Integer
C          The type of the orbit.
C   GENER  Integer array of dimension (3).
C          The generator for the orbit in homogeneous coordinates.
C   VER    Real array of dimension (3,4).
C          The coordinates of the vertices of the tetrahedron.
C          vertex i -> ( ver(1,i),ver(2,i),ver(3,i) )
C   NUMFUN Integer.
C          Number of components of the vector integrand.
C   FUNSUB Externally declared subroutine for computing
C            all components of the integrand at the given
C            evaluation point.
C            It must have parameters (X,NUMFUN,FUNVLS)
C            Input parameters:
C              X(1)   The x-coordinate of the evaluation point.
C              X(2)   The y-coordinate of the evaluation point.
C              X(3)   The z-coordinate of the evaluation point.
C              NUMFUN Integer that defines the number of
C                     components of the vector integrand.
C            Output parameter:
C              FUNVLS Real array of dimension NUMFUN
C                     that defines NUMFUN components of the integrand.
C   WORK   Real array of dimension (NUMFUN).
C          A work array
C
C   ON RETURN
C
C   SUMVAL Real array of dimension (NUMFUN).
C          The sum of function values over all points
C          of the given orbit.
C
C***END PROLOGUE  DORTET
C
C  Global variables
C
      INTEGER NUMFUN,TYPE
      DOUBLE PRECISION GENER(3),SUMVAL(NUMFUN),VER(3,4),WORK(NUMFUN)
      EXTERNAL FUNSUB
      common/ierdcu/iero
C
C  Local variables
C
      INTEGER I,J,NUMBER
      DOUBLE PRECISION X(3,12),Z1,Z2,Z3
C
C  The array 'gener' contains the necessary information
C   for the generator
C
C***FIRST EXECUTABLE STATEMENT  DORTET
      GO TO (100,110,120,130),TYPE + 1
C
C  Generator with homogeneous coordinates (1/4,1/4,1/4;1/4)
C
  100 NUMBER = 1
      X(1,1) = (VER(1,1)+VER(1,2)+VER(1,3)+VER(1,4))/4
      X(2,1) = (VER(2,1)+VER(2,2)+VER(2,3)+VER(2,4))/4
      X(3,1) = (VER(3,1)+VER(3,2)+VER(3,3)+VER(3,4))/4
      GO TO 200
C
C  Generator with homogeneous coordinates (z1,z2,z2;z2)
C
  110 NUMBER = 4
      Z1 = GENER(1)
      Z2 = GENER(2)
      DO 115 J = 1,3
          X(J,1) = Z1*VER(J,1) + Z2* (VER(J,2)+VER(J,3)+VER(J,4))
          X(J,2) = Z1*VER(J,2) + Z2* (VER(J,1)+VER(J,3)+VER(J,4))
          X(J,3) = Z1*VER(J,3) + Z2* (VER(J,2)+VER(J,1)+VER(J,4))
          X(J,4) = Z1*VER(J,4) + Z2* (VER(J,2)+VER(J,3)+VER(J,1))
  115 CONTINUE
      GO TO 200
C
C  Generator with homogeneous coordinates (z1,z1,z2;z2)
C
  120 NUMBER = 6
      Z1 = GENER(1)
      Z2 = GENER(2)
      DO 125 J = 1,3
          X(J,1) = Z1* (VER(J,1)+VER(J,2)) + Z2* (VER(J,3)+VER(J,4))
          X(J,2) = Z1* (VER(J,1)+VER(J,3)) + Z2* (VER(J,2)+VER(J,4))
          X(J,3) = Z1* (VER(J,1)+VER(J,4)) + Z2* (VER(J,3)+VER(J,2))
          X(J,4) = Z1* (VER(J,2)+VER(J,3)) + Z2* (VER(J,1)+VER(J,4))
          X(J,5) = Z1* (VER(J,2)+VER(J,4)) + Z2* (VER(J,1)+VER(J,3))
          X(J,6) = Z1* (VER(J,3)+VER(J,4)) + Z2* (VER(J,1)+VER(J,2))
  125 CONTINUE
      GO TO 200
C
C  Generator with homogeneous coordinates (z1,z2,z3;z3)
C
  130 NUMBER = 12
      Z1 = GENER(1)
      Z2 = GENER(2)
      Z3 = GENER(3)
      DO 135 J = 1,3
          X(J,1) = Z1*VER(J,1) + Z2*VER(J,2) + Z3* (VER(J,3)+VER(J,4))
          X(J,2) = Z1*VER(J,1) + Z2*VER(J,3) + Z3* (VER(J,2)+VER(J,4))
          X(J,3) = Z1*VER(J,1) + Z2*VER(J,4) + Z3* (VER(J,2)+VER(J,3))
          X(J,4) = Z1*VER(J,2) + Z2*VER(J,1) + Z3* (VER(J,3)+VER(J,4))
          X(J,5) = Z1*VER(J,2) + Z2*VER(J,3) + Z3* (VER(J,1)+VER(J,4))
          X(J,6) = Z1*VER(J,2) + Z2*VER(J,4) + Z3* (VER(J,1)+VER(J,3))
          X(J,7) = Z1*VER(J,3) + Z2*VER(J,1) + Z3* (VER(J,2)+VER(J,4))
          X(J,8) = Z1*VER(J,3) + Z2*VER(J,2) + Z3* (VER(J,1)+VER(J,4))
          X(J,9) = Z1*VER(J,3) + Z2*VER(J,4) + Z3* (VER(J,1)+VER(J,2))
          X(J,10) = Z1*VER(J,4) + Z2*VER(J,1) + Z3* (VER(J,2)+VER(J,3))
          X(J,11) = Z1*VER(J,4) + Z2*VER(J,2) + Z3* (VER(J,1)+VER(J,3))
          X(J,12) = Z1*VER(J,4) + Z2*VER(J,3) + Z3* (VER(J,1)+VER(J,2))
  135 CONTINUE
  200 CONTINUE
      CALL FUNSUB(X(1,1),NUMFUN,SUMVAL)
      if(iero.ne.0) return
      DO 220 J = 2,NUMBER
          CALL FUNSUB(X(1,J),NUMFUN,WORK)
          if(iero.ne.0) return
          DO 210 I = 1,NUMFUN
              SUMVAL(I) = SUMVAL(I) + WORK(I)
  210     CONTINUE
  220 CONTINUE
C
C***END DORTET
C
      RETURN
      END
