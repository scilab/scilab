      subroutine intwrite
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
      integer    lch
      parameter (lch=1024)
      character chaine*(lch)
c     
      integer top2,id(nlgh),h(nsiz)
      integer ftyp,fmttyp,mode(2)
      integer top0
      logical opened,eptover
      integer iadr,sadr
      character bu1*(bsiz),bu2*(bsiz)
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      top0=top-rhs+1
c
      ftyp=0

      if(rhs.gt.4.or.rhs.lt.2) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif

c     opening file
      top2 = top
      top = top-rhs+1
      il=iadr(lstk(top))
      mode(1)=0
      mode(2)=0
      call v2unit(top,mode,lunit,opened,ierr)
      if(ierr.gt.0) return
c     

      nc=0
      iacces=0
      if(rhs.eq.2) goto 61
      il=iadr(lstk(top2))
      if(rhs.eq.4) then
         iacces=top2-1
      else
         if(istk(il).ne.10) then
            call error(61)
            goto 996
         endif
      endif
c     analyse du format
      if(istk(il).ne.10) then
         err=top2-top+1
         call error(55)
         goto 996
      endif
      nc=istk(il+5)-1
      l=il+5+istk(il+1)*istk(il+2)
      ftyp=fmttyp(istk(l),nc)
      if(ftyp.eq.0) then
         call error(49)
         goto 996
      endif
      call cvstr(nc,istk(l),buf,1)
      top2=top2-1
      fin=-fin
 61   if(iacces.ne.0) then
c     analyse des numero d'enregistrement
         ilb=iadr(lstk(iacces))
         if(istk(ilb+3).ne.0) then
            err=top2-top
            call error(52)
            return
         endif
         nb=istk(ilb+1)*istk(ilb+2)
         lb=sadr(ilb+4)
         do 62 i=1,nb
            istk(ilb-1+i)=int(stk(lb-1+i))
 62      continue
         top2=iacces-1
      endif
      il=iadr(lstk(top2))
      if(istk(il).ne.1) goto 70
      if(istk(il+3).ne.0) then
         err=top2-top+1
         call error(52)
         goto 996
      endif
      if(fin.lt.0.and.ftyp.ne.1.and.ftyp.ne.2) then
         call error(49)
         goto 996
      endif
      m=istk(il+1)
      n=istk(il+2)
      l=sadr(il+4)
      if(ftyp.eq.2) then
         call entier(m*n,stk(l),istk(iadr(l)))
         l=iadr(l)
      endif
c---- ecriture de flottants
      if(iacces.eq.0) then
c     acces sequentiel
         if(lunit.ne.wte) then
            do 63 i=1,m
               li=l+i-1
               if(fin.gt.0) then
                  write(lunit,*,err=139)
     1                 (stk(li+(j-1)*m),j=1,n)
               else
                  if(ftyp.eq.1) then
                     write(lunit,buf(1:nc),err=139)
     1                    (stk(li+(j-1)*m),j=1,n)
                  else
                     write(lunit,buf(1:nc),err=139)
     1                    (istk(li+(j-1)*m),j=1,n)
                  endif
               endif
 63         continue
         else
            do 68 i=1,m
               li=l+i-1
               if(fin.gt.0) then
                  buf=' '
                  nv=lct(5)/18
                  write(chaine,'(''('',i2,''(1x,1pd17.10))'')') nv
                  do 64 k1=1,n,nv
                     k2=min(n,k1+nv-1)
                     write(buf,chaine,err=139) 
     &                    (stk(li+(j-1)*m),j=k1,k2)
                     lc=18*(k2-k1+1)
                     call basout(io,wte,buf(1:lc))
 64               continue
               else
                  ib=nc+1
                  if(ftyp.eq.1) then
                     write(buf(ib:),buf(1:nc),err=139)
     &                    (stk(li+j*m),j=0,n-1)
                  else
                     write(buf(ib:),buf(1:nc),err=139)
     &                    (istk(li+j*m),j=0,n-1)
                  endif
                  lb1=bsiz+1
 66               lb1=lb1-1
                  if(lb1.ge.ib+1.and.buf(lb1:lb1).eq.' ') goto 66
                  call basout(io,wte,buf(ib:lb1))
               endif
 68         continue
         endif
c     
      else
c     acces direct
         if(nb.ne.m) then
            call error(42)
            goto 996
         endif
         do 69 i=1,m
            li=l+i-1
            if(ftyp.eq.1) then
               write(lunit,buf(1:nc),rec=istk(ilb+i-1),
     1              err=139)  (stk(li+(j-1)*m),j=1,n)
            else
               write(lunit,buf(1:nc),rec=istk(ilb+i-1),
     1              err=139)  (istk(li+(j-1)*m),j=1,n)
            endif
 69      continue
      endif
      goto 78
c     
c     ecriture des chaines de caracteres
 70   if(istk(il).ne.10) then
         err=top2-top+1
         call error(55)
         goto 996
      endif
      if(fin.lt.0.and.ftyp.ne.4) then
         call error(49)
         goto 996
      endif
      n=istk(il+2)*istk(il+1)
      m=istk(il+1)
      il=il+4
      l=il+n+1
      if(iacces.ne.0) then
         if(nb.ne.m) then
            call error(42)
            goto 996
         endif
      endif
      do 77 i=1,n
         m=istk(il+i)-istk(il+i-1)
         if(iacces.eq.0) then
            lm=l
            if(lunit.ne.wte) then
c     fichier format libre
               if(fin.gt.0) then
                  if(m.eq.0) then
                     write(lunit,*,err=139)
                  else
                     do 73 i1=1,m,lch
                        i2=min(m,i1+lch-1)
                        m1=i2-i1+1
                        call cvstr(m1,istk(lm),chaine(1:m1),1)
                        write(lunit,*,err=139) chaine(1:m1)
                        lm=lm+m1
 73                  continue
                  endif
               else
c     fichier format donne
                  if(m.eq.0) then
                     write(lunit,buf(1:nc),err=139)
                  else
                     do 74 i1=1,m,lch
                        i2=min(m,i1+lch-1)
                        m1=i2-i1+1
                        call cvstr(m1,istk(lm),chaine(1:m1),1)
                        write(lunit,buf(1:nc),err=139) chaine(1:m1)
                        lm=lm+m1
 74                  continue
                  endif
               endif
            else
               if(fin.gt.0) then
c     ecran format libre
                  if(m.eq.0) then
                     call basout(io,lunit,' ')
                  else
                     do 75 i1=1,m,bsiz
                        i2=min(m,i1+bsiz-1)
                        m1=i2-i1+1
                        call cvstr(m1,istk(lm),buf(1:m1),1)
                        call basout(io,lunit, buf(1:m1))
                        lm=lm+m1
 75                  continue
                  endif
               else
c     ecran format donne
                  if(m.eq.0) then
                     call basout(io,wte,' ')
                  else
                     m1=min(bsiz,m)
                     call cvstr(m1,istk(l),bu1,1)
                     write(bu2,buf(1:nc),err=139) bu1(1:m1)
                     lb=bsiz+1
 71                  lb=lb-1
                     if(lb.ge.2.and.bu2(lb:lb).eq.' ') goto 71
                     call basout(io,wte,bu2(1:lb))
                  endif
               endif
            endif
         else
c     acces direct
            m1=min(bsiz,m)
            call cvstr(m1,istk(l),bu1,1)
            write(lunit,buf(1:nc),rec=istk(ilb+i-1),err=139)
     +           bu1(1:m1)
         endif
         l=l+m
 77   continue
c     
c     fin generale de write
 78   il=iadr(lstk(top))
      istk(il)=0
      if(.not.opened) then
         mode(1)=0
         mode(2)=0
         call clunit(-lunit,buf,mode)
      endif
      goto 999

 139  call error(49)
      if(.not.opened) call clunit(-lunit,buf,mode)
      return

 996  if(.not.opened) call clunit(-lunit,buf,mode)
      return
c
 999  return
      end
