      subroutine lsosp(op,ma,na,a,mb,nb,nelb,indb,
c     Copyright INRIA
     $     nelc,indc,ierr)
c!purpose
c     compare the elements of a full boolean matrix A and a sparse boolean 
c     matrix B
c!parameters 
c     ma,na: row and column dimension of the a matrix  
c     mb,nb: row and column dimension of the b matrix 
c     a: (ma,na) array
c     nelb :integer: number of non zero elements of b
c     nelc :integer: 
c          on entry maximum number  of non zero elements of c
c          on return number of non zero elements of c
c     indb : b matrix control data:
c          indb(i) 1<=i<=mb contains the number of ith row non zero elements
c          of b
c          indb(mb+i) 1<=i<=nelb column index of each non zero element
c     
c     indc : on return contains c matrix control data:
c          indc(i) 1<=i<=nr contains the number of ith row non zero elements
c          of c
c          indc(mr+i) 1<=i<=nelc column index of each non zero element
c     ierr : if non zero initial value of nelc is to small
c     !
      integer a(ma,na)
      integer op,nr,nc,nelb,indb(*),nelc,indc(*),ierr
c     
      integer jc,ka,kb,jb,i,j2
      integer t
      logical lcompa,z
      external lcompa
c     
      nr=max(ma,mb)
      nc=max(na,nb)
c     
      nelmx=nelc
      ierr=0

c     jc counts elements of c.
      jc     = 1
c     ka,kb are numbers in first i rows of a,b.
      ka     = 1
      kb     = 1
      kc     = 1
c     jb counts elements of b.
      jb     = 1
c     i counts rows of a,b,c.
      if(ma*na.eq.1.and.mb*nb.gt.1) then
c     compare all element of b with scalar a
         t=a(1,1)
         z=lcompa(t,0,op)   
         do 10 i=1,nr
            indc(i)=0
            nirb=indb(i)
            jb=kb        
            jc=kc
            if(nirb.eq.0) then
               do 03 j=1,nc
                  if (lcompa(t,0,op)) then
                     if(jc+1.gt.nelmx) goto 99
                     indc(nr+jc)=j
                     jc=jc+1
                  endif
 03            continue
            else
               j2=indb(nr+jb)
               do 04 j=1,nc
                  if(j2.eq.j) then
                     if (lcompa(t,1,op)) then
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif 
                     if(jb-kb+1.lt.nirb) jb=jb+1
                     j2=indb(nr+jb)
                  else
                     if (z) then
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif 
                  endif
 04            continue
            endif
            kb=kb+indb(i)
            indc(i)=jc-kc
            kc=jc
 10      continue

      elseif(ma*na.gt.1.and.mb*nb.eq.1) then
c     compare all elements of a with scalar b  
         t=0
         if(indb(1).eq.1) t=1
         z=lcompa(0,t,op)
         do 20 i=1,nr
            indc(i)=0
            jc=kc
            do 12 j=1,nc
               if (lcompa(a(i,j),t,op)) then
                  if(jc+1.gt.nelmx) goto 99
                  indc(nr+jc)=j
                  jc=jc+1
               endif
 12         continue
            indc(i)=jc-kc
            kc=jc
 20      continue 
      else
         z=lcompa(0,0,op)   
         do 30 i=1,nr
            indc(i)=0
            nirb=indb(i)
            jb=kb        
            jc=kc
            if(nirb.eq.0) then
               do 23 j=1,nc
                  if (lcompa(a(i,j),0,op)) then
                     if(jc+1.gt.nelmx) goto 99
                     indc(nr+jc)=j
                     jc=jc+1
                  endif
 23            continue
            else
               j2=indb(nr+jb)
               do 24 j=1,nc
                  if(j2.eq.j) then
                     if (lcompa(a(i,j),1,op)) then
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif 
                     if(jb-kb+1.lt.nirb) jb=jb+1
                     j2=indb(nr+jb)
                  else
                     if (lcompa(a(i,j),0,op)) then
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif 
                  endif
 24            continue
            endif
            kb=kb+indb(i)
            indc(i)=jc-kc
            kc=jc
 30      continue
      endif
      nelc  = jc-1
      return
c     error messages.
 99   ierr=1
c     no more place for c

      return 
      end


