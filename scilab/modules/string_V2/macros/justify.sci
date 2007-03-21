function t=justify(t,job)
  //Justify character array.
  [m,n]=size(t)
  for k=1:n
    L=length(t(:,k));mx=max(L);
    if part(job,1)=='l' then //right
      t(:,k)=part(t(:,k),1:mx)
    elseif part(job,1)=='r' then //left
      for l=1:m
	t(l,k)=part(' ',1:(mx-length(t(l,k))))+t(l,k)
      end
    elseif part(job,1)=='c' then //center
      for l=1:m
	nl=int((mx-length(t(l,k)))/2)
	nr=(mx-length(t(l,k)))-nl
	t(l,k)=part(' ',1:nl)+t(l,k)+part(' ',1:nr)
      end
    else
      error('justify: Invalid option')
    end
  end
endfunction
