      subroutine matzs(ar,ai,lda,m,n,name,job)
c!Kpurpose
c     same as matz but for real (single precision) matrix.
c
c!
c     Copyright INRIA
      integer lda,m,n,job
      character*(*) name
      real ar(lda,*),ai(lda,*)
c
      include '../stack.h'
      integer iadr,sadr
      integer il,it,l,l4,lec,nc,srhs,id(nsiz)
      integer i,j,k
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      it=0
      if(job.ge.10) it=1
      lec=job-10*it
c
      nc=min(nlgh,len(name))
      call cvname(id,name(1:nc),0)
      srhs=rhs
      rhs=0
c
      if(lec.ge.1) goto 10
c
c lecture : scilab -> fortran
c -------
c
      fin=-1
      call stackg(id)
      if (err .gt. 0) return
      if (fin .eq. 0) call putid(ids(1,pt+1),id)
      if (fin .eq. 0) call error(4)
      if (err .gt. 0) return
      il=iadr(lstk(fin))
      if(istk(il).ne.1.or.istk(il+3).ne.it) call error(44)
      if(err.gt.0) return
      m=istk(il+1)
      n=istk(il+2)
      l=sadr(il+4)
c
      k=l
      do 03 j=1,n
        do 02 i=1,min(lda,m)
          ar(i,j)=real(stk(k))
          k=k+1
   02   continue
   03 continue
      if(it.ne.1) goto 07
      do 05 j=1,n
        do 04 i=1,min(lda,m)
          ai(i,j)=real(stk(k))
          k=k+1
   04   continue
   05 continue
c
   07 continue
      goto 99
c
c ecriture : fortran -> scilab
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
      k=l
      do 13 j=1,n
        do 12 i=1,min(lda,m)
          stk(k)=dble(ar(i,j))
          k=k+1
   12   continue
   13 continue
      if(it.ne.1) goto 16
      do 15 j=1,n
        do 14 i=1,min(lda,m)
          stk(k)=dble(ai(i,j))
          k=k+1
   14   continue
   15 continue
c
   16 continue
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
