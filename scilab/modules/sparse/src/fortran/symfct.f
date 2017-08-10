C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  January 12, 1995
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C**************    SFINIT  ..... SET UP FOR SYMB. FACT.     ************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       THIS SUBROUTINE COMPUTES THE STORAGE REQUIREMENTS AND SETS UP 
C       PRELIMINARY DATA STRUCTURES FOR THE SYMBOLIC FACTORIZATION.
C
C   NOTE:
C       THIS VERSION PRODUCES THE MAXIMAL SUPERNODE PARTITION (I.E.,
C       THE ONE WITH THE FEWEST POSSIBLE SUPERNODES).
C
C   INPUT PARAMETERS:
C       NEQNS       -   NUMBER OF EQUATIONS.
C       NNZA        -   LENGTH OF ADJACENCY STRUCTURE.
C       XADJ(*)     -   ARRAY OF LENGTH NEQNS+1, CONTAINING POINTERS
C                       TO THE ADJACENCY STRUCTURE.
C       ADJNCY(*)   -   ARRAY OF LENGTH XADJ(NEQNS+1)-1, CONTAINING
C                       THE ADJACENCY STRUCTURE.
C       PERM(*)     -   ARRAY OF LENGTH NEQNS, CONTAINING THE
C                       POSTORDERING.
C       INVP(*)     -   ARRAY OF LENGTH NEQNS, CONTAINING THE
C                       INVERSE OF THE POSTORDERING.
C       IWSIZ       -   SIZE OF INTEGER WORKING STORAGE.
C
C   OUTPUT PARAMETERS:
C       COLCNT(*)   -   ARRAY OF LENGTH NEQNS, CONTAINING THE NUMBER
C                       OF NONZEROS IN EACH COLUMN OF THE FACTOR,
C                       INCLUDING THE DIAGONAL ENTRY.
C       NNZL        -   NUMBER OF NONZEROS IN THE FACTOR, INCLUDING
C                       THE DIAGONAL ENTRIES.
C       NSUB        -   NUMBER OF SUBSCRIPTS.
C       NSUPER      -   NUMBER OF SUPERNODES (<= NEQNS).
C       SNODE(*)    -   ARRAY OF LENGTH NEQNS FOR RECORDING
C                       SUPERNODE MEMBERSHIP.
C       XSUPER(*)   -   ARRAY OF LENGTH NEQNS+1, CONTAINING THE
C                       SUPERNODE PARTITIONING.
C       IFLAG(*)    -   ERROR FLAG.
C                          0: SUCCESSFUL SF INITIALIZATION.
C                         -1: INSUFFICENT WORKING STORAGE
C                             [IWORK(*)].
C
C   WORK PARAMETERS:
C       IWORK(*)    -   INTEGER WORK ARRAY OF LENGTH 7*NEQNS+3.
C
C   FIRST CREATED ON    NOVEMEBER 14, 1994.
C   LAST UPDATED ON     January 12, 1995.
C
C***********************************************************************
C
      SUBROUTINE SFINIT  (  NEQNS , NNZA  , XADJ  , ADJNCY, PERM  ,
     &                      INVP  , COLCNT, NNZL  , NSUB  , NSUPER,
     &                      SNODE , XSUPER, IWSIZ , IWORK , IFLAG   )
C
C       ----------- 
C       PARAMETERS.  
C       -----------
        INTEGER             IFLAG , IWSIZ , NNZA  , NEQNS , NNZL  , 
     &                      NSUB  , NSUPER
        INTEGER             ADJNCY(NNZA)    , COLCNT(NEQNS)   ,
     &                      INVP(NEQNS)     , IWORK(7*NEQNS+3),
     &                      PERM(NEQNS)     , SNODE(NEQNS)    , 
     &                      XADJ(NEQNS+1)   , XSUPER(NEQNS+1)
C
C***********************************************************************
C
C       --------------------------------------------------------
C       RETURN IF THERE IS INSUFFICIENT INTEGER WORKING STORAGE.
C       --------------------------------------------------------
        IFLAG = 0
        IF  ( IWSIZ .LT. 7*NEQNS+3 )  THEN
            IFLAG = -1
            RETURN
        ENDIF
C
C       ------------------------------------------
C       COMPUTE ELIMINATION TREE AND POSTORDERING.
C       ------------------------------------------
        CALL  ETORDR (  NEQNS , XADJ  , ADJNCY, PERM  , INVP  ,
     &                  IWORK(1)              ,
     &                  IWORK(NEQNS+1)        ,
     &                  IWORK(2*NEQNS+1)      ,
     &                  IWORK(3*NEQNS+1)        ) 
C
C       ---------------------------------------------
C       COMPUTE ROW AND COLUMN FACTOR NONZERO COUNTS.
C       ---------------------------------------------
        CALL  FCNTHN (  NEQNS , NNZA  , XADJ  , ADJNCY, PERM  , 
     &                  INVP  , IWORK(1)      , SNODE , COLCNT, 
     &                  NNZL  ,
     &                  IWORK(NEQNS+1)        ,
     &                  IWORK(2*NEQNS+1)      ,
     &                  XSUPER                ,
     &                  IWORK(3*NEQNS+1)      ,
     &                  IWORK(4*NEQNS+2)      ,
     &                  IWORK(5*NEQNS+3)      ,
     &                  IWORK(6*NEQNS+4)        )
C
C       ---------------------------------------------------------
C       REARRANGE CHILDREN SO THAT THE LAST CHILD HAS THE MAXIMUM 
C       NUMBER OF NONZEROS IN ITS COLUMN OF L.
C       ---------------------------------------------------------
        CALL  CHORDR (  NEQNS , XADJ  , ADJNCY, PERM  , INVP  ,
     &                  COLCNT, 
     &                  IWORK(1)              ,
     &                  IWORK(NEQNS+1)        ,
     &                  IWORK(2*NEQNS+1)      ,
     &                  IWORK(3*NEQNS+1)        )
C
C       ----------------
C       FIND SUPERNODES.
C       ----------------
        CALL  FSUP1  (  NEQNS , IWORK(1)      , COLCNT, NSUB  , 
     &                  NSUPER, SNODE                           )
        CALL  FSUP2  (  NEQNS , NSUPER, IWORK(1)      , SNODE ,
     &                  XSUPER                                  )
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Joseph W.H. Liu
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C**********     ETORDR ..... ELIMINATION TREE REORDERING     ***********
C***********************************************************************
C***********************************************************************
C
C   WRITTEN BY JOSEPH LIU (JUL 17, 1985)
C
C   PURPOSE:
C       TO DETERMINE AN EQUIVALENT REORDERING BASED ON THE STRUCTURE OF
C       THE ELIMINATION TREE.  A POSTORDERING OF THE GIVEN ELIMINATION
C       TREE IS RETURNED.
C
C   INPUT PARAMETERS:
C       NEQNS           -   NUMBER OF EQUATIONS.
C       (XADJ,ADJNCY)   -   THE ADJACENCY STRUCTURE.
C
C   UPDATED PARAMETERS:
C       (PERM,INVP)     -   ON INPUT, THE GIVEN PERM AND INVERSE PERM
C                           VECTORS.  ON OUTPUT, THE NEW PERM AND
C                           INVERSE PERM VECTORS OF THE EQUIVALENT
C                           ORDERING.
C
C   OUTPUT PARAMETERS:
C       PARENT          -   THE PARENT VECTOR OF THE ELIMINATION TREE
C                           ASSOCIATED WITH THE NEW ORDERING.
C
C   WORKING PARAMETERS:
C       FSON            -   THE FIRST SON VECTOR.
C       BROTHR          -   THE BROTHER VECTOR.
C       INVPOS          -   THE INVERSE PERM VECTOR FOR THE
C                           POSTORDERING.
C
C   PROGRAM SUBROUTINES:
C       BETREE, ETPOST, ETREE , INVINV.
C
C***********************************************************************
C
      SUBROUTINE  ETORDR (  NEQNS , XADJ  , ADJNCY, PERM  , INVP  ,
     &                      PARENT, FSON  , BROTHR, INVPOS          )
C
C***********************************************************************
C
        INTEGER           ADJNCY(*)     , BROTHR(*)     ,
     &                      FSON(*)       , INVP(*)       ,
     &                      INVPOS(*)     , PARENT(*)     ,
     &                      PERM(*)
C
        INTEGER           XADJ(*)
        INTEGER           NEQNS
C
C***********************************************************************
C
C       -----------------------------
C       COMPUTE THE ELIMINATION TREE.
C       -----------------------------
        CALL  ETREE ( NEQNS, XADJ, ADJNCY, PERM, INVP, PARENT, INVPOS )
C
C       --------------------------------------------------------
C       COMPUTE A BINARY REPRESENTATION OF THE ELIMINATION TREE.
C       --------------------------------------------------------
        CALL  BETREE ( NEQNS, PARENT, FSON, BROTHR )
C
C       -------------------------------
C       POSTORDER THE ELIMINATION TREE.
C       -------------------------------
        CALL  ETPOST ( NEQNS, FSON, BROTHR, INVPOS, PARENT, PERM )
C
C       --------------------------------------------------------
C       COMPOSE THE ORIGINAL ORDERING WITH THE NEW POSTORDERING.
C       --------------------------------------------------------
        CALL  INVINV ( NEQNS, INVP, INVPOS, PERM )
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Joseph W.H. Liu
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C****************     ETREE ..... ELIMINATION TREE     *****************
C***********************************************************************
C***********************************************************************
C
C   WRITTEN BY JOSEPH LIU (JUL 17, 1985)
C
C   PURPOSE:
C       TO DETERMINE THE ELIMINATION TREE FROM A GIVEN ORDERING AND
C       THE ADJACENCY STRUCTURE.  THE PARENT VECTOR IS RETURNED.
C
C   INPUT PARAMETERS:
C       NEQNS           -   NUMBER OF EQUATIONS.
C       (XADJ,ADJNCY)   -   THE ADJACENCY STRUCTURE.
C       (PERM,INVP)     -   PERMUTATION AND INVERSE PERMUTATION VECTORS
C
C   OUTPUT PARAMETERS:
C       PARENT          -   THE PARENT VECTOR OF THE ELIMINATION TREE.
C
C   WORKING PARAMETERS:
C       ANCSTR          -   THE ANCESTOR VECTOR.
C
C***********************************************************************
C
      SUBROUTINE  ETREE (   NEQNS , XADJ  , ADJNCY, PERM  , INVP  ,
     &                      PARENT, ANCSTR                          )
C
C***********************************************************************
C
        INTEGER           ADJNCY(*)     , ANCSTR(*)     ,
     &                      INVP(*)       , PARENT(*)     ,
     &                      PERM(*)
C
        INTEGER           NEQNS
        INTEGER           XADJ(*)
C
C***********************************************************************
C
        INTEGER           I     , J     , JSTOP , JSTRT , NBR   ,
     &                      NEXT  , NODE
C
C***********************************************************************
C
        IF  ( NEQNS .LE. 0 )  RETURN
C
        DO  400  I = 1, NEQNS
            PARENT(I) = 0
            ANCSTR(I) = 0
            NODE = PERM(I)
C
            JSTRT = XADJ(NODE)
            JSTOP = XADJ(NODE+1) - 1
            IF  ( JSTRT .LE. JSTOP )  THEN
                DO  300  J = JSTRT, JSTOP
                    NBR = ADJNCY(J)
                    NBR = INVP(NBR)
                    IF  ( NBR .LT. I )  THEN
C                       -------------------------------------------
C                       FOR EACH NBR, FIND THE ROOT OF ITS CURRENT
C                       ELIMINATION TREE.  PERFORM PATH COMPRESSION
C                       AS THE SUBTREE IS TRAVERSED.
C                       -------------------------------------------
  100                   CONTINUE
                            IF  ( ANCSTR(NBR) .EQ. I )  GO TO 300
                            IF  ( ANCSTR(NBR) .GT. 0 )  THEN
                                NEXT = ANCSTR(NBR)
                                ANCSTR(NBR) = I
                                NBR = NEXT
                                GO TO 100
                            ENDIF
C                       --------------------------------------------
C                       NOW, NBR IS THE ROOT OF THE SUBTREE.  MAKE I
C                       THE PARENT NODE OF THIS ROOT.
C                       --------------------------------------------
                        PARENT(NBR) = I
                        ANCSTR(NBR) = I
                    ENDIF
  300           CONTINUE
            ENDIF
  400   CONTINUE
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Joseph W.H. Liu
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******     BETREE ..... BINARY TREE REPRESENTATION OF ETREE     *******
C***********************************************************************
C***********************************************************************
C
C   WRITTEN BY JOSEPH LIU (JUL 17, 1985)
C
C   PURPOSE:
C       TO DETERMINE THE BINARY TREE REPRESENTATION OF THE ELIMINATION
C       TREE GIVEN BY THE PARENT VECTOR.  THE RETURNED REPRESENTATION
C       WILL BE GIVEN BY THE FIRST-SON AND BROTHER VECTORS.  THE ROOT
C       OF THE BINARY TREE IS ALWAYS NEQNS.
C
C   INPUT PARAMETERS:
C       NEQNS           -   NUMBER OF EQUATIONS.
C       PARENT          -   THE PARENT VECTOR OF THE ELIMINATION TREE.
C                           IT IS ASSUMED THAT PARENT(I) > I EXCEPT OF
C                           THE ROOTS.
C
C   OUTPUT PARAMETERS:
C       FSON            -   THE FIRST SON VECTOR.
C       BROTHR          -   THE BROTHER VECTOR.
C
C***********************************************************************
C
      SUBROUTINE  BETREE (  NEQNS , PARENT, FSON  , BROTHR          )
C
C***********************************************************************
C
        INTEGER           BROTHR(*)     , FSON(*)       ,
     &                      PARENT(*)
C
        INTEGER           NEQNS
C
C***********************************************************************
C
        INTEGER           LROOT , NODE  , NDPAR
C
C***********************************************************************
C
        IF  ( NEQNS .LE. 0 )  RETURN
C
        DO  100  NODE = 1, NEQNS
            FSON(NODE) = 0
            BROTHR(NODE) = 0
  100   CONTINUE
        LROOT = NEQNS
C       ------------------------------------------------------------
C       FOR EACH NODE := NEQNS-1 STEP -1 DOWNTO 1, DO THE FOLLOWING.
C       ------------------------------------------------------------
        IF  ( NEQNS .LE. 1 )  RETURN
        DO  300  NODE = NEQNS-1, 1, -1
            NDPAR = PARENT(NODE)
            IF  ( NDPAR .LE. 0  .OR.  NDPAR .EQ. NODE )  THEN
C               -------------------------------------------------
C               NODE HAS NO PARENT.  GIVEN STRUCTURE IS A FOREST.
C               SET NODE TO BE ONE OF THE ROOTS OF THE TREES.
C               -------------------------------------------------
                BROTHR(LROOT) = NODE
                LROOT = NODE
            ELSE
C               -------------------------------------------
C               OTHERWISE, BECOMES FIRST SON OF ITS PARENT.
C               -------------------------------------------
                BROTHR(NODE) = FSON(NDPAR)
                FSON(NDPAR) = NODE
            ENDIF
  300   CONTINUE
        BROTHR(LROOT) = 0
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Joseph W.H. Liu
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C***************     ETPOST ..... ETREE POSTORDERING     ***************
C***********************************************************************
C***********************************************************************
C
C   WRITTEN BY JOSEPH LIU (SEPT 17, 1986)
C
C   PURPOSE:
C       BASED ON THE BINARY REPRESENTATION (FIRST-SON,BROTHER) OF
C       THE ELIMINATION TREE, A POSTORDERING IS DETERMINED. THE
C       CORRESPONDING PARENT VECTOR IS ALSO MODIFIED TO REFLECT
C       THE REORDERING.
C
C   INPUT PARAMETERS:
C       ROOT            -   ROOT OF THE ELIMINATION TREE (USUALLY IT
C                           IS NEQNS).
C       FSON            -   THE FIRST SON VECTOR.
C       BROTHR          -   THE BROTHR VECTOR.
C
C   UPDATED PARAMETERS:
C       PARENT          -   THE PARENT VECTOR.
C
C   OUTPUT PARAMETERS:
C       INVPOS          -   INVERSE PERMUTATION FOR THE POSTORDERING.
C
C   WORKING PARAMETERS:
C       STACK           -   THE STACK FOR POSTORDER TRAVERSAL OF THE
C                           TREE.
C
C***********************************************************************
C
      SUBROUTINE  ETPOST (  ROOT  , FSON  , BROTHR, INVPOS, PARENT,
     &                      STACK                                   )
C
C***********************************************************************
C
        INTEGER           BROTHR(*)     , FSON(*)       ,
     &                      INVPOS(*)     , PARENT(*)     ,
     &                      STACK(*)
C
        INTEGER           ROOT
C
C***********************************************************************
C
        INTEGER           ITOP  , NDPAR , NODE  , NUM   , NUNODE
C
C***********************************************************************
C
        NUM = 0
        ITOP = 0
        NODE = ROOT
C       -------------------------------------------------------------
C       TRAVERSE ALONG THE FIRST SONS POINTER AND PUSH THE TREE NODES
C       ALONG THE TRAVERSAL INTO THE STACK.
C       -------------------------------------------------------------
  100   CONTINUE
            ITOP = ITOP + 1
            STACK(ITOP) = NODE
            NODE = FSON(NODE)
            IF  ( NODE .GT. 0 )  GO TO 100
C           ----------------------------------------------------------
C           IF POSSIBLE, POP A TREE NODE FROM THE STACK AND NUMBER IT.
C           ----------------------------------------------------------
  200       CONTINUE
                IF  ( ITOP .LE. 0 )  GO TO 300
                NODE = STACK(ITOP)
                ITOP = ITOP - 1
                NUM = NUM + 1
                INVPOS(NODE) = NUM
C               ----------------------------------------------------
C               THEN, TRAVERSE TO ITS YOUNGER BROTHER IF IT HAS ONE.
C               ----------------------------------------------------
                NODE = BROTHR(NODE)
                IF  ( NODE .LE. 0 )  GO TO 200
            GO TO 100
C
  300   CONTINUE
C       ------------------------------------------------------------
C       DETERMINE THE NEW PARENT VECTOR OF THE POSTORDERING.  BROTHR
C       IS USED TEMPORARILY FOR THE NEW PARENT VECTOR.
C       ------------------------------------------------------------
        DO  400  NODE = 1, NUM
            NUNODE = INVPOS(NODE)
            NDPAR = PARENT(NODE)
            IF  ( NDPAR .GT. 0 )  NDPAR = INVPOS(NDPAR)
            BROTHR(NUNODE) = NDPAR
  400   CONTINUE
C
        DO  500  NUNODE = 1, NUM
            PARENT(NUNODE) = BROTHR(NUNODE)
  500   CONTINUE
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Joseph W.H. Liu
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C***********     INVINV ..... CONCATENATION OF TWO INVP     ************
C***********************************************************************
C***********************************************************************
C
C   WRITTEN BY JOSEPH LIU (JUL 17, 1985)
C
C   PURPOSE:
C       TO PERFORM THE MAPPING OF
C           ORIGINAL-INVP --> INTERMEDIATE-INVP --> NEW INVP
C       AND THE RESULTING ORDERING REPLACES INVP.  THE NEW PERMUTATION
C       VECTOR PERM IS ALSO COMPUTED.
C
C   INPUT PARAMETERS:
C       NEQNS           -   NUMBER OF EQUATIONS.
C       INVP2           -   THE SECOND INVERSE PERMUTATION VECTOR.
C
C   UPDATED PARAMETERS:
C       INVP            -   THE FIRST INVERSE PERMUTATION VECTOR.  ON
C                           OUTPUT, IT CONTAINS THE NEW INVERSE
C                           PERMUTATION.
C
C   OUTPUT PARAMETER:
C       PERM            -   NEW PERMUTATION VECTOR (CAN BE THE SAME AS
C                           INVP2).
C
C***********************************************************************
C
      SUBROUTINE  INVINV (  NEQNS , INVP  , INVP2 , PERM            )
C
C***********************************************************************
C
        INTEGER           INVP(*)       , INVP2(*)      ,
     &                      PERM(*)
C
        INTEGER           NEQNS
C
C***********************************************************************
C
        INTEGER           I     , INTERM, NODE
C
C***********************************************************************
C
        DO  100  I = 1, NEQNS
            INTERM = INVP(I)
            INVP(I) = INVP2(INTERM)
  100   CONTINUE
C
        DO  200  I = 1, NEQNS
            NODE = INVP(I)
            PERM(NODE) = I
  200   CONTINUE
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C**********     CHORDR ..... CHILD REORDERING                ***********
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       REARRANGE THE CHILDREN OF EACH VERTEX SO THAT THE LAST ONE 
C       MAXIMIZES (AMONG THE CHILDREN) THE NUMBER OF NONZEROS IN THE 
C       CORRESPONDING COLUMN OF L.  ALSO DETERMINE AN NEW POSTORDERING 
C       BASED ON THE STRUCTURE OF THE MODIFIED ELIMINATION TREE.
C
C   INPUT PARAMETERS:
C       NEQNS           -   NUMBER OF EQUATIONS.
C       (XADJ,ADJNCY)   -   THE ADJACENCY STRUCTURE.
C
C   UPDATED PARAMETERS:
C       (PERM,INVP)     -   ON INPUT, THE GIVEN PERM AND INVERSE PERM
C                           VECTORS.  ON OUTPUT, THE NEW PERM AND
C                           INVERSE PERM VECTORS OF THE NEW
C                           POSTORDERING.
C       COLCNT          -   COLUMN COUNTS IN L UNDER INITIAL ORDERING;
C                           MODIFIED TO REFLECT THE NEW ORDERING.
C
C   OUTPUT PARAMETERS:
C       PARENT          -   THE PARENT VECTOR OF THE ELIMINATION TREE
C                           ASSOCIATED WITH THE NEW ORDERING.
C
C   WORKING PARAMETERS:
C       FSON            -   THE FIRST SON VECTOR.
C       BROTHR          -   THE BROTHER VECTOR.
C       INVPOS          -   THE INVERSE PERM VECTOR FOR THE
C                           POSTORDERING.
C
C   PROGRAM SUBROUTINES:
C       BTREE2, EPOST2, INVINV.
C
C***********************************************************************
C
      SUBROUTINE  CHORDR (  NEQNS , XADJ  , ADJNCY, PERM  , INVP  ,
     &                      COLCNT, PARENT, FSON  , BROTHR, INVPOS  )
C
C***********************************************************************
C
        INTEGER             ADJNCY(*)     , BROTHR(*)     ,
     &                      COLCNT(*)     , FSON(*)       , 
     &                      INVP(*)       , INVPOS(*)     , 
     &                      PARENT(*)     , PERM(*)
C
        INTEGER             XADJ(*)
        INTEGER             NEQNS
C
C***********************************************************************
C
C       ----------------------------------------------------------
C       COMPUTE A BINARY REPRESENTATION OF THE ELIMINATION TREE, 
C       SO THAT EACH "LAST CHILD" MAXIMIZES AMONG ITS SIBLINGS THE 
C       NUMBER OF NONZEROS IN THE CORRESPONDING COLUMNS OF L.
C       ----------------------------------------------------------
        CALL  BTREE2  ( NEQNS , PARENT, COLCNT, FSON  , BROTHR, 
     &                  INVPOS                                  )
C
C       ----------------------------------------------------
C       POSTORDER THE ELIMINATION TREE (USING THE NEW BINARY  
C       REPRESENTATION.  
C       ----------------------------------------------------
        CALL  EPOST2  ( NEQNS , FSON  , BROTHR, INVPOS, PARENT, 
     &                  COLCNT, PERM                            ) 
C
C       --------------------------------------------------------
C       COMPOSE THE ORIGINAL ORDERING WITH THE NEW POSTORDERING.
C       --------------------------------------------------------
        CALL  INVINV  ( NEQNS , INVP  , INVPOS, PERM    )
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  January 12, 1995
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******     BTREE2 ..... BINARY TREE REPRESENTATION OF ETREE     *******
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       TO DETERMINE A BINARY TREE REPRESENTATION OF THE ELIMINATION 
C       TREE, FOR WHICH EVERY "LAST CHILD" HAS THE MAXIMUM POSSIBLE
C       COLUMN NONZERO COUNT IN THE FACTOR.  THE RETURNED REPRESENTATION 
C       WILL BE GIVEN BY THE FIRST-SON AND BROTHER VECTORS.  THE ROOT OF 
C       THE BINARY TREE IS ALWAYS NEQNS.
C 
C   INPUT PARAMETERS:
C       NEQNS           -   NUMBER OF EQUATIONS.
C       PARENT          -   THE PARENT VECTOR OF THE ELIMINATION TREE.
C                           IT IS ASSUMED THAT PARENT(I) > I EXCEPT OF
C                           THE ROOTS.
C       COLCNT          -   COLUMN NONZERO COUNTS OF THE FACTOR.
C
C   OUTPUT PARAMETERS:
C       FSON            -   THE FIRST SON VECTOR.
C       BROTHR          -   THE BROTHER VECTOR.
C
C   WORKING PARAMETERS:
C       LSON            -   LAST SON VECTOR.
C
C***********************************************************************
C
      SUBROUTINE  BTREE2 (  NEQNS , PARENT, COLCNT, FSON  , BROTHR,
     &                      LSON    )
C
C***********************************************************************
C
        INTEGER             BROTHR(*)     , COLCNT(*)     ,
     &                      FSON(*)       , LSON(*)       ,
     &                      PARENT(*)
C
        INTEGER             NEQNS
C
C***********************************************************************
C
        INTEGER           LROOT , NODE  , NDLSON, NDPAR
C
C***********************************************************************
C
        IF  ( NEQNS .LE. 0 )  RETURN
C
        DO  100  NODE = 1, NEQNS
            FSON(NODE) = 0
            BROTHR(NODE) = 0
            LSON(NODE) = 0
  100   CONTINUE
        LROOT = NEQNS
C       ------------------------------------------------------------
C       FOR EACH NODE := NEQNS-1 STEP -1 DOWNTO 1, DO THE FOLLOWING.
C       ------------------------------------------------------------
        IF  ( NEQNS .LE. 1 )  RETURN
        DO  300  NODE = NEQNS-1, 1, -1
            NDPAR = PARENT(NODE)
            IF  ( NDPAR .LE. 0  .OR.  NDPAR .EQ. NODE )  THEN
C               -------------------------------------------------
C               NODE HAS NO PARENT.  GIVEN STRUCTURE IS A FOREST.
C               SET NODE TO BE ONE OF THE ROOTS OF THE TREES.
C               -------------------------------------------------
                BROTHR(LROOT) = NODE
                LROOT = NODE
            ELSE
C               -------------------------------------------
C               OTHERWISE, BECOMES FIRST SON OF ITS PARENT.
C               -------------------------------------------
                NDLSON = LSON(NDPAR)
                IF  ( NDLSON .NE. 0 )  THEN
                    IF  ( COLCNT(NODE) .GE. COLCNT(NDLSON) )  THEN
                        BROTHR(NODE) = FSON(NDPAR)
                        FSON(NDPAR) = NODE
                    ELSE
                        BROTHR(NDLSON) = NODE
                        LSON(NDPAR) = NODE
                    ENDIF
                ELSE
                    FSON(NDPAR) = NODE
                    LSON(NDPAR) = NODE
                ENDIF
            ENDIF
  300   CONTINUE
        BROTHR(LROOT) = 0
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C***************     EPOST2 ..... ETREE POSTORDERING #2  ***************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       BASED ON THE BINARY REPRESENTATION (FIRST-SON,BROTHER) OF THE 
C       ELIMINATION TREE, A POSTORDERING IS DETERMINED. THE
C       CORRESPONDING PARENT AND COLCNT VECTORS ARE ALSO MODIFIED TO 
C       REFLECT THE REORDERING.
C
C   INPUT PARAMETERS:
C       ROOT            -   ROOT OF THE ELIMINATION TREE (USUALLY IT
C                           IS NEQNS).
C       FSON            -   THE FIRST SON VECTOR.
C       BROTHR          -   THE BROTHR VECTOR.
C
C   UPDATED PARAMETERS:
C       PARENT          -   THE PARENT VECTOR.
C       COLCNT          -   COLUMN NONZERO COUNTS OF THE FACTOR.
C
C   OUTPUT PARAMETERS:
C       INVPOS          -   INVERSE PERMUTATION FOR THE POSTORDERING.
C
C   WORKING PARAMETERS:
C       STACK           -   THE STACK FOR POSTORDER TRAVERSAL OF THE
C                           TREE.
C
C***********************************************************************
C
      SUBROUTINE  EPOST2 (  ROOT  , FSON  , BROTHR, INVPOS, PARENT,
     &                      COLCNT, STACK                           )
C
C***********************************************************************
C
        INTEGER           BROTHR(*)     , COLCNT(*)     , 
     &                      FSON(*)       , INVPOS(*)     , 
     &                      PARENT(*)     , STACK(*)
C
        INTEGER           ROOT
C
C***********************************************************************
C
        INTEGER           ITOP  , NDPAR , NODE  , NUM   , NUNODE
C
C***********************************************************************
C
        NUM = 0
        ITOP = 0
        NODE = ROOT
C       -------------------------------------------------------------
C       TRAVERSE ALONG THE FIRST SONS POINTER AND PUSH THE TREE NODES
C       ALONG THE TRAVERSAL INTO THE STACK.
C       -------------------------------------------------------------
  100   CONTINUE
            ITOP = ITOP + 1
            STACK(ITOP) = NODE
            NODE = FSON(NODE)
            IF  ( NODE .GT. 0 )  GO TO 100
C           ----------------------------------------------------------
C           IF POSSIBLE, POP A TREE NODE FROM THE STACK AND NUMBER IT.
C           ----------------------------------------------------------
  200       CONTINUE
                IF  ( ITOP .LE. 0 )  GO TO 300
                NODE = STACK(ITOP)
                ITOP = ITOP - 1
                NUM = NUM + 1
                INVPOS(NODE) = NUM
C               ----------------------------------------------------
C               THEN, TRAVERSE TO ITS YOUNGER BROTHER IF IT HAS ONE.
C               ----------------------------------------------------
                NODE = BROTHR(NODE)
                IF  ( NODE .LE. 0 )  GO TO 200
            GO TO 100
C
  300   CONTINUE
C       ------------------------------------------------------------
C       DETERMINE THE NEW PARENT VECTOR OF THE POSTORDERING.  BROTHR
C       IS USED TEMPORARILY FOR THE NEW PARENT VECTOR.
C       ------------------------------------------------------------
        DO  400  NODE = 1, NUM
            NUNODE = INVPOS(NODE)
            NDPAR = PARENT(NODE)
            IF  ( NDPAR .GT. 0 )  NDPAR = INVPOS(NDPAR)
            BROTHR(NUNODE) = NDPAR
  400   CONTINUE
C
        DO  500  NUNODE = 1, NUM
            PARENT(NUNODE) = BROTHR(NUNODE)
  500   CONTINUE
C
C       ----------------------------------------------
C       PERMUTE COLCNT(*) TO REFLECT THE NEW ORDERING.
C       ----------------------------------------------
        DO  600  NODE = 1, NUM
            NUNODE = INVPOS(NODE)
            STACK(NUNODE) = COLCNT(NODE)
  600   CONTINUE
C
        DO  700  NODE = 1, NUM
            COLCNT(NODE) = STACK(NODE)
  700   CONTINUE
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  January 12, 1995
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C**************     FCNTHN  ..... FIND NONZERO COUNTS    ***************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       THIS SUBROUTINE DETERMINES THE ROW COUNTS AND COLUMN COUNTS IN
C       THE CHOLESKY FACTOR.  IT USES A DISJOINT SET UNION ALGORITHM.
C
C       TECHNIQUES:
C       1) SUPERNODE DETECTION.
C       2) PATH HALVING.
C       3) NO UNION BY RANK.
C
C   NOTES:
C       1) ASSUMES A POSTORDERING OF THE ELIMINATION TREE.
C
C   INPUT PARAMETERS:
C       (I) NEQNS       -   NUMBER OF EQUATIONS.
C       (I) ADJLEN      -   LENGTH OF ADJACENCY STRUCTURE.
C       (I) XADJ(*)     -   ARRAY OF LENGTH NEQNS+1, CONTAINING POINTERS
C                           TO THE ADJACENCY STRUCTURE.
C       (I) ADJNCY(*)   -   ARRAY OF LENGTH XADJ(NEQNS+1)-1, CONTAINING
C                           THE ADJACENCY STRUCTURE.
C       (I) PERM(*)     -   ARRAY OF LENGTH NEQNS, CONTAINING THE
C                           POSTORDERING.
C       (I) INVP(*)     -   ARRAY OF LENGTH NEQNS, CONTAINING THE
C                           INVERSE OF THE POSTORDERING.
C       (I) ETPAR(*)    -   ARRAY OF LENGTH NEQNS, CONTAINING THE
C                           ELIMINATION TREE OF THE POSTORDERED MATRIX.
C
C   OUTPUT PARAMETERS:
C       (I) ROWCNT(*)   -   ARRAY OF LENGTH NEQNS, CONTAINING THE NUMBER
C                           OF NONZEROS IN EACH ROW OF THE FACTOR,
C                           INCLUDING THE DIAGONAL ENTRY.
C       (I) COLCNT(*)   -   ARRAY OF LENGTH NEQNS, CONTAINING THE NUMBER
C                           OF NONZEROS IN EACH COLUMN OF THE FACTOR,
C                           INCLUDING THE DIAGONAL ENTRY.
C       (I) NLNZ        -   NUMBER OF NONZEROS IN THE FACTOR, INCLUDING
C                           THE DIAGONAL ENTRIES.
C
C   WORK PARAMETERS:
C       (I) SET(*)      -   ARRAY OF LENGTH NEQNS USED TO MAINTAIN THE
C                           DISJOINT SETS (I.E., SUBTREES).
C       (I) PRVLF(*)    -   ARRAY OF LENGTH NEQNS USED TO RECORD THE
C                           PREVIOUS LEAF OF EACH ROW SUBTREE.
C       (I) LEVEL(*)    -   ARRAY OF LENGTH NEQNS+1 CONTAINING THE LEVEL
C                           (DISTANCE FROM THE ROOT).
C       (I) WEIGHT(*)   -   ARRAY OF LENGTH NEQNS+1 CONTAINING WEIGHTS
C                           USED TO COMPUTE COLUMN COUNTS.
C       (I) FDESC(*)    -   ARRAY OF LENGTH NEQNS+1 CONTAINING THE
C                           FIRST (I.E., LOWEST-NUMBERED) DESCENDANT.
C       (I) NCHILD(*)   -   ARRAY OF LENGTH NEQNS+1 CONTAINING THE
C                           NUMBER OF CHILDREN.
C       (I) PRVNBR(*)   -   ARRAY OF LENGTH NEQNS USED TO RECORD THE
C                           PREVIOUS ``LOWER NEIGHBOR'' OF EACH NODE.
C
C   FIRST CREATED ON    APRIL 12, 1990.
C   LAST UPDATED ON     JANUARY 12, 1995.
C
C***********************************************************************
C
      SUBROUTINE FCNTHN  (  NEQNS , ADJLEN, XADJ  , ADJNCY, PERM  ,
     &                      INVP  , ETPAR , ROWCNT, COLCNT, NLNZ  ,
     &                      SET   , PRVLF , LEVEL , WEIGHT, FDESC ,
     &                      NCHILD, PRVNBR                          )
C
C       -----------
C       PARAMETERS.
C       -----------
        INTEGER             ADJLEN, NEQNS , NLNZ
        INTEGER             ADJNCY(ADJLEN)  , COLCNT(NEQNS) ,
     &                      ETPAR(NEQNS)    , FDESC(0:NEQNS),
     &                      INVP(NEQNS)     , LEVEL(0:NEQNS),
     &                      NCHILD(0:NEQNS) , PERM(NEQNS)   ,
     &                      PRVLF(NEQNS)    , PRVNBR(NEQNS) ,
     &                      ROWCNT(NEQNS)   , SET(NEQNS)    ,
     &                      WEIGHT(0:NEQNS)
        INTEGER             XADJ(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
        INTEGER             HINBR , IFDESC, J     , JSTOP , JSTRT , 
     &                      K     , LAST1 , LAST2 , LCA   , LFLAG , 
     &                      LOWNBR, OLDNBR, PARENT, PLEAF , TEMP  , 
     &                      XSUP
C
C***********************************************************************
C
C       --------------------------------------------------
C       COMPUTE LEVEL(*), FDESC(*), NCHILD(*).
C       INITIALIZE ROWCNT(*), COLCNT(*),
C                  SET(*), PRVLF(*), WEIGHT(*), PRVNBR(*).
C       --------------------------------------------------
CSS     Next line added, because XSUP may be indetermined below see
CSS     other CSS comment
        XSUP = 0
        LEVEL(0) = 0
        DO  100  K = NEQNS, 1, -1
            ROWCNT(K) = 1
            COLCNT(K) = 0
            SET(K) = K
            PRVLF(K) = 0
            LEVEL(K) = LEVEL(ETPAR(K)) + 1
            WEIGHT(K) = 1
            FDESC(K) = K
            NCHILD(K) = 0
            PRVNBR(K) = 0
  100   CONTINUE
        NCHILD(0) = 0
        FDESC(0) = 0
        DO  200  K = 1, NEQNS
            PARENT = ETPAR(K)
            WEIGHT(PARENT) = 0
            NCHILD(PARENT) = NCHILD(PARENT) + 1
            IFDESC = FDESC(K)
            IF  ( IFDESC .LT. FDESC(PARENT) )  THEN
                FDESC(PARENT) = IFDESC
            ENDIF
  200   CONTINUE
C       ------------------------------------
C       FOR EACH ``LOW NEIGHBOR'' LOWNBR ...
C       ------------------------------------
        DO  600  LOWNBR = 1, NEQNS
            LFLAG = 0
            IFDESC = FDESC(LOWNBR)
            OLDNBR = PERM(LOWNBR)
            JSTRT = XADJ(OLDNBR)
            JSTOP = XADJ(OLDNBR+1) - 1
C           -----------------------------------------------
C           FOR EACH ``HIGH NEIGHBOR'', HINBR OF LOWNBR ...
C           -----------------------------------------------
            DO  500  J = JSTRT, JSTOP
                HINBR = INVP(ADJNCY(J))
                IF  ( HINBR .GT. LOWNBR )  THEN
                    IF  ( IFDESC .GT. PRVNBR(HINBR) )  THEN
C                       -------------------------
C                       INCREMENT WEIGHT(LOWNBR).
C                       -------------------------
                        WEIGHT(LOWNBR) = WEIGHT(LOWNBR) + 1
                        PLEAF = PRVLF(HINBR)
C                       -----------------------------------------
C                       IF HINBR HAS NO PREVIOUS ``LOW NEIGHBOR'' 
C                       THEN ...
C                       -----------------------------------------
                        IF  ( PLEAF .EQ. 0 )  THEN
C                           -----------------------------------------
C                           ... ACCUMULATE LOWNBR-->HINBR PATH LENGTH 
C                               IN ROWCNT(HINBR).
C                           -----------------------------------------
                            ROWCNT(HINBR) = ROWCNT(HINBR) +
     &                                  LEVEL(LOWNBR) - LEVEL(HINBR)
                        ELSE
C                           -----------------------------------------
C                           ... OTHERWISE, LCA <-- FIND(PLEAF), WHICH 
C                               IS THE LEAST COMMON ANCESTOR OF PLEAF 
C                               AND LOWNBR.
C                               (PATH HALVING.)
C                           -----------------------------------------
                            LAST1 = PLEAF
                            LAST2 = SET(LAST1)
                            LCA = SET(LAST2)
  300                       CONTINUE
                                IF  ( LCA .NE. LAST2 )  THEN
                                    SET(LAST1) = LCA
                                    LAST1 = LCA
                                    LAST2 = SET(LAST1)
                                    LCA = SET(LAST2)
                                    GO TO 300
                                ENDIF
C                           -------------------------------------
C                           ACCUMULATE PLEAF-->LCA PATH LENGTH IN 
C                           ROWCNT(HINBR).
C                           DECREMENT WEIGHT(LCA).
C                           -------------------------------------
                            ROWCNT(HINBR) = ROWCNT(HINBR)
     &                                  + LEVEL(LOWNBR) - LEVEL(LCA)
                            WEIGHT(LCA) = WEIGHT(LCA) - 1
                        ENDIF
C                       ----------------------------------------------
C                       LOWNBR NOW BECOMES ``PREVIOUS LEAF'' OF HINBR.
C                       ----------------------------------------------
                        PRVLF(HINBR) = LOWNBR
                        LFLAG = 1
                    ENDIF
C                   --------------------------------------------------
C                   LOWNBR NOW BECOMES ``PREVIOUS NEIGHBOR'' OF HINBR.
C                   --------------------------------------------------
                    PRVNBR(HINBR) = LOWNBR
                ENDIF
  500       CONTINUE
C           ----------------------------------------------------
C           DECREMENT WEIGHT ( PARENT(LOWNBR) ).
C           SET ( P(LOWNBR) ) <-- SET ( P(LOWNBR) ) + SET(XSUP).
C           ----------------------------------------------------
            PARENT = ETPAR(LOWNBR)
            WEIGHT(PARENT) = WEIGHT(PARENT) - 1
            IF  ( LFLAG .EQ. 1     .OR.
     &            NCHILD(LOWNBR) .GE. 2 )  THEN
                XSUP = LOWNBR
            ENDIF
CSS   original code was "SET(XSUP) = PARENT" 
            IF (XSUP.gt.0) SET(XSUP) = PARENT
  600   CONTINUE
C       ---------------------------------------------------------
C       USE WEIGHTS TO COMPUTE COLUMN (AND TOTAL) NONZERO COUNTS.
C       ---------------------------------------------------------
        NLNZ = 0
        DO  700  K = 1, NEQNS
            TEMP = COLCNT(K) + WEIGHT(K)
            COLCNT(K) = TEMP
            NLNZ = NLNZ + TEMP
            PARENT = ETPAR(K)
            IF  ( PARENT .NE. 0 )  THEN
                COLCNT(PARENT) = COLCNT(PARENT) + TEMP
            ENDIF
  700   CONTINUE
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C****************    FSUP1 ..... FIND SUPERNODES #1    *****************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       THIS SUBROUTINE IS THE FIRST OF TWO ROUTINES FOR FINDING A
C       MAXIMAL SUPERNODE PARTITION.  IT RETURNS ONLY THE NUMBER OF
C       SUPERNODES NSUPER AND THE SUPERNODE MEMBERSHIP VECTOR SNODE(*), 
C       WHICH IS OF LENGTH NEQNS.  THE VECTORS OF LENGTH NSUPER ARE 
C       COMPUTED SUBSEQUENTLY BY THE COMPANION ROUTINE FSUP2.
C
C   METHOD AND ASSUMPTIONS:
C       THIS ROUTINE USES THE ELIMINATION TREE AND THE FACTOR COLUMN 
C       COUNTS TO COMPUTE THE SUPERNODE PARTITION; IT ALSO ASSUMES A 
C       POSTORDERING OF THE ELIMINATION TREE.
C
C   INPUT PARAMETERS:
C       (I) NEQNS       -   NUMBER OF EQUATIONS.
C       (I) ETPAR(*)    -   ARRAY OF LENGTH NEQNS, CONTAINING THE
C                           ELIMINATION TREE OF THE POSTORDERED MATRIX.
C       (I) COLCNT(*)   -   ARRAY OF LENGTH NEQNS, CONTAINING THE
C                           FACTOR COLUMN COUNTS: I.E., THE NUMBER OF 
C                           NONZERO ENTRIES IN EACH COLUMN OF L
C                           (INCLUDING THE DIAGONAL ENTRY).
C
C   OUTPUT PARAMETERS:
C       (I) NOFSUB      -   NUMBER OF SUBSCRIPTS.
C       (I) NSUPER      -   NUMBER OF SUPERNODES (<= NEQNS).
C       (I) SNODE(*)    -   ARRAY OF LENGTH NEQNS FOR RECORDING
C                           SUPERNODE MEMBERSHIP.
C
C   FIRST CREATED ON    JANUARY 18, 1992.
C   LAST UPDATED ON     NOVEMBER 11, 1994.
C
C***********************************************************************
C
      SUBROUTINE  FSUP1  (  NEQNS , ETPAR , COLCNT, NOFSUB, NSUPER,
     &                      SNODE                                   )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
        INTEGER             NEQNS , NOFSUB, NSUPER
        INTEGER             COLCNT(*)     , ETPAR(*)      ,
     &                      SNODE(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
        INTEGER             KCOL
C
C***********************************************************************
C
C       --------------------------------------------
C       COMPUTE THE FUNDAMENTAL SUPERNODE PARTITION.
C       --------------------------------------------
        NSUPER = 1
        SNODE(1) = 1
        NOFSUB = COLCNT(1)
        DO  300  KCOL = 2, NEQNS
            IF  ( ETPAR(KCOL-1) .EQ. KCOL )  THEN
                IF  ( COLCNT(KCOL-1) .EQ. COLCNT(KCOL)+1 )  THEN
                    SNODE(KCOL) = NSUPER
                    GO TO 300
                ENDIF
            ENDIF
            NSUPER = NSUPER + 1
            SNODE(KCOL) = NSUPER
            NOFSUB = NOFSUB + COLCNT(KCOL)
  300   CONTINUE
C
      RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C****************    FSUP2  ..... FIND SUPERNODES #2   *****************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       THIS SUBROUTINE IS THE SECOND OF TWO ROUTINES FOR FINDING A
C       MAXIMAL SUPERNODE PARTITION.  IT'S SOLE PURPOSE IS TO 
C       CONSTRUCT THE NEEDED VECTOR OF LENGTH NSUPER: XSUPER(*).  THE
C       FIRST ROUTINE FSUP1 COMPUTES THE NUMBER OF SUPERNODES AND THE 
C       SUPERNODE MEMBERSHIP VECTOR SNODE(*), WHICH IS OF LENGTH NEQNS.
C
C
C   ASSUMPTIONS:
C       THIS ROUTINE ASSUMES A POSTORDERING OF THE ELIMINATION TREE.  IT
C       ALSO ASSUMES THAT THE OUTPUT FROM FSUP1 IS AVAILABLE.
C
C   INPUT PARAMETERS:
C       (I) NEQNS       -   NUMBER OF EQUATIONS.
C       (I) NSUPER      -   NUMBER OF SUPERNODES (<= NEQNS).
C       (I) ETPAR(*)    -   ARRAY OF LENGTH NEQNS, CONTAINING THE
C                           ELIMINATION TREE OF THE POSTORDERED MATRIX.
C       (I) SNODE(*)    -   ARRAY OF LENGTH NEQNS FOR RECORDING
C                           SUPERNODE MEMBERSHIP.
C
C   OUTPUT PARAMETERS:
C       (I) XSUPER(*)   -   ARRAY OF LENGTH NSUPER+1, CONTAINING THE
C                           SUPERNODE PARTITIONING.
C
C   FIRST CREATED ON    JANUARY 18, 1992.
C   LAST UPDATED ON     NOVEMEBER 22, 1994.
C
C***********************************************************************
C
      SUBROUTINE  FSUP2  (  NEQNS , NSUPER, ETPAR , SNODE , XSUPER  )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
        INTEGER             NEQNS , NSUPER
        INTEGER             ETPAR(*)      , SNODE(*)      , 
     &                      XSUPER(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
        INTEGER             KCOL  , KSUP  , LSTSUP
C
C***********************************************************************
C
C       -------------------------------------------------
C       COMPUTE THE SUPERNODE PARTITION VECTOR XSUPER(*).
C       -------------------------------------------------
        LSTSUP = NSUPER + 1
        DO  100  KCOL = NEQNS, 1, -1
            KSUP = SNODE(KCOL)
            IF  ( KSUP .NE. LSTSUP )  THEN
                XSUPER(LSTSUP) = KCOL + 1
            ENDIF
            LSTSUP = KSUP
  100   CONTINUE
        XSUPER(1) = 1
C
      RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  February 13, 1995
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C*************     SYMFCT ..... SYMBOLIC FACTORIZATION    **************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE: 
C       THIS ROUTINE CALLS SYMFC2 WHICH PERFORMS SUPERNODAL SYMBOLIC
C       FACTORIZATION ON A REORDERED LINEAR SYSTEM.
C
C   INPUT PARAMETERS:
C       (I) NEQNS       -   NUMBER OF EQUATIONS
C       (I) ADJLEN      -   LENGTH OF THE ADJACENCY LIST.
C       (I) XADJ(*)     -   ARRAY OF LENGTH NEQNS+1 CONTAINING POINTERS
C                           TO THE ADJACENCY STRUCTURE.
C       (I) ADJNCY(*)   -   ARRAY OF LENGTH XADJ(NEQNS+1)-1 CONTAINING
C                           THE ADJACENCY STRUCTURE.
C       (I) PERM(*)     -   ARRAY OF LENGTH NEQNS CONTAINING THE
C                           POSTORDERING.
C       (I) INVP(*)     -   ARRAY OF LENGTH NEQNS CONTAINING THE
C                           INVERSE OF THE POSTORDERING.
C       (I) COLCNT(*)   -   ARRAY OF LENGTH NEQNS, CONTAINING THE NUMBER
C                           OF NONZEROS IN EACH COLUMN OF THE FACTOR,
C                           INCLUDING THE DIAGONAL ENTRY.
C       (I) NSUPER      -   NUMBER OF SUPERNODES.
C       (I) XSUPER(*)   -   ARRAY OF LENGTH NSUPER+1, CONTAINING THE
C                           FIRST COLUMN OF EACH SUPERNODE.
C       (I) SNODE(*)    -   ARRAY OF LENGTH NEQNS FOR RECORDING
C                           SUPERNODE MEMBERSHIP.
C       (I) NOFSUB      -   NUMBER OF SUBSCRIPTS TO BE STORED IN
C                           LINDX(*).
C       (I) IWSIZ       -   SIZE OF INTEGER WORKING STORAGE.
C
C   OUTPUT PARAMETERS:
C       (I) XLINDX      -   ARRAY OF LENGTH NEQNS+1, CONTAINING POINTERS 
C                           INTO THE SUBSCRIPT VECTOR.
C       (I) LINDX       -   ARRAY OF LENGTH MAXSUB, CONTAINING THE
C                           COMPRESSED SUBSCRIPTS.
C       (I) XLNZ        -   COLUMN POINTERS FOR L.
C       (I) FLAG        -   ERROR FLAG:
C                               0 - NO ERROR.
C                              -1 - INSUFFICIENT INTEGER WORKING SPACE.
C                              -2 - INCONSISTANCY IN THE INPUT.
C       
C   WORKING PARAMETERS:
C       (I) IWORK       -   WORKING ARRAY OF LENGTH NSUPER+2*NEQNS.
C
C***********************************************************************
C
      SUBROUTINE  SYMFCT (  NEQNS , ADJLEN, XADJ  , ADJNCY, PERM  , 
     &                      INVP  , COLCNT, NSUPER, XSUPER, SNODE ,
     &                      NOFSUB, XLINDX, LINDX , XLNZ  , IWSIZ ,
     &                      IWORK ,
     &                      FLAG    )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
        INTEGER             ADJLEN, FLAG  , IWSIZ , NEQNS , NOFSUB, 
     &                      NSUPER
        INTEGER             ADJNCY(ADJLEN), COLCNT(NEQNS) ,
     &                      INVP(NEQNS)   , 
     &                      IWORK(NSUPER+2*NEQNS+1),
     &                      LINDX(NOFSUB) , 
     &                      PERM(NEQNS)   , SNODE(NEQNS)  , 
     &                      XSUPER(NSUPER+1)
        INTEGER             XADJ(NEQNS+1) , XLINDX(NSUPER+1),
     &                      XLNZ(NEQNS+1)
C
C***********************************************************************
C
        FLAG = 0
        IF  ( IWSIZ .LT. NSUPER+2*NEQNS+1 )  THEN
            FLAG = -1
            RETURN
        ENDIF
        CALL  SYMFC2 (  NEQNS , ADJLEN, XADJ  , ADJNCY, PERM  , 
     &                  INVP  , COLCNT, NSUPER, XSUPER, SNODE ,
     &                  NOFSUB, XLINDX, LINDX , XLNZ  , 
     &                  IWORK(1)              ,
     &                  IWORK(NSUPER+1)       ,
     &                  IWORK(NSUPER+NEQNS+2) ,
     &                  FLAG    )
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  February 13, 1995
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C*************     SYMFC2 ..... SYMBOLIC FACTORIZATION    **************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE: 
C       THIS ROUTINE PERFORMS SUPERNODAL SYMBOLIC FACTORIZATION ON A 
C       REORDERED LINEAR SYSTEM.  IT ASSUMES ACCESS TO THE COLUMNS 
C       COUNTS, SUPERNODE PARTITION, AND SUPERNODAL ELIMINATION TREE
C       ASSOCIATED WITH THE FACTOR MATRIX L.
C
C   INPUT PARAMETERS:
C       (I) NEQNS       -   NUMBER OF EQUATIONS
C       (I) ADJLEN      -   LENGTH OF THE ADJACENCY LIST.
C       (I) XADJ(*)     -   ARRAY OF LENGTH NEQNS+1 CONTAINING POINTERS
C                           TO THE ADJACENCY STRUCTURE.
C       (I) ADJNCY(*)   -   ARRAY OF LENGTH XADJ(NEQNS+1)-1 CONTAINING
C                           THE ADJACENCY STRUCTURE.
C       (I) PERM(*)     -   ARRAY OF LENGTH NEQNS CONTAINING THE
C                           POSTORDERING.
C       (I) INVP(*)     -   ARRAY OF LENGTH NEQNS CONTAINING THE
C                           INVERSE OF THE POSTORDERING.
C       (I) COLCNT(*)   -   ARRAY OF LENGTH NEQNS, CONTAINING THE NUMBER
C                           OF NONZEROS IN EACH COLUMN OF THE FACTOR,
C                           INCLUDING THE DIAGONAL ENTRY.
C       (I) NSUPER      -   NUMBER OF SUPERNODES.
C       (I) XSUPER(*)   -   ARRAY OF LENGTH NSUPER+1, CONTAINING THE
C                           FIRST COLUMN OF EACH SUPERNODE.
C       (I) SNODE(*)    -   ARRAY OF LENGTH NEQNS FOR RECORDING
C                           SUPERNODE MEMBERSHIP.
C       (I) NOFSUB      -   NUMBER OF SUBSCRIPTS TO BE STORED IN
C                           LINDX(*).
C
C   OUTPUT PARAMETERS:
C       (I) XLINDX      -   ARRAY OF LENGTH NEQNS+1, CONTAINING POINTERS 
C                           INTO THE SUBSCRIPT VECTOR.
C       (I) LINDX       -   ARRAY OF LENGTH MAXSUB, CONTAINING THE
C                           COMPRESSED SUBSCRIPTS.
C       (I) XLNZ        -   COLUMN POINTERS FOR L.
C       (I) FLAG        -   ERROR FLAG:
C                               0 - NO ERROR.
C                               1 - INCONSISTANCY IN THE INPUT.
C       
C   WORKING PARAMETERS:
C       (I) MRGLNK      -   ARRAY OF LENGTH NSUPER, CONTAINING THE 
C                           CHILDREN OF EACH SUPERNODE AS A LINKED LIST.
C       (I) RCHLNK      -   ARRAY OF LENGTH NEQNS+1, CONTAINING THE 
C                           CURRENT LINKED LIST OF MERGED INDICES (THE 
C                           "REACH" SET).
C       (I) MARKER      -   ARRAY OF LENGTH NEQNS USED TO MARK INDICES
C                           AS THEY ARE INTRODUCED INTO EACH SUPERNODE'S
C                           INDEX SET.
C
C***********************************************************************
C
      SUBROUTINE  SYMFC2 (  NEQNS , ADJLEN, XADJ  , ADJNCY, PERM  , 
     &                      INVP  , COLCNT, NSUPER, XSUPER, SNODE ,
     &                      NOFSUB, XLINDX, LINDX , XLNZ  , MRGLNK,
     &                      RCHLNK, MARKER, FLAG    )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
        INTEGER             ADJLEN, FLAG  , NEQNS , NOFSUB, NSUPER
        INTEGER             ADJNCY(ADJLEN), COLCNT(NEQNS) ,
     &                      INVP(NEQNS)   , MARKER(NEQNS) ,
     &                      MRGLNK(NSUPER), LINDX(NOFSUB) , 
     &                      PERM(NEQNS)   , RCHLNK(0:NEQNS), 
     &                      SNODE(NEQNS)  , XSUPER(NSUPER+1)
        INTEGER             XADJ(NEQNS+1) , XLINDX(NSUPER+1),
     &                      XLNZ(NEQNS+1)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
        INTEGER             FSTCOL, HEAD  , I     , JNZBEG, JNZEND,
     &                      JPTR  , JSUP  , JWIDTH, KNZ   , KNZBEG,
     &                      KNZEND, KPTR  , KSUP  , LENGTH, LSTCOL,
     &                      NEWI  , NEXTI , NODE  , NZBEG , NZEND ,
     &                      PCOL  , PSUP  , POINT , TAIL  , WIDTH
C
C***********************************************************************
C
        FLAG = 0
        IF  ( NEQNS .LE. 0 )  RETURN
C
C       ---------------------------------------------------
C       INITIALIZATIONS ...
C           NZEND  : POINTS TO THE LAST USED SLOT IN LINDX.
C           TAIL   : END OF LIST INDICATOR 
C                    (IN RCHLNK(*), NOT MRGLNK(*)).
C           MRGLNK : CREATE EMPTY LISTS.
C           MARKER : "UNMARK" THE INDICES.
C       ---------------------------------------------------
        NZEND = 0
        HEAD = 0
        TAIL = NEQNS + 1
        POINT = 1
        DO  50  I = 1, NEQNS
            MARKER(I) = 0
            XLNZ(I) = POINT
            POINT = POINT + COLCNT(I)
   50   CONTINUE
        XLNZ(NEQNS+1) = POINT
        POINT = 1
        DO  100  KSUP = 1, NSUPER
            MRGLNK(KSUP) = 0
            FSTCOL = XSUPER(KSUP)
            XLINDX(KSUP) = POINT
            POINT = POINT + COLCNT(FSTCOL)
  100   CONTINUE
        XLINDX(NSUPER+1) = POINT
C
C       ---------------------------
C       FOR EACH SUPERNODE KSUP ... 
C       ---------------------------
        DO  1000  KSUP = 1, NSUPER
C
C           ---------------------------------------------------------
C           INITIALIZATIONS ...
C               FSTCOL : FIRST COLUMN OF SUPERNODE KSUP.
C               LSTCOL : LAST COLUMN OF SUPERNODE KSUP.
C               KNZ    : WILL COUNT THE NONZEROS OF L IN COLUMN KCOL.
C               RCHLNK : INITIALIZE EMPTY INDEX LIST FOR KCOL.
C           ---------------------------------------------------------
            FSTCOL = XSUPER(KSUP)
            LSTCOL = XSUPER(KSUP+1) - 1
            WIDTH  = LSTCOL - FSTCOL + 1
            LENGTH = COLCNT(FSTCOL)
            KNZ = 0
            RCHLNK(HEAD) = TAIL
            JSUP = MRGLNK(KSUP)
C
C           -------------------------------------------------
C           IF KSUP HAS CHILDREN IN THE SUPERNODAL E-TREE ...
C           -------------------------------------------------
            IF  ( JSUP .GT. 0 )  THEN
C               ---------------------------------------------
C               COPY THE INDICES OF THE FIRST CHILD JSUP INTO 
C               THE LINKED LIST, AND MARK EACH WITH THE VALUE 
C               KSUP.
C               ---------------------------------------------
                JWIDTH = XSUPER(JSUP+1) - XSUPER(JSUP)
                JNZBEG = XLINDX(JSUP) + JWIDTH
                JNZEND = XLINDX(JSUP+1) - 1
                DO  200  JPTR = JNZEND, JNZBEG, -1
                    NEWI = LINDX(JPTR)
                    KNZ = KNZ+1
                    MARKER(NEWI) = KSUP
                    RCHLNK(NEWI) = RCHLNK(HEAD)
                    RCHLNK(HEAD) = NEWI
  200           CONTINUE
C               ------------------------------------------
C               FOR EACH SUBSEQUENT CHILD JSUP OF KSUP ...
C               ------------------------------------------
                JSUP = MRGLNK(JSUP)
  300           CONTINUE
                IF  ( JSUP .NE. 0  .AND.  KNZ .LT. LENGTH )  THEN
C                   ----------------------------------------
C                   MERGE THE INDICES OF JSUP INTO THE LIST,
C                   AND MARK NEW INDICES WITH VALUE KSUP.
C                   ----------------------------------------
                    JWIDTH = XSUPER(JSUP+1) - XSUPER(JSUP)
                    JNZBEG = XLINDX(JSUP) + JWIDTH
                    JNZEND = XLINDX(JSUP+1) - 1
                    NEXTI = HEAD
                    DO  500  JPTR = JNZBEG, JNZEND
                        NEWI = LINDX(JPTR)
  400                   CONTINUE
                            I = NEXTI
                            NEXTI = RCHLNK(I)
                            IF  ( NEWI .GT. NEXTI )  GO TO 400
                        IF  ( NEWI .LT. NEXTI )  THEN
                            KNZ = KNZ+1
                            RCHLNK(I) = NEWI
                            RCHLNK(NEWI) = NEXTI
                            MARKER(NEWI) = KSUP
                            NEXTI = NEWI
                        ENDIF
  500               CONTINUE
                    JSUP = MRGLNK(JSUP)
                    GO TO 300
                ENDIF
            ENDIF
C           ---------------------------------------------------
C           STRUCTURE OF A(*,FSTCOL) HAS NOT BEEN EXAMINED YET.  
C           "SORT" ITS STRUCTURE INTO THE LINKED LIST,
C           INSERTING ONLY THOSE INDICES NOT ALREADY IN THE
C           LIST.
C           ---------------------------------------------------
            IF  ( KNZ .LT. LENGTH )  THEN
                NODE = PERM(FSTCOL)
                KNZBEG = XADJ(NODE)
                KNZEND = XADJ(NODE+1) - 1
                DO  700  KPTR = KNZBEG, KNZEND
                    NEWI = ADJNCY(KPTR)
                    NEWI = INVP(NEWI)
                    IF  ( NEWI .GT. FSTCOL  .AND.
     &                    MARKER(NEWI) .NE. KSUP )  THEN
C                       --------------------------------
C                       POSITION AND INSERT NEWI IN LIST
C                       AND MARK IT WITH KCOL.
C                       --------------------------------
                        NEXTI = HEAD
  600                   CONTINUE
                            I = NEXTI
                            NEXTI = RCHLNK(I)
                            IF  ( NEWI .GT. NEXTI )  GO TO 600
                        KNZ = KNZ + 1
                        RCHLNK(I) = NEWI
                        RCHLNK(NEWI) = NEXTI
                        MARKER(NEWI) = KSUP
                    ENDIF
  700           CONTINUE
            ENDIF
C           ------------------------------------------------------------
C           IF KSUP HAS NO CHILDREN, INSERT FSTCOL INTO THE LINKED LIST.
C           ------------------------------------------------------------
            IF  ( RCHLNK(HEAD) .NE. FSTCOL )  THEN
                RCHLNK(FSTCOL) = RCHLNK(HEAD)
                RCHLNK(HEAD) = FSTCOL
                KNZ = KNZ + 1
            ENDIF
C
C           --------------------------------------------
C           COPY INDICES FROM LINKED LIST INTO LINDX(*).
C           --------------------------------------------
            NZBEG = NZEND + 1
            NZEND = NZEND + KNZ
            IF  ( NZEND+1 .NE. XLINDX(KSUP+1) )  GO TO 8000
            I = HEAD
            DO  800  KPTR = NZBEG, NZEND
                I = RCHLNK(I)
                LINDX(KPTR) = I
  800       CONTINUE
C
C           ---------------------------------------------------
C           IF KSUP HAS A PARENT, INSERT KSUP INTO ITS PARENT'S 
C           "MERGE" LIST.
C           ---------------------------------------------------
            IF  ( LENGTH .GT. WIDTH )  THEN
                PCOL = LINDX ( XLINDX(KSUP) + WIDTH )
                PSUP = SNODE(PCOL)
                MRGLNK(KSUP) = MRGLNK(PSUP)
                MRGLNK(PSUP) = KSUP
            ENDIF
C
 1000   CONTINUE
C
        RETURN
C
C       -----------------------------------------------
C       INCONSISTENCY IN DATA STRUCTURE WAS DISCOVERED.
C       -----------------------------------------------
 8000   CONTINUE
        FLAG = -2
        RETURN
C
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******     BFINIT ..... INITIALIZATION FOR BLOCK FACTORIZATION   ******
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       THIS SUBROUTINE COMPUTES ITEMS NEEDED BY THE LEFT-LOOKING
C       BLOCK-TO-BLOCK CHOLESKY FACTORITZATION ROUTINE BLKFCT.
C
C   INPUT PARAMETERS:
C       NEQNS           -   NUMBER OF EQUATIONS.
C       NSUPER          -   NUMBER OF SUPERNODES.
C       XSUPER          -   INTEGER ARRAY OF SIZE (NSUPER+1) CONTAINING
C                           THE SUPERNODE PARTITIONING.
C       SNODE           -   SUPERNODE MEMBERSHIP.
C       (XLINDX,LINDX)  -   ARRAYS DESCRIBING THE SUPERNODAL STRUCTURE.
C       CACHSZ          -   CACHE SIZE (IN KBYTES).
C
C   OUTPUT PARAMETERS:
C       TMPSIZ          -   SIZE OF WORKING STORAGE REQUIRED BY BLKFCT.
C       SPLIT           -   SPLITTING OF SUPERNODES SO THAT THEY FIT
C                           INTO CACHE.
C
C***********************************************************************
C
      SUBROUTINE  BFINIT  ( NEQNS , NSUPER, XSUPER, SNODE , XLINDX, 
     &                      LINDX , CACHSZ, TMPSIZ, SPLIT           )
C
C***********************************************************************
C
        INTEGER     CACHSZ, NEQNS , NSUPER, TMPSIZ
        INTEGER     XLINDX(*)       , XSUPER(*)
        INTEGER     LINDX (*)       , SNODE (*)   ,
     &              SPLIT(*)
C
C***********************************************************************
C
C       ---------------------------------------------------
C       DETERMINE FLOATING POINT WORKING SPACE REQUIREMENT.
C       ---------------------------------------------------
        CALL  FNTSIZ (  NSUPER, XSUPER, SNODE , XLINDX, LINDX ,
     &                  TMPSIZ                                  )
C
C       -------------------------------
C       PARTITION SUPERNODES FOR CACHE.
C       -------------------------------
        CALL  FNSPLT (  NEQNS , NSUPER, XSUPER, XLINDX, CACHSZ,
     &                  SPLIT                                   )
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******     FNTSIZ ..... COMPUTE WORK STORAGE SIZE FOR BLKFCT     ******
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       THIS SUBROUTINE DETERMINES THE SIZE OF THE WORKING STORAGE
C       REQUIRED BY BLKFCT.
C
C   INPUT PARAMETERS:
C       NSUPER          -   NUMBER OF SUPERNODES.
C       XSUPER          -   INTEGER ARRAY OF SIZE (NSUPER+1) CONTAINING
C                           THE SUPERNODE PARTITIONING.
C       SNODE           -   SUPERNODE MEMBERSHIP.
C       (XLINDX,LINDX)  -   ARRAYS DESCRIBING THE SUPERNODAL STRUCTURE.
C
C   OUTPUT PARAMETERS:
C       TMPSIZ          -   SIZE OF WORKING STORAGE REQUIRED BY BLKFCT.
C
C***********************************************************************
C
        SUBROUTINE  FNTSIZ ( NSUPER, XSUPER, SNODE , XLINDX, 
     &                       LINDX , TMPSIZ  )
C
C***********************************************************************
C
        INTEGER     NSUPER, TMPSIZ
        INTEGER     XLINDX(*)       , XSUPER(*)
        INTEGER     LINDX (*)       , SNODE (*)
C
        INTEGER     BOUND , CLEN  , CURSUP, I     , IBEGIN, IEND  , 
     &              KSUP  , LENGTH, NCOLS , NXTSUP, 
     &              TSIZE , WIDTH
C
C***********************************************************************
C
C       RETURNS SIZE OF TEMP ARRAY USED BY BLKFCT FACTORIZATION ROUTINE.
C       NOTE THAT THE VALUE RETURNED IS AN ESTIMATE, THOUGH IT IS USUALLY
C       TIGHT.
C
C       ----------------------------------------
C       COMPUTE SIZE OF TEMPORARY STORAGE VECTOR
C       NEEDED BY BLKFCT.
C       ----------------------------------------
        TMPSIZ = 0
        DO  500  KSUP = NSUPER, 1, -1
            NCOLS = XSUPER(KSUP+1) - XSUPER(KSUP)
            IBEGIN = XLINDX(KSUP) + NCOLS
            IEND = XLINDX(KSUP+1) - 1
            LENGTH = IEND - IBEGIN + 1
            BOUND = LENGTH * (LENGTH + 1) / 2
            IF  ( BOUND .GT. TMPSIZ )  THEN
                CURSUP = SNODE(LINDX(IBEGIN))
                CLEN = XLINDX(CURSUP+1) - XLINDX(CURSUP)
                WIDTH = 0
                DO  400  I = IBEGIN, IEND
                    NXTSUP = SNODE(LINDX(I))
                    IF  ( NXTSUP .EQ. CURSUP )  THEN
                        WIDTH = WIDTH + 1
                        IF  ( I .EQ. IEND )  THEN
                            IF  ( CLEN .GT. LENGTH )  THEN
                                TSIZE = LENGTH * WIDTH - 
     &                                  (WIDTH - 1) * WIDTH / 2
                                TMPSIZ = MAX ( TSIZE , TMPSIZ )
                            ENDIF
                        ENDIF
                    ELSE
                        IF  ( CLEN .GT. LENGTH )  THEN
                            TSIZE = LENGTH * WIDTH - 
     &                              (WIDTH - 1) * WIDTH / 2
                            TMPSIZ = MAX ( TSIZE , TMPSIZ )
                        ENDIF
                        LENGTH = LENGTH - WIDTH
                        BOUND = LENGTH * (LENGTH + 1) / 2
                        IF  ( BOUND .LE. TMPSIZ )  GO TO 500
                        WIDTH = 1
                        CURSUP = NXTSUP
                        CLEN = XLINDX(CURSUP+1) - XLINDX(CURSUP)
                    ENDIF
  400           CONTINUE
            ENDIF
  500   CONTINUE
C
        RETURN
        END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C****     FNSPLT ..... COMPUTE FINE PARTITIONING OF SUPERNODES     *****
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       THIS SUBROUTINE DETERMINES A FINE PARTITIONING OF SUPERNODES
C       WHEN THERE IS A CACHE AVAILABLE ON THE MACHINE.  THE FINE
C       PARTITIONING IS CHOSEN SO THAT DATA RE-USE IS MAXIMIZED.
C       
C   INPUT PARAMETERS:
C       NEQNS           -   NUMBER OF EQUATIONS.
C       NSUPER          -   NUMBER OF SUPERNODES.
C       XSUPER          -   INTEGER ARRAY OF SIZE (NSUPER+1) CONTAINING
C                           THE SUPERNODE PARTITIONING.
C       XLINDX          -   INTEGER ARRAY OF SIZE (NSUPER+1) CONTAINING
C                           POINTERS IN THE SUPERNODE INDICES.
C       CACHSZ          -   CACHE SIZE IN KILO BYTES.
C                           IF THERE IS NO CACHE, SET CACHSZ = 0.
C
C   OUTPUT PARAMETERS:
C       SPLIT           -   INTEGER ARRAY OF SIZE NEQNS CONTAINING THE
C                           FINE PARTITIONING.
C
C***********************************************************************
C
        SUBROUTINE  FNSPLT ( NEQNS , NSUPER, XSUPER, XLINDX,
     &                       CACHSZ, SPLIT )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
        INTEGER         CACHSZ, NEQNS , NSUPER
        INTEGER         XSUPER(*), SPLIT(*)
        INTEGER         XLINDX(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
        INTEGER         CACHE , CURCOL, FSTCOL, HEIGHT, KCOL  , 
     1                  KSUP  , LSTCOL, NCOLS , NXTBLK, USED  , 
     1                  WIDTH
C
C *******************************************************************
C
C       --------------------------------------------
C       COMPUTE THE NUMBER OF 8-BYTE WORDS IN CACHE.
C       --------------------------------------------
        IF  ( CACHSZ .LE. 0 )  THEN
            CACHE = 2 000 000 000
        ELSE
            CACHE = ( FLOAT(CACHSZ) * 1024. / 8. ) * 0.9
        ENDIF
C
C       ---------------
C       INITIALIZATION.
C       ---------------
        DO  100  KCOL = 1, NEQNS
            SPLIT(KCOL) = 0
  100   CONTINUE
C
C       ---------------------------
C       FOR EACH SUPERNODE KSUP ...
C       ---------------------------
        DO  1000  KSUP = 1, NSUPER
C           -----------------------
C           ... GET SUPERNODE INFO.
C           -----------------------
            HEIGHT = XLINDX(KSUP+1) - XLINDX(KSUP)
            FSTCOL = XSUPER(KSUP)
            LSTCOL = XSUPER(KSUP+1) - 1
            WIDTH = LSTCOL - FSTCOL + 1
            NXTBLK = FSTCOL
C           --------------------------------------
C           ... UNTIL ALL COLUMNS OF THE SUPERNODE 
C               HAVE BEEN PROCESSED ...
C           --------------------------------------
            CURCOL = FSTCOL - 1
  200       CONTINUE
C               -------------------------------------------
C               ... PLACE THE FIRST COLUMN(S) IN THE CACHE.
C               -------------------------------------------
                CURCOL = CURCOL + 1
                IF  ( CURCOL .LT. LSTCOL )  THEN
                    CURCOL = CURCOL + 1
                    NCOLS = 2
                    USED = 3 * HEIGHT - 1
                    HEIGHT = HEIGHT - 2
                ELSE
                    NCOLS = 1
                    USED = 2 * HEIGHT
                    HEIGHT = HEIGHT - 1
                ENDIF
C
C               --------------------------------------
C               ... WHILE THE CACHE IS NOT FILLED AND
C                   THERE ARE COLUMNS OF THE SUPERNODE 
C                   REMAINING TO BE PROCESSED ...
C               --------------------------------------
  300           CONTINUE
                IF  ( USED+HEIGHT .LT. CACHE  .AND.
     &                CURCOL      .LT. LSTCOL       )  THEN
C                   --------------------------------
C                   ... ADD ANOTHER COLUMN TO CACHE.
C                   --------------------------------
                    CURCOL = CURCOL + 1
                    NCOLS = NCOLS + 1
                    USED = USED + HEIGHT
                    HEIGHT = HEIGHT - 1
                    GO TO 300
                ENDIF
C               -------------------------------------
C               ... RECORD THE NUMBER OF COLUMNS THAT 
C                   FILLED THE CACHE.
C               -------------------------------------
                SPLIT(NXTBLK) = NCOLS
                NXTBLK = NXTBLK + 1
C               --------------------------
C               ... GO PROCESS NEXT BLOCK.
C               --------------------------
                IF  ( CURCOL .LT. LSTCOL )  GO TO 200
 1000   CONTINUE
C
        RETURN
        END
