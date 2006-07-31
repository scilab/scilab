function out_vect = %h_get(varargin)
  if (lstsize(varargin) == 2)
    hdl = varargin(1)
    str = varargin(2)
    for i=1:size(hdl,'*')
      out_vect(i) = get(hdl(i),str)
    end,

  elseif (lstsize(varargin) == 1)
    str = varargin(1)
    out_vect = get(str)
  end,
endfunction
