c     algorithm 582, collected algorithms from acm.
c     algorithm appeared in acm-trans. math. software, vol.8, no. 2,
c     jun., 1982, p. 190.
c     ==============================================================
      subroutine   gpskca   (n, degree, rstart, connec, optpro, wrklen, 
     1                       permut, work, bandwd, profil, error, space)
      integer   n, rstart(n), wrklen, bandwd, profil, error, space
      integer   degree(n),connec(1),permut(n),work(wrklen),optpro
c     ------------------------------------------------------------------
c input parameters:
c     n      -- the dimension of the matrix
c     degree,rstart,connec -- 
c           describe the structure of the sparsematrix. degree(i) 
c           specifies the number of non-zero  off-diagonal entries in the
c           i-th  row of the sparse matrix.  the column indices of these
c           entries are given in consecutive locations in connec, starting at
c           location  rstart(i).in other words, the indices of the non-zero
c           off-diagonal elements of the  i-th  row are found
c                   in:
c                       connec (rstart(i)),
c                       connec (rstart(i) + 1),
c                                . . .
c                       connec (rstart(i) + degree(i) - 1)
c
c                   dimensions:
c                       rstart is dimension  n  (or longer).
c                       degree is dimension  n  (or longer).
c                       connec is dimension roughly the number of non-
c                              zero entries in the matrix.
c
c     optpro -- 1 if reducing the profile of the matrix
c                      is more important than reducing the bandwidth
c               0 if bandwidth reduction is most important
c
c     wrklen -- if not enough space is given in work the error flag
c                   will be set and further computation stopped.
c
c input and output parameter:
c     permut -- on input, an alternative reordering for the
c                   rows and columns of the matrix.  permut(i) gives
c                   the position in which row and column  i  should
c                   be placed to reduce the bandwidth or the profile.
c                   if the user has no alternative to the natural
c                   ordering implicit in  degree,  rstart  and  connec,
c                   he should initialize  permut  to be the identity
c                   permutation  permut(i) = i .
c
c                   on output,  permut  will contain the permutation
c                   for reordering the rows and columns which reduces
c                   the bandwidth and/or profile.  the result will be
c                   the reordering found by 'gpskca' or the reordering
c                   given by the user in 'permut', whichever does the
c                   job better.
c
c
c output parameters:
c     work   -- a temporary storage vector, of 6n+3  locations.
c               the 1st  n  locations of original row and column indices 
c               locations  n+1, ... , 2n  new positions 
c                   found by gpskca.  
c     bandwd -- the bandwidth of the reordered matrix
c     profil -- the profile of the reordered matrix
c     error  -- will be equal to zero if a new numbering could be
c                   found in the space provided.  otherwise,  error
c                   will be set to a positive error code (see table
c                   given below).  if the reordering algorithm has been
c                   stopped by lack of workspace, the space parameter
c                   will be set to the number of additional locations
c                   required to complete at least the next phase of
c                   the algorithm.
c     space  -- will indicate either how much space the reordering
c                   actually required or how much space will be
c                   required to complete the next phase of the
c                   reordering algorithm.  the possible outcomes are ..
c                      error = 0          space is the minimal value for
c                                         wrklen  required to reorder
c                                         this matrix again.
c                      error <> 0         space is the minimum number
c                      due to lack of     of extra workspace required
c                      workspace          to continue the reordering
c                                         algorithm on this matrix.
c                      error <> 0         space = -1
c                      due to error
c                      in data structures
c     ==================================================================
c     e r r o r    c o d e s
c         error codes have the form  0xy  or  1xy.
c         errors of the form  1xy  result from inadequate workspace.
c         errors of the form  0xy  are internal program checks
c         the last digit (y) is mainly useful for debugging the
c         the reordering algorithm.  the middle digit  (x)  indicates
c         how much of the algorithm has been performed.
c         the table below gives the correspondence between the
c         values of  x   and the structure of the algorithm.
c             x = 0     initial processing
c             x = 1     computing pseudo-diameter  (algorithm i)
c             x = 2     transition between algorithm i and ii
c             x = 3     combining level structures (algorithm ii)
c             x = 4     transition between algorithm ii and iii
c             x = 5     bandwidth numbering (algorithm iiia)
c             x = 6     profile numbering (algorithm iiib)
c             x = 7     final bandwidth/profile computation
      integer     i, inc1, inc2, avail, nxtnum, lowdg, stnode, nleft,
     1            tree1, tree2, depth, empty, stotal, reqd, cspace,
     2            lvllst, lvlptr, active, rvnode, width1, width2, mxdg
      logical     revrs1, oneis1
c
c     << number any degree zero nodes >>
c
c     while << some nodes yet unnumbered >> do
c         << find a pseudo-diameter of the matrix graph >>
c         << convert form of level trees >>
c         << combine level trees into one level structure >>
c         << convert form of level structure >>
c         if optpro=1 then
c             << renumber by king algorithm >>
c         else
c             << renumber by reverse cuthill-mckee algorithm >>
c     ==================================================================
c     ... initialize counters, then number any nodes of degree  0.
c         the list of nodes, by new number, will be built in place at
c         the front of the work area.
      nxtnum = 1
      error = 0
      space = 2*n
c
      mxdg = 0
      do 300 i = 1, n
         CRES=degree(i)
          if (CRES .lt. 0) then
             goto 6000
          elseif (CRES .eq. 0) then 
             goto 100
          else
             goto 200
          endif
  100         work(nxtnum) = i
              nxtnum = nxtnum + 1
              go to 300
  200         if  (degree(i) .gt. mxdg)  mxdg = degree(i)
  300 continue
c     ... while  nxtnum <= n  do ...
 1000 if  ( nxtnum .gt. n )  go to 2000
c         ... find an unnumbered node of minimal degree
          lowdg = mxdg + 1
          stnode = 0
          do 400 i = 1, n
              if ( (degree(i) .le. 0) .or. (degree(i) .ge. lowdg) )
     1           go to 400
                  lowdg = degree(i)
                  stnode = i
  400     continue
c
          if ( stnode .eq. 0 )  go to 6100
c set up pointers for three lists in work area, then look
c for pseudo-diameter, beginning with stnode.
          avail = (wrklen - nxtnum + 1) / 3
          nleft = n - nxtnum + 1
          space = max0 (space, nxtnum + 3*n - 1)
          if ( avail .lt. n )  go to 5200
c
          call gpskcb (n, degree, rstart, connec, avail, nleft,
     1                 stnode, rvnode, work(nxtnum), tree1, tree2,
     2                 active, depth, width1, width2,
     3                 error, space)
          if ( error .ne. 0 )  go to 5000
          space = max0 (space, nxtnum + 3*(active+depth+1) - 1)
c dynamic space check for most of remainder of algorithm
          reqd = max0 (nxtnum + 2*n + 3*depth - 1, 3*n + 2*depth + 1)
          space = max0 (space, reqd)
          if  ( wrklen .lt. reqd )  go to 5300
c output from gpskcb is a pair of level trees, in the form of lists of 
c nodes by level.  convert this to two lists of  of level number by node.
c at the same time pack storage so that one of the level tree vectors is
c at the back end of the work area.
          lvlptr = nxtnum + avail - depth
          call gpskce (n, avail, active, depth, wrklen, work(nxtnum),
     1                 work(lvlptr), work(1), nxtnum, tree1,
     2                 tree2, width1, width2, oneis1, error, space)
          if ( error .ne. 0 ) go to 5000
          if (( tree1 .ne. wrklen - n + 1 ) .or. (tree2 .ne. nxtnum))
     1      go to 6200
c combine the two level trees into a more general level structure.
          avail = wrklen - nxtnum + 1 - 2*n - 3*depth
          stotal = n + nxtnum
          empty = stotal + depth
          inc1 = tree1 - depth
          inc2 = inc1 - depth
c
          call gpskcg (n, degree, rstart, connec, active, width1,
     1                 width2, work(tree1), work(tree2), work(empty),
     2                 avail, depth, work(inc1), work(inc2),
     3                 work(stotal), oneis1, revrs1, error, cspace)
c
          if ( error .ne. 0 )  go to 5000
          space = max0 (space, nxtnum + cspace - 1)
c combined level structure is represented by gpskcg as a vector of level 
c numbers. for renumbering phase, convert this also to the inverse permutation.
          lvlptr = tree1 - (depth + 1)
          lvllst = lvlptr - active
          if ( stotal + depth .gt. lvlptr )  go to 6300
c
          call gpskci (n, active, depth, work(tree1), work(lvllst),
     1                 work(lvlptr), work(stotal), error, space)
          if  (error .ne. 0)  go to 5000
c now renumber all members of this component using either a reverse
c cuthill-mckee or a king strategy,
c as profile or bandwidth reduction is more important.
          if ( optpro.eq.1 )  go to 500
              call gpskcj (n, degree, rstart, connec, active,
     1                     work(nxtnum), stnode, rvnode, revrs1, depth,
     2                     work(lvllst), work(lvlptr), work(tree1),
     3                     error, space)
              if ( error .ne. 0 )  go to 5000
              nxtnum = nxtnum + active
              go to 600
c
  500         call gpskck (n, degree, rstart, connec, lvllst-1, nxtnum,
     1                     work, active, depth, work(lvllst),
     2                     work(lvlptr), work(tree1), error, space)
              if ( error .ne. 0 )  go to 5000
c         =========================================================
c         ... end of while loop ... repeat if graph is disconnected
c         =========================================================
  600     go to 1000
c check whether initial numbering or final numbering provides better results
 2000 if  (wrklen .lt. 2*n)  go to 5400
c
      if  (optpro.eq.1)  go to 2100
          call gpskcl (n, degree, rstart, connec, work(1), work(n+1),
     1                 permut, bandwd, profil, error, space)
          go to 2200
c
 2100     call gpskcm (n, degree, rstart, connec, work(1), work(n+1),
     1                 permut, bandwd, profil, error, space)
c
 2200 return
c     . . .  e r r o r   d i a g n o s t i c s
c error detected by lower level routine.  make sure that signs
c of degree are properly set
 5000 do 5100 i = 1, n
          if  (degree(i) .lt. 0)  degree(i) = -degree(i)
 5100 continue
c
      bandwd = -1
      profil = -1
      return
c storage allocation errors detected in this routine
 5200 error = 101
      space = -1
      go to 5000
 5300 error = 102
      space = -1
      go to 5000
 5400 error =  10
      space = 2*n - wrklen
      go to 5000
c data structure errors detected in this routine
 6000 error = 1
      go to 6900
 6100 error = 2
      go to 6900
 6200 error = 3
      go to 6900
 6300 error = 4
 6900 space = -1
      go to 5000
      end
c *****************************************************************************
      subroutine  gpskcb  (n, degree, rstart, connec, avail, nleft,     
     1                     stnode, rvnode, work, forwd, bestbk, nnodes,
     2                     depth, fwidth, bwidth, error, space)
c find a pseudo-diameter of the matrix graph ...
c         << build a level tree from stnode >>
c         repeat
c             << build a level tree from each node 'bknode' in the
c                deepest level of  stnode's tree >>
c             << replace 'stnode' with 'bknode' if a deeper and
c                narrower tree was found. >>
c         until
c             << no further improvement made >>
c
c     ... heuristic above differs from the algorithm published in
c         siam j. numerical analysis, but matches the code
c         distributed by toms.
c parameters :
c     n, degree, rstart & connec  describe the matrix structure
c     work   -- working space, of length  3*avail, used to store
c               three level trees.
c     stnode is initially the number of a node to be used to start the
c            process, to be the root of the first tree. on output, stnode is
c            the end of the pseudo-diameter whose level tree is narrowest.
c     rvnode will be the other end of the pseudo-diameter.
c     nnodes will be the number of nodes in this connected componnent
c            of the matrix graph, i.e., the length of the level trees.
c     depth  -- the depth of the level trees being returned,
c                   i.e., the length of the pseudo-diameter.
c     ==================================================================
c     structure of workspace ...
c     ---------------------------------------------------------------
c     : numbered :  tlist1  ptr1  :  tlist2  ptr2  :  tlist3  ptr3  :
c     ---------------------------------------------------------------
c     tlisti is a list of nodes of length  'active'
c     ptri   is a list of pointers into tlisti, of length  'depth+1'
      integer n,rstart(n),avail,nleft,stnode,rvnode,forwd,bestbk,
     1 nnodes,depth,fwidth,bwidth,error,space
      integer degree(n), connec(1), work(avail,3),backwd
      integer mxdpth,width,fdepth,lstlvl,nlast,t,i,bknode,lstlvi
      logical improv
c build initial level tree from 'stnode'.  find out how many
c nodes lie in the current connected component.
      forwd = 1
      backwd = 2
      bestbk = 3
c
      call gpskcc (n, degree, rstart, connec, stnode, avail, nleft,
     1              work(1,forwd), nnodes, depth, width, error,
     2              space)
      if ( error .ne. 0 )  go to 5000
c
      mxdpth = avail - nnodes - 1
c repeat until no deeper trees are found ...
 1000     fwidth = width
          fdepth = depth
          lstlvl = avail - depth + 1
          nlast = work (lstlvl-1, forwd) - work (lstlvl, forwd)
          lstlvl = work (lstlvl, forwd)
          bwidth = n+1
c sort the deepest level of 'forwd' tree into increasing order of node degree.
          call gpskcq (nlast, work(lstlvl,forwd), n, degree, error)
          if  (error .ne. 0)  go to 6000
c build level tree from nodes in 'lstlvl' until a deeper
c and narrower tree is found or the list is exhausted.
          improv = .false.
          do 1200 i = 1, nlast
              lstlvi = lstlvl + i - 1
              bknode = work (lstlvi, forwd)
              call gpskcd (n, degree, rstart, connec, bknode, avail,
     1                     nnodes, mxdpth, work(1,backwd), depth, width,
     2                     bwidth, error, space)
              if ( error .ne. 0 )  go to 5000
c
              if ( depth .le. fdepth )  go to 1100
c new deeper tree ... make it new 'forwd' tree and break out of 'do' loop.

                  improv = .true.
                  t = forwd
                  forwd = backwd
                  backwd = t
                  stnode = bknode
                  go to 1300
c else check for narrower tree.
 1100             if ( width .ge. bwidth )  go to 1200
                      t = bestbk
                      bestbk = backwd
                      backwd = t
                      bwidth = width
                      rvnode = bknode
 1200     continue
c end of repeat loop
 1300     if ( improv )  go to 1000
c
      depth = fdepth
      return
c in case of error, simply return error flag to user.
 5000 return
c
 6000 error = 11
      space = -1
      return
c
      end
c***************************************************************************
      subroutine gpskcc(n,degree,rstart,connec,stnode,avail,  
     1 nleft, list, active, depth, width, error,space)
c     build the level tree rooted at 'stnode' in the space provided in
c     list.  check for overrun of space allocation.
      integer     n, rstart(n), stnode, avail, nleft,
     1            active, depth, width, error, space
      integer     degree(n), connec(1), list(avail)
c input ...
c     n, degree, rstart, connec -- describe the matrix structure
c     stnode -- the root of the level tree.
c     avail  -- the length of the working space available
c     nleft  -- the number of nodes yet to be numbered
c     list   -- the working space.
c output ...
c     active -- the number of nodes in the component
c     depth  -- the depth of the level tree rooted at  stnode.
c     width  -- the width of the level tree rooted at  stnode.
c     error  -- zero unless storage was insufficient.
      integer lstart,nlevel,front,j,newnod,ptr,cdgree,lfront,listj
c build the level tree using  list  as a queue and leaving the nodes in place.
c this generates the nodes ordered by level  put pointers to the beginning
c of each level, building from the back of the work area.
      active = 1
      depth = 0
      width = 0
      error = 0
      lstart = 1
      front = 1
      list (active) = stnode
      degree (stnode) = -degree (stnode)
      list (avail)  = 1
      nlevel = avail
c     ... repeat until queue becomes empty or we run out of space.
 1000     if ( front .lt. lstart ) go to 1100
c         ... first node of level.  update pointers.
              lstart = active + 1
              width = max0 (width, lstart - list(nlevel))
              nlevel = nlevel - 1
              depth = depth + 1
              if ( nlevel .le. active )  go to 5000
                  list (nlevel) = lstart
c         ... find all neighbors of current node, add them to queue.
 1100     lfront = list (front)
          ptr = rstart (lfront)
          cdgree = -degree (lfront)
          if (cdgree .le. 0)  go to 6000
          do 1200 j = 1, cdgree
              newnod = connec (ptr)
              ptr = ptr + 1
c             ... add to queue only nodes not already in queue
              if ( degree(newnod) .le. 0 )  go to 1200
                  degree (newnod) = -degree (newnod)
                  active = active + 1
                  if ( nlevel .le. active )  go to 5000
                  if ( active .gt. nleft  )  go to 6000
                      list (active) = newnod
 1200     continue
          front = front + 1
c         ... is queue empty?
          if ( front .le. active )  go to 1000
c     ... yes, the tree is built.  undo our markings.
      do 1300 j = 1, active
          listj = list(j)
          degree (listj) = -degree (listj)
 1300 continue
c
      return
c     ... insufficient storage ...
 5000 space = 3 * ( (nleft+1-active)*depth / nleft + (nleft+1-active) )
      error = 110
      return
c
 6000 error = 12
      space = -1
      return
c
      end
c**************************************************************************
      subroutine   gpskcd(n,degree,rstart,connec,stnode,avail,  
     1 active, mxdpth, list, depth, width, maxwid,error, space)
c     build the level tree rooted at 'stnode' in the space provided in
c     list.  overflow check needed only on depth of tree.
c
c     build the level tree to completion only if the width of all
c     levels is smaller than 'maxwid'.  if a wider level is found
c     terminate the construction.
      integer     n, rstart(n), stnode, avail, active, mxdpth,
     1            depth, width, maxwid, error, space
      integer     degree(n), connec(1), list(avail)
c         input ...
c             n, degree, rstart, connec -- describe the matrix structure
c             stnode -- the root of the level tree.
c             avail  -- the length of the working space available
c             nleft  -- the number of nodes yet to be numbered
c             active -- the number of nodes in the component
c             mxdpth -- maximum depth of level tree possible in
c                       allotted working space
c             list   -- the working space.
c         output ...
c             depth  -- the depth of the level tree rooted at  stnode.
c             width  -- the width of the level tree rooted at  stnode.
c             maxwid -- limit on width of the tree.  tree will not be
c                       used if width of any level is as great as
c                       maxwid, so construction of tree need not
c                       continue if any level that wide is found.
c             error  -- zero unless storage was insufficient.
      integer     lstart, nlevel, front, j, newnod, ptr, back,
     1            sptr, fptr, lfront, listj
c
c     ... build the level tree using  list  as a queue and leaving
c         the nodes in place.  this generates the nodes ordered by level
c         put pointers to the beginning of each level, building from
c         the back of the work area.
c
      back = 1
      depth = 0
      width = 0
      error = 0
      lstart = 1
      front = 1
      list (back) = stnode
      degree (stnode) = -degree (stnode)
      list (avail)  = 1
      nlevel = avail
c     ... repeat until queue becomes empty or we run out of space.
c     ------------------------------------------------------------
 1000     if ( front .lt. lstart ) go to 1100
c         ... first node of level.  update pointers.
              lstart = back + 1
              width = max0 (width, lstart - list(nlevel))
              if  ( width .ge. maxwid )  go to 2000
              nlevel = nlevel - 1
              depth = depth + 1
              if ( depth .gt. mxdpth )  go to 5000
                  list (nlevel) = lstart
c         ... find all neighbors of current node, add them to queue.
 1100     lfront = list (front)
          sptr = rstart (lfront)
          fptr = sptr - degree (lfront) - 1
          do 1200 ptr = sptr, fptr
              newnod = connec (ptr)
c             ... add to queue only nodes not already in queue
              if ( degree(newnod) .le. 0 )  go to 1200
                  degree (newnod) = -degree (newnod)
                  back = back + 1
                  list (back) = newnod
 1200     continue
          front = front + 1
c ... is queue empty?
          if ( front .le. back )  go to 1000
c     ... yes, the tree is built.  undo our markings.
      if (back .ne. active)  go to 6000
c
 1300 do 1400 j = 1, back
          listj = list(j)
          degree (listj) = -degree (listj)
 1400 continue
      return
c     ... abort generation of tree because it is already too wide
 2000 width = n + 1
      depth = 0
      go to 1300
c     ... insufficient storage ...
 5000 space = 3 * ( (active+1-back)*depth / active + (active+1-back) )
      error = 111
      return
 6000 error = 13
      space = -1
      return
      end
c*************************************************************************
      subroutine gpskce(n,avail,active,depth,wrklen,lvllst,lvlptr,           
     1 work,nxtnum,tree1,tree2,width1,width2,oneis1,error,space)
c
c     transition between algorithm i and algorithm ii of
c     the gibbs-poole-stockmeyer paper.
c
c     in this implementation algorithm i represents level trees as
c     lists of nodes ordered by level.  algorithm ii appears to require
c     level numbers indexed by node -- vectors for efficiency.
c     this subroutine changes the level tree representation to that
c     required by algorithm ii.  note that the first algorithm can be
c     carried out with the level number vector format, probably requring
c     more computation time, but perhaps less storage.
c
c     input:  two level trees, as level lists and level pointers,
c             found in two of the three columns of the arrays 'lvllst'
c             and 'lvlptr'
c
c     output: two level trees, as vectors of level numbers,
c             one packed to the front, one to the rear of the working
c             area 'work'.  note that 'work', 'lvllst' and 'lvlptr'
c             share common locations.
c     ================================================================
c     ... structure of workspace
c         input .. (output from gpskcb)
c     --------------------------------------------------------------
c     : numbered : tlist1  ptr1  :  tlist2  ptr2  :  tlist3  ptr3  :
c     --------------------------------------------------------------
c         output .. (goes to combin)
c     --------------------------------------------------------------
c     : numbered :  tree2  :           ...               :  tree1  :
c     --------------------------------------------------------------
      integer     n, avail, active, depth, wrklen, nxtnum,
     1            width1, width2, tree1, tree2, error, space
      integer     lvllst(avail,3), lvlptr(avail,3), work(wrklen)
      logical     oneis1
      integer     i, btree, ftree, fwidth, bwidth
c     ... check that we have enough room to do the necessary unpacking
      if (3*avail .gt. wrklen)  go to 6000
      if (avail .lt. n)  go to 5100
c
c     ... input has three possible cases:
c             lvllst(*,1) is empty
c             lvllst(*,2) is empty
c             lvllst(*,3) is empty
      ftree = tree1
      btree = tree2
      fwidth = width1
      bwidth = width2
c
      tree1 = wrklen - n + 1
      tree2 = nxtnum
c
      if ( (ftree .eq. 1) .or. (btree .eq. 1) )  go to 300
c         ... case 1:  1st slot is empty.  unpack 3 into 1, 2 into 3
          if (ftree .ne. 2)  go to 100
              oneis1 = .true.
              width2 = bwidth
              width1 = fwidth
              go to 200
c
  100         oneis1 = .false.
              width1 = bwidth
              width2 = fwidth
c
  200     call gpskcf (n, active, depth, lvllst(1,3), lvlptr(1,3),
     1                    work(tree2), oneis1)
c
          call gpskcf (n, active, depth, lvllst(1,2), lvlptr(1,2),
     1                    work(tree1), .not. oneis1)
c
          go to 1000
c
c
  300 if ( (ftree .eq. 2) .or. (btree .eq. 2) )  go to 600
c         ... case 2:  2nd slot is empty.  to enable complete
c              repacking, move 3 into 2, then fall into next case
          do 400 i = 1, active
              lvllst(i,2) = lvllst(i,3)
  400     continue
c
          do 500 i = 0, depth
              lvlptr(i,2) = lvlptr(i,3)
  500     continue
c         ... case 3:  slot 3 is empty.  move 1 into 3, then 2 into 1.
  600     if (ftree .eq. 1) go to 700
              oneis1 = .false.
              width1 = bwidth
              width2 = fwidth
              go to 800
c
  700         oneis1 = .true.
              width1 = fwidth
              width2 = bwidth
c
  800     call gpskcf (n, active, depth, lvllst(1,1), lvlptr(1,1),
     1                    work(tree1), .not. oneis1)
c
          call gpskcf (n, active, depth, lvllst(1,2), lvlptr(1,2),
     1                    work(tree2), oneis1)
 1000 return
c
c     ------------------------------------------------------------------
c
 5100 space = 3 * (n - avail)
      error = 120
      return
c
 6000 error = 20
      space = -1
      return
c
      end
c**************************************************************************
      subroutine  gpskcf(n,active,depth,lvllst,lvlptr,lvlnum,revers)
c convert level structure representation from a list of nodes
c grouped by level to a vector giving level number for each node.
c     lvllst, lvlptr -- list of lists
c     lvlnum -- output vector of level numbers
c     revers -- if .true., number level structure from back end
c               instead of from front
      integer  n,active,depth,lvllst(active),lvlptr(depth),lvlnum(n)
      logical     revers
      integer     i, level, lstart, lend, xlevel, plstrt, lvllsi
      if  (active .eq. n)  go to 200
c if not all nodes of graph are active, mask out the nodes which are not active
          do 100 i = 1, n
              lvlnum(i) = 0
  100     continue
c
  200 do 400 level = 1, depth
          xlevel = level
          plstrt = depth - level + 1
          if (revers) xlevel = plstrt
          lstart = lvlptr (plstrt)
          lend = lvlptr (plstrt - 1) - 1
c
          do 300 i = lstart, lend
              lvllsi = lvllst(i)
              lvlnum (lvllsi) = xlevel
  300     continue
  400 continue
      return
      end
c***************************************************************************
      subroutine gpskcg(n,degree,rstart,connec,active,width1,width2, 
     1 tree1,tree2,work,wrklen,depth,inc1,inc2,total,oneis1,revrs1,
     2 error,space)
c     combine the two rooted level trees into a single level structure
c     which may have smaller width than either of the trees.  the new
c     structure is not necessarily a rooted structure.
c parameters:
c     n, degree, rstart, connec -- give the dimension and structure
c                                      of the sparse symmetric matrix
c     active -- the number of nodes in this connected component of
c                   the matrix graph
c     tree1  -- on input, one of the input level trees.  on
c                   output, the combined level structure
c     tree2  -- the second input level tree
c     width1 -- the maximum width of a level in tree1
c     width2 -- the maximum width of a level in tree2
c     work   -- a working area of length 'wrklen'
c     inc1,  -- vectors of length 'depth'
c     inc2,total,oneis1
c          -- indicates whether tree1 or tree2 represents the
c                   forward tree or the backwards tree of phase 1.
c                   used to mimic arbitrary tie-breaking procedure of
c                   original gibbs-poole-stockmeyer code.
c
c         revrs1 -- output parameter indicating whether a backwards
c                   ordering was used for the largest component of
c                   the reduced graph
c         error  -- non-zero only if failure of space allocation or
c                   data structure error found
c         space -- minimum space required to rerun or complete phase.
      integer n,rstart(n),active,width1,width2,wrklen,depth,error,space
      integer     degree(n), connec(1), tree1(n), tree2(n),
     1            work(wrklen), inc1(depth), inc2(depth), total(depth)
      logical     oneis1, revrs1
c     << remove all nodes of pseudo-diameters >>
c     << find connected components of reduced graph >>
c     << combine level trees, component by component >>
c     structure of workspace ...
c     ------------------------------------------------------------------
c     : numbered : tree2 : total : nodes : start : size : inc1 : inc2 :
c     ------------------------------------------------------------------
c     --------
c      tree1 :
c     --------
c         numbered  is the set of  numbered nodes (probably empty)
c         tree1 and tree1 are level trees (length n)
c         total, inc1 and inc2  are vectors of node counts per level
c             (length 'depth')
c         nodes is the set of nodes in the reduced graph (the nodes
c             not on any shortest path from one end of the
c             pseudodiameter to the other)
c         start, zsize are pointers into 'nodes', one of each for
c         each connected component of the reduced graph.
c         the sizes of nodes, start and size are not known apriori.
c
c     ==================================================================
      integer     i, zsize, avail, cstop, start, compon, tree1i, pcstrt,
     1            cstart, mxinc1, mxinc2, compns, mxcomp, offdia,
     2            csize, pcsize, worki, tworki
c find all shortest paths from start to finish.  remove nodes onthese 
c paths and in other connected components of full graph from further
c consideration.  sign of entries in tree1 is used as a mask.
      offdia = active
c
      do 100 i = 1, depth
          total(i) = 0
  100 continue
c
      do 200 i = 1, n
          tree1i = tree1 (i)
          if ((tree1(i).ne.tree2(i)) .or. (tree1(i).eq.0)) go to 200
              total (tree1i) = total (tree1i) + 1
              tree1(i) = - tree1(i)
              offdia = offdia - 1
  200 continue
c
      if ( offdia .eq. 0 )  go to 1100
      if ( offdia .lt. 0 )  go to 6000
c find connected components of graph induced by the nodes not removed. 
c 'mxcomp' is the largest number of components representable in
c the working space available.
      avail = wrklen - offdia
      mxcomp = avail/2
      start = offdia + 1
      zsize = start + mxcomp
c
      if  (mxcomp .le. 0)  go to 5100
c
      call gpskch (n, degree, rstart, connec, tree1, offdia, work,
     1             mxcomp, work(start), work(zsize), compns, error,
     2             space)
      if ( error .ne. 0 )  go to 5000
c record space actually used  (not including  numbered )
      space = 2*n + 3*(depth) + 2*compns + offdia
c sort the component start pointers into increasing order of size of component
      if (compns .gt. 1)
     1    call gpskcn (compns, work(zsize), work(start), error)
          if  (error .ne. 0)  go to 6200
c for each component in turn, choose to use the ordering of the 'forward' 
c tree1 or of the 'backward' tree2 to number the nodes in this component. 
c the numbering is chosen to minimize the maximum increment to any level.
      do 1000 compon = 1, compns
          pcstrt = start + compon - 1
          cstart = work (pcstrt)
          pcsize = zsize + compon - 1
          csize = work (pcsize)
          cstop  = cstart + csize - 1
          if ((csize .lt. 0) .or. (csize .gt. offdia))  go to 6100
c
          do 300 i = 1, depth
              inc1(i) = 0
              inc2(i) = 0
  300     continue
c
          mxinc1 = 0
          mxinc2 = 0
c
          do 400 i = cstart, cstop
              worki = work(i)
              tworki = -tree1 (worki)
              inc1 (tworki) = inc1 (tworki) + 1
              tworki =  tree2 (worki)
              inc2 (tworki) = inc2 (tworki) + 1
  400     continue
c         ... baroque tests below duplicate the gibbs-poole-stockmeyer-
c             crane program, *** not *** the published algorithm.
          do 500 i = 1, depth
              if ((inc1(i) .eq. 0) .and. (inc2(i) .eq. 0))  go to 500
                  if  (mxinc1  .lt.  total(i) + inc1(i))
     1                 mxinc1 = total(i) + inc1(i)
                  if  (mxinc2  .lt.  total(i) + inc2(i))
     1                 mxinc2 = total(i) + inc2(i)
  500     continue
c         ... use ordering of narrower tree unless it increases
c             width more than wider tree.  in case of tie, use tree 2!
          if ( (mxinc1 .gt. mxinc2)  .or.
     1         ( (mxinc1 .eq. mxinc2) .and. ( (width1 .gt. width2) .or.
     2                                        ( (width1 .eq. width2)
     3                                         .and. oneis1) ) ) )
     4      go to 700
c
              if ( compon .eq. 1 )  revrs1 = .not. oneis1
c
              do 600 i = 1, depth
                  total(i) = total(i) + inc1(i)
  600         continue
              go to 1000
c
  700         if ( compon .eq. 1 )  revrs1 = oneis1
              do 800 i = cstart, cstop
                  worki = work(i)
                  tree1 (worki) = - tree2 (worki)
  800         continue
c
              do 900 i = 1, depth
                  total(i) = total(i) + inc2(i)
  900         continue
c
 1000 continue
      go to 2000
c     ... default when the reduced graph is empty
 1100 revrs1 = .true.
      space = 2*n
 2000 return
c     error found ...
 5000 space = -1
      go to 2000
 5100 space = 2 - avail
      error = 131
      go to 2000
 6000 error = 30
      go to 5000
 6100 error = 31
      go to 5000
 6200 error = 32
      go to 5000
      end
c**************************************************************************
      subroutine gpskch(n,degree,rstart,connec,status,nreduc,
     1 work, mxcomp, start, zsize, compns, error, space)
c     find the connected components of the graph induced by the set
c     of nodes with positive 'status'.  we shall build the list of
c     connected components in 'work', with a list of pointers
c     to the beginning nodes of components located in 'start'
      integer     n, rstart(n), nreduc, mxcomp, compns, error, space
      integer     degree(n), connec(1), status(n), work(nreduc),
     1            start(mxcomp), zsize(mxcomp)
c     parameters ...
c         n      -- dimension of the original matrix
c         degree, rstart, connec -- the structure of the original matrix
c         status -- derived from a level tree. positive entries indicate
c                   active nodes.  nodes with status <= 0 are ignored.
c         nreduc -- the number of active nodes
c         work   -- work space, used as a queue to build connected
c                   components in place.
c         mxcomp -- maximum number of components allowed by current
c                   space allocation.  must not be violated.
c         start  -- pointer to beginning of  i-th  connected component
c         zsize   -- size of each component
c         compns -- number of components actually found
c         error  -- should be zero on return unless we have too little
c                   space or we encounter an error in the data structure
c         space  -- maximum amount of workspace used / needed
      integer     i, j, free, jptr, node, jnode, front, cdgree, root
c     repeat
c         << find an unassigned node and start a new component >>
c         repeat
c             << add all new neighbors of front node to queue, >>
c             << remove front node.                            >>
c         until <<queue empty>>
c     until << all nodes assigned >>
      free   = 1
      compns = 0
      root   = 1
c ... start of outer repeat loop
c         ... find an unassigned node
  100     do 200 i = root, n
              if (status(i) .le. 0) go to 200
                  node = i
                  go to 300
  200     continue
          go to 6100
c         ... start new component
  300     compns = compns + 1
          root   = node + 1
          if (compns .gt. mxcomp)  go to 5000
          start (compns) = free
          work (free) = node
          status (node) = -status (node)
          front = free
          free = free + 1
c             ... inner repeat until queue becomes empty
  400         node = work (front)
              front = front + 1
c
              jptr = rstart (node)
              cdgree = degree (node)
              do 500 j = 1, cdgree
                  jnode = connec (jptr)
                  jptr = jptr + 1
                  if (status(jnode) .lt. 0) go to 500
                  if (status(jnode) .eq. 0) go to 6000
                      status (jnode) = -status (jnode)
                      work (free) = jnode
                      free = free + 1
  500         continue
c
              if (front .lt. free) go to 400
c ... end of inner repeat.  compute size of component and
c             see if there are more nodes to be assigned
          zsize (compns) = free - start (compns)
          if (free .le. nreduc)  go to 100
c
      if (free .ne. nreduc+1)  go to 6200
      return
 5000 space = nreduc - free + 1
      error = 130
      return
 6000 error = 33
      space = -1
      return
 6100 error = 34
      space = -1
      return
 6200 error = 35
      space = -1
      return
      end
c**************************************************************************
      subroutine gpskci(n,active,depth,lstruc,lvllst,lvlptr, 
     1 ltotal, error, space)
c transitional subroutine, algorithm ii to iiia or iiib.
c convert level structure given as vector of level numbers for nodes
c to structure as list of nodes by level
c     n, active, depth -- problem sizes
c     lstruc -- input level structure
c     lvllst, lvlptr -- output level structure
c     ltotal -- number of nodes at each level (precomputed)
      integer     n, active, depth, error, space
      integer     lstruc(n), lvllst(active), lvlptr(1), ltotal(depth)
c     structure of workspace ..
c         input (from combin) ..
c     ------------------------------------------------------------------
c     :  numbered  :  ..(n)..  :  total  :         ...        :  tree  :
c     ------------------------------------------------------------------
c
c         output (to gpskcj or gpskck) ..
c
c     ------------------------------------------------------------------
c     :  numbered  :       ...             :  tlist  :  tptr  :  tree  :
c     ------------------------------------------------------------------
c
c     here, numbered is the set of nodes in numbered components
c         total is a vector of length 'depth' giving the number
c         of nodes in each level of the 'tree'.
c         tlist, tptr are lists of nodes of the tree, arranged
c         by level.  tlist is of length 'active', tptr 'depth+1'.
      integer     i, acount, start, level, plevel
c     ... establish starting and ending pointers for each level
      start = 1
      do 100 i = 1, depth
          lvlptr(i) = start
          start = start + ltotal(i)
          ltotal(i) = start
  100 continue
      lvlptr(depth+1) = start
c
      acount = 0
      do 300 i = 1, n
         CRES=lstruc(i)
          if (CRES .LT. 0) then
             goto 200
          elseif (CRES .EQ. 0) then
             goto 300
          else
             goto 6000
          endif
  200         level = -lstruc(i)
              lstruc(i) = level
              plevel = lvlptr (level)
              lvllst (plevel) = i
              lvlptr (level) = lvlptr (level) + 1
              acount = acount + 1
              if (lvlptr (level) .gt. ltotal (level))  go to 6100
  300 continue
c     ... reset starting pointers
      lvlptr(1) = 1
      do 400 i = 1, depth
          lvlptr(i+1) = ltotal(i)
  400 continue
      return
 6000 error = 40
      go to 6200
 6100 error = 41
 6200 space = -1
      return
      end
c************************************************************************
      subroutine gpskcj(n,degree,rstart,connec,ncompn,invnum,snode1,
     1 snode2, revrs1,depth, lvllst, lvlptr, lvlnum, error,space)
c     number the nodes in a generalized level structure according
c     to a generalization of the cuthill mckee strategy.
c
c     n      -- dimension of original problem
c     degree, rstart, connec -- give structure of sparse and
c                               symmetric matrix
c     ncompn -- number of nodes in this component of matrix graph
c     invnum -- will become a list of the original nodes in the order
c               which reduces the bandwidth of the matrix.
c     nxtnum -- the next index to be assigned (1 for first component)
c     revrs1 -- if .true., first component of reduced graph was numbered
c               backwards.
c     lvllst -- list of nodes in level tree ordered by level.
c     lvlptr -- position of initial node in each level of lvllst.
c     lvlnum -- level number of each node in component
      integer     n, rstart(n), ncompn, snode1, snode2, depth,
     1            error, space
      integer     degree(n), connec(1), invnum(ncompn),
     1            lvllst(ncompn), lvlptr(depth), lvlnum(n)
      logical     revrs1
c numbering requires two queues, which can be build in place in invnum.
c     ==================================================================
c     a l g o r i t h m    s t r u c t u r e
c     ==================================================================
c     << set queue1 to be the set containing only the start node. >>
c
c     for level = 1 to depth do
c         begin
c         loop
c             repeat
c                 begin
c                 << cnode <- front of queue1                        >>
c                 << add unnumbered neighbors of cnode to the back   >>
c                 << of queue1 or queue2 (use queue1 if neighbor     >>
c                 << at same level, queue2 if at next level).  sort  >>
c                 << the newly queued nodes into increasing order of >>
c                 << degree.  number cnode, delete it from queue1.   >>
c                 end
c             until
c                 << queue1 is empty >>
c         exit if << all nodes at this level numbered >>
c             begin
c             << find the unnumbered node of minimal degree at this >>
c             << level, restart queue1 with this node.              >>
c             end
c         end << loop loop >>
c         << promote queue2 to be initial queue1 for next iteration >>
c         << of  for  loop.                                         >>
c         end <<for loop>>
c     ==================================================================
c     structure of workspace ..
c     --------------------------------------------------------------
c     : numbered :  queue1  :  queue2  : ... : tlist : tptr : tree :
c     --------------------------------------------------------------
c     on completion, we have only a new, longer numbered set.
      integer     i, bq1, bq2, fq1, inc, cptr, cnode,
     1            inode, level, nleft, lstart, lwidth, queue1,
     2            queue2, cdgree, xlevel, stnode, ilevel, sq1, sq2,
     3            nsort, lowdg, bptr, lvllsc, lvllsb, invnmi
c
      logical     forwrd, rlevel
c ... gibbs-poole-stockmeyer heuristic choice of order
      if  (degree(snode1) .gt. degree(snode2))  go to 10
          forwrd = revrs1
          stnode = snode1
          go to 20
c
   10     forwrd = .not. revrs1
          stnode = snode2
c ... set up initial queues at front of 'invnum' for forwrd order,
c         at back for reversed order.
   20 if (forwrd) go to 100
          inc = -1
          queue1 = ncompn
          go to 200
c
  100     inc = +1
          queue1 = 1
c
  200 invnum (queue1) = stnode
      rlevel = (lvlnum(stnode) .eq. depth)
      lvlnum (stnode) = 0
      fq1 = queue1
      bq1 = queue1 + inc
c number nodes level by level ...
      do 3000 xlevel = 1, depth
          level = xlevel
          if  (rlevel)  level = depth - xlevel + 1
c
          lstart = lvlptr (level)
          lwidth = lvlptr (level+1) - lstart
          nleft = lwidth
          queue2 = queue1 + inc*lwidth
          bq2 = queue2
c ... 'loop' construct begins at statement 1000
c                 the inner 'repeat' will be done as many times as
c                 is necessary to number all the nodes at this level.
 1000     continue
c ... repeat ... until queue1 becomes empty
c take node from front of queue1, find each of its neighbors which have
c not yet been numbered, and add the neighbors to queue1 or queue2 
c according to their levels.
c
 1100             cnode = invnum (fq1)
                  fq1 = fq1 + inc
                  sq1 = bq1
                  sq2 = bq2
                  nleft = nleft - 1
c
                  cptr = rstart (cnode)
                  cdgree = degree (cnode)
                  do 1300 i = 1, cdgree
                      inode = connec (cptr)
                      cptr = cptr + 1
                      ilevel = lvlnum (inode)
                      if (ilevel .eq. 0)  go to 1300
                          lvlnum (inode) = 0
                          if ( ilevel .eq. level ) go to 1200
c
                              if  (iabs(level-ilevel) .ne. 1) go to 6400
                                  invnum (bq2) = inode
                                  bq2 = bq2 + inc
                                  go to 1300
c
 1200                             invnum (bq1) = inode
                                  bq1 = bq1 + inc
 1300             continue
c ... sort the nodes just added to queue1 and queue2
c separately into increasing order of degree.
                  if  (iabs (bq1 - sq1) .le. 1)  go to 1500
                      nsort = iabs (bq1 - sq1)
                      if  (forwrd)  go to 1400
                          call gpskcp (nsort, invnum(bq1+1), n, degree,
     1                                 error)
                          if  (error .ne. 0)  go to 6600
                          go to 1500
c
 1400                     call gpskcq (nsort, invnum(sq1), n, degree,
     1                                 error)
                          if  (error .ne. 0)  go to 6600
c
 1500             if  (iabs (bq2 - sq2) .le. 1)  go to 1700
                      nsort = iabs (bq2 - sq2)
                      if  (forwrd)  go to 1600
                          call gpskcp (nsort, invnum(bq2+1), n, degree,
     1                                 error)
                          if  (error .ne. 0)  go to 6600
                          go to 1700
c
 1600                     call gpskcq (nsort, invnum(sq2), n, degree,
     1                                 error)
                          if  (error .ne. 0)  go to 6600
c
c                     ... end of repeat loop
c
 1700             if  (fq1 .ne. bq1)  go to 1100
c ... queue1 is now empty ...
c if there are any unnumbered nodes left at this level,find the one of 
c minimal degree and return to the repeat loop above.
 2000     if  ((bq1 .eq. queue2) .and. (nleft .eq. 0))  go to 2900
c
              if ((nleft .le. 0) .or. (nleft .ne. inc * (queue2 - bq1)))
     1             go to 6200
c
              lowdg = n + 1
              bptr  = n + 1
              cptr  = lstart - 1
              do 2800 i = 1, nleft
 2600             cptr   = cptr + 1
                  lvllsc = lvllst (cptr)
                  if (lvlnum (lvllsc) .eq. level)  go to 2700
                      if (lvlnum (lvllsc) .ne. 0)  go to 6300
                      go to 2600
c
 2700             if  (degree(lvllsc) .ge. lowdg)  go to 2800
                      lowdg = degree (lvllsc)
                      bptr  = cptr
c
 2800         continue
c             ... minimal degree unnumbered node found ...
              if  (bptr .gt. n)  go to 6500
              lvllsb = lvllst (bptr)
              invnum (bq1) = lvllsb
              lvlnum (lvllsb) = 0
              bq1 = bq1 + inc
              go to 1000
c  ... advance queue pointers to make queue2 the
c                 new queue1 for the next iteration.
 2900     queue1 = queue2
          fq1 = queue1
          bq1 = bq2
          if  ((bq1 .eq. fq1) .and. (xlevel .lt. depth))  go to 6100
c
 3000 continue
c     ... change sign of degree to mark these nodes as 'numbered'
      do 3100 i = 1, ncompn
          invnmi = invnum(i)
          degree (invnmi) = -degree (invnmi)
 3100 continue
      return
 6000 space = -1
      return
 6100 error = 51
      go to 6000
 6200 error = 52
      go to 6000
 6300 error = 53
      go to 6000
 6400 error = 54
      go to 6000
 6500 error = 55
      go to 6000
 6600 error = 56
      go to 6000
      end
c************************************************************************
      subroutine gpskck(n,degree,rstart,connec,wrklen,nxtnum,
     1 work, ncompn, depth, lvllst, lvlptr, lvlnum, error, space)
c
      integer     n,rstart(n),wrklen,nxtnum,ncompn,depth,error,space
      integer     degree(n), connec(1), work(wrklen), lvllst(n),
     1            lvlptr(depth), lvlnum(n)
c     number nodes in a generalized level structure according to
c     a generalization of the king algorithm, which reduces
c     the profile of the sparse symmetric matrix.
c     ---------------------
c     code uses a priority queue to choose the next node to be numbered
c     the priority queue is represented by a simple linear-linked list
c     to save space.  this will require more searching than a fully
c     linked representation, but the data manipulation is simpler.
c     -------------------
c     << establish priority queue 'active' for level 1 nodes >>
c
c     for i = 1 to depth do
c         << set queue 'queued' to be empty, list 'next' to be >>
c         << set of nodes at next level.                       >>
c
c         for j = 1 to 'nodes at this level' do
c             << find first node in active with minimal connections >>
c             << to 'next'.  number this node and remove him from   >>
c             << 'active'.  for each node in 'next' which connected >>
c             << to this node, move it to 'queued' and remove it    >>
c             << from 'next'.                                       >>
c
c         << set new queue 'active' to be 'queued' followed by any >>
c         << nodes still in 'next'.                                >>
c
c     ==================================================================
c
c     data structure assumptions:
c     the first 'nxtnum-1' elements of  work  are already in use.
c     the level structure 'lvllst' is contiguous with  work, that is,
c     it resides in elements  wrklen+1, ...  of  work.  'lvlptr' and
c     'lvlnum' are also embedded in work, behind 'lvllst'.  the
c     three vectors are passed separately to clarify the indexing,
c     but the queues developed will be allowed to overrun 'lvllst'
c     as needed.
c
c     ... build the first 'active' queue starting w1 locations from
c         the front of the current working area  (w1 is the width of the
c         first level).  build the first 'queued' queue starting from
c         the back of work space.  the list 'next' will be realized
c         implicitly in 'lvlnum' as:
c                  lvlnum(i) > 0   <== level number of node.  'next' is
c                                      set with lvlnum(i) = level+1
c                  lvlnum(i) = 0   <== i-th node is in 'queued' or is
c                                      not in this component of graph,
c                                      or has just been numbered.
c                  lvlnum(i) < 0   <== i-th node is in 'active' and is
c                                      connected to -lvlnum(i) nodes in
c                                      'next'.
c     ==================================================================
c     structure of workspace ..
c     --------------------------------------------------------------
c     : numbered : done : active : alevel : ... : queued : lvllst :
c     --------------------------------------------------------------
c     -------------------
c       lvlptr : lvlnum :
c     -------------------
c     in the above,
c         numbered is the set of nodes already numbered from
c         previous components and earlier levels of this component.
c         done, active, alevel  are vectors of length the width of
c         the current level.  active is a set of indices into
c         alevel.  as the nodes in alevel are numbered, they
c         are placed into 'done'.
c         queued is a queue of nodes in the 'next' level, which
c         grows from the start of the 'next' level in lvllst
c         forwards toward 'alevel'.  queued is of length no more
c         than the width of the next level.
c         lvllst is the list of unnumbered nodes in the tree,
c         arranged by level.
c
c     ==================================================================
      integer     i, j, k, ptr, jptr, kptr, lptr, mptr, pptr, rptr,
     1            mpptr, jnode, knode, cnode, level, lowdg, unused,
     2            mxque, nnext, astart, mindg, lstart, lwidth, active,
     2            queueb, queued, qcount, nconnc, nactiv, cdgree,
     3            ldgree, nfinal, jdgree, strtic, added, twrkln,
     4            lvllsl, connej, conner, astptr, actptr, activi,
     5            astrti, queuei, acpptr
c
      twrkln = wrklen + ncompn + n + depth + 1
      unused = twrkln
c
      astart = lvlptr(1)
      lwidth = lvlptr(2) - astart
      astart = wrklen  + 1
      active = nxtnum + lwidth + 1
      nactiv = lwidth
      nfinal = nxtnum + ncompn 
c
      nnext = lvlptr(3) - lvlptr(2)
      queued = wrklen
      queueb = queued
      mxque = active + lwidth
c     ... build first priority queue 'active'
      lowdg = - (n + 1)
      lptr = lvlptr(1)
      do 200 i = 1, lwidth
          nconnc = 0
          lvllsl= lvllst (lptr)
          jptr = rstart (lvllsl)
          ldgree = degree(lvllsl)
          do 100 j = 1, ldgree
              connej = connec (jptr)
              if ( lvlnum (connej) .eq. 2 )  nconnc = nconnc - 1
              jptr = jptr + 1
  100     continue
c
          activi = active + i - 1
          work (activi) = i
          lvlnum (lvllsl) = nconnc
          lowdg = max0 (lowdg, nconnc)
          lptr = lptr + 1
  200 continue
      work (active-1) = 0
c     now number nodes level by level ...
      do 2000 level = 1, depth
c         ... number all nodes in this level
          do 1100 i = 1, lwidth
              pptr = -1
              ptr = work (active-1)
              if (nnext .eq. 0)  go to 1000
c if nodes remain in next, find the earliest node in active of minimal degree.
                  mindg = -(n+1)
                  do 400 j = 1, nactiv
                      astptr = astart + ptr
                      cnode = work (astptr)
                      if ( lvlnum (cnode) .eq. lowdg )  go to 500
                      if ( lvlnum (cnode) .le. mindg )  go to 300
                          mpptr = pptr
                          mptr = ptr
                          mindg = lvlnum (cnode)
  300                 pptr = ptr
                      actptr = active + ptr
                      ptr = work (actptr)
  400             continue
c establish  ptr  as first min degree node pptr as predecessor in list.
                  ptr = mptr
                  pptr = mpptr
c
  500             astptr = astart + ptr
                  cnode = work (astptr)
                  lowdg = lvlnum (cnode)
                  lvlnum (cnode) = 0
                  jptr = rstart (cnode)
c update connection counts for all nodes which
c connect to  cnode's  neighbors in  next.
                  cdgree = degree(cnode)
                  strtic = queueb
c
                  do 700 j = 1, cdgree
                      jnode = connec (jptr)
                      jptr = jptr + 1
                      if (lvlnum (jnode) .ne. level+1 )  go to 700
                          if (queueb .lt. mxque)  go to 5000
                          work (queueb) = jnode
                          queueb = queueb - 1
                          nnext = nnext - 1
                          lvlnum (jnode) = 0
                          if  (nactiv .eq. 1)  go to 700
                            kptr = rstart (jnode)
                            jdgree = degree (jnode)
                            do 600 k = 1, jdgree
                                knode = connec (kptr)
                                kptr = kptr + 1
                                if (lvlnum (knode) .ge. 0)  go to 600
                                    lvlnum (knode) = lvlnum (knode) + 1
                                    if  (lowdg .lt. lvlnum(knode))
     1                                   lowdg = lvlnum(knode)
  600                       continue
  700             continue
c to mimic the algorithm as implemented by gibbs, sort the nodes just
c added to the queue into increasing order of original index. (but, because
c the queue is stored backwards in memory, the sort routine is called
c for decreasing index.)
c
c treat  0, 1 or 2  nodes added as special cases
                  added = strtic - queueb
                  CRES=added-2
                  if (CRES .lt. 0) then
                     goto 1000
                  elseif (CRES .eq. 0) then
                     goto 800
                  else
                     goto 900
                  endif
c
  800                 if (work(strtic-1) .gt. work(strtic))  go to 1000
                          jnode = work(strtic)
                          work(strtic) = work(strtic-1)
                          work(strtic-1) = jnode
                          go to 1000
c
  900                 call gpskco (added, work(queueb+1), error)
                      if  (error .ne. 0)  go to 5500
c number this node and delete it from 'active'.
c mark it unavailable by changing sign of degree
 1000         nactiv = nactiv - 1
              astptr = astart + ptr
              cnode = work (astptr)
              work (nxtnum) = cnode
              degree (cnode) = -degree (cnode)
              nxtnum = nxtnum + 1
c             ... delete link to this node from list
              acpptr = active + pptr
              actptr = active + ptr
              work (acpptr) = work (actptr)
 1100     continue
c ... now move the queue 'queued' forward, at the same
c             time computing connection counts for its elements.
c             then do the same for the remaining nodes in 'next'.
          unused = min0 (unused, queueb - mxque)
          if ( nxtnum .ne. active-1 )  go to 5100
          if ( level .eq. depth ) go to 2000
              lstart = lvlptr (level+1)
              lwidth = lvlptr (level+2) - lstart
              active = nxtnum + lwidth + 1
              astart = active + lwidth
              nactiv = lwidth
              mxque = astart + lwidth
              if ( mxque .gt. queueb + 1 )  go to 5000
              unused = min0 (unused, queueb - mxque + 1)
c
              qcount = queued - queueb
              lowdg = -n-1
              work (active-1) = 0
c
              ptr = lstart
              do 1600 i = 1, lwidth
c ... choose next node from either 'queued' or 'next'
                  if (i .gt. qcount )  go to 1200
                      queuei = queued + 1 - i
                      cnode = work (queuei)
                      go to 1300
c
 1200                 cnode = lvllst (ptr)
                      ptr = ptr + 1
                       if ( ptr .gt. lvlptr(level+2) )  go to 5200
                          if (lvlnum (cnode) .gt. 0)  go to 1300
                              go to 1200
c
1300             if ( level+1 .eq. depth ) go to 1500
c
                      rptr = rstart (cnode)
                      nconnc = 0
                      jdgree = degree (cnode)
                      do 1400 j = 1, jdgree
                          conner = connec (rptr)
                          if ( lvlnum (conner) .eq. level+2 )
     1                        nconnc = nconnc - 1
                          rptr = rptr + 1
 1400                 continue
                      lvlnum (cnode) = nconnc
                      lowdg = max0 (lowdg, nconnc)
c ... add cnode to new 'active' queue
 1500             activi = active + (i - 1)
                  astrti = astart + (i - 1)
                  work (activi) = i
                  work (astrti) = cnode
 1600         continue
c
              if (depth .eq. level+1 ) go to 1700
                  nnext = lvlptr (level+3) - lvlptr (level+2)
                  queued = lstart - 1 + lwidth + wrklen
                  queueb = queued
                  go to 2000
c
 1700             nnext = 0
c
 2000 continue
c
c      print *,nxtnum,nfinal
      if  (nxtnum .ne. nfinal)  go to 5300
      space = max0 (space, twrkln - unused)
      return
 5000 space = nactiv + nnext
      error = 160
      return
 5100 error = 61
      go to 5400
 5200 error = 62
      go to 5400
c !!!!!!!
 5300 error = 63
 5400 return
 5500 error = 64
      go to 5400
      end
c************************************************************************
      subroutine   gpskcl   (n, degree, rstart, connec, invnum, newnum, 
     1                       oldnum, bandwd, profil, error, space)
c
      integer     n, rstart(n), bandwd, profil, error, space
      integer     degree(n), connec(1), invnum(n), newnum(n), oldnum(n)
c compute the bandwidth and profile for the renumbering given
c by 'invnum' and also for the renumbering given by 'oldnum'.
c 'newnum' will be a permutation vector copy of the node list 'invnum'.
      integer     i, j, jptr, idgree, oldbnd, oldpro, newbnd, newpro,
     1            oldrwd, newrwd, oldorg, neworg, jnode, invnmi
c     ... create newnum as a permutation vector
      do 100 i = 1, n
          invnmi = invnum (i)
          newnum (invnmi) = i
  100 continue
c compute profile and bandwidth for both the old and the new orderings.
      oldbnd = 0
      oldpro = 0
      newbnd = 0
      newpro = 0
c
      do 300 i = 1, n
          if (degree(i) .eq. 0)  go to 300
          if (degree(i) .gt. 0)  go to 6000
              idgree = -degree(i)
              degree(i) = idgree
              neworg = newnum(i)
              oldorg = oldnum(i)
              newrwd = 0
              oldrwd = 0
              jptr = rstart (i)
c find neighbor which is numbered farthest ahead of the current node.
              do 200 j = 1, idgree
                  jnode = connec(jptr)
                  jptr = jptr + 1
                  newrwd = max0 (newrwd, neworg - newnum(jnode))
                  oldrwd = max0 (oldrwd, oldorg - oldnum(jnode))
  200         continue
c
              newpro = newpro + newrwd
              newbnd = max0 (newbnd, newrwd)
              oldpro = oldpro + oldrwd
              oldbnd = max0 (oldbnd, oldrwd)
  300 continue
c ... if new ordering has better bandwidth than old ordering,
c         replace old ordering by new ordering
      if  (newbnd .gt. oldbnd)  go to 500
          bandwd = newbnd
          profil = newpro
          do 400 i = 1, n
              oldnum(i) = newnum(i)
  400     continue
          go to 600
c ... retain old ordering
  500     bandwd = oldbnd
          profil = oldpro
c
  600 return
 6000 space = -1
      error = 70
      return
      end
c**************************************************************************
      subroutine   gpskcm   (n, degree, rstart, connec, invnum, newnum, 
     1                       oldnum, bandwd, profil, error, space)
      integer     n, rstart(n), bandwd, profil, error, space
      integer     degree(n), connec(n), invnum(n), newnum(n), oldnum(n)
c     compute the bandwidth and profile for the renumbering given
c     by 'invnum', by the reverse of numbering 'invnum', and also
c     by the renumbering given in 'oldnum'.
c     'newnum' will be a permutation vector copy of the node
c     list 'invnum'.
      integer     i, j, jptr, idgree, oldbnd, oldpro, newbnd, newpro,
     1            oldrwd, newrwd, oldorg, neworg, jnode, nrvbnd, nrvpro,
     2            nrvorg, nrvrwd, invnmi, nmip1
c     ... create newnum as a permutation vector
      do 100 i = 1, n
          invnmi = invnum (i)
          newnum (invnmi) = i
  100 continue
c compute profile and bandwidth for both the old and the new orderings.
      oldbnd = 0
      oldpro = 0
      newbnd = 0
      newpro = 0
      nrvbnd = 0
      nrvpro = 0
c
      do 300 i = 1, n
          if (degree(i) .eq. 0)  go to 300
          if (degree(i) .gt. 0)  go to 6000
              idgree = -degree(i)
              degree(i) = idgree
              newrwd = 0
              oldrwd = 0
              nrvrwd = 0
              neworg = newnum(i)
              oldorg = oldnum(i)
              nrvorg = n - newnum(i) + 1
              jptr = rstart (i)
c ... find neighbor which is numbered farthest ahead of the current node.
              do 200 j = 1, idgree
                  jnode = connec(jptr)
                  jptr = jptr + 1
                  newrwd = max0 (newrwd, neworg - newnum(jnode))
                  oldrwd = max0 (oldrwd, oldorg - oldnum(jnode))
                  nrvrwd = max0 (nrvrwd, nrvorg - n + newnum(jnode) - 1)
  200         continue
c
              newpro = newpro + newrwd
              newbnd = max0 (newbnd, newrwd)
              nrvpro = nrvpro + nrvrwd
              nrvbnd = max0 (nrvbnd, nrvrwd)
              oldpro = oldpro + oldrwd
              oldbnd = max0 (oldbnd, oldrwd)
  300 continue
c ... if new ordering has better bandwidth than old ordering,
c         replace old ordering by new ordering
      if  ((newpro .gt. oldpro)  .or. (newpro .gt. nrvpro)) go to 500
          bandwd = newbnd
          profil = newpro
          do 400 i = 1, n
              oldnum(i) = newnum(i)
  400     continue
          go to 800
c ... check new reversed ordering for best profile
  500 if  (nrvpro .gt. oldpro)  go to 700
          bandwd = nrvbnd
          profil = nrvpro
          do 600 i = 1, n
              oldnum(i) = n - newnum(i) + 1
              if  (i .gt. n/2)  go to 600
                  j = invnum(i)
                  nmip1 = (n + 1) - i
                  invnum(i) = invnum (nmip1)
                  invnum (nmip1) = j
  600     continue
          go to 800
c ... retain old ordering
  700     bandwd = oldbnd
          profil = oldpro
c
  800 return
 6000 error = 71
      space = -1
      return
      end
c***************************************************************************
      subroutine   gpskcn   (n, key, data, error)                       
c     i n s e r t i o n    s o r t
c     input:
c         n    -- number of elements to be sorted
c         key  -- an array of length  n  containing the values
c                 which are to be sorted
c         data -- a second array of length  n  containing data
c                 associated with the individual keys.
c     output:
c         key  -- will be arranged so that values are in decreasing
c                 order
c         data -- rearranged to correspond to rearranged keys
c         error -- will be zero unless the program is malfunctioning,
c                  in which case it will be equal to 1.
      integer     n,error,key(n),data(n)
      integer     i, j, d, k, ip1, jm1
c
      if (n .eq. 1)  return
      if  (n .le. 0)  go to 6000
c
      error = 0
c     ... insertion sort ... for i := n-1 step -1 to 1 do ...
 2400 i = n - 1
      ip1 = n
c
 2500     if ( key (i) .ge. key (ip1) )  go to 2800
c             ... out of order ... move up to correct place
              k = key (i)
              d = data (i)
              j = ip1
              jm1 = i
c             ... repeat ... until 'correct place for k found'
 2600             key (jm1) = key (j)
                  data (jm1) = data (j)
                  jm1 = j
                  j = j + 1
                  if  (j .gt. n)  go to 2700
                  if (key (j) .gt. k)  go to 2600
c
 2700         key (jm1) = k
              data (jm1) = d
c
 2800     ip1 = i
          i = i - 1
          if ( i .gt. 0 )  go to 2500
c
 3000 return
c
 6000 error = 1
      go to 3000
c
      end
c**************************************************************************
      subroutine   gpskco   (n, key, error)                             
c     i n s e r t i o n    s o r t
c     input:
c         n    -- number of elements to be sorted
c         key  -- an array of length  n  containing the values
c                 which are to be sorted
c     output:
c         key  -- will be arranged so that values are in decreasing
c                 order
      integer     n, error,key(n)
      integer     i, j, k, ip1, jm1
      if (n .eq. 1)  return
      if  (n .le. 0)  go to 6000
c
      error = 0
c
c     ... insertion sort ... for i := n-1 step -1 to 1 do ...
c
 2400 i = n - 1
      ip1 = n
c
 2500     if ( key (i) .ge. key (ip1) )  go to 2800
c ... out of order ... move up to correct place
              k = key (i)
              j = ip1
              jm1 = i
c ... repeat ... until 'correct place for k found'
 2600             key (jm1) = key (j)
                  jm1 = j
                  j = j + 1
                  if  (j .gt. n)  go to 2700
                  if (key (j) .gt. k)  go to 2600
c
 2700         key (jm1) = k
c
 2800     ip1 = i
          i = i - 1
          if ( i .gt. 0 )  go to 2500
 3000 return
 6000 error = 1
      go to 3000
c
      end
c**************************************************************************
      subroutine  gpskcp  (n, index, nvec, degree, error)               
c     i n s e r t i o n      s o r t
c     input:
c         n    -- number of elements to be sorted
c         index  -- an array of length  n  containing the indices
c                 whose degrees are to be sorted
c         degree -- an  nvec  vector, giving the degrees of nodes
c                   which are to be sorted.
c     output:
c         index  -- will be arranged so that values are in decreasing
c                 order
c         error -- will be zero unless the program is malfunctioning,
c                  in which case it will be equal to 1.
      integer     n, nvec, error,index(n), degree(nvec)
      integer     i, j, v, ip1, jm1, indexi, indxi1, indexj
      if (n .eq. 1)  return
      if  (n .le. 0)  go to 6000
c
      error = 0
c     insertion sort the entire file
c     ... insertion sort ... for i := n-1 step -1 to 1 do ...
c
 2400 i = n - 1
      ip1 = n
c
 2500     indexi = index (i)
          indxi1 = index (ip1)
          if ( degree(indexi) .ge. degree(indxi1) )  go to 2800
c ... out of order ... move up to correct place
              v = degree (indexi)
              j = ip1
              jm1 = i
              indexj = index (j)
c ... repeat ... until 'correct place for v found'
 2600             index (jm1) = indexj
                  jm1 = j
                  j = j + 1
                  if (j .gt. n)  go to 2700
                  indexj = index (j)
                  if (degree(indexj) .gt. v)  go to 2600
c
 2700         index (jm1) = indexi
c
 2800     ip1 = i
          i = i - 1
          if ( i .gt. 0 )  go to 2500
c
 3000 return
 6000 error = 1
      go to 3000
      end
c************************************************************************
      subroutine  gpskcq (n, index, nvec, degree, error)                
c     i n s e r t i o n      s o r t
c     input:
c         n    -- number of elements to be sorted
c         index  -- an array of length  n  containing the indices
c                 whose degrees are to be sorted
c         degree -- an  nvec  vector, giving the degrees of nodes
c                   which are to be sorted.
c     output:
c         index  -- will be arranged so that values are in increasing
c                   order
c         error -- will be zero unless the program is malfunctioning,
c                  in which case it will be equal to 1.
      integer     n, nvec, error,index(n), degree(nvec)
      integer     i, j, v, indexi, indxi1, indexj, ip1, jm1
      if (n .eq. 1)  return
      if  (n .le. 0)  go to 6000
c
      error = 0
c     insertion sort the entire file
c     ... insertion sort ... for i := n-1 step -1 to 1 do ...
c
 2400 i = n - 1
      ip1 = n
c
 2500     indexi = index (i)
          indxi1 = index (ip1)
          if ( degree(indexi) .le. degree(indxi1) )  go to 2800
c ... out of order ... move up to correct place
              v = degree (indexi)
              j = ip1
              jm1 = i
              indexj = index (j)
c             ... repeat ... until 'correct place for v found'
 2600             index (jm1) = indexj
                  jm1 = j
                  j = j + 1
                  if (j .gt. n)  go to 2700
                  indexj = index (j)
                  if (degree(indexj) .lt. v)  go to 2600
 2700         index (jm1) = indexi
 2800     ip1 = i
          i = i - 1
          if ( i .gt. 0 )  go to 2500
 3000 return
 6000 error = 1
      go to 3000
      end
c *************************main main main**************************************
c         gpstrf ..                                                     
c a subroutine which generates the full structure of the graph of a sparse     
c matrix, given the graph of the lower 
c triangle stored by columns  (or upper triangle stored by rows).
c         gpsrpk ..                                                     
c             a subroutine which repacks the connectivity or graph      
c             structure to the original lower triangular form           
c         gpslpk ..                                                     
c             a subroutine which takes the original data and the output 
c             reordering from the gibbs-poole-stockmeyer algorithm, and 
c             produces the required input for the linpack positive      
c             definite band linear equation solvers.  a simple printer  
c             listing of the output data structures is provided.        
c         gpsenv ..                                                     
c             a subroutine which takes the original data, and the output
c             reordering from the gibbs-king algorithm, and produces    
c             a reasonable data structure for a hypothetical envelope   
c             (variable bandwidth) linear equation solver (e.g., the    
c             envelope solver in sparspak).  a simple routine for       
c             printing the output data structures is provided.          
c                                                                       
c     the code assumes symmetric matrices with the lower triangle stored    
c     by columns.
      subroutine bandred(n,nz,liwork,iwork,lrwork,rwork,optpro,error,
     1 iperm,mrepi,iband)
      integer n,nz,error,optpro,iperm(n),mrepi(n),iband(n) 
      integer cstart,connec,invcol,degree,bandwd,         
     1 profil,permut,gpswrk,ireqd,space,gpserr,mxreqd,gpswln
      integer liwork,lrwork,iwork(liwork)
      double precision  rwork(lrwork)
      error=0
      do 1,i=1,n
         iband(i)=1
         mrepi(i)=i
         iperm(i)=i
 1    continue
   10 continue
      if  (error.ne.0)  go to 3000                                           
      cstart = 1                                                        
      permut = cstart + (n + 1)                                         
      connec = permut + n                                               
c     ... build full adjacency (connectivity) structure required for    
c         the reordering routines.  integer workspace is partitioned as 
c             column starts         (   n+1  )  (lower triangle)        
c             permutation from gps  (    n   )                          
c             adjacency structure   (2*nz - n)  (nz = nonzeroes in lower
c                                                triangle and diagonal) 
c             node degrees          (    n   )  (full structure)        
c             gps workspace         ( 6*n + 3)                          
c                                                                       
c         the 2*nz locations consisting of the adjacency structure      
c         and the vector of degrees are remapped into two vectors       
c         of length  nz  by subroutine  gpsrpk, and used in that        
c         form by the routines which reformat the real data.            
      degree = connec + (2*nz - n)                                      
      gpswrk = degree + n                                               
      ireqd = gpswrk + 6*n + 3                                          
      if  (liwork .lt. ireqd)  go to 3100                               
      gpswln = liwork - gpswrk + 1                                      
      call gpstrf(n,iwork(cstart),iwork(connec),iwork(gpswrk),     
     1              iwork(degree), iwork(permut), error)
      if  (error.ne.0)  go to 3000 
      call gpskca (n, iwork(degree), iwork(cstart), iwork(connec),      
     1             optpro, gpswln, iwork(permut), iwork(gpswrk),        
     2             bandwd, profil, gpserr, space)                       
c     
      do 21 i=1,n
         iperm(i)=iwork(permut+i-1)
         mrepi(i)=iwork(gpswrk+i-1)
 21   continue
c !!!!!
      if  (gpserr .ne. 0)  go to 3000                                    
      invcol = connec + nz                                              
      call gpsrpk (n, nz,iwork(cstart),iwork(connec),iwork(invcol),  
     1              error)                                       
      if  (error.ne.0)  go to 3000                                           
c                                                                       
      if  (optpro.eq.1)  go to 100                                           
      mxreqd = (bandwd + 1)  *  n                                       
      if  (mxreqd .gt. lrwork)  go to 4000                              
c                                                                       
        call gpslpk (n,nz,lrwork,bandwd,iwork(cstart),            
     1     iwork(connec),iwork(permut),iwork(invcol),rwork, error)
        iband(1)=bandwd
          go to 200                                                     
  100   call gpsenv (n, nz, profil, lrwork, iwork(connec),           
     1       iwork(cstart), iwork(invcol), iwork(permut),    
     2                  iwork(gpswrk), rwork, error)            
c
        do 22 i=1,n
         iband(i)=iwork(i)
 22   continue
  200 if  (error.ne.0)  go to 3000 
  300 return
 3000 continue
      error=64000
c call erro('program terminated by error')
      go to 300                                                         
 3100 call erro('insufficient workspace for integers')
c     write (6, 63100) ireqd                                       
      go to 300                                                         
 4000 call erro('reordering completed')
      call erro('real workspace insufficient for factorization')
      go to 300                                                         
63100 format (36h0insufficient workspace for integers    /              
     1        17h0require at least , i8)                                
      end                                                               
c************************************************************************
      subroutine gpstrf(n,cstart,connec,cstrt2,degree,permut,error)
c     transform connectivity data from form as lower triangle by columns
c         (upper triangle by rows) to complete connectivity structure.
c
c     this subroutine will check the data to ensure that only the
c     lower triangle is present.  repeated elements are allowed,
c     but no attempt will be made to detect them and reduce storage.
      integer     n,degree(1), connec(1), permut(n)
      integer     cstart(1), cstrt2(1),error
c         ... (actual dimension for cstart and cstrt2 is  n+1.  actual
c              dimension for connec is  2*nz - n)
      integer     i, j, k, kstart, nk, kstop, rk, rj, istart,
     1            jstart, jk
c     ... determine strict lower triangle degree of each node
      do 100 j = 1, n
          degree(j) = cstart(j+1) - cstart(j)
  100 continue
c     ... determine degree in full adjacency structure
      do 400 j = 1, n
          kstart = cstart(j)
          kstop  = cstart(j+1) - 1
          if  (kstart .gt. kstop)  go to 400
          do 300 k = kstart, kstop
              i = connec(k)
              CRES=i - j
              if (CRES .lt. 0) then
                 goto 3100
              elseif (CRES .eq. 0) then
                 goto 300
              else
                 goto 200
              endif
c                         i<j   i=j   i>j
c             ... in lower triangle
  200             if  (i .gt. n)  go to 3000
                      degree(i) = degree(i) + 1
  300     continue
  400 continue
c     ... compute row starts in full adjacency structure
      cstrt2(1) = 1
      if  (n .le. 1)  go to 1000
c
      do 500 i = 2, n
          cstrt2(i) = cstrt2(i-1) + degree(i-1)
  500 continue
c     ... expand lower adjacency structure into space for full adjacency
c         structure.  real workspace is left alone.
c         loops run backward, from last column to first, and last row
c         to first within column, to ensure that no overwriting occurs
c         and that the lower triangle is contiguous as the first group
c         of entries within a column of the full structure.
      do 700 rj = 1, n
          j = n - rj + 1
          jstart = cstrt2 (j)
          kstart = cstart (j)
          nk     = cstart(j+1) - kstart
          if  (nk .le. 0)  go to 700
c
          do 600 rk = 1, nk
              k = kstart + (nk - rk)
              i = connec(k)
              jk = jstart + (nk - rk)
              connec(jk) = i
c             ... build upper triangle
              if  (i .eq. j)  go to 600
              istart = cstrt2(i)
              connec (istart) = j
              cstrt2 (i) = istart + 1
  600     continue
          cstrt2 (j) = jstart + nk
  700 continue
c     ... reset row start pointers for full connectivity structure
c         and initialize permutation vector for gps
      do 800 j = 1, n
          cstart (j) = cstrt2 (j) - degree (j)
          permut (j) = j
  800 continue
      cstart(n+1) = cstart(n) + degree(n)
 1000 return
 3000 call erro('row index out of range')
c write (6, 63000) i, j
      error = 999
      go to 1000
 3100 call erro('data out of proper triangle')
c write (6, 63100) i, j
      error = 999
      go to 1000
63000 format (23h0row index out of range /
     1        4h i =, i8 /  4h j =, i8)
63100 format (30h0data out of proper triangle      /
     1        32h0i  should be greater than  j        /
     2        4h i =, i8 /
     3        4h j =, i8)
c
      end
c************************************************************************
      subroutine  gpsrpk(n,nz,cstart,connec,invcol,error)
c     return connectivity information to original format
c     to match form of real data for nonzero values.
c     this repacking enables us to carry out the physical reordering
c     with only a single copy of the connectivity structure.
      integer  n,nz,invcol(nz),connec(1),cstart(2),error
c     ... actual dimensions of cstart and connec are n+1  and  2*nz - n
c         respectively.  on input, connec contains the connectivity
c         structure of both triangles of the matrix, including one
c         copy of the diagonal.  on output, the first  nz  locations
c         of connec have returned to their original values, giving
c         the lower triangle as a list of row indices grouped by
c         column.  in this package, the vector  invcol  is allowed
c         to overlap the last  nz-n  locations of connec, and contains
c         on output the column indices for the corresponding row
c         indices in connec.
      integer     i, istart, istop, iptr, j
c     ... repack to lower triangle form (eliminating upper triangle)
c         (row indices stored by columns)
      iptr = cstart(1)
      do 200 j = 1, n
          istart = cstart(j)
          istop  = cstart(j+1) - 1
          cstart(j) = iptr
          if  (istop .lt. istart)  go to 200
          do 100 i = istart, istop
              if  (connec(i) .lt. j)  go to 100
                  connec(iptr) = connec(i)
                  iptr = iptr + 1
  100     continue
  200 continue
      cstart(n+1) = iptr
c
      if  (iptr .ne. nz+1)  go to 3000
c store column indices in next nz locations in connec to enable us to
c quickly recover the column index in the repacking of the real data.
      do 400 j = 1, n
          istart = cstart(j)
          istop  = cstart(j+1) - 1
          if  (istop .lt. istart)  go to 400
          do 300 i = istart, istop
              invcol(i) = j
  300     continue
  400 continue
c
 1000 return
c
 3000 call erro('program check in subroutine gpsrpk')
      error = 999
      go to 1000
c
      end
c************************************************************************
      subroutine  gpslpk (n,nz,alen,bandwd,cstart,connec,permut, 
     1                     invcol, a, error)
c     reorder real data into form for input to linpack positive
c     definite symmetric band matrix factorizer and solver
      integer     n, nz, alen, bandwd,cstart(1)
      integer     connec(nz), permut(n), invcol(nz),error
      double precision        a(alen)
c     ... actual dimensions required above
c         cstart -- n+1
c         a      -- n * (bandwd + 1)
c
c         the vector  a  represents, in input, the packed nonzeroes
c         of the lower triangle of the matrix, stored by columns.
c         on output, it is the upper triangle of the matrix, stored
c         by columns in the linpack band format, with  lda = bandwd+1
      integer     i, j, k, kstart, kstop, newi, newj, gnewj, bandp1,
     1            nzp1, laband, xaddr, nxaddr, nextj, nexti
      double precision        x, nextx
c     ... zero out currently unused part of a
      bandp1 = (bandwd + 1)
      laband = bandp1 * n
      if  (laband .gt. alen)  go to 3100
c
      nzp1 = nz + 1
      if  (nzp1 .gt. laband)  go to 200
      do 100 k = nzp1, laband
          a(k) = 0.0
  100 continue
c move entries from packed data structure in front part  of vector  a  into
c their final locations.  if the final  location is occupied (a collision), 
c recursively, move that entry on to its final location, ...
  200 do 800 j = 1, n
          kstart = cstart (j)
          kstop  = cstart (j+1) - 1
          gnewj  = permut (j)
c move all entries in  j-th  column     statement 300 below is return 
c point for an indefinite loop of the form  repeat ... until no collisions
          if  (kstop .lt. kstart)  go to 800
          do 700 k = kstart, kstop
              i = connec(k)
              if  (i .eq. 0)  go to 700
c
              newj = gnewj
              xaddr = k
              x     = a(k)
              a(k)  = 0.0
c
  300         newi  = permut (i)
              connec (xaddr) = 0
              if  (iabs (newi - newj) .gt. bandwd)  go to 3000
c get address of permuted (i,j) - element
              if  (newj .lt. newi)  go to 400
                  nxaddr = bandp1 * newj  +  (newi - newj)
                  go to 500
c
  400             nxaddr = bandp1 * newi  +  (newj - newi)
c             ... check for collison
  500         if  (nxaddr .gt. nz)  go to 600
              nexti = connec (nxaddr)
              if  (nexti .eq. 0)  go to 600
c collision -- target of current nonzero is still occupied. pick up values in
c target location, store current x, and go back to 300 with the new value for x
                  nextx = a(nxaddr)
                  a (nxaddr) = x
                  x     = nextx
c get row and column indices for element with which we collided
                  i     = nexti
                  nextj = invcol(nxaddr)
                  newj  = permut (nextj)
                  xaddr = nxaddr
                  go to 300
c
  600         a (nxaddr) = a (nxaddr)  +  x
c
  700     continue
  800 continue
 1000 return
c     ... error handling for data structure destruction
 3000 continue
      call erro('program failure in subroutine gpslpk')
c write (6, 63000) i, j, newi, newj
      error = 999
      go to 1000
 3100 continue
      call erro('program failure in subroutine gpslpk')
c write (6, 63100) alen, laband
      error = 999
      go to 1000
63000 format (1h0 / 37h0program failure in subroutine gpslpk /
     1        17h0i, j, newi, newj, 4i10)
63100 format (1h0 / 37h0program failure in subroutine gpslpk /
     1        35h0length supplied for reals is only , i8 /
     2        20h0length required is , i8)
c
      end
c************************************************************************
      subroutine  gpsenv  (n, nz, profil, alen, connec, cstart, invcol, 
     1                     permut, xenv, a, error)
c     produce the lower triangle of the envelope matrix resulting
c     from reordering a sparse symmetric matrix with the permutation
c     given by gibbs-poole-stockmeyer-king.
c
c     the lower triangle of the the original matrix is stored by
c     columns, with row indices in 'connec', nonzero values in
c     'a'.  on output, the reordered matrix will be stored with the
c     diagonal entries in the first  n  locations of  a, and the
c     off-diagonal envelope for the lower triangle stored by rows
c     in the next  alen - n  locations of  a.  the vector  cstart
c     will be overwritten to contain the initial entry in the
c     envelope for each row of  a.
c
      integer     n, nz, profil, alen, cstart(1), xenv(1)
      integer     connec(nz), invcol(nz), permut(n),error
      double precision        a(alen)
      integer     i, j, k, newi, newj, lenv, nextj, nxaddr, xaddr,
     1            kstart, kstop, eptr, nzp1, gnewj, nexti
      double precision        x, nextx
c     ... determine envelope structure from permutation
      do 100 i = 1, n
          xenv(i) = 0
  100 continue
c
      do 400 i = 1, n
          newi = permut (i)
          kstart = cstart (i)
          kstop  =  cstart (i+1) - 1
          if  (kstop .lt.  kstart)  go to 400
          do 300 k = kstart, kstop
              j = connec(k)
              newj = permut (j)
              if  (newi .gt. newj)  go to 200
                  xenv(newj) = max0( xenv(newj), newj - newi )
                  go to 300
  200             xenv(newi) = max0( xenv(newi), newi - newj )
  300     continue
  400 continue
c
      lenv = 0
      do 310 i = 1, n
          lenv = lenv + xenv(i)
  310 continue
      if  (profil .ne. lenv)  go to 3100
      if  (profil + n  .gt.  alen)  go to 3000
      if  (xenv(1) .ne. 0)  go to 3200
c     ... reset xenv entries to point to last entry in row
      eptr = n
      do 500 i = 1, n
          eptr = eptr + xenv(i)
          xenv(i) = eptr
  500 continue
c     ... initialize envelope
      nzp1 = nz + 1
      lenv = lenv + n
      if  (nzp1 .gt. lenv)  go to 700
      do 600 i = nzp1, lenv
          a(i) = 0.0
  600 continue
c     ... move entries from packed data structure in front part
c         of vector  a  into their final locations.  if the final
c         location is occupied (a collision), recursively, move that
c         entry on to its final location, ...
  700 do 1500 j = 1, n
          kstart = cstart (j)
          kstop  = cstart (j+1) - 1
          gnewj  = permut (j)
c         ... move all entries in  j-th  column
c             statement 800 below is return point for an indefinite
c             loop of the form  repeat ... until no collisions
          if  (kstop .lt. kstart)  go to 1500
          do 1400 k = kstart, kstop
              i = connec(k)
              if  (i .eq. 0)  go to 1400
c
              newj = gnewj
              xaddr = k
              x     = a(k)
              a(k)  = 0.0
c
  800         newi  = permut (i)
              connec (xaddr) = 0
c             ... get address of permuted (i,j) - element
              CRES=newj - newi
              if (CRES .lt. 0) then
                 goto 900
              elseif (CRES .eq. 0) then
                 goto 1000
              else
                 goto 1100
              endif
c     
  900             nxaddr = xenv(newi) -  (newi - newj - 1)
                  if  (nxaddr .le. xenv(newi-1))  go to 3200
                  go to 1200
c
 1100             nxaddr = xenv(newj) - (newj - newi - 1)
                  if  (nxaddr .le. xenv(newj-1))  go to 3200
c
                  go to 1200
 1000             nxaddr = newi
c             ... check for collison
 1200         if  (nxaddr .gt. nz)  go to 1300
              nexti = connec (nxaddr)
              if  (nexti .eq. 0)  go to 1300
c collision -- target of current nonzero is still occupied. pick up values in
c target location, store current x, and go back to 800 with the new value for x
                  nextx = a(nxaddr)
                  a (nxaddr) = x
                  x     = nextx
c  check row and column indices for element with which we collided
                  i     = nexti
                  nextj = invcol(nxaddr)
                  newj  = permut (nextj)
                  xaddr = nxaddr
                  go to 800
 1300         a (nxaddr) = a (nxaddr)  +  x
 1400     continue
 1500 continue
c     ... reset envelope pointers to beginnings of row
c         with offset of  zero  instead of  n
      do 1600 i = 1, n
          cstart(i+1) = xenv(i) - n + 1
 1600 continue
      cstart(1) = 1
 2000 return
 3000 call erro('length of vector for envelope too short')
c write (6, 63000)
63000 format (40h0length of vector for envelope too short)
      go to 4000
 3100 call erro('internal diagnostic -- program failure')
c write (6, 63100)
63100 format (39h0internal diagnostic -- program failure)
      go to 4000
 3200 call erro('program failure')
c write (6, 63200)
63200 format (34h0program failure, i, j, newi, newj, 4i9)
      go to 4000
 4000 error = 999
      go to 2000
      end
