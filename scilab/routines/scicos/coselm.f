
      subroutine coselm
c ====================================================================
c
c     evaluate scicos utility functions
c
c ====================================================================
c
c     Copyright INRIA
      INCLUDE '../stack.h'
      integer id(nsiz)
      integer kfun
      common /curblk/ kfun
      double precision ptr
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' coselm '//buf(1:4))
      endif
c
c     functions/fin
c     var2vec  vec2var get_import set_import get_curblk getlabel duplicate
c       1          2       3         4         5          6         7
c     scicos_cpass2  diffobjs
c           8            9
      goto(10,20,30,40,50,60,70,80,90) fin

c     var2vec
 10   continue
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c      if(istk(il).eq.1) return
      il=iadr(lstk(top))
      il1=il
      l1=sadr(il1+4)
      n=lstk(top+1)-lstk(top)
      call unsfdcopy(n,stk(lstk(top)),-1,stk(l1),-1)
      istk(il1)=1
      istk(il1+1)=n
      istk(il1+2)=1
      istk(il1+3)=0
      lstk(top+1)=l1+n
      goto 999
c
c     vec2var
 20   continue
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      il1=iadr(lstk(top))
      if(istk(il1).ne.1) then
         err=1
         call error(44)
         return
      endif
      l1=sadr(il1+4)

      n=lstk(top+1)-l1
      call unsfdcopy(n,stk(l1),1,stk(lstk(top)),1)
      lstk(top+1)=lstk(top)+n
      goto 999
c
 30   continue
c     getscicosvars
      call intgetscicosvars
      return
c
 40   continue
c     setscicosvars
      call intgetscicosvars
      return
c
c     curblock
 50   continue
      call intcurblk
      goto 999

c     getblocklabel
 60   continue
      call intgetlabel
      goto 999
c
c     duplicate
 70   continue
      call intduplicate('duplicate')
      call putlhsvar
      goto 999
c     cpass2
 80   continue
      call intcpass2('scicos_cpass2')
      call putlhsvar
      goto 999
c     cpass2
 90   continue
      call intdiffobjs('diffobjs')
      call putlhsvar
      goto 999
c     
 999  return
      end

      subroutine intcurblk
      include '../stack.h'
      integer kfun
      common /curblk/ kfun
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      l=sadr(il+4)
      stk(l)=kfun
      lstk(top+1)=l+1
      return
      end

      subroutine intgetscicosvars
      include '../stack.h'
      integer kfun
      common /curblk/ kfun
      integer vol,type,getscicosvars,ierr
      double precision ptr
      external getscicosvars
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.ne.1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      il1=iadr(lstk(top))
      if(istk(il1).ne.10) then
         err=1
         call error(55)
         return
      endif
c     
      if(istk(il1+1)*istk(il1+2).ne.1) then
         err=1
         call error(36)
         return
      endif
c     
      mn1=1
      id1=il1+4
      l1=id1+mn1+1
      vol=istk(id1+mn1)-1
      call cvstr(vol,istk(l1),buf,1)
c
      if(buf(1:vol).eq.'x') then
        ierr=getscicosvars(1,ptr,nv,type)
      elseif(buf(1:vol).eq.'xptr') then
        ierr=getscicosvars(2,ptr,nv,type)
      elseif(buf(1:vol).eq.'z') then
        ierr=getscicosvars(3,ptr,nv,type)
      elseif(buf(1:vol).eq.'zptr') then
        ierr=getscicosvars(4,ptr,nv,type)
      elseif(buf(1:vol).eq.'rpar') then
        ierr=getscicosvars(5,ptr,nv,type)
      elseif(buf(1:vol).eq.'rpptr') then
        ierr=getscicosvars(6,ptr,nv,type)
      elseif(buf(1:vol).eq.'ipar') then
        ierr=getscicosvars(7,ptr,nv,type)
      elseif(buf(1:vol).eq.'ipptr') then
        ierr=getscicosvars(8,ptr,nv,type)
      elseif(buf(1:vol).eq.'outtb') then
        ierr=getscicosvars(9,ptr,nv,type)
      elseif(buf(1:vol).eq.'inpptr') then
        ierr=getscicosvars(10,ptr,nv,type)
      elseif(buf(1:vol).eq.'outptr') then
        ierr=getscicosvars(11,ptr,nv,type)
      elseif(buf(1:vol).eq.'inplnk') then
        ierr=getscicosvars(12,ptr,nv,type)
      elseif(buf(1:vol).eq.'outlnk') then
        ierr=getscicosvars(13,ptr,nv,type)
      elseif(buf(1:vol).eq.'lnkptr') then
        ierr=getscicosvars(14,ptr,nv,type)
      else
         buf='Undefined field'
         call error(999)
         return
      endif
      if(ierr.ne.0) then
         buf='scicosim is not running'
         call error(999)
         return
      endif
      l=sadr(il1+4)
      if(type.eq.0) then
         call cint(nv,ptr,stk(l))
      else
         call cdouble(nv,ptr,stk(l))
      endif
      istk(il1)=1
      istk(il1+1)=nv
      istk(il1+2)=min(nv,1)
      istk(il1+3)=0
      lstk(top+1)=l+nv
      return
      end


      subroutine intsetscicosvars
      include '../stack.h'
      integer kfun
      common /curblk/ kfun
      integer vol,type,getscicosvars,ierr
      double precision ptr
      external getscicosvars
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.ne.2) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     
      lw = lstk(top+1)
      il2=iadr(lstk(top))
      if(istk(il2).ne.1) then 
         err=2
         call error(53)
         return
      endif
      if(istk(il2+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      nv2=istk(il2+1)*istk(il2+2)
      lv=sadr(il2+4)
      top=top-1
c
      il1=iadr(lstk(top))
      if(istk(il1+1)*istk(il1+2).ne.1) then
         err=1
         call error(36)
         return
      endif
c     
      mn1=1
      id1=il1+4
      l1=id1+mn1+1
      vol=istk(id1+mn1)-1
      call cvstr(vol,istk(l1),buf,1)

      if(buf(1:vol).eq.'x') then
        ierr=getscicosvars(1,ptr,nv,type)
      elseif(buf(1:vol).eq.'xptr') then
        ierr=getscicosvars(2,ptr,nv,type)
      elseif(buf(1:vol).eq.'z') then
        ierr=getscicosvars(3,ptr,nv,type)
      elseif(buf(1:vol).eq.'zptr') then
        ierr=getscicosvars(4,ptr,nv,type)
      elseif(buf(1:vol).eq.'rpar') then
        ierr=getscicosvars(5,ptr,nv,type)
      elseif(buf(1:vol).eq.'rpptr') then
        ierr=getscicosvars(6,ptr,nv,type)
      elseif(buf(1:vol).eq.'ipar') then
        ierr=getscicosvars(7,ptr,nv,type)
      elseif(buf(1:vol).eq.'ipptr') then
        ierr=getscicosvars(8,ptr,nv,type)
      elseif(buf(1:vol).eq.'outtb') then
        ierr=getscicosvars(9,ptr,nv,type)
      elseif(buf(1:vol).eq.'inpptr') then
        ierr=getscicosvars(10,ptr,nv,type)
      elseif(buf(1:vol).eq.'outptr') then
        ierr=getscicosvars(11,ptr,nv,type)
      elseif(buf(1:vol).eq.'inplnk') then
        ierr=getscicosvars(12,ptr,nv,type)
      elseif(buf(1:vol).eq.'outlnk') then
        ierr=getscicosvars(13,ptr,nv,type)
      elseif(buf(1:vol).eq.'lnkptr') then
        ierr=getscicosvars(14,ptr,nv,type)
      else
         buf='Undefined field'
         call error(999)
         return
      endif
      if(ierr.ne.0) then
         buf='scicosim is not running'
         call error(999)
         return
      endif
      if(nv.ne.nv2) then
         pstk(pt+1)=nv
         err=2
         call error(206)
         return
      endif
      if(type.eq.0) then
c     integer  
         call entier(nv,stk(lv),istk(iadr(lv)))
         call int2cint(nv,ptr,istk(iadr(lv)))
      else
         call dbl2cdbl(nv,ptr,stk(lv))
c     double
      endif
      il=iadr(lstk(top))
      istk(il)=0
      lstk(top+1)=lstk(top)+1
      return
      end

      subroutine intgetlabel
      include '../stack.h'
      integer iadr, sadr
      integer kfun
      integer getscilabel
      external getscilabel
      common /curblk/ kfun
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.gt.1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs.eq.1) then
         il=iadr(lstk(top))
         if(istk(il).ne.1) then 
            err=1
            call error(54)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            call error(36)
            return
         endif
         kf=stk(sadr(il+4))
         top=top-1
      else
         kf=kfun
      endif

c     
      lw = lstk(top+1)
      top=top+1
      il1=iadr(lstk(top))
      
      id1=il1+4
      l1=id1+2
      err=sadr(l1+50)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      ierr=getscilabel(kf,istk(l1),n)
      if(ierr.ne.0) then
         buf='scicosim is not running'
         call error(999)
         return
      endif
      istk(il1)=10
      istk(il1+1)=1
      istk(il1+2)=1
      istk(il1+3)=0
      istk(il1+4)=1
      istk(il1+5)=1+n
      lstk(top+1)=sadr(l1+n)
      return
      end
