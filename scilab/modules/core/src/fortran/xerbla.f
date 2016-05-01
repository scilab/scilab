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

      SUBROUTINE XERBLA( SRNAME, INFO )
      
c      INCLUDE 'stack.h'
*    XERBLA CUSTOMIZED FOR SCILAB
*      
*
*  -- LAPACK auxiliary routine (version 3.0) --
*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
*     Courant Institute, Argonne National Lab, and Rice University
*     September 30, 1994
*
*     .. Scalar Arguments ..
      CHARACTER*6        SRNAME
      INTEGER            INFO
*     ..
*
*  Purpose
*  =======
*
*  XERBLA  is an error handler for the LAPACK routines.
*  It is called by an LAPACK routine if an input parameter has an
*  invalid value.  A message is printed and execution stops.
*
*  Installers may consider modifying the STOP statement in order to
*  call system-specific exception-handling facilities.
*
*  Arguments
*  =========
*
*  SRNAME  (input) CHARACTER*6
*          The name of the routine which called XERBLA.
*
*  INFO    (input) INTEGER
*          The position of the invalid parameter in the parameter list
*          of the calling routine.
*
* =====================================================================
*
*     .. Executable Statements ..
*
      character bufstr*(4096)
*     
      WRITE(bufstr, FMT = 9999 ) SRNAME(1:lnblnk(SRNAME)), INFO
      
*
 9999 FORMAT( 'xerbla: On entry to ', A6, ' parameter number ', I2,
     $ ' had ',
     $ 'an illegal value (lapack library problem)' )
     

      call basout(io ,wte ,bufstr(1:lnblnk(bufstr)))
      call error(998)
*
*     End of XERBLA
*
      END
