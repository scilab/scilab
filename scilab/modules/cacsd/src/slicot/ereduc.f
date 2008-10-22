      SUBROUTINE EREDUC(E, M, N, Q, Z, ISTAIR, RANKE, TOL)
C     PURPOSE: 
C        
C     Given an M x N matrix E (not necessarily regular) the subroutine         
C     EREDUC computes a unitary transformed matrix Q*E*Z which is in 
C     column echelon form (trapezoidal form). Furthermore the rank of
C     matrix E is determined.
C        
C     CONTRIBUTOR: Th.G.J. Beelen (Philips Glass Eindhoven). 
C     Copyright SLICOT
C        
C     REVISIONS: 1988, January 29.     
C        
C     Specification of the parameters. 
C        
C     .. Scalar arguments .. 
C        
      INTEGER LDE, LDQ, LDZ, M, N, RANKE         
      DOUBLE PRECISION TOL   
C        
C     .. Array arguments ..  
C        
      INTEGER ISTAIR(M)      
C      DOUBLE PRECISION E(LDE,N), Q(LDQ,M), Z(LDZ,N)
C      SET E(M,N) Q(M,M) Z(N,N)
      DOUBLE PRECISION E(M,N), Q(M,M), Z(N,N)
C     Local variables.       
C        
      INTEGER I, J, JMX, K, KM1, L, LK, MNK, NR1 
      DOUBLE PRECISION EMXNRM, EMX, SC, SS       
      LOGICAL LZERO
C
      LDE=M
      LDQ=M
      LDZ=N
      do 991 i=1,m
      do 991 j=1,m
      q(i,j)=0.d0
 991  continue
      do 992 i=1,m
      q(i,i)=1.0d0
 992  continue
      do 993 i=1,n
      do 993 j=1,n
      z(i,j)=0.d0
 993  continue
      do 994 i=1,n
      z(i,i)=1.0d0
 994  continue
      RANKE = MIN0(M,N)      
C        
      K = N        
      LZERO = .FALSE.        
C        
C     WHILE ((K > 0) AND (NOT a zero submatrix encountered) DO       
   10 IF ((K .GT. 0) .AND. (.NOT.LZERO)) THEN    
C        
C        
         MNK = M - N + K     
         EMXNRM = 0.0D0      
         LK = MNK  
         DO 20 L = MNK, 1, -1
             JMX = IDAMAX(K, E(L,1), LDE)                             
             EMX = DABS(E(L,JMX))        
            IF (EMX .GT. EMXNRM) THEN   
               EMXNRM = EMX                                                    
               LK = L
            END IF                                                           
   20    CONTINUE 
C                  
         IF (EMXNRM .LT. TOL) THEN     
C                  
C           Set submatrix Ek to zero.  
C                  
            DO 40 J = 1, K             
               DO 30 L = 1, MNK        
                  E(L,J) = 0.0D0       
   30          CONTINUE                
   40       CONTINUE                   
            LZERO = .TRUE.             
            RANKE = N - K              
         ELSE      
C                  
C           Submatrix Ek is not considered to be identically zero.             
C           Check whether rows have to be interchanged.    
C                  
            IF (LK .NE. MNK) THEN      
C                  
C              Interchange rows lk and m-n+k in whole E-matrix and             
C              update the row transformation matrix Q.     
C              (# elements involved = m)                   
C                  
               CALL DSWAP(N, E(LK,1), LDE, E(MNK,1), LDE)  
               CALL DSWAP(M, Q(LK,1), LDQ, Q(MNK,1), LDQ)  
            END IF 
C                  
            KM1 = K - 1                
            DO 50 J = 1, KM1           
C                  
C              Determine the column Givens transformation to annihilate        
C              E(m-n+k,j) using E(m-n+k,k) as pivot.       
C              Apply the transformation to the columns of Ek.                  
C              (# elements involved = m-n+k)               
C              Update the column transformation matrix Z.  
C              (# elements involved = n)                   
C                  
               CALL DGIV(E(MNK,K), E(MNK,J), SC, SS)       
               CALL DROT(MNK, E(1,K), 1, E(1,J), 1, SC, SS)
               E(MNK, J) = 0.0D0       
               CALL DROT(N, Z(1,K), 1, Z(1,J), 1, SC, SS)  
   50       CONTINUE                   
C                  
            K = K - 1                  
         END IF    
         GOTO 10   
      END IF       
C     END WHILE 10 
C                  
C     Initialise administration staircase form, i.e.,      
C     ISTAIR(i) =  j  if E(i,j) is a nonzero corner point  
C               = -j  if E(i,j) is on the boundary but is no corner pt.        
C     Thus,        
C     ISTAIR(m-k) =   n-k           for k=0,...,rank(E)-1  
C                 = -(n-rank(E)+1)  for k=rank(E),...,m-1. 
C                  
      DO 60 I = 1, RANKE               
         ISTAIR(M - I + 1) = N - I + 1 
   60 CONTINUE     
C                  
      NR1 = N - RANKE + 1              
      DO 70 I = RANKE, M - 1           
         ISTAIR(M-I) = -NR1            
   70 CONTINUE     
C                  
      RETURN       
C *** Last line of EREDUC *********************************************        
      END          
