function [blocks] = %debug_scicos(blocks,flag)
if execstr('load(TMPDIR+''/debug_scicos'')','errcatch')<>0 then
  disp('Instantiate the Debug block')
else
  [blocks] = debug_scicos(blocks,flag)
end
  
endfunction
