c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      SUBROUTINE DLBLKS( NAME, NBC)
C ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
C
C        ENLEVER LES BLANCS EN DEBUT D'UNE CHAINE DE CARACTERES
C
C
C ENTREE : NAME  LA CHAINE DE CARACTERES
C
C SORTIE : NAME  LA CHAINE SANS LES BLANCS
C          NBC   NOMBRE DE CARACTERES DE LA CHAINE
C
C FORTRAN  INDEX, LEN
C ....................................................................
c
      CHARACTER*(*) NAME
      INTEGER       NBC
      INTEGER       I,K,J,LL
C
      LL = LEN ( NAME )
      I  =  0
    1 I  =  I + 1
         K  = INDEX ( NAME(I:LL) , ' ' )
         IF ( K .EQ. 0 ) K = LL - I + 2
         IF ( K .EQ. 1  .AND.  I .LT. LL ) GO TO 1
C
      NBC = K - 1
      I   = I - 1
      DO 3 J = 1,NBC
         NAME(J:J) = NAME(J+I:J+I)
    3 CONTINUE
      DO 5 J = NBC+1,LL
         NAME(J:J) = ' '
    5 CONTINUE
C
C
      END
