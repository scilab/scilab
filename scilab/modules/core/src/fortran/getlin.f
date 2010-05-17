c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


      subroutine getlin(job,menusflag)
c ====================================================================
c     get a new line from the terminal (rio==rte) or from a file, the
c     characters read are stored in the lin array
c
c     job=1 : append read characters after the current end of lin lpt(6)
c     job=0 : store read characters after the current position (lpt(1))
c     job=2 : same as job=0 but if EOF occurs getlin returns with 
c     .       fin=-2  instead of fin=0
c     job=-1: ???
c     job=3 : used to reenter getlin when it was interrupted while
C     .       getting a continuation line
c     menuflag is used to determine what to do if a dynamical menu is
C     activated while reading a line on the Scilab console
c     menuflag = 1 : line acquisition is interrupted, callback is
C                    executed and line acquisition is restarted
c     menuflag = 0 : callback execution is delayed
C                    menus used when the parser is not in a state allowing
C                    to execute the callback
c ====================================================================
c
      include 'stack.h'
c
      integer job, menusflag
      integer lrecl,eol,slash,dot,blank,tab
      integer retu(6)
      integer r,quit(4),lnblnk, ncont
      logical isinstring,eof,continued,incomment
      character*20 tmp
      external isinstring,lnblnk, getfastcode
      integer getfastcode

      data eol/99/,dot/51/,blank/40/,tab/-40/
      data retu/27,14,29,30,27,23/
      data slash/48/,lrecl/512/
      data quit/26,30,18,29/
c
      if(ddt.eq.4) then
         write(tmp(1:10),'(2i5)') rio,job
         call basout(io,wte,' getlin rio:'//tmp(1:5)//' job: '
     &              //tmp(6:10))
      endif
c
      n=1
c     continued is set to true when continuation mark found at the end of a line
      continued=.false.
      ncont=0
      l1=lpt(1)
      lct(8)=lct(8)+1
c     next line to preserve end-of-line marks (eol)
      if(job.eq.1) then
         l1=lpt(6)+1
         l=l1
      elseif(job.eq.-1) then
         if (lpt(6).lt.0) then
            l=-lpt(6)
         else
            l=l1
         endif
      elseif(job.eq.3) then
         l=lpt(6)+1
         l1=lpt(1)
      else
         l=l1
      endif
      r=0
      if(pt.gt.0) r=rstk(pt)
      l0=0
      if(job.lt.0) then
         n=lnblnk(buf(1:lrecl))
         goto 12
      endif
      if(r.eq.503) goto 11
      if(macr.gt.0.and.fin.ne.2) then
         k=lpt(1)-(13+nsiz)
         il=lin(k+7)
c        check if getlin is call in a macro or an exec 
         if(il.gt.0) goto 80
      endif
      info=1

 11   continue
      call getfiletype(rio,ltype,info)
      if(info.ne.0) goto 50 
      if(ltype.eq.1) then
         call basin(ierr,rio,buf(1:lrecl),'*',menusflag)
         if(ierr.lt.0) then
c     .     interrupted line acquisition (callback)
            goto 90
         endif
         if(ierr.ne.0) then
c     .     end of file encountered
            goto 50
         endif
         n=lnblnk(buf(1:lrecl))
      else
         call readnextline(rio,buf,bsiz,n,nr,info)
         if(info.eq.-1) goto 50
         n=n-1
         n=lnblnk(buf(1:n))
      endif

      eof=.false.
 12   l0=l
      if(n.le.0) goto 45
      if (mod(lct(4),2) .eq. 1.and.rio.ne.rte) then
         call promptecho(wte,buf(1:n),n)
      endif
      if (rio.eq.rte) then
c     -2 equals INPUT for diary (see basout)
        call promptecho(-2,buf(1:n),n)
        lct(1)=1
      endif
c
c     loop on read characters
      incomment=.false.
      j=0
 17   j=j+1
      if(j.gt.n) goto 45

*     modif bruno : appel a getfastcode au lieu de la boucle
      k = getfastcode(buf(j:j))
      if (k .eq. eol) go to 45
      if (k.eq.tab) k=blank


c     special cases        //    ..
      if(buf(j+1:j+1).ne.buf(j:j)) goto 31

      if(k.eq.slash) then
c     .  check if // occurs in a string
         if(.not.isinstring(lin(l0),l-l0+1)) incomment=.true.
      endif
c
      if(k.ne.dot.or.incomment) goto 31
      if(j.eq.1) goto 70
c     . .. find
c     check if .. is followed by more dots or //
      jj=j
 28   continue
      if(jj.eq.n)goto 29
      jj=jj+1
      if(buf(jj:jj).eq.buf(j:j)) goto 28
      if(buf(jj:jj).eq.' '.or.buf(jj:jj).eq.char(9)) goto 28
      if(buf(jj:jj).ne.'/') goto 31
      if(jj.eq.n) goto 31
      if(buf(jj+1:jj+1).eq.'/') goto 29
      goto 31
c
 29   continue
c     next line is a continuation line
      continued=.true.
      ncont=ncont+1
      if(job.ne.-1) goto 11
c     handle continuation lines when scilab is call as a procedure
      fin=-1
      lpt(6)=-l
      return
c     There is no continuation line or syntax error
 31   continue
      continued=.false.
c     increase the line counter to take the contuation lines into acount
      lct(8)=lct(8)+ncont
      ncont=0
      lin(l) = k
      if (l.lt.lsiz) l = l+1
      if (l.ge.lsiz) then
         call error(108)
         return
      endif
      goto 17
c
 45   continue
      if(l.eq.l0) then
         lin(l)=blank
         l=l+1
      endif
      lin(l) = eol
      lin(l+1)=blank
      lpt(6) = l
      lpt(4) = l1
      lpt(3) = l1
      lpt(2) = l1
cc_ex      lct(1) = 0
      fin=0
      if(job.eq.2.and.eof) fin=-2
      call fortrangetch
      return
c
   50 eof=.true.
      if (rio .eq. rte) go to 52
      call icopy(6,retu,1,lin(l),1)
      if(job.eq.1) then
         call error(47)
         return
      endif
c     rio=rte
      l = l + 6
      go to 45
   52 continue
      call icopy(4,quit,1,lin(l),1)
      l = l + 4
      go to 45
c
c
   70 continue
      if (n.gt.2) then
         call xscion(iflagx)
         if(iflagx.eq.1) then
            call cvstr(n-2+10,lin(l),'unix_x('''//buf(3:n)//''');',job)
         else
            call cvstr(n-2+10,lin(l),'unix_w('''//buf(3:n)//''');',job)
         endif
         l=l+n-2+10
         call basout(io,wte,' ')
      endif
      goto 45
c
   80 eof=.false.
      k=lpt(1)-(13+nsiz)
      ilk=lin(k+6)
      if(istk(ilk).eq.10) then
         mn=istk(ilk+1)*istk(ilk+2)
         lf=ilk+4+mn+istk(ilk+4+mn)-1
      else
         mlhs=istk(ilk+1)
         mrhs=istk(ilk+1+nsiz*mlhs+1)
         ll=ilk+4+nsiz*(mlhs+mrhs)
         lf= ll+istk(ll-1)+1
      endif
      il=lin(k+7)
      if(il.gt.lf) then
         eof=.true.
         goto 45
      endif

   81 if(istk(il).eq.eol) goto 82
      lin(l)=istk(il)
      l=l+1
      if(l.gt.lsiz) then
         call error(26)
         return
      endif
      il=il+1
      goto 81
   82 if(istk(il+1).ne.eol) goto 83

      lin(l)=eol
      l=l+1
      il=il+1
   83 lin(k+7)=il+1
c%%
      if((ddt .lt. 1 .or. ddt.gt.4).and.mod(lct(4),2) .ne. 1) goto 45
      l2=l1
   84 n=l-l2
      n1=n
      if(n.le.lct(5)) then
         call cvstr(n,lin(l2),buf,1)
      else
         n=lct(5)
         n1=n-3
         call cvstr(n1,lin(l2),buf,1)
         buf(n1+1:n1+3)='...'
         n1=n1-1
      endif
      call basout(io,wte,buf(1:n))
      l2=l2+n1+1
      if(l2.lt.l) goto 84
      goto 45
 90   continue
c     interrupted line acquisition (callbacks)
c     l should be memorized for correct continuation line handling    
      if (continued) then
          lpt(6) = l-1
       else
          lpt(6) = lpt(1)
       endif
      fin=-3
      return
      end
