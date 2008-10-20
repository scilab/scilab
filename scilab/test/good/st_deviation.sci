function sd=st_deviation(x,cr)
//Copyright Inria/Enpc 
//Philippe.Castagliola Ecole des Mines de Nantes
// 
  if argn(2)<2 then cr='*',end
  if x == [] then sd=%nan;return ;end 
  if typeof(x)=='hypermat' then sd=%hm_st_deviation(x,cr),return,end
  [m,n]=size(x);
  if cr=='*' then
    n=m*n
    select n
      case 0 then sd=%nan
      case 1 then sd=0
    else 
      sd=sqrt(sum((x-mean(x)).^2)/(n-1));
    end
  elseif cr=='c'|cr==2
    if n==1 then
      sd=zeros(m,1)
    else
      sd=sqrt(sum((x-mean(x,'c')*ones(x(1,:))).^2,'c')/(n-1));
    end
  elseif cr=='r'|cr==1
    if m==1 then
      sd=zeros(1,n)
    else
      sd=sqrt(sum((x-ones(x(:,1))*mean(x,'r')).^2,'r')/(m-1));
    end
  else
    error('Optional 2nd argument cr must be equal to ''*'', ''c'' or 2, ''r'' or 1');
  end
endfunction
