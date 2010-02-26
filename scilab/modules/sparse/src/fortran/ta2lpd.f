c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) DIGITEO 2010 - Yann COLLETTE
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
