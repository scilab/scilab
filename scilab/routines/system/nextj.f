      subroutine nextj(id,j)
c     ==============================================================
C     extracts the jth occurence of x in do x=val and stores its value on top 
C     of the stack 
c     ==============================================================
c     Copyright INRIA
      include '../stack.h'
c
      double precision dlamch,x
      integer id(nsiz),j,vt,semi
c
      integer ogettype,lr,lc,lr1,lc1,v
      character*4 name
      logical eqid
      logical getmat,cremat,smatj,lmatj,getsmat,getilist,getpoly,pmatj
c
      integer iadr,sadr
      data semi/43/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') j
         call cvname(id,buf(5:4+nsiz*4),1)
         call basout(io,wte,' nextj j:'//buf(1:4)//' var:'//
     $        buf(5:4+nsiz*4))
      endif
c
      j = j + 1
      if(comp(1).ne.0) return
      top = top + 1
      vt=ogettype(top-1)
      goto (10,20,2,2,2,2,2,2,2,30,2,2,2,2,40) vt
 2    err=vt
      call error(76)
      return
c---  matrices scalaires 
 10   if (.not.getmat("nextj",top-1,top-1,it,m,n,lr,lc)) return

      if (m.eq.-3) then
C        boucle implicite 
         x = stk(lr) + (j - 1)*stk(lr + 1)
         if( stk(lr+1) * (x - stk(lr+2)) .gt. 0.0d0) then
            if(abs(x-stk(lr+2)).ge.
     $           2.0d0*max(abs(stk(lr+1)),abs(stk(lr+2)))*dlamch('p'))
     $           goto 50
         endif
C     The accelaration expected of the following lines is not obvious!
c         if (j.gt.1) then
c     .     check if loop variable has moved or be modified since previous j
C             k=idstk(1,top-1)
C             if(k.ge.bot.and.eqid(id,idstk(1,k))) then
C c     .        check header
C                il=iadr(lstk(k))
C                if (istk(il).eq.1.and.istk(il+1).eq.1.and.
C      $              istk(il+2).eq.1.and.istk(il+3).eq.0) then
C c     .            No, loop variable is updated in place
C                   lr1=sadr(il+4)
C                   stk(lr1)=x
C                   top=top-1
C                   return
C                endif
C             endif
C          endif
         if (.not.cremat("nextj",top,0,1,1,lr1,lc1)) return
         stk(lr1)=x
      else
         if (j .gt. n .or. m.eq.0) go to 50
C     The accelaration expected of the following lines is not obvious!
C          if (j.gt.1) then
C             k=idstk(1,top-1)
C             if(k.ge.bot.and.eqid(id,idstk(1,k))) then
C c     .        check header
C                il=iadr(lstk(k))
C                if (istk(il).eq.1.and.istk(il+1).eq.m.and.
C      $              istk(il+2).eq.1.and.istk(il+3).eq.it) then
C c     .           loop variable is updated in place
C                   lr1=sadr(il+4)
C                   call unsfdcopy(m,stk(lr+(j-1)*m),1,stk(lr1),1)
C                   if(it.eq.1) call unsfdcopy(m,stk(lc+(j-1)*m),1,
C      $                 stk(lr1+m),1)
C                   top=top-1
C                   return
C                endif
C             endif
C          endif
         if (.not.cremat("nextj",top,it,m,1,lr1,lc1)) return
         call unsfdcopy(m,stk(lr+(j-1)*m),1,stk(lr1),1)
         if(it.eq.1) call unsfdcopy(m,stk(lc+(j-1)*m),1,stk(lr1+m),1)
      endif

      goto 21
c--   matrices de polynomes
 20   if (.not.getpoly("nextj",top-1,top-1,it,m,n,name,namel,ilp,lr,lc))
     $     return 
      if(j.gt.n) goto 50
      if (.not.pmatj("nextj",top,j)) return 
      goto 21
c---  chaines de caracteres
 30   if (.not.getsmat("nextj",top-1,top-1,m,n,1,1,lr,nlj)) return
      if ( j .gt.n) goto 50
      if (.not.smatj("nextj",top,j)) return 
      goto 21
c---- listes
 40   if (.not.getilist("nextj",top-1,top-1,m,j,ilj)) return 
      if(j.gt.m) goto 50
      if (.not.lmatj("nextj",top,j)) return 
      goto 21
 21   rhs = 0
      sym=semi
      if (j.gt.1) then
         k=idstk(1,top-1)
         v=lstk(top+1)-lstk(top)
         if(k.ge.bot.and.eqid(id,idstk(1,k))
     $        .and.v.eq.lstk(k+1)-lstk(k)) then
            call unsfdcopy(v,stk(lstk(top)),-1,stk(lstk(k)),-1)
            top=top-1
            return
         endif
      endif
      call stackp(id,0)
c     save location where loop variable has been saved in the expression
c     identifier 
      idstk(1,top) = fin
      return
 50   continue
      rhs = 0
C     the next commented lines are replaced by top=top-2 not to remove
c     loop variable at the end of the loop
c      top=top-1
c      il = iadr(lstk(top))
c      istk(il) = 0
c      sym=semi
c      call stackp(id,0)
      top=top-2
      j = 0
      return
      end
