function fprintf(fil,frmt,varargin)
// fprintf - Emulator of C language fprintf
//!
// Copyright INRIA
if type(fil)==1 then
  [units,typ] =file()
  k=find(units==fil)
  if k==[] then
    write(fil,sprintf(frmt,varargin(:)),'(a)')
  elseif typ(k)=='F' then
    write(fil,sprintf(frmt,varargin(:)),'(a)')
  else
    mfprintf(fil,frmt,varargin(:))
  end
else
  write(fil,sprintf(frmt,varargin(:)),'(a)')
end
endfunction
