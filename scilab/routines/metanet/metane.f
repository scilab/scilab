c SCILAB function : m6loadg, fin = 1
       subroutine intsm6loadg(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getsmat,checkval,bufstore,crepointer,cr
     $ emat,listcrestring,listcremat,lcrestringmatfromC
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable path (number 1)
c       
       if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
       if(.not.checkval(fname,m1*n1,1)) return
c     
c       cross variable size checking
c     
       if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
       if(.not.crepointer(fname,top+1,lw3)) return
       if(.not.cremat(fname,top+2,0,1,1,lw5,loc5)) return
       if(.not.cremat(fname,top+3,0,1,1,lw6,loc6)) return
       if(.not.cremat(fname,top+4,0,1,1,lw7,loc7)) return
       if(.not.cremat(fname,top+5,0,1,1,lw8,loc8)) return
       mm9=1
       if(.not.crepointer(fname,top+6,lw9)) return
       if(.not.cremat(fname,top+7,0,1,1,lw10,loc10)) return
       if(.not.cremat(fname,top+8,0,1,1,lw11,loc11)) return
       if(.not.cremat(fname,top+9,0,1,1,lw12,loc12)) return
       if(.not.cremat(fname,top+10,0,1,1,lw13,loc13)) return
       if(.not.cremat(fname,top+11,0,1,1,lw14,loc14)) return
       if(.not.cremat(fname,top+12,0,1,1,lw15,loc15)) return
       if(.not.cremat(fname,top+13,0,1,1,lw16,loc16)) return
       if(.not.cremat(fname,top+14,0,1,1,lw17,loc17)) return
       if(.not.crepointer(fname,top+15,lw18)) return
       if(.not.cremat(fname,top+16,0,1,1,lw19,loc19)) return
       if(.not.cremat(fname,top+17,0,1,1,lw20,loc20)) return
       if(.not.cremat(fname,top+18,0,1,1,lw21,loc21)) return
       if(.not.cremat(fname,top+19,0,1,1,lw22,loc22)) return
       if(.not.cremat(fname,top+20,0,1,1,lw23,loc23)) return
       if(.not.cremat(fname,top+21,0,1,1,lw24,loc24)) return
       if(.not.cremat(fname,top+22,0,1,1,lw25,loc25)) return
       if(.not.cremat(fname,top+23,0,1,1,lw26,loc26)) return
       if(.not.cremat(fname,top+24,0,1,1,lw27,loc27)) return
       if(.not.cremat(fname,top+25,0,1,1,lw28,loc28)) return
       if(.not.cremat(fname,top+26,0,1,1,lw29,loc29)) return
       if(.not.cremat(fname,top+27,0,1,1,lw30,loc30)) return
       if(.not.cremat(fname,top+28,0,1,1,lw31,loc31)) return
       if(.not.cremat(fname,top+29,0,1,1,lw32,loc32)) return
       if(.not.cremat(fname,top+30,0,1,1,lw33,loc33)) return
       if(.not.cremat(fname,top+31,0,1,1,lw34,loc34)) return
       call loadg(buf(lbufi1:lbuff1),nlr1,stk(lw3),ne3,stk(lw5),stk(lw6)
     $ ,stk(lw7),stk(lw8),stk(lw9),stk(lw10),stk(lw11),stk(lw12),stk(lw1
     $ 3),stk(lw14),stk(lw15),stk(lw16),stk(lw17),stk(lw18),stk(lw19),st
     $ k(lw20),stk(lw21),stk(lw22),stk(lw23),stk(lw24),stk(lw25),stk(lw2
     $ 6),stk(lw27),stk(lw28),stk(lw29),stk(lw30),stk(lw31),stk(lw32),st
     $ k(lw33),stk(lw34),ne13,ne7)
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+31
c       Creation of output list
       top=topl+1
       call crelist(top,31,lw)
c     
c       Element 1: name
       if(.not.listcrestring(fname,top,1,lw,ne3,ilrs)) return
       call cchar(ne3,stk(lw3),istk(ilrs))
c     
c       Element 2: directed
       if(.not.listcremat(fname,top,2,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw5)),-1,stk(lrs),-1)
c     
c       Element 3: n
       if(.not.listcremat(fname,top,3,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw6)),-1,stk(lrs),-1)
c     
c       Element 4: tail
       if(.not.listcremat(fname,top,4,lw,0,1,ne7,lrs,lcs)) return
       call cintf(1*ne7,stk(lw7),stk(lrs))
c     
c       Element 5: head
       if(.not.listcremat(fname,top,5,lw,0,1,ne7,lrs,lcs)) return
       call cintf(1*ne7,stk(lw8),stk(lrs))
c     
c       Element 6: node_name
       if(.not.lcrestringmatfromC(fname,top,6,lw,lw9,mm9,ne13)) return
c     
c       Element 7: node_type
       if(.not.listcremat(fname,top,7,lw,0,1,ne13,lrs,lcs)) return
       call cintf(1*ne13,stk(lw10),stk(lrs))
c     
c       Element 8: node_x
       if(.not.listcremat(fname,top,8,lw,0,1,ne13,lrs,lcs)) return
       call cintf(1*ne13,stk(lw11),stk(lrs))
c     
c       Element 9: node_y
       if(.not.listcremat(fname,top,9,lw,0,1,ne13,lrs,lcs)) return
       call cintf(1*ne13,stk(lw12),stk(lrs))
c     
c       Element 10: node_color
       if(.not.listcremat(fname,top,10,lw,0,1,ne13,lrs,lcs)) return
       call cintf(1*ne13,stk(lw13),stk(lrs))
c     
c       Element 11: node_diam
       if(.not.listcremat(fname,top,11,lw,0,1,ne13,lrs,lcs)) return
       call cintf(1*ne13,stk(lw14),stk(lrs))
c     
c       Element 12: node_border
       if(.not.listcremat(fname,top,12,lw,0,1,ne13,lrs,lcs)) return
       call cintf(1*ne13,stk(lw15),stk(lrs))
c     
c       Element 13: node_font_size
       if(.not.listcremat(fname,top,13,lw,0,1,ne13,lrs,lcs)) return
       call cintf(1*ne13,stk(lw16),stk(lrs))
c     
c       Element 14: node_demand
       if(.not.listcremat(fname,top,14,lw,0,1,ne13,lrs,lcs)) return
       call cdoublef(1*ne13,stk(lw17),stk(lrs))
c     
c       Element 15: edge_name
       if(.not.lcrestringmatfromC(fname,top,15,lw,lw18,mm9,ne7)) return
c     
c       Element 16: edge_color
       if(.not.listcremat(fname,top,16,lw,0,1,ne7,lrs,lcs)) return
       call cintf(1*ne7,stk(lw19),stk(lrs))
c     
c       Element 17: edge_width
       if(.not.listcremat(fname,top,17,lw,0,1,ne7,lrs,lcs)) return
       call cintf(1*ne7,stk(lw20),stk(lrs))
c     
c       Element 18: edge_hi_width
       if(.not.listcremat(fname,top,18,lw,0,1,ne7,lrs,lcs)) return
       call cintf(1*ne7,stk(lw21),stk(lrs))
c     
c       Element 19: edge_font_size
       if(.not.listcremat(fname,top,19,lw,0,1,ne7,lrs,lcs)) return
       call cintf(1*ne7,stk(lw22),stk(lrs))
c     
c       Element 20: edge_length
       if(.not.listcremat(fname,top,20,lw,0,1,ne7,lrs,lcs)) return
       call cdoublef(1*ne7,stk(lw23),stk(lrs))
c     
c       Element 21: edge_cost
       if(.not.listcremat(fname,top,21,lw,0,1,ne7,lrs,lcs)) return
       call cdoublef(1*ne7,stk(lw24),stk(lrs))
c     
c       Element 22: edge_min_cap
       if(.not.listcremat(fname,top,22,lw,0,1,ne7,lrs,lcs)) return
       call cdoublef(1*ne7,stk(lw25),stk(lrs))
c     
c       Element 23: edge_max_cap
       if(.not.listcremat(fname,top,23,lw,0,1,ne7,lrs,lcs)) return
       call cdoublef(1*ne7,stk(lw26),stk(lrs))
c     
c       Element 24: edge_q_weight
       if(.not.listcremat(fname,top,24,lw,0,1,ne7,lrs,lcs)) return
       call cdoublef(1*ne7,stk(lw27),stk(lrs))
c     
c       Element 25: edge_q_orig
       if(.not.listcremat(fname,top,25,lw,0,1,ne7,lrs,lcs)) return
       call cdoublef(1*ne7,stk(lw28),stk(lrs))
c     
c       Element 26: edge_weight
       if(.not.listcremat(fname,top,26,lw,0,1,ne7,lrs,lcs)) return
       call cdoublef(1*ne7,stk(lw29),stk(lrs))
c     
c       Element 27: default_node_diam
       if(.not.listcremat(fname,top,27,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw30)),-1,stk(lrs),-1)
c     
c       Element 28: default_node_border
       if(.not.listcremat(fname,top,28,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw31)),-1,stk(lrs),-1)
c     
c       Element 29: default_edge_width
       if(.not.listcremat(fname,top,29,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw32)),-1,stk(lrs),-1)
c     
c       Element 30: default_edge_hi_width
       if(.not.listcremat(fname,top,30,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw33)),-1,stk(lrs),-1)
c     
c       Element 31: default_font_size
       if(.not.listcremat(fname,top,31,lw,0,1,1,lrs,lcs)) return
       call int2db(1*1,istk(iadr(lw34)),-1,stk(lrs),-1)
c     
c     Putting in order the stack
       call copyobj(fname,topl+1,topk+1)
       top=topk+1
       return
       end
c
c SCILAB function : m6saveg, fin = 2
       subroutine intsm6saveg(fname)
c
       character*(*) fname
       include '../stack.h'
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
c SCILAB function : m6prevn2p, fin = 3
       subroutine intsm6prevn2p(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable i (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable j (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable pln (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable la (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable lp (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable ls (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable direct (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n4,n6)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(1,stk(lr7),istk(iadr(lr7)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+1,0,1,1,lw10,loc10)) return
       call prevn2p(istk(iadr(lr1)),istk(iadr(lr2)),n4,n3,istk(iadr(lr4)
     $ ),istk(iadr(lr5)),istk(iadr(lr6)),istk(iadr(lr7)),istk(iadr(lr3))
     $ ,stk(lw10),ne11)
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: p
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne11,lrs,lcs)) return
        call cintf(1*ne11,stk(lw10),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6ns2p, fin = 4
       subroutine intsm6ns2p(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,5,5)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable ns (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable la (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable lp (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable ls (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable n (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n4)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       if(.not.cremat(fname,top+1,0,1,1,lw3,loc3)) return
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       call ns2p(istk(iadr(lr1)),n1,stk(lw3),ne9,istk(iadr(lr2)),istk(ia
     $ dr(lr3)),istk(iadr(lr4)),istk(iadr(lr5)))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: p
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne9,lrs,lcs)) return
        call cintf(1*ne9,stk(lw3),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6p2ns, fin = 5
       subroutine intsm6p2ns(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,6,6)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable p (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable la (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable lp (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable ls (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable direct (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable n (number 6)
c       
       if(.not.getscalar(fname,top,top-rhs+6,lr6)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n4)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       if(.not.cremat(fname,top+1,0,1,1,lw3,loc3)) return
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr6),istk(iadr(lr6)))
       call p2ns(istk(iadr(lr1)),n1,stk(lw3),ne10,istk(iadr(lr2)),istk(i
     $ adr(lr3)),istk(iadr(lr4)),istk(iadr(lr5)),n2,istk(iadr(lr6)))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: ns
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne10,lrs,lcs)) return
        call cintf(1*ne10,stk(lw3),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6edge2st, fin = 6
       subroutine intsm6edge2st(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,1,1)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable alpha (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c     
c       cross variable size checking
c     
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       if(.not.cremat(fname,top+1,0,1,1,lw3,loc3)) return
       call edge2st(n1,istk(iadr(lr1)),stk(lw3),ne3)
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: tree
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne3,lrs,lcs)) return
        call cintf(1*ne3,stk(lw3),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6prevn2st, fin = 7
       subroutine intsm6prevn2st(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,4,4)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable nodes (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable la (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable lp (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable ls (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n4)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       if(.not.cremat(fname,top+1,0,1,1,lw3,loc3)) return
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call prevn2st(n1,istk(iadr(lr1)),stk(lw3),ne8,istk(iadr(lr2)),ist
     $ k(iadr(lr3)),istk(iadr(lr4)))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: tree
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne8,lrs,lcs)) return
        call cintf(1*ne8,stk(lw3),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6compc, fin = 8
       subroutine intsm6compc(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable lp (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable ls (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable n (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c     
c       cross variable size checking
c     
       if(.not.cremat(fname,top+1,0,1,1,lw1,loc1)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+2,0,istk(iadr(lr3)),1,lw6,loc6)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr3)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+4,0,istk(iadr(lr3)),1,lw8,loc8)) return
       call compc(stk(lw1),istk(iadr(lr1)),istk(iadr(lr2)),n2,istk(iadr(
     $ lr3)),stk(lw6),stk(lw7),stk(lw8))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+4
c     
       if(lhs .ge. 1) then
c       --------------output variable: l
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw1)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: ncomp
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr3)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr3)),istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6concom, fin = 9
       subroutine intsm6concom(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,2,2)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable icomp (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable ncomp (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       if(.not.cremat(fname,top+1,0,1,1,lw4,loc4)) return
       call concom(istk(iadr(lr1)),n2,istk(iadr(lr2)),stk(lw4),ne4)
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: ns
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne4,lrs,lcs)) return
        call cintf(1*ne4,stk(lw4),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6compfc, fin = 10
       subroutine intsm6compfc(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable lp (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable ls (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable n (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c     
c       cross variable size checking
c     
       nn1= int(stk(lr3))
       if(.not.cremat(fname,top+1,0,nn1,1,lw1,loc1)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+2,0,1,1,lw6,loc6)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr3)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+4,0,istk(iadr(lr3)),1,lw8,loc8)) return
       if(.not.cremat(fname,top+5,0,istk(iadr(lr3)),1,lw9,loc9)) return
       if(.not.cremat(fname,top+6,0,istk(iadr(lr3)),1,lw10,loc10)) retur
     $ n
       if(.not.cremat(fname,top+7,0,istk(iadr(lr3)),1,lw11,loc11)) retur
     $ n
       call compfc(stk(lw1),istk(iadr(lr1)),istk(iadr(lr2)),n2,istk(iadr
     $ (lr3)),stk(lw6),stk(lw7),stk(lw8),stk(lw9),stk(lw10),stk(lw11))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+7
c     
       if(lhs .ge. 1) then
c       --------------output variable: nc
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: nfcomp
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr3)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr3)),istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6sconcom, fin = 11
       subroutine intsm6sconcom(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,2,2)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable icomp (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable nfcomp (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       if(.not.cremat(fname,top+1,0,1,1,lw4,loc4)) return
       call sconcom(istk(iadr(lr1)),n2,istk(iadr(lr2)),stk(lw4),ne4)
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: ns
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne4,lrs,lcs)) return
        call cintf(1*ne4,stk(lw4),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6pcchna, fin = 12
       subroutine intsm6pcchna(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,4,4)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable i0 (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable lp (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable ls (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr4)),1,lw6,loc6)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr4)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr4)),1,lw8,loc8)) return
       call pcchna(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),n3,is
     $ tk(iadr(lr4)),stk(lw6),stk(lw7),stk(lw8))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: pani
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr4)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr4)),istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: pan
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr4)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr4)),istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6fordfulk, fin = 13
       subroutine intsm6fordfulk(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,3)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable ma (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable source (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable sink (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable u (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable tail (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable head (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n5,n6)) return
       if(.not.checkval(fname,n5,n7)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr2)),1,lw6,loc6)) return
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       if(.not.cremat(fname,top+2,0,istk(iadr(lr1)),1,lw9,loc9)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw10,loc10)) retur
     $ n
       if(.not.cremat(fname,top+4,0,istk(iadr(lr1)),1,lw11,loc11)) retur
     $ n
       if(.not.cremat(fname,top+5,0,istk(iadr(lr2)),1,lw12,loc12)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr2)),1,lw13,loc13)) retur
     $ n
       if(.not.cremat(fname,top+7,0,istk(iadr(lr1)),1,lw14,loc14)) retur
     $ n
       if(.not.cremat(fname,top+8,0,1,1,lw15,loc15)) return
       if(.not.cremat(fname,top+9,0,1,1,lw16,loc16)) return
       if(.not.cremat(fname,top+10,0,istk(iadr(lr1)),1,lw17,loc17)) retu
     $ rn
       if(.not.cremat(fname,top+11,0,istk(iadr(lr1)),1,lw18,loc18)) retu
     $ rn
       if(.not.cremat(fname,top+12,0,1,1,lw19,loc19)) return
       call fordfulk(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),ist
     $ k(iadr(lr4)),istk(iadr(lr5)),stk(lw6),istk(iadr(lr6)),istk(iadr(l
     $ r7)),stk(lw9),stk(lw10),stk(lw11),stk(lw12),stk(lw13),stk(lw14),s
     $ tk(lw15),stk(lw16),stk(lw17),stk(lw18),stk(lw19))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+12
c     
       if(lhs .ge. 1) then
c       --------------output variable: maxflow
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw16)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: f
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: ierr
        top=topl+3
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw19)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6johns, fin = 14
       subroutine intsm6johns(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,6,6)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable i0 (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable la (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable length (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable lp (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable ls (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable n (number 6)
c       
       if(.not.getscalar(fname,top,top-rhs+6,lr6)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n5)) return
       nn1= int(stk(lr6))
       if(.not.cremat(fname,top+1,0,nn1,1,lw1,loc1)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       if(.not.cremat(fname,top+2,0,nn1,1,lw3,loc3)) return
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr6),istk(iadr(lr6)))
       if(.not.cremat(fname,top+3,0,istk(iadr(lr6)),1,lw10,loc10)) retur
     $ n
       if(.not.cremat(fname,top+4,0,istk(iadr(lr6)),1,lw11,loc11)) retur
     $ n
       call johns(stk(lw1),istk(iadr(lr1)),stk(lw3),istk(iadr(lr2)),stk(
     $ lr3),istk(iadr(lr4)),istk(iadr(lr5)),n2,istk(iadr(lr6)),stk(lw10)
     $ ,stk(lw11))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+4
c     
       if(lhs .ge. 1) then
c       --------------output variable: pi
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr6)),lrs,lcs)) return
        call dcopy(1*istk(iadr(lr6)),stk(lw11),1,stk(lrs),1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: p
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr6)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr6)),istk(iadr(lw10)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6dijkst, fin = 15
       subroutine intsm6dijkst(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,6,6)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable i0 (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable la (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable length (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable lp (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable ls (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable n (number 6)
c       
       if(.not.getscalar(fname,top,top-rhs+6,lr6)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n5)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr6),istk(iadr(lr6)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr6)),1,lw8,loc8)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr6)),1,lw9,loc9)) return
       call dijkst(istk(iadr(lr1)),istk(iadr(lr2)),stk(lr3),istk(iadr(lr
     $ 4)),istk(iadr(lr5)),n2,istk(iadr(lr6)),stk(lw8),stk(lw9))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: pi
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr6)),lrs,lcs)) return
        call dcopy(1*istk(iadr(lr6)),stk(lw9),1,stk(lrs),1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: p
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr6)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr6)),istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6frang, fin = 16
       subroutine intsm6frang(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable lp (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable ls (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable n (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c     
c       cross variable size checking
c     
       if(.not.cremat(fname,top+1,0,1,1,lw1,loc1)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+2,0,istk(iadr(lr3)),1,lw6,loc6)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr3)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+4,0,istk(iadr(lr3)),1,lw8,loc8)) return
       call frang(stk(lw1),istk(iadr(lr1)),istk(iadr(lr2)),n2,istk(iadr(
     $ lr3)),stk(lw6),stk(lw7),stk(lw8))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+4
c     
       if(lhs .ge. 1) then
c       --------------output variable: i0
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw1)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: rang
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr3)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr3)),istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6chcm, fin = 17
       subroutine intsm6chcm(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,6,6)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable i0 (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable la (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable lp (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable ls (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable n (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable maxcap (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n4)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr5)),1,lw8,loc8)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr5)),1,lw9,loc9)) return
       call chcm(stk(lr6),istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)
     $ ),istk(iadr(lr4)),n2,istk(iadr(lr5)),stk(lw8),stk(lw9))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: pcapi
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr5)),lrs,lcs)) return
        call dcopy(1*istk(iadr(lr5)),stk(lw9),1,stk(lrs),1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: pcap
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr5)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr5)),istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6transc, fin = 18
       subroutine intsm6transc(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable lp (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable ls (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable n (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c     
c       cross variable size checking
c     
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       if(.not.cremat(fname,top+1,0,1,1,lw2,loc2)) return
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       if(.not.cremat(fname,top+2,0,1,1,lw4,loc4)) return
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call transc(istk(iadr(lr1)),stk(lw2),istk(iadr(lr2)),stk(lw4),n2,
     $ ne8,ne6,istk(iadr(lr3)))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: lpft
        top=topl+1
        if(.not.cremat(fname,top,0,1,ne6,lrs,lcs)) return
        call cintf(1*ne6,stk(lw2),stk(lrs))
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: lsft
        top=topl+2
        if(.not.cremat(fname,top,0,1,ne8,lrs,lcs)) return
        call cintf(1*ne8,stk(lw4),stk(lrs))
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6dfs, fin = 19
       subroutine intsm6dfs(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,4,4)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable i0 (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable lp (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable ls (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr4)),1,lw6,loc6)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr4)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr4)),1,lw8,loc8)) return
       call dfs(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),n3,istk(
     $ iadr(lr4)),stk(lw6),stk(lw7),stk(lw8))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: num
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr4)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr4)),istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: pw
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr4)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr4)),istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6umtree, fin = 20
       subroutine intsm6umtree(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,5,5)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable la (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable lp (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable ls (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable weight (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1,n3)) return
       nn1= int(stk(lr4))
       if(.not.cremat(fname,top+1,0,nn1,1,lw1,loc1)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+2,0,istk(iadr(lr4)),1,lw8,loc8)) return
       call prim(stk(lw1),istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)
     $ ),n5,n1,istk(iadr(lr4)),stk(lw8),stk(lr5))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: alpha
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr4)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr4)),istk(iadr(lw1)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6umtree1, fin = 21
       subroutine intsm6umtree1(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,5,5)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable la (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable lp (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable ls (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable weight (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1,n3)) return
       nn1= int(stk(lr4))
       if(.not.cremat(fname,top+1,0,nn1,1,lw1,loc1)) return
       if(.not.cremat(fname,top+2,0,nn1,1,lw2,loc2)) return
       if(.not.cremat(fname,top+3,0,nn1,1,lw3,loc3)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+4,0,istk(iadr(lr4)),1,lw10,loc10)) retur
     $ n
       call prim1(stk(lw1),stk(lw2),stk(lw3),istk(iadr(lr1)),istk(iadr(l
     $ r2)),istk(iadr(lr3)),n5,n1,istk(iadr(lr4)),stk(lw10),stk(lr5))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+4
c     
       if(lhs .ge. 1) then
c       --------------output variable: alpha
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr4)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr4)),istk(iadr(lw1)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6dmtree, fin = 22
       subroutine intsm6dmtree(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,6,6)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable i0 (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable la (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable lp (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable ls (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable n (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable weight (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n4)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr5)),1,lw7,loc7)) return
       call dmtree(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(
     $ iadr(lr4)),n2,istk(iadr(lr5)),stk(lw7),stk(lr6))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: pred
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr5)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr5)),istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6tconex, fin = 23
       subroutine intsm6tconex(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,4,4)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable la (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable lp (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable ls (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1,n3)) return
       nn1= int(stk(lr4))
       if(.not.cremat(fname,top+1,0,nn1,1,lw1,loc1)) return
       if(.not.cremat(fname,top+2,0,1,1,lw2,loc2)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+3,0,istk(iadr(lr4)),1,lw8,loc8)) return
       call tconex(stk(lw1),stk(lw2),istk(iadr(lr1)),istk(iadr(lr2)),ist
     $ k(iadr(lr3)),n1,istk(iadr(lr4)),stk(lw8))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: iscon
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw2)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6flomax, fin = 24
       subroutine intsm6flomax(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,10,10)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable is (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable it (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable la (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable lp (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable he (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable ta (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable mincap (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c       checking variable maxcap (number 8)
c       
       if(.not.getvectrow(fname,top,top-rhs+8,it8,m8,n8,lr8,lc8)) return
c       checking variable n (number 9)
c       
       if(.not.getscalar(fname,top,top-rhs+9,lr9)) return
c       checking variable phi (number 10)
c       
       if(.not.getvectrow(fname,top,top-rhs+10,it10,m10,n10,lr10,lc10)) 
     $ return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n5,n6)) return
       if(.not.checkval(fname,n5,n7)) return
       if(.not.checkval(fname,n5,n8)) return
       if(.not.checkval(fname,n5,n10)) return
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       call entier(n8,stk(lr8),istk(iadr(lr8)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       nn9= int(stk(lr9))
       if(.not.cremat(fname,top+1,0,nn9,1,lw9,loc9)) return
       call entier(1,stk(lr9),istk(iadr(lr9)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n10,stk(lr10),istk(iadr(lr10)))
       if(.not.cremat(fname,top+2,0,istk(iadr(lr9)),1,lw14,loc14)) retur
     $ n
       if(.not.cremat(fname,top+3,0,1,1,lw15,loc15)) return
       call flomax(istk(iadr(lr7)),istk(iadr(lr8)),istk(iadr(lr5)),istk(
     $ iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(iadr(lr4)),n5,stk
     $ (lw9),n3,istk(iadr(lr9)),istk(iadr(lr6)),istk(iadr(lr10)),stk(lw1
     $ 4),stk(lw15))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: vflow
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw15)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: phi
        top=topl+2
        if(.not.cremat(fname,top,0,1,n5,lrs,lcs)) return
        call int2db(n10*m10,istk(iadr(lr10)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6kilter, fin = 25
       subroutine intsm6kilter(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,8,8)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable mincap (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable maxcap (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable he (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable ta (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable la (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable lp (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable n (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c       checking variable cost (number 8)
c       
       if(.not.getvectrow(fname,top,top-rhs+8,it8,m8,n8,lr8,lc8)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1,n2)) return
       if(.not.checkval(fname,n1,n3)) return
       if(.not.checkval(fname,n1,n4)) return
       if(.not.checkval(fname,n1,n8)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(1,stk(lr7),istk(iadr(lr7)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,n1,1,lw11,loc11)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr7)),1,lw12,loc12)) retur
     $ n
       if(.not.cremat(fname,top+3,0,istk(iadr(lr7)),1,lw13,loc13)) retur
     $ n
       if(.not.cremat(fname,top+4,0,istk(iadr(lr7)),1,lw14,loc14)) retur
     $ n
       if(.not.cremat(fname,top+5,0,istk(iadr(lr7)),1,lw15,loc15)) retur
     $ n
       if(.not.cremat(fname,top+6,0,n1,1,lw16,loc16)) return
       if(.not.cremat(fname,top+7,0,1,1,lw17,loc17)) return
       call kilter(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),stk(l
     $ r8),istk(iadr(lr5)),istk(iadr(lr6)),n1,n5,istk(iadr(lr7)),istk(ia
     $ dr(lr4)),stk(lw11),stk(lw12),stk(lw13),stk(lw14),stk(lw15),stk(lw
     $ 16),stk(lw17))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+7
c     
       if(lhs .ge. 1) then
c       --------------output variable: phi
        top=topl+1
        if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
        call int2db(1*n1,istk(iadr(lw11)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: flag
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw17)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6busack, fin = 26
       subroutine intsm6busack(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,10,10)) return
       if(.not.checklhs(fname,1,3)) return
c       checking variable is (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable it (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable v (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable maxcap (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable he (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable ta (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable la (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c       checking variable lp (number 8)
c       
       if(.not.getvectrow(fname,top,top-rhs+8,it8,m8,n8,lr8,lc8)) return
c       checking variable n (number 9)
c       
       if(.not.getscalar(fname,top,top-rhs+9,lr9)) return
c       checking variable cost (number 10)
c       
       if(.not.getvectrow(fname,top,top-rhs+10,it10,m10,n10,lr10,lc10)) 
     $ return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n4,n5)) return
       if(.not.checkval(fname,n4,n6)) return
       if(.not.checkval(fname,n4,n10)) return
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       call entier(n8,stk(lr8),istk(iadr(lr8)))
       nn9= int(stk(lr9))
       if(.not.cremat(fname,top+1,0,nn9,1,lw9,loc9)) return
       call entier(1,stk(lr9),istk(iadr(lr9)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       if(.not.cremat(fname,top+2,0,istk(iadr(lr9)),1,lw13,loc13)) retur
     $ n
       if(.not.cremat(fname,top+3,0,n4,1,lw14,loc14)) return
       if(.not.cremat(fname,top+4,0,istk(iadr(lr9)),1,lw15,loc15)) retur
     $ n
       call entier(1,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+5,0,1,1,lw17,loc17)) return
       if(.not.cremat(fname,top+6,0,1,1,lw18,loc18)) return
       call busack(istk(iadr(lr4)),istk(iadr(lr5)),stk(lr10),istk(iadr(l
     $ r1)),istk(iadr(lr2)),istk(iadr(lr7)),istk(iadr(lr8)),n4,stk(lw9),
     $ n7,istk(iadr(lr9)),istk(iadr(lr6)),stk(lw13),stk(lw14),stk(lw15),
     $ istk(iadr(lr3)),stk(lw17),stk(lw18))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+6
c     
       if(lhs .ge. 1) then
c       --------------output variable: valflo
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw17)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: phi
        top=topl+2
        if(.not.cremat(fname,top,0,1,n4,lrs,lcs)) return
        call int2db(1*n4,istk(iadr(lw14)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: flag
        top=topl+3
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw18)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6floqua, fin = 27
       subroutine intsm6floqua(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,10,10)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable eps (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable mincap (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable maxcap (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable he (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable ta (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable la (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable lp (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c       checking variable n (number 8)
c       
       if(.not.getscalar(fname,top,top-rhs+8,lr8)) return
c       checking variable qorig (number 9)
c       
       if(.not.getvectrow(fname,top,top-rhs+9,it9,m9,n9,lr9,lc9)) return
c       checking variable qweight (number 10)
c       
       if(.not.getvectrow(fname,top,top-rhs+10,it10,m10,n10,lr10,lc10)) 
     $ return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n3)) return
       if(.not.checkval(fname,n2,n4)) return
       if(.not.checkval(fname,n2,n5)) return
       if(.not.checkval(fname,n2,n9)) return
       if(.not.checkval(fname,n2,n10)) return
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       if(.not.cremat(fname,top+1,0,n2,1,lw2,loc2)) return
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+2,0,n2,1,lw4,loc4)) return
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       call entier(1,stk(lr8),istk(iadr(lr8)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       if(.not.cremat(fname,top+3,0,n2,1,lw14,loc14)) return
       if(.not.cremat(fname,top+4,0,istk(iadr(lr8)),1,lw15,loc15)) retur
     $ n
       if(.not.cremat(fname,top+5,0,istk(iadr(lr8)),1,lw16,loc16)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr8)),1,lw17,loc17)) retur
     $ n
       if(.not.cremat(fname,top+7,0,istk(iadr(lr8)),1,lw18,loc18)) retur
     $ n
       if(.not.cremat(fname,top+8,0,n2,1,lw19,loc19)) return
       if(.not.cremat(fname,top+9,0,1,1,lw21,loc21)) return
       call floqua(istk(iadr(lr2)),stk(lw2),istk(iadr(lr3)),stk(lw4),stk
     $ (lr1),istk(iadr(lr4)),istk(iadr(lr6)),istk(iadr(lr7)),n2,n6,istk(
     $ iadr(lr8)),istk(iadr(lr5)),stk(lr9),stk(lw14),stk(lw15),stk(lw16)
     $ ,stk(lw17),stk(lw18),stk(lw19),stk(lr10),stk(lw21))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+9
c     
       if(lhs .ge. 1) then
c       --------------output variable: phi
        top=topl+1
        if(.not.cremat(fname,top,0,1,n2,lrs,lcs)) return
        call dcopy(1*n2,stk(lw14),1,stk(lrs),1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: flag
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw21)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6relax, fin = 28
       subroutine intsm6relax(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,3)) return
c       checking variable he (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable ta (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable cost (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable maxcap (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable demand (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable m (number 6)
c       
       if(.not.getscalar(fname,top,top-rhs+6,lr6)) return
c       checking variable n (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1,n2)) return
       if(.not.checkval(fname,n1,n3)) return
       if(.not.checkval(fname,n1,n4)) return
       call entier(1,stk(lr6),istk(iadr(lr6)))
       call entier(1,stk(lr7),istk(iadr(lr7)))
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr6)),1,lw8,loc8)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr7)),1,lw9,loc9)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr7)),1,lw10,loc10)) retur
     $ n
       if(.not.cremat(fname,top+4,0,istk(iadr(lr7)),1,lw11,loc11)) retur
     $ n
       if(.not.cremat(fname,top+5,0,istk(iadr(lr6)),1,lw12,loc12)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr7)),1,lw13,loc13)) retur
     $ n
       if(.not.cremat(fname,top+7,0,istk(iadr(lr6)),1,lw14,loc14)) retur
     $ n
       if(.not.cremat(fname,top+8,0,istk(iadr(lr6)),1,lw15,loc15)) retur
     $ n
       if(.not.cremat(fname,top+9,0,istk(iadr(lr7)),1,lw16,loc16)) retur
     $ n
       if(.not.cremat(fname,top+10,0,istk(iadr(lr7)),1,lw17,loc17)) retu
     $ rn
       if(.not.cremat(fname,top+11,0,istk(iadr(lr7)),1,lw18,loc18)) retu
     $ rn
       if(.not.cremat(fname,top+12,0,istk(iadr(lr6)),1,lw19,loc19)) retu
     $ rn
       if(.not.cremat(fname,top+13,0,istk(iadr(lr7)),1,lw20,loc20)) retu
     $ rn
       if(.not.cremat(fname,top+14,0,istk(iadr(lr6)),1,lw21,loc21)) retu
     $ rn
       if(.not.cremat(fname,top+15,0,istk(iadr(lr6)),1,lw22,loc22)) retu
     $ rn
       if(.not.cremat(fname,top+16,0,istk(iadr(lr6)),1,lw23,loc23)) retu
     $ rn
       if(.not.cremat(fname,top+17,0,istk(iadr(lr6)),1,lw24,loc24)) retu
     $ rn
       if(.not.cremat(fname,top+18,0,1,1,lw25,loc25)) return
       if(.not.cremat(fname,top+19,0,1,1,lw26,loc26)) return
       call relax(istk(iadr(lr6)),istk(iadr(lr7)),istk(iadr(lr1)),istk(i
     $ adr(lr2)),istk(iadr(lr3)),istk(iadr(lr4)),istk(iadr(lr5)),stk(lw8
     $ ),stk(lw9),stk(lw10),stk(lw11),stk(lw12),stk(lw13),stk(lw14),stk(
     $ lw15),stk(lw16),stk(lw17),stk(lw18),stk(lw19),stk(lw20),stk(lw21)
     $ ,stk(lw22),stk(lw23),stk(lw24),stk(lw25),stk(lw26))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+19
c     
       if(lhs .ge. 1) then
c       --------------output variable: tcost
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call dcopy(1*1,stk(lw25),1,stk(lrs),1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: x
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr6)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr6)),istk(iadr(lw24)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: flag
        top=topl+3
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw26)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6findiso, fin = 29
       subroutine intsm6findiso(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable tail (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable head (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable n (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1,n2)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr3)),1,lw5,loc5)) return
       call findiso(istk(iadr(lr1)),istk(iadr(lr2)),n1,istk(iadr(lr3)),s
     $ tk(lw5))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+1
c     
       if(lhs .ge. 1) then
c       --------------output variable: v
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr3)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr3)),istk(iadr(lw5)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6ta2lpd, fin = 30
       subroutine intsm6ta2lpd(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,4,4)) return
       if(.not.checklhs(fname,1,3)) return
c       checking variable tail (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable head (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable n1 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1,n2)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       nn5= int(stk(lr3))
       if(.not.cremat(fname,top+1,0,nn5,1,lw5,loc5)) return
       if(.not.cremat(fname,top+2,0,n1,1,lw6,loc6)) return
       if(.not.cremat(fname,top+3,0,n1,1,lw7,loc7)) return
       call ta2lpd(istk(iadr(lr1)),istk(iadr(lr2)),n1,istk(iadr(lr4)),st
     $ k(lw5),stk(lw6),stk(lw7))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: lp
        top=topl+1
        if(.not.cremat(fname,top,0,1,nn5,lrs,lcs)) return
        call int2db(1*nn5,istk(iadr(lw5)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: la
        top=topl+2
        if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
        call int2db(1*n1,istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: ls
        top=topl+3
        if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
        call int2db(1*n1,istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6ta2lpu, fin = 31
       subroutine intsm6ta2lpu(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,5,5)) return
       if(.not.checklhs(fname,1,3)) return
c       checking variable tail (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable head (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable n1 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable m (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n1,n2)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       nn5= int(stk(lr3))
       if(.not.cremat(fname,top+1,0,nn5,1,lw5,loc5)) return
       nn6= int(stk(lr5))
       if(.not.cremat(fname,top+2,0,nn6,1,lw6,loc6)) return
       if(.not.cremat(fname,top+3,0,nn6,1,lw7,loc7)) return
       call ta2lpu(istk(iadr(lr1)),istk(iadr(lr2)),n1,istk(iadr(lr4)),st
     $ k(lw5),stk(lw6),stk(lw7))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: lp
        top=topl+1
        if(.not.cremat(fname,top,0,1,nn5,lrs,lcs)) return
        call int2db(1*nn5,istk(iadr(lw5)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: la
        top=topl+2
        if(.not.cremat(fname,top,0,1,nn6,lrs,lcs)) return
        call int2db(1*nn6,istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: ls
        top=topl+3
        if(.not.cremat(fname,top,0,1,nn6,lrs,lcs)) return
        call int2db(1*nn6,istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6lp2tad, fin = 32
       subroutine intsm6lp2tad(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,4,4)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable lp (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable la (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable ls (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n3)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,n2,1,lw5,loc5)) return
       if(.not.cremat(fname,top+2,0,n2,1,lw6,loc6)) return
       call lp2tad(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(
     $ iadr(lr4)),stk(lw5),stk(lw6))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: tail
        top=topl+1
        if(.not.cremat(fname,top,0,1,n2,lrs,lcs)) return
        call int2db(1*n2,istk(iadr(lw5)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: head
        top=topl+2
        if(.not.cremat(fname,top,0,1,n2,lrs,lcs)) return
        call int2db(1*n2,istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6lp2tau, fin = 33
       subroutine intsm6lp2tau(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,5,5)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable lp (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable la (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable ls (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable ma (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n3)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       nn5= int(stk(lr5))
       if(.not.cremat(fname,top+1,0,nn5,1,lw5,loc5)) return
       if(.not.cremat(fname,top+2,0,nn5,1,lw6,loc6)) return
       call lp2tau(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(
     $ iadr(lr4)),stk(lw5),stk(lw6))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: tail
        top=topl+1
        if(.not.cremat(fname,top,0,1,nn5,lrs,lcs)) return
        call int2db(1*nn5,istk(iadr(lw5)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: head
        top=topl+2
        if(.not.cremat(fname,top,0,1,nn5,lrs,lcs)) return
        call int2db(1*nn5,istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6dfs2, fin = 34
       subroutine intsm6dfs2(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,4,4)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable i0 (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable lp (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable ls (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable n (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr4)),1,lw6,loc6)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr4)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr4)),1,lw8,loc8)) return
       if(.not.cremat(fname,top+4,0,istk(iadr(lr4)),1,lw9,loc9)) return
       if(.not.cremat(fname,top+5,0,istk(iadr(lr4)),1,lw10,loc10)) retur
     $ n
       if(.not.cremat(fname,top+6,0,1,1,lw11,loc11)) return
       call dfs2(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),n3,istk
     $ (iadr(lr4)),stk(lw6),stk(lw7),stk(lw8),stk(lw9),stk(lw10),stk(lw1
     $ 1))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+6
c     
       if(lhs .ge. 1) then
c       --------------output variable: nart
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr4)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr4)),istk(iadr(lw10)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: iart
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw11)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6diam, fin = 35
       subroutine intsm6diam(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,5,5)) return
       if(.not.checklhs(fname,1,4)) return
c       checking variable fe (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable che (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable head (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable lngt (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable n1 (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n3,n4)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,1,1,lw6,loc6)) return
       if(.not.cremat(fname,top+2,0,1,1,lw7,loc7)) return
       if(.not.cremat(fname,top+3,0,1,1,lw8,loc8)) return
       if(.not.cremat(fname,top+4,0,n1,1,lw9,loc9)) return
       nn10= int(stk(lr5))
       if(.not.cremat(fname,top+5,0,nn10,1,lw10,loc10)) return
       if(.not.cremat(fname,top+6,0,n1,1,lw11,loc11)) return
       call diam(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(ia
     $ dr(lr4)),n1,stk(lw6),stk(lw7),stk(lw8),stk(lw9),stk(lw10),stk(lw1
     $ 1))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+6
c     
       if(lhs .ge. 1) then
c       --------------output variable: iropt
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: ii
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: p
        top=topl+3
        if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
        call int2db(1*n1,istk(iadr(lw9)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 4) then
c       --------------output variable: idmax
        top=topl+4
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       if(lhs .ge. 4) then
        call copyobj(fname,topl+4,topk+4)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6cent, fin = 36
       subroutine intsm6cent(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getvectrow,getscalar,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,5,5)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable fe (number 1)
c       
       if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c       checking variable che (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable head (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable lngt (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable n1 (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n3,n4)) return
       call entier(n1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n3,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,1,1,lw6,loc6)) return
       if(.not.cremat(fname,top+2,0,1,1,lw7,loc7)) return
       if(.not.cremat(fname,top+3,0,1,1,lw8,loc8)) return
       if(.not.cremat(fname,top+4,0,n1,1,lw9,loc9)) return
       nn10= int(stk(lr5))
       if(.not.cremat(fname,top+5,0,nn10,1,lw10,loc10)) return
       if(.not.cremat(fname,top+6,0,n1,1,lw11,loc11)) return
       call cent(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(ia
     $ dr(lr4)),n1,stk(lw6),stk(lw7),stk(lw8),stk(lw9),stk(lw10),stk(lw1
     $ 1))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+6
c     
       if(lhs .ge. 1) then
c       --------------output variable: r
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: idmax
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6hullcvex, fin = 37
       subroutine intsm6hullcvex(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getmat,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable nn (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable xx (number 3)
c       
       if(.not.getmat(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
       if(.not.checkval(fname,m3,2)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       if(.not.cremat(fname,top+1,0,1,1,lw4,loc4)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr2)),1,lw5,loc5)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw6,loc6)) return
       if(.not.cremat(fname,top+4,0,istk(iadr(lr1)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+5,0,istk(iadr(lr2)),1,lw8,loc8)) return
       call hullcvex(istk(iadr(lr1)),istk(iadr(lr2)),stk(lr3),stk(lw4),s
     $ tk(lw5),stk(lw6),stk(lw7),stk(lw8))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+5
c     
       if(lhs .ge. 1) then
c       --------------output variable: nhull
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw4)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: iwork
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw5)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6clique, fin = 38
       subroutine intsm6clique(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getmat,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,4,4)) return
       if(.not.checklhs(fname,1,3)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable m (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable mn (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable madj (number 4)
c       
       if(.not.getmat(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
       if(.not.checkval(fname,m4,n4)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,m4,n4)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(n4*m4,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,1,1,lw4,loc4)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr1)),1,lw5,loc5)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw6,loc6)) return
       if(.not.cremat(fname,top+4,0,istk(iadr(lr1)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+5,0,istk(iadr(lr1)),1,lw8,loc8)) return
       if(.not.cremat(fname,top+6,0,istk(iadr(lr1)),1,lw9,loc9)) return
       nn10= int(stk(lr3))
       if(.not.cremat(fname,top+7,0,nn10,1,lw10,loc10)) return
       call clique(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr4)),stk(l
     $ w4),stk(lw5),stk(lw6),stk(lw7),stk(lw8),stk(lw9),stk(lw10))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+7
c     
       if(lhs .ge. 1) then
c       --------------output variable: clmax
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw4)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: clnod
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw5)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: bestn
        top=topl+3
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6clique1, fin = 39
       subroutine intsm6clique1(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable m (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable m2 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable np1 (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable nwk (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable kat (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable hat (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr1)),1,lw8,loc8)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr5)),1,lw9,loc9)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw10,loc10)) retur
     $ n
       call clique1(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk
     $ (iadr(lr4)),istk(iadr(lr5)),istk(iadr(lr6)),istk(iadr(lr7)),stk(l
     $ w8),stk(lw9),stk(lw10))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: wcl
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6hamil, fin = 40
       subroutine intsm6hamil(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable m (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable np1 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable pr (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable ar (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable nc (number 6)
c       
       if(.not.getscalar(fname,top,top-rhs+6,lr6)) return
c       checking variable nb (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr1)),1,lw6,loc6)) return
       call entier(1,stk(lr6),istk(iadr(lr6)))
       call entier(1,stk(lr7),istk(iadr(lr7)))
       if(.not.cremat(fname,top+2,0,istk(iadr(lr3)),1,lw9,loc9)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr2)),1,lw10,loc10)) retur
     $ n
       if(.not.cremat(fname,top+4,0,istk(iadr(lr1)),1,lw11,loc11)) retur
     $ n
       if(.not.cremat(fname,top+5,0,istk(iadr(lr1)),1,lw12,loc12)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr1)),1,lw13,loc13)) retur
     $ n
       if(.not.cremat(fname,top+7,0,istk(iadr(lr1)),1,lw14,loc14)) retur
     $ n
       if(.not.cremat(fname,top+8,0,istk(iadr(lr1)),1,lw15,loc15)) retur
     $ n
       if(.not.cremat(fname,top+9,0,istk(iadr(lr1)),1,lw16,loc16)) retur
     $ n
       call hamil(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(i
     $ adr(lr4)),istk(iadr(lr5)),stk(lw6),istk(iadr(lr6)),istk(iadr(lr7)
     $ ),stk(lw9),stk(lw10),stk(lw11),stk(lw12),stk(lw13),stk(lw14),stk(
     $ lw15),stk(lw16))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+9
c     
       if(lhs .ge. 1) then
c       --------------output variable: s
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6visitor, fin = 41
       subroutine intsm6visitor(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getmat,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,3,3)) return
       if(.not.checklhs(fname,1,1)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable a (number 2)
c       
       if(.not.getmat(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
       if(.not.checkval(fname,m2,n2)) return
c       checking variable nstac (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,m2,n2)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2*m2,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr1)),1,lw4,loc4)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr3)),1,lw5,loc5)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw6,loc6)) return
       call visitor(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),stk(
     $ lw4),stk(lw5),stk(lw6))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: iroad
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw4)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6bmatch, fin = 42
       subroutine intsm6bmatch(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       lbuf = 1
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,14,14)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable m (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable nd2 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable n8t8 (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable nd8 (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable np1 (number 6)
c       
       if(.not.getscalar(fname,top,top-rhs+6,lr6)) return
c       checking variable nt2 (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c       checking variable mt2 (number 8)
c       
       if(.not.getscalar(fname,top,top-rhs+8,lr8)) return
c       checking variable mt3 (number 9)
c       
       if(.not.getscalar(fname,top,top-rhs+9,lr9)) return
c       checking variable mt4 (number 10)
c       
       if(.not.getscalar(fname,top,top-rhs+10,lr10)) return
c       checking variable ni (number 11)
c       
       if(.not.getscalar(fname,top,top-rhs+11,lr11)) return
c       checking variable mi (number 12)
c       
       if(.not.getscalar(fname,top,top-rhs+12,lr12)) return
c       checking variable tail (number 13)
c       
       if(.not.getvectrow(fname,top,top-rhs+13,it13,m13,n13,lr13,lc13)) 
     $ return
c       checking variable head (number 14)
c       
       if(.not.getvectrow(fname,top,top-rhs+14,it14,m14,n14,lr14,lc14)) 
     $ return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n13,n14)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr6),istk(iadr(lr6)))
       call entier(1,stk(lr8),istk(iadr(lr8)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr11),istk(iadr(lr11)))
       call entier(1,stk(lr12),istk(iadr(lr12)))
       call entier(n13,stk(lr13),istk(iadr(lr13)))
       call entier(n14,stk(lr14),istk(iadr(lr14)))
       if(.not.cremat(fname,top+1,0,1,1,lw12,loc12)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr1)),1,lw13,loc13)) retur
     $ n
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw14,loc14)) retur
     $ n
       nn15= int(stk(lr10))
       if(.not.cremat(fname,top+4,0,nn15,1,lw15,loc15)) return
       if(.not.cremat(fname,top+5,0,istk(iadr(lr1)),1,lw16,loc16)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr6)),1,lw17,loc17)) retur
     $ n
       if(.not.cremat(fname,top+7,0,istk(iadr(lr8)),1,lw18,loc18)) retur
     $ n
       if(.not.cremat(fname,top+8,0,istk(iadr(lr1)),1,lw19,loc19)) retur
     $ n
       if(.not.cremat(fname,top+9,0,istk(iadr(lr1)),1,lw20,loc20)) retur
     $ n
       if(.not.cremat(fname,top+10,0,istk(iadr(lr1)),1,lw21,loc21)) retu
     $ rn
       if(.not.cremat(fname,top+11,0,istk(iadr(lr1)),1,lw22,loc22)) retu
     $ rn
       if(.not.cremat(fname,top+12,0,istk(iadr(lr1)),1,lw23,loc23)) retu
     $ rn
       nn24= int(stk(lr7))
       if(.not.cremat(fname,top+13,0,nn24,1,lw24,loc24)) return
       if(.not.cremat(fname,top+14,0,istk(iadr(lr1)),1,lw25,loc25)) retu
     $ rn
       if(.not.cremat(fname,top+15,0,istk(iadr(lr3)),1,lw26,loc26)) retu
     $ rn
       if(.not.cremat(fname,top+16,0,istk(iadr(lr3)),1,lw27,loc27)) retu
     $ rn
       if(.not.cremat(fname,top+17,0,istk(iadr(lr3)),1,lw28,loc28)) retu
     $ rn
       if(.not.cremat(fname,top+18,0,istk(iadr(lr3)),1,lw29,loc29)) retu
     $ rn
       if(.not.cremat(fname,top+19,0,istk(iadr(lr1)),1,lw30,loc30)) retu
     $ rn
       if(.not.cremat(fname,top+20,0,istk(iadr(lr1)),1,lw31,loc31)) retu
     $ rn
       nn32= int(stk(lr9))
       if(.not.cremat(fname,top+21,0,nn32,1,lw32,loc32)) return
       if(.not.cremat(fname,top+22,0,istk(iadr(lr3)),1,lw33,loc33)) retu
     $ rn
       if(.not.cremat(fname,top+23,0,istk(iadr(lr8)),1,lw34,loc34)) retu
     $ rn
       if(.not.cremat(fname,top+24,0,istk(iadr(lr1)),1,lw35,loc35)) retu
     $ rn
       if(.not.cremat(fname,top+25,0,istk(iadr(lr8)),1,lw36,loc36)) retu
     $ rn
       if(.not.cremat(fname,top+26,0,istk(iadr(lr1)),1,lw37,loc37)) retu
     $ rn
       if(.not.cremat(fname,top+27,0,istk(iadr(lr1)),1,lw38,loc38)) retu
     $ rn
       if(.not.cremat(fname,top+28,0,istk(iadr(lr8)),1,lw39,loc39)) retu
     $ rn
       if(.not.cremat(fname,top+29,0,istk(iadr(lr1)),1,lw40,loc40)) retu
     $ rn
       if(.not.cremat(fname,top+30,0,istk(iadr(lr1)),1,lw41,loc41)) retu
     $ rn
       if(.not.cremat(fname,top+31,0,istk(iadr(lr1)),1,lw42,loc42)) retu
     $ rn
       if(.not.cremat(fname,top+32,0,istk(iadr(lr1)),1,lw43,loc43)) retu
     $ rn
       if(.not.cremat(fname,top+33,0,istk(iadr(lr1)),1,lw44,loc44)) retu
     $ rn
       if(.not.cremat(fname,top+34,0,istk(iadr(lr4)),1,lw45,loc45)) retu
     $ rn
       call bmatch(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr6)),istk(
     $ iadr(lr8)),istk(iadr(lr3)),istk(iadr(lr4)),istk(iadr(lr5)),istk(i
     $ adr(lr11)),istk(iadr(lr12)),istk(iadr(lr13)),istk(iadr(lr14)),stk
     $ (lw12),stk(lw13),stk(lw14),stk(lw15),stk(lw16),stk(lw17),stk(lw18
     $ ),stk(lw19),stk(lw20),stk(lw21),stk(lw22),stk(lw23),stk(lw24),stk
     $ (lw25),stk(lw26),stk(lw27),stk(lw28),stk(lw29),stk(lw30),stk(lw31
     $ ),stk(lw32),stk(lw33),stk(lw34),stk(lw35),stk(lw36),stk(lw37),stk
     $ (lw38),stk(lw39),stk(lw40),stk(lw41),stk(lw42),stk(lw43),stk(lw44
     $ ),stk(lw45))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+34
c     
       if(lhs .ge. 1) then
c       --------------output variable: card
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw12)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: match
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw13)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6knapsk, fin = 43
       subroutine intsm6knapsk(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,9,9)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable m (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable np1 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable mn (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable mnp1 (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable p (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable w (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c       checking variable k (number 8)
c       
       if(.not.getvectrow(fname,top,top-rhs+8,it8,m8,n8,lr8,lc8)) return
c       checking variable bck (number 9)
c       
       if(.not.getscalar(fname,top,top-rhs+9,lr9)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n6,n7)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       call entier(n8,stk(lr8),istk(iadr(lr8)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr1)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+2,0,1,1,lw8,loc8)) return
       call entier(1,stk(lr9),istk(iadr(lr9)))
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw10,loc10)) retur
     $ n
       if(.not.cremat(fname,top+4,0,istk(iadr(lr1)),1,lw11,loc11)) retur
     $ n
       if(.not.cremat(fname,top+5,0,istk(iadr(lr1)),1,lw12,loc12)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr3)),1,lw13,loc13)) retur
     $ n
       if(.not.cremat(fname,top+7,0,istk(iadr(lr3)),1,lw14,loc14)) retur
     $ n
       if(.not.cremat(fname,top+8,0,istk(iadr(lr1)),1,lw15,loc15)) retur
     $ n
       nn16= int(stk(lr4))
       if(.not.cremat(fname,top+9,0,nn16,1,lw16,loc16)) return
       nn17= int(stk(lr5))
       if(.not.cremat(fname,top+10,0,nn17,1,lw17,loc17)) return
       if(.not.cremat(fname,top+11,0,nn16,1,lw18,loc18)) return
       if(.not.cremat(fname,top+12,0,nn16,1,lw19,loc19)) return
       if(.not.cremat(fname,top+13,0,istk(iadr(lr3)),1,lw20,loc20)) retu
     $ rn
       if(.not.cremat(fname,top+14,0,istk(iadr(lr1)),1,lw21,loc21)) retu
     $ rn
       if(.not.cremat(fname,top+15,0,istk(iadr(lr2)),1,lw22,loc22)) retu
     $ rn
       if(.not.cremat(fname,top+16,0,istk(iadr(lr2)),1,lw23,loc23)) retu
     $ rn
       if(.not.cremat(fname,top+17,0,istk(iadr(lr2)),1,lw24,loc24)) retu
     $ rn
       if(.not.cremat(fname,top+18,0,istk(iadr(lr2)),1,lw25,loc25)) retu
     $ rn
       if(.not.cremat(fname,top+19,0,istk(iadr(lr1)),1,lw26,loc26)) retu
     $ rn
       if(.not.cremat(fname,top+20,0,istk(iadr(lr1)),1,lw27,loc27)) retu
     $ rn
       if(.not.cremat(fname,top+21,0,istk(iadr(lr1)),1,lw28,loc28)) retu
     $ rn
       if(.not.cremat(fname,top+22,0,istk(iadr(lr1)),1,lw29,loc29)) retu
     $ rn
       if(.not.cremat(fname,top+23,0,istk(iadr(lr1)),1,lw30,loc30)) retu
     $ rn
       call knapsk(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(
     $ iadr(lr6)),istk(iadr(lr7)),istk(iadr(lr8)),stk(lw7),stk(lw8),istk
     $ (iadr(lr9)),stk(lw10),stk(lw11),stk(lw12),stk(lw13),stk(lw14),stk
     $ (lw15),stk(lw16),stk(lw17),stk(lw18),stk(lw19),stk(lw20),stk(lw21
     $ ),stk(lw22),stk(lw23),stk(lw24),stk(lw25),stk(lw26),stk(lw27),stk
     $ (lw28),stk(lw29),stk(lw30))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+23
c     
       if(lhs .ge. 1) then
c       --------------output variable: xstar
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: vstar
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6prfmatch, fin = 44
       subroutine intsm6prfmatch(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable m (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable np1 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable m2 (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable nbl (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable cc (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable index (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n5,n6)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       if(.not.cremat(fname,top+1,0,1,1,lw8,loc8)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr1)),1,lw9,loc9)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw10,loc10)) retur
     $ n
       if(.not.cremat(fname,top+4,0,istk(iadr(lr1)),1,lw11,loc11)) retur
     $ n
       if(.not.cremat(fname,top+5,0,istk(iadr(lr1)),1,lw12,loc12)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr1)),1,lw13,loc13)) retur
     $ n
       if(.not.cremat(fname,top+7,0,istk(iadr(lr1)),1,lw14,loc14)) retur
     $ n
       if(.not.cremat(fname,top+8,0,istk(iadr(lr1)),1,lw15,loc15)) retur
     $ n
       if(.not.cremat(fname,top+9,0,istk(iadr(lr1)),1,lw16,loc16)) retur
     $ n
       if(.not.cremat(fname,top+10,0,istk(iadr(lr1)),1,lw17,loc17)) retu
     $ rn
       if(.not.cremat(fname,top+11,0,istk(iadr(lr1)),1,lw18,loc18)) retu
     $ rn
       if(.not.cremat(fname,top+12,0,istk(iadr(lr1)),1,lw19,loc19)) retu
     $ rn
       if(.not.cremat(fname,top+13,0,istk(iadr(lr1)),1,lw20,loc20)) retu
     $ rn
       call prfmatch(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),ist
     $ k(iadr(lr4)),istk(iadr(lr5)),istk(iadr(lr6)),istk(iadr(lr7)),stk(
     $ lw8),stk(lw9),stk(lw10),stk(lw11),stk(lw12),stk(lw13),stk(lw14),s
     $ tk(lw15),stk(lw16),stk(lw17),stk(lw18),stk(lw19),stk(lw20))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+13
c     
       if(lhs .ge. 1) then
c       --------------output variable: cost
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: nmatch
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw9)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6permuto, fin = 45
       subroutine intsm6permuto(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getmat,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,6,6)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable n3 (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable n4 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable c (number 4)
c       
       if(.not.getmat(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
       if(.not.checkval(fname,m4,n4)) return
c       checking variable f (number 5)
c       
       if(.not.getmat(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
       if(.not.checkval(fname,m5,n5)) return
c       checking variable d (number 6)
c       
       if(.not.getmat(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
       if(.not.checkval(fname,m6,n6)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,m4,n4)) return
       if(.not.checkval(fname,m4,m5)) return
       if(.not.checkval(fname,m4,n5)) return
       if(.not.checkval(fname,m4,m6)) return
       if(.not.checkval(fname,m4,n6)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+1,0,1,1,lw7,loc7)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr2)),1,lw8,loc8)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr3)),1,lw9,loc9)) return
       call permuto(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),stk(
     $ lr4),stk(lr5),stk(lr6),stk(lw7),stk(lw8),stk(lw9))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+3
c     
       if(lhs .ge. 1) then
c       --------------output variable: crit
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call dcopy(1*1,stk(lw7),1,stk(lrs),1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: loc3n
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6mesh2b, fin = 46
       subroutine intsm6mesh2b(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getmat,checkval,getvectrow,cr
     $ emat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,4)) return
c       checking variable nbs (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable n6 (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable n4 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable lfront (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable cr (number 5)
c       
       if(.not.getmat(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
       if(.not.checkval(fname,m5,2)) return
c       checking variable tri (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable front (number 7)
c       
       if(.not.getvectrow(fname,top,top-rhs+7,it7,m7,n7,lr7,lc7)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       if(.not.cremat(fname,top+1,0,m5,istk(iadr(lr1)),lw6,lwc6)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr2)),1,lw7,loc7)) return
       call entier(n6,stk(lr6),istk(iadr(lr6)))
       call entier(n7,stk(lr7),istk(iadr(lr7)))
       if(.not.cremat(fname,top+3,0,1,1,lw10,loc10)) return
       if(.not.cremat(fname,top+4,0,1,1,lw11,loc11)) return
       call mesh2b(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk(
     $ iadr(lr4)),stk(lr5),stk(lw6),stk(lw7),istk(iadr(lr6)),istk(iadr(l
     $ r7)),stk(lw10),stk(lw11))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+4
c     
       if(lhs .ge. 1) then
c       --------------output variable: nbt
        top=topl+1
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw10)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: nu
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: c
        top=topl+3
        if(.not.cremat(fname,top,0,m5,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(m5*istk(iadr(lr1)),istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 4) then
c       --------------output variable: err
        top=topl+4
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw11)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       if(lhs .ge. 4) then
        call copyobj(fname,topl+4,topk+4)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6deumesh, fin = 47
       subroutine intsm6deumesh(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,5,5)) return
       if(.not.checklhs(fname,1,5)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable n4 (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable n6 (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable x (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable y (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n4,n5)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr3)),1,lw6,loc6)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr1)),1,lw7,loc7)) return
       if(.not.cremat(fname,top+3,0,1,1,lw8,loc8)) return
       nn9=3
       if(.not.cremat(fname,top+4,0,nn9,istk(iadr(lr2)),lw9,lwc9)) retur
     $ n
       if(.not.cremat(fname,top+5,0,1,1,lw10,loc10)) return
       call deumesh(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),stk(
     $ lr4),stk(lr5),stk(lw6),stk(lw7),stk(lw8),stk(lw9),stk(lw10))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+5
c     
       if(lhs .ge. 1) then
c       --------------output variable: iadj
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr3)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr3)),istk(iadr(lw6)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: iend
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw7)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: nbt
        top=topl+3
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 4) then
c       --------------output variable: nu
        top=topl+4
        if(.not.cremat(fname,top,0,nn9,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(nn9*istk(iadr(lr2)),istk(iadr(lw9)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 5) then
c       --------------output variable: iero
        top=topl+5
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw10)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       if(lhs .ge. 4) then
        call copyobj(fname,topl+4,topk+4)
       endif
       if(lhs .ge. 5) then
        call copyobj(fname,topl+5,topk+5)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6bandred, fin = 48
       subroutine intsm6bandred(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,7,7)) return
       if(.not.checklhs(fname,1,4)) return
c       checking variable n (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable nz (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable liwork (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable iwork (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable lrwork (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable rwork (number 6)
c       
       if(.not.getvectrow(fname,top,top-rhs+6,it6,m6,n6,lr6,lc6)) return
c       checking variable optpro (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr7),istk(iadr(lr7)))
       if(.not.cremat(fname,top+1,0,1,1,lw8,loc8)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr1)),1,lw9,loc9)) return
       if(.not.cremat(fname,top+3,0,istk(iadr(lr1)),1,lw10,loc10)) retur
     $ n
       if(.not.cremat(fname,top+4,0,istk(iadr(lr1)),1,lw11,loc11)) retur
     $ n
       call bandred(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),istk
     $ (iadr(lr4)),istk(iadr(lr5)),stk(lr6),istk(iadr(lr7)),stk(lw8),stk
     $ (lw9),stk(lw10),stk(lw11))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+4
c     
       if(lhs .ge. 1) then
c       --------------output variable: iperm
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw9)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: mrepi
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw10)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: iband
        top=topl+3
        if(.not.cremat(fname,top,0,1,istk(iadr(lr1)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr1)),istk(iadr(lw11)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 4) then
c       --------------output variable: ierr
        top=topl+4
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       if(lhs .ge. 4) then
        call copyobj(fname,topl+4,topk+4)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6meshmesh, fin = 49
       subroutine intsm6meshmesh(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,19,19)) return
       if(.not.checklhs(fname,1,8)) return
c       checking variable inseed (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable n (number 2)
c       
       if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
c       checking variable nsorc (number 3)
c       
       if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
c       checking variable nsink (number 4)
c       
       if(.not.getscalar(fname,top,top-rhs+4,lr4)) return
c       checking variable mincst (number 5)
c       
       if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
c       checking variable maxcst (number 6)
c       
       if(.not.getscalar(fname,top,top-rhs+6,lr6)) return
c       checking variable itsup (number 7)
c       
       if(.not.getscalar(fname,top,top-rhs+7,lr7)) return
c       checking variable otsup (number 8)
c       
       if(.not.getscalar(fname,top,top-rhs+8,lr8)) return
c       checking variable bhicst (number 9)
c       
       if(.not.getscalar(fname,top,top-rhs+9,lr9)) return
c       checking variable bcap (number 10)
c       
       if(.not.getscalar(fname,top,top-rhs+10,lr10)) return
c       checking variable mincap (number 11)
c       
       if(.not.getscalar(fname,top,top-rhs+11,lr11)) return
c       checking variable maxcap (number 12)
c       
       if(.not.getscalar(fname,top,top-rhs+12,lr12)) return
c       checking variable ns (number 13)
c       
       if(.not.getscalar(fname,top,top-rhs+13,lr13)) return
c       checking variable ns2 (number 14)
c       
       if(.not.getscalar(fname,top,top-rhs+14,lr14)) return
c       checking variable ns4 (number 15)
c       
       if(.not.getscalar(fname,top,top-rhs+15,lr15)) return
c       checking variable ns10 (number 16)
c       
       if(.not.getscalar(fname,top,top-rhs+16,lr16)) return
c       checking variable ns12 (number 17)
c       
       if(.not.getscalar(fname,top,top-rhs+17,lr17)) return
c       checking variable ns20 (number 18)
c       
       if(.not.getscalar(fname,top,top-rhs+18,lr18)) return
c       checking variable ns30 (number 19)
c       
       if(.not.getscalar(fname,top,top-rhs+19,lr19)) return
c     
c       cross variable size checking
c     
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(1,stk(lr2),istk(iadr(lr2)))
       call entier(1,stk(lr3),istk(iadr(lr3)))
       call entier(1,stk(lr4),istk(iadr(lr4)))
       call entier(1,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr6),istk(iadr(lr6)))
       call entier(1,stk(lr7),istk(iadr(lr7)))
       call entier(1,stk(lr8),istk(iadr(lr8)))
       call simple(1,stk(lr9),stk(lr9))
       call simple(1,stk(lr10),stk(lr10))
       call entier(1,stk(lr11),istk(iadr(lr11)))
       call entier(1,stk(lr12),istk(iadr(lr12)))
       if(.not.cremat(fname,top+1,0,1,1,lw13,loc13)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr2)),1,lw14,loc14)) retur
     $ n
       nn15= int(stk(lr16))
       if(.not.cremat(fname,top+3,0,nn15,1,lw15,loc15)) return
       if(.not.cremat(fname,top+4,0,nn15,1,lw16,loc16)) return
       if(.not.cremat(fname,top+5,0,istk(iadr(lr2)),1,lw17,loc17)) retur
     $ n
       if(.not.cremat(fname,top+6,0,istk(iadr(lr2)),1,lw18,loc18)) retur
     $ n
       if(.not.cremat(fname,top+7,0,nn15,1,lw19,loc19)) return
       if(.not.cremat(fname,top+8,0,nn15,1,lw20,loc20)) return
       call entier(1,stk(lr13),istk(iadr(lr13)))
       call entier(1,stk(lr15),istk(iadr(lr15)))
       call entier(1,stk(lr17),istk(iadr(lr17)))
       call entier(1,stk(lr16),istk(iadr(lr16)))
       nn25= int(stk(lr14))
       if(.not.cremat(fname,top+9,0,nn25,1,lw25,loc25)) return
       if(.not.cremat(fname,top+10,0,istk(iadr(lr13)),1,lw26,loc26)) ret
     $ urn
       if(.not.cremat(fname,top+11,0,istk(iadr(lr13)),1,lw27,loc27)) ret
     $ urn
       if(.not.cremat(fname,top+12,0,istk(iadr(lr17)),1,lw28,loc28)) ret
     $ urn
       if(.not.cremat(fname,top+13,0,istk(iadr(lr15)),1,lw29,loc29)) ret
     $ urn
       if(.not.cremat(fname,top+14,0,istk(iadr(lr13)),1,lw30,loc30)) ret
     $ urn
       nn31= int(stk(lr19))
       if(.not.cremat(fname,top+15,0,nn31,1,lw31,loc31)) return
       if(.not.cremat(fname,top+16,0,istk(iadr(lr13)),1,lw32,loc32)) ret
     $ urn
       if(.not.cremat(fname,top+17,0,istk(iadr(lr13)),1,lw33,loc33)) ret
     $ urn
       call meshmesh(istk(iadr(lr1)),istk(iadr(lr2)),istk(iadr(lr3)),ist
     $ k(iadr(lr4)),istk(iadr(lr5)),istk(iadr(lr6)),istk(iadr(lr7)),istk
     $ (iadr(lr8)),stk(lr9),stk(lr10),istk(iadr(lr11)),istk(iadr(lr12)),
     $ stk(lw13),stk(lw14),stk(lw15),stk(lw16),stk(lw17),stk(lw18),stk(l
     $ w19),stk(lw20),istk(iadr(lr13)),istk(iadr(lr15)),istk(iadr(lr17))
     $ ,istk(iadr(lr16)),stk(lw25),stk(lw26),stk(lw27),stk(lw28),stk(lw2
     $ 9),stk(lw30),stk(lw31),stk(lw32),stk(lw33))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+17
c     
       if(lhs .ge. 1) then
c       --------------output variable: ntype
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw14)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: m
        top=topl+2
        if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
        call int2db(1*1,istk(iadr(lw13)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 3) then
c       --------------output variable: tail
        top=topl+3
        if(.not.cremat(fname,top,0,1,istk(iadr(lr16)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr16)),istk(iadr(lw15)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 4) then
c       --------------output variable: head
        top=topl+4
        if(.not.cremat(fname,top,0,1,istk(iadr(lr16)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr16)),istk(iadr(lw16)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 5) then
c       --------------output variable: xnod
        top=topl+5
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw17)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 6) then
c       --------------output variable: ynod
        top=topl+6
        if(.not.cremat(fname,top,0,1,istk(iadr(lr2)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr2)),istk(iadr(lw18)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 7) then
c       --------------output variable: cap
        top=topl+7
        if(.not.cremat(fname,top,0,1,istk(iadr(lr16)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr16)),istk(iadr(lw19)),-1,stk(lrs),-1)
       endif
c     
       if(lhs .ge. 8) then
c       --------------output variable: cost
        top=topl+8
        if(.not.cremat(fname,top,0,1,istk(iadr(lr16)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr16)),istk(iadr(lw20)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       if(lhs .ge. 3) then
        call copyobj(fname,topl+3,topk+3)
       endif
       if(lhs .ge. 4) then
        call copyobj(fname,topl+4,topk+4)
       endif
       if(lhs .ge. 5) then
        call copyobj(fname,topl+5,topk+5)
       endif
       if(lhs .ge. 6) then
        call copyobj(fname,topl+6,topk+6)
       endif
       if(lhs .ge. 7) then
        call copyobj(fname,topl+7,topk+7)
       endif
       if(lhs .ge. 8) then
        call copyobj(fname,topl+8,topk+8)
       endif
       top=topk+lhs
       return
       end
c
c SCILAB function : m6ford, fin = 50
       subroutine intsm6ford(fname)
c
       character*(*) fname
       include '../stack.h'
c
       integer iadr, sadr
       integer topk,rhsk,topl
       logical checkrhs,checklhs,getscalar,getvectrow,checkval,cremat
       iadr(l)=l+l-1
       sadr(l)=(l/2)+1
       rhs = max(0,rhs)
c
       topk = top 
       rhsk = rhs 
       if(.not.checkrhs(fname,6,6)) return
       if(.not.checklhs(fname,1,2)) return
c       checking variable i0 (number 1)
c       
       if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
c       checking variable la (number 2)
c       
       if(.not.getvectrow(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
c       checking variable length (number 3)
c       
       if(.not.getvectrow(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
c       checking variable lp (number 4)
c       
       if(.not.getvectrow(fname,top,top-rhs+4,it4,m4,n4,lr4,lc4)) return
c       checking variable ls (number 5)
c       
       if(.not.getvectrow(fname,top,top-rhs+5,it5,m5,n5,lr5,lc5)) return
c       checking variable n (number 6)
c       
       if(.not.getscalar(fname,top,top-rhs+6,lr6)) return
c     
c       cross variable size checking
c     
       if(.not.checkval(fname,n2,n5)) return
       call entier(1,stk(lr1),istk(iadr(lr1)))
       call entier(n2,stk(lr2),istk(iadr(lr2)))
       call entier(n4,stk(lr4),istk(iadr(lr4)))
       call entier(n5,stk(lr5),istk(iadr(lr5)))
       call entier(1,stk(lr6),istk(iadr(lr6)))
       if(.not.cremat(fname,top+1,0,istk(iadr(lr6)),1,lw8,loc8)) return
       if(.not.cremat(fname,top+2,0,istk(iadr(lr6)),1,lw9,loc9)) return
       call ford(istk(iadr(lr1)),istk(iadr(lr2)),stk(lr3),istk(iadr(lr4)
     $ ),istk(iadr(lr5)),n2,istk(iadr(lr6)),stk(lw8),stk(lw9))
       if(err .gt. 0 .or. err1 .gt. 0) return 
c
       topk=top-rhs
       topl=top+2
c     
       if(lhs .ge. 1) then
c       --------------output variable: pi
        top=topl+1
        if(.not.cremat(fname,top,0,1,istk(iadr(lr6)),lrs,lcs)) return
        call dcopy(1*istk(iadr(lr6)),stk(lw9),1,stk(lrs),1)
       endif
c     
       if(lhs .ge. 2) then
c       --------------output variable: p
        top=topl+2
        if(.not.cremat(fname,top,0,1,istk(iadr(lr6)),lrs,lcs)) return
        call int2db(1*istk(iadr(lr6)),istk(iadr(lw8)),-1,stk(lrs),-1)
       endif
c     Putting in order the stack
       if(lhs .ge. 1) then
        call copyobj(fname,topl+1,topk+1)
       endif
       if(lhs .ge. 2) then
        call copyobj(fname,topl+2,topk+2)
       endif
       top=topk+lhs
       return
       end
c

c  interface function 
c   ********************
       subroutine metane
       include '../stack.h'
       rhs = max(0,rhs)
c
       goto (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
     $ ,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,4
     $ 5,46,47,48,49,50) fin
       return
1      call intsm6loadg('m6loadg')
       return
2      call intsm6saveg('m6saveg')
       return
3      call intsm6prevn2p('m6prevn2p')
       return
4      call intsm6ns2p('m6ns2p')
       return
5      call intsm6p2ns('m6p2ns')
       return
6      call intsm6edge2st('m6edge2st')
       return
7      call intsm6prevn2st('m6prevn2st')
       return
8      call intsm6compc('m6compc')
       return
9      call intsm6concom('m6concom')
       return
10      call intsm6compfc('m6compfc')
       return
11      call intsm6sconcom('m6sconcom')
       return
12      call intsm6pcchna('m6pcchna')
       return
13      call intsm6fordfulk('m6fordfulk')
       return
14      call intsm6johns('m6johns')
       return
15      call intsm6dijkst('m6dijkst')
       return
16      call intsm6frang('m6frang')
       return
17      call intsm6chcm('m6chcm')
       return
18      call intsm6transc('m6transc')
       return
19      call intsm6dfs('m6dfs')
       return
20      call intsm6umtree('m6umtree')
       return
21      call intsm6umtree1('m6umtree1')
       return
22      call intsm6dmtree('m6dmtree')
       return
23      call intsm6tconex('m6tconex')
       return
24      call intsm6flomax('m6flomax')
       return
25      call intsm6kilter('m6kilter')
       return
26      call intsm6busack('m6busack')
       return
27      call intsm6floqua('m6floqua')
       return
28      call intsm6relax('m6relax')
       return
29      call intsm6findiso('m6findiso')
       return
30      call intsm6ta2lpd('m6ta2lpd')
       return
31      call intsm6ta2lpu('m6ta2lpu')
       return
32      call intsm6lp2tad('m6lp2tad')
       return
33      call intsm6lp2tau('m6lp2tau')
       return
34      call intsm6dfs2('m6dfs2')
       return
35      call intsm6diam('m6diam')
       return
36      call intsm6cent('m6cent')
       return
37      call intsm6hullcvex('m6hullcvex')
       return
38      call intsm6clique('m6clique')
       return
39      call intsm6clique1('m6clique1')
       return
40      call intsm6hamil('m6hamil')
       return
41      call intsm6visitor('m6visitor')
       return
42      call intsm6bmatch('m6bmatch')
       return
43      call intsm6knapsk('m6knapsk')
       return
44      call intsm6prfmatch('m6prfmatch')
       return
45      call intsm6permuto('m6permuto')
       return
46      call intsm6mesh2b('m6mesh2b')
       return
47      call intsm6deumesh('m6deumesh')
       return
48      call intsm6bandred('m6bandred')
       return
49      call intsm6meshmesh('m6meshmesh')
       return
50      call intsm6ford('m6ford')
       return
       end
