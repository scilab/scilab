      subroutine gwstring
c     ==================================================================
c     
c     evaluation des fonctions elementaires sur les chaines de
c     caracteres
c     
c     ==================================================================
c     
c     Copyright INRIA
      INCLUDE '../stack.h'
c     
      integer iadr,sadr
      integer id(nsiz)
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
c     
c     functions/fin
c     1       2       3       4       5           6       7       8   
c     length    part   string  convstr  emptystr str2code code2str sort
c     9         10       11        12    13    14     15
c     strcat    strindex strsubst ascii  grep tokens  strsplit

      goto (10,20,25,50,60,70,80,90,100,110,120,125,130, 135,140) fin
c     
c     length
c     
 10   continue
      call intlength(id)
      goto 900


c     part
 20   continue
      call intpart(id)
      goto 900
c     
c     string
c     
 25   continue
      call intstring(id)
      goto 900

c     
c     convstr
 50   continue
      call intconvstr(id)
      goto 900

c     emptystr
 60   continue
      call intemptystr(id)
      goto 900
c     
c     str2code
 70   continue
      call intstr2code(id)
      goto 900
c     
c     code2str
 80   continue
      call intcode2str(id)
      goto 900
c     
c     sort
 90   continue
      call intssort(id)
      goto 900
c     
c     strcat(str [,ins])
 100  continue
      call instrcat(id)
      goto 900
c     
c     strindex(str1,str2)
 110  continue
      call instrindex(id)
      goto 900
c     
c     strsubst(str1,str2,str3)
 120  continue 
      call intstrsubst(id)
      goto 900
c     
c     ascii
c     
 125  continue
      call intascii(id)
      goto 900
c     
c     grep(str1,str2) 
 130  continue
      call intgrep()
      goto 900
c     
c     tokens(str [,sep])
 135  continue
      call inttokens(id)
      goto 900

c     strsplit(str,v)
 140  continue
      call intstrsplit(id)
      goto 900

c     
 900  return
      end


