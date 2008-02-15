
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c SCILAB function : m6loadg, fin = 1
       subroutine intsm6loadg(fname)
c
       character*(*) fname
       include 'stack.h'
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
