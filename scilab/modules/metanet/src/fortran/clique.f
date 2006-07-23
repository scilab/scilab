c partial enumerative algorithm for the maximum clique problem
c      subroutine clique(n,m,madj,clmax,clnod,bestn)
      subroutine clique(n,m,mat,maxclique,actnode,best,edge,start,
     1 last,adj)
      integer    row,col
      integer    mat(n,n),templog
c      logical*1  mat(n,n),templog
      integer    actnode(n),edge(n),temp,adj(m,n)
      integer    start(n),last(n),d,dtemp,best(n)
c     maxm       greater or equal to the actual size of maxclique
c     n          number of nodes in graph
c     mat        adjacency matrix
c     actnode    actual nodes in original matrix
c     adj        represents nodes found at all depths
c     maxclique  size of largest currently know clique
c     best       nodes in largest currently know clique
c     d          current depth of traversal
c     start      node currently being expanded at depth d
c     last       last node to (possibly) be expanded at depth d
      maxclique=1
c     -- maintain pointers to original matrix
      maxclique=0
      do 25 node = 1,n
        actnode(node) = node
        best(node)=0
25    continue
        do 30 row = 1,n
          edge(row) = 0
          do 35 col = 1,n
            if (mat(row,col).eq.1) edge(row) = edge(row)+1
35        continue
30      continue
        do 40 node = 1,n-2
          min = n
          do 45 row = node,n
            if (edge(row).lt.min) then
              min = edge(row)
              minnode = row
            end if
45        continue
          edge(minnode) = edge(node)
          if (node.ne.minnode) then
            temp = actnode(node)
            actnode(node) = actnode(minnode)
            actnode(minnode) = temp
            do 50 row = 1,n
              templog = mat(row,node)
              mat(row,node) = mat(row,minnode)
              mat(row,minnode) = templog
50          continue
            do 55 col = 1,n
              templog = mat(node,col)
              mat(node,col) = mat(minnode,col)
              mat(minnode,col) = templog
55          continue
          end if
          do 60 col = node,n
            if (mat(node,col).eq.1) edge(col) = edge(col) - 1
60        continue
40      continue
c      end if
      d = 1
      start(1) = 0
      last(1) = n
      do 65 col = 1,n
        adj(1,col) = col
65    continue
 
c     -- main algorithm
 
70    start(d) = start(d) + 1
      if ((d+last(d)-start(d)).gt.maxclique) then
        dtemp = d
        d = d + 1
        start(d) = 0
        last(d) = 0
c       -- determine node for next depth
        do 75 col = (start(dtemp)+1),last(dtemp)
          if (mat(adj(dtemp,start(dtemp)),adj(dtemp,col)).eq.1) then
            last(d) = last(d) + 1
            adj(d,last(d)) = adj(dtemp,col)
          end if
75      continue
c       -- if the next depth doesn't contain any nodes, see if a new
c       -- maxclique has been found and return to previous depth
        if (last(d).eq.0.) then
          d = d - 1
          if (d.gt.maxclique) then
            maxclique = d
            do 80 col = 1,d
              best(col) = adj(col,start(col))
80          continue
          end if
        end if
      else
c       -- prune, further expansion would not find a better incumbent
        d = d - 1
      end if
c     -- continue traversal until a depth of zero is reached
      if (d.gt.0.) goto 70
      return 
      end
 

