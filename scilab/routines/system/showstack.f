      subroutine showstack()
c     -------------------------
c     this routines handle  display of calling tree
c!
c     Copyright INRIA
c     Revised and corrected by Francois VOGEL, July 2004 (bug 908)
      include '../stack.h'
c
      integer sadr
c
      character mg*9
      integer lunit,lpt1(6),top1,rios
      integer l1,ilk,m,lk,km,k,ll,r,p
      logical first
      data mg /' !       '/
c
      sadr(l)=(l/2)+1
c
      ll=lct(5)
      first=.true.
      lunit=wte
      top1=top
      call icopy(6,lpt,1,lpt1,1)
      lct4=lct(4)
      rios=rio
c
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
      m1=lpt(2)-l1

      if(m.gt.0) then
         call cvstr(m,lin(l1),buf,1)
         call basout(io,lunit,buf(1:max(m,1)))
      endif
 1000 buf=' '
      buf(m1+1:m1+9)=mg
      call basout(io,lunit,buf(1:m1+9))
 
c depilement de l'environnement
      lct(4)=2

      p=pt+1
 1501 p=p-1
      if(p.eq.0) goto 1510
      r=rstk(p)
      goto(1502,1502,1504) r-500
      goto 1501
c
c     on depile une function
 1502 k=lpt1(1)-(13+nsiz)
      lpt1(1)=lin(k+1)
      lpt1(2)=lin(k+2)
      lpt1(6)=k
c
c     recherche du nom de la function correspondant a ce niveau
      lk=sadr(lin(k+6))
      if(lk.le.lstk(top1+1)) then
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
      if(istk(ilk).ne.10) then
         if(first) then
            buf='at line '
            m=11
            first=.false.
            nlc=0
         else
            buf='line '
            m=6
            call whatln(lpt1(1),lpt1(2),lpt1(6),nlc,l1,ifin)
         endif
         write(buf(m+1:m+5),'(i4)') lct(8)-nlc
         m=m+4
         buf(m+1:m+18)=' of function     '
         m=m+13
         if (km.le.isiz) call cvname(idstk(1,km),buf(m+1:m+nlgh),1)
         m=m+nlgh
      else
         buf='in  execstr instruction'
         m=26
      endif
      buf(m+1:m+14)=' called by :'
      m=m+14
      call basout(io,lunit,buf(1:m))
      lct(8)=lin(k+12+nsiz)
c     
      call whatln(lpt1(1),lpt1(2),lpt1(6),nlc,l1,ifin)
      m=ifin-l1+1
      if(m.gt.ll) then
         l1=max(l1,lpt1(2)-ll/2)
         m=min(ifin-l1,ll)
      endif
      if(l1.gt.0.and.m.gt.0.and.m+l1-1.le.lsiz) then
         call cvstr(m,lin(l1),buf(1:m),1)
         call basout(io,lunit,buf(1:m))
      endif
c
      goto 1501
c
c     on depile un exec ou une pause
 1504 if(rio.ne.rte) then
c     exec
         k=lpt1(1)-(13+nsiz)
         lpt1(1)=lin(k+1)
         lpt1(2)=lin(k+4)
         lpt1(6)=k
c     
         if(first) then
            buf='at line '
            m=11
            first=.false.
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
         call whatln(lpt1(1),lpt1(2),lpt1(6),nlc,l1,ifin)
         m=ifin-l1+1
         if(m.gt.ll) then
            l1=max(l1,lpt1(2)-ll/2)
            m=min(ifin-l1,ll)
         endif
         call cvstr(m,lin(l1),buf,1)
         call basout(io,lunit,buf(1:m))
 1505    p=p-1
         if(rstk(p).ne.902) goto 1505
         rio=pstk(p)
         goto 1501
      else
c     pause
         top1=ids(2,p-1)
         goto 1510
      endif
c
 1510 continue

      call basout(io,lunit,' ')
      lct(4)=lct4
      rio=rios
c
      return
      end
