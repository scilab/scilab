c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
