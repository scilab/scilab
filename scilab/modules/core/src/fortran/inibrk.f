c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


      subroutine inibrk
c ====================================================================
c
c       initialisation systeme pour recuperer le break
c

      logical iflag,interruptible
      common /basbrk/ iflag,interruptible
      iflag=.false.
      call csignal()
c      call sunieee
      return
      end

      subroutine sunieee
C     jpc temporairement
c      external my_handler, my_ignore
c      is=signal( 8,sigbas,-1)
c      i=ieee_handler("set","underflow",my_ignore)
c      i=ieee_handler("set","inexat",my_ignore)
c      i=ieee_handler("set","invalid",my_handler)
c      i=ieee_handler("set","overflow",my_handler)
c      i=ieee_handler("set","division",my_handler)
      return
      end
      
      integer function my_handler(sig,code,sigcontext,addr)
      integer sig,code,sigcontext(5),addr
      call sigbas(8)
      my_handler=0
      return
      end 

C     on peut aussi utiliser SIGFPE_ABORT mais il faut 
C     rajouter un include de <f77/f77_floatingpoint.h>
      integer function my_ignore(sig,code,sigcontext,addr)
      integer sig,code,sigcontext(5),addr
      my_ignore=0
      return
      end 
