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

      subroutine formatnumber(a,ifmt,maxc,str,fl)
c maxc: Maximum character authorized
      double precision a
      integer n1,n2,fl
      character str*(*)
      character*10 form
      if (ifmt .eq. 1) then
         fl = maxc
         n2 = 1
         write(form,130) maxc,maxc-7
         write (str,form) a
         n1=floor(log10(a))
         if(n1.ge.100.or.n1.le.-100) then
C     .     Workaround to add the exponent mark "D" that fortran omits
C     .     for denormalized numbers. To preserve the max length the
C     .     exponent mark is put in place of the last digit
            i=maxc+1
 10         i=i-1
            if(i.eq.0) goto 20
            if (str(i:i).eq.'+'.or.str(i:i).eq.'-') then
               i=i-1
               str(i:i)='D'
               goto 20
            endif
            goto 10
 20         continue
         endif
       elseif (ifmt .ge. 0) then
          n1 = ifmt / 32
c         bug 8420 check values n1, n2
          if (n1.le.0) then
            n1 = 1
          endif
          n2 = ifmt - 32*n1
          if (n2.lt.0) then
            n2 = 0
          endif
          fl = n1
          if(a.lt.0.0d0) fl=fl+1
          write(form,120) n1,n2
          write(str(1:n1),form) a
C     workaround to fix gfortran4.0 bug: 0.999999999--> 0
C     See: http://bugzilla.scilab.org/show_bug.cgi?id=1985
C     See: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=28354
          if (str(1:n1).eq.' 0.'.and.a.gt.0.9999d0) str(2:2)='1'
C     workaround to fix gfortran 4.2.1 & 4.3.2 bug: 0.999999999--> 2
C     See: http://bugzilla.scilab.org/show_bug.cgi?id=2647
C     See: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=37863
          if (str(1:n1).eq.' 2.'.and.a.lt.1.0d0) str(2:2)='1'
       elseif (ifmt .eq. -1) then
C     Inf
          fl = 4
          str = ' Inf'
       elseif (ifmt .eq. -2) then
C     Nan
          fl = 4
          str = ' Nan'
       endif

      return

  120 format('(f',i2,'.',i2,')')
  130 format('(1pd',i2,'.',i2,')')
      end
