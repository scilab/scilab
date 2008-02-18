c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine scisetmenu(fname)
      external setmen
      call scisetum("setmenu_old",setmen)
      end
C     ================================================================
      subroutine sciunsetmenu(fname)
      external unsmen
      call scisetum("unsetmenu_old",unsmen)
      end
C     ================================================================
      subroutine scisetum(fname,func)
C     ================================================================
C     SCILAB function :  setmenu, unsetmenu,
C     ================================================================ 
      character*(*) fname
c      implicit undefined (a-z)
      include 'stack.h'
      logical checkrhs,getsmat,getscalar
      integer topk,m,n,lr,nlr,gettype
      external func
      call xscion(inxsci)
c      if (inxsci.eq.0) then
c         return
c      endif
      if (.not.checkrhs(fname,1,3)) return
      topk = top
      nsub=0
      itype = gettype(top-rhs+1)
      if ( itype.eq.1 ) then 
         if (.not.checkrhs(fname,2,3)) return
         if (rhs.eq.3 ) then 
            if(.not.getscalar(fname,topk,top,lr))return
            nsub = int(stk(lr))
            top = top -1 
         endif
         if (.not.getsmat(fname,topk,top,m,n,1,1,lr,nlr)) return
         call  cvstr(nlr,istk(lr),buf,1)
         buf(nlr+1:nlr+1)=char(0)
         top = top-1
         if (.not.getscalar(fname,topk,top,lr)) return
         iwin = int(stk(lr))
      else
         if (.not.checkrhs(fname,1,2)) return
c      if -nw mode main window does not exists - error
        if (inxsci.eq.0) then
         buf='No main graphic window in -nw mode, specify 1-th arg...'
          call error(1020)
          return
         endif
         iwin = -1
         if ( rhs.eq.2 ) then 
            if(.not.getscalar(fname,topk,top,lr))return
            nsub = int(stk(lr))
            top = top -1 
         endif
         if (.not.getsmat(fname,topk,top,m,n,1,1,lr,nlr)) return
         call  cvstr(nlr,istk(lr),buf,1)
         buf(nlr+1:nlr+1)=char(0)
      endif
      call func(iwin,buf,0,0,nsub,ierr)
      call objvide(fname,top)
      return
      end
C     ================================================================
