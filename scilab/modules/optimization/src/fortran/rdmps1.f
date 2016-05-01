
C****************************************************
C     ****  RDMPS1 ... READ THE  MPS FILE  ****
C****************************************************
      SUBROUTINE rdmps1(RCODE,buffer,MAXM,MAXN,MAXNZA,
     X M,N,NZA,IROBJ,BIG,DLOBND,DUPBND,
     X NAMEC,NAMEB,NAMRAN,NAMBND,NAMMPS,inmps,
     X RWNAME,CLNAME,STAVAR,RWSTAT,
     X HDRWCD,LNKRW,HDCLCD,LNKCL,
     X RWNMBS,CLPNTS,IROW,
     X ACOEFF,RHSB,RANGES,
     X UPBND,LOBND,RELT)
C
C *** PARAMETERS
      INTEGER*4 RCODE,MAXM,MAXN,MAXNZA,M,N,NZA,IROBJ
      DOUBLE PRECISION BIG,DLOBND,DUPBND
      CHARACTER*(*) NAMEC,NAMEB,NAMRAN,NAMBND,NAMMPS
      CHARACTER*(*) BUFFER
      CHARACTER*8 RWNAME(MAXM),CLNAME(MAXN)
      INTEGER*4 STAVAR(*),RWSTAT(*),RWNMBS(*)
      INTEGER*4 HDRWCD(*),LNKRW(*)
      INTEGER*4 HDCLCD(*),LNKCL(*)
      INTEGER*4 CLPNTS(*),IROW(*)
      DOUBLE PRECISION ACOEFF(*),RHSB(*),RANGES(*)
      DOUBLE PRECISION UPBND(*),LOBND(*),RELT(*)
C
C
C
C *** PARAMETERS DESCRIPTION
C     RCODE   Return code:
C             0   Everything OK;
C             21  Number of constraints exceeds MAXM.
C             22  Number of variables   exceeds MAXN.
C             23  Number of nonzeros    exceeds MAXNZA.
C             83  Error in MPS file (in RHSB or RANGES).
C             84  Error in MPS file (in ROWS, COLUMNS or BOUNDS).
C             86  Unable to open the MPS file.
C     MAXM    Maximum number of constraints.
C     MAXN    Maximum number of variables.
C     MAXNZA  Maximum number of nonzeros of the LP constraint matrix.
C     M       Current number of constraints.
C     N       Current number of variables.
C     NZA     Current number of nonzeros of the LP constraint matrix.
C     IROBJ   Index of the objective row.
C     BIG     "Big" number.
C     DUPBND  Default UPPER bound.
C     DLOBND  Default LOWER bound.
C     NAMEC   Name of the objective row.
C     NAMEB   Name of the right hand side section.
C     NAMRAN  Name of the ranges section.
C     NAMBND  Name of the bounds section.
C     NAMMPS  Name of the  LP problem.
C     FILMPS  Name of the MPS input file.
C     RWNAME  Array of row names.
C     CLNAME  Array of column names.
C     STAVAR  Work array for (local) variable status.
C     RWSTAT  Array of row types:
C             1  row type is = ;
C             2  row type is >= ;
C             3  row type is <= ;
C             4  objective row;
C             5  other free row.
C     HDRWCD  Header to the linked list of rows with the same codes.
C     LNKRW   Linked list of rows with the same codes.
C     HDCLCD  Header to the linked list of columns with the same codes.
C     LNKCL   Linked list of columns with the same codes.
C     RWNMBS  Row numbers of nonzeros in columns of matrix A.
C     CLPNTS  Pointers to the beginning of columns of matrix A.
C     IROW    Integer work array.
C     ACOEFF  Array of nonzero elements for each column.
C     RHSB     Right hand side of the linear program.
C     RANGES  Array of constraint ranges.
C     UPBND   Array of upper bounds.
C     LOBND   Array of lower bounds.
C     RELT    Real work array.
C
C
C
C *** LOCAL VARIABLES
      INTEGER*4 LINE,I,INMPS,J,COLLEN,INDEX,IPOS,STATUS,NSTRCT,KCODE
      INTEGER*4 IMPSOK
      DOUBLE PRECISION SMALLA,VAL1,VAL2
      CHARACTER*8 NAME0,NAMRW1,NAMRW2,NAMCLN
      CHARACTER*2 TYPROW,BNDTYP
      CHARACTER*4 NM
      CHARACTER*100 RDLINE
      CHARACTER SECT
C
C
C
C *** PURPOSE
C     This routine reads the  MPS input file.
C
C *** SUBROUTINES CALLED
C     LKINDX,RDRHS,LKCODE
C
C *** NOTES
C
C
C *** REFERENCES:
C     Altman A., Gondzio J. (1993). An efficient implementation of
C        a higher order primal-dual interior point method for large
C        sparse linear programs, Archives of Control Sciences 2,
C        No 1-2, pp. 23-40.
C     Altman A., Gondzio J. (1993). HOPDM - A higher order primal-
C        dual method for large scale linear programmming, European
C        Journal of Operational Research 66 (1993) pp 158-160.
C     Gondzio J., Tachat D. (1994). The design and application of
C        IPMLO - a FORTRAN library for linear optimization with
C        interior point methods, RAIRO Recherche Operationnelle 28,
C        No 1, pp. 37-56.
C     Murtagh B. (1981). Advanced Linear Programming, McGrew-Hill,
C        New York, 1981.
C     Murtagh B., Saunders M. (1983). MINOS 5.0 User's guide,
C        Technical Report SOL 83-20, Department of Operations Research,
C        Stanford University, Stanford, 1983.
C
C *** HISTORY:
C     Written by:    Jacek Gondzio, Systems Research Institute,
C                    Polish Academy of Sciences, Newelska 6,
C                    01-447 Warsaw, Poland.
C     Date written:  November 15, 1992
C     Last modified: February 8, 1997
C     DIGITEO - Michael Baudin, 06/2011: Ignore blank lines
C
C
C *** BODY OF (RDMPS1) ***
C
      SMALLA=1.0D-10
C
C     Format used to read every line of the MPS file.
 1000 FORMAT(A80)
C
C
C     Initialize.
      M=0
      LINE=0
      IROBJ=-1
C


      DO 20 I=1,MAXM
         RWNAME(I)=' '
         RWSTAT(I)=0
   20 CONTINUE
C

C     Initialize linked lists of rows/cols with the same codes.
      DO 40 I=1,MAXM
         HDRWCD(I)=0
         LNKRW(I)=0
   40 CONTINUE
      DO 50 J=1,MAXN
         HDCLCD(J)=0
         LNKCL(J)=0
   50 CONTINUE
C
C
C
C     Read the problem name.
   60 LINE=LINE+1
      READ(INMPS,1000,END=9000) RDLINE
      IF(RDLINE(1:1).EQ.'*'.OR.  LNBLNK(RDLINE).EQ.0) GO TO 60
      READ(RDLINE,61,ERR=9000) NM,NAMMPS
   61 FORMAT(A4,10X,A8)
      IF(NM.NE.'NAME'.AND.NM.NE.'name') GO TO 60

   70 LINE=LINE+1
      READ(INMPS,1000,END=9000) RDLINE
      IF(RDLINE(1:1).EQ.'*'.OR. LNBLNK(RDLINE).EQ.0) GO TO 70
      READ(RDLINE,71,ERR=9000) SECT
   71 FORMAT(A1)
      IF(SECT.NE.'R'.AND.SECT.NE.'r') GO TO 9000
C
C
C

C
C     Read the ROWS section.
  100 LINE=LINE+1
      READ(INMPS,1000,END=9000) RDLINE
      IF(RDLINE(1:1).EQ.'*'.OR. LNBLNK(RDLINE).EQ.0) GO TO 100
      READ(RDLINE,101,ERR=9000) SECT,TYPROW,NAMRW1
  101 FORMAT(A1,A2,1X,A8)
      IF(SECT.NE.' ') GO TO 200
C
C     Here if a constraint has been found. Determine its type.
C     Check if there is enough space for a new row.
      M=M+1
css      IF(M.GE.MAXM) GO TO 9010
      IF(M.GT.MAXM) GO TO 9010
C
      IF(TYPROW.EQ.' E'.OR.TYPROW.EQ.'E '.OR.
     X   TYPROW.EQ.' e'.OR.TYPROW.EQ.'e ') THEN
         RWSTAT(M)=1
         GO TO 120
      ENDIF
C
      IF(TYPROW.EQ.' G'.OR.TYPROW.EQ.'G '.OR.
     X   TYPROW.EQ.' g'.OR.TYPROW.EQ.'g ') THEN
         RWSTAT(M)=2
         GO TO 120
      ENDIF
C
      IF(TYPROW.EQ.' L'.OR.TYPROW.EQ.'L '.OR.
     X   TYPROW.EQ.' l'.OR.TYPROW.EQ.'l ') THEN
         RWSTAT(M)=3
         GO TO 120
      ENDIF
C
      IF(TYPROW.EQ.' N'.OR.TYPROW.EQ.'N '.OR.
     X   TYPROW.EQ.' n'.OR.TYPROW.EQ.'n ') THEN
         IF(NAMRW1.EQ.NAMEC(1:8)) THEN
C
C     Save index of the objective row.
            IROBJ=M
            RWSTAT(M)=4
         ELSE
            RWSTAT(M)=5
C
C     The first free row is a default objective.
            IF(NAMEC(1:8).EQ.'        ') THEN
               IROBJ=M
               RWSTAT(M)=4
               NAMEC(1:8)=NAMRW1
            ENDIF
         ENDIF
         GO TO 120
      ENDIF
C
C     Invalid row type.
      GO TO 9050
C
C     Here to save the row name.
  120 RWNAME(M)=NAMRW1
C
C     Continue reading of the  ROWS section.
      GO TO 100
C
C
C
C
C
C
C     Read COLUMNS section.
  200 CONTINUE

      INDEX=1
C
C     ENCODE all row names and create linked lists of rows
C     with the same codes.
      IMPSOK=1
      DO 210 I=1,M
         CALL MYCODE(IOLOG,RWNAME(I),KCODE,M)
         LNKRW(I)=HDRWCD(KCODE)
         HDRWCD(KCODE)=I
C
C     Check for multiple row definition (February 10, 1996).
C     Scan all rows with the same code.
         IPOS=LNKRW(I)
  205    IF(IPOS.EQ.0) GO TO 210
            IF(RWNAME(IPOS).EQ.RWNAME(I)) THEN
               WRITE(BUFFER,206) RWNAME(IPOS)
  206          FORMAT(1X,'RDMPS1 error: Row ',A8,'repeated.')
C               CALL basout(io,wte,BUFFER)
               IMPSOK=0
               GO TO 210
            ENDIF
         IPOS=LNKRW(IPOS)
         GO TO 205
  210 CONTINUE
      IF(IMPSOK.EQ.0) GO TO 9400
C
      IF(SECT.NE.'C'.AND.SECT.NE.'c') GO TO 9000
      NAME0='        '
  220 LINE=LINE+1
      READ(INMPS,1000,END=9000) RDLINE
      IF(RDLINE(1:1).EQ.'*'.OR. LNBLNK(RDLINE).EQ.0) GO TO 220
      READ(RDLINE,221,ERR=9000) SECT,NAMCLN,NAMRW1,VAL1,NAMRW2,VAL2
  221 FORMAT(A1,3X,A8,2X,A8,2X,D12.0,3X,A8,2X,D12.0)
C
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
      IF(NZA+COLLEN.GT.MAXNZA) GO TO 9020
C
      CLPNTS(N)=NZA+1
      CLNAME(N)=NAME0
      DO 240 I=1,COLLEN
         IPOS=NZA+I
         RWNMBS(IPOS)=IROW(I)
         ACOEFF(IPOS)=RELT(I)
  240 CONTINUE
      NZA=NZA+COLLEN
C
C     Check if there are still columns to be read.
      IF(SECT.NE.' ') THEN
         CLPNTS(N+1)=NZA+1
         NSTRCT=N
         GO TO 300
      ELSE
C
C     Initialize the new column.
         N=N+1
css         IF(N.GE.MAXN) GO TO 9030
         IF(N.GT.MAXN) GO TO 9030
         NAME0=NAMCLN
         COLLEN=0
         GO TO 260
      ENDIF
C
C
C     Find the position of the nonzero element.
C 260 CALL LKINDX(RWNAME,M,NAMRW1,INDEX)
  260 CALL LKCODE(RWNAME,M,NAMRW1,INDEX,HDRWCD,LNKRW,IOLOG)
      IF(INDEX.EQ.0) GO TO 9040
C
C
C     Save nonzero element of the  N-th column.
      IF(DABS(VAL1).LE.SMALLA) GO TO 280
      COLLEN=COLLEN+1
      IROW(COLLEN)=INDEX
      RELT(COLLEN)=VAL1
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
C
C
C
C     Initialize RHSB and RANGES arrays.
  300 DO 320 I=1,MAXM
         RHSB(I)=0.0
         RANGES(I)=BIG
  320 CONTINUE
C
C
C
C     Set the default bounds for all structural variables.
      DO 520 J=1,MAXN
         STAVAR(J)=0
         LOBND(J)=DLOBND
         UPBND(J)=DUPBND
  520 CONTINUE
C
C
C
C
C
C
C     Read the RHSB section.
C
      IF(SECT.NE.'R'.AND.SECT.NE.'r') GO TO 9000
      CALL RDRHS(RCODE,BUFFER,MAXM,M,LINE,
     X HDRWCD,LNKRW,HDCLCD,LNKCL,
     X NAMEB,RHSB,RWNAME,SECT,INMPS,IOLOG)
C
      IF(RCODE.GT.0) GO TO 6000
C
C
C
C
C     Check if there is a  RANGES section to be read.
      IF(SECT.NE.'R'.AND.SECT.NE.'r') GO TO 400
C
C
C
C
C
C
C     Read the RANGES section.
C
      CALL RDRHS(RCODE,BUFFER,MAXM,M,LINE,
     X HDRWCD,LNKRW,HDCLCD,LNKCL,
     X NAMRAN,RANGES,RWNAME,SECT,INMPS,IOLOG)
C
      IF(RCODE.GT.0) GO TO 6000
C
C
C
  400 CONTINUE
      IF(SECT.NE.'B'.AND.SECT.NE.'b') GO TO 600
C
C
C
C
C
C
C     Read the BOUNDS section.
C
      INDEX=1
  550 LINE=LINE+1
      READ(INMPS,1000,END=9000) RDLINE
      IF(RDLINE(1:1).EQ.'*'.OR. LNBLNK(RDLINE).EQ.0) GO TO 550
C
C     ENCODE all column names and create linked lists of columns
C     with the same codes.
C     DO 560 J=1,N
C        LNKCL(J)=HDCLCD(KCODE)
C        HDCLCD(KCODE)=J
C 560 CONTINUE
C
      READ(RDLINE,561,ERR=9000) SECT,BNDTYP,NAME0,NAMCLN,VAL1
  561 FORMAT(A1,A2,1X,A8,2X,A8,2X,D12.0)
C
      IF(SECT.NE.' ') GO TO 600
C
C     First record met defines default section name.
      IF(NAMBND(1:8).EQ.'        ') THEN
         NAMBND(1:8)=NAME0
      ENDIF
C
C     Ignore the record that define unimportant bound.
      IF(NAME0.NE.NAMBND(1:8)) GO TO 550
C
C     Determine index of the variable to which the bound refers.
      CALL LKINDX(CLNAME,N,NAMCLN,INDEX)
C     CALL LKCODE(CLNAME,N,NAMCLN,INDEX,HDCLCD,LNKCL,IOLOG)
      IF(INDEX.EQ.0) GO TO 9060
C
C
C     Here to detect the type of the bound read.
      STATUS=STAVAR(INDEX)
C
C
C
      IF(BNDTYP.EQ.'UP'.OR.BNDTYP.EQ.'up') THEN
C
C     Here when an UPPER bound is being defined.
C     Accept multiple definition of the UPPER bound.
C     The last definition is valid.
         IF(STATUS.EQ.6) GO TO 9070
         IF(STATUS.EQ.-1) GO TO 9080
C
         IF(STATUS.EQ.0.OR.STATUS.EQ.1) THEN
C
C     Not yet bounded variable (or multiple UPPER bound).
            UPBND(INDEX)=VAL1
            STAVAR(INDEX)=1
            GO TO 550
         ENDIF
C
         IF(STATUS.EQ.2.OR.STATUS.EQ.3) THEN
C
C     Already LOWER bounded variable.
            UPBND(INDEX)=VAL1
            STAVAR(INDEX)=3
            GO TO 550
         ENDIF
C
      ENDIF
C
C
C
      IF(BNDTYP.EQ.'LO'.OR.BNDTYP.EQ.'lo') THEN
C
C     Here when a LOWER bound is being defined.
         IF(STATUS.EQ.2.OR.STATUS.EQ.3.OR.STATUS.EQ.6) GO TO 9070
         IF(STATUS.EQ.-1) GO TO 9080
C
         IF(STATUS.EQ.0) THEN
C
C     Not yet bounded variable.
            LOBND(INDEX)=VAL1
            STAVAR(INDEX)=2
            GO TO 550
         ENDIF
C
         IF(STATUS.EQ.1) THEN
C
C     Already UPPER bounded variable.
            LOBND(INDEX)=VAL1
            STAVAR(INDEX)=3
            GO TO 550
         ENDIF
C
      ENDIF
C
C
C
      IF(BNDTYP.EQ.'FR'.OR.BNDTYP.EQ.'fr') THEN
C
C     Here when a FREE variable is being defined.
         IF(STATUS.GT.0) GO TO 9090
C
C     Not yet bounded variable.
         LOBND(INDEX)=-BIG
         UPBND(INDEX)=BIG
         STAVAR(INDEX)=-1
         GO TO 550
C
      ENDIF
C
C
C
      IF(BNDTYP.EQ.'FX'.OR.BNDTYP.EQ.'fx') THEN
C
C     Here when a FIXED variable is being defined.
         IF(STATUS.EQ.-1) GO TO 9080
         IF(STATUS.NE.0) GO TO 9100
C
C     Not yet bounded variable.
         LOBND(INDEX)=VAL1
         UPBND(INDEX)=VAL1
         STAVAR(INDEX)=6
         GO TO 550
C
      ENDIF
C
C
C
      IF(BNDTYP.EQ.'PL'.OR.BNDTYP.EQ.'pl') THEN
C
C     Here when a PLUS INFINITY bound is being defined.
         IF(STATUS.EQ.-1) GO TO 9080
         IF(STATUS.NE.0) GO TO 9070
C
C     Not yet bounded variable.
C        LOBND(INDEX)=VAL1
         UPBND(INDEX)=BIG
         STAVAR(INDEX)=2
         GO TO 550
C
      ENDIF
C
C
C
      IF(BNDTYP.EQ.'MI'.OR.BNDTYP.EQ.'mi') THEN
C
C     Here when a MINUS INFINITY bound is being defined.
         IF(STATUS.EQ.-1) GO TO 9080
         IF(STATUS.NE.0) GO TO 9070
C
C     Not yet bounded variable.
         LOBND(INDEX)=-BIG
C        UPBND(INDEX)=VAL1
         STAVAR(INDEX)=1
         GO TO 550
C
      ENDIF
C
      GO TO 9110
C
C
C
  600 CONTINUE
      IF(SECT.NE.'E'.AND.SECT.NE.'e') GO TO 9000
C
C
C
C
C
C
C     The ENDATA card has been found.
C
      IF(IROBJ.EQ.-1) GO TO 9130
 5000 CONTINUE
      RCODE=0
C
 6000 CONTINUE
C     Close the MPS input file.
css      call clunit(-inmps,filmps(1:ilen),mode)
c      CLOSE(INMPS)
      RETURN
C
C
C
C
C
C     Here when error occurs.
 9000 WRITE(BUFFER,9001) LINE
 9001 FORMAT(1X,'RDMPS1: Error while reading line',I10,
     X     ' of the MPS file.')
css      CALL basout(io,wte,BUFFER)
      RCODE=84
      GO TO 6000
C
 9010 WRITE(BUFFER,9011)
 9011 FORMAT(1X,'RDMPS1 ERROR: Number of constraints',
     X ' in the MPS file exceeds MAXM.')
css      CALL basout(io,wte,BUFFER)
      RCODE=21
      GO TO 6000
C
 9020 WRITE(BUFFER,9021)
 9021 FORMAT(1X,'RDMPS1 ERROR: Number of nonzeros',
     X ' of matrix A exceeds MAXNZA.')
css      CALL basout(io,wte,BUFFER)
      RCODE=23
      GO TO 6000
C
 9030 WRITE(BUFFER,9031)
 9031 FORMAT(1X,'RDMPS1 ERROR: Number of variables',
     X ' in the MPS file exceeds MAXN.')
css      CALL basout(io,wte,BUFFER)
      RCODE=22
      GO TO 6000
C
 9040 WRITE(BUFFER,9041) LINE
 9041 FORMAT(1X,'RDMPS1 ERROR: Unknown row found',
     X ' at line',I10,' of the MPS file.')
css      CALL basout(io,wte,BUFFER)
      RCODE=84
      GO TO 6000
C
 9050 WRITE(BUFFER,9051) TYPROW,LINE
 9051 FORMAT(1X,'RDMPS1 ERROR: Unknown row type=',A2,
     X ' at line',I10,' of the MPS file.')
css      CALL basout(io,wte,BUFFER)
      RCODE=84
      GO TO 6000
C
 9060 WRITE(BUFFER,9061) LINE
 9061 FORMAT(1X,'RDMPS1 ERROR: Unknown column found',
     X ' at line',I10,' of the MPS file.')
css      CALL basout(io,wte,BUFFER)
      RCODE=84
      GO TO 6000
C
 9070 WRITE(BUFFER,9071) LINE,BNDTYP
 9071 FORMAT(1X,'RDMPS1 ERROR: Line',I10,' in MPS file',
     X ' defines ',A2,' bound')
css      CALL basout(io,wte,BUFFER)
      WRITE(BUFFER,9072) NAMCLN
 9072 FORMAT(14X,'for variable ',A8,
     X ' that has already been bounded.')
css      CALL basout(io,wte,BUFFER)
      RCODE=84
      GO TO 6000
C
 9080 WRITE(BUFFER,9081) LINE,BNDTYP
 9081 FORMAT(1X,'RDMPS1 ERROR: Line',I10,' in MPS file',
     X ' defines ',A2,' bound')
      CALL basout(io,wte,BUFFER)
      WRITE(BUFFER,9082) NAMCLN
 9082 FORMAT(14X,'for variable ',A8,
     X ' that has earlier been declared FREE.')
css      CALL basout(io,wte,BUFFER)
      RCODE=84
      GO TO 6000
C
 9090 WRITE(BUFFER,9091) LINE
 9091 FORMAT(1X,'RDMPS1 ERROR: Line',I10,' in MPS file',
     X ' declares as  FREE')
css      CALL basout(io,wte,BUFFER)
      WRITE(BUFFER,9092) NAMCLN
 9092 FORMAT(14X,' variable ',A8,
     X ' that has earlier been bounded.')
css      CALL basout(io,wte,BUFFER)
      RCODE=84
      GO TO 6000
C
 9100 WRITE(BUFFER,9101) LINE,NAMCLN
 9101 FORMAT(1X,'RDMPS1 ERROR: Line',I10,' in MPS file',
     X     ' declares as  FIXED',14X,' variable ',A8,
     X     ' that has earlier been bounded.')
css      CALL basout(io,wte,BUFFER)
css      WRITE(BUFFER,9102) NAMCLN
css 9102 FORMAT(14X,' variable ',A8,
css     X ' that has earlier been bounded.')
css      CALL basout(io,wte,BUFFER)
      RCODE=84
      GO TO 6000
C
 9110 WRITE(BUFFER,9111) LINE,BNDTYP
 9111 FORMAT(1X,'RDMPS1 ERROR: Line',I10,' in MPS file',
     X ' has invalid bound type ',A2)
css      CALL basout(io,wte,BUFFER)
      RCODE=84
      GO TO 6000
C
 9130 WRITE(BUFFER,9131) NAMEC(1:8)
 9131 FORMAT(1X,'RDMPS1 ERROR: Objective row =',A8,
     X ' has no entries.')
css    CALL basout(io,wte,BUFFER)
      RCODE=84
      GO TO 6000

C
 9400 WRITE(BUFFER,9401)
 9401 FORMAT(1X,'RDMPS1 ERROR: Multiple row definition.')
css      CALL basout(io,wte,BUFFER)
      RCODE=84
      GO TO 6000
C *** LAST CARD OF (RDMPS1) ***
      END
C******************************************************************
      SUBROUTINE LKCODE(RWNAME,M,NAME,INDEX,HEADER,LINKS,IOLOG)
C
      INTEGER*4 KCODE,M,I,INDEX,IOLOG

      INTEGER*4 HEADER(M),LINKS(M)
      CHARACTER*8 RWNAME(M),NAME
C
C     Get code of the NAME.
      CALL MYCODE(IOLOG,NAME,KCODE,M)
      INDEX=HEADER(KCODE)
C
C     Determine the index such that   RWNAME(index) = NAME.
      DO 100 I=1,M
         IF(INDEX.EQ.0) GO TO 200
         IF(RWNAME(INDEX).EQ.NAME) GO TO 200
         INDEX=LINKS(INDEX)
  100 CONTINUE
C
  200 CONTINUE
      RETURN
      END
C*******************************************************************
      SUBROUTINE LKINDX(RWNAME,M,NAME,INDEX)
C
      INTEGER*4 M,I,INDEX,INDEX2
      CHARACTER*8 RWNAME(M),NAME
C
      INDEX2=INDEX
C     WRITE(0,10) INDEX
C  10 FORMAT(1X,' old index=',I5)
      INDEX=0
      DO 100 I=INDEX2,M
         IF(RWNAME(I).EQ.NAME) THEN
            INDEX=I
            GO TO 200
         ENDIF
  100 CONTINUE
      DO 150 I=1,INDEX2
         IF(RWNAME(I).EQ.NAME) THEN
            INDEX=I
            GO TO 200
         ENDIF
  150 CONTINUE
C
  200 CONTINUE
      RETURN
      END
C********************************************************************
C     ******* RDRHS ... READ THE RHS SECTION OF THE MPS FILE *******
C********************************************************************
C
      SUBROUTINE RDRHS(RCODE,BUFFER,MAXM,M,LINE,
     X HDRWCD,LNKRW,HDCLCD,LNKCL,
     X NAMEB,RRHS,RWNAME,SECT,INMPS,IOLOG)
C
C
C
C *** PARAMETERS
      INTEGER*4 RCODE,MAXM,M,LINE,INMPS,IOLOG
      CHARACTER*8 NAMEB,RWNAME(MAXM)
      INTEGER*4 HDRWCD(M+1),LNKRW(M+1)
      INTEGER*4 HDCLCD(M+1),LNKCL(M+1)
      DOUBLE PRECISION RRHS(MAXM)
      CHARACTER*100 BUFFER
      CHARACTER SECT
C
C
C
C *** LOCAL VARIABLES
      INTEGER*4 INDEX
      DOUBLE PRECISION VAL1,VAL2
      CHARACTER*8 NAME0,NAMRW1,NAMRW2
      CHARACTER*100 RDLINE
C
C
C
C *** PARAMETERS DESCRIPTION
C     ON INPUT:
C     MAXM    Maximum number of constraints.
C     M       Current number of constraints.
C     LINE    Current number of the line read from the  MPS file.
C     NAMEB   The name of the right hand side section chosen.
C     RWNAME  Array of row names.
C     HDRWCD  Header to the linked list of rows with the same codes.
C     LNKRW   Linked list of rows with the same codes.
C     HDCLCD  Header to the linked list of columns with the same codes.
C     LNKCL   Linked list of columns with the same codes.
C     IOLOG   Output unit number where log messages are to be written.
C     INMPS   Input unit number where the input MPS file is read from.
C
C     ON OUTPUT:
C     RCODE   Return code:
C             0   Everything OK;
C             83  Error in MPS file (in RRHS or RANGES section).
C     RRHS     The right hand side vector.
C     SECT    Indicator of the section that follows  RRHS one.
C
C
C
C *** SUBROUTINES CALLED
C     LKINDX
C
C
C
C *** PURPOSE
C     This routine reads the  RRHS section of the  MPS file.
C     (It can also be used to read the  RANGES section).
C
C
C
C *** NOTES
C
C
C
C *** REFERENCES:
C     Altman A., Gondzio J. (1993). An efficient implementation of
C        a higher order primal-dual interior point method for large
C        sparse linear programs, Archives of Control Sciences 2,
C        No 1-2, pp. 23-40.
C     Altman A., Gondzio J. (1993). HOPDM - A higher order primal-
C        dual method for large scale linear programmming, European
C        Journal of Operational Research 66 (1993) pp 158-160.
C     Gondzio J., Tachat D. (1994). The design and application of
C        IPMLO - a FORTRAN library for linear optimization with
C        interior point methods, RAIRO Recherche Operationnelle 28,
C        No 1, pp. 37-56.
C
C
C
C *** HISTORY:
C     Written by:    Jacek Gondzio, Systems Research Institute,
C                    Polish Academy of Sciences, Newelska 6,
C                    01-447 Warsaw, Poland.
C     Last modified: February 8, 1997
C
C
C
C *** BODY OF (RDRHS) ***
C
C     Format used to read every line of the MPS file.
 1000 FORMAT(A80)
C
C
C
C
C     Main loop begins here.
  200 LINE=LINE+1
      READ(INMPS,1000,ERR=9000) RDLINE
      IF(RDLINE(1:1).EQ.'*'.OR. LNBLNK(RDLINE).EQ.0) GO TO 200
      INDEX=1
      READ(RDLINE,201,ERR=9000) SECT,NAME0,NAMRW1,VAL1,NAMRW2,VAL2
  201 FORMAT(A1,3X,A8,2X,A8,2X,D12.0,3X,A8,2X,D12.0)
C
C     Check if the line belongs to the same section.
      IF(SECT.NE.' ') GO TO 300
C
C     First record met defines default section name.
      IF(NAMEB.EQ.'        ') THEN
         NAMEB=NAME0
      ENDIF
      IF(NAME0.NE.NAMEB) GO TO 9000
C
C
C     Find the position of the nonzero element.
C 250 CALL LKINDX(RWNAME,M,NAMRW1,INDEX)
  250 CALL LKCODE(RWNAME,M,NAMRW1,INDEX,HDRWCD,LNKRW,IOLOG)
      IF(INDEX.EQ.0) GO TO 9010
C
C     Save the  RRHS coefficient.
      RRHS(INDEX)=VAL1
C     WRITE(BUFFER,251) INDEX,RWNAME(INDEX),VAL1
C 251 FORMAT(1X,'RDRHS: rw=',I6,'  rwname=',A8,'  elt=',D14.6)
C     CALL MYWRT(IOLOG,BUFFER)
C
C     Check if there is another nonzero read in the analysed line.
      IF(NAMRW2.NE.'        ') THEN
         NAMRW1=NAMRW2
         VAL1=VAL2
         NAMRW2='        '
         GO TO 250
      ELSE
         GO TO 200
      ENDIF
C
C
C
  300 CONTINUE
      RCODE=0
C
 6000 CONTINUE
      RETURN
C
C
C
C     Here if an error occurs.
 9000 WRITE(BUFFER,9001) LINE
 9001 FORMAT(1X,'RDRHS ERROR: Unexpected characters found',
     X ' at line',I10,' of the MPS file.')
css      CALL basout(io,wte,BUFFER)
      RCODE=83
      GO TO 6000
C
 9010 WRITE(BUFFER,9011) LINE
 9011 FORMAT(1X,'RDRHS ERROR: Unknown row was found',
     X ' at line',I10,' of the MPS file.')
css      CALL basout(io,wte,BUFFER)
      RCODE=83
      GO TO 6000
C
C
C
C *** LAST CARD OF (RDRHS) ***
      END

C*******************************************************************
C     **  MYCODE ... ENCODE THE 8-CHARACTER NAME INTO AN INTEGER  **
C*******************************************************************
C
      SUBROUTINE MYCODE(IOLOG,NAME,KCODE,M)
C
C
C *** PARAMETERS
      CHARACTER*9 NAME
      INTEGER*4 IOLOG,KCODE,M
C
C
C *** LOCAL VARIABLES
      INTEGER*4 IPOS
C
C
C *** PARAMETERS DESCRIPTION
C     NAME    8-character name (row or column name).
C     KCODE   Integer code associated to the name.
C     M       The number of rows (or columns) in matrix A.
C     IOLOG   Output unit number where log messages are to be written.
C
C *** HISTORY:
C     Written by:    Jacek Gondzio, Systems Research Institute,
C                    Polish Academy of Sciences, Newelska 6,
C                    01-447 Warsaw, Poland.
C     Date written:  October 14, 1994
C     Last modified: May 17, 1995
C
C
C *** BODY OF (MYCODE) ***
C
C
      KCODE=0
      DO 100 IPOS=1,8
         KCODE=KCODE+ICHAR(NAME(IPOS:IPOS))*IPOS
C        WRITE(BUFFER,101) IPOS,NAME(IPOS:IPOS)
C 101    FORMAT(1X,'ipos=',I2,'  char=',A1)
C        CALL MYWRT(IOLOG,BUFFER)
  100 CONTINUE
      KCODE=MOD(KCODE,M)+1
C     WRITE(BUFFER,102) NAME,KCODE
C 102 FORMAT(1X,'  name=',A8,'  has a code=',I6)
C     CALL MYWRT(IOLOG,BUFFER)
      RETURN
C
C
C *** LAST CARD OF (MYCODE) ***
      END

