      subroutine dspis(ma,na,a,nela,inda,i,ni,j,nj,
c     Copyright INRIA
     $     mb,nb,b,mr,nr,r,nelr,indr,ierr)
c     insert a full submatrix in a sparse matrix
c!
      integer inda(*),indr(*),i(*),j(*)
      integer mb,nb
      integer ma,na,ni,nj,mr,nr,nela,nelr,ierr
      double precision a(*),r(*),b(mb,*)
      logical allrow,allcol,bscal
      integer findl
      external findl
c
      nelmx=nelr
      ierr=0
      mr=ni
      nr=nj
      allrow=ni.lt.0
      allcol=nj.lt.0
      
      
      if(allrow) then 
         mr=ma
      else
         mi=0
         do 01 kk=1,ni
            mi=max(mi,i(kk))
 01      continue
         mr=max(ma,mi)
      endif
      if(allcol) then 
         nr=na
      else
         mj=0
         do 02 kk=1,nj
            mj=max(mj,j(kk))
 02      continue
         nr=max(na,mj)
      endif
      if (allrow.and.allcol) then
c     a(:,:)=b
         bscal=mb*nb.eq.1.and.ma*na.gt.1
         if(.not.bscal) then
            if(mb*nb.gt.nelmx) then
               ierr=1
               return
            endif
            call iset(mb,0,indr,1)
            jr=1
            if(mb*nb.ne.0) then
               do 04 l=1,mb
                  do 03 k=1,nb
                     if(b(l,k).ne.0.0d0) then
                        indr(mb+jr)=k
                        r(jr)=b(l,k)
                        indr(l)=indr(l)+1
                        jr=jr+1
                     endif
 03               continue
 04            continue
            endif
            nelr=jr-1
            return
         else
            if(ma*na.gt.nelmx) then
               ierr=1
               return
            endif
            call iset(ma,0,indr,1)
            jr=1
            if(ma*na.ne.0) then
               do 06 l=1,ma
                  do 05 k=1,na
                     if(b(1,1).ne.0.0d0) then
                        indr(ma+jr)=k
                        r(jr)=b(1,1)
                        indr(l)=indr(l)+1
                        jr=jr+1
                     endif
 05               continue
 06            continue
            endif
            nelr=jr-1
            return
         endif
      elseif(allcol) then
c     a(i,:)=b
         bscal=mb*nb.eq.1.and.na*ni.gt.1
         jr=1
         ja=1

         do 21 l=1,mr
            indr(l)=0
            ii=findl(l,i,ni)
            if(ii.eq.0) then
c     this line is not modified
               if(l.le.ma) then
                  indr(l)=inda(l)
                  call icopy(indr(l),inda(ma+ja),1,indr(mr+jr),1)
                  call unsfdcopy(indr(l),a(ja),1,r(jr),1)
                  jr=jr+indr(l)
                  ja=ja+indr(l)
               else
                  indr(l)=0
               endif
            else
c     all this line is replaced by corresponding b line
               if(.not.bscal) then 
                  if(nb.gt.0) then
                     if(jr+nb.gt.nelmx) then
                        ierr=1
                        return
                     endif
                     do 19 k1=1,nb
                        if(b(ii,k1).ne.0.0d0) then
                           indr(mr+jr)=k1
                           indr(l)=indr(l)+1               
                           r(jr)=b(ii,k1)
                           jr=jr+1
                        endif
 19                  continue
                  endif
               else
                  if(na.gt.0) then
                     if(b(1,1).ne.0.0d0) then
                        if(jr+na.gt.nelmx) then
                           ierr=1
                           return
                        endif
                        do 20 k1=1,na
                           indr(mr+jr)=k1
                           indr(l)=indr(l)+1               
                           r(jr)=b(1,1)
                           jr=jr+1
 20                     continue
                     endif
                  endif
               endif
               if(l.le.ma) then
                  ja=ja+inda(l)
               endif
            endif
 21      continue
         nelr=jr-1
         if(nb.eq.0) mr=mr-ni
      elseif(allrow) then
c     a(:,j)=b
         bscal=mb*nb.eq.1.and.na*nj.gt.1
         jr=1
         ja=0
         do 35 l=1,ma
            ja1=1
            nal=inda(l)
            indr(l)=0
            kr=1
            do 31 k=1,nr
               jj=findl(k,j,nj)
               if(jj.eq.0) then
c     the  a(l,k) element is not modified insert it in r if non zero
                  if(ja1.le.nal.and.inda(ma+ja+ja1).eq.k) then
c     *              a(l,k) element is non zero
                     r(jr)=a(ja+ja1)
                     indr(l)=indr(l)+1
                     indr(mr+jr)=kr
                     jr=jr+1
                     ja1=ja1+1
                  endif
                  kr=kr+1
               else
c     the  a(l,k) element is replaced by b(l,jj) element 
                  if(.not.bscal) then
                     if(nb.gt.0) then
                        if(b(l,jj).ne.0.0d0) then
                           if(jr+1.gt.nelmx) then
                              ierr=1
                              return
                           endif
                           r(jr)=b(l,jj)
                           indr(l)=indr(l)+1
                           indr(mr+jr)=kr
                           jr=jr+1
                        endif
                        kr=kr+1
                     endif
                  else
                     if(na.gt.0) then
                        if(b(1,1).ne.0.0d0) then
                           if(jr+1.gt.nelmx) then
                              ierr=1
                              return
                           endif
                           r(jr)=b(1,1)
                           indr(l)=indr(l)+1
                           indr(mr+jr)=kr
                           jr=jr+1
                        endif
                        kr=kr+1
                     endif

                  endif
               endif
               if(ja1.le.nal.and.inda(ma+ja+ja1).eq.k) ja1=ja1+1
 31         continue
            ja=ja+nal
 35      continue
         nelr=jr-1
         if(nb.eq.0) nr=nr-nj
         return
      else
c     a(i,j)=b
         bscal=mb*nb.eq.1.and.ni*nj.gt.1
         jr=1
         ja=0
         do 45 l=1,mr
            ja1=1
            if(l.le.ma) then 
               nal=inda(l)
            else
               nal=0
            endif
            indr(l)=0
            ii=findl(l,i,ni)
            if(ii.eq.0) then
c     *     the a(l,:) is not modified
               if(l.le.ma) then
                  indr(l)=inda(l)
                  call icopy(indr(l),inda(ma+ja+ja1),1,indr(mr+jr),1)
                  call unsfdcopy(indr(l),a(ja+ja1),1,r(jr),1)
                  jr=jr+indr(l)
                  ja1=ja1+indr(l)
               else
                  indr(l)=0
               endif
            else
               kr=1
               do 42 k=1,nr
                  jj=findl(k,j,nj)
                  if(jj.eq.0) then
c     *           insert a(l,k) element  in r if non zero
                     if(ja1.le.nal.and.inda(ma+ja+ja1).eq.k) then
c     *              a(l,k) element is non zero
                        r(jr)=a(ja+ja1)
                        indr(l)=indr(l)+1
                        indr(mr+jr)=kr
                        jr=jr+1
                        ja1=ja1+1
                     endif
                     kr=kr+1
                  else
c     *           replace a(l,k) element  by b(ii,jj) element if non
c                     zero
                     if(.not.bscal) then
                        if(nb.gt.0) then
                           if(b(ii,jj).ne.0.0d0) then
                              if(jr+1.gt.nelmx) then
                                 ierr=1
                                 return
                              endif
                              r(jr)=b(ii,jj)
                              indr(l)=indr(l)+1
                              indr(mr+jr)=kr
                              jr=jr+1
                           endif
                           kr=kr+1
                        endif
                        if(ja1.le.nal.and.inda(ma+ja+ja1).eq.k) ja1=ja1
     $                       +1
                     else
                        if(na.gt.0) then
                           if(b(1,1).ne.0.0d0) then
                              if(jr+1.gt.nelmx) then
                                 ierr=1
                                 return
                              endif
                              r(jr)=b(1,1)
                              indr(l)=indr(l)+1
                              indr(mr+jr)=kr
                              jr=jr+1
                           endif
                           kr=kr+1
                        endif
                        if(ja1.le.nal.and.inda(ma+ja+ja1).eq.k) ja1=ja1
     $                       +1
                     endif
                  endif
 42            continue
            endif
            ja=ja+nal
 45      continue
         nelr=jr-1
      endif
      end


