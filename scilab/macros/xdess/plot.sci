function []=plot(x,y,legx,legy,leg)
// 
// For backward compatibility with some very old stuff
//! 
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs >=2,
   if type(y)==1, 
     if prod(size(x))<>prod(size(y)),
       write(%io(2),'x and y must have the same size');
       return;end
   end;
end;

select rhs,
  case 1 then 
        [nl,nk]=size(x);
        if nl==1|nk==1 then
        plot2d1("enn",1,matrix(x,prod(size(x)),1));
        else
        xbasc();plot2d((ones(nl,1)*(1:nk))',x')
        end
  case 2 then xbasc();
         if type(y)==10,
           plot2d1("enn",1,matrix(x,prod(size(x)),1));
           xtitle(' ',y,' ');
         else
           plot2d(matrix(x,prod(size(x)),1),matrix(y,prod(size(y)),1));
         end;
  case 3 then xbasc();
         if type(y)==10,
           plot2d1("enn",1,matrix(x,prod(size(x)),-1));
           xtitle(' ',y,legx);
         else 
           plot2d(matrix(x,prod(size(x)),1),matrix(y,prod(size(y)),1));
           xtitle(' ',legx,' ');
         end;
  case 4 then xbasc();
         if type(y)==10,
           plot2d1("enn",1,matrix(x,prod(size(x)),1));
           xtitle(legy,y,legx);
         else 
           plot2d(matrix(x,prod(size(x)),1),matrix(y,prod(size(y)),1));
           xtitle(' ',legx,legy);
         end;
  case 5 then xbasc();
           plot2d(matrix(x,prod(size(x)),1),matrix(y,prod(size(y)),1));
           xtitle(leg,legx,legy);
end



  
endfunction
