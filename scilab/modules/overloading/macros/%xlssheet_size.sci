function [m,n]=%xlssheet_size(sheet,opt)
  if argn(2)>1 then
    m=size(sheet.value,opt)
  else
    m=size(sheet.value)
    if argn(1)>1 then
      [m,n]=(m(1),m(2))
    end
  end
endfunction
