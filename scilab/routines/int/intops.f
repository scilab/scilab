      subroutine intops
c     
c     operations matricielles
c     
c     Copyright INRIA
      include '../stack.h'
      integer op
c     
      integer star,dstar,slash,bslash,dot,colon,quote
      integer less,great,equal,et,ou,non
c
      data star/47/,dstar/62/,slash/48/
      data bslash/49/,dot/51/,colon/44/,quote/53/
      data less/59/,great/60/,equal/50/
      data ou/57/,et/58/,non/61/
c     
      op=fin
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' intops op: '//buf(1:4))
      endif
c     
c     operations binaires et ternaires
c     --------------------------------
c     
      fun = 0
c     
c        cconc  extrac insert rconc
      goto(75  ,  95  ,  78   ,76) op
c     
c           :  +  -  * /  \  =          '
      goto(50,07,08,10,20,25,130,06,06,70) op+1-colon
c     
 06   if(op.eq.dstar) goto 31
      if(op.eq.quote+dot) goto 71
      if(op.eq.dstar+dot) goto 30
      if(op.ge.3*dot+star) goto 65
      if(op.ge.2*dot+star) goto 120
      if(op.ge.less+equal) goto 130
      if(op.eq.dot+star) goto 51
      if(op.eq.dot+slash) goto 52
      if(op.eq.dot+bslash) goto 53


      if(op.eq.et.or.op.eq.ou) goto 140
      if(op.eq.non) goto 150
      if(op.ge.less) goto 130
      fin=-fin
      return
    
c     
c     addition
 07   continue
      call i_a_i
      go to 999
c     
c     substraction
 08   if(rhs.eq.1) then
c     .  unary minus
         call i_s_i
      else
         call i_s_i
      endif
      go to 999
c     
c     multiplication
 10   continue
      call i_m_i
      go to 999
c     
c     division a droite
 20   continue
      call i_r_i
      go to 999
c     
c     \
 25   continue
      call i_l_i
      go to 999
c     
c     .^
 30   continue
c      call i_j_i
      fin=-fin
      goto 999
c     
c     ^
 31   continue
c      call i_p_i
      fin=-fin
      go to 999
c     
c     :
 50   continue
c      call i_b_i
      fin=-fin
      go to 999
c     
c     .*
 51   continue
      call i_x_i
      go to 999
c     
c     ./
 52   continue
      call i_d_i
      go to 999
c     
c     .\
 53   continue
      call i_q_i
      go to 999

c     .*. ./. .\.
c     kronecker
 65   call i_kron
      go to 999
c     
c     '
 70   continue
      call i_t
      goto 999
c     
c     .'
 71   continue
      call i_t
      goto 999
c     
c     concatenation [a b]
 75   continue
      call i_c_i
      goto 999
c     
c     concatenation [a;b]
 76   continue
      call i_f_i
      goto 999
c     
c     extraction a(i) and a(i,j)
c     
 78   continue
      if (rhs.eq.2) then
         call i_e
      elseif (rhs.eq.3) then
         call i_e
      else
         fin=-fin
      endif
      go to 999
c      
c     insertion
c     
 95   continue
      if (rhs.eq.3) then
         call i_i_i
      elseif (rhs.eq.4) then
         call i_i_i
      else
         fin=-fin
      endif
      goto 999

c     
c     *. /. \.
 120  fin=-fin
      goto 999
c     
c     == <= >= ~=
 130  continue
      call i_logic(op)
      goto 999
c
c     & | 
 140  continue
      call bitops(op)
      goto 999
c
c     ~
 150  continue
      call unarybit(61)
      goto 999

 999  return
      end

