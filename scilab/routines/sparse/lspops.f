      subroutine lspops
c     
c     operations on boolean sparse matrices
c     
c     Copyright INRIA
      include '../stack.h'
      integer op
c     
      integer iadr,sadr
c     
      integer star,dstar,dot,colon
      integer less,great,equal,ou,et,non
      integer insert,extrac
      integer top0
      logical isany
c
      data star/47/,dstar/62/,dot/51/,colon/44/
      data less/59/,great/60/,equal/50/
      data ou/57/,et/58/,non/61/
      data insert/2/,extrac/3/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      op=fin
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' lspops op: '//buf(1:4))
      endif
c     
      top0=top
      lw=lstk(top+1)+1
      if(op.eq.extrac) goto 70
      if(op.eq.insert) goto 80

      it2=0
      goto (04,03,02,01) rhs
      call error(39)
      return
c     
 01   il4=iadr(lstk(top))
      if(istk(il4).lt.0) il4=iadr(istk(il4+1))
      m4=istk(il4+1)
      n4=istk(il4+2)
      it4=istk(il4+3)
      if(istk(il4).eq.6) then
         nel4=istk(il4+4)
         irc4=il4+5
         l4=sadr(irc4+m4+nel4)
      else
         nel4=m4*n4
         l4=sadr(il4+4)
      endif
      mn4=m4*n4
      top=top-1
c     
 02   il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      if(istk(il3).eq.6) then
         nel3=istk(il3+4)
         irc3=il3+5
         l3=sadr(irc3+m3+nel3)
      else
         l3=sadr(il3+4)
         nel3=m3*n3
      endif
      mn3=m3*n3
      top=top-1
c     
 03   il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      if(istk(il2).eq.6) then
         nel2=istk(il2+4)
         irc2=il2+5
         l2=sadr(irc2+m2+nel2)
      else
         l2=sadr(il2+4)
         nel2=m2*n2
      endif
      mn2=m2*n2
      top=top-1
c     
 04   il1=iadr(lstk(top))
      il1r=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      if(istk(il1).eq.6) then
         nel1=istk(il1+4)
         irc1=il1+5
         l1=sadr(irc1+m1+nel1)
      else
         l1=sadr(il1+4)
         nel1=m1*n1
      endif
      mn1=m1*n1
      top=top-1
c     
c     operations binaires et ternaires
c     --------------------------------
c     
      top=top+1
      itr=max(it1,it2)
c     
      fun = 0
c     
c     column concatenation
      if(op.eq.1) goto 65
c     row concatenation
      if(op.eq.4) goto 66
c           :  +  -  * /  \  =          '
      goto(07,07,07,07,07,07,130,05,05,60) op+1-colon
      if(op.eq.ou.or.op.eq.et) goto 20
      if(op.eq.non) goto 30
c     
 05   if(op.eq.dstar) goto 07
      if(op.ge.3*dot+star) goto 07
      if(op.ge.2*dot+star) goto 07
      if(op.ge.less+equal) goto 130
      if(op.ge.dot+star) goto 07
      if(op.ge.less) goto 130

 06   call error(43)
      return
c
 07   fin=-fin
      top=top0
      go to 999
c     
c     ou/et logique
 20   if(istk(il1).ne.6.or.istk(il2).ne.6) then
         fin=-fin
         top=top0
         go to 999
      endif
      if(mn1.eq.1.and.mn2.gt.1) then
         top=top0
         fin=-fin
         return
      elseif(mn2.eq.1.and.mn1.gt.1) then
         top=top0
         fin=-fin
         return
      else if (n1 .ne. n2.or.m1.ne.m2) then
         call error(60)
         return
      endif
      irc=iadr(lw)
      nelmx=iadr(lstk(bot))-irc-m1-10
      lw=sadr(irc+m1+nelmx)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif  
      nel=nelmx
      if(fin.eq.ou) then
         call lspasp(m1,n1,nel1,istk(irc1),nel2,istk(irc2),nel,
     $        istk(irc),ierr)
      else
         call lspxsp(m1,n1,nel1,istk(irc1),nel2,istk(irc2),nel,
     $        istk(irc),ierr)
      endif
      if(ierr.ne.0) then
         call error(17)
         return
      endif
      istk(il1+3)=0
      istk(il1+4)=nel
      call icopy(m1+nel,istk(irc),1,istk(irc1),1)
      l1=sadr(irc1+m1+nel)
      lstk(top+1)=l1
      go to 999

c NOT
 30   continue
      lw=iadr(lstk(top+1))
      err=sadr(lw+m1*n1-nel1+m1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      istk(il1+4)=m1*n1-nel1
      ij2=lw
      ij1=irc1+m1
c     may be improved
      do 35 i=0,m1-1
         do 31 j=1,n1
            istk(ij2+j-1)=j
 31      continue
         do 32 j=1,istk(irc1+i)
            istk(ij2+istk(ij1+j-1)-1)=0
 32      continue
         j1=0
         do 33 j=1,n1
            if(istk(ij2+j-1).ne.0) then
               j1=j1+1
               istk(ij2+j1-1)=istk(ij2+j-1)
            endif
 33      continue
         ij2=ij2+j1
         ij1=ij1+istk(irc1+i)
         istk(irc1+i)=n1-istk(irc1+i)
 35   continue
      call icopy(ij2-lw,istk(lw),1,istk(irc1+m1),1)
      lstk(top+1)=sadr(irc1+m1+ij2-lw)
      goto 999

c     
c     
c     transposition
 60   istk(il1+1)=n1
      istk(il1+2)=m1
      if(nel1.eq.0) then
         lw=sadr(il1+5+n1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call iset(n1,0,istk(il1+5),1)
         lstk(top+1)=lw
         goto 999
      endif
      ia=iadr(lw)
      iat=ia+m1+1
      irc=iat+n1+1
      lw=sadr(irc+n1+nel1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(ia)=1
      do 61 i=1,m1
         istk(ia+i)=istk(ia+i-1)+istk(irc1+i-1)
 61   continue
      call lspt(m1,n1,nel1,istk(irc1),istk(ia),
     $     istk(iat),istk(irc))
      call icopy(n1+nel1,istk(irc),1,istk(irc1),1)
      l1=sadr(irc1+n1+nel1)
      lstk(top+1)=l1
      goto 999
c     
c     concatenation [a b]
 65   continue
      if(m1.lt.0.or.m2.lt.0) then
         call error(14)
         return
      endif
      if(m2.eq.0) then
         return
      elseif(m1.eq.0) then
         call icopy(5+m2+nel2,istk(il2),1,istk(il1),1)
         l1=sadr(il1+5+m2+nel2)
         lstk(top+1)=l1
         return
      elseif(m1.ne.m2) then
         call error(5)
         return
      endif
      if(istk(il1).ne.6.or.istk(il2).ne.6) then
         top=top0
         fin=-fin
         return
      endif
c
      nelr=nel1+nel2
      istk(il1+2)=n1+n2
      istk(il1+3)=0
      istk(il1+4)=nelr
      irc=iadr(lw)
      lw=sadr(irc+m1+nelr)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call lspcsp(0,m1,n1,nel1,istk(irc1),
     $     m2,n2,nel2,istk(irc2),
     $     nelr,istk(irc))
      call icopy(m1+nelr,istk(irc),1,istk(irc1),1)
      l1=sadr(irc1+m1+nelr)
      lstk(top+1)=l1
      return
c     
c     concatenation [a;b]
 66   continue
      if(n1.lt.0.or.n2.lt.0) then
         call error(14)
         return
      endif
      if(n2.eq.0) then
         goto 999
      elseif(n1.eq.0)then
         call icopy(5+m2+nel2,istk(il2),1,istk(il1),1)
         l1=sadr(il1+5+m2+nel2)
         lstk(top+1)=l1
         goto 999
      elseif(n1.ne.n2) then
         call error(6)
         return
      endif
      if(istk(il1).ne.6.or.istk(il2).ne.6) then
         top=top0
         fin=-fin
         return
      endif


      nelr=nel1+nel2
      istk(il1+1)=m1+m2
      istk(il1+3)=0
      istk(il1+4)=nelr
      irc=iadr(lw)
      lw=sadr(irc+m1+m2+nelr)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call lspcsp(1,m1,n1,nel1,istk(irc1),
     $        m2,n2,nel2,istk(irc2),
     $        nelr,istk(irc))
      call icopy(m1+m2+nelr,istk(irc),1,istk(irc1),1)
      l1=sadr(irc1+m1+m2+nelr)
      lstk(top+1)=l1
      goto 999
c     
c     extraction
c     
 70   continue
      if(rhs.gt.2) goto 75
c     arg2(arg1)
c     get arg2
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      nel2=istk(il2+4)
      irc2=il2+5
      l2=sadr(irc2+m2+nel2)
      mn2=m2*n2
      top=top-1
c     get arg1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
c
      if(mn2.eq.0) then 
c     .  arg2=[]
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)+1
         goto 999
      elseif(m2.lt.0) then
c     .  arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg2(:), just reshape to column vector
         if(n2.eq.1) then
            call icopy(5+m2+nel2,istk(il2),1,istk(ilrs),1)
            l1=sadr(ilrs+5+m2+nel2)
            lstk(top+1)=l1
         else
c     .     reshape to column vector
            ilrs=iadr(lstk(top))
            istk(ilrs)=6
            istk(ilrs+1)=mn2
            istk(ilrs+2)=1
            istk(ilrs+3)=0
            istk(ilrs+4)=nel2
            irc1=ilrs+5
            l1=sadr(ilrs+5+m2*n2+nel2)

            ircr=iadr(lw)
            iw=ircr+m2*n2+nel2
            lw=sadr(iw+3*nel2)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return 
            endif
            call lspmat(m2,n2,nel2,istk(irc2),m2*n2,istk(ircr),istk(iw))
            call icopy(m2*n2+nel2,istk(ircr),1,istk(irc1),1)
            lstk(top+1)=l1
         endif
         return
      elseif(m2.gt.1.and.n2.gt.1) then
c     .  call macro coded operation
         top=top0
         fin=-fin
         return
      endif
c     check and convert indices variable
      call indxg(il1,mn2,ilr,mi,mx,lw,1)
      if(err.gt.0) return
      if(mx.gt.mn2) then
         call error(21)
         return
      endif
 72   if(mi.eq.0) then
c     arg2([])
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)+1
         goto 999
      endif
c     set output sizes
      if (m2 .gt. 1.or.m1.lt.0) then
c     .  column vector
         m=mi
         n=-1
         mr = mi
         nr = 1
      else
c     .  row vector
         m=-1
         n=mi
         nr = mi
         mr = 1
      endif
      lptr=iadr(lw)
      irc=lptr+m2+1
      lw=sadr(irc+mr)
      nelr=iadr(lstk(bot))-iadr(lw)
      if(nelr.le.0) then
         err=lw-lstk(bot)
         call error(17)
         return
      endif
      lw=sadr(irc+mr+nelr)
      nel=nelr

      call lspe2(m2,n2,nel2,istk(irc2),
     $        istk(ilr),m,istk(ilr),n,mr,nr,
     $        nelr,istk(irc),istk(lptr),ierr)
      ilrs=iadr(lstk(top))
      istk(ilrs)=6
      istk(ilrs+1)=mr
      istk(ilrs+2)=nr
      istk(ilrs+3)=0
      istk(ilrs+4)=nelr
      call icopy(m+nelr,istk(irc),1,istk(ilrs+5),1)
      l1=sadr(ilrs+5+mr+nelr)
      lstk(top+1)=l1
      go to 999

c     
c     arg3(arg1,arg2)
 75   if(rhs.gt.3) then
         call error(36)
         return
      endif
c     get arg3
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      nel3=istk(il3+4)
      irc3=il3+5
      l3=sadr(irc3+m3+nel3)
      mn3=m3*n3
      top=top-1
c     get arg2
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      top=top-1
c     get arg1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)

      if(mn3.eq.0) then 
c     .  arg3=[]
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)+1
         goto 999
      elseif(m3.lt.0) then
c     .arg3=eye
         call error(14)
         return
      endif
c     check and convert indices variables
      call indxg(il1,m3,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mi.lt.0) then
         mr=mxi
      else
         mr=mi
      endif
      call indxg(il2,n3,ilj,nj,mxj,lw,1)
      if(err.gt.0) return
      if(nj.lt.0) then
         nr=mxj
      else
         nr=nj
      endif
c
 76   continue
      mn=mr*nr
      if(mn.eq.0) then 
c     .  arg1=[] or arg2=[] 
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)+1
         goto 999
      endif
      lptr=iadr(lw)
      irc=lptr+m3+1
      lw=sadr(irc+mr)
      nelr=iadr(lstk(bot))-iadr(lw)
      if(nelr.le.0) then
         err=lw-lstk(bot)
         call error(17)
         return
      endif
      lw=sadr(irc+mr+nelr)
      nel=nelr
      call lspe2(m3,n3,nel3,istk(irc3),istk(ili),mi,
     $        istk(ilj),nj,mr,nr,nelr,istk(irc),istk(lptr),ierr)
      ilrs=iadr(lstk(top))
      istk(ilrs)=6
      istk(ilrs+1)=mr
      istk(ilrs+2)=nr
      istk(ilrs+3)=0
      istk(ilrs+4)=nelr
      call icopy(mr+nelr,istk(irc),1,istk(ilrs+5),1)
      l1=sadr(ilrs+5+mr+nelr)
      lstk(top+1)=l1
      go to 999
c      
c     insert
 80   continue
      if(rhs.eq.4) goto 90
c     arg3(arg1)=arg2
c     get arg3      
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      if(istk(il3).eq.6) then
         nel3=istk(il3+4)
         irc3=il3+5
         l3=sadr(irc3+m3+nel3)
      else
         top=top0
         fin=-fin
         return
      endif
      mn3=m3*n3

c     get arg2
      top=top-1
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      if(istk(il2).eq.6) then
         nel2=istk(il2+4)
         irc2=il2+5
         l2=sadr(irc2+m2+nel2)
      elseif(istk(il2).eq.4) then
         l2=il2+3
         nel2=m2*n2
      elseif(istk(il2).eq.1.and.m2*n2.eq.0) then
         l2=sadr(il2+4)
         nel2=m2*n2
         istk(il2)=4
         l2=il2+3
      else
         top=top0
         fin=-fin
         return
      endif
      mn2=m2*n2

c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if (istk(il1).eq.10.or.istk(il1).eq.15) then
         top=top0
         fin=-fin
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)


      if (m2.eq.0) then
c     .  arg3(arg1)=[] -->[]
         if(m1.eq.-1) then
c     .    arg3(:)=[] 
            ilrs=iadr(lstk(top))
            istk(ilrs)=1
            istk(ilrs+1)=0
            istk(ilrs+2)=0
            istk(ilrs+3)=0
            lstk(top+1)=sadr(ilrs+4)+1
            goto 999
         elseif(m1.eq.0) then
c     .     arg3([])=[]  --> arg3
            call icopy(5+m3+nel3,istk(il3),1,istk(ilrs),1)
            l=sadr(ilrs+5+m3+nel3)
            lstk(top+1)=l
            goto 999
         else
c     .     arg3(arg1)=[]
            if(istk(il1).eq.4.and.m3.eq.m1.and.n3.eq.n1) then
               if(.not.isany(il1)) then
c     .           arg3([])=[]  --> arg3
                  call icopy(5+m3+nel3,istk(il3),1,istk(ilrs),1)
                  l=sadr(ilrs+5+m3+nel3)
                  lstk(top+1)=l
                  goto 999
               endif
            endif
c     .     arg3(arg1)=[] -->arg3(compl(arg1),:)
            if(m3.gt.1.and.n3.gt.1) then
c     .        call macro coded op to reshape and insert
               top=top0
               fin=-fin
               return
            else
               call indxgc(il1,mn3,ilr,mi,mx,lw)
               if(err.gt.0) return
               l2=l3
               n2=n3
               m2=m3
               mn2=m2*n2
               it2=it3
               nel2=nel3
               irc2=irc3
c     .     call extraction
               goto 72
            endif
         endif
      elseif(m2.lt.0.or.m3.lt.0) then
c     .  arg3=eye,arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg3(:)=arg2 reshape arg2 according to arg3
         if(mn2.ne.mn3) then
            call error(15)
            return
         endif
         if(m2.ne.m3) then
            top=top0
            fin=-fin
            return
         endif
         ilrs=iadr(lstk(top))
         istk(ilrs)=6
         istk(ilrs+1)=m3
         istk(ilrs+2)=n3
         call icopy(2+m2+nel2,istk(il2+3),1,istk(ilrs+3),1)
         l1=sadr(ilrs+5+m2+nel2)
         lstk(top+1)=l1
         return
      elseif(m3.gt.1.and.n3.gt.1) then
c     .  arg3(arg1)=arg2 with arg3 not a vector
         top=top0
         fin=-fin
         return
      endif
      call indxg(il1,mn3,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mi.eq.0) then
c     .  arg3([])=arg2
         call error(15)
         return
      endif
      if(mi.ne.mn2) then
         call error(15)
         return
      endif
c     
      if (n3.gt.1.and.m3.gt.1) then
c     .  arg3 is not a vector
         if(n2.gt.1.and.m2.gt.1) then
            call error(15)
            return
         endif
         if(mxi.gt.m3*n3) then
            call error(21)
            return
         endif
         mr=m3
         nr=n3
      elseif (n3.le.1.and.n2.le.1) then
c     .  arg3 and arg2 are  column vectors
         mr=max(m3,mxi)
         nr=max(n3,1)
      elseif (m3.le.1.and.m2.le.1) then
c     .  row vectors
         nr=max(n3,mxi)
         mr=max(m3,1)
      else
c     .  arg3 and arg2 dimensions dont agree
         call error(15)
         return
      endif

c     set output sizes
      if (m3 .gt. 1.or.m1.lt.0) then
c     .  column vector
         m=mi
         n=-1
         mr = mi
         nr = 1
      else
c     .  row vector
         m=-1
         n=mi
         nr = mi
         mr = 1
      endif
c     
      lptr=iadr(lw)
      irc=lptr+mr+1
      nelr=iadr(lstk(bot))-irc-mr
      if(nelr.le.0) then
         err=lw-lstk(bot)
         call error(17)
         return
      endif
      lw=sadr(irc+mr+nelr)
      nel=nelr
      if(istk(il3).eq.6) then
         if(istk(il2).eq.6) then
            call lspisp(m3,n3,nel3,istk(irc3),istk(ili),m,istk(ili),n,m2
     $           ,n2,nel2,istk(irc2),mr,nr,nelr,istk(irc),istk(lptr)
     $           ,ierr)
         elseif(istk(il2).eq.4) then
            call lspis(m3,n3,nel3,istk(irc3),istk(ili),m,istk(ili),n,m2
     $           ,n2,istk(l2),mr,nr,nelr,istk(irc),ierr) 
         endif
      endif
      if(ierr.ne.0) then
         buf='not enough memory'
         call error(9999)
         return
      endif
      ilrs=iadr(lstk(top))
      istk(ilrs)=6
      istk(ilrs+1)=mr
      istk(ilrs+2)=nr
      istk(ilrs+3)=0
      istk(ilrs+4)=nelr
      call icopy(mr+nelr,istk(irc),1,istk(ilrs+5),1)
      l1=sadr(ilrs+5+mr+nelr)
      lstk(top+1)=l1
      go to 999
c     
 90   continue
c     arg4(arg1,arg2)=arg3
c     get arg4      
      il4=iadr(lstk(top))
      if(istk(il4).lt.0) il4=iadr(istk(il4+1))
      m4=istk(il4+1)
      n4=istk(il4+2)
      it4=istk(il4+3)
      if(istk(il4).eq.6) then
         nel4=istk(il4+4)
         irc4=il4+5
         l4=sadr(irc4+m4+nel4)
      else
         top=top0
         fin=-fin
         return
      endif
      mn4=m4*n4
c     get arg3
      top=top-1
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      if(istk(il3).eq.6) then
         nel3=istk(il3+4)
         irc3=il3+5
         l3=sadr(irc3+m3+nel3)
      elseif(istk(il3).eq.4) then
         nel3=m3*n3
         l3=il3+3
      elseif(istk(il3).eq.1.and.m3*n3.eq.0) then
         l3=sadr(il3+4)
         nel3=m3*n3
         istk(il3)=4
         l3=il3+3
      else
         top=top0
         fin=-fin
         return
      endif
      mn3=m3*n3
c     get arg2
      top=top-1
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)

      if (m3.eq.0) then
c     .  arg4(arg1,arg2)=[]
         if(m1.eq.-1.and.m2.eq.-1) then
c     .    arg4(:,:)=[] -->[]
            ilrs=iadr(lstk(top))
            istk(ilrs)=1
            istk(ilrs+1)=0
            istk(ilrs+2)=0
            istk(ilrs+3)=0
            lstk(top+1)=sadr(ilrs+4)+1
            goto 999
         elseif(m1.eq.0.or.m2.eq.0) then
c     .     arg4([],arg2)=[],  arg4(arg1,[])=[] --> arg4
            call icopy(5+m4+nel4,istk(il4),1,istk(ilrs),1)
            l=sadr(ilrs+5+m4+nel4)
            lstk(top+1)=l
            goto 999
         elseif(m2.eq.-1) then
c     .     arg3(arg1,:)=[] --> arg3(compl(arg1),:)
            call indxgc(il1,m4,ili,mi,mxi,lw)
            if(err.gt.0) return
            mr=mi
            call indxg(il2,n4,ilj,nj,mxj,lw,1)
            if(err.gt.0) return
            if(nj.lt.0) then
               nr=mxj
            else
               nr=nj
            endif
            l3=l4
            n3=n4
            m3=m4
            mn3=m3*n3
            it3=it4
            irc3=irc4
            nel3=nel4
c     .     call extraction
            goto 76
         elseif(m1.eq.-1) then
c     .     arg3(:,arg2)=[] --> arg3(:,compl(arg2))
            call indxgc(il2,n4,ilj,nj,mxj,lw)
            if(err.gt.0) return
            nr=nj
            call indxg(il1,m4,ili,mi,mxi,lw,1)
            if(err.gt.0) return
            if(mi.lt.0) then
               mr=mxi
            else
               mr=mi
            endif
            l3=l4
            n3=n4
            m3=m4
            mn3=m3*n3
            it3=it4
            irc3=irc4
            nel3=nel4
c     .     call extraction
            goto 76
         else
c     .     arg4(arg1,arg2)=[] 
            lw1=lw
            call indxgc(il2,n4,ilj,nj,mxj,lw)
            if(err.gt.0) return
            nr=nj
            if(nj.eq.0) then
c     .        arg4(arg1,1:n4)=[] 
               call indxgc(il1,m4,ili,mi,mxi,lw)
               lw2=lw
               if(err.gt.0) return
               mr=mi
c     .        arg2=1:n4
               if(mi.eq.0) then
c     .           arg4(1:m4,1:n4)=[] 
                  ilrs=iadr(lstk(top))
                  istk(ilrs)=1
                  istk(ilrs+1)=0
                  istk(ilrs+2)=0
                  istk(ilrs+3)=0
                  lstk(top+1)=sadr(ilrs+4)+1
                  goto 999
               else
c     .           arg4(arg1,1:n4)=[] 
c     .           replace arg2 by ":"
                  il2=iadr(lw2)
                  istk(il2)=1
                  istk(il2+1)=-1
                  istk(il2+2)=-1
                  istk(il2+3)=0
c     .
                  lw=lw2+2
                  call indxg(il2,n4,ilj,nj,mxj,lw,1)
                  if(err.gt.0) return
                  if(nj.lt.0) then
                     nr=mxj
                  else
                     nr=nj
                  endif
                  l3=l4
                  n3=n4
                  m3=m4
                  it3=it4
                  mn3=m3*n3
                  irc3=irc4
                  nel3=nel4
c     .           call extraction
                  goto 76
               endif
            else
               lw=lw1
               call indxgc(il1,m4,ili,mi,mxi,lw)
               if(err.gt.0) return
               if(mi.eq.0) then
c     .           arg4(1:m4,arg2)=[] 
                  call indxg(il1,m4,ili,mi,mxi,lw,1)
                  if(err.gt.0) return
                  if(mi.lt.0) then
                     mr=mxi
                  else
                     mr=mi
                  endif
                  l3=l4
                  n3=n4
                  m3=m4
                  it3=it4
                  mn3=m3*n3
                  irc3=irc4
                  nel3=nel4
c     .           call extraction
                  goto 76
               else
                  call error(15)
                  return
               endif
            endif
         endif
      elseif(m3.lt.0.or.m4.lt.0) then
c     .  arg3=eye , arg4=eye
         call error(14)
         return
      elseif(m1.eq.-1.and.m2.eq.-1) then
c     .  arg4(:,:)=arg3
         if(mn3.ne.mn4) then
            call error(15)
            return
         endif
         if(m3.ne.m4) then
            top=top0
            fin=-fin
            return
         endif
c     .  reshape arg3 according to arg4
         ilrs=iadr(lstk(top))
         istk(ilrs)=6
         istk(ilrs+1)=m4
         istk(ilrs+2)=n4
         call icopy(2+m3+nel3,istk(il3+3),1,istk(ilrs+3),1)
         l1=sadr(ilrs+5+m3+nel3)
         lstk(top+1)=l1
         return
      endif

      call indxg(il1,m4,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mi.lt.0) then
         mr1=mxi
      else
         mr1=mi
      endif
      call indxg(il2,n4,ilj,mj,mxj,lw,1)
      if(err.gt.0) return
      if(mj.lt.0) then
         nr1=mxj
      else
         nr1=mj
      endif
      if(mr1.eq.0.or.nr1.eq.0) then
         call error(15)
         return
      endif
      if(mr1.ne.m3.or.nr1.ne.n3) then
c     .  sizes of arg1 or arg2 dont agree with arg3 sizes
         call error(15)
         return
      endif
      mr=max(m4,mxi)
      nr=max(n4,mxj)
c
      lptr=iadr(lw)
      irc=lptr+mr+1
      nelr=iadr(lstk(bot))-irc-mr
      if(nelr.le.0) then
         err=lw-lstk(bot)
         call error(17)
         return
      endif
      lw=sadr(irc+mr+nelr)
      nel=nelr
      if(istk(il4).eq.6) then
         if(istk(il3).eq.6) then
            call lspisp(m4,n4,nel4,istk(irc4),istk(ili),mi,istk(ilj),mj,
     $           m3,n3,nel3,istk(irc3),mr,nr,nelr,istk(irc),istk(lptr)
     $           ,ierr)
         elseif(istk(il3).eq.4) then
            l3=il3+3
            call lspis(m4,n4,nel4,istk(irc4),istk(ili),mi,istk(ilj),mj
     $           ,m3,n3,istk(l3),mr,nr,nelr,istk(irc),ierr) 
         endif
      endif
      if(ierr.ne.0) then
         buf='not enough memory'
         call error(9999)
         return
      endif
      ilrs=iadr(lstk(top))
      istk(ilrs)=6
      istk(ilrs+1)=mr
      istk(ilrs+2)=nr
      istk(ilrs+3)=0
      istk(ilrs+4)=nelr
      call icopy(mr+nelr,istk(irc),1,istk(ilrs+5),1)
      l1=sadr(ilrs+5+mr+nelr)
      lstk(top+1)=l1
      go to 999
c
 130  continue
c     comparaisons
      if(mn2.eq.0.and.mn1.eq.0) then
         if(op.eq.equal.or.op.eq.less+great) then
            il1=iadr(lstk(top))
            istk(il1)=4
            istk(il1+1)=1
            istk(il1+2)=1
            istk(il1+3)=1
            if(op.eq.less+great) istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            goto 999
         else
            call error(60)
            return
         endif
      endif
      if(mn1.ne.1.and.mn2.ne.1) then
         if(n1.ne.n2.or.m1.ne.m2) then
            if(op.eq.equal.or.op.eq.less+great) then
               il1=iadr(lstk(top))
               istk(il1)=4
               istk(il1+1)=1
               istk(il1+2)=1
               istk(il1+3)=0
               if(op.eq.less+great) istk(il1+3)=1
               lstk(top+1)=sadr(il1+4)
               return
            else
               call error(60)
               return
            endif
         endif
      endif
      if(istk(il1).ne.4.and.istk(il1).ne.6.or.istk(il2).ne.4.and
     $     .istk(il2).ne.6) then
         top=top0
         fin=-fin
         return
      endif
c
      mr=m1
      nr=n1
      if(m1*n1.eq.1) then
         mr=m2
         nr=n2
      endif
      irc=iadr(lw)
      nelmx=(iadr(lstk(bot))-irc-mr-10)
      lw=sadr(irc+mr+nelmx)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif  
      nel=nelmx
      if(istk(il1).eq.4) then
         l1=il1+3
         call lsosp(op,m1,n1,istk(l1),m2,n2,nel2,istk(irc2),nel,istk(irc
     $        ),ierr)
      elseif(istk(il2).eq.4) then
         l2=il2+3
         call lspos(op,m1,n1,nel1,istk(irc1),
     $           m2,n2,istk(l2),nel,istk(irc),ierr)
      else
         call lsposp(op,m1,n1,nel1,istk(irc1),
     $           m2,n2,nel2,istk(irc2),
     $           nel,istk(irc),ierr)
      endif
      if(ierr.ne.0) then
         buf='not enough memory'
         call error(9999)
         return
      endif
      il1=iadr(lstk(top))
      istk(il1)=6
      istk(il1+1)=mr
      istk(il1+2)=nr
      istk(il1+3)=0
      istk(il1+4)=nel
      irc1=il1+5
      call icopy(mr+nel,istk(irc),1,istk(irc1),1)
      l1=sadr(irc1+mr+nel)
      lstk(top+1)=l1
      go to 999
c     
 999  return
      end



