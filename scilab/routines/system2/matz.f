      subroutine matz(ar,ai,lda,m,n,name,job)
c!purpose
c     matz reads or writes in scilab's internal stack
c
c!calling sequence
c
c     integer lda,m,n,job
c     character*(*) name
c     double precision ar(lda,*),ai(lda,*)
c
c     ar,ai   : working array of size n*lda. Contains the a matrix
c               If a is real ai is null
c     lda     : number of rows in the calling program
c     name    : character string; name of the scilab variable.
c     job     : job= 0  scilab  -> fortran , real matrix
c               job= 1 fortran -> scilab  , real matrix
c               job=10  scilab  -> fortran , complex matrix
c               job=11 fortran -> scilab  , complex matrix
c
c    CAUTION: For scilab->fortran   m and n
c    are defined by  matz. Calling sequence must be:
c    call matz(x,y,lda,m,n,name,0) and NOT
c    call matz(x,y,lda,10,10,name,0) in the case where a
c     is a 10 by 10 matrix. (Here y is not referenced: real case).
c
c!
c     Copyright INRIA
      integer lda,m,n,job
      character*(*) name
      double precision ar(lda,*),ai(lda,*)
c
      include '../stack.h'
      integer iadr,sadr
      integer il,it,l,l4,lec,srhs,id(nsiz)
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      it=0
      if(job.ge.10) it=1
      lec=job-10*it
c
      lon=0
      do 100 i=1,csiz
         if(name(i:i).eq.char(0)) goto 200
         lon=lon+1
 100  continue
 200  continue
      lon1=len(name)
      if((lon1.gt.0).and.(lon1.lt.lon)) then
         ln=lon1
      else
         ln=lon
      endif
      if(lon.eq.lon1) ln=lon
      ln=min(nlgh,ln)
      call cvname(id,name(1:ln),0)
      srhs=rhs
      rhs=0
c
      if(lec.ge.1) goto 10
c
c read   : from scilab stack -> fortran variable ar
c -------
c
      fin=-1
      call stackg(id)
      if (err .gt. 0) return
      if (fin .eq. 0) then
         call putid(ids(1,pt+1),id)
         call error(4)
         return
      endif
      il=iadr(lstk(fin))
      if(istk(il).ne.1.or.istk(il+3).ne.it) call error(44)
      if(err.gt.0) return
      m=istk(il+1)
      n=istk(il+2)
      l=sadr(il+4)
      call dmcopy(stk(l),m,ar,lda,m,n)
      if(it.eq.1) call dmcopy(stk(l+m*n),m,ai,lda,m,n)
      goto 99
c
c write   : fortran -> scilab
c --------
c
   10 if(top+2.ge.bot) then
         call error(18)
         return
      endif
      top=top+1
      il=iadr(lstk(top))
      l=sadr(il+4)
      err=l+m*n*(it+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(il)=1
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=it
c
      call dmcopy(ar,lda,stk(l),m,m,n)
      if(it.eq.1) call dmcopy(ai,lda,stk(l+m*n),m,m,n)
      lstk(top+1)=l+m*n*(it+1)
      l4=lct(4)
      lct(4)=-1
      call stackp(id,0)
      lct(4)=l4
      if(err.gt.0) return
      goto 99
c
   99 rhs=srhs
      return
c
      end
