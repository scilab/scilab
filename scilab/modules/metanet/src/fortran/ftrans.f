      subroutine ftrans(cw,m,mq,n,nq,p1,s,sdim,som,suc)
      implicit integer (a-z)
      dimension som(*),suc(m)
      dimension p1(*),s(sdim),cw(n)
      do 10 k=1,nq+1
         p1(k)=1
 10   continue
      do 20 k=1,nq
         cw(k)=0
 20   continue
      ll=1
      do 220 k=1,nq
         d=-1
         if(som(k).eq.som(k+1)) goto 210
         do 120 l=som(k),som(k+1)-1
            i=suc(l)
            if(cw(i).ne.0) goto 120
            cw(i)=d
            d=i
            if(p1(i).eq.p1(i+1)) goto 120
            do 110 l1=p1(i),p1(i+1)-1
               j=s(l1)
               if(cw(j).ne.0) goto 110
               cw(j)=d
               d=j
 110        continue
 120     continue
         ll=p1(k)
         if(d.eq.-1) goto 210
         j=d
 200     s(ll)=j
         ll=ll+1
         j1=cw(j)
         cw(j)=0
         j=j1
         if(j.eq.-1) goto 210
         goto 200
 210     p1(k+1)=ll
 220  continue
      end
