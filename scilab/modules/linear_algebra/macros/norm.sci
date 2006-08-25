function y=norm(A,flag)
// Copyright INRIA  
//compute various matrix norms
if argn(2)==1 then flag=2,end

if type(A)==1 then
  if A==[] then y=0,return,end
  if or(size(A)==1) then // vector norm
    if type(flag)==10 then //'inf' or 'fro'
      select convstr(part(flag,1))
      case 'i' then //'inf'
	y=max(abs(A))
      case 'f' then //'fro'
	A=A(:)
	y=sqrt(A'*A)
      else
	error("invalid value for flag")
      end
    elseif type(flag)==1 then //p_norm
      p=flag;
      if ~isreal(p) then
	error('flag must be real')
      end
      if p==%inf then
	y=max(abs(A))
      elseif p==1 then
	y=sum(abs(A))
      elseif p==-%inf then
	y=min(abs(A))
      elseif isnan(p) then
	y=%nan
      elseif p==0 then
	y=%inf
      else
	y=sum(abs(A).^p)^(1/p)
      end
    else
      error("invalid value for flag")
    end
  else //matrix norm
    if type(flag)==10 then //'inf' or 'fro'
      select convstr(part(flag,1))
      case 'i' then //'inf'
	y=max(sum(abs(A),2))  
	case 'f' then //'fro'
	if size(A,1)>size(A,2) then
	  y=sqrt(sum(diag(A'*A))) 
	else
	  y=sqrt(sum(diag(A*A'))) 
	end
      else
	error("invalid value for flag")
      end
    elseif type(flag)==1 then //p_norm
      p=flag;
      select p
	case 1 then 
	y=max(sum(abs(A),1))
	case 2 then
	y=max(svd(A))
	case %inf then
	y=max(sum(abs(A),2))  
      else
	error('flag must be 1 2 or inf')
      end
    else
      error("invalid value for flag")
    end    
  end
else
  if type(A)==16|type(A)==17 then
    n=getfield(1,A);n=n(1)
  else
    [t,n]=typename()
    n=stripblanks(n(find(t==type(A))))
  end
  fun='%'+n+'_norm'
  if exists(fun)==1 then
    execstr('y='+fun+'(A,flag)')
  else
    error('norm not defined for type ""'+n+'"" .'+..
	  'Check argument or define function '+fun)
  end
end
endfunction

