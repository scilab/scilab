function []=mtlb_format(type,prec)
// Copyright INRIA
// Emulation function for format() Matlab function
// V.C.

rhs1=rhs
if rhs==2 then
  fp=sci2exp('e')
  rhs=rhs-1
else
  fp=sci2exp('v')
end

d=5;
hex=%f;
bank=%f;
compact=%f;
loose=%f,
ratf=%f;
symb=%f;
unk=%f

if rhs==1 then
  select type
  case "''short''" then
    d=5
  case "''long''" then
    d=15
  case "''hex''" then
    hex=%t
  case "''+''" then
    symb=%t
  case "''bank''" then
    bank=%t
  case "''compact''" then
    compact=%t
  case "''loose''" then
    loose=%t
  case "''rat''" then
    ratf=%t
  else
    unk=%t
  end
else
  d=5
end


if unk then
  error("Unknown type in mtlb_format() !");
elseif ratf|loose|compact|bank|hex then
  warning("Unknown type "+type+" in mtlb_format(): INGNORED !")
else
  if rhs1==1 then
    format(d+1)
  elseif rhs1==2 then
    format(fp,d+1)
  else
    format("v",6)
  end
end
endfunction
