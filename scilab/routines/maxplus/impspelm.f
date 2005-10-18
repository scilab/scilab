      subroutine impspelm
c     
c     Copyright  INRIA 

      include  '../stack.h'
      include 'mpstack.h'
c      double precision PCZERO,PCONE,PCTOP,ZERO,TOPM
c      integer PCALGCODE


      integer id(nsiz)
      integer iadr, sadr,typecode,algtype,fd
      logical getscalar
      double precision tmp

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
c     the following global variables 
c     should be set more properly later on, since
c     all these combinatorial handlings are essentially algebra independent
c     SG
c      tmp=1000.d0
c      ZERO=-exp(tmp)
      call commons(ZERO)
      TOPM=-ZERO
      PCONE=0.d0
c      call commons(PCZERO,PCTOP,PCALGCODE)
c      write(*,*)PCZERO,ZERO,PCTOP,TOPM,tutu,PCALGCODE
      PCZERO=ZERO
      PCTOP=TOPM
      PCALGCODE=1

c
      if (ddt .eq. 4) then
c         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' spelm '//buf(1:4))
      endif

c     
c     functions/fin
c     1       2       3     4     5   

      goto (10,20,30,40,50) fin
c     
c     sparse
 10   continue
c      write(*,*)'sparse',PCZERO
      call intmpsparse(id)
      goto 900

 20   continue
c      write(*,*)'spget'
c     [ij,v,sz]=spget(sp)
      call intmpspget(id)
      goto 900

 30   continue
c     [A]=full(sp)
c      write(*,*)'full'
      call intmpfull(id)
      goto 900

 900  continue
 999  return
c
c    save and load sparse matrix 
c
 40   continue
c     get pointer on variable to save
c      write(*,*)'save'
      il=iadr(lstk(top-1))
      if(istk(il).lt.0) il=iadr(istk(il+1))
c     get logical unit
      if(.not.getscalar('save',top,top,lr)) return
      fd=stk(lr)
      call savesparse(fd,il,ierr)
      if(ierr.ne.0) return
      return

 50   if(.not.getscalar('load',top,top,lr)) return
      fd=stk(lr)
      top=top-1
      il=iadr(lstk(top))
      call loadsparse(fd,il,n,ierr)
      if(ierr.ne.0) return   
      istk(il)=261
      ids(4,pt)=n
      lstk(top+1)=sadr(il+n)
      return

      return
      end

      subroutine intmpsparse(id)
      include '../stack.h'
      include 'mpstack.h'
      integer id(nsiz),top0,tops
      integer iadr, sadr, basetype,typecode,algtype
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      basetype(itype)=mod(itype,100)
c
      rhs = max(0,rhs)
      top0=top+1-rhs
      tops=top
c     
      lw = lstk(top+1)
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     creation de matrice creuse
      if(rhs.eq.1) then
c     sparse(x)
         il=iadr(lstk(top))
         if(typecode(istk(il)).eq.5.or.typecode(istk(il)).eq.6) return
         m=istk(il+1)
         n=istk(il+2)
         if(m*n.eq.0.or.m.eq.-1) return
         if(typecode(istk(il)).eq.1) then
            it=istk(il+3)
            l=sadr(il+4)
c     
            ilr=iadr(lw)
            lw=sadr(ilr+m+m*n)

            ls=lw
            li=ls+m*n
            lw=ls+m*n*(it+1)
            
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            if(it.eq.0) then
c               write(*,*)'sparse/1'
               call dful2sp(m,n,stk(l),nel,istk(ilr),stk(ls),PCZERO)
            else
c               write(*,*)'sparse/1bis'
               call wful2sp(m,n,stk(l),stk(l+m*n),nel,
     $              istk(ilr),stk(ls),stk(li),PCZERO,PCZERO)
            endif
c            write(*,*)'sparse/2',PCZERO,PCALGCODE
            call icopy(m+nel,istk(ilr),1,istk(il+5),1)
            l=sadr(il+5+m+nel)
            call dcopy(nel,stk(ls),1,stk(l),1)
            if(it.eq.1) call dcopy(nel,stk(li),1,stk(l+nel),1)
            istk(il)=algtype(PCALGCODE,5)
            istk(il+4)=nel
            lstk(top+1)=l+nel*(it+1)
         else
            buf='we should not be here in intmpsparse'
            call error(9999)
c            call funnam(ids(1,pt+1),'sparse',iadr(lstk(top)))
c            fun=-1
c            return
         endif
         return
      endif
c
c     sparse([i,j],value,[m,n])
      if (rhs .ne. 3.and.rhs.ne.2) then
         call error(39)
         return
      endif
      m=-1
      n=-1
      if(rhs.eq.3) then
c     [m,n] given
         il=iadr(lstk(top))
         if(typecode(istk(il)).ne.1.or.istk(il+3).ne.0) then
            err=1
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.2) then
            err=1
            call error(60)
            return
         endif
         l=sadr(il+4)
         m=stk(l)
         n=stk(l+1)
         if(m.lt.0.or.n.lt.0) then
            call error(60)
            return
         endif
         if (m*n.eq.0) then
            top=top0
            il=iadr(lstk(top))
            istk(il)=algtype(PCALGCODE,1)
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)+1
            return
         endif
         top=top-1
         lw=lstk(top+1)
      else
         il=iadr(lstk(top))
         if(istk(il+1)*istk(il+2).eq.0) then
            top=top-1
            return
         endif
      endif
      
      ilij=iadr(lstk(top-1))
      if(typecode(istk(ilij)).ne.1.or.istk(ilij+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      if(istk(ilij+1).ne.0.and.
     $     istk(ilij+1).ne.2.and.istk(ilij+2).ne.2) then
         err=2
         call error(60)
         return
      endif
      nel=istk(ilij+1)*istk(ilij+2)/2
      lij=sadr(ilij+4)
      ilij=iadr(lij)
      if(nel.gt.0) then
         call entier(nel*2,stk(lij),istk(ilij))
      endif
      if(rhs.eq.2) then
         mm=0
         do 10 i=0,nel-1
            mm=max(mm,istk(ilij+i))
 10      continue
      else
         mm=m
      endif
c     
      ilv=iadr(lstk(top))
c      if(typecode(istk(ilv)).ne.1.and.typecode(istk(ilv)).ne.4) then
c         buf='we should not be here in intmpsparse'
c         call error(9999)
c         call funnam(ids(1,pt+1),'sparse',iadr(lstk(top)))
c         fun=-1
c         top=tops
c         return
c      endif
      if(istk(ilv+1)*istk(ilv+2).ne.nel) then
         err=2
         call error(60)
         return
      endif
      il1=ilij
      if(typecode(istk(ilv)).eq.1) then
         itv=istk(ilv+3)
         lv=sadr(ilv+4)
         lind=iadr(max(lw,sadr(il1+5+mm+nel)+nel*(itv+1)))
         liw=lind+mm+nel
         lw=sadr(liw+nel)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         nel0=nel
         if(itv.eq.0) then
            call dij2sp(m,n,nel,istk(ilij),stk(lv),
     $           istk(lind),mm+nel0,istk(liw),ierr)
         else
            call wij2sp(m,n,nel,stk(lij),stk(lv),stk(lv+nel0),
     $           istk(lind),mm+nel0,istk(liw),ierr)
         endif
         if(ierr.eq.2) then
            buf='not enough memory'
            call error(9999)
            return
         elseif(ierr.eq.1) then
            call error(42)
            return
         elseif(ierr.eq.3) then
            call error(221)
            return
         endif
         top=top-1
         il1=iadr(lstk(top))
         istk(il1)=algtype(PCALGCODE,5)
         istk(il1+1)=m
         istk(il1+2)=n
         istk(il1+3)=itv
         istk(il1+4)=nel
         ilr=il1+5
         l=sadr(ilr+m+nel)
         inc=1
         if(l.gt.lv) inc=-1
         call dcopy(nel,stk(lv),inc,stk(l),inc)
         if(itv.eq.1) then
            call dcopy(nel,stk(lv+nel0),inc,stk(l+nel),inc)
         endif
         lstk(top+1)=l+nel*(itv+1)
         inc=1
         if(ilr.gt.lind) inc=-1
         call icopy(m+nel,istk(lind),inc,istk(ilr),inc)
         return
      else
         lv=ilv+3
         lind=iadr(max(lw,sadr(il1+5+mm+nel)))
         liw=lind+mm+nel
         lw=sadr(liw+nel)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         nel0=nel
         call lij2sp(m,n,nel,istk(ilij),istk(lv),
     $        istk(lind),mm+nel,istk(liw),ierr)
         if(ierr.eq.2) then
            buf='not enough memory'
            call error(9999)
            return
         elseif(ierr.eq.1) then
            call error(42)
            return
         elseif(ierr.eq.3) then
            call error(221)
            return
         endif
         top=top-1
         il1=iadr(lstk(top))
         istk(il1)=6
         istk(il1+1)=m
         istk(il1+2)=n
         istk(il1+3)=0
         istk(il1+4)=nel
         ilr=il1+5
         l=sadr(ilr+m+nel)
         lstk(top+1)=l
         inc=1
         if(ilr.gt.lind) inc=-1
         call icopy(m+nel,istk(lind),inc,istk(ilr),inc)
         return
      endif
      end

      subroutine intmpspget(id)
      include '../stack.h'
      include 'mpstack.h'
      double precision tv
      integer id(nsiz),top0
      integer iadr, sadr,typecode,algtype
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      if (rhs .ne.1) then
         call error(39)
         return
      endif
      if (lhs .gt. 3) then
         call error(41)
         return
      endif 
      il=iadr(lstk(top))
      ityp=istk(il)
c      if(typecode(ityp).ne.5.and.typecode(ityp).ne.6) then
c         buf='we should not be here in intmpspget'
c         call error(9999)
c         call funnam(ids(1,pt+1),'spget',iadr(lstk(top)))
c         fun=-1
c         return
c      endif
      nel=istk(il+4)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      ilr=il+5
      ilc=ilr+m
      l=sadr(ilc+nel)

      if(nel.eq.0) then
         istk(il)=algtype(PCALGCODE,1)
         istk(il+1)=0
         istk(il+2)=0
         istk(il+3)=0
         lstk(top+1)=sadr(il+4)
         if(lhs.ge.2) then
            top=top+1
            il=iadr(lstk(top))
            istk(il)=algtype(PCALGCODE,1)
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
         endif
         if(lhs.eq.3) then
            top=top+1
            il=iadr(lstk(top))
            istk(il)=algtype(PCALGCODE,1)
            istk(il+1)=1
            istk(il+2)=2
            istk(il+3)=0
            l=sadr(il+4)
            stk(l)=m
            stk(l+1)=n
            lstk(top+1)=l+2
         endif
         return
      endif
      lij=sadr(il+4)
      ilv=iadr(lij+2*nel)
      if(typecode(ityp).eq.5) then
         lv=sadr(ilv+4)
         ilrs=iadr(max(lw,lv+nel*(it+1)))
      else
         ilrs=iadr(max(lw,lij+2*nel))
      endif
      lw=sadr(ilrs+m+nel)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     sauvegarde
      call icopy(m+nel,istk(ilr),1,istk(ilrs),1)
c     constitution des variables de sortie
      if(typecode(ityp).eq.5) then
         if(l.ge.lv) then
            call dcopy(nel*(it+1),stk(l),1,stk(lv),1)
         else
            call dcopy(nel*(it+1),stk(l),-1,stk(lv),-1)
         endif
      endif
      call int2db(nel,istk(ilrs+m),1,stk(lij+nel),1)
      i1=0
      do 30 i=1,m
         if(istk(ilrs-1+i).ne.0) then
            tv=i
            call dset(istk(ilrs-1+i),tv,stk(lij+i1),1)
            i1=i1+istk(ilrs-1+i)
         endif
 30   continue
      istk(il)=1
      istk(il+1)=nel
      istk(il+2)=2
      istk(il+3)=0
      lstk(top+1)=lij+2*nel
      if(lhs.ge.2) then
         top=top+1
         if(typecode(ityp).eq.5) then
            il=iadr(lstk(top))
            istk(il)=algtype(PCALGCODE,1)
            istk(il+1)=nel
            istk(il+2)=1
            istk(il+3)=it
            lstk(top+1)=lv+nel*(it+1)
         else
            buf='we should not be here in intmpspget'
            call error(9999)
            return
         endif
      endif
      if(lhs.eq.3) then
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=2
         istk(il+3)=0
         l=sadr(il+4)
         stk(l)=m
         stk(l+1)=n
         lstk(top+1)=l+2
      endif
      return
      end

      subroutine intmpfull(id)
      include '../stack.h'
      include 'mpstack.h'
      integer id(nsiz),top0
      integer iadr, sadr,typecode,algtype
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c      write(*,*)'intmpfull/0'
      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif 
c      write(*,*)'intmpfull/1'
      il=iadr(lstk(top))
      if(typecode(istk(il)).eq.1.or.typecode(istk(il)).eq.2) return
c      if(typecode(istk(il)).ne.5.and.typecode(istk(il)).ne.6) then
c         buf='we should not be here in intmpfull'
c         call error(9999)
c         call funnam(ids(1,pt+1),'spfull',iadr(lstk(top)))
c         fun=-1
c         return
c      endif
      nel=istk(il+4)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      ilr=il+5
      ilc=ilr+m
c      write(*,*)'intmpfull/2 itsk(il)=',istk(il),typecode(istk(il))
      if(typecode(istk(il)).eq.5) then
c     matrix of scalar
c         write(*,*)'intmpfull/3'
         l=sadr(ilc+nel)
         ils=iadr(max(sadr(il+4)+m*n*(it+1),lw))
         ls=sadr(ils+m+nel)
         lw=ls+nel*(it+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(m+nel,istk(ilr),1,istk(ils),1)
         call dcopy(nel*(it+1),stk(l),1,stk(ls),1)
         istk(il)=algtype(PCALGCODE,1)
         l=sadr(il+4)
         if(it.eq.0) then
            call mpdspful(m,n,stk(ls),nel,istk(ils),stk(l))
         else
            call mpwspful(m,n,stk(ls),stk(ls+nel),nel,istk(ils),
     $           stk(l),stk(l+m*n))
         endif
         lstk(top+1)=l+m*n*(it+1)
      else
c         write(*,*)'intmpfull/4'
c     matrix of boolean
         buf='we should not be here in intmpful'
         call error(9999)
         return
      endif
      return
      end


