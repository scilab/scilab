      subroutine comand(id)
C     ====================================================================
C     Scilab Command and Keyword 
C     ====================================================================
C     id(nsiz) coded name of the comand 
c     Copyright INRIA
      include '../stack.h'
      logical compil
C     
      integer lunit,mode(2)
C     
      integer cmdl
      parameter (cmdl = 27)
      parameter (nz1 = nsiz-1, nz2 = nsiz-2, nz3 = nsiz-3)
      parameter (iif=1,iwhile=2,iselect=3)
C     
      integer cmd(nsiz,cmdl),a,blank,name
      integer id(nsiz),ennd(nsiz),sel(nsiz),while(nsiz),for(nsiz)
      integer iff(nsiz),func(nsiz),endfunc(nsiz)
      integer semi,comma,eol,percen,lparen,slash
      integer count,equal,nchar,pchar
      logical eqid,cresmat
      integer iadr
      common/cmds/cmd
      save cmds
C     
      data a/10/
      data eol/99/,semi/43/,comma/52/,lparen/41/,equal/50/,slash/48/
      data blank/40/,name/1/,percen/56/
      data ennd/671946510,nz1*673720360/
      data sel/236260892,673717516,nz2*673720360/
      data while/353505568,673720334,nz2*673720360/
      data iff/673713938,nz1*673720360/
      data for/672864271,nz1*673720360/
      data func/202841615,387453469,nz2*673720360/
      data endfunc/252516110,487331614,672602130,nz3*673720360/ 
C     ------command names--------
C     if        else      for
C     while     end       select
C     case      quit      exit
C     return    help      what
C     who       
C     pause     clear     resume
C     then      do
C     apropos   abort     break
C     elseif    pwd       clc
C     continue
C     
      data ((cmd(i,j), i = 1,nsiz), j = 1,10)/
     &     673713938,nz1*673720360,
     &     236721422,nz1*673720360,
     &     672864271,nz1*673720360,
     &     353505568,673720334,nz2*673720360,
     &     671946510,nz1*673720360,
     $     236260892,673717516,nz2*673720360,
     &     236718604,nz1*673720360,
     $     487726618,nz1*673720360,
     &     487727374,nz1*673720360,
     $     505220635,673715995,nz2*673720360/
      data ((cmd(i,j), i = 1,nsiz), j = 11,20)/
     &     420810257,nz1*673720360,
     &     487199008,nz1*673720360,
     &     672665888,nz1*673720360,
     &     471730713,673720334,nz2*673720360,
     &     168695052,673720347,nz2*673720360,
     &     505155099,673713686,nz2*673720360,
     &     386797853,nz1*673720360,
     &     673716237,nz1*673720360,
     &     404429066,672929817,nz2*673720360,
     &     454560522,673720349,nz2*673720360/
      data ((cmd(i,j), i = 1,nsiz), j = 21,27)/
     &     168696587,673720340,nz2*673720360,
     &     236721422,673713938,nz2*673720360,
     &     671948825,nz1*673720360,
     &     202841615,387453469,nz2*673720360,
     &     252516110,487331614,672602130,nz3*673720360,
     &     671880460,nz1*673720360,
     &     488052748,236853010,nz2*673720360/
C     

      iadr(l) = l + l - 1
C     
      if (ddt .eq. 4) then
         call cvname(id,buf,1)
         call basout(io,wte,' comand   : '//buf(1:nlgh))
      endif
C     
      kcont=27
      kbrk=21

      fun = 0


      do 10 k = 1,cmdl
         if (eqid(id,cmd(1,k))) then
            if(k.eq.15.or.(k.ge.11.and.k.le.13).or.k.eq.19.or.
     *           k.eq.26) goto 11
            goto 15
         endif
 10   continue

c     form function like call
      fin = 0
      nchar=lin(lpt(4))
      if(char1.eq.comma.or.char1.eq.semi.or.char1.eq.eol) goto 11
      if (char1.eq.slash.and.lin(lpt(4)).eq.slash) goto 11
      if(lpt(4).ge.2) then
         pchar=lin(lpt(4)-2)
         if(pchar.ne.blank) return
      endif
 11   continue
      fin=0
      if(char1.eq.lparen.or.char1.eq.equal) return
      rhs=0
      call funs(id)
      if(fin.eq.0) then
         if(comp(1).eq.0) then
            fin=-4
            call funs(id)
            if(fin.eq.0) return
            sym=char1
            call cmdstr
         else
            sym=char1
            call cmdstr
            fin=-2
            call stackg(id)
            if(err.gt.0) return
            fun=-1
         endif
      else
         if(char1.eq.comma.or.char1.eq.semi.or.char1.eq.eol) then
            sym=char1
            return
         endif
         if (char1.eq.slash.and.lin(lpt(4)).eq.slash) then
            sym=comma
            return
         endif
         call cmdstr
      endif

      return
C     
 15   if (fin .eq. -1) return
C     
      fin = 1
C     mots cles if  then else for do  while end case selec
      goto (32,33,30,31,35,36,37) k
      goto (42,42) k-16
C     
      goto (50,55,45,16,16,16,20,16,45,16,
     &     16,16,120,130,38,140,150,16,16,130) k-7
 16   call error(16)
      return
C     
C     -----
C     pause
C     -----
C     
 20   continue
c     if special compilation mode skip  comands
      if (comp(3).eq.1) then
         fin=0
         fun=0
         return
      endif
      if (char1.ne.eol .and. char1.ne.comma .and. char1.ne.semi) then
         if (.not.(char1.eq.slash.and.lin(lpt(4)).eq.slash)) then
            call error(16)
            return
         endif
      endif
C     compilation de pause:<12>
      if (compil(12,0,0,0,0)) return
      fin = 3
      goto 999
C     

C     
C     ---------------------------------------
C     for, while, if, else, end, select, case, elseif
C     ---------------------------------------
C     
 30   fin = -11
      goto 999
 31   fin = -12
      goto 999
 32   fin = -13
      goto 999
 33   fin = -14
      if (pt .eq. 0) then
         goto 42
      elseif (abs(rstk(pt)) .ne. 805) then
         goto 42
      endif
      goto 999
 35   fin = -10
      if (pt .eq. 0) then
         goto 42
      elseif (abs(rstk(pt)).ne.805 .and. abs(rstk(pt)).ne.802 .and.
     &        abs(rstk(pt)).ne.806) then
         goto 42
      endif
      goto 999
 36   fin = -15
      goto 999
 37   fin = -16
      if (pt .eq. 0) then
         goto 42
      elseif (abs(rstk(pt)).ne.805 .or. ids(1,pt).ne.iselect) then
         goto 42
      endif
      goto 999
 38   fin = -17
      if (pt .eq. 0) then
         goto 42
      elseif (abs(rstk(pt)) .ne. 805) then
         goto 42
      endif
      goto 999
C     
 42   call error(34)
      return
C     
C     -------------
C     return/resume
C     -------------
C     
 45   continue
      if (char1 .eq. lparen) then
C     return/resume avec rhs et sans lhs --> fonction et non commande
         fin = 0
         goto 999
      endif
C     compilation return:<99>
      if (compil(99,0,0,0,0)) return
 46   continue
      k = lpt(1) - (13+nsiz)
      if (pt.eq.0 .or. k.le.0) goto 998
      pt = pt + 1
 47   pt = pt - 1
      if (pt .eq. 0) goto 48
      if (rstk(pt).eq.802 .or. rstk(pt).eq.612 .or.
     &     (rstk(pt).eq.805.and.ids(1,pt).eq.iselect) .or.
     &     (rstk(pt).eq.616.and.pstk(pt).eq.10)) top = top - 1
      ir = rstk(pt) / 100
      if (ir .ne. 5) goto 47
 48   continue
      fin = 2
      lhs = 0
      goto 999
C     
C     -------------
C     quit
C     -------------
C     
 50   continue
c     if special compilation mode skip  comands
      if (comp(3).eq.1) then
         fin=0
         fun=0
         return
      endif
C     compilation quit:<17>
      if (compil(17,0,0,0,0)) return
      if (paus .ne. 0) then
C     quit dans une pause
         paus = paus - 1
         pt = pt + 1
 51      pt = pt - 1
         if (rstk(pt) .ne. 503) goto 51
         k = lpt(1) - (13+nsiz)
         lpt(1) = lin(k+1)
         lpt(2) = lin(k+4)
         lpt(6) = k
         bot = lin(k+5)
         pt = pt - 1
         rio = pstk(pt)
         if (rstk(pt) .eq. 701) pt = pt - 1
         goto 46
      else
C     quit (sortie)
         fun = 99
      endif
      goto 998

C     
C     -------------
C     exit
C     -------------
C     
 55   continue
c     if special compilation mode skip  comands
      if (comp(3).eq.1) then
         fin=0
         fun=0
         return
      endif
C     compilation exit:<20>
      if (compil(20,0,0,0,0)) return
      if (niv.gt.0) then
         call sciquit
         stop
      endif
      fun = 99
      goto 998
C     
C     pwd
C     ---
 140  continue
      if(char1.eq.lparen) then
         fin=0
         fun=0
         return
      endif
c     if special compilation mode skip  comands
      if (comp(3).eq.1) then
         fin=0
         fun=0
         return
      endif
      fun=13
      fin=44
      rhs=0
      return
C     

 60   continue
      
C     
C     abort
C     -----
C     
 120  continue
c     if special compilation mode skip  comands
      if (comp(3).eq.1) then
         fin=0
         fun=0
         return
      endif
      if (compil(14,0,0,0,0)) return
C     compilation abort:<14>
      pt = pt + 1
 121  pt = pt - 1
      if (pt .eq. 0) goto 122
      if (int(rstk(pt)/100) .eq. 5) then
         k = lpt(1) - (13+nsiz)
         lpt(1) = lin(k+1)
         lpt(2) = lin(k+2)
         lpt(3) = lin(k+3)
         lpt(4) = lin(k+4)
         lct(4) = lin(k+6)
         lpt(6) = k
         if (rstk(pt) .le. 502) then
c     . abort in a  macro  an execstr or external
            if(pt.gt.1) then
               if(rstk(pt-1).eq.1002) then
c     .         abort in syncexec, make current parser finish
                  fin = 4
                  fun = 99
                  macr=macr-1
                  lct(8) = 0
                  lct(4)=pstk(pt)
                  pt = pt-1
                  goto 999
               elseif(rstk(pt-1).ne.903.and.rstk(pt-1).ne.909.and.
     $                 rstk(pt-1).ne.706) then
c     .         abort in a macro
                  bot = lin(k+5)
               endif
            else
c     .      abort in a macro
               bot = lin(k+5)
            endif
         elseif (rstk(pt) .eq. 503) then
            if (rio .eq. rte) then
c     .     abort in a pause
               rio = pstk(pt-1)
               paus = paus - 1
               bot = lin(k+5)
            else
c     .     abort in an exec
               mode(1)=0
               call clunit(-rio,buf,mode)
               rio = pstk(pt-1)
            endif
         endif
      elseif  (rstk(pt) .eq. 1001) then
c     .  abort in an external unstack it
         niv=niv-1
      endif
      goto 121
 122  continue
      lct(8) = 0
      fin = 4
      comp(1) = 0
      if (niv .gt. 0) err = 9999999
      goto 999
C     
C     break, continue
C------
 130  continue
      kcmd=k
c     if special compilation mode skip  comands
      if (comp(3).eq.1) then
         fin=0
         fun=0
         return
      endif
      if(kcmd.eq.kcont) then
C     compilation de continue:<13>
         if(compil(28,0,0,0,0)) return
      else
C     compilation de break:<13>
         if (compil(13,0,0,0,0)) return
      endif
      count = 0
      pt = pt + 1
 131  pt = pt - 1
      if (pt .eq. 0) then
         pt = 1
         call putid(ids(1,pt),cmd(1,kcmd))
         call error(72)
         return
      endif
C     
      ir = rstk(pt) / 100
c     96
      if (ir .eq. 5) then
         call putid(ids(1,pt),cmd(1,kcmd))
         call error(72)
         return
      endif
c     
      if (ir .ne. 8) goto 131
      count = count + 1
      if (rstk(pt) .eq. 802) then
C     .  break or continue in a for loop
         if (kcmd.eq.kbrk) then
            top = top - 1
            pt = pt - 2
         endif
      elseif(ids(1,pt).eq.iselect) then
c     .  discard select variable
         top = top - 1
         goto 131
      elseif (ids(1,pt).eq.iwhile) then
C     .  break or continue in a  while
         if (kcmd.eq.kbrk) pt = pt - 1
      elseif (int(rstk(pt)/100) .eq. 5) then
         call putid(ids(1,pt),cmd(1,kcmd))
         call error(72)
         return
      else
         goto 131
      endif
 132  continue
c     call getsym
      call skpins(1)
      count = count - 1
      if (count .gt. 0) goto 132
C     char1=blank
      if (kcmd.eq.kbrk) then
         fin = 1
      else
         fin = -10
      endif
      return
      

 150  if (eqid(id,func)) then
c     .  inline function definition
         call getfunction
         if(err.gt.0) return
         call getsym
         rhs=2
c     if (.false.) then
c     c     .     next lines for compilation option
c     top=top+1
c     if (.not.cresmat("getfunction",top,1,1,1)) return
c     l=iadr(lstk(top))+6
c     istk(l)=23
c     rhs=3
c     endif
c     .
         lhs=1

         fun=5
         fin=11
         go to 999
      endif


C     
C     
C     fin
C     ---
C     
 998  continue
      fin = 0
      call getsym
      fin = 1
C     
 999  continue
      lunit = 0
      end

