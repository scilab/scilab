      subroutine funs(id)
c     ====================================================================
c     scan primitive function and scilab code function lists for a given name
c     ====================================================================
c     Copyright INRIA
      include '../stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2)
      integer id(nsiz),id1(nsiz),istr(nlgh)
c
      logical eqid,cresmat
      integer srhs,percen,blank,fptr,mode(2),sfun,slhs,r
      integer iadr
      data nclas/29/,percen/56/,blank/40/
c
      iadr(l)=l+l-1
c     
c     look only in scilab code function libraries
      if(fin.eq.-3) goto 35
      if(fin.eq.-4) goto 30
c     
c     
      if (comp(1).ne.0) then
c     if  compilation mode skip primitive functions
         fin=0
         fun=0
         return
      endif
c
c     look for name in primitive functions
      call funtab(id,fptr,1)
      if(fptr.le.0) then
         if(comp(1).eq.0.and.fin.ne.-5) goto 30
         fin=0
         fun=0
      else
         fun = fptr/100
         fin = mod(fptr,100)
      endif
      return
c     
c     is a scilab code function already loaded in the variables stack
 30   k=bot-1
 31   k=k+1
      if(k.gt.isiz) goto 35
      if(.not.eqid(idstk(1,k),id)) goto 31
      il=iadr(lstk(k))
c     modif 1.3 SS
      if(istk(il).ne.11.and.istk(il).ne.13) then
         fin=0
         fun=0
         return
      endif
      fin=k
      fun=-1
      return
c     
c     look in scilab code function libraries
 35   k=bot-1
 36   k=k+1
      if(k.ge.isiz) then
         fin=0
         fun=0
         return
      endif
      il=iadr(lstk(k))
      if(istk(il).ne.14) goto 36
      nbibn=istk(il+1)
      lbibn=il+2
      il=lbibn+nbibn
      ilp=il+1
      call namstr(id,istr,nn,1)
      ip=abs(istr(1))
      if(ip.eq.percen) ip=abs(istr(2))
      ip=max(1,ip-9)
      if(ip.gt.nclas) goto 36
      n=istk(ilp+ip)-istk(ilp+ip-1)
      if(n.eq.0) goto 36
      iln=ilp+nclas+1+(istk(ilp+ip-1)-1)*nsiz
      do 37 l=1,n
         if(eqid(id,istk(iln))) goto 39
         iln=iln+nsiz
 37   continue
      goto 36
c     
c     
 39   if(fin.ne.-1.and.fin.ne.-3) goto 40
      fun=k
      fin=l
      return
c     
 40   fin=l
      if(err1.ne.0) then
         fun=0
         fin=0
         return
      endif
c     
c     load it in the variables stack
      
c     create a variable with the bin file path
      n=nbibn
c     get name and its length
      call  namstr(id,istr,nn,1)
      top=top+1
      if(.not.cresmat(' ',top,1,1,nbibn+4+nn)) return
      call getsimat(fname,top,top,mp,np,1,1,ilp,nlp)
c     path
      call icopy(nbibn,istk(lbibn),1,istk(ilp),1)
c     name
      call icopy(nn,istr,1,istk(ilp+nbibn),1) 
c     extension
      call cvstr(4,istk(ilp+nbibn+nn),'.bin',0)
c     load variables stored in the given file
      srhs=rhs
      slhs=lhs
      fun=0
      rhs=1
      lhs=1
c     just avoid the case where rstk(pt) has been set to 906 by a
c     unterminated call to intload
      r=rstk(pt)
      rstk(pt)=0
      call intload(id,k)
      if(err.gt.0) return
      if (fun.eq.-1) then
         buf='Overloaded load cannot occur in this context'
         call error(999)
         return
      endif
      rstk(pt)=r
      rhs=srhs
      lhs=slhs
      top=top-1
      if(k.eq.0) then
c     .  requested varible not loaded
         fun=0
         fin=0
      else
         il=iadr(lstk(k))
         if(istk(il).ne.11.and.istk(il).ne.13.and.istk(il).ne.130) then
            fin=0
            fun=0
            return
         endif
         fun=-2
         fin=k
      endif

      return
c
      end




