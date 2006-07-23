C  *****************************************************************
      subroutine relax(na,n,startn,endn,c,u,dfct,
     $rc,label,prdcsr,fou,nxtou,fin,nxtin,save,scan,mark,
     $tfstou,tnxtou,tfstin,tnxtin,ddpos,ddneg,x,tcost,flag)
      IMPLICIT INTEGER (A-Z)
      INTEGER C(na),X(na),U(na),RC(na),Dfct(n)
      INTEGER STARTN(na),ENDN(na)
      INTEGER Label(n),Prdcsr(n),Fou(n),Nxtou(na),Fin(n)
      INTEGER Nxtin(na),Save(na)
      integer Scan(n),Mark(n)
      DIMENSION TFSTOU(n),TNXTOU(na),TFSTIN(n),TNXTIN(na)
      DIMENSION DDPOS(na),DDNEG(na)
      DOUBLEPRECISION TCOST
      character*60 tampon
      logical repeat
      flag=1
      LARGE=20000000
      REPEAT=.FALSE.
      CALL INIDAT(STARTN,ENDN,Label,Prdcsr,
     $     Fou,Nxtou,Fin,Nxtin,N,NA)  
C     ***** Set initial dual prices to zero *****
      DO 10 I=1,NA
10      RC(I)=C(I)
      DO 11, I=1,N
 11      DFCT(I)=-DFCT(I)
      CALL RELAXT(startn,endn,u,x,rc,dfct,
     $     label,prdcsr,fou,nxtou,fin,nxtin,save,scan,mark,
     $     N,NA,LARGE,REPEAT,
     $     TFSTOU,TNXTOU,TFSTIN,TNXTIN,DDPOS,DDNEG,flag)
C     ***** Display previous optimal cost *****
      IF (REPEAT)  then 
         write(tampon,1000) TCOST
         call out(tampon)
C         WRITE(6,1000)TCOST
      endif
1000  FORMAT(' ','PREVIOUS OPTIMAL COST=',F14.2)
      TCOST=DFLOAT(0)
      DO 330 I=1,NA
330     TCOST=TCOST+DFLOAT(X(I)*C(I))
      END
C
C  *****************************************************************
      SUBROUTINE INIDAT(STARTN,ENDN,TEMPIN,TEMPOU,
     $     FOU,NXTOU,FIN,NXTIN,N,NA)
C     ***** This subroutine uses the data arrays STARTN and ENDN
C     to construct auxiliary data arrays FOU, NXTOU, FIN, and 
C     NXTIN that are required by RELAX.  In this subroutine we
C     arbitrarily order the arcs leaving each node and store 
C     this information in FOU and NXTOU.  Similarly, we arbitra-
C     rilly order the arcs entering each node and store this
C     information in FIN and NXTIN.  At the completion of the 
C     construction, we have that
C
C         FOU(I)     = First arc leaving node I.
C         NXTOU(J)   = Next arc leaving the head node of arc J.
C         FIN(I)     = First arc entering node I.
C         NXTIN(J)   = Next arc entering the tail node of arc J.
C
      INTEGER STARTN(1),ENDN(1),TEMPIN(1),TEMPOU(1),FOU(1)
      INTEGER NXTOU(1),FIN(1),NXTIN(1)
C     ******* construct data structure required by RELAx *************
      DO 25 I=1,N
        FIN(I)=0
        FOU(I)=0
        TEMPIN(I)=0
25      TEMPOU(I)=0
      DO 27 I=1,NA
        NXTIN(I)=0
        NXTOU(I)=0
        I1=STARTN(I)
        I2=ENDN(I)
        IF (FOU(I1).NE.0) THEN
          NXTOU(TEMPOU(I1))=I
        ELSE
          FOU(I1)=I
        END IF
        TEMPOU(I1)=I
        IF (FIN(I2).NE.0) THEN
          NXTIN(TEMPIN(I2))=I
        ELSE
          FIN(I2)=I
        END IF
27      TEMPIN(I2)=I
      RETURN
      END
C relaxtII.f
C **********************************************************************
C       SUBROUTINE RELAXT 2.1  RELEASED SEPT. 1987
C **********************************************************************
C
C     This subroutine solves the minimum (linear) cost ordinary
C     network flow problem.  
C     The routine implements the relaxation method of 
C     Bertsekas, D. P., "A Unified Framework for Primal-Dual Methods ..."
C              Math. Programming, Vol. 32, 1985, pp. 125-145
C     Bertsekas, D. P., & Tseng, P., "Relaxation Methods for Minimum Cost .."
C              Operations Research J., 1987 (to appear)
C     The routine was written by Dimitri Bertsekas and Paul Tseng with
C     contributions by Jonathan Eckstein.
C
C     This code is in the public domain and can be used for any 
C     purpose. It can be distributed freely.  Users are requested to 
C     acknowledge the authorship of the code, and the relaxation
C     algorithm.  They should also register with the authors to receive
C     updates and subsequent releases.
C
C     No modifications should be made to this code other 
C     than the minimal necessary
C     to make it compatible with the FORTRAN compilers of specific
C     machines. When reporting computational results please be sure
C     to describe the memory limitations of your machine. Generally
C     RELAXT requires more memory than primal simplex codes and may
C     be penalized severely by limited machine memory.
C
C     The difference between this routine and the similar code RELAX-II is
C     that it maintains a data structure that gives all the balanced arcs
C     in the network.  This structure is called the "tree" for historical
C     reasons, even though it describes a subnetwork that will generally
C     be neither acyclic nor connected.  Also, the tree may contain some
C     arcs that are not balanced:  it turns out to be cheaper to purge
C     arcs that have become unbalanced only when their end nodes are being 
C     scanned, as opposed to always maintaining an exact set of balanced arcs.
C
C
C     The user must supply the following inputs to the subroutine:
C     ( See also the documentation.)
C     All data should be in INTEGER*4. To run in limited memory systems 
C      the arrays STARTN, ENDN, NXTIN, NXTOU, SAVE, FIN, FOU, LABEL,
C      PRDCSR may be declared as INTEGER*2. 
C
C          N (the number of nodes)
C          NA (the number of arcs)
C          LARGE (a very large positive integer to represent infinity.
C           All problem data should be less than LARGE in magnitude,
C           and LARGE should be less than, say, 1/4 the largest INTEGER*4
C           of the machine used. This will guard primarily against 
C           overflow in uncapacitated problems where the arc capacities
C           are taken finite but very large.)
C          STARTN(NA) (the head node array)
C          ENDN(NA) (the tail node array)
C          RC(NA) (the reduced cost array)
C          X(NA) (the arc flow array)
C          U(NA) (the arc flow capacity array)
C          DFCT(N) (the deficit array)
C          FOU(N) (the first arc out array)
C          FIN(N) (the first arc in array)
C          NXTOU(NA) (the next arc out array)
C          NXTIN(NA) (the next arc in array)
C     
C     This subroutine places the optimal flow in the array X
C     and the corresponding reduced cost vector in the array RC.
C
C       The code makes no attempt to find a favorable starting flow 
C        and reduced cost vector. If favorable arrays X, U, and RC are 
C       known they can be passed to the routine directly. This requires 
C        that the initialization portion of the routine be skipped.
C     ********************************************************************
      SUBROUTINE RELAXT(startn,endn,u,x,rc,dfct,
     $     label,PRDCSR,FOU,NXTOU,FIN,NXTIN,SAVE,SCAN,MARK,
     $     N,NA,LARGE,REPEAT,
     $     TFSTOU,TNXTOU,TFSTIN,TNXTIN,DDPOS,DDNEG,flag)
      IMPLICIT INTEGER (A-Z)
      character sortie*100
      LOGICAL REPEAT,FEASBL,QUIT,SWITCH,POSIT,PCHANGE
C
C     Each common block contains just one array, so the arrays in RELAXT
C     can be dimensioned to 1 element and take their dimension from the 
C     main calling routine. With this trick RELAXT need not be recompiled
C     if the problem dimension changes. If your FORTRAN does not support
C     this feature change the dimensions below to be the same as the ones
C     declared in your main calling program.
C
      DIMENSION TFSTOU(1),TNXTOU(1),TFSTIN(1),TNXTIN(1)
      DIMENSION STARTN(1),ENDN(1),U(1),X(1),RC(1),DFCT(1)
      DIMENSION LABEL(1),PRDCSR(1),SCAN(1),FOU(1),NXTOU(1)
      DIMENSION FIN(1),NXTIN(1),SAVE(1),MARK(1)
C
C     DDPOS and DDNEG are arrays that give the directional derivatives for
C     all positive and negative single-node price changes.  These are used
C     only in the initial phase of the algorithm, before the "tree" data 
C     structure comes into play.  Therefore, they are equivalenced to
C     TFSTOU and TFSTIN, which are the same size (number of nodes) and are
C     only used after the tree comes into use.
C
      DIMENSION   DDPOS(1),DDNEG(1)
C
C    Reduce arc capacity as much as possible w/out changing the problem 
C    If this is a sensitivity run via routine SENSTV skip the 
C    initialization by setting REPEAT to .TRUE. in the calling program.
C
C     The initialization (from here up to line 70) can be skipped 
C     (by setting REPEAT to .TRUE.) if the calling program places in common
C     user-chosen values for the arc flows, residual arc capacities, nodal  
C     deficits and reduced costs. It is mandatory that arc flows and residual
C     costs satisfy complementary slackess, on each arc, 
C      and that the DFCT  array properly correspond to the initial arc flows
C
      IF (REPEAT) GO TO 75
      DO 40 NODE=1,N
C
C     Note that we also set up the initial DDPOS and DDNEG for each node.
C     (this is not necessary in RELAX)
C
        DDPOS(NODE)=DFCT(NODE)
        DDNEG(NODE)=-DFCT(NODE)

        SCAPOU=0
        ARC=FOU(NODE)
41      IF (ARC.GT.0) THEN
          SCAPOU=MIN0(LARGE,SCAPOU+U(ARC))
          ARC=NXTOU(ARC)
          GO TO 41
        END IF
        CAPOUT=MIN0(LARGE,SCAPOU+DFCT(NODE))
        IF (CAPOUT.LT.0) THEN
C
C  ** PROBLEM IS INFEASIBLE - EXIT
C
       call out('EXIT DURING INITIALIZATION')
       write(sortie,1000)node
 1000  format('EXOGENOUS FLOW INTO NODE',i5,'  EXCEEDS OUT CAPACITY')
       call out(sortie)
       CALL PRINTFLOWS(NODE,
     $     STARTN,ENDN,U,X,DFCT,FOU,NXTOU,FIN,NXTIN)
       GO TO 400
        END IF
C
        SCAPIN=0
        ARC=FIN(NODE)
43      IF (ARC.GT.0) THEN
          U(ARC)=MIN0(U(ARC),CAPOUT)
          SCAPIN=MIN0(LARGE,SCAPIN+U(ARC))
          ARC=NXTIN(ARC)
          GO TO 43
        END IF
44      CAPIN=MIN0(LARGE,SCAPIN-DFCT(NODE))
        IF (CAPIN.LT.0) THEN
C  *** PROBLEM IS INFEASIBLE - EXIT
       call out('EXIT DURING INITIALIZATION')
       write(sortie,1000)node
       call out(sortie)
       CALL PRINTFLOWS(NODE,
     $     STARTN,ENDN,U,X,DFCT,FOU,NXTOU,FIN,NXTIN)
       GO TO 400
       END IF
C
        ARC=FOU(NODE)
45      IF (ARC.GT.0) THEN
          U(ARC)=MIN0(U(ARC),CAPIN)
          ARC=NXTOU(ARC)
          GO TO 45
        END IF
40      CONTINUE
C
C     ******* initialize the arc flows and the nodal deficits ********
C     *** note that U(ARC) is redefined as the residual capacity of ARC ***
C
C     Now compute the directional derivatives for each coordinate exactly.
C     As well as computing the actual defecits.  U(ARC) is the residual
C     capacity on ARC, and X(ARC) is the flow.  These always add up to the
C     total capacity.
C
      DO 70 ARC=1,NA
         X(ARC) = 0
         IF (RC(ARC) .LE. 0) THEN
            T  = U(ARC)
            T1 = STARTN(ARC)
            T2 = ENDN(ARC)
            DDPOS(T1) = DDPOS(T1) + T
            DDNEG(T2) = DDNEG(T2) + T
            IF (RC(ARC) .LT. 0) THEN
               X(ARC) = T
               U(ARC) = 0                       
               DFCT(T1) = DFCT(T1) + T          
               DFCT(T2) = DFCT(T2) - T
               DDNEG(T1) = DDNEG(T1) - T
               DDPOS(T2) = DDPOS(T2) - T
            END IF
         END IF
70    CONTINUE
C
C     Adaptive strategy:  the number of strictly single-node iteration
C     passes attempted is a function of the average density of the network.
C
      IF (NA.GT.N*10) THEN
        NUMPASSES=2
      ELSE
        NUMPASSES=3
      END IF
C
C     We now do 2 or 3 passes through all the nodes.  This is the initial
C     phase:  if a single node iteration is not possible, we just go on to
C     the next node.
C
      DO 390 PASSES = 1,NUMPASSES

      DO 300 NODE=1,N

       IF (DFCT(NODE) .NE. 0) THEN

C       Price rise or price drop?  (Note: it is impossible to have both.)

        IF (DDPOS(NODE) .LE. 0) THEN

C     Price rise.  Loop over breakpoints in +Price(NODE) direction.
C     On outgoing arcs, tension will rise and reduced cost will fall
C        -- so, next break comes at smallest positive reduced cost.
C     On incoming arcs, tension will fall and reduced cost will rise
C        -- so, next break comes at smallest negative reduced cost.

           DELPRC = LARGE

           ARC = FOU(NODE)              
303        IF (ARC .GT. 0) THEN         
              TRC = RC(ARC)
              IF ((TRC .GT. 0) .AND. (TRC .LT. DELPRC)) THEN
                 DELPRC = TRC
              END IF
              ARC = NXTOU(ARC)
              GOTO 303
           END IF

           ARC = FIN(NODE)               
304        IF (ARC .GT. 0) THEN
              TRC = RC(ARC)
              IF ((TRC .LT. 0) .AND. (-TRC .LT. DELPRC)) THEN
                 DELPRC = -TRC
              END IF
              ARC = NXTIN(ARC)
              GOTO 304
           END IF

C          If no breakpoints left and ascent still possible, the problem 
C          is infeasible.

           IF (DELPRC .GE. LARGE) THEN
              IF (DDPOS(NODE) .EQ. 0) GOTO 300
              GOTO 400
           ENDIF

C          We have an actual breakpoint.  Increase price by that quantity.
C          First check the effect on all outbound arcs, which will have a
C          tension increase and reduced cost drop.

350        NXTBRK = LARGE
           ARC = FOU(NODE)
305        IF (ARC .GT. 0) THEN         
              TRC = RC(ARC)
              IF (TRC .EQ. 0) THEN      
                 T1 = ENDN(ARC)         
                 T  = U(ARC)
                 IF (T .GT. 0) THEN
                    DFCT(NODE) = DFCT(NODE) + T
                    DFCT(T1) = DFCT(T1) - T
                    X(ARC) = T
                    U(ARC) = 0
                 ELSE
                    T = X(ARC)
                 END IF
                 DDNEG(NODE) = DDNEG(NODE) - T      
                 DDPOS(T1) = DDPOS(T1) - T          
              END IF
C             For all outgoing arcs tension rises, and reduced cost drops.
              TRC = TRC - DELPRC
              IF ((TRC .GT. 0) .AND. (TRC .LT. NXTBRK)) THEN
                 NXTBRK = TRC
              ELSE IF (TRC .EQ. 0) THEN
C                Arc goes from inactive to balanced.  Just change tension 
C                increase derivatives, and check for status change at other end.
                 DDPOS(NODE) = DDPOS(NODE) + U(ARC)
                 DDNEG(ENDN(ARC)) = DDNEG(ENDN(ARC)) + U(ARC)
              END IF
              RC(ARC) = TRC
              ARC =  NXTOU(ARC)
              GOTO 305
           END IF

C          Time to check the incoming arcs into the node.               
C          These arcs will have an tension decrease and a reduced cost rise.

           ARC = FIN(NODE)
306        IF (ARC .GT. 0) THEN
              TRC = RC(ARC)
              IF (TRC .EQ. 0) THEN        
                 T1 = STARTN(ARC)         
                 T  = X(ARC)
                 IF (T .GT. 0) THEN
                    DFCT(NODE) = DFCT(NODE) + T
                    DFCT(T1) = DFCT(T1) - T
                    U(ARC) = T
                    X(ARC) = 0
                 ELSE
                    T = U(ARC)
                 END IF
                 DDPOS(T1) = DDPOS(T1) - T     
                 DDNEG(NODE) = DDNEG(NODE) - T 
              END IF
C             Note the reduced cost rise for every arc.
              TRC = TRC + DELPRC
              IF ((TRC .LT. 0) .AND. (-TRC .LT. NXTBRK)) THEN
                 NXTBRK = -TRC
              ELSE IF (TRC .EQ. 0) THEN
C                Now check for movement from active to balanced.
C                If so, tension decrease derivatives increase.
                 DDNEG(STARTN(ARC)) = DDNEG(STARTN(ARC)) + X(ARC)
                 DDPOS(NODE) = DDPOS(NODE) + X(ARC)
              END IF
              RC(ARC) = TRC
              ARC = NXTIN(ARC)
              GOTO 306
           END IF

C          We are now done with the iteration.  If the current direction
C          is still a (degenerate) ascent direction, push onward.

           IF ((DDPOS(NODE) .LE. 0) .AND. (NXTBRK .LT. LARGE)) THEN
              DELPRC = NXTBRK
              GOTO 350
           END IF


C     Now comes the code for a price decrease at NODE.
C     On outgoing arcs, tension will drop and reduced cost will increase
C        -- so, next break comes at smallest negative reduced cost.
C     On incoming arcs, tension will increase and reduced cost will fall
C        -- so, next break comes at smallest positive reduced cost.

      ELSE IF (DDNEG(NODE) .LE. 0) THEN

         DELPRC = LARGE                  

         ARC = FOU(NODE)                 
307      IF (ARC .GT. 0) THEN
            TRC = RC(ARC)
            IF ((TRC .LT. 0) .AND. (-TRC .LT. DELPRC)) THEN
               DELPRC = -TRC
            ENDIF
            ARC = NXTOU(ARC)
            GOTO 307
         ENDIF

         ARC = FIN(NODE)                 
308      IF (ARC .GT. 0) THEN
            TRC = RC(ARC)
            IF ((TRC .GT. 0) .AND. (TRC .LT. DELPRC)) THEN
               DELPRC = TRC
            END IF
            ARC = NXTIN(ARC)
            GOTO 308
         END IF

C        If there is no breakpoint, the problem is infeasible,
C        unless we are making a degenerate step.

         IF (DELPRC .EQ. LARGE) THEN
            IF (DDNEG(NODE) .EQ. 0) GOTO 300
            GOTO 400
         END IF
C        Now we make the step to the next breakpoint.  We start with the
C        outbound arcs.  These have a tension decrease and reduced cost
C        rise.  Therefore, the possible transitions are from balanced to
C        inactive or active to balanced.
360      NXTBRK = LARGE
         ARC = FOU(NODE)
309      IF (ARC .GT. 0) THEN
            TRC = RC(ARC)
            IF (TRC .EQ. 0) THEN   
               T1 = ENDN(ARC)
               T  = X(ARC)
               IF (T .GT. 0) THEN
                  DFCT(NODE) = DFCT(NODE) - T   
                  DFCT(T1) = DFCT(T1) + T       
                  U(ARC) = T                    
                  X(ARC) = 0
               ELSE
                  T = U(ARC)
               END IF
               DDPOS(NODE) = DDPOS(NODE) - T
               DDNEG(T1) = DDNEG(T1) - T
            END IF
C           Log the reduced cost rise for all arcs.
            TRC = TRC + DELPRC
            IF ((TRC .LT. 0) .AND. (-TRC .LT. NXTBRK)) THEN
               NXTBRK = -TRC
            ELSE IF (TRC .EQ. 0) THEN
C              Active to balanced.  Tension decrease derivs go up.
               DDNEG(NODE) = DDNEG(NODE) + X(ARC) 
               DDPOS(ENDN(ARC)) = DDPOS(ENDN(ARC)) + X(ARC)
            END IF
            RC(ARC) = TRC
            ARC = NXTOU(ARC)
            GOTO 309
         END IF

C        Now do the incoming arcs.  These have a tension increase and 
C        therefore a reduced cost drop.  The possible transitions are
C        from inactive to balanced and from balanced to active.

         ARC = FIN(NODE)
310      IF (ARC .GT. 0) THEN
            TRC = RC(ARC)
            IF (TRC .EQ. 0) THEN     
               T1 = STARTN(ARC)
               T  = U(ARC)
               IF (T .GT. 0) THEN
                  DFCT(NODE) = DFCT(NODE) - T
                  DFCT(T1) = DFCT(T1) + T
                  X(ARC) = T
                  U(ARC) = 0
               ELSE
                  T = X(ARC)
               END IF
               DDNEG(T1) = DDNEG(T1) - T     
               DDPOS(NODE) = DDPOS(NODE) - T
            END IF
            TRC = TRC - DELPRC
            IF ((TRC .GT. 0) .AND. (TRC .LT. NXTBRK)) THEN
               NXTBRK = TRC
            ELSE IF (TRC .EQ. 0) THEN
               DDPOS(STARTN(ARC)) = DDPOS(STARTN(ARC)) + U(ARC)
               DDNEG(NODE) = DDNEG(NODE) + U(ARC)
            END IF
            RC(ARC) = TRC
            ARC = NXTIN(ARC)
            GOTO 310
         END IF

C        OK.  Movement is done.  Is this direction still a (degenerate)
C        ascent direction.  If so, keep going.

         IF ((DDNEG(NODE) .LE. 0) .AND. (NXTBRK .LT. LARGE)) THEN
            DELPRC = NXTBRK
            GOTO 360
         END IF
       END IF
      END IF
300   CONTINUE
390   CONTINUE
C     ******* initialize the tree ************************************
      DO 50 I=1,N
        TFSTOU(I)=0
50      TFSTIN(I)=0
      DO 51 I=1,NA
        TNXTIN(I)=-1
        TNXTOU(I)=-1
        IF (RC(I).EQ.0) THEN
          TNXTOU(I)=TFSTOU(STARTN(I))
          TFSTOU(STARTN(I))=I
          TNXTIN(I)=TFSTIN(ENDN(I))
          TFSTIN(ENDN(I))=I
        END IF
51    CONTINUE
C
C     *********** Initialize other variables ***********
75    FEASBL=.TRUE.
      NDFCT=N
      NUMNONZERO=0
      SWITCH=.FALSE.
      DO 76 I=1,N
        MARK(I)=0
        SCAN(I)=0
76    CONTINUE
      NLABEL=0
C     ******* Set threshold for SWITCH *******************************
C     RELAX uses an adaptive strategy for deciding whether to
C     continue the scanning process after a price change.
C     The threshold parameters tp and ts that control
C     this strategy are set in the next few lines.
      TP=10
      TS=INT(N/15)
C     **** start relaxation algorithm **************
80    CONTINUE
        DO 100 NODE=1,N
          DEFCIT=DFCT(NODE)
          IF (DEFCIT.EQ.0) THEN
            GO TO 100
          ELSE
            POSIT = (DEFCIT .GT. 0)
            NUMNONZERO=NUMNONZERO+1
          END IF
C         ***** ATTEMPT A SINGLE NODE ITERATION FROM NODE ****
      IF (POSIT) THEN
C    ************* CASE OF NODE W/ POSITIVE DEFICIT ********
      PCHANGE = .FALSE.
      INDEF=DEFCIT
      DELX=0
      NB=0
C     Check outgoing (probably) balanced arcs from NODE.
      ARC=TFSTOU(NODE)
500   IF (ARC .GT. 0) THEN
         IF ((RC(ARC) .EQ. 0) .AND. (X(ARC) .GT. 0)) THEN
            DELX = DELX + X(ARC)
            NB = NB + 1
            SAVE(NB) = ARC
         ENDIF
         ARC = TNXTOU(ARC)
         GOTO 500
      END IF
C     Check incoming arcs now.
      ARC = TFSTIN(NODE)
501   IF (ARC .GT. 0) THEN
         IF ((RC(ARC) .EQ. 0) .AND. (U(ARC) .GT. 0)) THEN
            DELX = DELX + U(ARC)
            NB = NB + 1
            SAVE(NB) = -ARC
         ENDIF
         ARC = TNXTIN(ARC)
         GOTO 501
      END IF
C     ***** end of initial node scan *******
18     CONTINUE
C     ********* IF no price change is possible exit **********
      IF (DELX.GT.DEFCIT) THEN
        QUIT = (DEFCIT .LT. INDEF)
        GO TO 16
      END IF
C     Now compute distance to next breakpoint.
      DELPRC = LARGE
      ARC = FOU(NODE)
502   IF (ARC .GT. 0) THEN
         RDCOST = RC(ARC)
         IF ((RDCOST .LT. 0) .AND. (-RDCOST .LT. DELPRC)) THEN
            DELPRC = -RDCOST
         ENDIF
         ARC = NXTOU(ARC)
         GOTO 502
      END IF
      ARC = FIN(NODE)
503   IF (ARC .GT. 0) THEN
         RDCOST = RC(ARC)
         IF ((RDCOST .GT. 0) .AND. (RDCOST .LT. DELPRC)) THEN
            DELPRC = RDCOST
         ENDIF
         ARC = NXTIN(ARC)
         GOTO 503
      END IF
C     ******* check if the problem is infeasible *******
      IF ((DELX.LT.DEFCIT).AND.(DELPRC.EQ.LARGE)) THEN
C     ***** The dual cost can be decreased without bound  *****
        GO TO 400
      END IF
C     **** SKIP FLOW ADJUSTEMT IF THERE IS NO FLOW TO MODIFY ***
      IF (DELX.EQ.0) GO TO 14
C     *****  Adjust the flow on balanced arcs incident of NODE to
C     maintain complementary slackness after the price change *****
      DO 13 J=1,NB
        ARC=SAVE(J)
        IF (ARC.GT.0) THEN
          NODE2=ENDN(ARC)
          T1=X(ARC)
          DFCT(NODE2)=DFCT(NODE2)+T1
          U(ARC)=U(ARC)+T1
          X(ARC)=0
        ELSE
          NARC=-ARC
          NODE2=STARTN(NARC)
          T1=U(NARC)
          DFCT(NODE2)=DFCT(NODE2)+T1
          X(NARC)=X(NARC)+T1
          U(NARC)=0
        END IF
13     CONTINUE
      DEFCIT=DEFCIT-DELX
      IF (DELPRC.EQ.LARGE) THEN
        QUIT=.TRUE.
        GO TO 19
      END IF
C     ***** NODE corresponds to a dual ascent direction.  Decrease
C     the price of NODE by DELPRC and compute the stepsize to the
C     next breakpoint in the dual cost *****
14    NB=0
      PCHANGE = .TRUE.
      DP=DELPRC
      DELPRC=LARGE
      DELX=0
      ARC=FOU(NODE)
504   IF (ARC.GT.0) THEN
        RDCOST=RC(ARC)+DP
        RC(ARC)=RDCOST
        IF (RDCOST.EQ.0) THEN
          NB=NB+1
          SAVE(NB)=ARC
          DELX=DELX+X(ARC)
        END IF
        IF ((RDCOST.LT.0).AND.(-RDCOST.LT.DELPRC)) DELPRC=-RDCOST
        ARC=NXTOU(ARC)
        GOTO 504
      END IF
      ARC=FIN(NODE)
505   IF (ARC.GT.0) THEN
        RDCOST=RC(ARC)-DP
        RC(ARC)=RDCOST
        IF (RDCOST.EQ.0) THEN
          NB=NB+1
          SAVE(NB)=-ARC
          DELX=DELX+U(ARC)
        END IF
        IF ((RDCOST.GT.0).AND.(RDCOST.LT.DELPRC)) DELPRC=RDCOST
        ARC=NXTIN(ARC)
        GOTO 505
      END IF
C     ***** return to check if another price change is possible ******
      GO TO 18
C     ******* perform flow augmentation at NODE ****
16    DO 11 J=1,NB
        ARC=SAVE(J)
        IF (ARC.GT.0) THEN
C         *** ARC is an outgoing arc from NODE ********************
          NODE2=ENDN(ARC)
          T1=DFCT(NODE2)
          IF (T1.LT.0) THEN
C           ***** Decrease the total deficit by decreasing flow of ARC  *****
            QUIT=.TRUE.
            T2=X(ARC)
            DX=MIN0(DEFCIT,-T1,T2)
            DEFCIT=DEFCIT-DX
            DFCT(NODE2)=T1+DX
            X(ARC)=T2-DX
            U(ARC)=U(ARC)+DX
            IF (DEFCIT.EQ.0) GO TO 19
          END IF
        ELSE
C         *** -ARC is an incoming arc to NODE *********************
          NARC=-ARC
          NODE2=STARTN(NARC)
          T1=DFCT(NODE2)
          IF (T1.LT.0) THEN
C           ***** Decrease the total deficit by increasing flow of -ARC  *****
            QUIT=.TRUE.
            T2=U(NARC)
            DX=MIN0(DEFCIT,-T1,T2)
            DEFCIT=DEFCIT-DX
            DFCT(NODE2)=T1+DX
            X(NARC)=X(NARC)+DX
            U(NARC)=T2-DX
            IF (DEFCIT.EQ.0) GO TO 19
          END IF
        END IF
11     CONTINUE
19     DFCT(NODE)=DEFCIT
C
C      Reconstruct the list of balanced arcs adjacent to this node.
C      First, the list at this node is now totally different.  Eat 
C      the old lists of incoming and outgoing balanced arcs, and create
C      a whole new one.  This way we get the in and out lists of balanced 
C      arcs for NODE to be exactly correct.  For the adjacent nodes, we
C      add in all the newly balanced arcs, but do not bother getting rid
C      of formerly balanced ones (they will be purged the next time the
C      adjacent node is scanned).
C
       IF (PCHANGE) THEN

          ARC = TFSTOU(NODE)
          TFSTOU(NODE) = 0
506       IF (ARC .GT. 0) THEN
             NXTARC = TNXTOU(ARC)
             TNXTOU(ARC) = -1
             ARC = NXTARC
             GOTO 506
          END IF
          ARC = TFSTIN(NODE)
          TFSTIN(NODE) = 0
507       IF (ARC .GT. 0) THEN
             NXTARC = TNXTIN(ARC)
             TNXTIN(ARC) = -1
             ARC = NXTARC
             GOTO 507
          END IF
C         *** Now add the currently balanced arcs to the list for this node ***
C         *** (which is now empty), and the appropriate adjacent ones.      ***
          DO 508 J=1,NB
             ARC = SAVE(J)
             IF (ARC.LE.0) ARC=-ARC
             IF (TNXTOU(ARC) .LT. 0) THEN
                TNXTOU(ARC) = TFSTOU(STARTN(ARC))
                TFSTOU(STARTN(ARC)) = ARC
             END IF
             IF (TNXTIN(ARC) .LT. 0) THEN
                TNXTIN(ARC) = TFSTIN(ENDN(ARC))
                TFSTIN(ENDN(ARC)) = ARC
             END IF
508       CONTINUE
       END IF
C   *** end of single node iteration for a positive deficit node ***
      ELSE
C     ******* single node iteration for a negative deficit node ******
      PCHANGE = .FALSE.
      DEFCIT=-DEFCIT
      INDEF=DEFCIT
      DELX=0
      NB=0
C
      ARC = TFSTIN(NODE)
509   IF (ARC .GT. 0) THEN
         IF ((RC(ARC) .EQ. 0) .AND. (X(ARC) .GT. 0)) THEN
            DELX = DELX + X(ARC)
            NB = NB + 1
            SAVE(NB) = ARC
         ENDIF
         ARC = TNXTIN(ARC)
         GOTO 509
      END IF         
      ARC=TFSTOU(NODE)
510   IF (ARC .GT. 0) THEN
         IF ((RC(ARC) .EQ. 0) .AND. (U(ARC) .GT. 0)) THEN
            DELX = DELX + U(ARC)
            NB = NB + 1
            SAVE(NB) = -ARC
         ENDIF
         ARC = TNXTOU(ARC)
         GOTO 510
      END IF
C
28     CONTINUE
      IF (DELX.GE.DEFCIT) THEN
        QUIT = (DEFCIT .LT. INDEF)
        GO TO 26
      END IF
C     Now compute distance to next breakpoint.
      DELPRC = LARGE
      ARC = FIN(NODE)
511   IF (ARC .GT. 0) THEN
         RDCOST = RC(ARC)
         IF ((RDCOST .LT. 0) .AND. (-RDCOST .LT. DELPRC)) THEN
            DELPRC = -RDCOST
         ENDIF
         ARC = NXTIN(ARC)
         GOTO 511
      END IF
      ARC = FOU(NODE)
512   IF (ARC .GT. 0) THEN
         RDCOST = RC(ARC)
         IF ((RDCOST .GT. 0) .AND. (RDCOST .LT. DELPRC)) THEN
            DELPRC = RDCOST
         ENDIF
         ARC = NXTOU(ARC)
         GOTO 512
      END IF
C     ******* check if problem is infeasible ************************
      IF ((DELX.LT.DEFCIT).AND.(DELPRC.EQ.LARGE)) THEN
        GO TO 400
      END IF
      IF (DELX.EQ.0) GO TO 24
C     ******* flow augmentation is possible *************************
      DO 23 J=1,NB
        ARC=SAVE(J)
        IF (ARC.GT.0) THEN
          NODE2=STARTN(ARC)
          T1=X(ARC)
          DFCT(NODE2)=DFCT(NODE2)-T1
          U(ARC)=U(ARC)+T1
          X(ARC)=0
        ELSE
          NARC=-ARC
          NODE2=ENDN(NARC)
          T1=U(NARC)
          DFCT(NODE2)=DFCT(NODE2)-T1
          X(NARC)=X(NARC)+T1
          U(NARC)=0
        END IF
23     CONTINUE
      DEFCIT=DEFCIT-DELX
      IF (DELPRC.EQ.LARGE) THEN
        QUIT=.TRUE.
        GO TO 29
      END IF
C     ******* price increase at NODE is possible ********************
24    NB=0
      PCHANGE = .TRUE.
      DP=DELPRC
      DELPRC=LARGE
      DELX=0
      ARC=FIN(NODE)
513   IF (ARC.GT.0) THEN
        RDCOST=RC(ARC)+DP
        RC(ARC)=RDCOST
        IF (RDCOST.EQ.0) THEN
          NB=NB+1
          SAVE(NB)=ARC
          DELX=DELX+X(ARC)
        END IF
        IF ((RDCOST.LT.0).AND.(-RDCOST.LT.DELPRC)) DELPRC=-RDCOST
        ARC=NXTIN(ARC)
        GOTO 513
      END IF
      ARC=FOU(NODE)
514   IF (ARC.GT.0) THEN
        RDCOST=RC(ARC)-DP
        RC(ARC)=RDCOST
        IF (RDCOST.EQ.0) THEN
          NB=NB+1
          SAVE(NB)=-ARC
          DELX=DELX+U(ARC)
        END IF
        IF ((RDCOST.GT.0).AND.(RDCOST.LT.DELPRC)) DELPRC=RDCOST
        ARC=NXTOU(ARC)
        GOTO 514
      END IF
      GO TO 28
C     ******* perform flow augmentation at NODE ****
26    DO 21 J=1,NB
        ARC=SAVE(J)
        IF (ARC.GT.0) THEN
C         *** ARC is an incoming arc to NODE ********************
          NODE2=STARTN(ARC)
          T1=DFCT(NODE2)
          IF (T1.GT.0) THEN
            QUIT=.TRUE.
            T2=X(ARC)
            DX=MIN0(DEFCIT,T1,T2)
            DEFCIT=DEFCIT-DX
            DFCT(NODE2)=T1-DX
            X(ARC)=T2-DX
            U(ARC)=U(ARC)+DX
            IF (DEFCIT.EQ.0) GO TO 29
          END IF
        ELSE
C         *** -ARC is an outgoing arc from NODE *********************
          NARC=-ARC
          NODE2=ENDN(NARC)
          T1=DFCT(NODE2)
          IF (T1.GT.0) THEN
            QUIT=.TRUE.
            T2=U(NARC)
            DX=MIN0(DEFCIT,T1,T2)
            DEFCIT=DEFCIT-DX
            DFCT(NODE2)=T1-DX
            X(NARC)=X(NARC)+DX
            U(NARC)=T2-DX
            IF (DEFCIT.EQ.0) GO TO 29
          END IF
        END IF
21     CONTINUE
29     DFCT(NODE)=-DEFCIT
C      Reconstruct the list of balanced arcs adjacent to this node.
C      First, the list at this node is now totally different.  Eat 
C      the old lists of incoming and outgoing balanced arcs.       
       IF (PCHANGE) THEN
          ARC = TFSTOU(NODE)
          TFSTOU(NODE) = 0
515       IF (ARC .GT. 0) THEN
             NXTARC = TNXTOU(ARC)
             TNXTOU(ARC) = -1
             ARC = NXTARC
             GOTO 515
          END IF
          ARC = TFSTIN(NODE)
          TFSTIN(NODE) = 0
516       IF (ARC .GT. 0) THEN
             NXTARC = TNXTIN(ARC)
             TNXTIN(ARC) = -1
             ARC = NXTARC
             GOTO 516
          END IF
C
C         *** Now add the currently balanced arcs to the list for this node ***
C         *** (which is now empty), and the appropriate adjacent ones.      ***
C
          DO 517 J=1,NB
             ARC = SAVE(J)
             IF (ARC.LE.0) ARC=-ARC
             IF (TNXTOU(ARC) .LT. 0) THEN
                TNXTOU(ARC) = TFSTOU(STARTN(ARC))
                TFSTOU(STARTN(ARC)) = ARC
             END IF
             IF (TNXTIN(ARC) .LT. 0) THEN
                TNXTIN(ARC) = TFSTIN(ENDN(ARC))
                TFSTIN(ENDN(ARC)) = ARC
             END IF
517       CONTINUE

       END IF
C    ***** end of single node iteration for a negative deficit node ***
       END IF
       IF (QUIT) GO TO 100
C      ******* do a multi-node operation from NODE *****************
       SWITCH = (NDFCT .LT. TP)
C      ******* UNMARK NODES LABELED EARLIER *******
          DO 90 J=1,NLABEL
            NODE2=LABEL(J)
            MARK(NODE2)=0
            SCAN(NODE2)=0
90        CONTINUE
C      ******* INITIALIZE LABELING ******
          NLABEL=1
          LABEL(1)=NODE
          MARK(NODE)=1
          PRDCSR(NODE)=0
C      ******** SCAN STARTING NODE *********
          SCAN(NODE)=1
          NSCAN=1
          DM=DFCT(NODE)
          DO 95 J=1,NB
           ARC=SAVE(J)
           IF (ARC.GT.0) THEN
            IF (POSIT) THEN
              NODE2=ENDN(ARC)
            ELSE
              NODE2=STARTN(ARC)
            END IF
            IF (MARK(NODE2).eq.0) THEN
               NLABEL=NLABEL+1
               LABEL(NLABEL)=NODE2
               PRDCSR(NODE2)=ARC
               MARK(NODE2)=1
            END IF
           ELSE
            NARC=-ARC
            IF (POSIT) THEN
              NODE2=STARTN(NARC)
            ELSE
              NODE2=ENDN(NARC)
            END IF
            IF (MARK(NODE2).eq.0) THEN
              NLABEL=NLABEL+1
              LABEL(NLABEL)=NODE2
              PRDCSR(NODE2)=ARC
              MARK(NODE2)=1
            END IF
           END IF
95       CONTINUE
C
C        **** start scanning labeled nodes ****
C
120      NSCAN=NSCAN+1
C
C        ****** check to see if SWITCH needs to be set ******
C        SWITCH indicates it may now be best to change over to a more
C        conventional primal-dual algorithm (one which can reuse old
C        labels to some extent).
C
         SWITCH = SWITCH .OR. ( (NSCAN .GT. TS) .AND. (NDFCT .LT. TS) )
C
C   **** scan next node on the list of labeled nodes ****
C   *** scanning will continue until either an OVERESTIMATE of the residual 
C   capacity across the cut corresponding to the scanned set of nodes (called
C   DELX) exceeds the absolute value of the total deficit of the scanned
C   nodes (called DM), or else an augmenting path is found.  Arcs that are
C   in the tree but are not balanced are purged as part of the scanning 
C   process.

      I=LABEL(NSCAN)
      SCAN(I)=1
      IF (POSIT) THEN
C                   
C     ******* scanning node I for case of positive deficit ******
C
      NAUGNOD=0
      PRVARC=0
      ARC = TFSTOU(I)

518   IF (ARC.GT.0) THEN
C
C       ***** ARC is an outgoing arc from NODE *****
C
        IF (RC(ARC) .EQ. 0) THEN
           IF (X(ARC) .GT. 0) THEN
              NODE2=ENDN(ARC)
              IF (MARK(NODE2).eq.0) THEN
C
C                ***** NODE2 is not in the labeled set.  Add NODE2 to the
C                labeled set. *****
C
                 PRDCSR(NODE2)=ARC
                 IF (DFCT(NODE2).LT.0) THEN
                    NAUGNOD=NAUGNOD+1
                    SAVE(NAUGNOD)=NODE2
                 END IF
                 NLABEL=NLABEL+1
                 LABEL(NLABEL)=NODE2
                 MARK(NODE2)=1
                 DELX=DELX+X(ARC)
              END IF                 
           END IF                    
           PRVARC = ARC              
           ARC = TNXTOU(ARC)
        ELSE                         
           TMPARC = ARC
           ARC = TNXTOU(ARC)         
           TNXTOU(TMPARC) = -1       
           IF (PRVARC .EQ. 0) THEN
              TFSTOU(I) = ARC        
           ELSE
              TNXTOU(PRVARC) = ARC   
           END IF                    
        END IF                       
        GOTO 518
      END IF
C
C
C
      PRVARC = 0
      ARC=TFSTIN(I)
519   IF (ARC.GT.0) THEN
C
C       ***** ARC is an incoming arc into NODE *****
C
        IF (RC(ARC) .EQ. 0) THEN
           IF (U(ARC) .GT. 0) THEN
              NODE2=STARTN(ARC)
              IF (MARK(NODE2).eq.0) THEN
C
C                ***** NODE2 is not in the labeled set.  Add NODE2 to the
C                labeled set. *****
C
                 PRDCSR(NODE2)=-ARC
                 IF (DFCT(NODE2).LT.0) THEN
                    NAUGNOD=NAUGNOD+1
                    SAVE(NAUGNOD)=NODE2
                 END IF
                 NLABEL=NLABEL+1
                 LABEL(NLABEL)=NODE2
                 MARK(NODE2)=1
                 DELX=DELX+U(ARC)
              END IF                      
           END IF                         
           PRVARC = ARC
           ARC = TNXTIN(ARC)
        ELSE
           TMPARC = ARC
           ARC = TNXTIN(ARC)
           TNXTIN(TMPARC) = -1
           IF (PRVARC .EQ. 0) THEN
              TFSTIN(I) = ARC
           ELSE
              TNXTIN(PRVARC) = ARC
           END IF                         
        END IF                            
        GOTO 519
      END IF
C
C   * correct the residual capacity of the scanned nodes cut *
C
      ARC=PRDCSR(I)
      IF (ARC.GT.0) THEN
         DELX=DELX-X(ARC)
      ELSE
         DELX=DELX-U(-ARC)
      END IF
C
C  ********** end of scanning of I for positive deficit case ****
C
              ELSE
C
C     ******* scanning node I for case of negative deficit ****
C
      NAUGNOD=0

      PRVARC = 0
      ARC=TFSTIN(I)
520   IF (ARC.GT.0) THEN
        IF (RC(ARC) .EQ. 0) THEN
           IF (X(ARC) .GT. 0) THEN
              NODE2=STARTN(ARC)
              IF (MARK(NODE2).eq.0) THEN
                 PRDCSR(NODE2)=ARC
                 IF (DFCT(NODE2).GT.0) THEN
                    NAUGNOD=NAUGNOD+1
                    SAVE(NAUGNOD)=NODE2
                 END IF
                 NLABEL=NLABEL+1
                 LABEL(NLABEL)=NODE2
                 MARK(NODE2)=1
                 DELX=DELX+X(ARC)
              END IF                                
           END IF                                   
           PRVARC = ARC
           ARC = TNXTIN(ARC)
        ELSE
           TMPARC = ARC
           ARC = TNXTIN(ARC)
           TNXTIN(TMPARC) = -1
           IF (PRVARC .EQ. 0) THEN
              TFSTIN(I) = ARC
           ELSE
              TNXTIN(PRVARC) = ARC
           END IF                                 
        END IF                                    
        GOTO 520
      END IF
C
C
C
      PRVARC = 0
      ARC = TFSTOU(I)
521   IF (ARC.GT.0) THEN
        IF (RC(ARC) .EQ. 0) THEN
           IF (U(ARC) .GT. 0) THEN
              NODE2=ENDN(ARC)
              IF (MARK(NODE2).eq.0) THEN
                 PRDCSR(NODE2)=-ARC
                 IF (DFCT(NODE2).GT.0) THEN
                    NAUGNOD=NAUGNOD+1
                    SAVE(NAUGNOD)=NODE2
                 END IF
                 NLABEL=NLABEL+1
                 LABEL(NLABEL)=NODE2
                 MARK(NODE2)=1
                 DELX=DELX+U(ARC)
              END IF
           END IF
           PRVARC = ARC
           ARC = TNXTOU(ARC)
        ELSE
           TMPARC = ARC
           ARC = TNXTOU(ARC)
           TNXTOU(TMPARC) = -1
           IF (PRVARC .EQ. 0) THEN
              TFSTOU(I) = ARC
           ELSE
              TNXTOU(PRVARC) = ARC
           END IF
        END IF
        GOTO 521
      END IF
C
      ARC=PRDCSR(I)
      IF (ARC.GT.0) THEN
         DELX=DELX-X(ARC)
      ELSE
         DELX=DELX-U(-ARC)
      END IF
      END IF
C
C   ****** ADD DEFICIT OF NODE SCANNED TO DM ******
C
       DM=DM+DFCT(I)
C
C      *** check if the set of scanned nodes correspond
C      to a dual ascent direction; if yes, perform a
C      price adjustment step, otherwise continue labeling *
C
       IF (NSCAN.LT.NLABEL) THEN
         IF (SWITCH) GO TO 210
         IF ((DELX.GE.DM).AND.(DELX.GE.-DM)) GO TO 210
       END IF
C
C   ************* TRY A PRICE CHANGE ***********
C   Note that since DELX-ABS(DM) is an OVERESTIMATE of ascent slope, we
C   may occasionally try a direction that is not really an ascent.  In
C   this case the ANCNTx routines return with QUIT set to .FALSE. .  The
C   main code, it turn, then tries to label some more node.
C
       IF (POSIT) THEN
          CALL ASCNT1(DM,DELX,NLABEL,AUGNOD,FEASBL,
     $         SWITCH,NSCAN,QUIT,
     $         STARTN,ENDN,U,X,RC,DFCT,LABEL,PRDCSR,FOU,NXTOU,
     $         FIN,NXTIN,SAVE,SCAN,MARK,N,NA,LARGE,
     $         TFSTOU,TNXTOU,TFSTIN,TNXTIN)
       ELSE
          CALL ASCNT2(DM,DELX,NLABEL,AUGNOD,FEASBL,
     $         SWITCH,NSCAN,QUIT,
     $         STARTN,ENDN,U,X,RC,DFCT,LABEL,PRDCSR,FOU,NXTOU,
     $         FIN,NXTIN,SAVE,SCAN,MARK,N,NA,LARGE,
     $         TFSTOU,TNXTOU,TFSTIN,TNXTIN)
       END IF
          IF (.NOT.FEASBL) GO TO 400
          IF ((.NOT.SWITCH).AND.QUIT)  GO TO 100
          IF (((SWITCH).OR.(.NOT.QUIT)).AND.(AUGNOD.GT.0)) THEN
            NAUGNOD=1
            SAVE(1)=AUGNOD
          END IF
C
C         *** CHECK IF AUGMENTATION IS POSSIBLE.
C         IF NOT RETURN TO SCAN ANOTHER NODE. ***
C
210       CONTINUE
C
          IF (NAUGNOD.EQ.0) GO TO 120
C
C         Do the augmentation.
C
          DO 96 J=1,NAUGNOD
          AUGNOD=SAVE(J)
          IF (POSIT) THEN
               CALL AUGFL1(AUGNOD,
     $            STARTN,ENDN,U,X,DFCT,PRDCSR)
            ELSE
               CALL AUGFL2(AUGNOD,
     $              STARTN,ENDN,U,X,DFCT,PRDCSR)
            END IF
96        CONTINUE
C
C        ** RETURN TO TAKE UP ANOTHER NODE W/ NONZERO DEFICIT **
C
100     CONTINUE
        
C        ********** TEST FOR TERMINATION ***********
C
C        We have just done a sweep throught all the nodes.  If they all
C        had zero defecit, we must be done.
C
         NDFCT=NUMNONZERO
         NUMNONZERO=0

         IF (NDFCT.EQ.0) THEN
            RETURN
         ELSE
           GO TO 80
         END IF
C
C     ******* problem is found to be infeasible *********************
400   call out(' PROBLEM IS FOUND TO BE INFEASIBLE.')
      FEASBL = .FALSE.
      flag=0
      RETURN
      END


C  *****************************************************************

      SUBROUTINE PRINTFLOWS(NODE,
     $     STARTN,ENDN,U,X,DFCT,FOU,NXTOU,FIN,NXTIN)
C
C     ***** This subroutine prints the deficit and the flows of 
C     arcs incident to NODE. It is used for diagnostic purposes
C     in case of an infeasible problem here. It can be used also
C     for more general diagnostic purposes. *****
C
C
      IMPLICIT INTEGER (A-Z)
      character sortie*60
C
C
      DIMENSION STARTN(1),ENDN(1),U(1),X(1),DFCT(1)
      DIMENSION FOU(1),NXTOU(1)
      DIMENSION FIN(1),NXTIN(1)
C
C  *****************************************************************
C
      write(sortie,1001)DFCT(NODE)
 1001 format('DEFICIT (I.E., NET FLOW OUT) OF NODE =',i10)
      call out(sortie)
      write(sortie,1002)NODE
 1002 format('FLOWS AND CAPACITIES OF INCIDENT ARCS OF NODE',i5)
      call out(sortie)
      IF (FOU(NODE).EQ.0) THEN
        call out('NO OUTGOING ARCS')
      ELSE
        ARC=FOU(NODE)
5       IF (ARC.GT.0) THEN
           write(sortie,1003)ARC,NODE,ENDN(ARC)
 1003      format('ARC',i5,'  BETWEEN NODES',i5,i5)
           call out(sortie)
           write(sortie,1004)X(ARC)
 1004      format('FLOW =',i10)
           call out(sortie)
           write(sortie,1005)U(ARC)
 1005      format('RESIDUAL CAPACITY =',i10)
           call out(sortie)
          ARC=NXTOU(ARC)
          GO TO 5
        END IF
      END IF
C
      IF (FIN(NODE).EQ.0) THEN
        call out('NO INCOMING ARCS')
      ELSE
        ARC=FIN(NODE)
10       IF (ARC.GT.0) THEN
           write(sortie,1003)ARC,STARTN(ARC),NODE
           call out(sortie)
           write(sortie,1004)X(ARC)
           call out(sortie)
           write(sortie,1005)U(ARC)
           call out(sortie)
          ARC=NXTIN(ARC)
          GO TO 10
        END IF
      END IF
C
      RETURN
      END
C  *****************************************************************
      SUBROUTINE AUGFL1(AUGNOD,
     $    STARTN,ENDN,U,X,DFCT,PRDCSR)
C
C     ***** This subroutine performs the flow augmentation step.
C     A flow augmenting path has been identified in the scanning
C     step and here the flow of all arcs positively (negatively)
C     oriented in the flow augmenting path is decreased (increased)
C     to decrease the total deficit. *****
C
      IMPLICIT INTEGER (A-Z)
      DIMENSION STARTN(1),ENDN(1),U(1),X(1),DFCT(1),PRDCSR(1)
C
C     ***** A flow augmenting path ending at AUGNOD is found.
C     Determine DX, the amount of flow change. *****
C
      DX=-DFCT(AUGNOD)
      IB=AUGNOD
500   IF (PRDCSR(IB).NE.0) THEN
        ARC=PRDCSR(IB)
        IF (ARC.GT.0) THEN
          DX=MIN0(DX,X(ARC))
          IB=STARTN(ARC)
        ELSE
          DX=MIN0(DX,U(-ARC))
          IB=ENDN(-ARC)
        END IF
        GOTO 500
      END IF
      ROOT=IB
      DX=MIN0(DX,DFCT(ROOT))
      IF (DX .LE. 0) RETURN
C
C     ***** Update the flow by decreasing (increasing) the flow of
C     all arcs positively (negatively) oriented in the flow
C     augmenting path.  Adjust the deficits accordingly. *****
C
      DFCT(AUGNOD)=DFCT(AUGNOD)+DX
      DFCT(ROOT)=DFCT(ROOT)-DX
      IB=AUGNOD
501   IF (IB.NE.ROOT) THEN
        ARC=PRDCSR(IB)
        IF (ARC.GT.0) THEN
          X(ARC)=X(ARC)-DX
          U(ARC)=U(ARC)+DX
          IB=STARTN(ARC)
        ELSE
          NARC=-ARC
          X(NARC)=X(NARC)+DX
          U(NARC)=U(NARC)-DX
          IB=ENDN(NARC)
        END IF
        GOTO 501
      END IF
      RETURN
      END



C  *****************************************************************

      SUBROUTINE ASCNT1(DM,DELX,NLABEL,AUGNOD,FEASBL,SWITCH,
     $     NSCAN,QUIT,
     $     STARTN,ENDN,U,X,RC,DFCT,LABEL,PRDCSR,FOU,NXTOU,
     $     FIN,NXTIN,SAVE,SCAN,MARK,N,NA,LARGE,
     $     TFSTOU,TNXTOU,TFSTIN,TNXTIN)

C
C     This subroutine essentially performs the multi-node
C     price adjustment step.  It first checks if the set
C     of scanned nodes correspond to a dual ascent direction.
C     If yes, then decrease the price of all scanned nodes.
C     There are two possibilities for price adjustment:
C     If SWITCH=.TRUE. then the set of scanned nodes 
C     corresponds to an elementary direction of maximal 
C     rate of ascent, in which case the price of all scanned
C     nodes are decreased until the next breakpoint in the
C     dual cost is encountered.  At this point some arc
C     becomes balanced and more node(s) are added to the 
C     labeled set.
C     If SWITCH=.FALSE. then the prices of all scanned nodes
C     are decreased until the rate of ascent becomes
C     negative (this corresponds to the price adjustment
C     step in which both the line search and the degenerate
C     ascent iteration are implemented).
C
      IMPLICIT INTEGER (A-Z)
C
C     The two "tree"-based ascent routines have a common temporary
C     storage area whose dimension is set below.  The maximum conceivable
C     amount needed equals the number of arcs, but this should never 
C     actually occur.
C
C
C MG      LOGICAL*1 SCAN,MARK,SWITCH,FEASBL,QUIT
      LOGICAL SWITCH,FEASBL,QUIT
      COMMON /ASCBLK/B
      DIMENSION TFSTOU(1),TNXTOU(1),TFSTIN(1),TNXTIN(1)
      DIMENSION STARTN(1),ENDN(1),U(1),X(1),RC(1),DFCT(1),LABEL(1)
      DIMENSION PRDCSR(1),FOU(1),NXTOU(1),FIN(1),NXTIN(1)
      DIMENSION SAVE(1),SCAN(1),MARK(1)

C     ***** Store the arcs between the set of scanned nodes and
C     its complement in SAVE and compute DELPRC, the stepsize
C     to the next breakpoint in the dual cost in the direction
C     of decreasing prices of the scanned nodes. *****
C
      DELPRC=LARGE
      DLX=0
      NSAVE=0
C
C   **** calculate the array SAVE of arcs across the cut of scanned
C   nodes in a different way depending on whether NSCAN>N/2 or not.
C   This is done for efficiency. ****
C
      IF (NSCAN.LE.N/2) THEN
      DO 1 I=1,NSCAN
        NODE=LABEL(I)
          ARC=FOU(NODE)
500       IF (ARC.GT.0) THEN
C
C           ***** ARC is an arc pointing from the set of scanned
C           nodes to its complement. *****
C
            NODE2=ENDN(ARC)
            IF (SCAN(NODE2).eq.0) THEN
              NSAVE=NSAVE+1
              SAVE(NSAVE)=ARC
              RDCOST=RC(ARC)
      IF ((RDCOST.EQ.0).AND.(PRDCSR(NODE2).NE.ARC)) DLX=DLX+X(ARC)
            IF ((RDCOST.LT.0).AND.(-RDCOST.LT.DELPRC)) DELPRC=-RDCOST
            END IF
            ARC=NXTOU(ARC)
            GOTO 500
          END IF
          ARC=FIN(NODE)

501       IF (ARC.GT.0) THEN
C
C           ***** ARC is an arc pointing to the set of scanned
C           nodes from its complement. *****
C
            NODE2=STARTN(ARC)
            IF (SCAN(NODE2).eq.0) THEN
              NSAVE=NSAVE+1
              SAVE(NSAVE)=-ARC
              RDCOST=RC(ARC)
      IF ((RDCOST.EQ.0).AND.(PRDCSR(NODE2).NE.-ARC)) DLX=DLX+U(ARC)
              IF ((RDCOST.GT.0).AND.(RDCOST.LT.DELPRC)) DELPRC=RDCOST
            END IF
            ARC=NXTIN(ARC)
            GOTO 501
          END IF
1     CONTINUE
C
      ELSE
C
      DO 2 NODE=1,N
        IF (SCAN(NODE).eq.1) GO TO 2
          ARC=FIN(NODE)
502       IF (ARC.GT.0) THEN
            NODE2=STARTN(ARC)
            IF (SCAN(NODE2).eq.1) THEN
              NSAVE=NSAVE+1
              SAVE(NSAVE)=ARC
              RDCOST=RC(ARC)
      IF ((RDCOST.EQ.0).AND.(PRDCSR(NODE).NE.ARC)) DLX=DLX+X(ARC)
            IF ((RDCOST.LT.0).AND.(-RDCOST.LT.DELPRC)) DELPRC=-RDCOST
            END IF
            ARC=NXTIN(ARC)
            GOTO 502
          END IF
          ARC=FOU(NODE)
503       IF (ARC.GT.0) THEN
            NODE2=ENDN(ARC)
            IF (SCAN(NODE2).eq.1) THEN
              NSAVE=NSAVE+1
              SAVE(NSAVE)=-ARC
              RDCOST=RC(ARC)
      IF ((RDCOST.EQ.0).AND.(PRDCSR(NODE).NE.-ARC)) DLX=DLX+U(ARC)
              IF ((RDCOST.GT.0).AND.(RDCOST.LT.DELPRC)) DELPRC=RDCOST
            END IF
            ARC=NXTOU(ARC)
            GOTO 503
          END IF
2     CONTINUE
      END IF
C
C     ***** Check if the set of scanned nodes truly corresponds
C     to a dual ascent direction.  Here DELX+DLX is the exact
C     sum of the flow on arcs from the scanned set to the 
C     unscanned set plus the ( capacity - flow ) on arcs from
C     the unscanned set to the scanned set. *****
C
      IF (DELX+DLX.GE.DM) THEN
        QUIT=.FALSE.
        AUGNOD=0
        DO 3 I=NSCAN+1,NLABEL
          NODE=LABEL(I)
          IF (DFCT(NODE).LT.0) AUGNOD=NODE
3       CONTINUE
        RETURN
      END IF
      DELX=DELX+DLX
      QUIT=.TRUE.
C
C     ******* check that the problem is feasible ********************
C
4     IF (DELPRC.EQ.LARGE) THEN
C
C       ***** We can decrease the dual cost without bound.
C       Therefore the primal problem is infeasible. *****
C
        FEASBL=.FALSE.
        RETURN
      END IF
C
C     ******* Decrease prices of the scanned nodes, add more 
C     nodes to the labeled set & check if a newly labeled node
C     has negative deficit. *****
C
      IF (SWITCH) THEN
        AUGNOD=0
        DO 7 I=1,NSAVE
          ARC=SAVE(I)
          IF (ARC.GT.0) THEN
            RC(ARC)=RC(ARC)+DELPRC
            IF (RC(ARC).EQ.0) THEN
              NODE2=ENDN(ARC)
              IF (TNXTOU(ARC) .LT. 0) THEN
                TNXTOU(ARC) = TFSTOU(STARTN(ARC))
                TFSTOU(STARTN(ARC)) = ARC
              END IF
              IF (TNXTIN(ARC) .LT. 0) THEN
                TNXTIN(ARC) = TFSTIN(NODE2)
                TFSTIN(NODE2) = ARC
              END IF
              PRDCSR(NODE2)=ARC
              IF (DFCT(NODE2).LT.0) THEN
                AUGNOD=NODE2
              ELSE
                IF (MARK(NODE2).eq.0) THEN
                 MARK(NODE2)=1
                 NLABEL=NLABEL+1
                 LABEL(NLABEL)=NODE2
                END IF
              END IF
            END IF
          ELSE
            ARC=-ARC
            RC(ARC)=RC(ARC)-DELPRC
            IF (RC(ARC).EQ.0) THEN
              NODE2=STARTN(ARC)
              IF (TNXTOU(ARC) .LT. 0) THEN
                TNXTOU(ARC) = TFSTOU(NODE2)
                TFSTOU(NODE2) = ARC
              END IF
              IF (TNXTIN(ARC) .LT. 0) THEN
                TNXTIN(ARC) = TFSTIN(ENDN(ARC))
                TFSTIN(ENDN(ARC)) = ARC
              END IF
              PRDCSR(NODE2)=-ARC
              IF (DFCT(NODE2).LT.0) THEN
                AUGNOD=NODE2
              ELSE
               IF (MARK(NODE2).eq.0) THEN
                MARK(NODE2)=1
                NLABEL=NLABEL+1
                LABEL(NLABEL)=NODE2
               END IF
              END IF
            END IF
          END IF
7       CONTINUE
        RETURN
C
      ELSE
C
C     ***** Decrease the prices of the scanned nodes by DELPRC.
C     Adjust arc flow to maintain complementary slackness with
C     the prices. *****
C
      NB = 0
      DO 6 I=1,NSAVE
        ARC=SAVE(I)
        IF (ARC.GT.0) THEN
            T1=RC(ARC)
            IF (T1.EQ.0) THEN
             T2=X(ARC)
             T3=STARTN(ARC)
             DFCT(T3)=DFCT(T3)-T2
             T3=ENDN(ARC)
             DFCT(T3)=DFCT(T3)+T2
             U(ARC)=U(ARC)+T2
             X(ARC)=0
            END IF
          RC(ARC)=T1+DELPRC
          IF (RC(ARC).EQ.0) THEN
             DELX=DELX+X(ARC)
             NB = NB + 1
             PRDCSR(NB) = ARC
          ENDIF
        ELSE
          ARC=-ARC
          T1=RC(ARC)
          IF (T1.EQ.0) THEN
            T2=U(ARC)
            T3=STARTN(ARC)
            DFCT(T3)=DFCT(T3)+T2
            T3=ENDN(ARC)
            DFCT(T3)=DFCT(T3)-T2
            X(ARC)=X(ARC)+T2
            U(ARC)=0
          END IF
          RC(ARC)=T1-DELPRC
          IF (RC(ARC).EQ.0) THEN
             DELX=DELX+U(ARC)
             NB = NB + 1
             PRDCSR(NB) = ARC
          END IF
        END IF
6     CONTINUE
      END IF
C
      IF (DELX.LE.DM) THEN
C
C       ***** The set of scanned nodes still corresponds to a 
C       dual (possibly degenerate) ascent direction.  Compute
C       the stepsize DELPRC to the next breakpoint in the
C       dual cost. *****
C
        DELPRC=LARGE
        DO 10 I=1,NSAVE
          ARC=SAVE(I)
          IF (ARC.GT.0) THEN
            RDCOST=RC(ARC)
            IF ((RDCOST.LT.0).AND.(-RDCOST.LT.DELPRC)) DELPRC=-RDCOST
          ELSE
            ARC=-ARC
            RDCOST=RC(ARC)
            IF ((RDCOST.GT.0).AND.(RDCOST.LT.DELPRC)) DELPRC=RDCOST
          END IF
10      CONTINUE
        IF ((DELPRC.NE.LARGE).OR.(DELX.LT.DM)) GO TO 4
      END IF
C
C       *** Add new balanced arcs to the superset of balanced arcs.  ***
C
      DO 9 I=1,NB
        ARC=PRDCSR(I)
          IF (TNXTIN(ARC).EQ.-1) THEN
            J=ENDN(ARC)
            TNXTIN(ARC)=TFSTIN(J)
            TFSTIN(J)=ARC
          END IF
          IF (TNXTOU(ARC).EQ.-1) THEN
            J=STARTN(ARC)
            TNXTOU(ARC)=TFSTOU(J)
            TFSTOU(J)=ARC
          END IF
9     CONTINUE

      RETURN
      END


C  *****************************************************************
      
      SUBROUTINE AUGFL2(AUGNOD,
     $    STARTN,ENDN,U,X,DFCT,PRDCSR)
      IMPLICIT INTEGER (A-Z)
      DIMENSION STARTN(1),ENDN(1),U(1),X(1),DFCT(1),PRDCSR(1)
C
C     ******* an augmenting path is found.  determine flow change ***
C
      DX=DFCT(AUGNOD)
      IB=AUGNOD
500   IF (PRDCSR(IB).NE.0) THEN
        ARC=PRDCSR(IB)
        IF (ARC.GT.0) THEN
          DX=MIN0(DX,X(ARC))
          IB=ENDN(ARC)
        ELSE
          DX=MIN0(DX,U(-ARC))
          IB=STARTN(-ARC)
        END IF
        GOTO 500
       END IF
      ROOT=IB
      DX=MIN0(DX,-DFCT(ROOT))
      IF (DX .LE. 0) RETURN
C
C     ******* update the flow and deficits *************************
C
      DFCT(AUGNOD)=DFCT(AUGNOD)-DX
      DFCT(ROOT)=DFCT(ROOT)+DX
      IB=AUGNOD
501   IF (IB.NE.ROOT) THEN
        ARC=PRDCSR(IB)
        IF (ARC.GT.0) THEN
          X(ARC)=X(ARC)-DX
          U(ARC)=U(ARC)+DX
          IB=ENDN(ARC)
        ELSE
          NARC=-ARC
          X(NARC)=X(NARC)+DX
          U(NARC)=U(NARC)-DX
          IB=STARTN(NARC)
        END IF
        GOTO 501
      END IF 
      RETURN
      END
C  *****************************************************************
C
      SUBROUTINE ASCNT2(DM,DELX,NLABEL,AUGNOD,FEASBL,SWITCH,
     $     NSCAN,QUIT,
     $     STARTN,ENDN,U,X,RC,DFCT,LABEL,PRDCSR,FOU,NXTOU,
     $     FIN,NXTIN,SAVE,SCAN,MARK,N,NA,LARGE,
     $     TFSTOU,TNXTOU,TFSTIN,TNXTIN)
      IMPLICIT INTEGER (A-Z)
C
C     The two "tree"-based ascent routines have a common temporary
C     storage area whose dimension is set below.  The maximum conceivable
C     amount needed equals the number of arcs, but this should never 
C     actually occur.
C
C MG      LOGICAL*1 SCAN,MARK,SWITCH,FEASBL,QUIT
      LOGICAL SWITCH,FEASBL,QUIT
      COMMON /ASCBLK/B
      DIMENSION TFSTOU(1),TNXTOU(1),TFSTIN(1),TNXTIN(1)
      DIMENSION STARTN(1),ENDN(1),U(1),X(1),RC(1),DFCT(1),LABEL(1)
      DIMENSION PRDCSR(1),FOU(1),NXTOU(1),FIN(1),NXTIN(1)
      DIMENSION SAVE(1),SCAN(1),MARK(1)
C
C     ******* augment flows across the cut & compute price rise *****
C
      DELPRC=LARGE
      DLX=0
      NSAVE=0
      IF (NSCAN.LE.N/2) THEN
      DO 1 I=1,NSCAN
        NODE=LABEL(I)
          ARC=FIN(NODE)
500       IF (ARC.GT.0) THEN
            NODE2=STARTN(ARC)
            IF (SCAN(NODE2).eq.0) THEN
              NSAVE=NSAVE+1
              SAVE(NSAVE)=ARC
              RDCOST=RC(ARC)
      IF ((RDCOST.EQ.0).AND.(PRDCSR(NODE2).NE.ARC)) DLX=DLX+X(ARC)
            IF ((RDCOST.LT.0).AND.(-RDCOST.LT.DELPRC)) DELPRC=-RDCOST
            END IF
            ARC=NXTIN(ARC)
            GOTO 500
          END IF
          ARC=FOU(NODE)
501       IF (ARC.GT.0) THEN
            NODE2=ENDN(ARC)
            IF (SCAN(NODE2).eq.0) THEN
              NSAVE=NSAVE+1
              SAVE(NSAVE)=-ARC
              RDCOST=RC(ARC)
        IF ((RDCOST.EQ.0).AND.(PRDCSR(NODE2).NE.-ARC)) DLX=DLX+U(ARC)
              IF ((RDCOST.GT.0).AND.(RDCOST.LT.DELPRC)) DELPRC=RDCOST
            END IF
            ARC=NXTOU(ARC)
            GOTO 501
          END IF
1     CONTINUE
      ELSE
      DO 2 NODE=1,N
        IF (SCAN(NODE).eq.1) GO TO 2
          ARC=FOU(NODE)
502       IF (ARC.GT.0) THEN
            NODE2=ENDN(ARC)
            IF (SCAN(NODE2).eq.1) THEN
              NSAVE=NSAVE+1
              SAVE(NSAVE)=ARC
              RDCOST=RC(ARC)
      IF ((RDCOST.EQ.0).AND.(PRDCSR(NODE).NE.ARC)) DLX=DLX+X(ARC)
            IF ((RDCOST.LT.0).AND.(-RDCOST.LT.DELPRC)) DELPRC=-RDCOST
            END IF
            ARC=NXTOU(ARC)
            GOTO 502
          END IF
          ARC=FIN(NODE)
503       IF (ARC.GT.0) THEN
            NODE2=STARTN(ARC)
            IF (SCAN(NODE2).eq.1) THEN
              NSAVE=NSAVE+1
              SAVE(NSAVE)=-ARC
              RDCOST=RC(ARC)
        IF ((RDCOST.EQ.0).AND.(PRDCSR(NODE).NE.-ARC)) DLX=DLX+U(ARC)
              IF ((RDCOST.GT.0).AND.(RDCOST.LT.DELPRC)) DELPRC=RDCOST
            END IF
            ARC=NXTIN(ARC)
            GOTO 503
          END IF
2     CONTINUE
      END IF
      IF (DELX+DLX.GE.-DM) THEN
        QUIT=.FALSE.
        AUGNOD=0
        DO 3 I=NSCAN+1,NLABEL
          NODE=LABEL(I)
          IF (DFCT(NODE).GT.0) AUGNOD=NODE
3       CONTINUE
        RETURN
      END IF
      DELX=DELX+DLX
      QUIT=.TRUE.
C
C     ******* check that the problem is feasible ********************
C
4     IF (DELPRC.EQ.LARGE) THEN
        FEASBL=.FALSE.
        RETURN
      END IF
C
C      ***** INCREASE PRICES *****
C
      IF (SWITCH) THEN
        AUGNOD=0
        DO 7 I=1,NSAVE
          ARC=SAVE(I)
          IF (ARC.GT.0) THEN
            RC(ARC)=RC(ARC)+DELPRC
            IF (RC(ARC).EQ.0) THEN
              NODE2=STARTN(ARC)
              IF (TNXTOU(ARC) .LT. 0) THEN
                TNXTOU(ARC) = TFSTOU(NODE2)
                TFSTOU(NODE2) = ARC
              END IF
              IF (TNXTIN(ARC) .LT. 0) THEN
                TNXTIN(ARC) = TFSTIN(ENDN(ARC))
                TFSTIN(ENDN(ARC)) = ARC
              END IF
              PRDCSR(NODE2)=ARC
              IF (DFCT(NODE2).GT.0) THEN
                AUGNOD=NODE2
              ELSE
               IF (MARK(NODE2).eq.0) THEN
                 MARK(NODE2)=1
                NLABEL=NLABEL+1
                LABEL(NLABEL)=NODE2
               END IF
              END IF
            END IF
          ELSE
            ARC=-ARC
            RC(ARC)=RC(ARC)-DELPRC
            IF (RC(ARC).EQ.0) THEN
              NODE2=ENDN(ARC)
              IF (TNXTOU(ARC) .LT. 0) THEN
                TNXTOU(ARC) = TFSTOU(STARTN(ARC))
                TFSTOU(STARTN(ARC)) = ARC
              END IF
              IF (TNXTIN(ARC) .LT. 0) THEN
                TNXTIN(ARC) = TFSTIN(NODE2)
                TFSTIN(NODE2) = ARC
              END IF
              PRDCSR(NODE2)=-ARC
              IF (DFCT(NODE2).GT.0) THEN
                AUGNOD=NODE2
              ELSE
               IF (MARK(NODE2).eq.0) THEN
                MARK(NODE2)=1
                NLABEL=NLABEL+1
                LABEL(NLABEL)=NODE2
               END IF
              END IF
            END IF
          END IF
7       CONTINUE
        RETURN
C
      ELSE
C
      NB = 0
      DO 6 I=1,NSAVE
        ARC=SAVE(I)
        IF (ARC.GT.0) THEN
            T1=RC(ARC)
            IF (T1.EQ.0) THEN
             T2=X(ARC)
             T3=STARTN(ARC)
             DFCT(T3)=DFCT(T3)-T2
             T3=ENDN(ARC)
             DFCT(T3)=DFCT(T3)+T2
             U(ARC)=U(ARC)+T2
             X(ARC)=0
            END IF
          RC(ARC)=T1+DELPRC
          IF (RC(ARC).EQ.0) THEN
             DELX=DELX+X(ARC)
             NB = NB + 1
             PRDCSR(NB) = ARC
          END IF
        ELSE
          ARC=-ARC
          T1=RC(ARC)
            IF (T1.EQ.0) THEN
              T2=U(ARC)
              T3=STARTN(ARC)
              DFCT(T3)=DFCT(T3)+T2
              T3=ENDN(ARC)
              DFCT(T3)=DFCT(T3)-T2
              X(ARC)=X(ARC)+T2
              U(ARC)=0
            END IF
          RC(ARC)=T1-DELPRC
          IF (RC(ARC).EQ.0) THEN
             DELX=DELX+U(ARC)
             NB = NB + 1
             PRDCSR(NB) = ARC
        END IF
      END IF
6     CONTINUE
C
      END IF
      IF (DELX.LE.-DM) THEN
        DELPRC=LARGE
        DO 10 I=1,NSAVE
          ARC=SAVE(I)
          IF (ARC.GT.0) THEN
            RDCOST=RC(ARC)
            IF ((RDCOST.LT.0).AND.(-RDCOST.LT.DELPRC)) DELPRC=-RDCOST
          ELSE
            ARC=-ARC
            RDCOST=RC(ARC)
            IF ((RDCOST.GT.0).AND.(RDCOST.LT.DELPRC)) DELPRC=RDCOST
          END IF
10      CONTINUE
        IF ((DELPRC.NE.LARGE).OR.(DELX.LT.-DM)) GO TO 4
      END IF
C
C     *** Add new balance arcs to the superset of balanced arcs.  ***
C
      DO 9 I=1,NB
        ARC=PRDCSR(I)
          IF (TNXTIN(ARC).EQ.-1) THEN
            J=ENDN(ARC)
            TNXTIN(ARC)=TFSTIN(J)
            TFSTIN(J)=ARC
          END IF
          IF (TNXTOU(ARC).EQ.-1) THEN
            J=STARTN(ARC)
            TNXTOU(ARC)=TFSTOU(J)
            TFSTOU(J)=ARC
          END IF
9     CONTINUE
C
      RETURN
      END
