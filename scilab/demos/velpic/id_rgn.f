      subroutine idrgn(indexlist,linelist,seed,ind,nol,nolt,
     +                  ic,icount,noe,newlist,bav,lln,ncount)
      
c Macro which determines all the indices of a matrix of dimension
c (nz X nx) which are in the foobar defined by the seed and the 
c linelist.  The elements which are in the same foobar as the seed
c are those which are on the same side of all the lines in the
c linelist.
c  indexlist :(2xN) vector containing all the indices to be searched
c  linelist  :list of lines
c  seed      :pair of indices of the matrix identifying the foobar
c  ind       :all indices of the matrix associated to the foobar
c            :defined by seed
c 
c  author: C. Bunks     date: 12-NOV-90

      integer   ind,indexlist,newlist,noe,ix
      integer   nol,nolt,flag,ncount,icount
      real      seed,linelist,lln,bav,p1
      dimension linelist(2,nolt),noe(ic),bav(3,nolt),seed(1,2)
      dimension indexlist(2,ic),newlist(2,ic),p1(1,2)
      dimension ind(2,ic),lln(2,nolt)

c if the line segment defined by the seed and another pair of indices
c intersects any line in the linelist then that pair of indices is
c not in the foobar

      ncount=0
      icount=0
      do 10 ii=1,ic 
c     !cycle through all indices
         p1(1,1)=float(indexlist(1,ii))
         p1(1,2)=float(indexlist(2,ii))
         ln=1
         flag=0               
c     !while flag=0 cycle thru lines
         do while(flag.eq.0)  
c     !cycle thru edges till xsection occurs
            if(ln.le.nol)then
               noe1=noe(ln)
               noe2=noe(ln+1)-1
               do 20 ll=noe1,noe2
                  lln(1,ll-noe1+1)=linelist(1,ll)
                  lln(2,ll-noe1+1)=linelist(2,ll)
 20            continue
               call testpt(p1,seed,noe2-noe1+1,lln,flag,ix,bav)
               if(flag.eq.1)then 
c     !intersection occurred
                  ncount=ncount+1
                  newlist(1,ncount)=int(p1(1,1))
                  newlist(2,ncount)=int(p1(1,2))
               endif
            else        
c     !no intersection occurred for index pair
               icount=icount+1
               ind(1,icount)=int(p1(1,1))
               ind(2,icount)=int(p1(1,2)) 
               flag=1
            endif
            ln=ln+1
         end do
 10   continue

      return
      end
