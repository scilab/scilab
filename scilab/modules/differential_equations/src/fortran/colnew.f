C**********************************************************************
C  this package solves boundary value problems for
C  ordinary differential equations, as described below.
C
C  COLNEW is a modification of the package COLSYS by ascher,
C  christiansen and russell [1]. It incorporates a new basis
C  representation replacing b-splines, and improvements for
C  the linear and nonlinear algebraic equation solvers.
C  the package can be referenced as either COLNEW or COLSYS.
C**********************************************************************
C----------------------------------------------------------------------
C                            p a r t  1
C        main storage allocation and program control subroutines
C----------------------------------------------------------------------
C
      SUBROUTINE COLNEW (NCOMP, M, ALEFT, ARIGHT, ZETA, IPAR, LTOL,
     1                   TOL, FIXPNT, ISPACE, FSPACE, IFLAG,
     2                   FSUB, DFSUB, GSUB, DGSUB, GUESS)
C
C
C**********************************************************************
C
C     written by
C                  u. ascher,
C                            department of computer science,
C                            university of british columbia,
C                            vancouver, b. c., canada   v6t 1w5
C                  g. bader,
C                            institut f. angewandte mathematik
C                            university of heidelberg
C                            im neuenheimer feld 294
C                            d-6900 heidelberg 1
C
C**********************************************************************
C
C     purpose
C
C     this package solves a multi-point boundary value
C     problem for a mixed order system of ode-s given by
C
C          (m(i))
C         u       =  f  ( x; z(u(x)) )      i = 1, ... ,ncomp
C          i          i
C
C                                          aleft .lt. x .lt. aright,
C
C
C         g  ( zeta(j); z(u(zeta(j))) ) = 0   j = 1, ... ,mstar
C          j
C                                    mstar = m(1)+m(2)+...+m(ncomp),
C
C
C         where                          t
C               u = (u , u , ... ,u     )  is the exact solution vector
C                     1   2        ncomp
C
C                (mi)
C               u     is the mi=m(i) th  derivative of u
C                i                                      i
C
C                                  (1)        (m1-1)       (mncomp-1)
C               z(u(x)) = ( u (x),u  (x),...,u    (x),...,u      (x) )
C                            1     1          1            ncomp
C
C                f (x,z(u))   is a (generally) nonlinear function of
C                 i
C                             z(u)=z(u(x)).
C
C                g (zeta(j);z(u))  is a (generally) nonlinear function
C                 j
C                               used to represent a boundary condition.
C
C         the boundary points satisfy
C               aleft .le. zeta(1) .le. .. .le. zeta(mstar) .le. aright
C
C         the orders mi of the differential equations satisfy
C                            1 .le. m(i) .le. 4.
C
C
C**********************************************************************
C
C     method
C
C        the method used to approximate the solution u is
C     collocation at gaussian points, requiring m(i)-1 continuous
C     derivatives in the i-th component, i = 1, ..., ncomp.
C     here, k is the number of collocation points (stages) per
C     subinterval and is chosen such that k .ge. max m(i).
C     a runge-kutta-monomial solution representation is utilized.
C
C     references
C
C     [1] u. ascher, j. christiansen and r.d. russell,
C         collocation software for boundary-value odes,
C         acm trans. math software 7 (1981), 209-222.
C         this paper contains EXAMPLES where use of the code
C         is demonstrated.
C
C     [2] g. bader and u. ascher,
C         a new basis implementation for a mixed order
C         boundary value ode solver,
C         siam j. scient. stat. comput. (1987).
C
C     [3] u. ascher, j. christiansen and r.d. russell,
C         a collocation solver for mixed order
C         systems of boundary value problems,
C         math. comp. 33 (1979), 659-679.
C
C     [4] u. ascher, j. christiansen and r.d. russell,
C         colsys - a collocation code for boundary
C         value problems,
C         lecture notes comp.sc. 76, springer verlag,
C         b. childs et. al. (eds.) (1979), 164-185.
C
C     [5] c. deboor and r. weiss,
C         solveblok: a package for solving almost block diagonal
C         linear systems,
C         acm trans. math. software 6 (1980), 80-87.
C
C**********************************************************************
C
C     ***************     input to colnew     ***************
C
C     variables
C
C     ncomp - no. of differential equations   (ncomp .le. 20)
C
C     m(j) - order of the j-th differential equation
C            ( mstar = m(1) + ... + m(ncomp) .le. 40 )
C
C     aleft - left end of interval
C
C     aright - right end of interval
C
C     zeta(j) - j-th side condition point (boundary point). must
C               have  zeta(j) .le. zeta(j+1). all side condition
C               points must be mesh points in all meshes used,
C               see description of ipar(11) and fixpnt below.
C
C     ipar - an integer array dimensioned at least 11.
C            a list of the parameters in ipar and their meaning follows
C            some parameters are renamed in colnew; these new names are
C            given in parentheses.
C
C     ipar(1)     ( = nonlin )
C             = 0 if the problem is linear
C             = 1 if the problem is nonlinear
C
C     ipar(2) = no. of collocation points per subinterval  (= k )
C               where max m(i) .le.  k .le. 7 . if ipar(2)=0 then
C               colnew sets  k = max ( max m(i)+1, 5-max m(i) )
C
C     ipar(3) = no. of subintervals in the initial mesh  ( = n ).
C               if ipar(3) = 0 then colnew arbitrarily sets n = 5.
C
C     ipar(4) = no. of solution and derivative tolerances.  ( = ntol )
C               we require  0 .lt. ntol .le. mstar.
C
C     ipar(5) = dimension of fspace.     ( = ndimf )
C
C     ipar(6) = dimension of ispace.     ( = ndimi )
C
C     ipar(7) -  output control ( = iprint )
C              = -1 for full diagnostic printout
C              = 0 for selected printout
C              = 1 for no printout
C
C     ipar(8)     ( = iread )
C             = 0 causes colnew to generate a uniform initial mesh.
C             = 1 if the initial mesh is provided by the user.  it
C                 is defined in fspace as follows:  the mesh
C                 aleft=x(1).lt.x(2).lt. ... .lt.x(n).lt.x(n+1)=aright
C                 will occupy  fspace(1), ..., fspace(n+1). the
C                 user needs to supply only the interior mesh
C                 points  fspace(j) = x(j), j = 2, ..., n.
C             = 2 if the initial mesh is supplied by the user
C                 as with ipar(8)=1, and in addition no adaptive
C                 mesh selection is to be done.
C
C     ipar(9)     ( = iguess )
C             = 0 if no initial guess for the solution is
C                 provided.
C             = 1 if an initial guess is provided by the user
C                 in subroutine  guess.
C             = 2 if an initial mesh and approximate solution
C                 coefficients are provided by the user in  fspace.
C                 (the former and new mesh are the same).
C             = 3 if a former mesh and approximate solution
C                 coefficients are provided by the user in fspace,
C                 and the new mesh is to be taken twice as coarse;
C                 i.e.,every second point from the former mesh.
C             = 4 if in addition to a former initial mesh and
C                 approximate solution coefficients, a new mesh
C                 is provided in fspace as well.
C                 (see description of output for further details
C                 on iguess = 2, 3, and 4.)
C
C     ipar(10)= 0 if the problem is regular
C             = 1 if the first relax factor is =rstart, and the
C                 nonlinear iteration does not rely on past covergence
C                 (use for an extra sensitive nonlinear problem only).
C             = 2 if we are to return immediately upon  (a) two
C                 successive nonconvergences, or  (b) after obtaining
C                 error estimate for the first time.
C
C     ipar(11)= no. of fixed points in the mesh other than aleft
C               and aright. ( = nfxpnt , the dimension of fixpnt)
C               the code requires that all side condition points
C               other than aleft and aright (see description of
C               zeta ) be included as fixed points in fixpnt.
C
C     ltol  -  an array of dimension ipar(4). ltol(j) = l  specifies
C              that the j-th tolerance in  tol  controls the error
C              in the l-th component of z(u).   also require that
C              1.le.ltol(1).lt.ltol(2).lt. ... .lt.ltol(ntol).le.mstar
C
C     tol    - an array of dimension ipar(4). tol(j) is the
C              error tolerance on the ltol(j) -th component
C              of z(u). thus, the code attempts to satisfy
C              for j=1,...,ntol  on each subinterval
C              abs(z(v)-z(u))       .le. tol(j)*abs(z(u))       +tol(j)
C                            ltol(j)                     ltol(j)
C
C              if v(x) is the approximate solution vector.
C
C     fixpnt - an array of dimension ipar(11).   it contains
C              the points, other than aleft and aright, which
C              are to be included in every mesh.
C
C     ispace - an integer work array of dimension ipar(6).
C              its size provides a constraint on nmax,
C              the maximum number of subintervals. choose
C              ipar(6) according to the formula
C                      ipar(6)  .ge.  nmax*nsizei
C                where
C                      nsizei = 3 + kdm
C                with
C                      kdm = kd + mstar  ;  kd = k * ncomp ;
C                      nrec = no. of right end boundary conditions.
C
C
C     fspace - a real work array of dimension ipar(5).
C              its size provides a constraint on nmax.
C              choose ipar(5) according to the formula
C                      ipar(5)  .ge.  nmax*nsizef
C                where
C                      nsizef = 4 + 3 * mstar + (5+kd) * kdm +
C                              (2*mstar-nrec) * 2*mstar.
C
C
C     iflag - the mode of return from colnew.
C           = 1 for normal return
C           = 0 if the collocation matrix is singular.
C           =-1 if the expected no. of subintervals exceeds storage
C               specifications.
C           =-2 if the nonlinear iteration has not converged.
C           =-3 if there is an input data error.
C
C
C**********************************************************************
C
C     *************    user supplied subroutines   *************
C
C
C     the following subroutines must be declared external in the
C     main program which calls colnew.
C
C
C     fsub  - name of subroutine for evaluating f(x,z(u(x))) =
C                            t
C             (f ,...,f     )  at a point x in (aleft,aright).  it
C               1      ncomp
C             should have the heading
C
C                       subroutine fsub (x , z , f)
C
C             where f is the vector containing the value of fi(x,z(u))
C             in the i-th component and                            t
C                                       z(u(x))=(z(1),...,z(mstar))
C             is defined as above under  purpose .
C
C
C     dfsub - name of subroutine for evaluating the jacobian of
C             f(x,z(u)) at a point x.  it should have the heading
C
C                       subroutine dfsub (x , z , df)
C
C             where z(u(x)) is defined as for fsub and the (ncomp) by
C             (mstar) array df should be filled by the partial deriv-
C             atives of f, viz, for a particular call one calculates
C                                df(i,j) = dfi / dzj, i=1,...,ncomp
C                                                     j=1,...,mstar.
C
C
C     gsub  - name of subroutine for evaluating the i-th component of
C             g(x,z(u(x))) = g (zeta(i),z(u(zeta(i)))) at a point x =
C                             i
C             zeta(i) where 1.le.i.le.mstar. it should have the heading
C
C                       subroutine gsub (i , z , g)
C
C             where z(u) is as for fsub, and i and g=g  are as above.
C                                                     i
C             note that in contrast to f in  fsub , here
C             only one value per call is returned in g.
C
C
C     dgsub - name of subroutine for evaluating the i-th row of
C             the jacobian of g(x,u(x)).  it should have the heading
C
C                       subroutine dgsub (i , z , dg)
C
C             where z(u) is as for fsub, i as for gsub and the mstar-
C             vector dg should be filled with the partial derivatives
C             of g, viz, for a particular call one calculates
C                   dg(i,j) = dgi / dzj      j=1,...,mstar.
C
C
C     guess - name of subroutine to evaluate the initial
C             approximation for  z(u(x)) and for dmval(u(x))= vector
C             of the mj-th derivatives of u(x). it should have the
C             heading
C
C                       subroutine guess (x , z , dmval)
C
C             note that this subroutine is needed only if using
C             ipar(9) = 1, and then all  mstar  components of z
C             and  ncomp  components of  dmval  should be specified
C             for any x,  aleft .le. x .le. aright .
C
C
C**********************************************************************
C
C     ************   use of output from colnew   ************
C
C                 ***   solution evaluation   ***
C
C     on return from colnew, the arrays fspace and ispace
C     contain information specifying the approximate solution.
C     the user can produce the solution vector  z( u(x) )  at
C     any point x, aleft .le. x .le. aright, by the statement,
C
C           call appsln (x, z, fspace, ispace)
C
C     when saving the coefficients for later reference, only
C     ispace(1),...,ispace(7+ncomp)    and
C     fspace(1),...,fspace(ispace(7))    need to be saved as
C     these are the quantities used by appsln.
C
C
C                 ***   simple continuation   ***
C
C
C     a formerly obtained solution can easily be used as the
C     first approximation for the nonlinear iteration for a
C     new problem by setting   (iguess =) ipar(9) = 2, 3 or 4.
C
C     if the former solution has just been obtained then the
C     values needed to define the first approximation are
C     already in ispace and fspace.
C     alternatively, if the former solution was obtained in a
C     previous run and its coefficients were saved then those
C     coefficients must be put back into
C     ispace(1),..., ispace(7+ncomp)    and
C     fspace(1),..., fspace(ispace(7)).
C
C     for ipar(9) = 2 or 3 set ipar(3) = ispace(1) ( = the
C     size of the previous mesh ).
C
C     for ipar(9) = 4 the user specifies a new mesh of n subintervals
C     as follows.
C     the values in  fspace(1),...,fspace(ispace(7))  have to be
C     shifted by n+1 locations to  fspace(n+2),..,fspace(ispace(7)+n+1)
C     and the new mesh is then specified in fspace(1),..., fspace(n+1).
C     also set ipar(3) = n.
C
C
C**********************************************************************
C
C     ***************      package subroutines      ***************
C
C     the following description gives a brief overview of how the
C     procedure is broken down into the subroutines which make up
C     the package called  colnew . for further details the
C     user should refer to documentation in the various subroutines
C     and to the references cited above.
C
C     the subroutines fall into four groups:
C
C part 1 - the main storage allocation and program control subr
C
C     colnew - tests input values, does initialization and breaks up
C              the work areas, fspace and ispace, into the arrays
C              used by the program.
C     colsys - another name for colnew
C
C     contrl - is the actual driver of the package. this routine
C              contains the strategy for nonlinear equation solving.
C
C     skale  - provides scaling for the control
C              of convergence in the nonlinear iteration.
C
C
C part 2 - mesh selection and error estimation subroutines
C
C     consts - is called once by  colnew  to initialize constants
C              which are used for error estimation and mesh selection.
C
C     newmsh - generates meshes. it contains the test to decide
C              whether or not to redistribute a mesh.
C
C     errchk - produces error estimates and checks against the
C              tolerances at each subinterval
C
C
C part 3 - collocation system set-up subroutines
C
C     lsyslv - controls the set-up and solution of the linear
C              algebraic systems of collocation equations which
C              arise at each newton iteration.
C
C     gderiv - is used by lsyslv to set up the equation associated
C              with a side condition point.
C
C     vwblok - is used by lsyslv to set up the equation(s) associated
C              with a collocation point.
C
C     gblock - is used by lsyslv to construct a block of the global
C              collocation matrix or the corresponding right hand
C              side.
C
C
C part 4 - service subroutines
C
C     appsln - sets up a standard call to  approx .
C
C     approx - evaluates a piecewise polynomial solution.
C
C     rkbas  - evaluates the mesh independent runge-kutta basis
C
C     vmonde - solves a vandermonde system for given right hand
C              side
C
C     horder - evaluates the highest order derivatives of the
C              current collocation solution used for mesh refinement.
C
C
C part 5 - linear algebra  subroutines
C
C     to solve the global linear systems of collocation equations
C     constructed in part 3,  colnew  uses a column oriented version
C     of the package  solveblok originally due to de boor and weiss.
C
C     to solve the linear systems for static parameter condensation
C     in each block of the collocation equations, the linpack
C     routines  dgefa and  dgesl  are included. but these
C     may be replaced when solving problems on vector processors
C     or when solving large scale sparse jacobian problems.
C
C----------------------------------------------------------------------
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION M(*), ZETA(*), IPAR(*), LTOL(*), TOL(*), DUMMY(1),
     1          FIXPNT(*), ISPACE(*), FSPACE(*)
C
      COMMON /COLOUT/ PRECIS, IOUT, IPRINT
      COMMON /COLLOC/ RHO(7), COEF(49)
      COMMON /COLORD/ K, NC, MSTAR, KD, MMAX, MT(20)
      COMMON /COLAPR/ N, NOLD, NMAX, NZ, NDMZ
      COMMON /COLMSH/ MSHFLG, MSHNUM, MSHLMT, MSHALT
      COMMON /COLSID/ TZETA(40), TLEFT, TRIGHT, IZETA, IDUM
      COMMON /COLNLN/ NONLIN, ITER, LIMIT, ICARE, IGUESS
      COMMON /COLEST/ TTL(40), WGTMSH(40), WGTERR(40), TOLIN(40),
     1                ROOT(40), JTOL(40), LTTOL(40), NTOL
C
      EXTERNAL FSUB, DFSUB, GSUB, DGSUB, GUESS
C
      common/iercol/iero
c
      CHARACTER TMPBUF*(4096)

C     this subroutine can be called either COLNEW or COLSYS
C
      ENTRY      COLSYS (NCOMP, M, ALEFT, ARIGHT, ZETA, IPAR, LTOL,
     1                   TOL, FIXPNT, ISPACE, FSPACE, IFLAG,
     2                   FSUB, DFSUB, GSUB, DGSUB, GUESS)

C
C*********************************************************************
C
C     the actual subroutine colnew serves as an interface with
C     the package of subroutines referred to collectively as
C     colnew. the subroutine serves to test some of the input
C     parameters, rename some of the parameters (to make under-
C     standing of the coding easier), to do some initialization,
C     and to break the work areas fspace and ispace up into the
C     arrays needed by the program.
C
C**********************************************************************
C
C...  specify machine dependent output unit  iout  and compute machine
C...  dependent constant  precis = 100 * machine unit roundoff
C
      IF ( IPAR(7) .LE. 0 )  THEN
c           replaces write(6 ...) by basout bug 2598
c      WRITE(6,99)
        WRITE(TMPBUF,99)
        CALL COLNEWMSGS(TMPBUF)
      ENDIF
  99  FORMAT(29H VERSION *COLNEW* OF COLSYS .)
C
      PRECIS = 1.D0
   10 PRECIS = PRECIS / 2.D0
      PRECP1 = PRECIS + 1.D0
      IF ( PRECP1 .GT. 1.D0 )                       GO TO 10
      PRECIS = PRECIS * 100.D0
C
C...  in case incorrect input data is detected, the program returns
C...  immediately with iflag=-3.
C
      IFLAG = -3
      IF ( NCOMP .LT. 1 .OR. NCOMP .GT. 20 )        RETURN
      DO 20 I=1,NCOMP
         IF ( M(I) .LT. 1 .OR. M(I) .GT. 4 )        RETURN
   20 CONTINUE
C
C...  rename some of the parameters and set default values.
C
      NONLIN = IPAR(1)
      K = IPAR(2)
      N = IPAR(3)
      IF ( N .EQ. 0 )  N = 5
      IREAD = IPAR(8)
      IGUESS = IPAR(9)
      IF ( NONLIN .EQ. 0 .AND. IGUESS .EQ. 1 )  IGUESS = 0
      IF ( IGUESS .GE. 2 .AND. IREAD .EQ. 0 )   IREAD = 1
      ICARE = IPAR(10)
      NTOL = IPAR(4)
      NDIMF = IPAR(5)
      NDIMI = IPAR(6)
      NFXPNT = IPAR(11)
      IPRINT = IPAR(7)
      MSTAR = 0
      MMAX = 0
      DO  30 I = 1, NCOMP
         MMAX = MAX0 ( MMAX, M(I) )
         MSTAR = MSTAR + M(I)
         MT(I) = M(I)
   30 CONTINUE
      IF ( K .EQ. 0 )   K = MAX0( MMAX + 1 , 5 - MMAX )
      DO 40 I = 1, MSTAR
   40 TZETA(I) = ZETA(I)
      DO 50 I = 1, NTOL
         LTTOL(I) = LTOL(I)
   50 TOLIN(I) = TOL(I)
      TLEFT = ALEFT
      TRIGHT = ARIGHT
      NC = NCOMP
      KD = K * NCOMP
C
C...  print the input data for checking.
C
      IF ( IPRINT .GT. -1 )                         GO TO 80
      IF ( NONLIN .GT. 0 )                          GO TO 60
      WRITE (TMPBUF,260) NCOMP
      CALL COLNEWMSGS(TMPBUF)
      WRITE (TMPBUF,261) (M(IP), IP=1,NCOMP)
      CALL COLNEWMSGS(TMPBUF)
      GO TO 70
   60 WRITE (TMPBUF,270) NCOMP
      CALL COLNEWMSGS(TMPBUF)
      WRITE (TMPBUF,271) (M(IP), IP=1, NCOMP)
      CALL COLNEWMSGS(TMPBUF)
   70 WRITE (TMPBUF,280) (ZETA(IP), IP=1,MSTAR)
      CALL COLNEWMSGS(TMPBUF)
      IF ( NFXPNT .GT. 0 ) THEN
        WRITE (TMPBUF,340) NFXPNT, (FIXPNT(IP), IP=1,NFXPNT)
        CALL COLNEWMSGS(TMPBUF)
      ENDIF
      WRITE (TMPBUF,290) K
      CALL COLNEWMSGS(TMPBUF)
      WRITE (TMPBUF,300) (LTOL(IP), IP=1,NTOL)
      CALL COLNEWMSGS(TMPBUF)
      WRITE (TMPBUF,310) (TOL(IP), IP=1,NTOL)
      CALL COLNEWMSGS(TMPBUF)
      IF (IGUESS .GE. 2) THEN
       WRITE (TMPBUF,320)
       CALL COLNEWMSGS(TMPBUF)
      ENDIF
      IF (IREAD .EQ. 2) THEN
       WRITE (TMPBUF,330)
       CALL COLNEWMSGS(TMPBUF)
      ENDIF
   80 CONTINUE
C
C...  check for correctness of data
C
      IF ( K .LT. 0 .OR. K .GT. 7 )                 RETURN
      IF ( N .LT. 0 )                               RETURN
      IF ( IREAD .LT. 0 .OR. IREAD .GT. 2 )         RETURN
      IF ( IGUESS .LT. 0 .OR. IGUESS .GT. 4 )       RETURN
      IF ( ICARE .LT. 0 .OR. ICARE .GT. 2 )         RETURN
      IF ( NTOL .LT. 0 .OR. NTOL .GT. MSTAR )       RETURN
      IF ( NFXPNT .LT. 0 )                          RETURN
      IF ( IPRINT .LT. (-1) .OR. IPRINT .GT. 1 )    RETURN
      IF ( MSTAR .LT. 0 .OR. MSTAR .GT. 40 )        RETURN
      IP = 1
      DO 100 I = 1, MSTAR
      IF ( DABS(ZETA(I) - ALEFT) .LT. PRECIS .OR.
     1     DABS(ZETA(I) - ARIGHT) .LT. PRECIS )     GO TO 100
   90 IF ( IP .GT. NFXPNT )                         RETURN
        IF ( ZETA(I) - PRECIS .LT. FIXPNT(IP) )     GO TO 95
        IP = IP + 1
      GO TO 90
   95 IF ( ZETA(I) + PRECIS .LT. FIXPNT(IP) )       RETURN
  100 CONTINUE
C
C...  set limits on iterations and initialize counters.
C...  limit = maximum number of newton iterations per mesh.
C...  see subroutine  newmsh  for the roles of  mshlmt , mshflg ,
C...  mshnum , and  mshalt .
C
      MSHLMT = 3
      MSHFLG = 0
      MSHNUM = 1
      MSHALT = 1
      LIMIT = 40
C
C...  compute the maxium possible n for the given sizes of
C...  ispace  and  fspace.
C
      NREC = 0
      DO 110 I = 1, MSTAR
           IB = MSTAR + 1 - I
           IF ( ZETA(IB) .GE. ARIGHT )  NREC = I
  110 CONTINUE
      NFIXI = MSTAR
      NSIZEI = 3 + KD + MSTAR
      NFIXF = NREC * (2*MSTAR) + 5 * MSTAR + 3
      NSIZEF = 4 + 3 * MSTAR + (KD+5) * (KD+MSTAR) +
     1(2*MSTAR-NREC) * 2*MSTAR
      NMAXF = (NDIMF - NFIXF) / NSIZEF
      NMAXI = (NDIMI - NFIXI) / NSIZEI
      IF ( IPRINT .LT. 1 )  THEN
       WRITE(TMPBUF,350) NMAXF, NMAXI
       CALL COLNEWMSGS(TMPBUF)
      ENDIF
      NMAX = MIN0( NMAXF, NMAXI )
      IF ( NMAX .LT. N )                            RETURN
      IF ( NMAX .LT. NFXPNT+1 )                     RETURN
      IF (NMAX .LT. 2*NFXPNT+2 .AND. IPRINT .LT. 1) THEN
       WRITE(TMPBUF,360)
       CALL COLNEWMSGS(TMPBUF)
      ENDIF
C
C...  generate pointers to break up  fspace  and  ispace .
C
      LXI = 1
      LG = LXI + NMAX + 1
      LXIOLD = LG + 2*MSTAR * (NMAX * (2*MSTAR-NREC) + NREC)
      LW     = LXIOLD + NMAX + 1
      LV     = LW + KD**2 * NMAX
      LZ     = LV + MSTAR * KD * NMAX
      LDMZ   = LZ + MSTAR * (NMAX + 1)
      LDELZ  = LDMZ + KD * NMAX
      LDELDZ = LDELZ + MSTAR * (NMAX + 1)
      LDQZ   = LDELDZ + KD * NMAX
      LDQDMZ = LDQZ + MSTAR * (NMAX + 1)
      LRHS   = LDQDMZ + KD * NMAX
      LVALST = LRHS   + KD * NMAX + MSTAR
      LSLOPE = LVALST + 4 * MSTAR * NMAX
      LACCUM = LSLOPE + NMAX
      LSCL   = LACCUM + NMAX + 1
      LDSCL  = LSCL + MSTAR * (NMAX + 1)
      LPVTG = 1
      LPVTW = LPVTG + MSTAR * (NMAX + 1)
      LINTEG = LPVTW + KD * NMAX
C
C...  if  iguess .ge. 2, move  xiold, z, and  dmz  to their proper
C...  locations in  fspace.
C
      IF ( IGUESS .LT. 2 )                          GO TO 160
      NOLD = N
      IF (IGUESS .EQ. 4)  NOLD = ISPACE(1)
      NZ = MSTAR * (NOLD + 1)
      NDMZ = KD * NOLD
      NP1 = N + 1
      IF ( IGUESS .EQ. 4 )  NP1 = NP1 + NOLD + 1
      DO 120 I=1,NZ
  120 FSPACE( LZ+I-1 )  =  FSPACE( NP1+I )
      IDMZ = NP1 + NZ
      DO 125 I=1,NDMZ
  125 FSPACE( LDMZ+I-1 )  =  FSPACE( IDMZ+I )
      NP1 = NOLD + 1
      IF ( IGUESS .EQ. 4 )                          GO TO 140
      DO 130 I=1,NP1
  130 FSPACE( LXIOLD+I-1 )  =  FSPACE( LXI+I-1 )
      GO TO 160
  140 DO 150 I=1,NP1
  150 FSPACE( LXIOLD+I-1 )  =  FSPACE( N+1+I )
  160 CONTINUE
C
C...  initialize collocation points, constants, mesh.
C
      CALL CONSTS ( K, RHO, COEF )
      CALL NEWMSH (3+IREAD, FSPACE(LXI), FSPACE(LXIOLD), DUMMY,
     1             DUMMY, DUMMY, DUMMY, DUMMY, NFXPNT, FIXPNT)
C
C...  determine first approximation, if the problem is nonlinear.
C
      IF (IGUESS .GE. 2)                            GO TO 230
      NP1 = N + 1
      DO 210 I = 1, NP1
  210 FSPACE( I + LXIOLD - 1 ) = FSPACE( I + LXI - 1 )
      NOLD = N
      IF ( NONLIN .EQ. 0  .OR. IGUESS .EQ. 1 )      GO TO 230
C
C...  system provides first approximation of the solution.
C...  choose z(j) = 0  for j=1,...,mstar.
C
      DO 220 I=1, NZ
  220 FSPACE( LZ-1+I ) = 0.D0
      DO 225 I=1, NDMZ
  225 FSPACE( LDMZ-1+I ) = 0.D0
  230 CONTINUE
      IF (IGUESS .GE. 2)  IGUESS = 0
      CALL CONTRL (FSPACE(LXI),FSPACE(LXIOLD),FSPACE(LZ),FSPACE(LDMZ),
     1     FSPACE(LRHS),FSPACE(LDELZ),FSPACE(LDELDZ),FSPACE(LDQZ),
     2     FSPACE(LDQDMZ),FSPACE(LG),FSPACE(LW),FSPACE(LV),
     3     FSPACE(LVALST),FSPACE(LSLOPE),FSPACE(LSCL),FSPACE(LDSCL),
     4     FSPACE(LACCUM),ISPACE(LPVTG),ISPACE(LINTEG),ISPACE(LPVTW),
     5     NFXPNT,FIXPNT,IFLAG,FSUB,DFSUB,GSUB,DGSUB,GUESS )
      if (iero.gt.0) return
C
C...  prepare output
C
      ISPACE(1) = N
      ISPACE(2) = K
      ISPACE(3) = NCOMP
      ISPACE(4) = MSTAR
      ISPACE(5) = MMAX
      ISPACE(6) = NZ + NDMZ + N + 2
      K2 = K * K
      ISPACE(7) = ISPACE(6) + K2 - 1
      DO 240 I = 1, NCOMP
  240 ISPACE(7+I) = M(I)
      DO 250 I = 1, NZ
  250 FSPACE( N+1+I ) = FSPACE( LZ-1+I )
      IDMZ = N + 1 + NZ
      DO 255 I = 1, NDMZ
  255 FSPACE( IDMZ+I ) = FSPACE( LDMZ-1+I )
      IC = IDMZ + NDMZ
      DO 258 I = 1, K2
  258 FSPACE( IC+I ) = COEF(I)
      RETURN
C----------------------------------------------------------------------
  260 FORMAT(37H THE NUMBER OF (LINEAR) DIFF EQNS IS , I3)
  261 FORMAT(17H THEIR ORDERS ARE, 20I3)
  270 FORMAT(40H THE NUMBER OF (NONLINEAR) DIFF EQNS IS , I3)
  271 FORMAT(17H THEIR ORDERS ARE, 20I3)
  280 FORMAT(27H SIDE CONDITION POINTS ZETA, 8F10.6, 4( 27X, 8F10.6))
  290 FORMAT(37H NUMBER OF COLLOC PTS PER INTERVAL IS, I3)
  300 FORMAT(39H COMPONENTS OF Z REQUIRING TOLERANCES -,8(7X,I2,1X),
     1       4(38X,8I10))
  310 FORMAT(33H CORRESPONDING ERROR TOLERANCES -,6X,8D10.2,
     1       4(39X,8D10.2))
  320 FORMAT(44H INITIAL MESH(ES) AND Z,DMZ PROVIDED BY USER)
  330 FORMAT(27H NO ADAPTIVE MESH SELECTION)
  340 FORMAT(10H THERE ARE ,I5,27H FIXED POINTS IN THE MESH - ,
     1       10(6F10.6))
  350 FORMAT(44H THE MAXIMUM NUMBER OF SUBINTERVALS IS MIN (, I4,
     1       23H (ALLOWED FROM FSPACE),,I4, 24H (ALLOWED FROM ISPACE) ))
  360 FORMAT(53H INSUFFICIENT SPACE TO DOUBLE MESH FOR ERROR ESTIMATE)
      END
      SUBROUTINE CONTRL (XI, XIOLD, Z, DMZ, RHS, DELZ, DELDMZ,
     1           DQZ, DQDMZ, G, W, V, VALSTR, SLOPE, SCALE, DSCALE,
     2           ACCUM, IPVTG, INTEGS, IPVTW, NFXPNT, FIXPNT, IFLAG,
     3           FSUB, DFSUB, GSUB, DGSUB, GUESS )
C
C**********************************************************************
C
C   purpose
C     this subroutine is the actual driver.  the nonlinear iteration
C     strategy is controlled here ( see [4] ). upon convergence, errchk
C     is called to test for satisfaction of the requested tolerances.
C
C   variables
C
C     check  - maximum tolerance value, used as part of criteria for
C              checking for nonlinear iteration convergence
C     relax  - the relaxation factor for damped newton iteration
C     relmin - minimum allowable value for relax  (otherwise the
C              jacobian is considered singular).
C     rlxold - previous relax
C     rstart - initial value for relax when problem is sensitive
C     ifrz   - number of fixed jacobian iterations
C     lmtfrz - maximum value for ifrz before performing a reinversion
C     iter   - number of iterations (counted only when jacobian
C              reinversions are performed).
C     xi     - current mesh
C     xiold  - previous mesh
C     ipred  = 0  if relax is determined by a correction
C            = 1  if relax is determined by a prediction
C     ifreez = 0  if the jacobian is to be updated
C            = 1  if the jacobian is currently fixed (frozen)
C     iconv  = 0  if no previous convergence has been obtained
C            = 1  if convergence on a previous mesh has been obtained
C     icare  =-1  no convergence occurred (used for regular problems)
C            = 0  a regular problem
C            = 1  a sensitive problem
C            = 2  used for continuation (see description of ipar(10)
C                 in colnew).
C     rnorm  - norm of rhs (right hand side) for current iteration
C     rnold  - norm of rhs for previous iteration
C     anscl  - scaled norm of newton correction
C     anfix  - scaled norm of newton correction at next step
C     anorm  - scaled norm of a correction obtained with jacobian fixed
C     nz     - number of components of  z  (see subroutine approx)
C     ndmz   - number of components of  dmz  (see subroutine approx)
C     imesh  - a control variable for subroutines newmsh and errchk
C            = 1  the current mesh resulted from mesh selection
C                 or is the initial mesh.
C            = 2  the current mesh resulted from doubling the
C                 previous mesh
C
C**********************************************************************
C
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION XI(*), XIOLD(*), Z(*), DMZ(*), RHS(*)
      DIMENSION G(*), W(*), V(*), VALSTR(*), SLOPE(*), ACCUM(*)
      DIMENSION DELZ(*), DELDMZ(*), DQZ(*), DQDMZ(*) , FIXPNT(*)
      DIMENSION DUMMY(1), SCALE(*), DSCALE(*)
      DIMENSION INTEGS(*), IPVTG(*), IPVTW(*)
C
      COMMON /COLOUT/ PRECIS, IOUT, IPRINT
      COMMON /COLORD/ K, NCOMP, MSTAR, KD, MMAX, M(20)
      COMMON /COLAPR/ N, NOLD, NMAX, NZ, NDMZ
      COMMON /COLMSH/ MSHFLG, MSHNUM, MSHLMT, MSHALT
      COMMON /COLSID/ ZETA(40), ALEFT, ARIGHT, IZETA, IDUM
      COMMON /COLNLN/ NONLIN, ITER, LIMIT, ICARE, IGUESS
      COMMON /COLEST/ TOL(40), WGTMSH(40), WGTERR(40), TOLIN(40),
     1                ROOT(40), JTOL(40), LTOL(40), NTOL
C
      EXTERNAL FSUB, DFSUB, GSUB, DGSUB, GUESS
C
      common/iercol/iero
c
      CHARACTER TMPBUF*(4096)
C...  constants for control of nonlinear iteration
C
      RELMIN = 1.D-3
      RSTART = 1.D-2
      LMTFRZ = 4
C
C...  compute the maximum tolerance
C
      CHECK = 0.D0
      DO 10 I = 1, NTOL
   10   CHECK = DMAX1 ( TOLIN(I), CHECK )
      IMESH = 1
      ICONV = 0
      IF ( NONLIN .EQ. 0 ) ICONV = 1
      ICOR = 0
      NOCONV = 0
      MSING = 0
C
C...  the main iteration begins here .
C...  loop 20 is executed until error tolerances are satisfied or
C...  the code fails (due to a singular matrix or storage limitations)
C
   20      CONTINUE
C
C...       initialization for a new mesh
C
           ITER = 0
           IF ( NONLIN .GT. 0 )                     GO TO 50
C
C...       the linear case.
C...       set up and solve equations
C
           CALL LSYSLV (MSING, XI, XIOLD, DUMMY, DUMMY, Z, DMZ, G,
     1          W, V, RHS, DUMMY, INTEGS, IPVTG, IPVTW, RNORM, 0,
     2          FSUB, DFSUB, GSUB, DGSUB, GUESS )
           if (iero.gt.0) return
C
C...       check for a singular matrix
C
           IF ( MSING .EQ. 0 )                      GO TO 400
   30      IF ( MSING .LT. 0 )                      GO TO 40
           IF ( IPRINT .LT. 1 )  THEN
            WRITE (TMPBUF,495)
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           GO TO 460
   40      IF ( IPRINT .LT. 1 )  THEN
            WRITE (TMPBUF,490)
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           IFLAG = 0
           RETURN
C
C...       iteration loop for nonlinear case
C...       define the initial relaxation parameter (= relax)
C
   50      RELAX = 1.D0
C
C...       check for previous convergence and problem sensitivity
C
           IF ( ICARE .EQ. 1 .OR. ICARE .EQ. (-1) )  RELAX = RSTART
           IF ( ICONV .EQ. 0 )                      GO TO 160
C
C...       convergence on a previous mesh has been obtained.    thus
C...       we have a very good initial approximation for the newton
C...       process.    proceed with one full newton and then iterate
C...       with a fixed jacobian.
C
           IFREEZ = 0
C
C...       evaluate right hand side and its norm  and
C...       find the first newton correction
C
           CALL LSYSLV (MSING, XI, XIOLD, Z, DMZ, DELZ, DELDMZ, G,
     1          W, V, RHS, DQDMZ, INTEGS, IPVTG, IPVTW, RNOLD, 1,
     2          FSUB, DFSUB, GSUB, DGSUB, GUESS )
           if (iero.gt.0) return
C
           IF ( IPRINT .LT. 0 )  THEN
            WRITE(TMPBUF,530)
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           IF ( IPRINT .LT. 0 )  THEN
            WRITE (TMPBUF,510) ITER, RNOLD
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           GO TO 70
C
C...       solve for the next iterate .
C...       the value of ifreez determines whether this is a full
C...       newton step (=0) or a fixed jacobian iteration (=1).
C
   60      IF ( IPRINT .LT. 0 )  THEN
            WRITE (TMPBUF,510) ITER, RNORM
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           RNOLD = RNORM
           CALL LSYSLV (MSING, XI, XIOLD, Z, DMZ, DELZ, DELDMZ, G,
     1          W, V, RHS, DUMMY, INTEGS, IPVTG, IPVTW, RNORM,
     2          3+IFREEZ, FSUB, DFSUB, GSUB, DGSUB, GUESS )
           if (iero.gt.0) return
C
C...       check for a singular matrix
C
   70      IF ( MSING .NE. 0 )                      GO TO 30
           IF ( IFREEZ .EQ. 1 )                     GO TO 80
C
C...       a full newton step
C
           ITER = ITER + 1
           IFRZ = 0
   80      CONTINUE
C
C...       update   z and dmz , compute new  rhs  and its norm
C
           DO 90 I = 1, NZ
             Z(I) = Z(I) + DELZ(I)
   90      CONTINUE
           DO 100 I = 1, NDMZ
             DMZ(I) = DMZ(I) + DELDMZ(I)
  100      CONTINUE
           CALL LSYSLV (MSING, XI, XIOLD, Z, DMZ, DELZ, DELDMZ, G,
     1          W, V, RHS, DUMMY, INTEGS, IPVTG, IPVTW, RNORM, 2,
     2          FSUB, DFSUB, GSUB, DGSUB, GUESS )
           if (iero.gt.0) return
C
C...       check monotonicity. if the norm of  rhs  gets smaller,
C...       proceed with a fixed jacobian; else proceed cautiously,
C...       as if convergence has not been obtained before (iconv=0).
C
           IF ( RNORM .LT. PRECIS )                 GO TO 390
           IF ( RNORM .GT. RNOLD )                  GO TO 130
           IF ( IFREEZ .EQ. 1 )                     GO TO 110
           IFREEZ = 1
           GO TO 60
C
C...       verify that the linear convergence with fixed jacobian
C...       is fast enough.
C
  110      IFRZ = IFRZ + 1
           IF ( IFRZ .GE. LMTFRZ )       IFREEZ = 0
           IF ( RNOLD .LT. 4.D0*RNORM )  IFREEZ = 0
C
C...       check convergence (iconv = 1).
C
           DO 120 IT = 1, NTOL
             INZ = LTOL(IT)
             DO 120 IZ = INZ, NZ, MSTAR
               IF ( DABS(DELZ(IZ)) .GT.
     1           TOLIN(IT) * (DABS(Z(IZ)) + 1.D0))  GO TO 60
  120      CONTINUE
C
C...       convergence obtained
C
           IF ( IPRINT .LT. 1 )  THEN
            WRITE (TMPBUF,560) ITER
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           GO TO 400
C
C...      convergence of fixed jacobian iteration failed.
C
  130      IF ( IPRINT .LT. 0 )  THEN
            WRITE (TMPBUF,510) ITER, RNORM
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           IF ( IPRINT .LT. 0 )  THEN
            WRITE (TMPBUF,540)
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           ICONV = 0
           RELAX = RSTART
           DO 140 I = 1, NZ
             Z(I) = Z(I) - DELZ(I)
  140      CONTINUE
           DO 150 I = 1, NDMZ
             DMZ(I) = DMZ(I) - DELDMZ(I)
  150      CONTINUE
C
C...       update old mesh
C
           NP1 = N + 1
           DO 155 I = 1, NP1
  155        XIOLD(I) = XI(I)
           NOLD = N
C
           ITER = 0
C
C...       no previous convergence has been obtained. proceed
C...       with the damped newton method.
C...       evaluate rhs and find the first newton correction.
C
  160      IF(IPRINT .LT. 0)  THEN
            WRITE (TMPBUF,500)
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           CALL LSYSLV (MSING, XI, XIOLD, Z, DMZ, DELZ, DELDMZ, G,
     1          W, V, RHS, DQDMZ, INTEGS, IPVTG, IPVTW, RNOLD, 1,
     2          FSUB, DFSUB, GSUB, DGSUB, GUESS )
           if (iero.gt.0) return
C
C...       check for a singular matrix
C
           IF ( MSING .NE. 0 )                       GO TO 30
C
C...       bookkeeping for first mesh
C
           IF ( IGUESS .EQ. 1 )  IGUESS = 0
C
C...       find initial scaling
C
           CALL SKALE (N, MSTAR, KD, Z, XI, SCALE, DSCALE)
           GO TO 220
C
C...       main iteration loop
C
  170      RNOLD = RNORM
           IF ( ITER .GE. LIMIT )                   GO TO 430
C
C...       update scaling
C
           CALL SKALE (N, MSTAR, KD, Z, XI, SCALE, DSCALE)
C
C...       compute norm of newton correction with new scaling
C
           ANSCL = 0.D0
           DO 180 I = 1, NZ
             ANSCL = ANSCL + (DELZ(I) * SCALE(I))**2
  180      CONTINUE
           DO 190 I = 1, NDMZ
             ANSCL = ANSCL + (DELDMZ(I) * DSCALE(I))**2
  190      CONTINUE
           ANSCL = DSQRT(ANSCL / DBLE(NZ+NDMZ))
C
C...       find a newton direction
C
           CALL LSYSLV (MSING, XI, XIOLD, Z, DMZ, DELZ, DELDMZ, G,
     1          W, V, RHS, DUMMY, INTEGS, IPVTG, IPVTW, RNORM, 3,
     2          FSUB, DFSUB, GSUB, DGSUB, GUESS )
           if (iero.gt.0) return
C
C...       check for a singular matrix
C
           IF ( MSING .NE. 0 )                      GO TO 30
C
C...       predict relaxation factor for newton step.
C
           ANDIF = 0.D0
           DO 200 I = 1, NZ
             ANDIF = ANDIF + ((DQZ(I) - DELZ(I)) * SCALE(I))**2
  200      CONTINUE
           DO 210 I = 1, NDMZ
             ANDIF = ANDIF + ((DQDMZ(I) - DELDMZ(I)) * DSCALE(I))**2
  210      CONTINUE
           ANDIF = DSQRT(ANDIF/DBLE(NZ+NDMZ) + PRECIS)
           RELAX = RELAX * ANSCL / ANDIF
           IF ( RELAX .GT. 1.D0 )  RELAX = 1.D0
  220      RLXOLD = RELAX
           IPRED = 1
           ITER = ITER + 1
C
C...       determine a new  z and dmz  and find new  rhs  and its norm
C
           DO 230 I = 1, NZ
             Z(I) = Z(I)  +  RELAX * DELZ(I)
  230      CONTINUE
           DO 240 I = 1, NDMZ
             DMZ(I) = DMZ(I)  +  RELAX * DELDMZ(I)
  240      CONTINUE
  250      CALL LSYSLV (MSING, XI, XIOLD, Z, DMZ, DQZ, DQDMZ, G,
     1          W, V, RHS, DUMMY, INTEGS, IPVTG, IPVTW, RNORM, 2,
     2          FSUB, DFSUB, GSUB, DGSUB, GUESS )
           if (iero.gt.0) return
C
C...       compute a fixed jacobian iterate (used to control relax)
C
           CALL LSYSLV (MSING, XI, XIOLD, Z, DMZ, DQZ, DQDMZ, G,
     1          W, V, RHS, DUMMY, INTEGS, IPVTG, IPVTW, RNORM, 4,
     2          FSUB, DFSUB, GSUB, DGSUB, GUESS )
           if (iero.gt.0) return
C
C...       find scaled norms of various terms used to correct relax
C
           ANORM = 0.D0
           ANFIX = 0.D0
           DO 260 I = 1, NZ
             ANORM = ANORM  +  (DELZ(I) * SCALE(I))**2
             ANFIX = ANFIX  +  (DQZ(I) * SCALE(I))**2
  260      CONTINUE
           DO 270 I = 1, NDMZ
             ANORM = ANORM  +  (DELDMZ(I) * DSCALE(I))**2
             ANFIX = ANFIX  +  (DQDMZ(I) * DSCALE(I))**2
  270      CONTINUE
           ANORM = DSQRT(ANORM / DBLE(NZ+NDMZ))
           ANFIX = DSQRT(ANFIX / DBLE(NZ+NDMZ))
           IF ( ICOR .EQ. 1 )                         GO TO 280
           IF (IPRINT .LT. 0)  THEN
            WRITE (TMPBUF,520) ITER, RELAX, ANORM,
     1           ANFIX, RNOLD, RNORM
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           GO TO 290
  280      IF (IPRINT .LT. 0) THEN
            WRITE (TMPBUF,550) RELAX, ANORM, ANFIX,
     1           RNOLD, RNORM
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
  290      ICOR = 0
C
C...       check for monotonic decrease in  delz and deldmz.
C
           IF (ANFIX.LT.PRECIS .OR. RNORM.LT.PRECIS)  GO TO 390
           IF ( ANFIX .GT. ANORM )                    GO TO 300
C
C...       we have a decrease.
C...       if  dqz  and dqdmz  small, check for convergence
C
           IF ( ANFIX .LE. CHECK )                    GO TO 350
C
C...       correct the predicted  relax  unless the corrected
C...       value is within 10 percent of the predicted one.
C
           IF ( IPRED .NE. 1 )                        GO TO 170
  300      IF ( ITER .GE. LIMIT )                     GO TO 430
C
C...       correct the relaxation factor.
C
           IPRED = 0
           ARG = (ANFIX/ANORM - 1.D0) / RELAX + 1.D0
           IF ( ARG .LT. 0.D0 )                       GO TO 170
           IF (ARG .LE. .25D0*RELAX+.125D0*RELAX**2)  GO TO 310
           FACTOR = -1.D0 + DSQRT (1.D0+8.D0 * ARG)
           IF ( DABS(FACTOR-1.D0) .LT. .1D0*FACTOR )  GO TO 170
           IF ( FACTOR .LT. 0.5D0 )  FACTOR = 0.5D0
           RELAX = RELAX / FACTOR
           GO TO 320
  310      IF ( RELAX .GE. .9D0 )                     GO TO 170
           RELAX = 1.D0
  320      ICOR = 1
           IF ( RELAX .LT. RELMIN )                   GO TO 440
           FACT = RELAX - RLXOLD
           DO 330 I = 1, NZ
            Z(I) = Z(I)  +  FACT * DELZ(I)
  330      CONTINUE
           DO 340 I = 1, NDMZ
             DMZ(I) = DMZ(I)  +  FACT * DELDMZ(I)
  340      CONTINUE
           RLXOLD = RELAX
           GO TO 250
C
C...       check convergence (iconv = 0).
C
  350      CONTINUE
           DO 360 IT = 1, NTOL
             INZ = LTOL(IT)
             DO 360 IZ = INZ, NZ, MSTAR
               IF ( DABS(DQZ(IZ)) .GT.
     1         TOLIN(IT) * (DABS(Z(IZ)) + 1.D0) )   GO TO 170
  360      CONTINUE
C
C...       convergence obtained
C
           IF ( IPRINT .LT. 1 )  THEN
            WRITE (TMPBUF,560) ITER
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
C
C...       since convergence obtained, update  z and dmz  with term
C...       from the fixed jacobian iteration.
C
           DO 370 I = 1, NZ
             Z(I) = Z(I)  +  DQZ(I)
  370      CONTINUE
           DO 380 I = 1, NDMZ
             DMZ(I) = DMZ(I)  +  DQDMZ(I)
  380      CONTINUE
  390      IF ( (ANFIX .LT. PRECIS .OR. RNORM .LT. PRECIS)
     1          .AND. IPRINT .LT. 1 )  THEN
            WRITE (TMPBUF,560) ITER
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           ICONV = 1
           IF ( ICARE .EQ. (-1) )  ICARE = 0
C
C...       if full output has been requested, print values of the
C...       solution components   z  at the meshpoints.
C
  400      IF ( IPRINT .GE. 0 )                     GO TO 420
           DO 410 J = 1, MSTAR
             WRITE(TMPBUF,610) J
             CALL COLNEWMSGS(TMPBUF)
c             WRITE(*,620) (Z(LJ), LJ = J, NZ, MSTAR)
c            Create and display buffer row by row
c            Format 620 write one space following by at most 8 double
c            that's why the increment of iter is multiply by 8
             DO 405 iter = J, NZ, MSTAR*8
  405            WRITE(TMPBUF,620) (Z(LJ), LJ = iter, iter+MSTAR*7,
     1              MSTAR)
             CALL COLNEWMSGS(TMPBUF)
  410        continue
C
C...       check for error tolerance satisfaction
C
  420      IFIN = 1
           IF (IMESH .EQ. 2) CALL ERRCHK (XI, Z, DMZ, VALSTR, IFIN)
           IF ( IMESH .EQ. 1 .OR.
     1          IFIN .EQ. 0 .AND. ICARE .NE. 2)     GO TO 460
           IFLAG = 1
           RETURN
C
C...       diagnostics for failure of nonlinear iteration.
C
  430      IF ( IPRINT .LT. 1 )  THEN
            WRITE (TMPBUF,570) ITER
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           GO TO 450
  440      IF( IPRINT .LT. 1 )  THEN
            WRITE(TMPBUF,580) RELAX, RELMIN
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
  450      IFLAG = -2
           NOCONV = NOCONV + 1
           IF ( ICARE .EQ. 2 .AND. NOCONV .GT. 1 )  RETURN
           IF ( ICARE .EQ. 0 )  ICARE = -1
C
C...       update old mesh
C
  460      NP1 = N + 1
           DO 470 I = 1, NP1
  470        XIOLD(I) = XI(I)
           NOLD = N
C
C...       pick a new mesh
C...       check safeguards for mesh refinement
C
           IMESH = 1
           IF ( ICONV .EQ. 0 .OR. MSHNUM .GE. MSHLMT
     1                       .OR. MSHALT .GE. MSHLMT )  IMESH = 2
           IF ( MSHALT .GE. MSHLMT .AND.
     1          MSHNUM .LT. MSHLMT )  MSHALT = 1
           CALL NEWMSH (IMESH, XI, XIOLD, Z, DMZ, VALSTR,
     1                  SLOPE, ACCUM, NFXPNT, FIXPNT)
C
C...       exit if expected n is too large (but may try n=nmax once)
C
           IF ( N .LE. NMAX )                       GO TO 480
           N = N / 2
           IFLAG = -1
           IF ( ICONV .EQ. 0 .AND. IPRINT .LT. 1 )  THEN
            WRITE (TMPBUF,590)
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           IF ( ICONV .EQ. 1 .AND. IPRINT .LT. 1 )  THEN
            WRITE (TMPBUF,600)
            CALL COLNEWMSGS(TMPBUF)
           ENDIF
           RETURN
  480      IF ( ICONV .EQ. 0 )  IMESH = 1
           IF ( ICARE .EQ. 1 )  ICONV = 0
      GO TO 20
C     ---------------------------------------------------------------
  490 FORMAT(35H THE GLOBAL BVP-MATRIX IS SINGULAR )
  495 FORMAT(40H A LOCAL ELIMINATION MATRIX IS SINGULAR )
  500 FORMAT(30H FULL DAMPED NEWTON ITERATION,)
  510 FORMAT(13H ITERATION = , I3, 15H  NORM (RHS) = , D10.2)
  520 FORMAT(13H ITERATION = ,I3,22H  RELAXATION FACTOR = ,D10.2,
     1       33H NORM OF SCALED RHS CHANGES FROM ,D10.2,3H TO,D10.2,
     2       33H NORM   OF   RHS  CHANGES  FROM  ,D10.2,3H TO,D10.2,
     2       D10.2)
  530 FORMAT(27H FIXED JACOBIAN ITERATIONS,)
  540 FORMAT(35H SWITCH TO DAMPED NEWTON ITERATION,)
  550 FORMAT(40H RELAXATION FACTOR CORRECTED TO RELAX = , D10.2,
     1       33H NORM OF SCALED RHS CHANGES FROM ,D10.2,3H TO,D10.2,
     2       33H NORM   OF   RHS  CHANGES  FROM  ,D10.2,3H TO,D10.2,
     2       D10.2)
  560 FORMAT(18H CONVERGENCE AFTER , I3,11H ITERATIONS )
  570 FORMAT(22H NO CONVERGENCE AFTER , I3, 11H ITERATIONS)
  580 FORMAT(37H NO CONVERGENCE.  RELAXATION FACTOR =,D10.3
     1       ,24H IS TOO SMALL (LESS THAN, D10.3, 1H))
  590 FORMAT(18H  (NO CONVERGENCE) )
  600 FORMAT(50H  (PROBABLY TOLERANCES TOO STRINGENT, OR NMAX TOO
     1       ,6HSMALL) )
  610 FORMAT( 19H MESH VALUES FOR Z(, I2, 2H), )
  620 FORMAT(1H , 8D15.7)
      END
      SUBROUTINE SKALE (N, MSTAR, KD, Z, XI, SCALE, DSCALE)
C
C**********************************************************************
C
C   purpose
C            provide a proper scaling of the state variables, used
C            to control the damping factor for a newton iteration [2].
C
C   variables
C
C            n      = number of mesh subintervals
C            mstar  = number of unknomns in z(u(x))
C            kd     = number of unknowns in dmz
C            z      = the global unknown vector
C            xi     = the current mesh
C            scale  = scaling vector for z
C            dscale = scaling vector for dmz
C
C**********************************************************************
C
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION Z(MSTAR,*), SCALE(MSTAR,*), DSCALE(KD,*)
      DIMENSION XI(*), BASM(5)
C
      COMMON /COLORD/ K, NCOMP, ID1, ID2, MMAX, M(20)
C
      BASM(1) = 1.D0
      DO 50 J=1,N
        IZ = 1
        H = XI(J+1) - XI(J)
        DO 10 L = 1, MMAX
          BASM(L+1) = BASM(L) * H / DBLE(L)
  10    CONTINUE
        DO 40 ICOMP = 1, NCOMP
          SCAL = (DABS(Z(IZ,J)) + DABS(Z(IZ,J+1))) * .5D0 + 1.D0
          MJ = M(ICOMP)
          DO 20 L = 1, MJ
            SCALE(IZ,J) = BASM(L) / SCAL
            IZ = IZ + 1
  20      CONTINUE
          SCAL = BASM(MJ+1) / SCAL
          DO 30 IDMZ = ICOMP, KD, NCOMP
            DSCALE(IDMZ,J) = SCAL
  30      CONTINUE
  40    CONTINUE
  50  CONTINUE
      NP1 = N + 1
      DO 60 IZ = 1, MSTAR
        SCALE(IZ,NP1) = SCALE(IZ,N)
  60  CONTINUE
      RETURN
      END
C----------------------------------------------------------------------
C                            p a r t  2
C          mesh selection, error estimation, (and related
C          constant assignment) routines -- see [3], [4]
C----------------------------------------------------------------------
C
      SUBROUTINE NEWMSH (MODE, XI, XIOLD, Z, DMZ, VALSTR,
     1                   SLOPE, ACCUM, NFXPNT, FIXPNT)
C
C**********************************************************************
C
C   purpose
C            select a mesh on which a collocation solution is to be
C            determined
C
C                           there are 5 possible modes of action:
C            mode = 5,4,3 - deal mainly with definition of an initial
C                           mesh for the current boundary value problem
C                 = 2,1   - deal with definition of a new mesh, either
C                           by simple mesh halving or by mesh selection
C            more specifically, for
C            mode = 5  an initial (generally nonuniform) mesh is
C                      defined by the user and no mesh selection is to
C                      be performed
C                 = 4  an initial (generally nonuniform) mesh is
C                      defined by the user
C                 = 3  a simple uniform mesh (except possibly for some
C                      fixed points) is defined; n= no. of subintervals
C                 = 1  the automatic mesh selection procedure is used
C                      (see [3] for details)
C                 = 2  a simple mesh halving is performed
C
C**********************************************************************
C
C   variables
C
C            n      = number of mesh subintervals
C            nold   = number of subintervals for former mesh
C            xi     - mesh point array
C            xiold  - former mesh point array
C            mshlmt - maximum no. of mesh selections which are permitted
C                     for a given n before mesh halving
C            mshnum - no. of mesh selections which have actually been
C                     performed for the given n
C            mshalt - no. of consecutive times ( plus 1 ) the mesh
C                     selection has alternately halved and doubled n.
C                     if mshalt .ge. mshlmt then  contrl  requires
C                     that the current mesh be halved.
C            mshflg = 1  the mesh is a halving of its former mesh
C                       (so an error estimate has been calculated)
C                   = 0  otherwise
C            iguess - ipar(9) in subroutine colnew.  it is used
C                     here only for mode=5 and 4, where
C                   = 2 the subroutine sets xi=xiold.  this is
C                       used e.g. if continuation is being per-
C                       formed, and a mesh for the old differen-
C                       tial equation is being used
C                   = 3 same as for =2, except xi uses every other
C                       point of xiold (so mesh xiold is mesh xi
C                       halved)
C                   = 4 xi has been defined by the user, and an old
C                       mesh xiold is also available
C                       otherwise, xi has been defined by the user
C                       and we set xiold=xi in this subroutine
C            slope  - an approximate quantity to be equidistributed for
C                     mesh selection (see [3]), viz,
C                             .                        (k+mj)
C                     slope(i)=     max   (weight(l) *u      (xi(i)))
C                               1.le.l.le.ntol         j
C
C                     where j=jtol(l)
C            slphmx - maximum of slope(i)*(xiold(i+1)-xiold(i)) for
C                     i = 1 ,..., nold.
C            accum  - accum(i) is the integral of  slope  from  aleft
C                     to  xiold(i).
C            valstr - is assigned values needed in  errchk  for the
C                     error estimate.
C**********************************************************************
C
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION D1(40), D2(40), SLOPE(*), ACCUM(*), VALSTR(*)
      DIMENSION XI(*), XIOLD(*), Z(*), DMZ(*), FIXPNT(*), DUMMY(1)
C
      COMMON /COLOUT/ PRECIS, IOUT, IPRINT
      COMMON /COLORD/ K, NCOMP, MSTAR, KD, MMAX, M(20)
      COMMON /COLAPR/ N, NOLD, NMAX, NZ, NDMZ
      COMMON /COLMSH/ MSHFLG, MSHNUM, MSHLMT, MSHALT
      COMMON /COLNLN/ NONLIN, ITER, LIMIT, ICARE, IGUESS
      COMMON /COLSID/  ZETA(40), ALEFT, ARIGHT, IZETA, IDUM
      COMMON /COLBAS/ B(28), ACOL(28,7), ASAVE(28,4)
      COMMON /COLEST/ TOL(40), WGTMSH(40), WGTERR(40), TOLIN(40),
     1                ROOT(40), JTOL(40), LTOL(40), NTOL
c
      CHARACTER TMPBUF*(4096)
C
C
      NFXP1 = NFXPNT +1
      GO TO (180, 100, 50, 20, 10), MODE

C
C...  mode=5   set mshlmt=1 so that no mesh selection is performed
C
   10 MSHLMT = 1
C
C...  mode=4   the user-specified initial mesh is already in place.
C
   20 IF ( IGUESS .LT. 2 )                          GO TO 40
C
C...  iguess=2, 3 or 4.
C
      NOLDP1 = NOLD + 1
      IF (IPRINT .LT. 1)  THEN
       WRITE(TMPBUF,360) NOLD, (XIOLD(I), I=1,NOLDP1)
       CALL COLNEWMSGS(TMPBUF)
      ENDIF
      IF ( IGUESS .NE. 3 )                          GO TO 40
C
C...  if iread ( ipar(8) ) .ge. 1 and iguess ( ipar(9) ) .eq. 3
C...  then the first mesh is every second point of the
C...  mesh in  xiold .
C
      N = NOLD /2
      I = 0
      DO 30 J = 1, NOLD, 2
           I = I + 1
   30 XI(I) = XIOLD(J)
   40 CONTINUE
      NP1 = N + 1
      XI(1) = ALEFT
      XI(NP1) = ARIGHT
      GO TO 320
C
C...  mode=3   generate a (piecewise) uniform mesh. if there are
C...  fixed points then ensure that the n being used is large enough.
C
   50 IF ( N .LT. NFXP1 )  N = NFXP1
      NP1 = N + 1
      XI(1) = ALEFT
      ILEFT = 1
      XLEFT = ALEFT
C
C...  loop over the subregions between fixed points.
C
      DO 90 J = 1, NFXP1
           XRIGHT = ARIGHT
           IRIGHT = NP1
           IF ( J .EQ. NFXP1 )                      GO TO 60
           XRIGHT = FIXPNT(J)
C
C...       determine where the j-th fixed point should fall in the
C...       new mesh - this is xi(iright) and the (j-1)st fixed
C...       point is in xi(ileft)
C
           NMIN = (XRIGHT-ALEFT) / (ARIGHT-ALEFT) * DBLE(N) + 1.5D0
           IF (NMIN .GT. N-NFXPNT+J)  NMIN = N - NFXPNT + J
           IRIGHT = MAX0 (ILEFT+1, NMIN)
   60      XI(IRIGHT) = XRIGHT
C
C...       generate equally spaced points between the j-1st and the
C...       j-th fixed points.
C
           NREGN = IRIGHT - ILEFT - 1
           IF ( NREGN .EQ. 0 )                      GO TO 80
           DX = (XRIGHT - XLEFT) / DBLE(NREGN+1)
           DO 70 I = 1, NREGN
   70      XI(ILEFT+I) = XLEFT  +  DBLE(I) * DX
   80      ILEFT = IRIGHT
           XLEFT = XRIGHT
   90 CONTINUE
      GO TO 320
C
C...  mode=2  halve the current mesh (i.e. double its size)
C
  100 N2 = 2 * N
C
C...  check that n does not exceed storage limitations
C
      IF ( N2 .LE. NMAX )                           GO TO 120
C
C...  if possible, try with n=nmax. redistribute first.
C
      IF ( MODE .EQ. 2 )                            GO TO 110
      N = NMAX / 2
      GO TO 220
  110 IF ( IPRINT .LT. 1 ) THEN
        WRITE(TMPBUF,370)
        CALL COLNEWMSGS(TMPBUF)
      ENDIF
      N = N2
      RETURN
C
C...  calculate the old approximate solution values at
C...  points to be used in  errchk  for error estimates.
C...  if  mshflg  =1 an error estimate was obtained for
C...  for the old approximation so half the needed values
C...  will already be in  valstr .
C
  120 IF ( MSHFLG .EQ. 0 )                          GO TO 140
C
C...  save in  valstr  the values of the old solution
C...  at the relative positions 1/6 and 5/6 in each subinterval.
C
      KSTORE = 1
      DO 130 I = 1, NOLD
          HD6 = (XIOLD(I+1) - XIOLD(I)) / 6.D0
          X = XIOLD(I) + HD6
          CALL APPROX (I, X, VALSTR(KSTORE), ASAVE(1,1), DUMMY, XIOLD,
     1         NOLD, Z, DMZ, K, NCOMP, MMAX, M, MSTAR, 4, DUMMY, 0)
          X = X + 4.D0 * HD6
          KSTORE = KSTORE  +  3 * MSTAR
          CALL APPROX (I, X, VALSTR(KSTORE), ASAVE(1,4), DUMMY, XIOLD,
     1         NOLD, Z, DMZ, K, NCOMP, MMAX, M, MSTAR, 4, DUMMY, 0)
          KSTORE = KSTORE  +  MSTAR
  130 CONTINUE
      GO TO 160
C
C...  save in  valstr  the values of the old solution
C...  at the relative positions 1/6, 2/6, 4/6 and 5/6 in
C...  each subinterval.
C
  140 KSTORE = 1
      DO 150 I = 1, N
         X = XI(I)
         HD6 = (XI(I+1) - XI(I)) / 6.D0
         DO 150 J = 1, 4
           X = X + HD6
           IF ( J.EQ.3 )  X = X + HD6
           CALL APPROX (I, X, VALSTR(KSTORE), ASAVE(1,J), DUMMY, XIOLD,
     1          NOLD, Z, DMZ, K, NCOMP, MMAX, M, MSTAR, 4, DUMMY, 0)
           KSTORE = KSTORE  +  MSTAR
  150 CONTINUE
  160 MSHFLG = 0
      MSHNUM = 1
      MODE = 2
C
C...  generate the halved mesh.
C
      J = 2
      DO 170 I = 1, N
           XI(J) = (XIOLD(I) + XIOLD(I+1)) / 2.D0
           XI(J+1) = XIOLD(I+1)
  170 J = J + 2
      N = N2
      GO TO 320
C
C...  mode=1  we do mesh selection if it is deemed worthwhile
C
  180 IF ( NOLD .EQ. 1 )                            GO TO 100
      IF ( NOLD .LE. 2*NFXPNT )                     GO TO 100
C
C...  the first interval has to be treated separately from the
C...  other intervals (generally the solution on the (i-1)st and ith
C...  intervals will be used to approximate the needed derivative, but
C...  here the 1st and second intervals are used.)
C
      I = 1
      HIOLD = XIOLD(2) - XIOLD(1)
      CALL HORDER (1, D1, HIOLD, DMZ, NCOMP, K)
      HIOLD = XIOLD(3) - XIOLD(2)
      CALL HORDER (2, D2, HIOLD, DMZ, NCOMP, K)
      ACCUM(1) = 0.D0
      SLOPE(1) = 0.D0
      ONEOVH = 2.D0 / ( XIOLD(3) - XIOLD(1) )
      DO 190 J = 1, NTOL
           JJ = JTOL(J)
           JZ = LTOL(J)
  190 SLOPE(1) = DMAX1(SLOPE(1),(DABS(D2(JJ)-D1(JJ))*WGTMSH(J)*
     1           ONEOVH / (1.D0 + DABS(Z(JZ)))) **ROOT(J))
      SLPHMX = SLOPE(1) * (XIOLD(2) - XIOLD(1))
      ACCUM(2) = SLPHMX
      IFLIP = 1
C
C...  go through the remaining intervals generating  slope
C...  and  accum .
C
      DO 210 I = 2, NOLD
           HIOLD = XIOLD(I+1) - XIOLD(I)
           IF ( IFLIP .EQ. -1 )
     1                   CALL HORDER ( I, D1, HIOLD, DMZ, NCOMP, K)
           IF ( IFLIP .EQ. 1 )
     1                   CALL HORDER ( I, D2, HIOLD, DMZ, NCOMP, K)
           ONEOVH = 2.D0 / ( XIOLD(I+1) - XIOLD(I-1) )
           SLOPE(I) = 0.D0
C
C...       evaluate function to be equidistributed
C
           DO 200 J = 1, NTOL
             JJ = JTOL(J)
             JZ = LTOL(J)  +  (I-1) * MSTAR
  200      SLOPE(I) = DMAX1(SLOPE(I),(DABS(D2(JJ)-D1(JJ))*WGTMSH(J)*
     1                ONEOVH / (1.D0 + DABS(Z(JZ)))) **ROOT(J))
C
C...       accumulate approximate integral of function to be
C...       equidistributed
C
           TEMP = SLOPE(I) * (XIOLD(I+1)-XIOLD(I))
           SLPHMX = DMAX1(SLPHMX,TEMP)
           ACCUM(I+1) = ACCUM(I) + TEMP
  210 IFLIP = - IFLIP
      AVRG = ACCUM(NOLD+1) / DBLE(NOLD)
      DEGEQU = AVRG / DMAX1(SLPHMX,PRECIS)
C
C...  naccum=expected n to achieve .1x user requested tolerances
C
      NACCUM = ACCUM(NOLD+1) + 1.D0
      IF ( IPRINT .LT. 0 )  THEN
       WRITE(TMPBUF,350) DEGEQU, NACCUM
       CALL COLNEWMSGS(TMPBUF)
      ENDIF
C
C...  decide if mesh selection is worthwhile (otherwise, halve)
C
      IF ( AVRG .LT. PRECIS )                       GO TO 100
      IF ( DEGEQU .GE. .5D0 )                       GO TO 100
C
C...  nmx assures mesh has at least half as many subintervals as the
C...  previous mesh
C
      NMX = MAX0 ( NOLD+1, NACCUM ) / 2
C
C...  this assures that halving will be possible later (for error est)
C
      NMAX2 = NMAX / 2
C
C...  the mesh is at most halved
C
      N = MIN0 ( NMAX2, NOLD, NMX )
  220 NOLDP1 = NOLD + 1
      IF ( N .LT. NFXP1 )  N = NFXP1
      MSHNUM = MSHNUM + 1
C
C...  if the new mesh is smaller than the old mesh set mshnum
C...  so that the next call to  newmsh  will produce a halved
C...  mesh. if n .eq. nold / 2 increment mshalt so there can not
C...  be an infinite loop alternating between n and n/2 points.
C
      IF ( N .LT. NOLD )  MSHNUM = MSHLMT
      IF ( N .GT. NOLD/2 )  MSHALT = 1
      IF ( N .EQ. NOLD/2 )  MSHALT = MSHALT + 1
      MSHFLG = 0
C
C...  having decided to generate a new mesh with n subintervals we now
C...  do so, taking into account that the nfxpnt points in the array
C...  fixpnt must be included in the new mesh.
C
      IN = 1
      ACCL = 0.D0
      LOLD = 2
      XI(1) = ALEFT
      XI(N+1) = ARIGHT
      DO 310 I = 1, NFXP1
           IF ( I .EQ. NFXP1 )                      GO TO 250
           DO 230 J = LOLD, NOLDP1
             LNEW = J
             IF ( FIXPNT(I) .LE. XIOLD(J) )         GO TO 240
  230      CONTINUE
  240      CONTINUE
           ACCR = ACCUM(LNEW) + (FIXPNT(I)-XIOLD(LNEW))*SLOPE(LNEW-1)
           NREGN = (ACCR-ACCL) / ACCUM(NOLDP1) * DBLE(N) - .5D0
           NREGN = MIN0(NREGN, N - IN - NFXP1 + I)
           XI(IN + NREGN + 1) = FIXPNT(I)
           GO TO 260
  250      ACCR = ACCUM(NOLDP1)
           LNEW = NOLDP1
           NREGN = N - IN
  260      IF ( NREGN .EQ. 0 )                      GO TO 300
           TEMP = ACCL
           TSUM = (ACCR - ACCL) / DBLE(NREGN+1)
           DO 290 J = 1, NREGN
             IN = IN + 1
             TEMP = TEMP + TSUM
             DO 270 L = LOLD, LNEW
               LCARRY = L
               IF ( TEMP .LE. ACCUM(L) )            GO TO 280
  270        CONTINUE
  280        CONTINUE
             LOLD = LCARRY
  290      XI(IN) = XIOLD(LOLD-1) + (TEMP - ACCUM(LOLD-1)) /
     1     SLOPE(LOLD-1)
  300      IN = IN + 1
           ACCL = ACCR
           LOLD = LNEW
  310 CONTINUE
      MODE = 1
  320 CONTINUE
      NP1 = N + 1
      IF ( IPRINT .LT. 1 )  THEN
       WRITE(TMPBUF,340) N, (XI(I),I=1,NP1)
       CALL COLNEWMSGS(TMPBUF)
      ENDIF
      NZ   = MSTAR * (N + 1)
      NDMZ = KD * N
      RETURN
C----------------------------------------------------------------
  340 FORMAT(17H THE NEW MESH (OF,I5,16H SUBINTERVALS), ,100(8F12.6))
  350 FORMAT(21H MESH SELECTION INFO,30H DEGREE OF EQUIDISTRIBUTION =
     1       , F8.5, 28H PREDICTION FOR REQUIRED N = , I8)
  360 FORMAT(20H THE FORMER MESH (OF,I5,15H SUBINTERVALS),,
     1       100(8F12.6))
  370 FORMAT (23H  EXPECTED N TOO LARGE  )
      END
      SUBROUTINE CONSTS (K, RHO, COEF)
C
C**********************************************************************
C
C   purpose
C            assign (once) values to various array constants.
C
C   arrays assigned during compilation:
C     cnsts1 - weights for extrapolation error estimate
C     cnsts2 - weights for mesh selection
C              (the above weights come from the theoretical form for
C              the collocation error -- see [3])
C
C   arrays assigned during execution:
C     wgterr - the particular values of cnsts1 used for current run
C              (depending on k, m)
C     wgtmsh - gotten from the values of cnsts2 which in turn are
C              the constants in the theoretical expression for the
C              errors. the quantities in wgtmsh are 10x the values
C              in cnsts2 so that the mesh selection algorithm
C              is aiming for errors .1x as large as the user
C              requested tolerances.
C     jtol   - components of differential system to which tolerances
C              refer (viz, if ltol(i) refers to a derivative of u(j),
C              then jtol(i)=j)
C     root   - reciprocals of expected rates of convergence of compo-
C              nents of z(j) for which tolerances are specified
C     rho    - the k collocation points on (0,1)
C     coef   -
C     acol  -  the runge-kutta coefficients values at collocation
C              points
C
C**********************************************************************
C
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION RHO(7), COEF(K,*), CNSTS1(28), CNSTS2(28), DUMMY(1)
C
      COMMON /COLORD/ KDUM, NCOMP, MSTAR, KD, MMAX, M(20)
      COMMON /COLBAS/ B(28), ACOL(28,7), ASAVE(28,4)
      COMMON /COLEST/ TOL(40), WGTMSH(40), WGTERR(40), TOLIN(40),
     1                ROOT(40), JTOL(40), LTOL(40), NTOL
C
      DATA CNSTS1 /    .25D0,     .625D-1,  7.2169D-2, 1.8342D-2,
     1     1.9065D-2, 5.8190D-2, 5.4658D-3, 5.3370D-3, 1.8890D-2,
     2     2.7792D-2, 1.6095D-3, 1.4964D-3, 7.5938D-3, 5.7573D-3,
     3     1.8342D-2, 4.673D-3,  4.150D-4,  1.919D-3,  1.468D-3,
     4     6.371D-3,  4.610D-3,  1.342D-4,  1.138D-4,  4.889D-4,
     5     4.177D-4,  1.374D-3,  1.654D-3,  2.863D-3  /
      DATA CNSTS2 /   1.25D-1,   2.604D-3,  8.019D-3,  2.170D-5,
     1     7.453D-5,  5.208D-4,  9.689D-8,  3.689D-7,  3.100D-6,
     2     2.451D-5,  2.691D-10, 1.120D-9,  1.076D-8,  9.405D-8,
     3     1.033D-6,  5.097D-13, 2.290D-12, 2.446D-11, 2.331D-10,
     4     2.936D-9,  3.593D-8,  7.001D-16, 3.363D-15, 3.921D-14,
     5     4.028D-13, 5.646D-12, 7.531D-11, 1.129D-9  /
C
C...  assign weights for error estimate
C
      KOFF = K * ( K + 1 ) / 2
      IZ = 1
      DO 10 J = 1, NCOMP
           MJ = M(J)
           DO 10 L = 1, MJ
             WGTERR(IZ) = CNSTS1(KOFF - MJ + L)
             IZ = IZ + 1
   10 CONTINUE
C
C...  assign array values for mesh selection: wgtmsh, jtol, and root
C
      JCOMP = 1
      MTOT = M(1)
      DO 40 I = 1, NTOL
           LTOLI = LTOL(I)
   20      CONTINUE
           IF ( JCOMP .GT. NCOMP )                  GO TO 30
           IF ( LTOLI .LE. MTOT )                   GO TO 30
           JCOMP = JCOMP + 1
           MTOT = MTOT + M(JCOMP)
           GO TO 20
   30      CONTINUE
           JTOL(I) = JCOMP
           WGTMSH(I) = 1.D1 * CNSTS2(KOFF+LTOLI-MTOT) / TOLIN(I)
           ROOT(I) = 1.D0 / DBLE(K+MTOT-LTOLI+1)
   40 CONTINUE
C
C...  specify collocation points
C
      GO TO (50,60,70,80,90,100,110), K
   50 RHO(1) = 0.D0
      GO TO 120
   60 RHO(2) = .57735026918962576451D0
      RHO(1) = - RHO(2)
      GO TO 120
   70 RHO(3) = .77459666924148337704D0
      RHO(2) = .0D0
      RHO(1) = - RHO(3)
      GO TO 120
   80 RHO(4) = .86113631159405257523D0
      RHO(3) = .33998104358485626480D0
      RHO(2) = - RHO(3)
      RHO(1) = - RHO(4)
      GO TO 120
   90 RHO(5) = .90617984593866399280D0
      RHO(4) = .53846931010568309104D0
      RHO(3) = .0D0
      RHO(2) = - RHO(4)
      RHO(1) = - RHO(5)
      GO TO 120
  100 RHO(6) = .93246951420315202781D0
      RHO(5) = .66120938646626451366D0
      RHO(4) = .23861918608319690863D0
      RHO(3) = -RHO(4)
      RHO(2) = -RHO(5)
      RHO(1) = -RHO(6)
      GO TO 120
  110 RHO(7) = .94910791234275852452D0
      RHO(6) = .74153118559939443986D0
      RHO(5) = .40584515137739716690D0
      RHO(4) = 0.D0
      RHO(3) = -RHO(5)
      RHO(2) = -RHO(6)
      RHO(1) = -RHO(7)
  120 CONTINUE
C
C...  map (-1,1) to (0,1) by  t = .5 * (1. + x)
C
      DO 130 J = 1, K
         RHO(J) = .5D0 * (1.D0 + RHO(J))
  130 CONTINUE
C
C...  now find runge-kutta coeffitients b, acol and asave
C...  the values of asave are to be used in  newmsh  and errchk .
C
      DO 140 J = 1, K
         DO 135 I = 1, K
  135      COEF(I,J) = 0.D0
         COEF(J,J) = 1.D0
         CALL VMONDE (RHO, COEF(1,J), K)
  140 CONTINUE
      CALL RKBAS ( 1.D0, COEF, K, MMAX, B, DUMMY, 0)
      DO 150 I = 1, K
         CALL RKBAS ( RHO(I), COEF, K, MMAX, ACOL(1,I), DUMMY, 0)
  150 CONTINUE
      CALL RKBAS ( 1.D0/6.D0, COEF, K, MMAX, ASAVE(1,1), DUMMY, 0)
      CALL RKBAS ( 1.D0/3.D0, COEF, K, MMAX, ASAVE(1,2), DUMMY, 0)
      CALL RKBAS ( 2.D0/3.D0, COEF, K, MMAX, ASAVE(1,3), DUMMY, 0)
      CALL RKBAS ( 5.D0/6.D0, COEF, K, MMAX, ASAVE(1,4), DUMMY, 0)
      RETURN
      END
      SUBROUTINE ERRCHK (XI, Z, DMZ, VALSTR, IFIN)
C
C**********************************************************************
C
C      purpose
C               determine the error estimates and test to see if the
C               error tolerances are satisfied.
C
C      variables
C        xi     - current mesh points
C        valstr - values of the previous solution which are needed
C                 for the extrapolation- like error estimate.
C        wgterr - weights used in the extrapolation-like error
C                 estimate. the array values are assigned in
C                 subroutine  consts.
C        errest - storage for error estimates
C        err    - temporary storage used for error estimates
C        z      - approximate solution on mesh xi
C        ifin   - a 0-1 variable. on return it indicates whether
C                 the error tolerances were satisfied
C        mshflg - is set by errchk to indicate to newmsh whether
C                 any values of the current solution are stored in
C                 the array valstr. (0 for no, 1 for yes)
C
C**********************************************************************
C
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION ERR(40), ERREST(40), DUMMY(1)
      DIMENSION XI(*), Z(*), DMZ(*), VALSTR(*)
C
      COMMON /COLOUT/ PRECIS, IOUT, IPRINT
      COMMON /COLORD/ K, NCOMP, MSTAR, KD, MMAX, M(20)
      COMMON /COLAPR/ N, NOLD, NMAX, NZ, NDMZ
      COMMON /COLMSH/ MSHFLG, MSHNUM, MSHLMT, MSHALT
      COMMON /COLBAS/ B(28), ACOL(28,7), ASAVE(28,4)
      COMMON /COLEST/ TOL(40), WGTMSH(40), WGTERR(40), TOLIN(40),
     1                ROOT(40), JTOL(40), LTOL(40), NTOL
C
      CHARACTER TMPBUF*(4096)
C
C...  error estimates are to be generated and tested
C...  to see if the tolerance requirements are satisfied.
C
      IFIN = 1
      MSHFLG = 1
      DO 10 J = 1, MSTAR
   10   ERREST(J) = 0.D0
      DO 60 IBACK = 1, N
           I = N + 1 - IBACK
C
C...       the error estimates are obtained by combining values of
C...       the numerical solutions for two meshes.
C...       for each value of iback we will consider the two
C...       approximations at 2 points in each of
C...       the new subintervals.  we work backwards through
C...       the subinterval so that new values can be stored
C...       in valstr in case they prove to be needed later
C...       for an error estimate. the routine  newmsh
C...       filled in the needed values of the old solution
C...       in valstr.
C
           KNEW = ( 4 * (I-1) + 2 ) * MSTAR + 1
           KSTORE = ( 2 * (I-1) + 1 ) * MSTAR + 1
           X = XI(I) +  (XI(I+1)-XI(I)) * 2.D0 / 3.D0
           CALL APPROX (I, X, VALSTR(KNEW), ASAVE(1,3), DUMMY, XI,
     1            N, Z, DMZ, K, NCOMP, MMAX, M, MSTAR, 4, DUMMY, 0)
           DO 20 L = 1,MSTAR
             ERR(L) = WGTERR(L) * DABS(VALSTR(KNEW) -
     1       VALSTR(KSTORE))
             KNEW = KNEW + 1
             KSTORE = KSTORE + 1
   20      CONTINUE
           KNEW = ( 4 * (I-1) + 1 ) * MSTAR + 1
           KSTORE = 2 * (I-1) * MSTAR + 1
           X = XI(I) +  (XI(I+1)-XI(I)) / 3.D0
           CALL APPROX (I, X, VALSTR(KNEW), ASAVE(1,2), DUMMY, XI,
     1            N, Z, DMZ, K, NCOMP, MMAX, M, MSTAR, 4, DUMMY, 0)
           DO 30 L = 1,MSTAR
             ERR(L) = ERR(L) + WGTERR(L) * DABS(VALSTR(KNEW) -
     1       VALSTR(KSTORE))
             KNEW = KNEW + 1
             KSTORE = KSTORE + 1
   30      CONTINUE
C
C...       find component-wise maximum error estimate
C
           DO 40 L = 1,MSTAR
             ERREST(L) = DMAX1(ERREST(L),ERR(L))
   40      CONTINUE
C


C...       test whether the tolerance requirements are satisfied
C...       in the i-th interval.
C
           IF ( IFIN .EQ. 0 )                       GO TO 60
           DO 50 J = 1, NTOL
             LTOLJ = LTOL(J)
             LTJZ = LTOLJ  +  (I-1) * MSTAR
           IF ( ERR(LTOLJ) .GT.
     1          TOLIN(J) * (DABS(Z(LTJZ))+1.D0) )  IFIN = 0
   50      CONTINUE
   60 CONTINUE
      IF ( IPRINT .GE. 0 )                          RETURN
      WRITE(TMPBUF,130)
      CALL COLNEWMSGS(TMPBUF)
      LJ = 1
      DO 70 J = 1,NCOMP
           MJ = LJ - 1 + M(J)
           WRITE(TMPBUF,120) J, (ERREST(L), L= LJ, MJ)
           CALL COLNEWMSGS(TMPBUF)
           LJ = MJ + 1
   70 CONTINUE
      RETURN
C--------------------------------------------------------------
  120 FORMAT (3H U(, I2, 3H) -,4D12.4)
  130 FORMAT (26H THE ESTIMATED ERRORS ARE,)
      END
C---------------------------------------------------------------------
C                            p a r t  3
C          collocation system setup routines
C---------------------------------------------------------------------
C
      SUBROUTINE LSYSLV (MSING, XI, XIOLD, Z, DMZ, DELZ, DELDMZ,
     1           G, W, V, RHS, DMZO, INTEGS, IPVTG, IPVTW, RNORM,
     2           MODE, FSUB, DFSUB, GSUB, DGSUB, GUESS )
C*********************************************************************
C
C   purpose
C         this routine controls the set up and solution of a linear
C      system of collocation equations.
C         the matrix  g  is cast into an almost block diagonal
C      form by an appropriate ordering of the columns and solved
C      using the package of de boor-weiss [5]. the matrix is composed
C      of n blocks. the i-th block has the size
C                  integs(1,i) * integs(2,i).
C      it contains in its last rows the linearized collocation
C      equations, condensed as described in [2],
C      and the linearized side conditions corresponding to
C      the i-th subinterval.  integs(3,i)  steps of gaussian
C      elimination are applied to it to achieve a  partial plu
C      decomposition.  the right hand side vector is put into  rhs
C      and the solution vector is returned in  delz and deldmz.
C
C         lsyslv operates according to one of 5 modes:
C      mode = 0 - set up the collocation matrices  v , w , g
C                 and the right hand side  rhs ,  and solve.
C                 (for linear problems only.)
C      mode = 1 - set up the collocation matrices  v , w , g
C                 and the right hand sides  rhs  and  dmzo ,
C                 and solve. also set up  integs .
C                 (first iteration of nonlinear problems only).
C      mode = 2 - set up  rhs  only and compute its norm.
C      mode = 3 - set up  v, w, g  only and solve system.
C      mode = 4 - perform forward and backward substitution only
C                 (do not set up the matrices nor form the rhs).
C
C   variables
C
C      ig,izeta  - pointers to g,zeta respectively
C                       (necessary to keep track of blocks of g
C                       during matrix manipulations)
C      idmz,irhs,iv,iw - pointers to  rhs,v,w rspectively
C      df    - partial derivatives of f from dfsub
C      rnorm - euclidean norm of rhs
C      lside - number of side conditions in current and previous blocks
C      iguess = 1 when current soln is user specified via  guess
C             = 0 otherwise
C
C*********************************************************************
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION  Z(*), DMZ(*), DELZ(*), DELDMZ(*), XI(*), XIOLD(*)
      DIMENSION  G(*), W(*), V(*),  RHS(*) , DMZO(*), DUMMY(1)
      DIMENSION  INTEGS(3,*), IPVTG(*), IPVTW(*)
      DIMENSION  ZVAL(40), F(40), DGZ(40), DMVAL(20), DF(800), AT(28)
C
      COMMON /COLOUT/ PRECIS, IOUT, IPRINT
      COMMON /COLLOC/ RHO(7), COEF(49)
      COMMON /COLORD/ K, NCOMP, MSTAR, KD,  MMAX, M(20)
      COMMON /COLSID/ ZETA(40), ALEFT, ARIGHT, IZETA, IZSAVE
      COMMON /COLAPR/ N, NOLD, NMAX, NZ, NDMZ
      COMMON /COLNLN/ NONLIN, ITER, LIMIT, ICARE, IGUESS
      COMMON /COLBAS/ B(28), ACOL(28,7), ASAVE(28,4)
C
      EXTERNAL DFSUB, DGSUB, GUESS, FSUB, GSUB
C
      common/iercol/iero
      M1 = MODE + 1
      GO TO (10, 30, 30, 30, 310), M1
C
C...  linear problem initialization
C
   10 DO 20 I=1,MSTAR
   20 ZVAL(I) = 0.D0
C
C...  initialization
C
   30 IDMZ = 1
      IDMZO = 1
      IRHS = 1
      IG = 1
      IW = 1
      IV = 1
      IZETA = 1
      LSIDE = 0
      IOLD = 1
      NCOL = 2 * MSTAR
      RNORM = 0.D0
      IF ( MODE .GT. 1 )                            GO TO 80
C
C...  build integs (describing block structure of matrix)
C
      DO 70 I = 1,N
           INTEGS(2,I) = NCOL
           IF (I .LT. N)                            GO TO 40
           INTEGS(3,N) = NCOL
           LSIDE = MSTAR
           GO TO 60
   40      INTEGS(3,I) = MSTAR
   50      IF( LSIDE .EQ. MSTAR )                   GO TO 60
           IF ( ZETA(LSIDE+1) .GE. XI(I)+PRECIS )   GO TO 60
           LSIDE = LSIDE + 1
           GO TO 50
   60      NROW = MSTAR + LSIDE
   70 INTEGS(1,I) = NROW
   80 CONTINUE
      IF ( MODE .EQ. 2 )                            GO TO 90
C
C...  zero the matrices to be computed
C
      LW = KD * KD * N
      DO 84 L = 1, LW
   84   W(L) = 0.D0
C
C...  the do loop 290 sets up the linear system of equations.
C
  90  CONTINUE
      DO 290 I=1, N
C
C...       construct a block of  a  and a corresponding piece of  rhs.
C
           XII = XI(I)
           H = XI(I+1) - XI(I)
           NROW = INTEGS(1,I)
C
C...       go thru the ncomp collocation equations and side conditions
C...       in the i-th subinterval
C
  100      IF ( IZETA .GT. MSTAR )                  GO TO 140
           IF ( ZETA(IZETA) .GT. XII + PRECIS )      GO TO 140
C
C...       build equation for a side condition.
C
           IF ( MODE .EQ. 0 )                       GO TO 110
           IF ( IGUESS .NE. 1 )                     GO TO 102
C
C...       case where user provided current approximation
C
           CALL GUESS (XII, ZVAL, DMVAL)
           if (iero.gt.0) return
           GO TO 110
C
C...       other nonlinear case
C
  102      IF ( MODE .NE. 1 )                       GO TO 106
           CALL APPROX (IOLD, XII, ZVAL, AT, COEF, XIOLD, NOLD,
     1          Z, DMZ, K, NCOMP, MMAX, M, MSTAR, 2, DUMMY, 0)
           GO TO 110
  106      CALL APPROX (I, XII, ZVAL, AT, DUMMY, XI, N, Z, DMZ,
     1                  K, NCOMP, MMAX, M, MSTAR, 1, DUMMY, 0)
  108      IF ( MODE .EQ. 3 )                       GO TO 120
C
C...       find  rhs  boundary value.
C
  110      CALL GSUB (IZETA, ZVAL, GVAL)
           if (iero.gt.0) return
           RHS(NDMZ+IZETA) = -GVAL
           RNORM = RNORM + GVAL**2
           IF ( MODE .EQ. 2 )                       GO TO 130
C
C...       build a row of  a  corresponding to a boundary point
C
  120      CALL GDERIV (G(IG), NROW, IZETA, ZVAL, DGZ, 1, DGSUB)
           if (iero.gt.0) return
  130      IZETA = IZETA + 1
           GO TO 100
C
C...       assemble collocation equations
C
  140      DO 220 J = 1, K
             HRHO = H * RHO(J)
             XCOL = XII + HRHO
C
C...         this value corresponds to a collocation (interior)
C...         point. build the corresponding  ncomp  equations.
C
             IF ( MODE .EQ. 0 )                     GO TO 200
             IF ( IGUESS .NE. 1 )                   GO TO 160
C
C...         use initial approximation provided by the user.
C
             CALL GUESS (XCOL, ZVAL, DMZO(IRHS) )
             if (iero.gt.0) return
             GO TO 170
C
C...         find  rhs  values
C
  160        IF ( MODE .NE. 1 )                     GO TO 190
             CALL APPROX (IOLD, XCOL, ZVAL, AT, COEF, XIOLD, NOLD,
     1            Z, DMZ, K, NCOMP, MMAX, M, MSTAR, 2, DMZO(IRHS), 1)
C
  170        CALL FSUB (XCOL, ZVAL, F)
             if (iero.gt.0) return
             DO 180 JJ = 1, NCOMP
               VALUE = DMZO(IRHS) - F(JJ)
               RHS(IRHS) = - VALUE
               RNORM = RNORM + VALUE**2
               IRHS = IRHS + 1
  180        CONTINUE
             GO TO 210
C
C...         evaluate former collocation solution
C
  190        CALL APPROX (I, XCOL, ZVAL, ACOL(1,J), COEF, XI, N,
     1            Z, DMZ, K, NCOMP, MMAX, M, MSTAR, 4, DUMMY, 0)
             IF ( MODE .EQ. 3 )                     GO TO 210
C
C...         fill in  rhs  values (and accumulate its norm).
C
             CALL FSUB (XCOL, ZVAL, F)
             if (iero.gt.0) return
             DO 195 JJ = 1, NCOMP
               VALUE = DMZ(IRHS) - F(JJ)
               RHS(IRHS) = - VALUE
               RNORM = RNORM + VALUE**2
               IRHS = IRHS + 1
  195        CONTINUE
             GO TO 220
C
C...         the linear case
C
  200        CALL FSUB (XCOL, ZVAL, RHS(IRHS))
             if (iero.gt.0) return
             IRHS = IRHS + NCOMP
C
C...         fill in ncomp rows of  w and v
C
  210        CALL VWBLOK (XCOL, HRHO, J, W(IW), V(IV), IPVTW(IDMZ),
     1       KD, ZVAL, DF, ACOL(1,J), DMZO(IDMZO), NCOMP, DFSUB, MSING)
             if (iero.gt.0) return
             IF ( MSING .NE. 0 )                    RETURN
  220      CONTINUE
C
C...       build global bvp matrix  g
C
           IF ( MODE .NE. 2 )
     1      CALL GBLOCK (H, G(IG), NROW, IZETA, W(IW), V(IV), KD,
     2                  DUMMY, DELDMZ(IDMZ), IPVTW(IDMZ), 1 )
           IF ( I .LT. N )                          GO TO 280
           IZSAVE = IZETA
  240      IF ( IZETA .GT. MSTAR )                  GO TO 290
C
C...       build equation for a side condition.
C
           IF ( MODE .EQ. 0 )                       GO TO 250
           IF ( IGUESS .NE. 1 )                     GO TO 245
C
C...       case where user provided current approximation
C
           CALL GUESS (ARIGHT, ZVAL, DMVAL)
           if (iero.gt.0) return
           GO TO 250
C
C...       other nonlinear case
C
  245      IF ( MODE .NE. 1 )                       GO TO 246
           CALL APPROX (NOLD+1, ARIGHT, ZVAL, AT, COEF, XIOLD, NOLD,
     1          Z, DMZ, K, NCOMP, MMAX, M, MSTAR, 1, DUMMY, 0)
           GO TO 250
  246      CALL APPROX (N+1, ARIGHT, ZVAL, AT, COEF, XI, N,
     1          Z, DMZ, K, NCOMP, MMAX, M, MSTAR, 1, DUMMY, 0)
  248      IF ( MODE .EQ. 3 )                       GO TO 260
C
C...       find  rhs  boundary value.
C
  250      CALL GSUB (IZETA, ZVAL, GVAL)
           if (iero.gt.0) return
           RHS(NDMZ+IZETA) = - GVAL
           RNORM = RNORM + GVAL**2
           IF ( MODE .EQ. 2 )                       GO TO 270
C
C...       build a row of  a  corresponding to a boundary point
C
  260      CALL GDERIV (G(IG), NROW, IZETA+MSTAR, ZVAL, DGZ, 2, DGSUB)
           if (iero.gt.0) return
  270      IZETA = IZETA + 1
           GO TO 240
C
C...       update counters -- i-th block completed
C
  280      IG = IG + NROW * NCOL
           IV = IV + KD * MSTAR
           IW = IW + KD * KD
           IDMZ = IDMZ + KD
           IF ( MODE .EQ. 1 )  IDMZO = IDMZO + KD
  290 CONTINUE
C
C...       assembly process completed
C
      IF ( MODE .EQ. 0 .OR. MODE .EQ. 3 )           GO TO 300
      RNORM = DSQRT(RNORM / DBLE(NZ+NDMZ))
      IF ( MODE .NE. 2 )                            GO TO 300
      RETURN
C
C...  solve the linear system.
C
C...  matrix decomposition
C
  300 CALL FCBLOK (G, INTEGS, N, IPVTG, DF, MSING)
C
C...  check for singular matrix
C
      MSING = - MSING
      IF( MSING .NE. 0 )                            RETURN
C
C...  perform forward and backward substitution for mode=4 only.
C
  310 CONTINUE
      DO 311 L = 1, NDMZ
        DELDMZ(L) = RHS(L)
  311 CONTINUE
      IZ = 1
      IDMZ = 1
      IW = 1
      IZET = 1
      DO 320 I=1, N
         NROW = INTEGS(1,I)
         IZETA = NROW + 1 - MSTAR
         IF ( I .EQ. N ) IZETA = IZSAVE
  322    IF ( IZET .EQ. IZETA )                     GO TO 324
           DELZ(IZ-1+IZET) = RHS(NDMZ+IZET)
           IZET = IZET + 1
         GO TO 322
  324    H = XI(I+1) - XI(I)
         CALL GBLOCK (H, G(1), NROW, IZETA, W(IW), V(1), KD,
     1                DELZ(IZ), DELDMZ(IDMZ), IPVTW(IDMZ), 2 )
         IZ = IZ + MSTAR
         IDMZ = IDMZ + KD
         IW = IW + KD * KD
         IF ( I .LT. N )                            GO TO 320
  326    IF ( IZET .GT. MSTAR )                     GO TO 320
           DELZ(IZ-1+IZET) = RHS(NDMZ+IZET)
           IZET = IZET + 1
         GO TO 326
  320 CONTINUE
C
C...  perform forward and backward substitution for mode=0,2, or 3.
C
      CALL SBBLOK (G, INTEGS, N, IPVTG, DELZ)
C
C...  finaly find deldmz
C
      CALL DMZSOL (KD, MSTAR, N, V, DELZ, DELDMZ)
C
      IF ( MODE .NE. 1 )                            RETURN
      DO 321 L = 1, NDMZ
        DMZ(L) = DMZO(L)
  321 CONTINUE
      IZ = 1
      IDMZ = 1
      IW = 1
      IZET = 1
      DO 350 I=1, N
         NROW = INTEGS(1,I)
         IZETA = NROW + 1 - MSTAR
         IF ( I .EQ. N ) IZETA = IZSAVE
  330    IF ( IZET .EQ. IZETA )                     GO TO 340
           Z(IZ-1+IZET) = DGZ(IZET)
           IZET = IZET + 1
         GO TO 330
  340    H = XI(I+1) - XI(I)
         CALL GBLOCK (H, G(1), NROW, IZETA, W(IW), DF, KD,
     1                Z(IZ), DMZ(IDMZ), IPVTW(IDMZ), 2 )
         IZ = IZ + MSTAR
         IDMZ = IDMZ + KD
         IW = IW + KD * KD
         IF ( I .LT. N )                            GO TO 350
  342    IF ( IZET .GT. MSTAR )                     GO TO 350
            Z(IZ-1+IZET) = DGZ(IZET)
            IZET = IZET + 1
         GO TO 342
  350 CONTINUE
      CALL SBBLOK (G, INTEGS, N, IPVTG, Z)
C
C...  finaly find dmz
C
      CALL DMZSOL (KD, MSTAR, N, V, Z, DMZ)
C
      RETURN
      END
      SUBROUTINE GDERIV ( GI, NROW, IROW, ZVAL, DGZ, MODE, DGSUB)
C
C**********************************************************************
C
C   purpose:
C
C      construct a collocation matrix row according to mode:
C      mode = 1  -  a row corresponding to a initial condition
C                   (i.e. at the left end of the subinterval).
C      mode = 2  -  a row corresponding to a final condition.
C
C   variables:
C
C      gi     - the sub-block of the global bvp matrix in
C               which the equations are to be formed.
C      nrow   - no. of rows in gi.
C      irow   - the row in gi to be used for equations.
C      zval   - z(xi)
C      dg     - the derivatives of the side condition.
C
C**********************************************************************
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION GI(NROW,*), ZVAL(*), DGZ(*), DG(40)
      EXTERNAL DGSUB
C
      COMMON /COLORD/ KDUM, NDUM, MSTAR, KD, MMAX, M(20)
      COMMON /COLSID/ ZETA(40), ALEFT, ARIGHT, IZETA, IDUM
      COMMON /COLNLN/ NONLIN, ITER, LIMIT, ICARE, IGUESS
C
      common/iercol/iero

C...  zero jacobian dg
C
      DO 10 J=1,MSTAR
   10   DG(J) = 0.D0
C
C...  evaluate jacobian dg
C
      CALL DGSUB (IZETA, ZVAL, DG)
      if (iero.gt.0) return
C
C...  evaluate  dgz = dg * zval  once for a new mesh
C
      IF (NONLIN .EQ. 0 .OR. ITER .GT. 0)           GO TO 30
      DOT = 0.D0
      DO 20 J = 1, MSTAR
   20   DOT = DOT  +  DG(J) * ZVAL(J)
      DGZ(IZETA) = DOT
C
C...  branch according to  m o d e
C
   30 IF ( MODE .EQ. 2 )                            GO TO 50
C
C...  provide coefficients of the j-th linearized side condition.
C...  specifically, at x=zeta(j) the j-th side condition reads
C...  dg(1)*z(1) + ... +dg(mstar)*z(mstar) + g = 0
C
C
C...  handle an initial condition
C
      DO 40 J = 1, MSTAR
        GI(IROW,J) =  DG(J)
   40 GI(IROW,MSTAR+J) = 0.D0
      RETURN
C
C...  handle a final condition
C
   50 DO 60 J= 1, MSTAR
        GI(IROW,J) = 0.D0
   60 GI(IROW,MSTAR+J) = DG(J)
      RETURN
      END
      SUBROUTINE VWBLOK (XCOL, HRHO, JJ, WI, VI, IPVTW, KD, ZVAL,
     1                   DF, ACOL, DMZO, NCOMP, DFSUB, MSING)
C
C**********************************************************************
C
C   purpose:
C
C      construct a group of  ncomp  rows of the matrices  wi  and  vi.
C      corresponding to an interior collocation point.
C
C
C   variables:
C
C      xcol   - the location of the collocation point.
C      jj     - xcol is the jj-th of k collocation points
C               in the i-th subinterval.
C      wi,vi  - the i-th block of the collocation matrix
C               before parameter condensation.
C      kd     - no. of rows in vi and wi .
C      zval   - z(xcol)
C      df     - the jacobian at xcol .
C      jcomp  - counter for the component being dealt with.
C
C**********************************************************************
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION WI(KD,*), VI(KD,*), ZVAL(*), DMZO(*), DF(NCOMP,*)
      DIMENSION IPVTW(*),  HA(7,4), ACOL(7,4), BASM(5)
      EXTERNAL DFSUB
C
      COMMON /COLORD/ K, NCDUM, MSTAR, KDUM, MMAX, M(20)
      COMMON /COLNLN/ NONLIN, ITER, LIMIT, ICARE, IGUESS
C
      common/iercol/iero
C...  if jj = 1 initialize  wi .
C
      IF ( JJ .GT. 1 )                              GO TO 30
      DO 10 ID = 1, KD
        WI(ID,ID) = 1.D0
   10 CONTINUE
C
C...  calculate local basis
C
   30        FACT = 1.D0
             DO 150 L=1,MMAX
                FACT = FACT * HRHO / DBLE(L)
                BASM(L) = FACT
                DO 150 J=1,K
                   HA(J,L) = FACT * ACOL(J,L)
  150        CONTINUE
C
C... zero jacobian
C
      DO 40 JCOL = 1, MSTAR
        DO 40 IR = 1, NCOMP
   40 DF(IR,JCOL) = 0.D0
C
C...  build ncomp rows for interior collocation point x.
C...  the linear expressions to be constructed are:
C...   (m(id))
C...  u     -  df(id,1)*z(1) - ... - df(id,mstar)*z(mstar)
C...   id
C...  for id = 1 to ncomp.
C
      CALL DFSUB (XCOL, ZVAL, DF)
      if (iero.gt.0) return
      I0 = (JJ-1) * NCOMP
      I1 = I0 + 1
      I2 = I0 + NCOMP
C
C...  evaluate  dmzo = dmz - df * zval  once for a new mesh
C
      IF (NONLIN .EQ. 0 .OR. ITER .GT. 0)          GO TO 60
      DO 50 J = 1, MSTAR
        FACT = - ZVAL(J)
        DO 50 ID = 1, NCOMP
          DMZO(I0+ID) = DMZO(I0+ID)  +  FACT * DF(ID,J)
  50  CONTINUE
C
C...  loop over the  ncomp  expressions to be set up for the
C...  current collocation point.
C
   60 DO 70 J = 1, MSTAR
        DO 70 ID = 1, NCOMP
          VI(I0+ID,J) = DF(ID,J)
   70 CONTINUE
      JN = 1
      DO 140 JCOMP = 1, NCOMP
         MJ = M(JCOMP)
         JN = JN + MJ
         DO 130 L = 1, MJ
            JV = JN - L
            JW = JCOMP
            DO 90 J = 1, K
              AJL = - HA(J,L)
              DO 80 IW = I1, I2
                 WI(IW,JW) = WI(IW,JW)  +  AJL * VI(IW,JV)
   80         CONTINUE
   90       JW = JW + NCOMP
            LP1 = L + 1
            IF ( L .EQ. MJ )                        GO TO 130
            DO 110 LL = LP1, MJ
              JDF = JN - LL
              BL = BASM(LL-L)
              DO 100 IW = I1, I2
                VI(IW,JV) = VI(IW,JV)  +  BL * VI(IW,JDF)
  100         CONTINUE
  110       CONTINUE
  130    CONTINUE
  140 CONTINUE
      IF ( JJ .LT. K )                          RETURN
C
C   ...decompose the wi block and solve for the mstar columns of vi
C
C
C...  do parameter condensation
C
      MSING = 0
      CALL DGEFA  (WI, KD, KD, IPVTW, MSING)
C
C...   check for singularity
C
      IF ( MSING .NE. 0 )                         RETURN
      DO 250 J= 1,MSTAR
         CALL DGESL  (WI, KD, KD, IPVTW, VI(1,J), 0)
  250 CONTINUE
      RETURN
      END
      SUBROUTINE GBLOCK (H, GI, NROW, IROW, WI, VI, KD,
     1                   RHSZ, RHSDMZ, IPVTW, MODE)
C
C**********************************************************************
C
C   purpose:
C
C      construct collocation matrix rows according to mode:
C      mode = 1  -  a group of  mstar    rows corresponding
C                   an interior mesh interval.
C           = 2  -  corresponding right hand side
C
C   variables:
C
C      h      - the  local stepsize.
C      gi     - the sub-block of the collocation matrix in
C               which the equations are to be formed.
C      wi     - the sub-block of noncondensed collocation equations,
C               left-hand side part.
C      vi     - the sub-block of noncondensed collocation equations,
C               right-hand side part.
C      rhsdmz - the inhomogenous term of the uncondensed collocation
C               equations.
C      rhsz   - the inhomogenous term of the condensed collocation
C               equations.
C      nrow   - no. of rows in gi.
C      irow   - the first row in gi to be used for equations.
C
C**********************************************************************
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION HB(7,4), BASM(5)
      DIMENSION GI(NROW,*), WI(*), VI(KD,*)
      DIMENSION RHSZ(*), RHSDMZ(*), IPVTW(*)
C
      COMMON /COLORD/  K, NCOMP, MSTAR, KDUM, MMAX, M(20)
      COMMON /COLBAS/ B(7,4), ACOL(28,7), ASAVE(28,4)
C
C...  compute local basis
C
      FACT = 1.D0
      BASM(1) = 1.D0
      DO 30 L=1,MMAX
         FACT = FACT * H / DBLE(L)
         BASM(L+1) = FACT
         DO 20 J=1,K
   20       HB(J,L) = FACT * B(J,L)
   30 CONTINUE
C
C...  branch according to  m o d e
C
      GO TO (40, 110), MODE
C
C...  set right gi-block to identity
C
   40 CONTINUE
      DO 60 J = 1, MSTAR
        DO 50 IR = 1, MSTAR
          GI(IROW-1+IR,J) = 0.D0
   50   GI(IROW-1+IR,MSTAR+J) = 0.D0
   60 GI(IROW-1+J,MSTAR+J) = 1.D0
C
C...  compute the block gi
C
      IR = IROW
      DO 100 ICOMP = 1, NCOMP
         MJ = M(ICOMP)
         IR = IR + MJ
         DO 90 L = 1, MJ
            ID = IR - L
            DO 80 JCOL = 1, MSTAR
               IND = ICOMP
               RSUM = 0.D0
               DO 70 J = 1, K
                  RSUM = RSUM  -  HB(J,L) * VI(IND,JCOL)
   70          IND = IND + NCOMP
               GI(ID,JCOL) = RSUM
   80       CONTINUE
            JD = ID - IROW
            DO 85 LL = 1, L
               GI(ID,JD+LL) = GI(ID,JD+LL) - BASM(LL)
   85       CONTINUE
   90    CONTINUE
  100 CONTINUE
      RETURN
C
C...  compute the appropriate piece of  rhsz
C
  110 CONTINUE
      CALL DGESL  (WI, KD, KD, IPVTW, RHSDMZ, 0)
      IR = IROW
      DO 140 JCOMP = 1, NCOMP
         MJ = M(JCOMP)
         IR = IR + MJ
         DO 130 L = 1, MJ
            IND = JCOMP
            RSUM = 0.D0
            DO 120 J = 1, K
               RSUM = RSUM  +  HB(J,L) * RHSDMZ(IND)
  120       IND = IND + NCOMP
            RHSZ(IR-L) = RSUM
  130    CONTINUE
  140 CONTINUE
      RETURN
      END
C
C----------------------------------------------------------------------
C                             p a r t  4
C               polynomial and service routines
C----------------------------------------------------------------------
C
      SUBROUTINE APPSLN (X, Z, FSPACE, ISPACE)
C
C*****************************************************************
C
C     purpose
C
C           set up a standard call to  approx  to evaluate the
C           approximate solution  z = z( u(x) )  at a point x
C           (it has been computed by a call to  colnew ).
C           the parameters needed for  approx  are retrieved
C           from the work arrays  ispace  and  fspace .
C
C*****************************************************************
C
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION Z(*), FSPACE(*), ISPACE(*), A(28), DUMMY(1)
      IS6 = ISPACE(6)
      IS5 = ISPACE(1) + 2
      IS4 = IS5 + ISPACE(4) * (ISPACE(1) + 1)
      I = 1
      CALL APPROX (I, X, Z, A, FSPACE(IS6), FSPACE(1), ISPACE(1),
     1             FSPACE(IS5), FSPACE(IS4), ISPACE(2), ISPACE(3),
     2             ISPACE(5), ISPACE(8), ISPACE(4), 2, DUMMY, 0)
      RETURN
      END
      SUBROUTINE APPROX (I, X, ZVAL, A, COEF, XI, N, Z, DMZ, K,
     1                   NCOMP, MMAX, M, MSTAR, MODE, DMVAL, MODM )
C
C**********************************************************************
C
C   purpose
C                                    (1)       (m1-1)     (mncomp-1)
C           evaluate z(u(x))=(u (x),u (x),...,u  (x),...,u  (x)      )
C                              1     1         1          mncomp
C           at one point x.
C
C   variables
C     a      - array of mesh independent rk-basis coefficients
C     basm   - array of mesh dependent monomial coefficients
C     xi     - the current mesh (having n subintervals)
C     z      - the current solution vector
C     dmz    - the array of mj-th derivatives of the current solution
C     mode   - determines the amount of initialization needed
C            = 4  forms z(u(x)) using z, dmz and ha
C            = 3  as in =4, but computes local rk-basis
C            = 2  as in =3, but determines i such that
C                       xi(i) .le. x .lt. xi(i+1) (unless x=xi(n+1))
C            = 1  retrieve  z=z(u(x(i)))  directly
C
C**********************************************************************
C
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION ZVAL(*), DMVAL(*), XI(*), M(*), A(7,*), DM(7)
      DIMENSION Z(*), DMZ(*), BM(4), COEF(*)
C
      COMMON /COLOUT/ PRECIS, IOUT, IPRINT
C
      CHARACTER TMPBUF*(4096)
C
      GO TO (10, 30, 80, 90), MODE
C
C...  mode = 1 , retrieve  z( u(x) )  directly for x = xi(i).
C
   10 X  = XI(I)
      IZ = (I-1) * MSTAR
      DO 20 J = 1, MSTAR
        IZ = IZ + 1
        ZVAL(J) = Z(IZ)
   20 CONTINUE
      RETURN
C
C...  mode = 2 ,  locate i so  xi(i) .le. x .lt. xi(i+1)
C
   30 CONTINUE
      IF ( X .GE. XI(1)-PRECIS .AND. X .LE. XI(N+1)+PRECIS )
     1                                              GO TO 40
      IF (IPRINT .LT. 1)  THEN
       WRITE(TMPBUF,900) X, XI(1), XI(N+1)
       CALL COLNEWMSGS(TMPBUF)
      ENDIF
      IF ( X .LT. XI(1) )  X = XI(1)
      IF ( X .GT. XI(N+1) )  X = XI(N+1)
   40 IF ( I .GT. N .OR. I .LT. 1 )  I = (N+1) / 2
      ILEFT = I
      IF ( X .LT. XI(ILEFT) )                       GO TO 60
      DO 50 L = ILEFT, N
           I = L
           IF ( X .LT. XI(L+1) )                    GO TO 80
   50 CONTINUE
      GO TO 80
   60 IRIGHT = ILEFT - 1
      DO 70 L = 1, IRIGHT
           I = IRIGHT + 1 - L
           IF ( X .GE. XI(I) )                      GO TO 80
   70 CONTINUE
C
C...  mode = 2 or 3 , compute mesh independent rk-basis.
C
   80 CONTINUE
      S = (X - XI(I)) / (XI(I+1) - XI(I))
      CALL RKBAS ( S, COEF, K, MMAX, A, DM, MODM )
C
C...  mode = 2, 3, or 4 , compute mesh dependent rk-basis.
C
   90 CONTINUE
      BM(1) = X - XI(I)
      DO 95 L = 2, MMAX
         BM(L) = BM(1) / DBLE(L)
   95 CONTINUE
C
C...  evaluate  z( u(x) ).
C
  100 IR = 1
      IZ = (I-1) * MSTAR + 1
      IDMZ = (I-1) * K * NCOMP
      DO 140 JCOMP = 1, NCOMP
          MJ = M(JCOMP)
          IR = IR + MJ
          IZ = IZ + MJ
          DO 130 L = 1, MJ
             IND = IDMZ + JCOMP
             ZSUM = 0.D0
             DO 110 J = 1, K
               ZSUM = ZSUM  +  A(J,L) * DMZ(IND)
  110        IND = IND + NCOMP
             DO 120 LL = 1, L
               LB = L + 1 - LL
  120          ZSUM = ZSUM * BM(LB)  +  Z(IZ-LL)
  130     ZVAL(IR-L) = ZSUM
  140 CONTINUE
      IF ( MODM .EQ. 0 )                            RETURN
C
C...  for modm = 1 evaluate  dmval(j) = mj-th derivative of uj.
C
      DO 150 JCOMP = 1, NCOMP
  150 DMVAL(JCOMP) = 0.D0
      IDMZ = IDMZ + 1
      DO 170 J = 1, K
         FACT = DM(J)
         DO 160 JCOMP = 1, NCOMP
            DMVAL(JCOMP) = DMVAL(JCOMP)  +  FACT * DMZ(IDMZ)
            IDMZ = IDMZ + 1
  160    CONTINUE
  170 CONTINUE
      RETURN
C--------------------------------------------------------------------
  900 FORMAT(37H ****** DOMAIN ERROR IN APPROX ******
     1       4H X =,D20.10, 10H   ALEFT =,D20.10,
     2       11H   ARIGHT =,D20.10)
      END
      SUBROUTINE RKBAS (S, COEF, K, M, RKB, DM, MODE)
C
C**********************************************************************
C
C   purpose
C           evaluate mesh independent runge-kutta basis for given s
C
C   variables
C     s      - argument, i.e. the relative position for which
C              the basis is to be evaluated ( 0. .le. s .le. 1. ).
C     coef   - precomputed derivatives of the basis
C     k      - number of collocatin points per subinterval
C     m      - maximal order of the differential equation
C     rkb    - the runge-kutta basis (0-th to (m-1)-th derivatives )
C     dm     - basis elements for m-th derivative
C
C**********************************************************************
C
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION COEF(K,*), RKB(7,*), DM(*), T(10)
C
      IF ( K .EQ. 1 )                            GO TO 70
      KPM1 = K + M - 1
      DO 10 I = 1, KPM1
   10   T(I) = S / DBLE(I)
      DO 40 L = 1, M
         LB = K + L + 1
         DO 30 I = 1, K
           P = COEF(1,I)
           DO 20 J = 2, K
             P = P * T(LB-J)  + COEF(J,I)
   20      CONTINUE
           RKB(I,L) = P
   30    CONTINUE
   40 CONTINUE
      IF ( MODE .EQ. 0 )                         RETURN
      DO 60 I = 1, K
         P = COEF(1,I)
         DO 50 J = 2, K
   50       P = P * T(K+1-J) + COEF(J,I)
         DM(I) = P
   60 CONTINUE
      RETURN
   70 RKB(1,1) = 1.0D0
      DM(1) = 1.0D0
      RETURN
      END
      SUBROUTINE VMONDE ( RHO, COEF, K )
C
C**********************************************************************
C
C   purpose
C          solve vandermonde system v * x = e
C          with  v(i,j) = rho(j)**(i-1)/(i-1)! .
C
C**********************************************************************
C
      INTEGER K, I,IFAC,J,KM1,KMI
      DOUBLE PRECISION RHO(K), COEF(K)
C
      IF ( K .EQ. 1 )                             RETURN
      KM1 = K - 1
      DO 10 I = 1, KM1
         KMI = K - I
         DO 10 J = 1, KMI
           COEF(J) = (COEF(J+1) - COEF(J)) / (RHO(J+I) - RHO(J))
  10  CONTINUE
C
      IFAC = 1
      DO 40 I = 1, KM1
         KMI = K + 1 - I
         DO 30 J = 2, KMI
  30        COEF(J) = COEF(J) - RHO(J+I-1) * COEF(J-1)
         COEF(KMI) = DBLE(IFAC) * COEF(KMI)
         IFAC = IFAC * I
  40  CONTINUE
      COEF(1) = DBLE(IFAC) * COEF(1)
      RETURN
      END
      SUBROUTINE HORDER (I, UHIGH, HI, DMZ, NCOMP, K)
C
C**********************************************************************
C
C   purpose
C           determine highest order (piecewise constant) derivatives
C           of the current collocation solution
C
C   variables
C     hi     - the stepsize, hi = xi(i+1) - xi(i)
C     dmz    - vector of mj-th derivative of the solution
C     uhigh  - the array of highest order (piecewise constant)
C              derivatives of the approximate solution on
C              (xi(i),xi(i+1)), viz,
C                          (k+mj-1)
C              uhigh(j) = u   (x)    on (xi(i),xi(i+1))
C                          j
C
C**********************************************************************
C
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION UHIGH(*), DMZ(*)
C
      COMMON /COLLOC/ RHO(7), COEF(49)
C
      DN = 1.D0 / HI**(K-1)
C
C...  loop over the ncomp solution components
C
      DO 10 ID = 1, NCOMP
         UHIGH(ID) = 0.D0
   10 CONTINUE
      KIN = 1
      IDMZ = (I-1) * K * NCOMP + 1
      DO 30 J = 1, K
         FACT = DN * COEF(KIN)
         DO 20 ID = 1, NCOMP
            UHIGH(ID) = UHIGH(ID)  +  FACT * DMZ(IDMZ)
            IDMZ = IDMZ + 1
   20    CONTINUE
         KIN = KIN + K
   30 CONTINUE
      RETURN
      END
      SUBROUTINE DMZSOL (KD, MSTAR, N, V, Z, DMZ)
C
C**********************************************************************
C
C   purpose
C          compute dmz in a blockwise manner
C          dmz(i) = dmz(i)  +  v(i) * z(i), i = 1,...,n
C
C**********************************************************************
C
C      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION V(KD,*), DMZ(KD,*), Z(*)
C
      JZ = 1
      DO 30 I = 1, N
         DO 20 J = 1, MSTAR
            FACT = Z(JZ)
            DO 10 L = 1, KD
               DMZ(L,I) = DMZ(L,I)  +  FACT * V(L,JZ)
   10       CONTINUE
            JZ = JZ + 1
   20    CONTINUE
   30 CONTINUE
      RETURN
      END
C----------------------------------------------------------------------
C                            p a r t  5
C          we list here a modified (column oriented, faster)
C          version of the package solveblok of de boor - weiss [5].
C          we also give a listing of the linpack
C          routines dgefa und dgesl used by colnew.
C----------------------------------------------------------------------
C
      SUBROUTINE FCBLOK (BLOKS, INTEGS, NBLOKS, IPIVOT, SCRTCH, INFO)
C
C
C     calls subroutines  factrb  and  shiftb .
C
C     fcblok  supervises the plu factorization with pivoting of
C     scaled rows of the almost block diagonal matrix stored in the
C     arrays  bloks  and  integs .
C
C     factrb = subprogram which carries out steps 1,...,last of gauss
C            elimination (with pivoting) for an individual block.
C     shiftb = subprogram which shifts the remaining rows to the top of
C            the next block
C
C     parameters
C      bloks   an array that initially contains the almost block diago-
C            nal matrix  a  to be factored, and on return contains the
C            computed factorization of  a .
C      integs  an integer array describing the block structure of  a .
C      nbloks  the number of blocks in  a .
C      ipivot  an integer array of dimension   sum (integs(3,n) ; n=1,
C            ...,nbloks) which, on return, contains the pivoting stra-
C            tegy used.
C      scrtch  work area required, of length  max (integs(1,n) ; n=1,
C            ...,nbloks).
C      info    output parameter;
C            = 0  in case matrix was found to be nonsingular.
C            otherwise,
C            = n if the pivot element in the nth gauss step is zero.
C
C**********************************************************************
C
      INTEGER INTEGS(3,NBLOKS),IPIVOT(*),INFO, I,INDEX,INDEXN,LAST,
     1        NCOL,NROW
      DOUBLE PRECISION BLOKS(*),SCRTCH(*)
      INFO = 0
      INDEXX = 1
      INDEXN = 1
      I = 1
C
C...  loop over the blocks.  i  is loop index
C
   10      INDEX = INDEXN
           NROW = INTEGS(1,I)
           NCOL = INTEGS(2,I)
           LAST = INTEGS(3,I)
C
C...       carry out elimination on the i-th block until next block
C...       enters, i.e., for columns 1,...,last  of i-th block.
C
           CALL FACTRB ( BLOKS(INDEX), IPIVOT(INDEXX), SCRTCH, NROW,
     1                   NCOL, LAST, INFO)
C
C...       check for having reached a singular block or the last block
C
           IF ( INFO .NE. 0 )                       GO TO 20
           IF ( I .EQ. NBLOKS )                     RETURN
           I = I+1
           INDEXN = NROW * NCOL + INDEX
           INDEXX = INDEXX + LAST
C
C...       put the rest of the i-th block onto the next block
C
           CALL SHIFTB ( BLOKS(INDEX), NROW, NCOL, LAST,
     1                   BLOKS(INDEXN), INTEGS(1,I), INTEGS(2,I) )
      GO TO 10
   20 INFO = INFO + INDEXX - 1
      RETURN
      END
      SUBROUTINE FACTRB ( W, IPIVOT, D, NROW, NCOL, LAST, INFO)
C
C********************************************************************
C
C     adapted from p.132 of  element.numer.analysis  by conte-de boor
C
C     constructs a partial plu factorization, corresponding to steps
C      1,..., last   in gauss elimination, for the matrix  w  of
C      order ( nrow ,  ncol ), using pivoting of scaled rows.
C
C     parameters
C       w       contains the (nrow,ncol) matrix to be partially factored
C               on input, and the partial factorization on output.
C       ipivot  an integer array of length last containing a record of
C               the pivoting strategy used; explicit interchanges
C               are used for pivoting.
C       d       a work array of length nrow used to store row sizes
C               temporarily.
C       nrow    number of rows of w.
C       ncol    number of columns of w.
C       last    number of elimination steps to be carried out.
C       info    on output, zero if the matrix is found to be non-
C               singular, in case a zero pivot was encountered in row
C               n,  info = n on output.
C
C**********************************************************************
C
      INTEGER IPIVOT(NROW),NCOL,LAST,INFO, I,J,K,L,KP1
      DOUBLE PRECISION W(NROW,NCOL),D(NROW), COLMAX,T,S
      DOUBLE PRECISION DABS,DMAX1
C
C...  initialize  d
C
      DO 10 I = 1, NROW
        D(I) = 0.D0
   10 CONTINUE
      DO 20 J = 1, NCOL
        DO 20 I = 1, NROW
          D(I) = DMAX1( D(I) , DABS(W(I,J)))
   20 CONTINUE
C
C...  gauss elimination with pivoting of scaled rows, loop over
C...  k=1,.,last
C
      K = 1
C
C...  as pivot row for k-th step, pick among the rows not yet used,
C...  i.e., from rows  k ,..., nrow , the one whose k-th entry
C...  (compared to the row size) is largest. then, if this row
C...  does not turn out to be row k, interchange row k with this
C...  particular row and redefine ipivot(k).
C
   30      CONTINUE
           IF ( D(K) .EQ. 0.D0 )                    GO TO 90
           IF (K .EQ. NROW)                         GO TO 80
           L = K
           KP1 = K+1
           COLMAX = DABS(W(K,K)) / D(K)
C
C...       find the (relatively) largest pivot
C
           DO 40 I = KP1, NROW
             IF ( DABS(W(I,K)) .LE. COLMAX * D(I) ) GO TO 40
             COLMAX = DABS(W(I,K)) / D(I)
             L = I
   40      CONTINUE
           IPIVOT(K) = L
           T = W(L,K)
           S = D(L)
           IF ( L .EQ. K )                          GO TO 50
             W(L,K) = W(K,K)
             W(K,K) = T
             D(L) = D(K)
             D(K) = S
   50      CONTINUE
C
C...       if pivot element is too small in absolute value, declare
C...       matrix to be noninvertible and quit.
C
           IF ( DABS(T)+D(K) .LE. D(K) )            GO TO 90
C
C...       otherwise, subtract the appropriate multiple of the pivot
C...       row from remaining rows, i.e., the rows (k+1),..., (nrow)
C...       to make k-th entry zero. save the multiplier in its place.
C...       for high performance do this operations column oriented.
C
           T = -1.0D0/T
           DO 60 I = KP1, NROW
   60        W(I,K) = W(I,K) * T
           DO 70 J=KP1,NCOL
             T = W(L,J)
             IF ( L .EQ. K )                        GO TO 62
               W(L,J) = W(K,J)
               W(K,J) = T
   62        IF ( T .EQ. 0.D0 )                     GO TO 70
             DO 64 I = KP1, NROW
   64           W(I,J) = W(I,J) + W(I,K) * T
   70      CONTINUE
           K = KP1
C
C...       check for having reached the next block.
C
           IF ( K .LE. LAST )                       GO TO 30
      RETURN
C
C...  if  last  .eq. nrow , check now that pivot element in last row
C...  is nonzero.
C
   80 IF( DABS(W(NROW,NROW))+D(NROW) .GT. D(NROW) ) RETURN
C
C...  singularity flag set
C
   90 INFO = K
      RETURN
      END
      SUBROUTINE SHIFTB (AI, NROWI, NCOLI, LAST, AI1, NROWI1, NCOLI1)
C
C*********************************************************************
C
C     shifts the rows in current block, ai, not used as pivot rows, if
C     any, i.e., rows  (last+1),..., (nrowi), onto the first mmax =
C      = nrow-last  rows of the next block, ai1, with column last+j of
C      ai  going to column j , j=1,...,jmax=ncoli-last. the remaining
C     columns of these rows of ai1 are zeroed out.
C
C                                picture
C
C          original situation after         results in a new block i+1
C          last = 2 columns have been       created and ready to be
C          done in factrb (assuming no      factored by next factrb
C          interchanges of rows)            call.
C                      1
C                 x  x 1x  x  x           x  x  x  x  x
C                      1
C                 0  x 1x  x  x           0  x  x  x  x
C     block i          1                       ---------------
C     nrowi = 4   0  0 1x  x  x           0  0 1x  x  x  0  01
C     ncoli = 5        1                       1             1
C     last = 2    0  0 1x  x  x           0  0 1x  x  x  0  01
C     -------------------------------          1             1   new
C                      1x  x  x  x  x          1x  x  x  x  x1  block
C                      1                       1             1   i+1
C     block i+1        1x  x  x  x  x          1x  x  x  x  x1
C     nrowi1= 5        1                       1             1
C     ncoli1= 5        1x  x  x  x  x          1x  x  x  x  x1
C     -------------------------------          1-------------1
C                      1
C
C*********************************************************************
C
      INTEGER LAST, J,JMAX,JMAXP1,M,MMAX
      DOUBLE PRECISION AI(NROWI,NCOLI),AI1(NROWI1,NCOLI1)
      MMAX = NROWI - LAST
      JMAX = NCOLI - LAST
      IF (MMAX .LT. 1 .OR. JMAX .LT. 1)             RETURN
C
C...  put the remainder of block i into ai1
C
      DO 10 J=1,JMAX
           DO 10 M=1,MMAX
   10 AI1(M,J) = AI(LAST+M,LAST+J)
      IF (JMAX .EQ. NCOLI1)                         RETURN
C
C...  zero out the upper right corner of ai1
C
      JMAXP1 = JMAX + 1
      DO 20 J=JMAXP1,NCOLI1
           DO 20 M=1,MMAX
   20 AI1(M,J) = 0.D0
      RETURN
      END
      SUBROUTINE SBBLOK ( BLOKS, INTEGS, NBLOKS, IPIVOT, X )
C
C**********************************************************************
C
C     calls subroutines  subfor  and  subbak .
C
C     supervises the solution (by forward and backward substitution) of
C     the linear system  a*x = b  for x, with the plu factorization of
C     a  already generated in  fcblok .  individual blocks of
C     equations are solved via  subfor  and  subbak .
C
C    parameters
C       bloks, integs, nbloks, ipivot    are as on return from fcblok.
C       x       on input: the right hand side, in dense storage
C               on output: the solution vector
C
C*********************************************************************
C
      INTEGER INTEGS(3,NBLOKS),IPIVOT(*), I,INDEX,INDEXX,J,LAST,
     1        NBP1,NCOL,NROW
      DOUBLE PRECISION BLOKS(*), X(*)
C
C...  forward substitution pass
C
      INDEX = 1
      INDEXX = 1
      DO 10 I = 1, NBLOKS
           NROW = INTEGS(1,I)
           LAST = INTEGS(3,I)
           CALL SUBFOR ( BLOKS(INDEX), IPIVOT(INDEXX), NROW, LAST,
     1                   X(INDEXX) )
           INDEX = NROW * INTEGS(2,I) + INDEX
   10 INDEXX = INDEXX + LAST
C
C...  back substitution pass
C
      NBP1 = NBLOKS + 1
      DO 20 J = 1, NBLOKS
           I = NBP1 - J
           NROW = INTEGS(1,I)
           NCOL = INTEGS(2,I)
           LAST = INTEGS(3,I)
           INDEX = INDEX - NROW * NCOL
           INDEXX = INDEXX - LAST
   20 CALL SUBBAK ( BLOKS(INDEX), NROW, NCOL, LAST, X(INDEXX) )
      RETURN
      END
      SUBROUTINE SUBFOR ( W, IPIVOT, NROW, LAST, X )
C
C**********************************************************************
C
C     carries out the forward pass of substitution for the current
C     block, i.e., the action on the right side corresponding to the
C     elimination carried out in  factrb  for this block.
C
C    parameters
C       w, ipivot, nrow, last  are as on return from factrb.
C       x(j)  is expected to contain, on input, the right side of j-th
C             equation for this block, j=1,...,nrow.
C       x(j)  contains, on output, the appropriately modified right
C             side of equation (j) in this block, j=1,...,last and
C             for j=last+1,...,nrow.
C
C*********************************************************************
C
      INTEGER IPIVOT(LAST), IP,K,KP1,LSTEP
      DOUBLE PRECISION W(NROW,LAST), X(NROW), T
C
      IF ( NROW .EQ. 1 )                            RETURN
      LSTEP = MIN0( NROW-1 , LAST )
      DO 20 K = 1, LSTEP
           KP1 = K + 1
           IP = IPIVOT(K)
           T = X(IP)
           X(IP) = X(K)
           X(K) = T
           IF ( T .EQ. 0.D0 )                       GO TO 20
           DO 10 I = KP1, NROW
   10         X(I) = X(I) + W(I,K) * T
   20 CONTINUE
   30 RETURN
      END
      SUBROUTINE SUBBAK ( W, NROW, NCOL, LAST, X )
C
C*********************************************************************
C
C     carries out backsubstitution for current block.
C
C    parameters
C       w, ipivot, nrow, ncol, last  are as on return from factrb.
C       x(1),...,x(ncol)  contains, on input, the right side for the
C               equations in this block after backsubstitution has been
C               carried up to but not including equation (last).
C               means that x(j) contains the right side of equation (j)
C               as modified during elimination, j=1,...,last, while
C               for j .gt. last, x(j) is already a component of the
C               solution vector.
C       x(1),...,x(ncol) contains, on output, the components of the
C               solution corresponding to the present block.
C
C*********************************************************************
C
      INTEGER  LAST,  I,J,K,KM1,LM1,LP1
      DOUBLE PRECISION W(NROW,NCOL),X(NCOL), T
C
      LP1 = LAST + 1
      IF ( LP1 .GT. NCOL )                          GO TO 30
      DO 20 J = LP1, NCOL
         T = - X(J)
         IF ( T .EQ. 0.D0 )                         GO TO 20
         DO 10 I = 1, LAST
   10       X(I) = X(I) + W(I,J) * T
   20 CONTINUE
   30 IF ( LAST .EQ. 1 )                            GO TO 60
      LM1 = LAST - 1
      DO 50 KB = 1, LM1
        KM1 = LAST - KB
        K = KM1 + 1
        X(K) = X(K)/W(K,K)
        T = - X(K)
        IF ( T .EQ. 0.D0 )                          GO TO 50
        DO 40 I = 1, KM1
   40     X(I) = X(I) + W(I,K) * T
   50 CONTINUE
   60 X(1) = X(1)/W(1,1)
      RETURN
      END
