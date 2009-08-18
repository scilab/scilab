c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) Bug 1091 corrected - Francois VOGEL November 2004
c Copyright (C) Bug 1417 corrected - Francois VOGEL May 2005
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine depfun(lunit,trace,first)
c     depile une macro ou un execstr
      include 'stack.h'
      integer lunit
      logical trace,first,callback
      integer sadr,ll,leng,ival(2)
      double precision val
      equivalence (ival(1),val)
c
      sadr(l)=(l/2)+1
c
      ll=lct(5)
      k=lpt(1)-(13+nsiz)
      lpt(1)=lin(k+1)
      lpt(2)=lin(k+2)
      lpt(3)=lin(k+3)
      lpt(4)=lin(k+4)
      lpt(6)=k
      ival(1)=lin(k+8)
      ival(2)=lin(k+9)
      stk(lstk(isiz))=val
c     recherche du nom de la function correspondant a ce niveau
      lk=sadr(lin(k+6))
      if(lk.le.lstk(top+1)) then
         km=0
      else
         km=lin(k+5)-1
      endif
 1503 km=km+1
      if(km.gt.isiz)goto 1513
      if(lstk(km).ne.lk) goto 1503
c
 1513 continue
      ilk=lin(k+6)
      if(trace) then
         m=0
         if(istk(ilk).ne.10) then
            if(first) then
               buf='at line '
               m=11
               if (istk(ilk).eq.13) then 
c     .           compiled macro
                  nlc=0
               else
c     .           uncompiled macro
                  nlc=1
               endif
            else
               buf='line '
               m=6
               call whatln(lpt(1),lpt(2),lpt(6),nlc,l1,ifin)
            endif
            write(buf(m+1:m+5),'(i4)') lct(8)-nlc
            m=m+4
            buf(m+1:m+18)=' of function     '
            m=m+13
            if (km.le.isiz) then
               call cvnamel(idstk(1,km),buf(m+1:m+nlgh),1,leng)
               m=m+leng
            endif
         else
            buf='in  execstr instruction'
            m=26
         endif
         callback=rstk(pt-1).eq.706.or.rstk(pt-1).eq.606
         if(callback) then
            buf(1:26)='while executing a callback'
            m=26
         else
            buf(m+1:m+14)=' called by :'
            m=m+14
         endif
         call basout(io,lunit,buf(1:m))
         lct(8)=lin(k+12+nsiz)
c     
         if (.not.callback) then
            call whatln(lpt(1),lpt(2),lpt(6),nlc,l1,ifin)
            m=ifin-l1+1
            if(m.gt.ll) then
               l1=max(l1,lpt(2)-ll/2)
               m=min(ifin-l1,ll)
            endif

            if(l1.gt.0.and.m.gt.0.and.m+l1-1.le.lsiz) then
               call cvstr(m,lin(l1),buf(1:m),1)
               call basout(io,lunit,buf(1:m))
            endif
         endif

      endif
c
      macr=macr-1
      if(istk(ilk).ne.10.and.rstk(pt-1).ne.909) bot=lin(k+5)
      return
      end

      subroutine depexec(lunit,trace,first,pflag)
c     pflag ,indique si c'est une pause qui a ete depilee
c Bug 1091 corrected - Francois VOGEL November 2004
c Bug 1417 corrected - Francois VOGEL May 2005
      include 'stack.h'
      logical trace,first,pflag
      integer mode(2),lunit,ll

      ll=lct(5)
      call whatln(lpt(1),lpt(2),lpt(6),nlc,l1,ifin)
      if(rio.ne.rte) then
c     exec
         k=lpt(1)-(13+nsiz)
         lpt(1)=lin(k+1)
         lpt(2)=lin(k+4)
         lpt(6)=k
c
         if (trace) then
            if(first) then
               buf='at line '
               m=11
               nlc=0
            else
               buf='line '
               m=6
            endif
            write(buf(m+1:m+5),'(i4)') lct(8)-nlc
            m=m+4
            buf(m+1:m+29)=' of exec file called by :'
            m=m+29
            call basout(io,lunit,buf(1:m))
            lct(8)=lin(k+12+nsiz)
c     
            call whatln(lpt(1),lpt(2),lpt(6),nlc,l1,ifin)
            m=ifin-l1+1
            if(m.gt.ll) then
               l1=max(l1,lpt(2)-ll/2)
               m=min(ifin-l1,ll)
            endif
            call cvstr(m,lin(l1),buf,1)
            call basout(io,lunit,buf(1:m))
            
         endif
         mode(1)=0
         call clunit(-rio,buf,mode)
 1505    pt=pt-1
         if(rstk(pt).ne.902) goto 1505
         rio=pstk(pt)
         pflag=.false.
      else
c     pause
         top=ids(2,pt-1)
         pflag=.true.
      endif
      return
      end


      subroutine errloc(n)
c     this routines shows the approximate location of the error in the
c     current statement
c!
      include 'stack.h'

c      integer sadr

      character mg*9,bel*1,line*340,fmt*7
      integer n,ll,m,m1,lp,nd
      data mg /' !--error'/,bel/' '/
c
c      sadr(l)=(l/2)+1
c
      ll=lct(5)

      lunit = wte
      m1=lpt(2)-lpt(1)
      if(m1.lt.1) m1=1
c
      if(macr.eq.0.and.rio.eq.rte) goto 1000
      call whatln(lpt(1),lpt(2),lpt(6),nlc,l1,ifin)
      m=ifin-l1+1
      if(m.gt.ll) then
         l1=max(l1,lpt(2)-ll/2)
         m=min(ifin-l1,ll)
      endif
      m1=max(0,lpt(2)-l1)

      if(m.gt.0) then
         call cvstr(m,lin(l1),line,1)
         call basout(io,lunit,line(1:max(m,1)))
      endif
 1000 line=' '
      if(m1+15.gt.ll) m1=ll-16
      line(m1+1:m1+9)=mg
      lp=m1+11
      nd=int(log10(float(n)))+1
      write(fmt,'(''(i'',i2,'')'')') nd
      write(line(lp:lp+nd-1),fmt) n
      lp=lp+nd
      line(lp:lp)=bel
      call basout(io,lunit,line(1:lp))
      return
      end

      subroutine errcontext()
c     this routines stores the line number  of the instruction that
c     produces  the error as well as the calling macro name if any
      include 'stack.h'

      integer sadr
      sadr(l)=(l/2)+1
      if (macr.le.0) return
      k=lpt(1)-(13+nsiz)


c     recherche du nom de la function correspondant a ce niveau
      lk=sadr(lin(k+6))
      if(lk.le.lstk(top+1)) then
         km=0
      else
         km=lin(k+5)-1
      endif
 10   km=km+1
      if(km.gt.isiz)goto 11
      if(lstk(km).ne.lk) goto 10

 11   continue
      ilk=lin(k+6)
      if(istk(ilk).ne.10) then
         if (istk(ilk).eq.13) then 
c     .     compiled macro
            call linestore(lct(8))
         else
c     .     uncompiled macro
            call linestore(lct(8)-1)
         endif
         if (km.le.isiz) then
            call cvnamel(idstk(1,km),buf(1:nlgh),1,leng)
            call funnamestore(buf(1:nlgh),leng)
         endif
      else
         call linestore(lct(8)-1)
c         if(m.gt.24) m=24
c         call funnamestore(buf(1:m),m) 
      endif
      end
