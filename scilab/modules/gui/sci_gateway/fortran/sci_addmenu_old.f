c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine sciaddm(fname)
C     ================================================================
C     SCILAB function : addmenu
C     ================================================================ 
      character*(*) fname
c      implicit undefined (a-z)
      include 'stack.h'
      logical getsmat,getscalar,getwsmat
      logical full,checklhs,checkrhs
      logical getilist,getlistscalar,getlistsimat
      integer topk,lr,typ,verb,iwin,owin,iadr,sadr
      double precision dv 
      character*(nlgh+1) mname
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      call xscion(inxsci)
c      if (inxsci.eq.0) then
c         buf='synchronous actions are not supported with -nw option'
c         call error(1020)
c         return
c      endif
      if (.not.checkrhs(fname,1,4)) return
      if (.not.checklhs(fname,1,1)) return
      topk = top
c     checking last variable
      ill=iadr(lstk(top))
      if(istk(ill).eq.15) then
         full=.true.
c        .last variable is list(typ,mname)
         if(.not.getilist(fname,topk,top,n1,1,il1)) return
         if(n1.ne.2) then
            buf='Last argument of addmenu must be list(typ,mname)'
            call error(9990)
            return
         endif
c        --   list element number 1 typ(g) --
         if(.not.getlistscalar(fname,topk,top,1,lr1e1)) return
         typ = int(stk(lr1e1))
c        --   list element number 2 name(g) --
         if(.not.getlistsimat(fname,topk,top,2,m1e2,n1e2,1,1,
     $        lr1e2,nlr1e2)) return
         if( m1e2*n1e2.ne.1) then 
            buf='Last argument of addmenu must be list(typ,mname)'
            call error(9990)
            return
         endif
         if ( nlr1e2+1 .gt. nlgh ) then 
            buf = 'Last argument list(typ,mname) : mname too long'
            call error(9990)
            return
         endif
         call cvstr(nlr1e2,istk(lr1e2),mname,1)
         mname(nlr1e2+1:nlr1e2+1)=char(0)
      else
         typ=0
         full=.false.
      endif

c     checking variable win (number 1)

      il1 = iadr(lstk(top-rhs+1))
      if (istk(il1) .eq. 1) then
c     in a graphic window
         if(.not.getscalar(fname,topk,top-rhs+1,lr))return
         iwin = int(stk(lr))
         iskip = 0
         elseif (istk(il1).eq.10) then
c      if -nw mode main window does not exists - error
        if (inxsci.eq.0) then
         buf='No main graphic window in -nw mode, specify 1-th arg...'
          call error(1020)
          return
         endif
c     in main window
         iskip =1
         iwin =-1
      else
         err = 1
         call error(44)
         return
      endif

c     checking variable name (number 2-iskip)

      if(.not.getsmat(fname,top,top-rhs+2-iskip,m2,n2,1,1,lr2,nlr2))
     $     return
      call cvstr(nlr2,istk(lr2),buf,1)
      buf(nlr2+1:nlr2+1)=char(0)
      if (.not.full ) then 
         mname = buf(1:nlr2+1)
      endif

      if (rhs.ge.3-iskip) then
         il1 = iadr(lstk(top-rhs+3-iskip))
c        checking variable submenu names (number 3-iskip)
         if ( istk(il1).ne.15 ) then 
            if (.not.getwsmat(fname,topk,top-rhs+3-iskip,m3,n3,
     $           il3,ild3))           return
            if (m3.ne.1.and.n3.ne.1) then 
               buf= fname// ': Third argument must be a vector'
               call error(999)
               return
            endif
            top=top-rhs+1
            verb=0
            if(iwin.ge.0) then
               call dr('xget'//char(0),'window'//char(0),verb,owin,na,
     $              v,v,v,dv,dv,dv,dv)
               call dr('xset'//char(0),'window'//char(0),iwin,iv,iv,
     $              v,v,v,dv,dv,dv,dv)
               call addmen(iwin,buf,istk(il3),istk(ild3),m3*n3,typ,
     $              mname, ierr) 
               call dr('xset'//char(0),'window'//char(0),owin,iv,iv,
     $              v,v,v,dv,dv,dv,dv)
            else
               call addmen(iwin,buf,istk(il3),istk(ild3),m3*n3,typ, 
     $              mname,         ierr) 
            endif
            call objvide(fname,top)
            return
         endif
      endif
      top=top-rhs+1
      verb=0
      if(iwin.ge.0) then
         call dr('xget'//char(0),'window'//char(0),verb,owin,na,
     $        v,v,v,dv,dv,dv,dv)
         call dr('xset'//char(0),'window'//char(0),iwin,iv,iv,
     $        v,v,v,dv,dv,dv,dv)
         call addmen(iwin,buf,0,0,0,typ, mname,ierr)
         call dr('xset'//char(0),'window'//char(0),owin,iv,iv,
     $        v,v,v,dv,dv,dv,dv)
      else
         call addmen(iwin,buf,0,0,0,typ,mname,ierr)
      endif
      call objvide(fname,top)
      return
      end
C     ================================================================


