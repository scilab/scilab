      subroutine matc(chai,lda,m,n,name,job)
c!but
c     this routine reads or writes a matrix of strings in scilab's
c     stack
c
c!calling sequence
c
c     integer       lda,m,n,job
c     character*(*) chai,name
c
c     chai     :array of size n*lda containing the matrix
c     lda     : number of rows of chai in the calling routine
c
c     name    : character string = name of scilab variable
c     job     : job= 0 scilab  -> fortran
c               job= 1 fortran -> scilab  
c
c    CAUTION: if job=1  m and n are defined by matc. 
c    must call matc as follows
c    call matc(ch,lda,m,n,name,0) 
c and NOT as:
c    call matc(ch,lda,10,10,name,0) if e.g. ch is a 
c    10 by 10 matrix of character string.
c
c     Copyright INRIA
      integer lda,m,n,job
      character*(*) chai(lda,*),name
      include '../stack.h'
      integer iadr,sadr
c
      integer i,j,k,k1,m1,n1
      integer il,it,l,l4,lec,nc,srhs,id(nsiz)
c
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
      nc=len(chai(1,1))
      if(lec.ge.1) goto 10
c
c lecture : scilab -> fortran
c -------
c
      fin=-1
      call stackg(id)
      if(err.gt.0) return
      if(fin.eq.0) call putid(ids(1,pt+1),id)
      if(fin.eq.0) call error(4)
      if(err.gt.0) return
      il=iadr(lstk(fin))
      if(istk(il).ne.10) call error(44)
      if(err.gt.0) return
c
      m=istk(il+1)
      n=istk(il+2)
      l=il+5
      k=l+m*n
      do 3 j=1,n
        do 2 i=1,m
          k1=istk(l)-istk(l-1)
          if(i.le.lda) then
            n1=min(k1,nc)
            chai(i,j)=' '
            call cvstr(n1,istk(k),chai(i,j),1)
          endif
          l=l+1
          k=k+k1
    2   continue
    3 continue
      m=min(m,lda)
c
      goto 99
c
c ecriture : fortran -> scilab
c --------
c
   10 continue
      if(top+2.ge.bot) call error(18)
      if(err.gt.0) return
      top=top+1
      il=iadr(lstk(top))
c
      m1=max(0,min(lda,m))
      n1=max(0,n)
      l=il+5
      err=l+m1*n1*(nc+1)-lstk(bot)
      if(err.gt.0) call error(17)
      if(err.gt.0) return
      istk(il)=10
      istk(il+1)=m1
      istk(il+2)=n1
      istk(il+4)=1
c
      k1=l+n1*m1
      do 13 j=1,n1
        do 12 i=1,m1
          do 11 k=1,nc
            call cvstr(1,istk(k1),chai(i,j)(k:k),0)
            k1=k1+1
   11     continue
          istk(l)=istk(l-1)+nc
          l=l+1
   12   continue
   13 continue
c
      lstk(top+1)=sadr(l+(nc+1)*m1*n1)
      l4=lct(4)
      lct(4)=-1
      call stackp(id,0)
      lct(4)=l4
      if(err.gt.0) return
      goto 99
c
c
   99 rhs=srhs
c
      end
