c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine prompt(pause,escape)
c ====================================================================
c     issue prompt with optional pause

      include 'stack.h'
      integer pause,escape,menusflag
      character *20,linfo
      logical iflag,interruptible
      common /basbrk/ iflag,interruptible

      escape=0
      if (pause .ne. 1) then
         call basout(io,wte,' ')
         call setprlev(paus)
      else
C     version with pause ( mode(7) )
         call setprlev(-1)
c     .  accept immediate dynamic callback execution
         menusflag=1
 10      call basin(ierr,rte,buf,'*',menusflag)
         if(buf(1:1).eq.'p') iflag=.true.
         if(ierr.eq.-1) then
c     .     a callback requires to be executed
            escape=1
         endif
      endif
      return
      end
