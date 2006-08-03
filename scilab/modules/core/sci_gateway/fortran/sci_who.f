c ====================================================================      
      subroutine intwho()
c     Copyright INRIA
      include 'stack.h'
      integer sz,szm,nv,nvm,top0
      character*6 what
      logical checkrhs,checklhs,getsmat,checkval,cremat
      integer iadr,sadr

c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c
      rhs=max(0,rhs)
      if(.not.checkrhs('who',0,1)) return

c
      if(rhs.eq.0) then
c     .  Display stored variables
         if(.not.checklhs('who',1,1)) return
c
c     .  local variables
         call msgs(38,0)
         sz=lstk(isiz) - lstk(bot) + 1
         szm=lstk(isiz)-lstk(1)
         nv=isiz-bot
         nvm=isiz-1
         call prntid(idstk(1,bot),nv-1,wte)
         write (buf,'(4i10)') sz,szm,nv,nvm
         call msgs(39,0)
c
c     .  global variables
         if(gtop.ge.isiz+2) then
            call msgs(44,0)
            sz=lstk(gtop+1) - lstk(isiz+2) + 1
            szm=lstk(gbot) - lstk(isiz+2) + 1
            nv=gtop-isiz-1
            nvm=isizt-isiz-1
            call prntid(idstk(1,isiz+2),nv,wte)
            write (buf,'(4i10)') sz,szm,nv,nvm
            call msgs(39,0)
         endif
c     .  return null matrix
         top=top+1
         il = iadr(lstk(top))
         istk(il) = 0
         lstk(top+1) = lstk(top) + 1
         return
      else
c     .  get stored variables
         if(.not.checklhs('who',1,2)) return
         top0=top
         if(.not.getsmat('who',top0,top,m,n,1,1,lr,nlr)) return
         if(.not.checkval('who',m*n,1)) return
         call codetoascii(max(5,nlr),istk(lr),what)
         if (what(1:3).eq.'get'.or.what(1:5).eq.'local') then
            n=isiz-bot
            il=iadr(lstk(top))
            lw=sadr(il+5+n+n*nlgh)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            ip=il+4
            l=ip+n+1
            istk(il)=10
            istk(il+1)=n
            istk(il+2)=1
            istk(il+3)=0
            istk(ip)=1
            do 21 i=0,n-1
               call namstr(idstk(1,bot+i),istk(l),nl,1)
               istk(ip+1+i)=istk(ip+i)+nl
               l=l+nl
 21         continue
            lstk(top+1)=sadr(l)
            if(lhs.eq.1) goto 999
            top=top+1
            if(.not.cremat('who',top,0,n,1,l,lc)) return
            do 22 i=0,n-1
               stk(l+i)=dble(lstk(bot+i+1)-lstk(bot+i))
 22         continue
         elseif (what.eq.'global') then
            n=gtop-(isiz+1)
            if(n.eq.0) then
               if(.not.cremat('who',top,0,0,0,l,lc)) return
               if(lhs.eq.1) goto 999
               top=top+1
               if(.not.cremat('who',top,0,0,0,l,lc)) return
               goto 999
            endif
            il=iadr(lstk(top))
            lw=sadr(il+5+n+n*nlgh)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            ip=il+4
            l=ip+n+1
            istk(il)=10
            istk(il+1)=n
            istk(il+2)=1
            istk(il+3)=0
            istk(ip)=1
            do 31 i=0,n-1
               call namstr(idstk(1,isiz+2+i),istk(l),nl,1)
               istk(ip+1+i)=istk(ip+i)+nl
               l=l+nl
 31         continue
            lstk(top+1)=sadr(l)
            if(lhs.eq.1) goto 999
            top=top+1
            if(.not.cremat('who',top,0,n,1,l,lc)) return
            do 32 i=0,n-1
               stk(l+i)=dble(lstk(isiz+2+i+1)-lstk(isiz+2+i))
 32         continue
         else
            err=1
            call error(116)
         endif
      endif
 999  return
      end

