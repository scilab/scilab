      subroutine insertfield(ilfrom,volfrom,iltos,ind,nind,lrn,lw,info)
c     insert a field (from) in a sublist "to"  of a list "tos"
c
c     ilfrom points on the first entry of the "from" data structure in
c     istk. data structure is contained in istk(ilfrom:ilfrom-1+2*volfrom)

c     ilto points on the first entry of the "to" data structure in istk
c     iltos points on the first entry of the "tos" data structure in istk
c     ind(1:nind) is the path of to in tos
c
c     is the position of the result,if info<>0
c     info 
c        if 0 list has been updated "in place"
c        if 1 list needs to be stored
      include '../stack.h'
c     
      integer volfrom,voltos,volto,dvol,ind(*)
      integer typfrom,typto

      Integer subptr
      external subptr

      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(istk(ilfrom).lt.0) ilfrom=iadr(istk(ilfrom+1))
      typfrom=istk(ilfrom)
      lfromt=sadr(ilfrom)
c
      if(istk(iltos).lt.0) iltos=iadr(istk(iltos+1))
      mtos=istk(iltos+1)
c     voltos is the memory used to store all fields of tos
      voltos=istk(iltos+2+mtos)-1
c     ltos points on the first entry of the "tos" fields in stk
      ltos=sadr(iltos+mtos+3)
c
c     get pointer on the inner-most modified sublist
      n=ind(nind)
      il=iltos
      if(istk(il).lt.0) il=iadr(istk(il+1))
      mi=istk(il+1)
      if(nind.gt.1) then
         do 05 k1=1,nind-1
            il=iadr(sadr(il+3+mi)+istk(il+1+ind(k1))-1)
            if(istk(il).lt.0) il=iadr(istk(il+1))
            mi=istk(il+1)
 05      continue
      endif
c
      ilto=il
      typto=istk(ilto)
      mto=istk(ilto+1)
c     volto is the memory used to store all fields of to
      volto=istk(ilto+2+mto)-1
      lto=sadr(ilto+mto+3)
c

      if(n.eq.0) then
c     .  add a new element "on the left"
         if(typfrom.eq.0) then
c     .     null element : nothing added, "to" variable is returned as it
            info=0
            return
         endif

c     .  get memory to install the resulting list (May be improved to avoid
c     .  copy when "to" list is at the top of the stack).
         info=1
         lrn=lw
         iln=iadr(lrn)
         lw1=sadr(iadr(lrn)+3+(mtos+1))+voltos+volfrom
         err=lw1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif

         if(iltos.ne.ilto) then
c     .     copy the beginning of "tos" up to the beginning of the modified sub-list. 
            ltod=sadr(iltos)
            ltto=sadr(ilto)
            call unsfdcopy(ltto-ltod,stk(ltod),1,stk(lrn),1)
c     .     lrn points to the beginning of the new entry
            lrn=lrn+ltto-ltod
         endif

c     .  create new sublist:
c     .  header
         ilr=iadr(lrn)
         istk(ilr)=typto
         istk(ilr+1)=mto+1
         istk(ilr+2)=1
         istk(ilr+3)=1+volfrom
         do 10 i=1,mto
            istk(ilr+3+i)=istk(ilr+2+i)+istk(ilto+2+i)-istk(ilto+1+i)
 10      continue
c     .  added field
         lr=sadr(ilr+4+mto)
         call unsfdcopy(volfrom,stk(lfromt),1,stk(lr),1)
c     .  copy old sublist fields
         lr=lr+volfrom
         call unsfdcopy(volto,stk(lto),1,stk(lr),1)
         lr=lr+volto
         dvol=(lr-lrn)-(lto+volto-sadr(ilto))

c     .  update new data structure pointers recursively 
         call updptr(iln,ind,nind-1,dvol)
c     .  copy the rest of data structure
         if(nind.gt.1) then
            ind(nind-1)=ind(nind-1)+1
            ltto=sadr(subptr(iltos,ind,nind-1))
            call unsfdcopy(ltos+voltos-ltto,stk(ltto),1,stk(lr),1)
            lr=lr+ltos+voltos-ltto
         endif
c     .  put the result in place
         lrn=sadr(iln)
c         call unsfdcopy(lr-lrn,stk(lrn),1,stk(lstk(top)),1)
c         lstk(top+1)=lstk(top)+lr-lrn
         info=1
      elseif(n.gt.mto) then
c     .  add a new elements "on the right" of the sublist
         if(typfrom.eq.0) then
c     .     from is a null element : nothing added
            info=0
            return
         endif

c     .  get memory to install the resulting list (May be improved to avoid
c     .  copy when "to" list is at the top of the stack).
         lrn=lw
         iln=iadr(lrn)
         lw1=sadr(iln+3+mtos+n-mto)+voltos+volfrom
         info=1
         err=lw1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif

         if(ilto.ne.iltos) then
c     .     copy the beginning of "tos" up to the beginning of the modified sub-list. 
            ltod=sadr(iltos)
            ltto=sadr(ilto)
            err=lrn+ltto-ltod-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(ltto-ltod,stk(ltod),1,stk(lrn),1)
c     .     lrn points to the beginning of the new entry
            lrn=lrn+ltto-ltod
         endif

c     .  copy the beginning of the sublist header
         ilr=iadr(lrn)
         lr=sadr(ilr+3+n)
         err=lr+volto+volfrom-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(mto+3,istk(ilto),1,istk(ilr),1)
c     .  update fields number
         istk(ilr+1)=n
c     .  set pointers on added fields (some can be empty)
         call iset(n-mto,istk(ilr+mto+2),istk(ilr+mto+3),1)
         istk(ilr+n+2)=istk(ilr+n+1)+volfrom

c     .  copy old fields values
         call unsfdcopy(volto,stk(lto),1,stk(lr),1)
         lr=lr+volto
c     .  copy added field value
         call unsfdcopy(volfrom,stk(lfromt),1,stk(lr),1)
         lr=lr+volfrom
c
         dvol=(lr-lrn)-(lto+volto-sadr(ilto))
c     .  update new data structure pointers recursively 
         call updptr(iln,ind,nind-1,dvol)
c     .  copy the rest of data structure
c???         ind(nind-1)=ind(nind-1)+n-mto
         ind(nind-1)=ind(nind-1)+1
         ltto=sadr(subptr(iltos,ind,nind-1))
         if(ltos+voltos-ltto.gt.0) then
            err=(lr+ltos+voltos-ltto)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(ltos+voltos-ltto,stk(ltto),1,stk(lr),1)
            lr=lr+ltos+voltos-ltto
         endif

c     .  store result  ????
         lrn=sadr(iln)
         return
      else
c     .  replace or delete an intermediate field of the sublist
         if(typfrom.ne.0) then
c     .     replace the specified field
            if (istk(ilto+2+n)-istk(ilto+1+n).eq.volfrom) then
c     .        old and new fields have the same size,
c     .        field is  replaced in place
               lr=lto+istk(ilto+1+n)-1
               call unsfdcopy(volfrom,stk(lfromt),1,stk(lr),1)
               info=0
               return
            else
c     .        get memory to install the resulting list 
c     .        (May be improved to avoid copy when "to" list is at the
c     .        top of the stack).
               iln=iadr(lw)
               lrn=lw

               info=1
c     .        dvol the size variation of modified sub-element (-old+new)
               dvol=-(istk(ilto+2+n)-istk(ilto+1+n))+volfrom
               lw1=sadr(iln+3+mtos)+voltos+dvol
               err=lw1-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
c     .        lr,ilr points to the entry to be replaced
               lr=lto+istk(ilto+1+n)-1
               ilr=iadr(lr)
c     .        change list type if necessary (tlist and mlist are
c     .        changed to simple list)
c     .        copy the beginning of "to" up to field to be replaced
               ltod=sadr(iltos)
               call unsfdcopy(lr-ltod,stk(ltod),1,stk(lrn),1)
               if(nind.eq.1.and.n.eq.1.and.typfrom.ne.10) istk(iln)=15

c     .        lrn points to the beginning of the new field
               lrn=lrn+lr-ltod
c     .        set new value of the entry
               call unsfdcopy(volfrom,stk(lfromt),1,stk(lrn),1)
               lrn=lrn+volfrom
c     .        copy last field of "to"
c     .        iltol points to the end of "to" data structure
               ltol=ltos+voltos
               lto=lto+istk(ilto+2+n)-1
               call unsfdcopy(ltol-lto,stk(lto),1,stk(lrn),1)
               lrn=lrn+ltol-lto
c     .        update new data structure pointers recursively 
               call  updptr(iln,ind,nind,dvol)
c     .        store result
               lrn=lw
               return
            endif
         else
c     .     suppress the specified entry
c     .     get memory to install the resulting list 
c     .     (May be improved to avoid copy when "to" list is at the
c     .     top of the stack).
            lrn=lw
            info=1
            iln=iadr(lrn)
            lw1=sadr(iln+3+mtos)+voltos-(istk(ilto+2+n)-istk(ilto+1+n))
            err=lw1-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
c     .     if necessary,copy the "to" list data structure up to the  
c     .     beginning of the modified sub-list
            if(ilto.ne.iltos) then
c     .     copy the beginning of "to" up to  the modified sub-list
               ltod=sadr(iltos)
               ltto=sadr(ilto)
               call unsfdcopy(ltto-ltod,stk(ltod),1,stk(lrn),1)
c     .        lrn points to the beginning of the new entry
               lrn=lrn+ltto-ltod
            endif
c     .     update sub_list  
c     .     ---------------
            il=iadr(lrn)
            lfrom=lto-1+istk(ilto+2+n)
c     .     copy variable header and n-1 first pointers
            call icopy(2+n,istk(ilto),1,istk(il),1)
            if(n.eq.1) istk(il)=15
c     .     reduce list size
            istk(il+1)=istk(il+1)-1
c     .     modify last pointers
            do 20 i=n,mto
               istk(il+i+2)=istk(il+i+1)+istk(ilto+i+3)-istk(ilto+i+2)
 20         continue
c     .     copy first n-1 elements
            l=sadr(il+2+mto)
            call unsfdcopy(istk(il+n+1)-1,stk(lto),1,stk(l),1)
            l=l+istk(il+n+1)-1
c     .     dvol the size variation of modified sub-element (-old+new)
            dvol=(l-sadr(il))-(lfrom-sadr(ilto))
c     .     copy last elements
            call unsfdcopy(istk(il+1+mto)-istk(il+1+n),stk(lfrom),1,
     $           stk(l),1)
            l=l+istk(il+1+mto)-istk(il+1+n)
c     .     update new data structure pointers recursively 
            call updptr(iln,ind,nind-1,dvol)
c     .     copy the rest of data structure
            if(nind.gt.1) then
               ind(nind-1)=ind(nind-1)+1
               ltto=sadr(subptr(iltos,ind,nind-1))
               call unsfdcopy(ltos+voltos-ltto,stk(ltto),1,stk(l),1)
               l=l+ltos+voltos-ltto
            endif
c     .     store result
            lrn=sadr(iln)
            info=1
            return
         endif
      endif
      return
      end
      subroutine updptr(ilt,ind,nind,dvol)
c!purpose
c     update new data structure pointers recursively for nested lists
c!parameters
c     ilt : pointer to the beginning of the list
c     ind : vector of indexes giving modified entry path.
c     nind: size of the path
c     dvol: difference of the old and new sizes of the modified entry
c!
      integer ind(*),dvol
      integer iadr,sadr
      include '../stack.h'

c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=ilt
      if(nind.le.0) return
c
      do 20 k=1,nind
c     .  m : sub-list size
         m=istk(il+1)
         mi=ind(k)
c     .  update pointers to entries following the mi sub list entry
         do 10 i=mi,m
            istk(il+i+2)=istk(il+i+2)+dvol
 10      continue
c     .  il pointer to ind(k) sub-list entry
         il=iadr(sadr(il+3+m)+istk(il+1+mi)-1)
 20   continue
      end

      integer function subptr(ilt,ind,nind)
c     !purpose
c     get pointer to an entry of a  nested list
c     !parameters
c     ilt : pointer to the beginning of the list
c     ind : vector of indexes giving modified entry path.
c     nind: size of the path
c     !
      integer ind(nind)
      integer iadr,sadr
      include '../stack.h'
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=ilt
      if(nind.le.0) goto 30
c
      do 20 k=1,nind
c     .  m : sub-list size
         m=istk(il+1)
c     .  il pointer to ind(k) sub-list entry
         il=iadr(sadr(il+3+m)+istk(il+1+ind(k))-1)
 20   continue
 30     subptr=il
      end
      
