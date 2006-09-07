function [%val,%ierr]=evstr(%str)
// Copyright INRIA
[lhs,rhs]=argn(0)
%val=[]
select type(%str)
 case 10 then
    for %l=1:size(%str,'r')
      %t1(%l)=strcat(%str(%l,:),',')+';'
    end
    %t1(1)='%val=['+%t1(1);
    %t1($)=part(%t1($),1:length(%t1($))-1)+']'
    if lhs==2 then
      %ierr=execstr(%t1,'errcatch')
    else
      execstr(%t1)
    end
 case 15 then
    %sexp=%str(2),
    %nstr=prod(size(%sexp)); %=list();
    if lhs==2 then
      for %k=1:%nstr,
	[%w,%ierr]=evstr(%sexp(%k)),
	%(%k)=%w
	if %ierr<>0 then %val=[];return;end
      end,
      [%val,%ierr]=evstr(%str(1))
    else
      for %k=1:%nstr, %(%k)=evstr(%sexp(%k)),end,
      %val=evstr(%str(1))
    end
case 1 then
   %val=%str
   %ierr=0
else 
   error('waiting for: matrix of strings or list'),
end
endfunction
