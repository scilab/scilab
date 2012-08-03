c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine dbasin(ierr,lunit,fmt,v,iv,n)
c ====================================================================
c
c     Management of the standard input
c

      include 'stack.h'
      character*(*) fmt
      double precision v(*)
      integer ierr,lunit
      character*512 string
c
      ierr=0
      if(lunit.eq.rte) then

         string=' '
         call xscion(iflag)
         call eventloopprompt(string,len(string),lline,status,0,iflag)
         if(status.ne.0) goto 10
         if (lline.eq.0) then
            string(1:1)=' '
            lline=1
	 endif
         if(fmt(1:1).ne.'*') then
            read(string(1:lline),fmt,end=10,err=20) (v(i),i=1,n*iv,iv)
         else
            call s2val(string(1:lline),v,iv,n1,n,ierr)
            if(ierr.ne.0) goto 20
            if(n1.lt.n) goto 10
c            read(string(1:lline),*,end=10,err=20) (v(i),i=1,n*iv,iv)
         endif
      else

         if(fmt(1:1).ne.'*') then
            read(lunit,fmt,end=10,err=20) (v(i),i=1,n*iv,iv)
         else
            read(lunit,*,end=10,err=20) (v(i),i=1,n*iv,iv)
         endif
      endif
c
      return
c
 10   ierr=1
      return
 20   ierr=2
      return
      end

      subroutine s2val(str,v,iv,n,maxv,ierr)
c!purpose
c     internal "list-directed" read mode is not standard fortran. 
c     This routine emulates :read(str,*) (v(1,i),i=1,n)
c!
      double precision v(iv,maxv),vv
      character*(*) str
      character*1 si
      logical del
c
      ls=len(str)
      del=.true.
c
      ierr=0
      n0=n
      n=0
      i=0
 10   i=i+1
c     loop on decriptors
      if(i.gt.ls) goto 20
      si=str(i:i)
      if(si.eq.'/'.or.si.eq.',') then
         if(del) then
            if(n+1.gt.maxv) return
            v(1,n+1)=0.0d0
            n=n+1
            goto 10
         else
            del=.true.
         endif
      endif
      if(si.ne.' '.and.si.ne.'/'.and.si.ne.',') then
c     conversion of a descriptor to a (repeated) value
         call nextv(str(i:),vv,nv,ir,ierr)
         if(ierr.ne.0) return
         if(n+nv.gt.maxv) nv=maxv-n
         if(nv.le.0)  return
         call dset(nv,vv,v(1,n+1),iv)
         n=n+nv
         i=i+ir-2
         del=.false.
      endif
      goto 10
 20   continue 
      if(del) then
         if(n+1.gt.maxv) return
         v(1,n+1)=0.0d0
         n=n+1
      endif
      return
      end

      subroutine nextv(str,v,nv,ir,ierr)
c     acquisition d'un nombre flottant decrit par une chaine
c     de caracteres de la forme
c      [nn*][+|-][mmmmm][.][ddd][e|E|d|D[+|-]eee]
      character*(*) str
      character*1 next
      double precision v

      integer c(4)
      integer sgn,expsgn

      ls=len(str)
      c(1)=0
      c(2)=0
      c(3)=0
      c(4)=0
      sgn=1
      nv=1

      i=1
      ic=0
      if(str(i:i).eq.'-') then
         sgn=-1
         i=i+1
      elseif(str(i:i).eq.'+') then
         sgn=1
         i=i+1
      else
         sgn=1
      endif

 10   if(i.gt.ls) goto 20
      call s2int(str(i:),nlz,iv,ir,ierr)
      if(ierr.ne.0) iv=0
      i1=i-1+ir
      if(i1.le.ls) then
         next=str(i1:i1)
         if (next.eq.'e'.or.next.eq.'E'.or.
     $        next.eq.'d'.or.next.eq.'D') next='e'
         if (next.eq.','.or.next.eq.'/') next=' '
      else
         next=' '
      endif
      if(next.eq.'*') then
         if(ic.eq.0.and.iv.gt.0.and.sgn.gt.0) then
c     number of values
            nv=iv
            ic=1
c     mantissa sign
            if(str(i1+1:i1+1).eq.'-') then
               sgn=-1
               i1=i1+1
            elseif(str(i1+1:i1+1).eq.'+') then
               sgn=1
               i1=i1+1
            else
               sgn=1
            endif
         else
            goto 100
         endif
      elseif(next.eq.'.') then
c     end of mantissa's interger part
         if(ic.le.1) then
            c(1)=iv
            ic=2
         else
            goto 100
         endif
      elseif(next.eq.'e') then
c     end of mantissa
         if(ic.eq.3.or.ic.eq.2) then
            c(2)=iv
            c(3)=i1-i
         elseif(ic.le.1) then
            c(1)=iv
         else
            goto 100
         endif
         ic=4 
         if(i1+1.gt.ls) goto 100
         if(str(i1+1:i1+1).eq.'-') then
            expsgn=-1
            i1=i1+1
         elseif(str(i1+1:i1+1).eq.'+') then
            expsgn=1
            i1=i1+1
         else
            expsgn=1
         endif
      elseif(next.eq.' ') then
c     end of number definition
         if(ic.eq.4) then
            c(4)=iv
         elseif(ic.eq.3.or.ic.eq.2) then
            c(2)=iv
            c(3)=i1-i
         elseif(ic.le.1) then
            c(1)=iv
         else
            goto 100
         endif
         goto 20
      else
         goto 100
      endif
      i=i1+1
      goto 10
      
 20   continue
      ir=i1
      v=sgn*(c(1)+c(2)*10.0d0**(-c(3)))*10.0d0**(expsgn*c(4))
      return
 100  ierr=2
      return
      end
      subroutine s2int(str,nlz,v,ir,ierr)
c if the first characters of str contain representation of an integer >0
c     s2int returns 
c        in v the integer value, 
c        in nlz the number of leading zeros
c        in ir the first unsued character in str
c else
c     s2int returns ierr=1 and v=0 and nlz=0
c 
      character*(*) str
      character*1 si
      integer ls,nlz,v,ir,ierr
      logical first
c
      ierr=0
      ls=len(str)
      
c
      i=0
      v=0
      nlz=0
      first=.true.
 10   i=i+1
      if(i.gt.ls) goto 20
      si=str(i:i)
      if(si.eq.'0') then
         if(first) then
            nlz=nlz+1
         else
            v=10*v
         endif
      else
         first=.false.
         if(si.eq.'1') then
            v=10*v+1
         elseif(si.eq.'2') then
            v=10*v+2
         elseif(si.eq.'3') then
            v=10*v+3
         elseif(si.eq.'4') then
            v=10*v+4
         elseif(si.eq.'5') then
            v=10*v+5
         elseif(si.eq.'6') then
            v=10*v+6
         elseif(si.eq.'7') then
            v=10*v+7
         elseif(si.eq.'8') then
            v=10*v+8
         elseif(si.eq.'9') then
            v=10*v+9
         else
            if(i.eq.1) then
               ierr=1
               v=0
               nlz=0
            endif
            ir=i
            return
         endif
      endif
      goto 10
 20   continue
      ir=i
      return
      end
