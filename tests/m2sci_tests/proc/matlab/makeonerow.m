% Copyright INRIA
function y=makeonerow(x)
if ~ischar(x)
  disp('Input argument is not a string matrix...');
  return;
end
y=['['];
newline=1;
for k=1:size(x,1)
  for l=1:size(x,2)
    if newline==1
      if x(k,l)==0
	y=sprintf('%s%s',y,['setstr(0)']);
      else
	y=sprintf('%s%s',y,['''',x(k,l),'''']);
      end
      newline=0;
    else
      if x(k,l)==0
	y=sprintf('%s%s',y,[',','setstr(0)']);
      else
	y=sprintf('%s%s',y,[',','''',x(k,l),'''']);
      end
    end
  end
  newline=1;
  if k~=size(x,1)
    y=sprintf('%s%s',y,[';']);
  end
end
y=sprintf('%s%s',y,']');
