
c     Copyright INRIA
      subroutine savlod(lunit,id,job,nvar)
c ====================================================================
c     Binary save and load of Scilab Objects 
c ====================================================================
      include '../stack.h'
      integer blank
      parameter (blank=40)
      integer izero,block
      parameter (izero=0,block=100000)
      logical eqid
      integer lunit,id(nsiz),job,h(nsiz)
      integer iadr,sadr
c     last variables for compatibility with old save files
      integer n,iblock,old,first
      save old,first,h,n,iblock
c
c     lunit = logical unit number
c     id = name, format 4a1
c     nvar  variable number in the stack
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (job .eq. -1) then
c     init save (only first and last wrote values are significant)
         write(lunit,err=32) block,(izero,i=1,nsiz-1),izero
         iblock=block
         return
      endif
      if (job .eq. -2) then
c     init load
         read(lunit,end=31,err=32) (h(i),i=1,nsiz),n
         if(n.eq.0) then
c     a new save file 
            old=0
            first=0
            iblock=h(1)
            h(1)=0
         else
c     an old format file
            old=1
            first=1
            iblock=0
         endif
         return
      endif
      if (job .gt. 0) go to 20
c     
c     save
 10   il=iadr(lstk(nvar))
      ilf=iadr(lstk(nvar+1))-1
      write(lunit,err=32) id,ilf-il+1
      n=ilf-il+1
      nw=n/block
      i1=0
      if(nw.gt.0) then
         do 19 ii=1,nw
            write(lunit,err=32) (istk(il+i1+i-1),i=1,block)
            i1=i1+block
 19      continue
      endif
      if(i1.lt.n) write(lunit,err=32) (istk(il+i-1),i=i1+1,n)
      return
c     
c     load
 20   continue
      il=iadr(lstk(nvar))
      if(id(1).eq.blank) goto 25
c     
 21   if (first.eq.0) then
         read(lunit,end=31,err=32) (h(i),i=1,nsiz),n
      endif
      nw=n/block
      first=0
 22   if(eqid(h,id)) goto 26
      if(old.eq.1) then
         read(lunit,end=30,err=32)
      else
         do 23 ii=1,nw
            read(lunit,end=30,err=32)
 23      continue
         if(nw*iblock.lt.n) read(lunit,end=30,err=32)
      endif
      goto 21
c     
 25   if (first.eq.0) then
         read(lunit,end=30,err=32) (h(i),i=1,nsiz),n
      endif
      first=0
 26   if(n-iadr(job).gt.0) call error(17)
      if(err.gt.0) goto 30
      if(old.eq.1) then 
         read(lunit,end=30,err=32) (istk(il+i-1),i=1,n)
      else
         nw=n/iblock
         i1=0
         if(nw.gt.0) then
            do 29 ii=1,nw
               read(lunit,end=30,err=32) (istk(il+i1+i-1),i=1,iblock)
               i1=i1+iblock
 29         continue
         endif
         if(i1.lt.n) read(lunit,end=30,err=32) (istk(il+i-1),i=i1+1,n)
      endif
      lstk(nvar+1)=sadr(il+n)
      call putid(id,h)
      return
c     
c     end of file
 30   il=iadr(lstk(nvar))
      istk(il)=0
      lstk(nvar+1)=lstk(nvar)+1
      return
c     looked for Variable does not exist
 31   call putid(ids(1,pt+1),id)
      call error(102)
      goto 30
c     file has an incorrect structure
 32   call error(49)
      goto 30

      end
