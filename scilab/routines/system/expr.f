      subroutine expr
c     
c     analyseur d'expressions
c     
c     Copyright INRIA
      INCLUDE '../stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2)
c     
      integer op,r,blank,sign,plus,minus,name,colon,eye(nsiz),ou,et
      integer equal,less,great,not,eol
      logical eptover
      data colon/44/,blank/40/,plus/45/,minus/46/,ou/57/,et/58/
      data equal/50/,less/59/,great/60/,not/61/
      data eye/672014862,nz1*673720360/,name/1/,eol/99/
c     
      if (ddt .eq. 4) then
         write(buf(1:8),'(2i4)') pt,rstk(pt)
         call basout(io,wte,' expr   pt:'//buf(1:4)//' rstk(pt):'
     &        //buf(5:8))
      endif
c     
      r = rstk(pt)
      ir=r/100
      if(ir.ne.1) goto 01
      goto(05,06,25,26,61,73,74,82,83,86,87,102,104,105),r-100
c     
 01   if(sym.ge.ou.and.sym.le.great) then
         call error(40)
         return
      endif
 02   continue
      kount = 1
      if(sym.eq.not) goto 70
      if (sym .eq. colon) call putid(syn(1),eye)
c      if (sym .eq. colon) sym = name
 03   sign = plus
      if (sym .eq. minus) sign = minus
      if (sym.eq.plus .or. sym.eq.minus) call getsym
      if ( eptover(1,psiz-1)) return
      pstk(pt) = sign + 256*kount
      rstk(pt) = 101
      icall=2
c     *call* term
      return
 05   sign = mod(pstk(pt),256)
      kount = pstk(pt)/256
      pt = pt-1
      if (sign .ne. minus) goto 10
      rhs=1
      pt=pt+1
      pstk(pt)=kount
      fin=minus
      rstk(pt)=102
      icall=4
c     *call* allops(minus)
      return
 06   kount=pstk(pt)
      pt=pt-1
 10   if (sym.eq.plus .or. sym.eq.minus) go to 20
      if(rstk(pt).eq.113) goto 104
      if(rstk(pt).eq.112) goto 102
      if(sym.eq.eol) goto 50
      if(sym.ge.ou.or.sym.eq.equal) goto 70
      go to 50
 20   if (rstk(pt) .ne. 301) go to 21
c     blank or tab is delimiter inside angle brackets
      ls = lpt(3) - 2
      if (abs(lin(ls)).eq.blank.and.abs(lin(lpt(3))).ne.blank) go to 50
 21   op = sym
      call getsym
      pt = pt+1
      pstk(pt) = op + 256*kount
      rstk(pt) = 103
      icall=2
c     *call* term
      return
 25   op = mod(pstk(pt),256)
      kount = pstk(pt)/256
      rhs=2
      pstk(pt)=kount
      rstk(pt)=104
      fin=op
      icall=4
c     *call* allops(op)
      return
 26   kount=pstk(pt)
      pt=pt-1
      go to 10
 50   if (sym .ne. colon) go to 60
      call getsym
      kount = kount+1
      go to 03
 60   if (kount .gt. 3) then
         call error(33)
         if (err .gt. 0) return
      endif
      rhs = kount
      if (kount .le. 1) return
      pt=pt+1
      rstk(pt)=105
      fin=colon
      icall=4
c     *call* allops(colon)
      return
 61   pt=pt-1
      r = rstk(pt)
      ir=r/100
      if(ir.ne.1) return
      goto(05,06,25,26,61,73,74,82,83,86,87,102,104,105),r-100
c     
c     in-line lexpr
 70   continue
      if (ddt .eq. 4) then
         write(buf(1:8),'(2i4)') pt,rstk(pt)
         call basout(io,wte,' lexpr  pt:'//buf(1:4)//' rstk(pt):'
     &        //buf(5:8))
      endif
 72   continue
      if (eptover(1,psiz-1)) return
      if(sym.eq.ou) then
c     call getsym
         pstk(pt)=ou
      else
         pstk(pt)=0
      endif
      pstk(pt)=pstk(pt)+256*kount
      rstk(pt)=106
c     *call* lterm
      goto 80
 73   continue
      op = mod(pstk(pt),256)
      kount = pstk(pt)/256
      pt=pt-1
      if(op.eq.0) goto 75
      icall=4
      fin=ou
      rhs=2
      pt=pt+1
      rstk(pt)=107
      pstk(pt)=kount
c     *call* allops(ou)
      return
 74   continue
      kount=pstk(pt)
      pt=pt-1
 75   if(sym.eq.ou) goto 72
      goto 50
c     
c     in-line lterm
 80   continue
      if (ddt .eq. 4) then
         write(buf(1:8),'(2i4)') pt,rstk(pt)
         call basout(io,wte,' lterm   pt:'//buf(1:4)//' rstk(pt):'
     &        //buf(5:8))
      endif
 81   continue
      if (eptover(1,psiz-1))  return
      if(sym.eq.et) then
         pstk(pt)=et
      else
         pstk(pt)=0
      endif
      pstk(pt)=pstk(pt)+256*kount
      rstk(pt)=108
c     *call* lfact
      goto 85
 82   continue
      op = mod(pstk(pt),256)
      kount = pstk(pt)/256
      pt=pt-1
      if(op.eq.0) goto 84
      icall=4
      fin=et
      rhs=2
      pt=pt+1
      pstk(pt)=kount
      rstk(pt)=109
c     *call* allops(et)
      return
 83   continue
      kount=pstk(pt)
      pt=pt-1
 84   if(sym.ne.et) goto 73
      goto 81
c     
c     in-line lfact
c     
 85   continue
      if (ddt .eq. 4) then
         write(buf(1:8),'(2i4)') pt,rstk(pt)
         call basout(io,wte,' lfact  pt:'//buf(1:4)//' rstk(pt):'
     &        //buf(5:8))
      endif
      if ( eptover(1,psiz-1)) return
      pstk(pt)=0
      if(sym.eq.not.and.char1.ne.equal) then
         pstk(pt)=not
         call getsym
      endif
      pstk(pt)=pstk(pt)+256*kount	  
      rstk(pt)=110
c     *call* lprim
      goto 100
 86   continue
      op = mod(pstk(pt),256)
      kount = pstk(pt)/256
      pt=pt-1
      if(op.eq.0) goto 82
      fin=op
      rhs=1
      pt=pt+1
      pstk(pt)=kount
      rstk(pt)=111
      icall=4
c     *call* allops(not)
      return
 87   kount=pstk(pt)
      pt=pt-1
      goto 82
c     
c     in-line lprim
c     
 100  continue
      if (ddt .eq. 4) then
         write(buf(1:8),'(2i4)') pt,rstk(pt)
         call basout(io,wte,' lprim  pt:'//buf(1:4)//' rstk(pt):'
     &        //buf(5:8))
      endif
      if(mod(pstk(pt),256).ne.0) goto 101
      if(sym.ne.et.and.sym.ne.ou) goto 103
      call getsym
C modif SS
C      if(sym.ge.ou.or.sym.eq.equal) goto 70
      if ( eptover(0,psiz-1)) return
 101  pt=pt+1
      rstk(pt)=112
      icall=1
c     *call* expr
      goto 02
 102  pt=pt-1
      if(sym.ne.equal.and.sym.lt.less.or.sym.eq.eol)  goto 86
 103  op=sym
      call getsym
c      if(rstk(pt-3).ne.803) then
         if(op.eq.equal.and.sym.ne.equal) then
            call msgs(7,0)
         endif
c      endif

      if(sym.eq.equal.or.sym.eq.great) then
         if(op.ne.equal) op=op+sym
         call  getsym
         if(op.eq.not+equal) op=less+great
      endif
      pt=pt+1
      rstk(pt)=113
      pstk(pt)=op
      icall=1
c     *call* expr
      goto 02
 104  fin=pstk(pt)
      rhs=2
      rstk(pt)=114
      icall=4
c     *call* allops(fin)
      return
 105  continue
      goto 102
c     
c 999  call error(22)
c      if (err .gt. 0) return
c      return
      end
