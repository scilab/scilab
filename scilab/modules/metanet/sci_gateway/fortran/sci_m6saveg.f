
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c
c SCILAB function : m6saveg, fin = 2
       subroutine intsm6saveg(fname)
c
       character*(*) fname
       include 'stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getilist,getlistscalar,getlistvectrow,g
     $ etlistsimat,getsmat,checkval,getscalar,bufstore,crestringv
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,4,4)) return
       if(.not.checklhs(fname,1,1)) return
       if(.not.getilist(fname,topk,top-rhs+1,n1,1,il1)) return
c      
c       --   list element number 3 directed(g) --
       if(.not.getlistscalar(fname,topk,top-rhs+1,3,lr1e3)) return
c      
c       --   list element number 4 node_number(g) --
       if(.not.getlistscalar(fname,topk,top-rhs+1,4,lr1e4)) return
c      
c       --   list element number 5 tail(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,5,it1e5,m1e5,n1e5,lr1
     $ e5,lc1e5)) return
c      
c       --   list element number 6 head(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,6,it1e6,m1e6,n1e6,lr1
     $ e6,lc1e6)) return
c      
c       --   list element number 7 node_name(g) --
       if(.not.getlistsimat(fname,topk,top-rhs+1,7,m1e7,n1e7,1,1,lr1e7,n
     $ lr1e7)) return
c      
c       --   list element number 8 node_type(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,8,it1e8,m1e8,n1e8,lr1
     $ e8,lc1e8)) return
c      
c       --   list element number 9 node_x(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,9,it1e9,m1e9,n1e9,lr1
     $ e9,lc1e9)) return
c      
c       --   list element number 10 node_y(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,10,it1e10,m1e10,n1e10
     $ ,lr1e10,lc1e10)) return
c      
c       --   list element number 11 node_color(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,11,it1e11,m1e11,n1e11
     $ ,lr1e11,lc1e11)) return
c      
c       --   list element number 12 node_diam(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,12,it1e12,m1e12,n1e12
     $ ,lr1e12,lc1e12)) return
c      
c       --   list element number 13 node_border(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,13,it1e13,m1e13,n1e13
     $ ,lr1e13,lc1e13)) return
c      
c       --   list element number 14 node_font_size(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,14,it1e14,m1e14,n1e14
     $ ,lr1e14,lc1e14)) return
c      
c       --   list element number 15 node_demand(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,15,it1e15,m1e15,n1e15
     $ ,lr1e15,lc1e15)) return
c      
c       --   list element number 16 edge_name(g) --
       if(.not.getlistsimat(fname,topk,top-rhs+1,16,m1e16,n1e16,1,1,lr1e
     $ 16,nlr1e16)) return
c      
c       --   list element number 17 edge_color(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,17,it1e17,m1e17,n1e17
     $ ,lr1e17,lc1e17)) return
c      
c       --   list element number 18 edge_width(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,18,it1e18,m1e18,n1e18
     $ ,lr1e18,lc1e18)) return
c      
c       --   list element number 19 edge_hi_width(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,19,it1e19,m1e19,n1e19
     $ ,lr1e19,lc1e19)) return
c      
c       --   list element number 20 edge_font_size(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,20,it1e20,m1e20,n1e20
     $ ,lr1e20,lc1e20)) return
c      
c       --   list element number 21 edge_length(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,21,it1e21,m1e21,n1e21
     $ ,lr1e21,lc1e21)) return
c      
c       --   list element number 22 edge_cost(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,22,it1e22,m1e22,n1e22
     $ ,lr1e22,lc1e22)) return
c      
c       --   list element number 23 edge_min_cap(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,23,it1e23,m1e23,n1e23
     $ ,lr1e23,lc1e23)) return
c      
c       --   list element number 24 edge_max_cap(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,24,it1e24,m1e24,n1e24
     $ ,lr1e24,lc1e24)) return
c      
c       --   list element number 25 edge_q_weight(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,25,it1e25,m1e25,n1e25
     $ ,lr1e25,lc1e25)) return
c      
c       --   list element number 26 edge_q_orig(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,26,it1e26,m1e26,n1e26
     $ ,lr1e26,lc1e26)) return
c      
c       --   list element number 27 edge_weight(g) --
       if(.not.getlistvectrow(fname,topk,top-rhs+1,27,it1e27,m1e27,n1e27
     $ ,lr1e27,lc1e27)) return
c      
c       --   list element number 28 default_node_diam(g) --
       if(.not.getlistscalar(fname,topk,top-rhs+1,28,lr1e28)) return
c      
c       --   list element number 29 default_node_border(g) --
       if(.not.getlistscalar(fname,topk,top-rhs+1,29,lr1e29)) return
c      
c       --   list element number 30 default_edge_width(g) --
       if(.not.getlistscalar(fname,topk,top-rhs+1,30,lr1e30)) return
c      
c       --   list element number 31 default_edge_hi_width(g) --
       if(.not.getlistscalar(fname,topk,top-rhs+1,31,lr1e31)) return
c      
c       --   list element number 32 default_font_size(g) --
       if(.not.getlistscalar(fname,topk,top-rhs+1,32,lr1e32)) return
c       checking variable name (number 2)
c       
       if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2)) return
       if(.not.checkval(fname,m2*n2,1)) return
c       checking variable ma (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable datanet (number 4)
c       
       if(.not.getsmat(fname,top,top-rhs+4,m4,n4,1,1,lr4,nlr4)) return
       if(.not.checkval(fname,m4*n4,1)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1e5,n1e6)) return
       if(.not.checkval(fname,n1e5,n1e16)) return
       if(.not.checkval(fname,n1e5,n1e17)) return
       if(.not.checkval(fname,n1e5,n1e18)) return
       if(.not.checkval(fname,n1e5,n1e19)) return
       if(.not.checkval(fname,n1e5,n1e20)) return
       if(.not.checkval(fname,n1e5,n1e21)) return
       if(.not.checkval(fname,n1e5,n1e22)) return
       if(.not.checkval(fname,n1e5,n1e23)) return
       if(.not.checkval(fname,n1e5,n1e24)) return
       if(.not.checkval(fname,n1e5,n1e25)) return
       if(.not.checkval(fname,n1e5,n1e26)) return
       if(.not.checkval(fname,n1e5,n1e27)) return
       if(.not.checkval(fname,m1e7,m1e16)) return
       if(.not.checkval(fname,n1e7,n1e8)) return
       if(.not.checkval(fname,n1e7,n1e9)) return
       if(.not.checkval(fname,n1e7,n1e10)) return
       if(.not.checkval(fname,n1e7,n1e11)) return
       if(.not.checkval(fname,n1e7,n1e12)) return
       if(.not.checkval(fname,n1e7,n1e13)) return
       if(.not.checkval(fname,n1e7,n1e14)) return
       if(.not.checkval(fname,n1e7,n1e15)) return
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr4,nlr4)) return
       if(.not.bufstore(fname,lbuf,lbufi3,lbuff3,lr2,nlr2)) return
       call entier(1,stk(lr1e3),istk(iadr(lr1e3)))
       call entier(1,stk(lr1e4),istk(iadr(lr1e4)))
       call entier(n1e5,stk(lr1e5),istk(iadr(lr1e5)))
       call entier(n1e6,stk(lr1e6),istk(iadr(lr1e6)))
       if(.not.crestringv(fname,top+1,lr1e7-5-m1e7*n1e7,lw1e7)) return
       call entier(n1e8,stk(lr1e8),istk(iadr(lr1e8)))
       call entier(n1e9,stk(lr1e9),istk(iadr(lr1e9)))
       call entier(n1e10,stk(lr1e10),istk(iadr(lr1e10)))
       call entier(n1e11,stk(lr1e11),istk(iadr(lr1e11)))
       call entier(n1e12,stk(lr1e12),istk(iadr(lr1e12)))
       call entier(n1e13,stk(lr1e13),istk(iadr(lr1e13)))
       call entier(n1e14,stk(lr1e14),istk(iadr(lr1e14)))
       if(.not.crestringv(fname,top+2,lr1e16-5-m1e16*n1e16,lw1e16)) retu
     $ rn
       call entier(n1e17,stk(lr1e17),istk(iadr(lr1e17)))
       call entier(n1e18,stk(lr1e18),istk(iadr(lr1e18)))
       call entier(n1e19,stk(lr1e19),istk(iadr(lr1e19)))
       call entier(n1e20,stk(lr1e20),istk(iadr(lr1e20)))
       call entier(1,stk(lr1e28),istk(iadr(lr1e28)))
       call entier(1,stk(lr1e29),istk(iadr(lr1e29)))
       call entier(1,stk(lr1e30),istk(iadr(lr1e30)))
       call entier(1,stk(lr1e31),istk(iadr(lr1e31)))
       call entier(1,stk(lr1e32),istk(iadr(lr1e32)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call saveg(buf(lbufi1:lbuff1),nlr4,buf(lbufi3:lbuff3),nlr2,istk(i
     $ adr(lr1e3)),istk(iadr(lr1e4)),istk(iadr(lr1e5)),istk(iadr(lr1e6))
     $ ,stk(lw1e7),istk(iadr(lr1e8)),istk(iadr(lr1e9)),istk(iadr(lr1e10)
     $ ),istk(iadr(lr1e11)),istk(iadr(lr1e12)),istk(iadr(lr1e13)),istk(i
     $ adr(lr1e14)),stk(lr1e15),stk(lw1e16),istk(iadr(lr1e17)),istk(iadr
     $ (lr1e18)),istk(iadr(lr1e19)),istk(iadr(lr1e20)),stk(lr1e21),stk(l
     $ r1e22),stk(lr1e23),stk(lr1e24),stk(lr1e25),stk(lr1e26),stk(lr1e27
     $ ),istk(iadr(lr1e28)),istk(iadr(lr1e29)),istk(iadr(lr1e30)),istk(i
     $ adr(lr1e31)),istk(iadr(lr1e32)),istk(iadr(lr3)))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+2
c       no output variable
       top=topk+1
       call objvide(fname,top)
       return
       end
c
