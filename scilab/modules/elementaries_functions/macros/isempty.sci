function answ=isempty(m)

// Copyright INRIA . 1998  1 
select type(m),
case 10
  answ =  max(length(m)) == 0;
case 15
  answ = %t;
  for i=1:size(m),
    answ=(answ & isempty(m(i)));
  end;
else
  answ = size(m,'*')==0;
end;
endfunction
