      subroutine ta2lpd(tail,head,ma,n,lp,la,ls)
c
c     ta2lpd computes the adjacency vectors lp, la and ls
c            from vectors tail and head for a directed graph
c     NO CHECKING IS MADE on tail, head and n
c     input: tail(ma) = tail nodes
c            head(ma) = head nodes
c            ma = number of edges
c            n = number of nodes
c     output: lp(n+1) = pointer vector
c             la(ma) = vector of arcs
c             ls(ma) = vector of corresponding head nodes
c
      integer tail(ma),head(ma),ma,n
      integer lp(*),la(ma),ls(ma)
c
      integer iarc,inode
c
c     first computation of lp
c     lp(i+1) = number of tail nodes
c             = number of arcs with tail node i+1
c
      do 1,inode=1,n+1
         lp(inode)=0
 1    continue
      do 2,iarc=1,ma
         lp(tail(iarc)+1)=lp(tail(iarc)+1)+1
 2    continue
c
c     second computation of lp
c     lp(i) = pointer to the first arc
c             with tail i in sorted tail
c
      lp(1)=1
      do 3,inode=2,n
         lp(inode)=lp(inode-1)+lp(inode)
 3    continue
c
c     computation of la and ls
c
      do 4,iarc=1,ma
         inode=tail(iarc)
         la(lp(inode))=iarc
         ls(lp(inode))=head(iarc)
         lp(inode)=lp(inode)+1
 4    continue
c
c     last computation of lp
c
      do 5,inode=n,1,-1
         lp(inode+1)=lp(inode)
 5    continue
      lp(1)=1
      end
c
      subroutine ta2lpu(tail,head,ma,n,lp,la,ls)
c
c     ta2lpu computes the adjacency vectors lp, la and ls
c            from vectors tail and head for an undirected graph
c     NO CHECKING IS MADE on tail, head and n
c     input: tail(ma) = tail nodes
c            head(ma) = head nodes
c            ma = number of edges
c            n = number of nodes
c     output: lp(n+1) = pointer vector
c             la(m) = vector of arcs (m=2*ma)
c             ls(m) = vector of corresponding head nodes
c
      integer tail(ma),head(ma),ma,n
      integer lp(*),la(*),ls(*)
c
      integer iarc,inode
c
c     first computation of lp
c     lp(i+1) = number of tail nodes
c             = number of arcs with tail node i+1
c
      do 1,inode=1,n+1
         lp(inode)=0
 1    continue
      do 2,iarc=1,ma
         lp(tail(iarc)+1)=lp(tail(iarc)+1)+1
         lp(head(iarc)+1)=lp(head(iarc)+1)+1
 2    continue
c
c     second computation of lp
c     lp(i) = pointer to the first arc
c             with tail i in sorted (tail,head)
c
      lp(1)=1
      do 3,inode=2,n
         lp(inode)=lp(inode-1)+lp(inode)
 3    continue
c
c     computation of la and ls
c
      do 4,iarc=1,ma
         inode=tail(iarc)
         la(lp(inode))=iarc
         ls(lp(inode))=head(iarc)
         lp(inode)=lp(inode)+1
         inode=head(iarc)
         la(lp(inode))=iarc
         ls(lp(inode))=tail(iarc)
         lp(inode)=lp(inode)+1
 4    continue
c
c     last computation of lp
c
      do 5,inode=n,1,-1
         lp(inode+1)=lp(inode)
 5    continue
      lp(1)=1
      end
c
      subroutine lp2tad(lp,la,ls,n,tail,head)
c
c     lp2tad computes the vectors tail and head
c            from the adjacency vectors lp, la and ls 
c            for a directed graph
c     NO CHECKING IS MADE on lp, la, ls and n
c     input: lp(n+1) = pointer vector
c            la(ma) = vector of arcs
c            ls(ma) = vector of corresponding head nodes
c            n = number of nodes
c     output: tail(ma) = tail nodes
c             head(ma) = head nodes
c
      integer lp(*),la(*),ls(*),n
      integer tail(*),head(*)
c
      do 1 inod=1,n
         do 2 ip=lp(inod),lp(inod+1)-1
            tail(la(ip))=inod
            head(la(ip))=ls(ip)
 2       continue
 1    continue
      end
c
      subroutine lp2tau(lp,la,ls,n,tail,head)
c
c     lp2tad computes the vectors tail and head
c            from the adjacency vectors lp, la and ls 
c            for an undirected graph
c     NO CHECKING IS MADE on lp, la, ls and n
c     input: lp(n+1) = pointer vector
c            la(ma) = vector of arcs
c            ls(ma) = vector of corresponding head nodes
c            n = number of nodes
c     output: tail(ma) = tail nodes
c             head(ma) = head nodes
c
c
      integer lp(*),la(*),ls(*),n
      integer tail(*),head(*)
c
      integer iarc
c
      do 1,inod=1,n
         do 2,ip=lp(inod),lp(inod+1)-1
            iarc=(la(ip)+1)/2
            tail(iarc)=inod
            head(iarc)=ls(ip)
 2       continue
 1    continue
      end
c
      subroutine findiso(tail,head,ma,n,v)
c
c     findiso finds isolated nodes from tail and head description 
c             of a graph
c     NO CHECKING IS MADE on tail, head and n
c     input: tail(ma) = tail nodes
c            head(ma) = head nodes
c            ma = number of edges
c            n = number of nodes
c     output: v(n) = vector of isolated nodes v(i)=1 is i 
c                    is the number of an isolated node, 0
c                    otherwise
c
      integer tail(ma),head(ma),ma,n,v(n)
c
      integer iarc,inode
c
      do 1,inode=1,n
         v(inode)=0
 1    continue
      do 2,iarc=1,ma
         v(tail(iarc))=1
         v(head(iarc))=1
 2    continue
      end

