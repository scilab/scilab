function   bool=iscellstr(c)
// Copyright INRIA
// This function returns 1 if input argument is a cell of strings, else it returns 0.  
// F.B

if argn(2)<>1 then
  error("Bad number of inputs arguments")
else
  if isempty(c) then
    bool=bool2s(%f)
  else
    if typeof(c)=="ce"
      bool=bool2s(%t)
      for i=1:size(c,"*")
        if typeof(c(i).entries)<>"string" then
          bool=bool2s(%f)
          break
        end
      end
    else
      bool=bool2s(%f)
    end
  end
end

endfunction