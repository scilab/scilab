      subroutine wspos(op,ma,na,ar,ai,nela,inda,mb,nb,br,bi,
c     Copyright INRIA
     $     nelc,indc,ierr,ita,itb)
c!purpose
c     compare the elements of a  sparse matrix A and a full matrix B.
c!parameters     
c     a : array. 
c         Contain non zero elements of the A matrix
c     ma,na: row and column dimension of the a matrix  
c     mb,nb: row and column dimension of the b matrix  
c     nela :integer: number of non zero elements of a
c     nelc :integer: 
c           on entry maximum number  of non zero elements of c
c           on return number of non zero elements of c
c     inda : a matrix control data:
c            inda(i) 1<=i<=ma contains the number of ith row non zero elements
c            of a
c            inda(ma+i) 1<=i<=nela column index of each non zero element
c     indc : on return contains c matrix control data:
c            indc(i) 1<=i<=mr contains the number of ith row non zero elements
c            of c
c            indc(mr+i) 1<=i<=nelb column index of each non zero element
c     b    :(mb,nb) matrix
c     ierr : if non zero initial value of nelc is to small
c     !
      double precision ar(nela),ai(nela),br(mb,nb),bi(mb,nb)
      integer op,nr,nc,nela,inda(*),nelc,indc(*),ierr
c     
      integer jc,ka,kb,jb,i,ja,j1
      double precision tr,ti,tai,tbi
      logical wcompa,z,z1
      external wcompa
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
         tr=0.0d0
         ti=0.0d0
         if(inda(1).eq.1) then
            tr=ar(1)
            ti=ai(1)
         endif
         z=wcompa(tr,ti,0.0d0,0.0d0,op)   
         do 10 i=1,nr
            indc(i)=0
            jc=kc
            do 04 j=1,nc                    
               if(itb.eq.0) then
                  z1=wcompa(tr,ti,br(i,j),0.0d0,op)
               else
                  z1=wcompa(tr,ti,br(i,j),bi(i,j),op)
               endif
               if (z1) then
                  if(jc+1.gt.nelmx) goto 99
                  indc(nr+jc)=j
                  jc=jc+1
               endif 
 04         continue
            indc(i)=jc-kc
            kc=jc
 10      continue

      elseif(ma*na.gt.1.and.mb*nb.eq.1) then
c     compare all elements of a with scalar b 
         tr=br(1,1)
         ti=0.0d0
         if(itb.ne.0) ti=bi(1,1)
         z=wcompa(0.0d0,0.0d0,tr,ti,op) 
         do 20 i=1,nr
            indc(i)=0
            nira=inda(i)
            ja=ka       
            jc=kc
            if(nira.eq.0) then
               if(z) then
                  if(kc+nc.gt.nelmx) goto 99
                  indc(i)=nc
                  do 11 j=1,nc
                     indc(nr+kc-1+j)=j
 11               continue
                  jc=kc+nc
               endif
            else
               j1=inda(nr+ja)
               do 12 j=1,nc
                  if(j1.eq.j) then
                     if(ita.eq.0) then
                        z1=wcompa(ar(ja),0.0d0,tr,ti,op)
                     else
                        z1=wcompa(ar(ja),ai(ja),tr,ti,op)
                     endif
                     if (z1) then                     
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif
                     if(ja-ka+1.lt.nira) ja=ja+1
                     j1=inda(nr+ja)
                  elseif(z) then
                     if(jc+1.gt.nelmx) goto 99
                     indc(nr+jc)=j
                     jc=jc+1
                  endif
 12            continue
            endif
            indc(i)=jc-kc
            ka=ka+nira
            kc=jc
 20      continue 
      else
         z=wcompa(0.0d0,0.0d0,0.0d0,0.0d0,op)   
         do 30 i=1,nr
            indc(i)=0
            nira=inda(i)
            ja=ka
            jc=kc
            if(nira.eq.0) then
               do 22 j=1,nc
                  if(itb.eq.0) then
                     z1=wcompa(0.0d0,0.0d0,br(i,j),0.0d0,op)
                  else
                     z1=wcompa(0.0d0,0.0d0,br(i,j),bi(i,j),op)
                  endif
                  if (z1) then
                     if(jc+1.gt.nelmx) goto 99
                     indc(nr+jc)=j
                     jc=jc+1
                  endif
 22            continue
            else
               j1=inda(nr+ja)
               do 24 j=1,nc
                  if(j1.eq.j) then
                     if(ita.eq.0) then
                        tai=0.0d0
                     else
                        tai=ai(ja)
                     endif
                     if(itb.eq.0) then
                        tbi=0.0d0
                     else
                        tbi=bi(i,j)
                     endif
                     if (wcompa(ar(ja),tai,br(i,j),tbi,op)) then
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif 
                     if(ja-ka+1.lt.nira) ja=ja+1
                     j1=inda(nr+ja)
                  else
                     if(itb.eq.0) then
                        z1=wcompa(0.0d0,0.0d0,br(i,j),0.0d0,op)
                     else
                        z1=wcompa(0.0d0,0.0d0,br(i,j),bi(i,j),op)
                     endif
                     if (z1) then
                        if(jc+1.gt.nelmx) goto 99
                        indc(nr+jc)=j
                        jc=jc+1
                     endif
                  endif
 24            continue
            endif
            ka=ka+inda(i)
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

