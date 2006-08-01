function [ok,mac,txt]=genfunc(txt)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<1 then txt=' ',end
mac=[]
while %t do
  txt=dialog(['Set Function Block';
      ' '
      'Enter Scilab instructions defining'
      'y as a function of u'],txt)


  if txt==[] then ok=%f,return,end
  // check if txt defines y from u
  deff('[]=mac()',txt)
  vars=macrovar(mac)
  if or(vars(3)=='u')&or(vars(5)=='y') then break,end
end
ok=%t
deff('[%_1,%_2]=mac(%_model,%_x,%_z,u,%_clock,%_flag,%_rpar,%_ipar)',..
      ['%_1=[];%_2=[];';
      'select %_flag';
      'case 1 then';
      txt
      '%_1=y';
      'case -1 then ';
      '  %_model=list(%_model(1),1,1,[],[],[],[],[],[],''c'',%f,[%t %f])';
      '  %_1=list(%_model,'' '')';
      'case -2 then ';
      '  txt=%_model.ipar';
      '  [ok,mac,txt]=genfunc(txt)';
      '  if ok then '
      '    %_model.sim=mac'
      '    %_model.ipar=txt'
      '    %_1=list(model,%_x)'
      '  end'
      'end'])
endfunction
