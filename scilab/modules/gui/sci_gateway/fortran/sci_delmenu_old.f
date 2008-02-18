c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine scidellm(fname)
C     ================================================================
C     SCILAB function : dellmenu 
C     ================================================================ 
      character*(*) fname
c      implicit undefined (a-z)
      include 'stack.h'
      logical checkrhs,getsmat,getscalar
      integer topk,m,n,lr,nlr
      call xscion(inxsci)
c      if (inxsci.eq.0) then
c         return
c      endif
      if (.not.checkrhs(fname,1,2)) return
      topk = top
      if (.not.getsmat(fname,topk,top,m,n,1,1,lr,nlr)) return
      call  cvstr(nlr,istk(lr),buf,1)
      buf(nlr+1:nlr+1)=char(0)
      if ( rhs.eq.2 ) then 
         top = top-1 
         if(.not.getscalar(fname,topk,top,lr))return
         iwin=int(stk(lr))
      else
c      if -nw mode main window does not exists - error
        if (inxsci.eq.0) then
         buf='No main graphic window in -nw mode, specify 1-th arg...'
          call error(1020)
          return
         endif
       iwin=-1
      endif
c     next two lines for compatibility "2D Zoom" button renamed in "Zoom"
      if (buf(1:7).eq.'2D Zoom') buf='Zoom'//char(0)
      if (buf(1:8).eq.'2D &Zoom') buf='&Zoom'//char(0)

      call delbtn(iwin,buf)
      call objvide(fname,top)
      return
      end
C     ================================================================
