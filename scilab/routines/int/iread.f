      subroutine iread(it)
c     Copyright INRIA
c
      INCLUDE '../stack.h'
c     
      integer top2,tops,id(nlgh),h(nsiz)
      integer ftyp,fmttyp,mode(2)
      logical opened,eptover
      integer iadr,sadr
      logical checkrhs,checklhs
c
      save opened,lunit,job,icomp
c     

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matio '//buf(1:4))
      endif
c     
      tops=top
c
 120  continue
      lw=lstk(top+1)
      if(.not.checkrhs('iread',3,5)) return
      if(.not.checklhs('iread',1,1)) return

c
c     opening file
      top2 = top
      top = top-rhs+1
      il=iadr(lstk(top))
      mode(1)=-1
      mode(2)=0
      call v2unit(top,mode,lunit,opened,ierr)
      if(ierr.gt.0) return
c     
      iacces=0
      if(rhs.gt.3) then
         il=iadr(lstk(top2))
         if(rhs.eq.5) then
            iacces=top2-1
         else
            if(istk(il).eq.1) then
               call error(61)
               goto 996
            endif
         endif
c     analyse du format
         if(istk(il).ne.10) then
            err=rhs
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
      endif
c
 121  if(iacces.ne.0) then
c     get record numbers
         ilb=iadr(lstk(iacces))
         if(istk(ilb+3).ne.0) then
            err=top2-top+1
            call error(52)
            goto 996
         endif
         nb=istk(ilb+1)*istk(ilb+2)
         lb=sadr(ilb+4)
         ilb=iadr(lw)
         err=sadr(ilb+nb)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            goto 996
         endif
         do 122 i=1,nb
            istk(ilb+i-1)=int(stk(lb+i-1))
 122     continue
         top2=iacces-1
      endif

 123  continue
c     number of rows
      il=iadr(lstk(top2))
      l=sadr(il+4)
      n=int(stk(l))
      top2=top2-1
c     number of columns
      il=iadr(lstk(top2))
      l=sadr(il+4)
      m=int(stk(l))
      il=iadr(lstk(top))
      l=sadr(il+4)

      if(m.eq.0.or.n.le.0) then
c     terminate if nothing to read
         istk(il)=1
         istk(il+1)=0
         istk(il+2)=0
         istk(il+3)=0
         lstk(top+1)=sadr(il+4)
         if(.not.opened) then
            mode(1)=0
            mode(2)=0
            call clunit(-lunit,buf,mode)
         endif
         goto 999
      endif
c     create header of variable to return
      istk(il)=8
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=it
      l=il+4

      lw=l+(m*n*it/4+1)
      err=sadr(lw+n*it/4+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if(fin.lt.0.and.ftyp.ne.1) then
         buf='incorrect format'
         call error(9999)
         return
      endif
      if(fin.gt.0) then 
         buf(1:1)='*'
         nc=1
      endif
c     
      if(iacces.eq.0) then
c     sequential acces 
         if(m.gt.0) then
c     .     specified number of rows
            if(it.eq.4) then
               call mibasin(ierr,lunit,buf(1:nc),istk(l),m,n)
            elseif(it.eq.2) then
               call mi2basin(ierr,lunit,buf(1:nc),istk(l),m,n)
            elseif(it.eq.1) then
               call mi2basin(ierr,lunit,buf(1:nc),istk(l),m,n)
               call i22i1(m*n,istk(l),1,istk(l),1)
            endif
            if(ierr.eq.1) then
               goto 997
            elseif(ierr.eq.2) then
               goto 998
            endif

         else
c     .     unspecified number of rows
            m=-(iadr(lstk(bot))-li)*it/4
            if(it.eq.4) then
               call mibasin(ierr,lunit,buf(1:nc),istk(l),n,m)
            elseif(it.eq.2) then
               call mi2basin(ierr,lunit,buf(1:nc),istk(l),n,m)
            elseif(it.eq.1) then
               call mi2basin(ierr,lunit,buf(1:nc),istk(l),n,m)
               call i22i1(n*m,istk(l),1,istk(l),1)
            endif
            if(ierr.eq.0) goto 1241
            if(ierr.eq.2) goto 998
            if(m.ne.0) then
               if(m.ge.1.and.n.ge.1) then
                  err=sadr(l+2*m*n*it/4)-lstk(bot)
                  if(err.gt.0) then
                     call error(17)
                     return
                  endif
                  l1=l+m*n
                  if(it.eq.4) then
                     call icopy(m*n,istk(l),1,istk(l1),1)
                     call imtran(istk(l1),n,istk(l),m,n,m)
                  elseif(it.eq.2) then
                     call i2copy(m*n,istk(l),1,istk(l1),1)
                     call i2mtran(istk(l1),n,istk(l),m,n,m)
                  elseif(it.eq.1) then
                     call i1copy(m*n,istk(l),1,istk(l1),1)
                     call i1mtran(istk(l1),n,istk(l),m,n,m)
                  endif
                  istk(il+1)=m
               endif
            else
               istk(il+1)=0
               istk(il+2)=0
            endif
         endif         
      else
c     acces direct
         if(nb.ne.m) then
            call error(42)
            goto 996
         endif
         if(fin.gt.0) then
            call error(61)
            goto 996
         endif
         if(m.lt.0) then
            call  error(43)
            return
         endif
         if(it.eq.4) then
            call irecread(lunit,buf(1:nc),istk(ilb),n,m,istk(l),ierr)
         elseif(it.eq.2) then
            call i2recread(lunit,buf(1:nc),istk(ilb),n,m,istk(l),ierr)
         elseif(it.eq.1) then
            call i2recread(lunit,buf(1:nc),istk(ilb),n,m,istk(l),ierr)
            call i22i1(m*n, istk(l),1, istk(l),1)
         endif
         if(ierr.eq.1) goto 998
      endif
      lstk(top+1)=sadr(l+m*n*it/4+1)
      if(.not.opened) then
         mode(1)=0
         mode(2)=0
         call clunit(-lunit,buf,mode)
      endif
      goto 999
 996  if(.not.opened) call clunit(-lunit,buf,mode)
      return
 997  err=i
      call error(62)
      if(.not.opened) call clunit(-lunit,buf,mode)
      return
 998  call error(49)
      if(.not.opened) call clunit(-lunit,buf,mode)
      return
c     
 999  return
      end

      subroutine irecread(lunit,fmt,records,n,m,a,ierr)
      integer lunit,records(*),n,m,ia,a(m,*),ierr
      character*(*) fmt

      do 10 i=1,m
         read(lunit,fmt,rec=records(i),err=20) (a(i,j),j=1,n)
 10   continue
      ierr=0
      return
 20   ierr=1
      return
      end

      subroutine i2recread(lunit,fmt,records,n,m,a,ierr)
      integer lunit,records(*),n,m,ia,ierr
      integer*2 a(m,*)
      character*(*) fmt

      do 10 i=1,m
         read(lunit,fmt,rec=records(i),err=20) (a(i,j),j=1,n)
 10   continue
      ierr=0
      return
 20   ierr=1
      return
      end


      subroutine mibasin(ierr,lunit,fmt,v,m,n)
c     Copyright INRIA
      include '../stack.h'
      character*(*) fmt
      integer v(*)
      integer ierr,lunit
      character*512 string
c
      ierr=0
      if(lunit.eq.rte) then
         string=' '
         call xscion(iflag)
         if(m.gt.0) then
         do 01 l=1,m
            call zzledt(string,len(string),lline,status,0,iflag)
            if(status.ne.0) goto 10
            if (lline.eq.0) then
               string(1:1)=' '
               lline=1
            endif
            if(fmt(1:1).ne.'*') then
               read(string(1:lline),fmt,end=10,err=20) (v(l-1+i),
     $              i=1,n*iv,iv)
            else
               call rldint(string(1:lline),v(l),iv,n1,n,ierr)
               if(ierr.ne.0) goto 20
               if(n1.lt.n) goto 10
            endif
 01      continue
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

      subroutine i2basin(ierr,lunit,fmt,v,iv,n)
c     Copyright INRIA
      include '../stack.h'
      character*(*) fmt
      integer*2 v(*)
      integer ierr,lunit
      character*512 string
c
      ierr=0
      if(lunit.eq.rte) then
         string=' '
         call xscion(iflag)
         call zzledt(string,len(string),lline,status,0,iflag)
         if(status.ne.0) goto 10
         if (lline.eq.0) then
            string(1:1)=' '
            lline=1
	 endif
         if(fmt(1:1).ne.'*') then
            read(string(1:lline),fmt,end=10,err=20) (v(i),i=1,n*iv,iv)
         else
            call rldi2(string(1:lline),v,iv,n1,n,ierr)
            if(ierr.ne.0) goto 20
            if(n1.lt.n) goto 10
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


      subroutine rldint(str,v,iv,n,maxv,ierr)
c!purpose
c     internal "list-directed" read mode is not standard fortran. 
c     This routine emulates :read(str,*) (v(1,i),i=1,n)
c!
      integer v(iv,maxv),vv
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
c     loop on descriptors
      if(i.gt.ls) goto 20
      si=str(i:i)
      if(si.eq.'/'.or.si.eq.',') then
         if(del) then
            if(n+1.gt.maxv) return
            v(1,n+1)=0
            n=n+1
            goto 10
         else
            del=.true.
         endif
      endif
      if(si.ne.' '.and.si.ne.'/'.and.si.ne.',') then
c     conversion of a descriptor to a (repeated) value
         call nextint(str(i:),vv,nv,ir,ierr)
         if(ierr.ne.0) return
         if(n+nv.gt.maxv) nv=maxv-n
         if(nv.le.0)  return
         call iset(nv,vv,v(1,n+1),iv)
         n=n+nv
         i=i+ir-2
         del=.false.
      endif
      goto 10
 20   continue 
      if(del) then
         if(n+1.gt.maxv) return
         v(1,n+1)=0
         n=n+1
      endif
      return
      end

      subroutine rldi2(str,v,iv,n,maxv,ierr)
c!purpose
c     internal "list-directed" read mode is not standard fortran. 
c     This routine emulates :read(str,*) (v(1,i),i=1,n)
c!
      integer*2 v(iv,maxv),vv2
      integer vv
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
c     loop on descriptors
      if(i.gt.ls) goto 20
      si=str(i:i)
      if(si.eq.'/'.or.si.eq.',') then
         if(del) then
            if(n+1.gt.maxv) return
            v(1,n+1)=0
            n=n+1
            goto 10
         else
            del=.true.
         endif
      endif
      if(si.ne.' '.and.si.ne.'/'.and.si.ne.',') then
c     conversion of a descriptor to a (repeated) value
         call nextint(str(i:),vv,nv,ir,ierr)
         if(ierr.ne.0) return
         if(n+nv.gt.maxv) nv=maxv-n
         if(nv.le.0)  return
         call i42i2(1,vv,1,vv2,1)
         call i2set(nv,vv2,v(1,n+1),iv)
         n=n+nv
         i=i+ir-2
         del=.false.
      endif
      goto 10
 20   continue 
      if(del) then
         if(n+1.gt.maxv) return
         v(1,n+1)=0
         n=n+1
      endif
      return
      end

      subroutine nextint(str,v,nv,ir,ierr)
c     form integer number described by a string with format:
c      [nn*][+|-][mmmmm]
      character*(*) str
      character*1 next
      integer v
      integer sgn

      ls=len(str)
      nv=1
      i=1
      ic=0
c
 01   if(str(i:i).eq.'-') then
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
      i=i-1+ir
      if(i.le.ls) then
         next=str(i:i)
      else
         next=' '
      endif
      if (next.eq.','.or.next.eq.'/') next=' '
c
      if(next.eq.'*') then
         if(ic.eq.0.and.iv.gt.0.and.sgn.gt.0) then
c     number of values
            nv=iv
            i=i+1
            goto 10
         else
            ierr=2
            return
         endif
      elseif(next.eq.' ') then
c     end of number definition
         goto 20
      else
         ierr=2
         return
      endif
      
 20   continue
      ir=i
      v=sgn*iv
      return
      end
