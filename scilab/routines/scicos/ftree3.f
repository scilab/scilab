      subroutine ftree3(vec,nb,deput,typl,bexe,boptr,
c     Copyright INRIA
     $     blnk,blptr,kk,ord,nord,ok)
c make sure nb > 0
      integer vec(*),nb,deput(*),typl(*),bexe(*),boptr(*)
      integer blnk(*),blptr(*),ord(*),nord,ok,kk(*),fini
c
      ok=1
      do 150 j=1,nb+2
         fini=1
         do 100 i=1,nb
            if((vec(i).eq.j-1).and.(typl(i).ne.-1)) then
               if(j.eq.nb+2) then
                  ok=0
                  nord=0
                  return
               endif
               if(typl(i).eq.1) then
                  fini=0
                  nkk=boptr(i+1)-boptr(i)
                  if(nkk.ne.0) then
                     do 50 m=1,nkk
                        kk(m)=bexe(m+boptr(i)-1)
 50                  continue
                  endif
               else
                  nkk=0
                  if(blptr(i+1)-blptr(i).ne.0) then
                     do 60 m=blptr(i),blptr(i+1)-1
                        ii=blnk(m)
                        if((vec(ii).gt.-1).AND.((deput(ii).eq.1)
     $                       .OR.(typl(ii).eq.1))) then
                           fini=0
                           nkk=nkk+1
                           kk(nkk)=ii
                        endif
 60                  continue
                  endif
               endif
               if(nkk.ne.0) then
                  do 70 m=1,nkk
                     vec(kk(m))=j
 70               continue
               endif
            endif
 100     continue
         if(fini.eq.1) goto 200
 150  continue
 200  continue
      do 202 m=1,nb
         vec(m)=-vec(m)
 202  continue 
      call isort(vec,nb,ord)
      do 300 m=1,nb
      if(vec(m).lt.1) then
         if(m.eq.1) then
            nord=nb
            return
         else
            nord=nb-m+1
            do 250 mm=1,nord
               ord(mm)=ord(mm+nb-nord)
 250        continue
            return
         endif
      endif
 300  continue
      nord=0
      return
      end

