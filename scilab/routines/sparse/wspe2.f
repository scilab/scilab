      subroutine wspe2(ma,na,ar,ai,nela,inda,i,ni,j,nj,
c     Copyright INRIA
     $     mr,nr,rr,ri,nelr,indr,ptr,ierr)
c     extract a submatrix from a sparse matrix
c!
      integer inda(*),indr(*),i(*),j(*),ptr(*)
      integer ma,na,ni,nj,mr,nr,nela,nelr,ierr
      double precision ar(nela),ai(nela),rr(*),ri(*)
      logical allrow,allcol
c
      mr=ni
      nr=nj
      allrow=ni.lt.0
      allcol=nj.lt.0
      if(allrow) then 
         mr=ma
         ni=mr
      endif
      if(allcol) then 
         nr=na
         nj=na
      endif
      ptr(1)=1
      do 10 kk=1,ma
         ptr(kk+1)=ptr(kk)+inda(kk)
 10   continue
      jr=1
      do 40 l=1,mr
         indr(l)=0
         if(allrow) then
            ii=l
         else
            ii=i(l)
         endif
         if(inda(ii).eq.0) goto 40
         if(allcol) then
            indr(l)=inda(ii)
            call icopy(inda(ii),inda(ma+ptr(ii)),1,indr(mr+jr),1)
            call unsfdcopy(inda(ii),ar(ptr(ii)),1,rr(jr),1)
            call unsfdcopy(inda(ii),ai(ptr(ii)),1,ri(jr),1)
            jr=jr+inda(ii)
         else
            do 30 k=1,nj
               jj=j(k)
               do 20 kk=ptr(ii),ptr(ii+1)-1
                  if(inda(ma+kk).eq.jj) then
                     indr(l)=indr(l)+1
                     indr(mr+jr)=k
                     rr(jr)=ar(kk)
                     ri(jr)=ai(kk)
                     jr=jr+1
                     goto 30
                  endif
 20            continue
 30         continue
         endif
 40   continue
      nelr=jr-1
      return
      end



