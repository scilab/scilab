      subroutine intfile
c     interface for builtin file
c     Copyright INRIA
      INCLUDE '../stack.h'
c     
      integer status,access,form,recl,old,new,scratc,unknow
      integer sequen,direct,forma1,unform
      integer clo,rew,bak,ope,mode(2)
      integer top0,tops
      logical getscalar

      integer iadr,sadr
c     
      data old/857368/,new/2100759/,scratc/1707037/,unknow/1316638/
      data sequen/1707548/,direct/1774093/,forma1/1775631/
      data unform/988958/
      data clo/12/,ope/24/,rew/27/,bak/11/,last/21/,nclas/29/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     

      tops=top
      top0=top-rhs+1
c
      lw=lstk(top+1)
      if(rhs.le.1) then
c     .  enquire
c
         if(lhs.gt.5) then
            call error(41)
            return
         endif

c     .  count opened files
         n=0
         nb=0
         if(rhs.eq.1) then
            if(.not.getscalar('file', top, top, lr))return
            iu=stk(lr)
            top=top-1
            call getfileinfo(iu,ifa,iswap,ltype,mode,buf,lb,info)
            if(info.ne.1.and.info.ne.2) n=1
            nb=lb
         else
            i=-1
 01         i=i+1
            call getfileinfo(i,ifa,iswap,ltype,mode,buf,lb,info)
            if(info.eq.1) goto 02
            if(info.eq.2) goto 01
            n=n+1
            nb=nb+lb
            goto 01
         endif
 02      continue

         if(n.eq.0) then
            err=lstk(top+1)+2*rhs-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            do 05 i=1,lhs
               top=top+1
               il=iadr(lstk(top))
               istk(il)=1
               istk(il+1)=0
               istk(il+2)=0
               istk(il+3)=0
               lstk(top+1)=sadr(il+4)
 05         continue
            return
         endif
         
c     .  allocate lhs variables
c
c     .  vector of units
         top=top+1
         ilu=iadr(lstk(top))
         lu=sadr(ilu+4)
         lstk(top+1)=lu+n
         if(lhs.eq.1) goto 06
c     .  vector of types
         top=top+1
         ilt=iadr(lstk(top))
         lt=ilt+5+n
         lstk(top+1)=sadr(lt+n)
         if(lhs.eq.2) goto 06
c     .  vector of file names
         top=top+1
         iln=iadr(lstk(top))
         ln=iln+5+n
         lstk(top+1)=sadr(ln+nb)
         if(lhs.eq.3) goto 06
c     .  vector of modes
         top=top+1
         ilm=iadr(lstk(top))
         lm=sadr(ilm+4)
         lstk(top+1)=lm+n
         if(lhs.eq.4) goto 06
c     .  vector of swap
         top=top+1
         ils=iadr(lstk(top))
         ls=sadr(ils+4)
         lstk(top+1)=ls+n
c
 06      err=lstk(top+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
c     .  create lhs variable headers

c     .  vector of units
         istk(ilu)=1
         istk(ilu+1)=1
         istk(ilu+2)=n
         istk(ilu+3)=0
         if(lhs.eq.1) goto 12
c     .  vector of types
         istk(ilt)=10
         istk(ilt+1)=1
         istk(ilt+2)=n
         istk(ilt+3)=0
         istk(ilt+4)=1
         do 10 i=1,n
            istk(ilt+4+i)=i+1
 10      continue
         if(lhs.eq.2) goto 12
c     .  vector of names
         istk(iln)=10
         istk(iln+1)=1
         istk(iln+2)=n
         istk(iln+3)=0
         istk(iln+4)=1
         iln=iln+4
c
         if(lhs.eq.3) goto 12
c     .  vector of modes
         istk(ilm)=1
         istk(ilm+1)=1
         istk(ilm+2)=n
         istk(ilm+3)=0
c
         if(lhs.eq.4) goto 12
c     .  vector of swap
         istk(ils)=1
         istk(ils+1)=1
         istk(ils+2)=n
         istk(ils+3)=0
 12      continue

c     .  get file infos
         if(rhs.eq.1) then
c     .     unit
            stk(lu)=iu
            if(lhs.eq.1) goto 17
c     .     type
            if(ltype.eq.2) then
               istk(lt)=-12
            else
               istk(lt)=-15
            endif
            if(lhs.eq.2) goto 17
c     .     name
            call cvstr(lb,istk(ln),buf,0)
            istk(iln+1)=istk(iln)+lb
            if(lhs.eq.3) goto 17
c     .     mode
            stk(lm)=mode(1)
            if(lhs.eq.4) goto 17
c     .     swap
            stk(ls)=iswap
         else
            i=-1
 15         i=i+1
            call getfileinfo(i,ifa,iswap,ltype,mode,buf,lb,info)
            if(info.eq.1) goto 17
            if(info.eq.2) goto 15
c     .     unit
            stk(lu)=i
            lu=lu+1
            if(lhs.eq.1) goto 15
c     .     type
            if(ltype.eq.2) then
               istk(lt)=-12
            else
               istk(lt)=-15
            endif
            lt=lt+1
            if(lhs.eq.2) goto 15
c     .     name
            call cvstr(lb,istk(ln),buf,0)
            ln=ln+lb
            istk(iln+1)=istk(iln)+lb
            iln=iln+1
            if(lhs.eq.3) goto 15
c     .     mode
            stk(lm)=mode(1)
            lm=lm+1
            if(lhs.eq.4) goto 15
c     .     swap
            stk(ls)=iswap
            ls=ls+1

            goto 15
         endif
 17      continue
         return
      endif

      il=iadr(lstk(top))
      if(lhs.gt.2) then
         call error(41)
         return
      endif

c     action
      il=iadr(lstk(top+1-rhs))
      if(istk(il).ne.10) then
         err=1
         call error(55)
         return
      endif      
      if(istk(il+1)*istk(il+2).ne.1) then
         err=1
         call error(36)
         return
      endif
      l=il+5+istk(il+1)*istk(il+2)
      itype=abs(istk(l))
c

      if(itype.eq.ope) then
c     
c     open
         if(rhs.gt.6) then
            call error(39)
            return
         endif
c     path
         il=iadr(lstk(top+2-rhs))
         if(istk(il).ne.10) then
            err=2
            call error(55)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=2
            call error(36)
            return
         endif
         l=il+5+istk(il+1)*istk(il+2)
         mn=istk(il+5)-1
         buf=' '
         call cvstr(mn,istk(l),buf,1)
         rhs=rhs-2
         status=0
         access=0
         form=0
         recl=0
         if(rhs.eq.0) goto 145
         do 143 i=1,rhs
            il=iadr(lstk(top))
            if(istk(il).eq.10) then
               l=il+5+istk(il+1)*istk(il+2)
               if(istk(il+5)-1.lt.3) then
                  call error(36)
                  return
               endif
               itype=abs(istk(l))+256*(abs(istk(l+1))+256*abs(istk(l+2))
     $              )
               if(itype.eq.new) then
                  status=0
               elseif(itype.eq.old) then
                  status=1
               elseif(itype.eq.scratc) then
                  status=2
               elseif(itype.eq.unknow) then
                  status=3
               elseif(itype.eq.sequen) then
                  access=0
               elseif(itype.eq.direct) then
                  access=1
               elseif(itype.eq.forma1) then
                  form=0
               elseif(itype.eq.unform) then
                  form=1
               endif
            elseif(istk(il).eq.1) then
               recl=int(stk(sadr(il+4)))
               mode(2)=recl
            else
               err=i
               call error(53)
               return
            endif
            top=top-1
 143     continue
 145     mode(1)=status+10*(access+10*(form))
         lunit=0
         call clunit(lunit,buf(1:mn),mode)
         if(err.gt.0) then
            if(lhs.eq.1) then
               buf(mn+1:)=' '
               call error(err)
               return
            else
               top=top-1
               il=iadr(lstk(top))
               istk(il)=1
               istk(il+1)=0
               istk(il+2)=0
               istk(il+3)=0
               l=sadr(il+4)
               lstk(top+1)=l+1 

               top=top+1
               il=iadr(lstk(top))
               istk(il)=1
               istk(il+1)=1
               istk(il+2)=1
               istk(il+3)=0
               l=sadr(il+4)
               stk(l)=err
               lstk(top+1)=l+1
               err=0
               return
            endif
         endif
         top=top-1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         l=sadr(il+4)
         stk(l)=dble(lunit)
         lstk(top+1)=l+1
         if(lhs.eq.2) then
            top=top+1
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=1
            istk(il+2)=1
            istk(il+3)=0
            l=sadr(il+4)
            stk(l)=0.0d0
            lstk(top+1)=l+1
         endif
      elseif(itype.eq.clo) then
c     close
         if(lhs.ne.1) then
            call error(41)
            return
         endif
         if(rhs.ne.2) then
            call error(36)
            return
         endif
         il1=iadr(lstk(top))
         if(istk(il1).ne.1) then
            err=rhs
            call error(53)
            return
         endif
         n=istk(il1+1)*istk(il1+2)
         l=sadr(il1+4)

         top=top-1
         if(n.gt.0) then
            mode(1)=0
            do 151 i=1,n
               lunit=int(stk(l-1+i))
               if(lunit.gt.0) then
                  call clunit(-lunit,buf,mode)
               endif
 151        continue
         endif
         istk(il)=0
      elseif(itype.eq.rew) then
c     rewind
         if(lhs.ne.1) then
            call error(41)
            return
         endif
         if(rhs.ne.2) then
            call error(36)
            return
         endif
         il1=iadr(lstk(top))
         if(istk(il1).ne.1) then
            err=rhs
            call error(53)
            return
         endif
         lunit=int(stk(sadr(il1+4)))
         top=top-1

         if(lunit.ge.0) then
            call getfiletype(lunit,ltype,info)
            if(info.eq.0) then
               if(ltype.eq.2) then
                  call mseek(lunit,0,'set'//char(0),err)
               else
                  rewind(lunit)
               endif
            endif
         endif
         istk(il)=0
         goto 999

      elseif(itype.eq.bak) then
c     backspace
         if(lhs.ne.1) then
            call error(41)
            return
         endif
         if(rhs.ne.2) then
            call error(36)
            return
         endif
         il1=iadr(lstk(top))
         if(istk(il1).ne.1) then
            err=rhs
            call error(53)
            return
         endif
         lunit=int(stk(sadr(il1+4)))
         top=top-1
         if(lunit.ge.0) then
            call getfiletype(lunit,ltype,info)
            if(info.eq.0) then
               if(ltype.eq.2) then
                  err=1
                  call error(36)
                  return
               else
                  backspace(lunit)
               endif
            endif
         endif
c     
         istk(il)=0
      elseif(itype.eq.last) then
c     last
         if(lhs.ne.1) then
            call error(41)
            return
         endif
         if(rhs.ne.2) then
            call error(36)
            return
         endif
         il1=iadr(lstk(top))
         if(istk(il1).ne.1) then
            err=rhs
            call error(53)
            return
         endif
         lunit=int(stk(sadr(il1+4)))
         top=top-1
         if(lunit.ge.0) then
            call getfiletype(lunit,ltype,info)
            if(info.eq.0) then
               if(ltype.eq.2) then
                  call mseek(lunit,0,'end'//char(0),err)
               else
 154              read(lunit,'(a)',err=156,end=155)
                  go to 154
 155              backspace(lunit)
               endif
            endif
         endif
         istk(il)=0
         return
 156     call error(49)
         return
      else
c     incorrect action name
         err=1
         call error(36)
         return
      endif
 999  return
      end

