      subroutine typ2cod(il,name,n)
c     returns in name(1:n) the code associated with the type of the
c     variable that began in istk(il)

c     Copyright INRIA
      INCLUDE '../stack.h'
      integer nmax
      parameter (nmax=8)
c     following common defines the initial database of type names
      integer maxtyp,nmmax,maxtypf,nmmaxf
      parameter (maxtyp=50,nmmax=200)
      integer tp(maxtyp),ptr(maxtyp),ln(maxtyp),namrec(nmmax),ptmax
      common /typnams/ tp,ptr,ln,namrec,ptmax
      integer name(*)
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      itype=abs(istk(il))
      if(itype.le.20) then
c     look for itype in predefined types
         if(itype.eq.16.or.itype.eq.17) then
            if(istk(il).lt.0) il=iadr(istk(il+1))
            n1=istk(il+1)
            iltyp=iadr(sadr(il+3+n1))
            nlt=min(nlgh-3,istk(iltyp+5)-1)
            iltyp=iltyp+5+istk(iltyp+1)*istk(iltyp+2)
            n=min(nlt,nmax)
            call icopy(n,istk(iltyp),1,name(1),1)
            return
         elseif(itype.eq.0) then
            n=1
            name(1)=0
         else
            n=ln(itype)
            call icopy(n,namrec(ptr(itype)),1,name,1)
         endif
      else
c     look for itype in dynamically added types
         it=20
 10      it=it+1
         if(it.gt.maxtyp) goto 9000
         if(tp(it).ne.itype) goto 10
         n=ln(it)
         call icopy(n,namrec(ptr(it)),1,name,1)
      endif
c      
      return
 9000 continue
      n=0
      return
      end
      
      subroutine settypnames()
c     initialize hard coded type names

c     Copyright INRIA
      INCLUDE '../stack.h'
c     following common defines the initial database of type names
      integer maxtyp,nmmax,maxtypf,nmmaxf
      parameter (maxtyp=50,nmmax=200)
      integer tp(maxtyp),ptr(maxtyp),ln(maxtyp),namrec(nmmax),ptmax
      common /typnams/ tp,ptr,ln,namrec,ptmax
      save /typnams/

      
      ip=1
      ptmax=1
      do 10 i=1,20
         tp(i)=i
         ln(i)=0
         ptr(i)=0
 10   continue


c     type 1 :s
      call addtypename(1,'s',ierr)
      if(ierr.ne.0) goto 99
c     type 2 : p
      call addtypename(2,'p',ierr)
      if(ierr.ne.0) goto 99
c     type 4 : b
      call addtypename(4,'b',ierr)
      if(ierr.ne.0) goto 99
c     type 5 : sp
      call addtypename(5,'sp',ierr)
      if(ierr.ne.0) goto 99
c     type 6 : spb
      call addtypename(6,'spb',ierr)
      if(ierr.ne.0) goto 99
C     type 7 : msp
      call addtypename(7,'msp',ierr)
      if(ierr.ne.0) goto 99
C     type 8 : i
      call addtypename(8,'i',ierr)
      if(ierr.ne.0) goto 99
C     type 9 : h
      call addtypename(9,'h',ierr)
      if(ierr.ne.0) goto 99
c     type 10 : c
      call addtypename(10,'c',ierr)
      if(ierr.ne.0) goto 99
c     type 11 : m
      call addtypename(11,'m',ierr)
      if(ierr.ne.0) goto 99
c     type 13 : mc
      call addtypename(13,'mc',ierr)
      if(ierr.ne.0) goto 99
c     type 14 : f
      call addtypename(14,'f',ierr)
      if(ierr.ne.0) goto 99
c     type 15 : l
      call addtypename(15,'l',ierr)
      if(ierr.ne.0) goto 99
c     type 16 : 
      call addtypename(16,'tl',ierr)
      if(ierr.ne.0) goto 99
c     type 17 : 
      call addtypename(17,'ml',ierr)
      if(ierr.ne.0) goto 99
c     type 128 : ptr (lu pointer)
      call addtypename(128,'ptr',ierr)
      if(ierr.ne.0) goto 99
c     type 129 : ip
      call addtypename(129,'ip',ierr)
      if(ierr.ne.0) goto 99
c     type 130 : fptr
      call addtypename(130,'fptr',ierr)
      if(ierr.ne.0) goto 99

      return
 99   if(ierr.eq.1) then
         call error(224)
      elseif(ierr.eq.2) then
         call error(225)
      elseif(ierr.eq.3) then
         call error(224)
      endif
      return
      end

      subroutine addtypename(typ,nam,ierr)
      integer pos,typ,ierr
      character*(*) nam
c
c     Copyright INRIA
c     following common defines the initial database of type names
      integer maxtyp,nmmax,maxtypf,nmmaxf
      parameter (maxtyp=50,nmmax=200)
      integer tp(maxtyp),ptr(maxtyp),ln(maxtyp),namrec(nmmax),ptmax
      common /typnams/ tp,ptr,ln,namrec,ptmax
      character*15 nam1
c
      ierr=0
c     
      n=len(nam)
      if(n.gt.0) then
c     add a type
         if(typ.le.20) then
            pos=typ
            if(ln(pos).ne.0) then
c     .     check if new type is the same as old type
               if(ln(pos).eq.len(nam)) then
                  n=ln(pos)
                  call cvstr(ln(pos),namrec(ptr(pos)),nam1,1)
                  if(nam1(1:n).eq.nam(1:n)) return
               endif
               ierr=2
               return
            endif
         else
            pos=20
 10         pos=pos+1
            if(pos.gt.maxtyp) then
               ierr=1
               return
            endif
            if(ln(pos).ne.0) goto 10
         endif
         tp(pos)=typ
         if(ptmax+n.gt.nmmax) then
            ierr=3
            return
         endif

         ln(pos)=n
         ptr(pos)=ptmax
         call cvstr(n,namrec(ptmax),nam,0)
         ptmax=ptmax+n
      else
c     supress a type
         if(typ.le.20) then
            pos=typ
            if(ln(pos).eq.0) return
         else
            pos=20
 20         pos=pos+1
            if(pos.gt.maxtyp) return
            if(tp(pos).ne.typ) goto 20
         endif
         n=ln(pos)
         ll=ptmax-(ptr(pos)+n)+1
         call icopy(ll,namrec(ptr(pos)+n),1,namrec(ptr(pos)),1)
         ptr(pos)=0
         ln(pos)=0
         ptmax=ptmax-n
      endif
      return
      end
