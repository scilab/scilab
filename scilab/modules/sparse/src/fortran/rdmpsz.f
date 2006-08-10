C**************************************************************
C     ****  RDMPSZ ... READ THE  MPS FILE TO GET MAX SIZES ****
C**************************************************************
C
      SUBROUTINE rdmpsz(INMPS,M,N,NZA,RCODE,TYPROW,LINE)
C *** PARAMETERS
      INTEGER*4 RCODE,M,N,NZA,INMPS,LINE
      CHARACTER*2 TYPROW
C     INMPS : logical unit of the MPS file
C     RCODE : error indicator
C             0 = OK
C             1 = Error while reading line "LINE" of the MPS file.
C             2 = ERROR: Unknown row type "TYPROW" at line "LINE".
C     TYPROW: SET IF RCODE==2
C     LINE  : SET IF RCODE>0
C     M     : NUMBER OF CONSTRAINTS
C     N     : number of variables.
C     NZA   : number of nonzeros of the LP constraint matrix.


C
C *** LOCAL VARIABLES
      INTEGER*4I,J,COLLEN
      DOUBLE PRECISION SMALLA,VAL1,VAL2
      CHARACTER*8 NAME0,NAMRW1,NAMRW2,NAMCLN
      CHARACTER*8 NAMMPS
      CHARACTER*4 NM
      CHARACTER*100 RDLINE
      CHARACTER SECT

C
      SMALLA=1.0D-10
C
C     Format used to read every line of the MPS file.
 1000 FORMAT(A80)
C
C     Initialize.
      M=0
      RCODE=0
      LINE=0
C
C     Read the problem name.
   60 LINE=LINE+1
      READ(INMPS,1000,END=9000) RDLINE
      IF(RDLINE(1:1).EQ.'*') GO TO 60
      READ(RDLINE,61,ERR=9000) NM,NAMMPS
   61 FORMAT(A4,10X,A8)
      IF(NM.NE.'NAME'.AND.NM.NE.'name') GO TO 60
   70 LINE=LINE+1
      READ(INMPS,1000,END=9000) RDLINE
      IF(RDLINE(1:1).EQ.'*') GO TO 70
      READ(RDLINE,71,ERR=9000) SECT
   71 FORMAT(A1)
      IF(SECT.NE.'R'.AND.SECT.NE.'r') GO TO 9000
C
C     Read the ROWS section.
  100 LINE=LINE+1
      READ(INMPS,1000,END=9000) RDLINE
      IF(RDLINE(1:1).EQ.'*') GO TO 100
      READ(RDLINE,101,ERR=9000) SECT,TYPROW,NAMRW1
  101 FORMAT(A1,A2,1X,A8)
      IF(SECT.NE.' ') GO TO 200
C
C     Here if a constraint has been found. Check its type.
      M=M+1
C
      IF(TYPROW.EQ.' E'.OR.TYPROW.EQ.'E '.OR.
     X   TYPROW.EQ.' e'.OR.TYPROW.EQ.'e ') THEN
         GO TO 100
      ENDIF
C
      IF(TYPROW.EQ.' G'.OR.TYPROW.EQ.'G '.OR.
     X   TYPROW.EQ.' g'.OR.TYPROW.EQ.'g ') THEN
         GO TO 100
      ENDIF
C
      IF(TYPROW.EQ.' L'.OR.TYPROW.EQ.'L '.OR.
     X   TYPROW.EQ.' l'.OR.TYPROW.EQ.'l ') THEN
         GO TO 100
      ENDIF
C
      IF(TYPROW.EQ.' N'.OR.TYPROW.EQ.'N '.OR.
     X   TYPROW.EQ.' n'.OR.TYPROW.EQ.'n ') THEN
         GO TO 100
      ENDIF
C
C     Invalid row type.
      GO TO 9050
C     Continue reading of the  ROWS section.
      GO TO 100
C
C     Read COLUMNS section.
  200 CONTINUE
C
      IF(SECT.NE.'C'.AND.SECT.NE.'c') GO TO 9000
      NAME0='        '
  220 LINE=LINE+1
      READ(INMPS,1000,END=9000) RDLINE
      IF(RDLINE(1:1).EQ.'*') GO TO 220
      READ(RDLINE,221,ERR=9000) SECT,NAMCLN,NAMRW1,VAL1,NAMRW2,VAL2
  221 FORMAT(A1,3X,A8,2X,A8,2X,D12.0,3X,A8,2X,D12.0)
      IF(NAMCLN.EQ.NAME0) GO TO 260
C
C     Here if the new column has been found.
C     Save the previous column in the LP data structures.
C
C     Check if this is the first column.
      IF(NAME0.EQ.'        ') THEN
         NAME0=NAMCLN
         COLLEN=0
         NZA=0
         N=1
         GO TO 260
      ENDIF
C
      NZA=NZA+COLLEN
C
C     Check if there are still columns to be read.
      IF(SECT.NE.' ') THEN
         RETURN
      ELSE
C
C     Initialize the new column.
         N=N+1
         NAME0=NAMCLN
         COLLEN=0
         GO TO 260
      ENDIF
C
C
C     Find the position of the nonzero element.
 260  continue

C     Save nonzero element of the  N-th column.
      IF(DABS(VAL1).LE.SMALLA) GO TO 280
      COLLEN=COLLEN+1
C
C     Check if there is another nonzero read in the analysed line.
  280 IF(NAMRW2.NE.'        ') THEN
         NAMRW1=NAMRW2
         VAL1=VAL2
         NAMRW2='        '
         GO TO 260
      ELSE
         GO TO 220
      ENDIF

C
C     Here when error occurs.
 9000 RCODE=1
      RETURN
C
 9050 RCODE=2
      RETURN
C *** LAST CARD OF (RDMPS1) ***
      END

