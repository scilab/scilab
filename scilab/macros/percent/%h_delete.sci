function %h_delete(varargin)

  if (length(varargin) == 1)
    hdl = varargin(1)
    for i=1:length(hdl)
      delete(hdl(i))
    end,
  end,

endfunction
