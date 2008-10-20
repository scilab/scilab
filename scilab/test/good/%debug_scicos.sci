function [block] = %debug_scicos(block,flag)
  if execstr('load(TMPDIR+''/debug_scicos'')','errcatch')<>0 then
    disp('Instantiate the Debug block')
  else
    [block] = debug_scicos(block,flag)
  end
  
endfunction
