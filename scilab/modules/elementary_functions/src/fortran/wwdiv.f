c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) Bruno Pincon
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine wwdiv(ar, ai, br, bi, cr, ci, ierr)
*
*     PURPOSE
*        complex division algorithm : compute c := a / b
*        where :
*       
*        a = ar + i ai
*        b = br + i bi
*        c = cr + i ci
*
*        inputs  : ar, ai, br, bi  (double precision)
*        outputs : cr, ci          (double precision)
*                  ierr  (integer) ierr = 1 if b = 0 (else 0)
*
*     IMPLEMENTATION NOTES
*        1/ Use scaling with ||b||_oo; the original wwdiv.f used a scaling
*           with ||b||_1.  It results fewer operations.  From the famous
*           Golberg paper.  This is known as Smith's method.
*        2/ Currently set c = NaN + i NaN in case of a division by 0 ;
*           is that the good choice ?
*
*     AUTHOR
*        Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
*
      implicit none
      
*     PARAMETERS
      double precision ar, ai, br, bi, cr, ci
      integer ierr

*     LOCAL VARIABLES
      double precision r, d

*     TEXT
      ierr = 0

*     Treat special cases
      if (bi .eq. 0d0) then
         if (br .eq. 0d0) then	
            ierr = 1	
*           got NaN + i NaN
            cr = bi / br
            ci = cr
         else
            cr = ar / br
            ci = ai / br
         endif
      elseif (br .eq. 0d0) then
         cr = ai / bi
         ci = (-ar) / bi
      else
*     Generic division algorithm
         if (abs(br) .ge. abs(bi)) then
            r = bi / br
            d = br + r*bi
            cr = (ar + ai*r) / d
            ci = (ai - ar*r) / d
         else
            r = br / bi
            d = bi + r*br
            cr = (ar*r + ai) / d
            ci = (ai*r - ar) / d
         endif
      endif
      
      end


