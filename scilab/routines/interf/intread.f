      subroutine intread
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
c     
      integer top2
      integer ftyp,fmttyp,mode(2)
      logical opened,eptover
      integer iadr,sadr
      character bu1*(bsiz),bu2*(bsiz)
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)

      if(rhs.gt.5.or.rhs.lt.3) then
         call error(39)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif
c
c     opening file
      top2 = top
      top = top-rhs+1
      il=iadr(lstk(top))
      mode(1)=-1
      mode(2)=0
      call v2unit(top,mode,lunit,opened,ierr)
      if(ierr.gt.0) return
c     
      iacces=0
      if(rhs.eq.3) goto 121
      il=iadr(lstk(top2))
      if(rhs.eq.5) then
         iacces=top2-1
      else
         if(istk(il).eq.1) then
            call error(61)
            goto 996
         endif
      endif
c     analyse du format
      if(istk(il).ne.10) then
         err=rhs
         call error(55)
         goto 996
      endif
      nc=istk(il+5)-1
      l=il+5+istk(il+1)*istk(il+2)
      ftyp=fmttyp(istk(l),nc)
      if(ftyp.eq.0) then
         call error(49)
         goto 996
      endif
      call cvstr(nc,istk(l),buf,1)
      top2=top2-1
      fin=-fin
 121  if(iacces.eq.0) goto 123
      ilb=iadr(lstk(iacces))
      if(istk(ilb+3).ne.0) then
         err=top2-top+1
         call error(52)
         goto 996
      endif
      nb=istk(ilb+1)*istk(ilb+2)
      lb=sadr(ilb+4)
      ilb=iadr(lw)
      err=sadr(ilb+nb)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         goto 996
      endif
      do 122 i=1,nb
         istk(ilb+i-1)=int(stk(lb+i-1))
 122  continue
      top2=iacces-1
 123  il=iadr(lstk(top2))
      l=sadr(il+4)
      n=int(stk(l))
      top2=top2-1
      il=iadr(lstk(top2))
      l=sadr(il+4)
      m=int(stk(l))
      il=iadr(lstk(top))
      l=sadr(il+4)
      if(m.eq.0.or.n.le.0) then
         istk(il)=1
         istk(il+1)=0
         istk(il+2)=0
         istk(il+3)=0
         lstk(top+1)=sadr(il+4)
         if(.not.opened) then
            mode(1)=0
            mode(2)=0
            call clunit(-lunit,buf,mode)
         endif
         goto 999
      endif

      istk(il)=1
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=0

      if(m.gt.0) then
         err=l+m*n-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      endif

      if(fin.lt.0.and.ftyp.ne.1) goto 126
      if(fin.gt.0) then 
         buf(1:1)='*'
         nc=1
      endif
c     
c---- lecture de flottants
      if(iacces.eq.0) then
c     acces sequentiel
         if(m.gt.0) then
c     .     nombre de ligne precise
            do 124,i=0,m-1
               call dbasin(ierr,lunit,buf(1:nc),stk(l+i),m,n)
               if(ierr.eq.1) then
                  goto 997
               elseif(ierr.eq.2) then
                  goto 998
               endif
 124        continue
         else
c     .     nombre de ligne non precise
            i=-1
 1241       i=i+1
            li=l+n*i
            err=li+n-lstk(bot)
            if(err.gt.0) then
               call error(17)
               goto 996
            endif
            call dbasin(ierr,lunit,buf(1:nc),stk(li),1,n)
            if(ierr.eq.0) goto 1241
            if(ierr.eq.2) goto 998
            m=i
            if(m.ne.0) then
               if(m.ge.1.and.n.ge.1) then
                  lstk(top+1)=l+m*n
                  err=lstk(top+1)+m*n-lstk(bot)
                  if(err.gt.0) then
                     call error(17)
                     return
                  endif
                  l1=l+m*n
                  call unsfdcopy(m*n,stk(l),1,stk(l1),1)
                  call mtran(stk(l1),n,stk(l),m,n,m)
                  istk(il+1)=m
               endif
            else
               istk(il+1)=0
               istk(il+2)=0
            endif
         endif         
      else
c     acces direct
         if(nb.ne.m) then
            call error(42)
            goto 996
         endif
         if(fin.gt.0) then
            call error(61)
            goto 996
         endif
         if(m.lt.0) then
            call  error(43)
            return
         endif
         do 125 i=0,m-1
            li=l+i
            read(lunit,buf(1:nc),rec=istk(ilb+i),err=998)
     +           (stk(li+j*m),j=0,n-1)
 125     continue
      endif
      lstk(top+1)=l+m*n
      if(.not.opened) then
         mode(1)=0
         mode(2)=0
         call clunit(-lunit,buf,mode)
      endif
      goto 999
c     
c---- lecture des chaines de caracteres
 126  if(ftyp.ne.4) then
         call error(49)
         goto 996
      endif
      if(n.ne.1) then
         err=iacces-top
         call error(36)
         goto 996
      endif
      if(iacces.ne.0) then
         if(nb.ne.m) then
            call error(42)
            goto 996
         endif
      endif
      ili=il+4
      if(m.gt.0) then
c     .  nombre de ligne a lire precise
         li=ili+m+1
         istk(ili)=1
         do 128 i=1,m
            if(iacces.eq.0) then
               call basin(ierr,lunit,buf(nc+1:),buf(1:nc),.false.)
               if(ierr.eq.1) goto 997
               if(ierr.eq.2) goto 998
            else
               read(lunit,buf(1:nc),rec=istk(ilb+i-1),err=998)
     $              buf(nc+1:)
            endif
            mn=bsiz+1
 127        mn=mn-1
            if(buf(mn:mn).eq.' ') goto 127
            mn=max(1,mn-nc)
            err=sadr(li+mn)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call cvstr(mn,istk(li),buf(nc+1:),0)
            li=li+mn
            ili=ili+1
            istk(ili)=istk(ili-1)+mn
 128     continue
         istk(il)=10
         lstk(top+1)=sadr(li)
      else
c     .  nombre de ligne a lire non precise
         if(iacces.ne.0) then
            call error(43)
            goto 996
         endif
         li=ili
         i=-1
 1281    i=i+1
         call basin(ierr,lunit,buf(nc+1:),buf(1:nc),.false.)
         mn=bsiz+1
 1282    mn=mn-1
         if(buf(mn:mn).eq.' ') goto 1282
         mn=max(1,mn-nc)
         err=sadr(li+mn+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            goto 996
         endif
         call cvstr(mn,istk(li+1),buf(nc+1:),0)
         istk(li)=mn
         li=li+mn+1
         if(ierr.eq.0) goto 1281
         if(ierr.eq.2) goto 998
         m=i
         if(m.le.0) then
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
         else
            call icopy(li-ili+1,istk(ili),-1,istk(li+2),-1)
            lis=li+2
            istk(il)=10
            istk(il+1)=m
            istk(il+2)=min(m,1)
            istk(ili)=1
            li=ili+m+1
            do 1283 j=1,m
               mn=istk(lis)
               istk(ili+1)=istk(ili)+mn
               call icopy(mn,istk(lis+1),1,istk(li),1)
               lis=lis+mn+1
               li=li+mn
               ili=ili+1
 1283       continue
            lstk(top+1)=sadr(li+1)
         endif
      endif
      if(.not.opened) then
         mode(1)=0
         mode(2)=0
         call clunit(-lunit,buf,mode)
      endif
      goto 999

 996  if(.not.opened) call clunit(-lunit,buf,mode)
      return
 997  err=i
      call error(62)
      if(.not.opened) call clunit(-lunit,buf,mode)
      return
 998  call error(49)
      if(.not.opened) call clunit(-lunit,buf,mode)
      return


 999  return
      end
