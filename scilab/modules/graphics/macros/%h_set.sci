function hdl=%h_set(varargin)
  if (lstsize(varargin) == 3)
    hdl = varargin(1)
    str = varargin(2)
    val = varargin(3)
    for i=1:size(hdl,'*')
      set(hdl(i),str,val)
    end,

  elseif (lstsize(varargin) == 2)
    str = varargin(1)
    val = varargin(2)
    hdl=[]
    set(str,val)
  end,
endfunction


