function delete(varargin) 

[lhs,rhs]=argn(0);

if rhs>0
  winNum=varargin(1);
  if ~or(winsid()==winNum)
    error(sprintf('delete : window %d does no exist',winNum)); 
  end
else
  if winsid()==[]
     error(sprintf('delete : no current window to delete'));
  else
     winNum=xget('window');
  end
end

xdel(winNum)

saveGraphicState(list(),winNum)
