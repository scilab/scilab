c--------------------------------------------------------------------------
c     max clique algorithm based on quadratic zero-one programming
c     This program is pardalos4 from Panos M. Pardalos & Gregory P. Rodgers
c     described in "A branch and bound algorithm for the maximum clique 
c     problem", Operations Research, Vol. 19, No. 5 (1992), pp. 363-375.
c--------------------------------------------------------------------------
      subroutine clique1(n,m,m2,np1,nwk,kat,hat,wcl,wk,x)
      integer hat,kat,wk,wcl
      logical x                                                         
      dimension hat(m2),kat(np1),x(n),wk(nwk),wcl(n)
      nprocs=1
      do 1,i=1,nwk
         wcl(i)=0
 1    continue
c     heuristic phase using the greedy method                           
      call greedy(n,hat,kat,maxc,x,wk,wk(n+1),wk((2*n)+1),              
     >            wk((3*n)+1) )                                         
c     branch and bound phase                                            
      ihsol= maxc                                                       
      if (nprocs.ne.0)                                                  
     >call bbnd(n,hat,kat,x,maxc,wk,nwk,nsubp,nsubpg,nprocs,nchg)     
c heuristic size  = ihsol ;  maximum size    = maxc                       
c number of min chages   = nchg ; number of subproblems  = nsubp       
c minimizer subproblem   = nsubpg                     
      iptr = 0                                                          
      do 10 i=1,n
         wcl(i)=0
         if (x(i)) then                                                 
            iptr = iptr+1                                               
            wcl(iptr) = i                                                
         endif                                                          
10    continue                                                          
      return
      end                                                               
c= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
      subroutine greedy(n,hat,kat,num1s,x,degfre,free,p,degfx1)         
      integer    n,hat(*),kat(*),num1s,degfre(n),p(n),degfx1(n)         
      logical    x(n),free(n),value                                     
c                                                                       
      imax = 1                                                          
      do 10 i=1,n                                                       
         p(i) = i                                                       
         degfre(i) = kat(i+1)-kat(i)                                    
         if (degfre(i).gt.degfre(imax)) imax= i                         
         degfx1(i) = 0                                                  
         free(i) = .true.                                               
10    continue                                                          
c     put the highest connected node into the greedy            
      num1s      = 0                                                    
      x(imax)    = .true.                                               
      p(1)       = imax                                                 
      p(imax)    = 1                                                    
      free(imax) = .false.                                              
c                                                                       
      lev  = 1                                                          
20    continue                                                          
         levp1 = lev+1                                                  
c    --- update the vectors degfre, degfx1, and num1s                   
         lp = p(lev)                                                    
c        lp was the last variable that was fixed                        
         if (x(lp)) then                                                
            num1s = num1s+1                                             
            do 70 k=kat(lp),kat(lp+1)-1                                 
               j = hat(k)                                               
               if (free(j)) then                                        
                  degfre(j)  = degfre(j)  - 1                           
                  degfx1(j)  = degfx1(j)  + 1                           
               endif                                                    
70          continue                                                    
         else                                                           
c           last vertex was thrown out so dont bother with degfx1       
            do 90 k=kat(lp),kat(lp+1)-1                                 
               degfre(hat(k)) = degfre(hat(k)) - 1                      
90          continue                                                    
         endif                                                          
         if (lev.eq.n) goto 1000                                        
c                                                                       
c    --- determine if any variable can be fixed                         
         do 130 i=levp1,n                                               
            ip = p(i)                                                   
c                                                                       
c           two tests to see if node ip can be fixed                    
c                                                                       
c       -1- is it disconnected to a node currently fixed to one?        
            if (degfx1(ip).lt.num1s) then                               
               value = .false.                                          
               ivar = i                                                 
               goto 150                                                 
            endif                                                       
c                                                                       
c       -2- after the 1st test we know ip is connected to               
c           all nodes fixed to one.  so now we can test if it           
c           is connected to all remaining free nodes.  if it is         
c           then fix ip to 1                                            
c mg            if (degfre(ip).ge.n-levp) then                              
            if (degfre(ip).ge.n-levp1) then                              
               value = .true.                                           
               ivar = i                                                 
               goto 150                                                 
            endif                                                       
c                                                                       
130      continue                                                       
c                                                                       
c    --- none could be forced so find variable with highest degree      
         ivar   = levp1                                                 
         do 140 i=levp1+1,n                                             
            if (degfre(p(i)).gt.degfre(p(ivar))) ivar=i                 
               ivar = i                                                 
140      continue                                                       
c                                                                       
c    --- set to one and stack the subproblem with x(ip) = 0             
         value = .true.                                                 
c                                                                       
c    --- ivar is new fixed variable, increase lev & swap in p           
150      lev      = levp1                                               
         it       = p(ivar)                                             
         p(ivar)  = p(lev)                                              
         p(lev)   = it                                                  
         x(it)    = value                                               
         free(it) = .false.                                             
         goto 20                                                        
1000  continue                                                          
      return                                                            
c     end greedy                                                        
      end                                                               
c**************************************************************************
      subroutine copyl(n,lfrom,lto)                                     
      logical lfrom(n),lto(n)                                           
      integer n                                                         
      do 10 i=1,n                                                       
         lto(i) = lfrom(i)                                              
10    continue                                                          
      return                                                            
c     end of copyl                                                      
      end                                                               
c**************************************************************************
      subroutine bbnd(n, hat,kat,xmin,maxc,iwk,nwk,nsubp,nsubpg,nprocs,
     >   nchg)                                                          
      integer n,nwk,hat(*),kat(*),maxc,iwk(nwk),nsubp,nsubpg,nprocs     
      logical xmin(n)                                                   
c     local variables                                                   
      integer csubp,cycles                                              
      integer nbusy,nfree,busy(6),free(6),strwk(6),split(6)             
      integer ldx,ldf,lstk,lp,lx,lfr,lxm,ltop,llev,lmin,lnsb,lnsg,lnwk, 
     >        lend                                                      
      integer nsplit,ipt,iptz,ipty,imin,imax,minspl,wsize,y,z,i,minsb   
      logical prempt,newmax                                             
c   --- initialize statistics                                           
      nchg   = 0                                                        
      nsubp  = 0                                                        
      nsubpg = 0                                                        
      csubp  = 0                                                        
      cycles = 0                                                        
c   --- initilize the mininum # of subproblems to solve in each cycle   
      minsb = 800                                                       
c   --- divide workspace among nprocs processors                        
      wsize = nwk/nprocs                                                
      strwk(1) = 0                                                      
      do 10 i=2,nprocs                                                  
         strwk(i) = strwk(i-1)+wsize                                    
10    continue                                                          
c   --- set up storage pointers                                         
      ldx =  1                                                          
      ldf =  ldx + (   n    )                                           
      lstk=  ldf + (   n    )                                           
      lp  = lstk + ( 3*(n+1))                                           
      lx  =   lp + (   n    )                                           
      lfr =   lx + (   n    )                                           
      lxm =  lfr + (   n    )                                           
      ltop=  lxm + (   n    )                                           
      llev= ltop + (   1    )                                           
      lmin= llev + (   1    )                                           
      lnsb= lmin + (   1    )                                           
      lnsg= lnsb + (   1    )                                           
      lnwk= lnsg + (   1    )                                           
      lend= lnwk + (   1    )                                           
      if (lend.ge.wsize) then                                           
c ww lend*nprocs,nwk                                   
         return                                                         
      endif                                                             
c   --- initialize subproblem 1                                         
      do 20 i=1,n                                                       
         iwk(lp+i-1)= i                                                 
20    continue                                                          
      iwk(ltop)=  1                                                     
      iwk(lstk)= -1                                                     
      iwk(llev)=  0                                                     
      iwk(lnwk)=  0                                                     
      iwk(lmin)= maxc                                                   
c   --- set up busy and free lists                                      
      nbusy     = 1                                                     
      busy(1)   = 1                                                     
      nfree     = nprocs-1                                              
      do 30 i=1,nfree                                                   
         free(i) = i+1                                                  
30    continue                                                          
c-----------------------------------------------------------------------
c     split subproblems till all procs are marked busy                  
c-----------------------------------------------------------------------
c     while (nbusy.lt.nprocs.and.nbusy.gt.0)                            
1000  if (nbusy.lt.nprocs.and.nbusy.gt.0) then                          
c        find all the split points in the nbusy subproblems             
         nsplit=0                                                       
         do 40 i=1,nbusy                                                
            ipt = strwk(busy(i))                                        
c           is the top of the stack greater than 1?                     
            if (iwk(ipt+ltop).gt.1) then                                
c              get the level from the top element of the stack          
               split(i) = iwk(ipt + lstk + 3)                           
               nsplit   = nsplit  + 1                                   
            else                                                        
               split(i) = 0                                             
            endif                                                       
40       continue                                                       
c                                                                       
         if (nsplit.eq.0) then                                          
c           if there are no problems to split then force splitting      
c           of the first process in the busy list by (re)starting       
c           the branch and bound algorithm                              
50          ipt = strwk(busy(1))                                        
            call bbspl1(n,hat,kat,wsize,iwk(ipt+1),nchg)                
            nsubp = nsubp + iwk(ipt+lnsb)                               
            csubp = csubp + iwk(ipt+lnsb)                               
c           did bbspl1 find a new minimum ?                             
            if (iwk(ipt+lmin).gt.maxc) then                             
               maxc = iwk(ipt+lmin)                                     
               call copyl(n,iwk(ipt+lxm),xmin)                          
               do 60 y=1,nprocs                                         
                  iwk(strwk(y)+lmin) = maxc                             
60             continue                                                 
               nsubpg = csubp + iwk(ipt+lnsg)                           
            endif                                                       
c           did bbspl1 solve the problem ?                              
            if (iwk(ipt+llev).eq.-1) then                               
c              attempted split solved the problem                       
c              take y off busy list and add it to the free list         
               nfree = nfree + 1                                        
               nbusy = nbusy - 1                                        
               free(nfree) = busy(1)                                    
               do 70  i=1,nbusy                                         
                  busy(i) = busy(i+1)                                   
70             continue                                                 
c              go back and try to split another subproblem              
               if (nbusy.gt.0) goto 50                                  
            else                                                        
c              no, then a new split point was found                     
               split(1) = iwk(ipt + lstk + 3)                           
               nsplit = 1                                               
            endif                                                       
         endif                                                          
c                                                                       
         if (nbusy.eq.0) goto 3000                                      
c        nsplit problems are now splitable                              
c        find lowest splitpoint (highest pt in tree) and split          
         imin   = 0                                                     
         minspl = n+1                                                   
c        examine splitpoints of all busy subproblems                    
         do 80 i=1,nbusy                                                
            if (split(i).ne.0.and.split(i).lt.minspl) then              
               imin = i                                                 
               minspl = split(imin)                                     
            endif                                                       
80       continue                                                       
c      --- now split the process denoted by busy(imin)                  
         y = busy(imin)                                                 
         z = free(nfree)                                                
         ipty = strwk(y)                                                
         iptz = strwk(z)                                                
c        take z off free list and add it to the busy list               
         nfree = nfree - 1                                              
         nbusy = nbusy + 1                                              
         busy(nbusy) = z                                                
c      --- initialize subproblem z from subproblem y                    
c        permutation vector p                                           
         do 90 i=1,n                                                    
            iwk((iptz+lp-1)+i)= iwk((ipty+lp-1)+i)                      
90       continue                                                       
c        initialize top of stack and stack as empty                     
         iwk(iptz+ltop) =  1                                            
         iwk(iptz+lstk) = -1                                            
         iwk(iptz+lnwk) =  0                                            
c        level of execution                                             
         iwk(iptz+llev) =  split(imin)                                  
c        incumbent                                                      
         iwk(iptz+lmin)= maxc                                           
c        zero-one variables x                                           
         call copyl(n,iwk(ipty+lx),iwk(iptz+lx))                        
c        change one x variable                                          
         call changl(iwk(iptz+lx),iwk(iptz+lp),split(imin))             
c        -->  subroutine changl(x,p,lev)                                
c        -->  x(p(lev)) = .not.x(p(lev))                                
c        remove subproblem from y's stack                               
         call pllstk(iwk(ipty+ltop),iwk(ipty+lstk))                     
         iwk(ipty+lnwk) =  0                                            
c        -->  subroutine pllstk(topstk,stack)                           
c        -->  integer topstk,stack(3,*)                                 
      goto 1000                                                         
      endif                                                             
c     endwhile(nbusy.lt.nprocs.and.nbusy.gt.0)                          
c-----------------------------------------------------------------------
c     solve subproblems in parallel                                     
c-----------------------------------------------------------------------
c     while (nbusy.eq.nprocs)                                           
2000  if (nbusy.eq.nprocs) then                                         
c        execute nprocs in paralel   this is called one cycle           
c        each proc will search its subproblem for maxvt vertices        
         cycles = cycles + 1                                            
         prempt = .false.                                               
         do 100 y=1,nprocs-1                                            
            ipt = strwk(y)+1                                            
c           solve subproblem y                                          
            call bbp(n,hat,kat,minsb,wsize,iwk(ipt),prempt,nchg)        
cccc        call dsptch('bbp',n,hat,kat,minsb,wsize,iwk(ipt),prempt)    
100      continue                                                       
         ipt = strwk(nprocs)+1                                          
         call bbp(n,hat,kat,minsb,wsize,iwk(ipt),prempt,nchg)           
c                                                                       
c        synchronize subproblems (wait for all to finish)               
cccc     if (nprocs.gt.1) call syncro                                   
c-----------------------------------------------------------------------
c        analysis and synchronization phase                             
c-----------------------------------------------------------------------
         nbusy  = 0                                                     
         nfree  = 0                                                     
         newmax = .false.                                               
         imax   = 0                                                     
         do 110 y=1,nprocs                                              
            ipt = strwk(y)                                              
c           iwk(ipt+lnsb)is the # of subproblems solved for this problem
            nsubp = nsubp + iwk(ipt+lnsb)                               
c           find which problem did the most to calculate critical path  
            if (iwk(ipt+lnsb).gt.imax) imax=iwk(ipt+lnsb)               
            if (iwk(ipt+lmin).gt.maxc) then                             
               nsubpg = csubp + iwk(ipt+lnsg)                           
               newmax = .true.                                          
               maxc   = iwk(ipt+lmin)                                   
               call copyl(n,iwk(ipt+lxm),xmin)                          
            endif                                                       
            if (iwk(ipt+llev).ne.-1) then                               
               nbusy = nbusy + 1                                        
               busy(nbusy) = y                                          
            else                                                        
               nfree = nfree + 1                                        
               free(nfree) = y                                          
            endif                                                       
110      continue                                                       
c        keep track of critical path count of subproblems               
         csubp = csubp + imax                                           
c        update new bound if one was found                              
         if (newmax) then                                               
            do 120 y=1,nprocs                                           
               iwk(strwk(y)+lmin) = maxc                                
120         continue                                                    
         endif                                                          
      goto 2000                                                         
      endif                                                             
c     endwhile(nbusy.eq.nprocs)                                         
c                                                                       
c     repeat until all subproblems have been solved (nbusy.eq.0)        
      if (nbusy.ne.0) goto 1000                                         
c--------------------------------------------------------------------   
c     end of cyeturn to q01                                     
c--------------------------------------------------------------------   
3000  continue                                                          
c nb of cycles for solving the problem = cycles
c critical path subproblem count = csubp              
200   format(' not enough workspace:',i8,' needed',i8,' specified')     
      return                                                            
c                                                                       
c     end of                                                    
      end                                                               
c**************************************************************************
      subroutine changl(x,p,lev)                                        
c     change the zero-one value in x(p(lev))                            
      integer p(*),lev                                                  
      logical x(*)                                                      
      x(p(lev)) = .not.x(p(lev))                                        
      return                                                            
      end                                                               
c**************************************************************************
      subroutine pllstk(topstk,stack)                                   
c     pull the top most element off of the stack                        
      integer topstk,stack(3,*)                                         
      topstk = topstk-1                                                 
c     -1 is at top of stack.  do not touch it                           
      do 10 i=2,topstk                                                  
         stack(1,i) = stack(1,i+1)                                      
         stack(2,i) = stack(2,i+1)                                      
c        cause bounds to be recalculated (save synchronization time)    
         stack(3,i) = -1                                                
10    continue                                                          
      return                                                            
      end                                                               
c**************************************************************************
      subroutine bbp(n,hat,kat,minsb,nwk,iwk,prempt,nchg)               
      integer n,hat(*),kat(*),minsb,nwk,iwk(nwk)                        
      logical prempt                                                    
c     local variables                                                   
      integer maxwk                                                     
c   --- set up storage pointers                                         
      ldx =  1                                                          
      ldf =  ldx + (   n    )                                           
      lstk=  ldf + (   n    )                                           
      lp  = lstk + ( 3*(n+1))                                           
      lx  =   lp + (   n    )                                           
      lfr =   lx + (   n    )                                           
      lxm =  lfr + (   n    )                                           
      ltop=  lxm + (   n    )                                           
      llev= ltop + (   1    )                                           
      lmin= llev + (   1    )                                           
      lnsb= lmin + (   1    )                                           
      lnsg= lnsb + (   1    )                                           
      lnwk= lnsg + (   1    )                                           
      lend= lnwk + (   1    )                                           
      maxwk = nwk-lend                                                  
      call bb(n,hat,kat,minsb,maxwk,                                    
     >    iwk(ldx),iwk(ldf),iwk(lstk),iwk(lp),iwk(lx),                  
     >    iwk(lfr),iwk(lxm),                                            
     >    iwk(ltop),iwk(llev),iwk(lmin),iwk(lnsb),iwk(lnsg),iwk(lnwk),  
     >    iwk(lend),prempt,nchg)                                        
      return                                                            
c     end of bbp                                                        
      end                                                               
cprocess vector(report) directive('*vdir:')                             
c**************************************************************************
      subroutine bb(n,hat,kat,maxs,maxwk,degfx1,degfre,stack,p,x,             
     > free,xmin,topstk,lev,maxc,nsubp,nsubpg,endwk,iwk,prempt,nchg) 
c  parameters                                                           
      integer n,maxs,maxwk,topstk,lev,maxc,nsubp,nsubpg,endwk           
      integer hat(*),kat(*),degfx1(n),degfre(n),stack(3,n),p(n),        
     >        iwk(maxwk)                                                
      logical x(n),xmin(n),free(n),prempt                               
c  local variables                                                      
      integer g,levp1,ip,lp,i,j,it,dgneed,num1s,nmlevp                  
      integer nfree,newewk,itemp,iptr,ivar                              
      logical uselst,value                                              
c*vdir:  prefer scalar on                                               
      nsubp  = 0                                                        
      nsubpg = 0                                                        
      uselst = .false.                                                  
c     calculate the upper bound on maxc, g                              
      g = n                                                             
      do 10 i=1,lev                                                     
         ip = p(i)                                                      
         free(ip) = .false.                                             
         if(.not.x(ip)) g = g-1                                         
10    continue                                                          
      do 20 i=lev+1,n                                                   
         free(p(i)) = .true.                                            
20    continue                                                          
                                                                        
30    if (lev.eq.-1.or.(nsubp.ge.maxs.and.prempt)) goto 1000            
c                                                                       
         levp1  = lev    + 1                                            
c                                                                       
         if (g.le.maxc.or.lev.eq.n) then                                
c                                                                       
c           terminal node found                                         
c                                                                       
c         --- check to see if it was a leaf                             
            if (g.gt.maxc) then                                         
               maxc = g                                                 
               do 40 j=1,n                                              
                  xmin(j) = x(j)                                        
40             continue                                                 
               nsubpg = nsubp+1                                         
               nchg = nchg+1                                            
            endif                                                       
c                                                                       
c         --- get new subproblem from the stack                         
            lev    = stack(1,topstk)                                    
            g      = stack(2,topstk)                                    
            itemp  = stack(3,topstk)                                    
            topstk = topstk - 1                                         
            if (lev.ne.-1) then                                         
               x(p(lev)) = (.not.x(p(lev)))                             
               do 50 i=lev+1,n                                          
                  free(p(i)) = .true.                                   
50             continue                                                 
c         ---  get degree counts from storage if there was enough room  
               if (itemp.ne.-1) then                                    
                  endwk = itemp                                         
                  itemp = endwk + (n-lev)                               
                  iptr = 0                                              
                  do 55 i=1,n                                           
                     if (free(i)) then                                  
                        iptr = iptr+1                                   
c                       the free list in p is now in order              
                        p(lev+iptr) = i                                 
                        degfre(i) = iwk(endwk+iptr)                     
                        degfx1(i) = iwk(itemp+iptr)                     
                     endif                                              
55                continue                                              
                  uselst = .true.                                       
               else                                                     
c                 there wasn't enough room, counts must be recalculated 
                  uselst = .false.                                      
               endif                                                    
            endif                                                       
            nsubp = nsubp + 1                                           
c                                                                       
         else                                                           
c                                                                       
c           proceed depth first                                         
c                                                                       
c         --- calculate the degree of free nodes to free nodes          
            if (uselst) then                                            
               lp = p(lev)                                              
c              lp was the last variable that was fixed                  
               if (x(lp)) then                                          
c*vdir: prefer vector                                                   
c*vdir: ignore recrdeps(degfre,degfx1)                                  
                  do 70 k=kat(lp),kat(lp+1)-1                           
                     degfre(hat(k))  = degfre(hat(k))  - 1              
                     degfx1(hat(k))  = degfx1(hat(k))  + 1              
70                continue                                              
               else                                                     
c                 last vertex was thrown out so dont bother with degfx1 
c*vdir: prefer vector                                                   
c*vdir: ignore recrdeps(degfre)                                         
                  do 90 k=kat(lp),kat(lp+1)-1                           
                     degfre(hat(k)) = degfre(hat(k)) - 1                
90                continue                                              
               endif                                                    
            else                                                        
c              calculate free node connectivity from scratch            
               do 120 i = levp1,n                                       
                  ip = p(i)                                             
                  degfre(ip) = 0                                        
                  degfx1(ip) = 0                                        
                  do 110 k=kat(ip),kat(ip+1)-1                          
                     j = hat(k)                                         
                     if (free(j)) then                                  
                        degfre(ip) = degfre(ip) + 1                     
                     else                                               
                        if (x(j)) degfx1(ip) = degfx1(ip) + 1           
                     endif                                              
110               continue                                              
120            continue                                                 
            endif                                                       
c                                                                       
c         --- determine if any variable can be fixed                    
            num1s = lev + g - n                                         
            dgneed = maxc - num1s -1                                    
            nmlevp = n - levp1                                          
            do 130 i=levp1,n                                            
               ip = p(i)                                                
c                                                                       
c              three tests to see if node ip can be fixed               
c                                                                       
c            -1- is it disconnected to a node currently fixed to one?   
               if (degfx1(ip).lt.num1s) then                            
                  value = .false.                                       
                  ivar = i                                              
                  g = g - 1                                             
                  goto 150                                              
               endif                                                    
c                                                                       
c            -2- after the 1st test we know ip is connected to          
c                all nodes fixed to one.  so now we can test if it      
c                is connected to all remaining free nodes.  if it is    
c                then fix ip to 1                                       
               if (degfre(ip).ge.nmlevp) then                           
                  value = .true.                                        
                  ivar = i                                              
                  goto 150                                              
               endif                                                    
c                                                                       
c            -3- is its total degree + 1 smaller than a known   
c              each free node needs at least degree dgneed to make      
c              a f size maxc. if not we can throw node away.    
               if (degfre(ip).le.dgneed) then                           
c                 fix to 0                                              
                  value = .false.                                       
                  g     = g - 1                                         
                  ivar  = i                                             
                  goto 150                                              
               endif                                                    
c                                                                       
130         continue                                                    
c                                                                       
c           note: at this point we know that all free nodes are         
c                 connected to all nodes fixed to one and that they     
c                 are also disconnected to some free nodes.  so         
c                 we may choose variable to branch on based on how      
c                 much they are connected or disconnected to other      
c                 free nodes.                                           
c                                                                       
c         --- none could be fixed so find variable i to branch on.      
ccc           rule1: choose node with highest free connectivity         
c             rule2: choose node with lowest free connectivity          
c             rule 2 is faster in verifying optimality and is used here 
c             rule 1 is the greedy method.                              
            ivar   = levp1                                              
            do 140 i=levp1+1,n                                          
               if (degfre(p(i)).lt.degfre(p(ivar))) ivar= i             
140         continue                                                    
c                                                                       
c         --- set to one and stack the subproblem with x(ip) = 0        
            value = .true.                                              
            if (g-1.gt.maxc) then                                       
c              only stack subproblems that are not suboptimal           
               topstk = topstk + 1                                      
               stack(1,topstk) = levp1                                  
               stack(2,topstk) = g-1                                    
               nfree = n-levp1                                          
               newewk = endwk + (nfree+nfree)                           
               if (newewk.le.maxwk) then                                
                  stack(3,topstk) = endwk                               
                  free(p(ivar)) = .false.                               
                  itemp = endwk + nfree                                 
                  iptr = 0                                              
                  do 145 i=1,n                                          
c                    note: the following requires that the free list in 
c                          the permutation p be in order                
                     if (free(i)) then                                  
                        iptr = iptr+1                                   
                        iwk(endwk+iptr) = degfre(i)                     
                        iwk(itemp+iptr) = degfx1(i)                     
                     endif                                              
145               continue                                              
                  endwk = newewk                                        
               else                                                     
                  stack(3,topstk) = -1                                  
               endif                                                    
            endif                                                       
c                                                                       
c         --- ivar is new fixed variable, increase lev & swap in p      
150         lev      = levp1                                            
            it       = p(ivar)                                          
            p(ivar)  = p(lev)                                           
            p(lev)   = it                                               
            x(it)    = value                                            
            free(it) = .false.                                          
            uselst   = .true.                                           
c                                                                       
         endif                                                          
         goto 30                                                        
c                                                                       
1000  prempt = .true.                                                   
      return                                                            
c                                                                       
c     end of bb                                                         
      end                                                               
c**************************************************************************
      subroutine bbspl1(n,hat,kat,nwk,iwk,nchg)                         
      integer n,hat(*),kat(*),nwk,iwk(nwk)                              
c   --- set up storage pointers                                         
      ldx =  1                                                          
      ldf =  ldx + (   n    )                                           
      lstk=  ldf + (   n    )                                           
      lp  = lstk + ( 3*(n+1))                                           
      lx  =   lp + (   n    )                                           
      lfr =   lx + (   n    )                                           
      lxm =  lfr + (   n    )                                           
      ltop=  lxm + (   n    )                                           
      llev= ltop + (   1    )                                           
      lmin= llev + (   1    )                                           
      lnsb= lmin + (   1    )                                           
      lnsg= lnsb + (   1    )                                           
      lnwk= lnsg + (   1    )                                           
      lend= lnwk + (   1    )                                           
      maxwk = nwk-lend                                                  
c     bbspl2 will stop after the first subproblem is stacked            
      call bbspl2(n,hat,kat,maxwk,                                      
     >    iwk(ldx),iwk(ldf),iwk(lstk),iwk(lp),iwk(lx),                  
     >    iwk(lfr),iwk(lxm),                                            
     >    iwk(ltop),iwk(llev),iwk(lmin),iwk(lnsb),iwk(lnsg),iwk(lnwk),  
     >    iwk(lend),nchg)                                               
      return                                                            
c                                                                       
c     end of bbspl1                                                     
      end                                                               
c**************************************************************************
      subroutine bbspl2(n,hat,kat,maxwk,                                
     >              degfx1,degfre,stack,p,x,free,xmin,                  
     >              topstk,lev,maxc,nsubp,nsubpg,endwk,iwk,nchg)        
c  parameters                                                           
      integer n,maxwk,topstk,lev,maxc,nsubp,nsubpg,endwk                
      integer hat(*),kat(*),degfx1(n),degfre(n),stack(3,n),p(n),        
     >        iwk(maxwk)                                                
      logical x(n),xmin(n),free(n)                                      
c  local variables                                                      
      integer g,levp1,ip,lp,i,j,it,dgneed,num1s,nmlevp                  
      integer nfree,newewk,itemp,iptr,ivar                              
      logical uselst,value                                              
c*vdir: prefer scalar on                                                
      nsubp  = 0                                                        
      nsubpg = 0                                                        
      uselst = .false.                                                  
c     calculate the upper bound on maxc, g                              
      g = n                                                             
      do 10 i=1,lev                                                     
         ip = p(i)                                                      
         free(ip) = .false.                                             
         if(.not.x(ip)) g = g-1                                         
10    continue                                                          
      do 20 i=lev+1,n                                                   
         free(p(i)) = .true.                                            
20    continue                                                          
                                                                        
30    if (lev.eq.-1.or.topstk.gt.1) goto 1000                           
c                                                                       
         levp1  = lev    + 1                                            
c                                                                       
         if (g.le.maxc.or.lev.eq.n) then                                
c                                                                       
c           terminal node found                                         
c                                                                       
c         --- check to see if it was a leaf                             
            if (g.gt.maxc) then                                         
               maxc = g                                                 
               do 40 j=1,n                                              
                  xmin(j) = x(j)                                        
40             continue                                                 
               nsubpg = nsubp+1                                         
               nchg = nchg + 1                                          
            endif                                                       
c                                                                       
c         --- get new subproblem from the stack                         
            lev    = stack(1,topstk)                                    
            g      = stack(2,topstk)                                    
            itemp  = stack(3,topstk)                                    
            topstk = topstk - 1                                         
            if (lev.ne.-1) then                                         
               x(p(lev)) = (.not.x(p(lev)))                             
               do 50 i=lev+1,n                                          
                  free(p(i)) = .true.                                   
50             continue                                                 
c         ---  get degree counts from storage if there was enough room  
               if (itemp.ne.-1) then                                    
                  endwk = itemp                                         
                  itemp = endwk + (n-lev)                               
                  iptr = 0                                              
                  do 55 i=1,n                                           
                     if (free(i)) then                                  
                        iptr = iptr+1                                   
c                       the free list in p is now in order              
                        p(lev+iptr) = i                                 
                        degfre(i) = iwk(endwk+iptr)                     
                        degfx1(i) = iwk(itemp+iptr)                     
                     endif                                              
55                continue                                              
                  uselst = .true.                                       
               else                                                     
c                 there wasn't enough room, counts must be recalculated 
                  uselst = .false.                                      
               endif                                                    
            endif                                                       
            nsubp = nsubp + 1                                           
c                                                                       
         else                                                           
c                                                                       
c           proceed depth first                                         
c                                                                       
c         --- calculate the degree of free nodes to free nodes          
            if (uselst) then                                            
               lp = p(lev)                                              
c              lp was the last variable that was fixed                  
               if (x(lp)) then                                          
c*vdir: prefer vector                                                   
c*vdir: ignore recrdeps(degfre,degfx1)                                  
                  do 70 k=kat(lp),kat(lp+1)-1                           
                     degfre(hat(k))  = degfre(hat(k))  - 1              
                     degfx1(hat(k))  = degfx1(hat(k))  + 1              
70                continue                                              
               else                                                     
c                 last vertex was thrown out so dont bother with degfx1 
c*vdir: prefer vector                                                   
c*vdir: ignore recrdeps(degfre)                                         
                  do 90 k=kat(lp),kat(lp+1)-1                           
                     degfre(hat(k)) = degfre(hat(k)) - 1                
90                continue                                              
               endif                                                    
            else                                                        
c              calculate free node connectivity from scratch            
               do 120 i = levp1,n                                       
                  ip = p(i)                                             
                  degfre(ip) = 0                                        
                  degfx1(ip) = 0                                        
                  do 110 k=kat(ip),kat(ip+1)-1                          
                     j = hat(k)                                         
                     if (free(j)) then                                  
                        degfre(ip) = degfre(ip) + 1                     
                     else                                               
                        if (x(j)) degfx1(ip) = degfx1(ip) + 1           
                     endif                                              
110               continue                                              
120            continue                                                 
            endif                                                       
c                                                                       
c         --- determine if any variable can be fixed                    
            num1s = lev + g - n                                         
            dgneed = maxc - num1s -1                                    
            nmlevp = n - levp1                                          
            do 130 i=levp1,n                                            
               ip = p(i)                                                
c                                                                       
c              three tests to see if node ip can be fixed               
c                                                                       
c            -1- is it disconnected to a node currently fixed to one?   
               if (degfx1(ip).lt.num1s) then                            
                  value = .false.                                       
                  ivar = i                                              
                  g = g - 1                                             
                  goto 150                                              
               endif                                                    
c                                                                       
c            -2- after the 1st test we know ip is connected to          
c                all nodes fixed to one.  so now we can test if it      
c                is connected to all remaining free nodes.  if it is    
c                then fix ip to 1                                       
               if (degfre(ip).ge.nmlevp) then                           
                  value = .true.                                        
                  ivar = i                                              
                  goto 150                                              
               endif                                                    
c                                                                       
c            -3- is its total degree + 1 smaller than a known   
c              each free node needs at least degree dgneed to make      
c              a f size maxc. if not we can throw node away.    
               if (degfre(ip).le.dgneed) then                           
c                 fix to 0                                              
                  value = .false.                                       
                  g     = g - 1                                         
                  ivar  = i                                             
                  goto 150                                              
               endif                                                    
c                                                                       
130         continue                                                    
c note: at this point we know that all free nodes are connected to all
c nodes connected to all nodes fixed to one and that they are also    
c disconnected to some free nodes.  so we may choose variable to branch  on     
c based on how much they are connected or disconnected to other free nodes.
c                                                                       
c         --- none could be fixed so find variable i to branch on.      
ccc           rule1: choose node with highest free connectivity         
c             rule2: choose node with lowest free connectivity          
c             rule 2 is faster in verifying optimality and is used here 
c             rule 1 is the greedy method.                              
            ivar   = levp1                                              
            do 140 i=levp1+1,n                                          
               if (degfre(p(i)).lt.degfre(p(ivar))) ivar=i              
140         continue                                                    
c         --- set to one and stack the subproblem with x(ip) = 0        
            value = .true.                                              
            if (g-1.gt.maxc) then                                       
c              only stack subproblems that are not suboptimal           
               topstk = topstk + 1                                      
               stack(1,topstk) = levp1                                  
               stack(2,topstk) = g-1                                    
               nfree = n-levp1                                          
               newewk = endwk + (nfree+nfree)                           
               if (newewk.le.maxwk) then                                
                  stack(3,topstk) = endwk                               
                  free(p(ivar)) = .false.                               
                  itemp = endwk + nfree                                 
                  iptr = 0                                              
                  do 145 i=1,n                                          
c                    note: the following requires that the free list in 
c                          the permutation p be in order                
                     if (free(i)) then                                  
                        iptr = iptr+1                                   
                        iwk(endwk+iptr) = degfre(i)                     
                        iwk(itemp+iptr) = degfx1(i)                     
                     endif                                              
145               continue                                              
                  endwk = newewk                                        
               else                                                     
                  stack(3,topstk) = -1                                  
               endif                                                    
            endif                                                       
c         --- ivar is new fixed variable, increase lev & swap in p      
150         lev      = levp1                                            
            it       = p(ivar)                                          
            p(ivar)  = p(lev)                                           
            p(lev)   = it                                               
            x(it)    = value                                            
            free(it) = .false.                                          
            uselst   = .true.                                           
c                                                                       
         endif                                                          
         goto 30                                                        
c                                                                       
1000  return
c     end of bbspl2                                                     
      end                                                               
