      subroutine xawelm
c     =============================================================
c     Primitives for scilab menus 
c     =============================================================
c     Copyright ENPC 
      include '../stack.h'
      external setmen, unsmen
      
      call withgui(irep)
      if (irep .eq. 0) then
        buf='interface disabled in -nogui mode.'
        call error(999)
        return
      endif
      
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' xawelm '//buf(1:4))
      endif
c     
      if(fin.eq.1) then
         call scidialog("x_dialog")
      endif
      if(fin.eq.2) then
         call scimess("x_message")
      endif
      if(fin.eq.3) then
         call scichoose("x_choose")
      endif
      if(fin.eq.4) then
         call scimdial("x_mdialog")
      endif
      if(fin.eq.7) then
         call scichoice("choices")
      endif
      if (fin .eq. 8) then
         call sciaddm("addmenu_old")
      endif
      if (fin .eq. 9) then
         call scidellm("delmenu_old")
      endif
      if (fin .eq. 10 ) then 
         call scisetum("setmenu_old",setmen)
      endif
      if (fin.eq.11) then 
         call scisetum("unsetmenu_old",unsmen)
      endif
      if (fin.eq.12) then
         call intsxgetfile("xgetfile")
      endif
      if (fin.eq.13) then
         call scimess1("x_message_modeless")
      endif
 9999 return
      end

      subroutine sciaddm(fname)
C     ================================================================
C     SCILAB function : addmenu
C     ================================================================ 
      character*(*) fname
c      implicit undefined (a-z)
      include '../stack.h'
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







      subroutine scidellm(fname)
C     ================================================================
C     SCILAB function : dellmenu 
C     ================================================================ 
      character*(*) fname
c      implicit undefined (a-z)
      include '../stack.h'
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

      subroutine scisetum(fname,func)
C     ================================================================
C     SCILAB function :  setmenu, unsetmenu,
C     ================================================================ 
      character*(*) fname
c      implicit undefined (a-z)
      include '../stack.h'
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


      subroutine scichoice(fname)
C     ================================================================
c     SCILAB function : x_choices
C     ================================================================ 
      character*(*) fname
cc    implicit undefined (a-z)
      include '../stack.h'
      integer topk,m1,n1,lr1,il1,iadr,sadr
      integer m3,n3,il3,ild3,m2,n2,il2,ild2,rep
      logical getrvect,getwsmat,checkrhs
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,3,3)) return
      topk=top
C     third argument is a string vector 
      if (.not.getwsmat(fname,topk,top,m3,n3,il3,ild3)) return
      if (m3.ne.1.and.n3.ne.1) then 
         buf= fname// ': Third argument must be a vector'
         call error(999)
         return
      endif
      top=top-1
C     third argument is a string matrix 
      if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
      top=top-1
c     first argument is an int vector
      if(.not.getrvect(fname,topk,top,m1,n1,lr1))return      
      il1=iadr(lr1)
      call entier(m1*n1,stk(lr1),istk(il1))
      call xchoices(istk(il2),istk(ild2),m2*n2,
     $     istk(il3),istk(ild3),m3*n3, istk(il1),m1*n1,rep)
      if(rep.eq.-1) then 
         buf= fname// ': Error'
         call error(999)
         return
      endif
      if(rep.eq.0) then
c     return an empty matrix when canceled  
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
      else
         call entier2d(m1*n1,stk(lr1),istk(il1))
      endif
      return
      end


      subroutine scidialog(fname)
C     ================================================================
c     SCILAB function : x_dialog 
C     ================================================================
      character*(*) fname
cc      implicit undefined (a-z)
      include '../stack.h'
      integer topk,iadr,isize(2),sadr
      integer m3,n3,il3,ild3,nlr3,m2,n2,il2,ild2,m1,n1,il1,ild1
      logical getwsmat,checkrhs,cresmat1,cresmat2
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,1,2)) return
      topk=top
C     creation of extra arguments
C     size of 'OK' and 'Cancel'
      isize(1) = 2
      isize(2) = 6 
C     2nd argument if missing 
      if (rhs.lt.2) then 
         top  = top+1
         nlr2 = 1
         if(.not.cresmat2(fname,top,nlr2,lr2)) return
         call cvstr(nlr2,istk(lr2),' ',0)
      endif
C     we create ['OK';'Cancel'] 
      top = top+1 
      if (.not.cresmat1(fname,top,2,isize)) return
      call getsimat(fname,top,top,m3,n3,2,1,lr3,nlr3)
      call cvstr(isize(2),istk(lr3),'Cancel',0)
      call getsimat(fname,top,top,m3,n3,1,1,lr3,nlr3)
      call cvstr(isize(1),istk(lr3),'OK',0)
      if (.not.getwsmat(fname,topk,top,m3,n3,il3,ild3)) return
      top = top-1
C     second argument is a string vector
      if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
      if (m2.ne.1.and.n2.ne.1) then 
         buf= fname// ': Second argument must be a vector'
         call error(999)
         return
      endif
      top=top-1
C    first argument is a string vector 
      if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
      if (m1.ne.1.and.n1.ne.1) then 
         buf= fname// ': First argument must be a vector'
         call error(999)
         return
      endif
C     using the whole stack from top to bottom 
C     to get back the result 
C     (we can use top since the top objet is used before being 
C     scratched 
C     we MUST use ierr inside xdialg to check for space 
      ilmes=iadr(lstk(top))
      nrep=200
      ilw=ilmes+4+nrep+1
      ierr=iadr(lstk(bot))-ilw
      call xdialg(istk(il2),istk(ild2),m2*n2,
     $     istk(il1),istk(ild1),m1*n1,
     $     istk(il3),istk(ild3),m3*n3,
     $     istk(ilw),istk(ilmes+4),nrep,ierr)
      if(ierr.eq.3) then
         call error(113)
         return
      elseif(ierr.eq.2) then
         call error(17)
         return
      elseif(ierr.eq.1) then
         call error(112)
         return
      endif
      if(nrep.eq.0) then
         istk(ilmes)=1
         istk(ilmes+1)=0
         istk(ilmes+2)=0
         istk(ilmes+3)=0
         lstk(top+1)=sadr(ilmes+4)
      else
         istk(ilmes)=10
         istk(ilmes+1)=nrep
         istk(ilmes+2)=1
         il = ilmes+5+nrep
         call icopy(istk(il-1)-1,istk(ilw),1,istk(il),1)
         lstk(top+1)=sadr(il +istk(il-1))
      endif
      return
      end



      subroutine scimdial(fname)
C     ================================================================
c     SCILAB function : x_dialog 
C     ================================================================
      character*(*) fname
cc      implicit undefined (a-z)
      include '../stack.h'
      integer topk,iadr,sadr
      integer m3,n3,il3,ild3,m2,n2,il2,ild2,m1,n1,il1,ild1
      logical getwsmat,checkrhs
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,3,4)) return
      topk=top
      if (rhs.eq.4) then 
         if (.not.getwsmat(fname,topk,top,m4,n4,il4,ild4)) return
         top  = top-1
      endif
      if (.not.getwsmat(fname,topk,top,m3,n3,il3,ild3)) return
      if (m3.ne.1.and.n3.ne.1) then 
         buf= fname// ': third argument must be a vector'
         call error(999)
         return
      endif
      top = top-1
      if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
      if (m2.ne.1.and.n2.ne.1) then 
         buf= fname// ': second argument must be a vector'
         call error(999)
         return
      endif
      top=top-1
      if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
      if (m1.ne.1.and.n1.ne.1) then 
         buf= fname// ': First argument must be a vector'
         call error(999)
         return
      endif
      if ( rhs.eq.4) then 
         if ( m2*n2.ne.m4 ) then 
            buf= fname// ': incompatible second and fourth argument'
            call error(999)
            return
         endif
         if ( m3*n3.ne.n4 ) then 
            buf= fname// ': incompatible third and fourth argument'
            call error(999)
            return
         endif
      endif
      if ( rhs.eq.3) then 
         if ( m3*n3.ne.m2*n2 ) then 
            buf= fname// ': incompatible second and third argument'
            call error(999)
            return
         endif
      endif

C     (we can use top since the top objet is used before being 
C     scratched 
C     we MUST use ierr inside xdialg to check for space 
      ilres=iadr(lstk(top))
      if (rhs.eq.4 ) then 
         mres = m4
         nres = n4
         ilw=ilres+4+m4*n4+1
         ierr=iadr(lstk(bot))-ilw
         call xmatdg(istk(il1),istk(ild1),m1*n1,
     $        istk(il4),istk(ild4),
     $        istk(il2),istk(ild2),
     $        istk(il3),istk(ild3),mres,nres,
     $        istk(ilw),istk(ilres+4),ierr)
      else
         mres=m2*n2
         nres=1
         ilw=ilres+4+m3*n3+1
         ierr=iadr(lstk(bot))-ilw
         call xmdial(istk(il1),istk(ild1),m1*n1,
     $        istk(il3),istk(ild3),
     $        istk(il2),istk(ild2),mres,
     $        istk(ilw),istk(ilres+4),ierr)
      endif
      if(ierr.eq.3) then
         call error(113)
         return
      elseif(ierr.eq.2) then
         call error(17)
         return
      elseif(ierr.eq.1) then
         call error(112)
         return
      endif
      if(mres.eq.0) then
         istk(ilres)=1
         istk(ilres+1)=0
         istk(ilres+2)=0
         istk(ilres+3)=0
         lstk(top+1)=sadr(ilres+4)
      else
         istk(ilres)=10
         istk(ilres+1)=mres
         istk(ilres+2)=nres
         istk(ilres+3)=0
         lstk(top+1)=sadr(ilres+5+mres*nres+istk(ilres+4+nres*mres))
         return
      endif
      return
      end


      subroutine scimess(fname) 
C     ================================================================
c     SCILAB function : xmessage 
C     ================================================================
      character*(*) fname
cc      implicit undefined (a-z)
      include '../stack.h'
      integer topk,iadr,sadr
      integer m2,n2,il2,ild2,m1,n1,il1,ild1
      logical getwsmat,checkrhs,cresmat2
      logical cremat
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,1,2)) return
      topk=top
C     creation of extra arguments
C     2nd argument if missing 
      if (rhs.lt.2) then 
         top  = top+1
         nlr2 = 2
         if(.not.cresmat2(fname,top,nlr2,lr2)) return
         call cvstr(nlr2,istk(lr2),'Ok',0)
      endif
C     second argument is a string vector
      if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
      if (m2.ne.1.and.n2.ne.1) then 
         buf= fname// ': Second argument must be a vector'
         call error(999)
         return
      endif
      top=top-1
C     first argument is a string vector 
      if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
      if (m1.ne.1.and.n1.ne.1) then 
         buf= fname// ': First argument must be a vector'
         call error(999)
         return
      endif
      call xmsg(istk(il1),istk(ild1),m1*n1,
     $     istk(il2),istk(ild2),m2*n2,nrep,ierr)
      if(ierr.eq.1) then 
         call error(112)
         return
      endif
      if(m2*n2.eq.1) then
         call objvide(fname,top)
      else
         if (.not.cremat(fname,top,0,1,1,lr1,lc)) return
         stk(lr1)= nrep
      endif
      return
      end

      subroutine scimess1(fname) 
C     ================================================================
c     SCILAB function : xmessage 
C     ================================================================
      character*(*) fname
cc      implicit undefined (a-z)
      include '../stack.h'
      integer topk,iadr,sadr
      integer m2,n2,il2,ild2,m1,n1,il1,ild1
      logical getwsmat,checkrhs,cresmat2
      logical cremat
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,1,1)) return
      topk=top
C     creation of extra arguments
C     add button name
      if (rhs.lt.2) then 
         top  = top+1
         nlr2 = 2
         if(.not.cresmat2(fname,top,nlr2,lr2)) return
         call cvstr(nlr2,istk(lr2),'Ok',0)
      endif
C     second argument is a string vector
      if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
      if (m2.ne.1.or.n2.ne.1) then 
         buf= fname// ': Second argument must be a string'
         call error(999)
         return
      endif
      top=top-1
C     first argument is a string vector 
      if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
      if (m1.ne.1.and.n1.ne.1) then 
         buf= fname// ': First argument must be a vector'
         call error(999)
         return
      endif
      call xmsg1(istk(il1),istk(ild1),m1*n1,
     $     istk(il2),istk(ild2),m2*n2,ierr)
      if(ierr.eq.1) then 
         call error(112)
         return
      endif
      call objvide(fname,top)
      end

      subroutine scichoose(fname) 
C     ================================================================
c     SCILAB function : x_choose
C     ================================================================
      character*(*) fname
cc      implicit undefined (a-z)
      include '../stack.h'
      integer topk,iadr,sadr
      integer m3,n3,il3,ild3,m2,n2,il2,ild2,m1,n1,il1,ild1
      logical getwsmat,checkrhs,cresmat2
      logical cremat
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,2,3)) return
      topk=top
      if(rhs.eq.2) then
C     Create button list ( just Cancel in fact ) 
         top  = top+1
         nlr2 = 6
         if(.not.cresmat2(fname,top,nlr2,lr2)) return
         call cvstr(nlr2,istk(lr2),'Cancel',0)
      endif
      if (.not.getwsmat(fname,topk,top,m3,n3,il3,ild3)) return
      top = top -1 
C     second argument is a string vector
      if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
      if (m2.ne.1.and.n2.ne.1) then 
         buf= fname// ': Second argument must be a vector'
         call error(999)
         return
      endif
      top=top-1
C     first argument is a string vector 
      if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
      if (m1.ne.1.and.n1.ne.1) then 
         buf= fname// ': First argument must be a vector'
         call error(999)
         return
      endif
      call xchoose(istk(il2),istk(ild2),m2*n2,
     $     istk(il1),istk(ild1),m1*n1,
     $     istk(il3),istk(ild3),m3*n3,nrep,ierr)
      if(ierr.eq.1) then 
         call error(112)
         return
      endif
      if (.not.cremat(fname,top,0,1,1,lr1,lc)) return
      stk(lr1)= nrep
      return
      end


      subroutine intsxgetfile(fname)
C     ================================================================
c     SCILAB function : xgetfile, fin = 1
C     ================================================================
       character*(*) fname
       integer topk,rhsk,topl
       logical checkrhs,checklhs,cresmat2,getsmat,checkval,bufstore
       logical crepointer,crestring,isoptlw,getwsmat
       include '../stack.h'
       character name*(nlgh)
c
       integer iadr, sadr
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,0,3)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable a (number 1)
c       
       if(rhs .le. 0) then
        top = top+1
        rhs = rhs+1
        nlr1 = 3
        if(.not.cresmat2(fname,top,nlr1,lr1)) return
        call cvstr(nlr1,istk(lr1),'*.*',0)
       endif
c       checking variable dirname (number 2)
c       
       if(rhs .le. 1) then
        top = top+1
        rhs = rhs+1
        nlr2 = 1
        if(.not.cresmat2(fname,top,nlr2,lr2)) return
        call cvstr(nlr2,istk(lr2),'.',0)
       endif

       if(rhs .le. 2) then
        top = top+1
        rhs = rhs+1
        nlr3 = 9
        if(.not.cresmat2(fname,top,nlr3,lr3)) return
        call cvstr(nlr3,istk(lr3),'File menu',0)
       endif

       if(.not.crepointer(fname,top+1,lw3)) return


       if (rhsk.eq.3) then 
          idir=1
          if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1))
     $         return
          if(.not.checkval(fname,m1*n1,1)) return
          if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return

          if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2))
     $         return
          if(.not.checkval(fname,m2*n2,1)) return
          if(.not.bufstore(fname,lbuf,lbufi2,lbuff2,lr2,nlr2)) return

          if (.not.getwsmat(fname,top,top-rhs+3,m3,n3,il3,ild3)) return

          call xgetfile(buf(lbufi1:lbuff1),buf(lbufi2:lbuff2),
     $         stk(lw3),ne5,err,idir,istk(il3),istk(ild3),m3*n3)
       endif
       if (rhsk.eq.2) then 
          if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1))
     $         return
          if(.not.checkval(fname,m1*n1,1)) return
          if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return

          if (isoptlw(top,top-rhs+2,name)) then 
             if (name.ne.'title') then 
                buf = fname // 'optional argument must be title='
                call error(999)
                return
             endif
             idir = 0
             if (.not.getwsmat(fname,top,top-rhs+2,m2,n2,il2,ild2))
     $            return
             call xgetfile(buf(lbufi1:lbuff1),'.',
     $            stk(lw3),ne5,err,idir,istk(il2),istk(ild2),m2*n2)
          else
             idir = 1
             if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2))
     $            return
             if(.not.checkval(fname,m2*n2,1)) return
             if(.not.bufstore(fname,lbuf,lbufi2,lbuff2,lr2,nlr2)) return

             if (.not.getwsmat(fname,top,top-rhs+3,m3,n3,il3,ild3))
     $            return
             call xgetfile(buf(lbufi1:lbuff1),buf(lbufi2:lbuff2),
     $            stk(lw3),ne5,err,idir,istk(il3),istk(ild3),m3*n3)
          endif
       endif
       if ( rhsk.eq.1) then 
          idir = 0
          if (isoptlw(top,top-rhs+1,name)) then 
             if (name.ne.'title') then 
                buf = fname // ' optional argument must be title='
                call error(999)
                return
             endif
             if (.not.getwsmat(fname,top,top-rhs+1,m1,n1,il1,ild1))
     $            return
             call xgetfile('*.*','.',
     $            stk(lw3),ne5,err,idir,istk(il1),istk(ild1),m1*n1)
          else
             if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1))
     $            return
             if(.not.checkval(fname,m1*n1,1)) return
             if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return

             if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2))
     $            return
             if(.not.checkval(fname,m2*n2,1)) return
             if(.not.bufstore(fname,lbuf,lbufi2,lbuff2,lr2,nlr2)) return

             if (.not.getwsmat(fname,top,top-rhs+3,m3,n3,il3,ild3))
     $            return
             call xgetfile(buf(lbufi1:lbuff1),buf(lbufi2:lbuff2),
     $            stk(lw3),ne5,err,idir,istk(il3),istk(ild3),m3*n3)
          endif
       endif
       if (rhsk.le.0 ) then 
          idir=0
          if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1))
     $         return
          if(.not.checkval(fname,m1*n1,1)) return
          if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
          
          if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2))
     $         return
          if(.not.checkval(fname,m2*n2,1)) return
          if(.not.bufstore(fname,lbuf,lbufi2,lbuff2,lr2,nlr2)) return
          
          if (.not.getwsmat(fname,top,top-rhs+3,m3,n3,il3,ild3))
     $         return
          call xgetfile(buf(lbufi1:lbuff1),buf(lbufi2:lbuff2),
     $         stk(lw3),ne5,err,idir,istk(il3),istk(ild3),m3*n3)
       endif
C
C      the result is a string and xgetfile 
C      will have to create ( malloc ) space for storing the result
C      we will keep track of this allocated string through lw3.

       if(err .gt. 0) then 
        buf = fname // 'Internal Error' 
        call error(999)
       endif
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: res
        top=topl+1
        if(.not.crestring(fname,top,ne5,ilrs)) return
        call ccharf(ne5,stk(lw3),istk(ilrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end


