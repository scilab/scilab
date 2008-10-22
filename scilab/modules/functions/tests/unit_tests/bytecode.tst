mode(-1)
//build the list of loaded libs
%libs=[];
for %name=who('local')'
  if %name<>'ans' then
    execstr('%typ=type('+%name+')');
    if %typ==14 then 
      if and(%name<>['parameterslib','simulated_annealinglib','genetic_algorithmslib','scilab2fortranlib']) then
        %libs=[%libs %name];
      end
    end
  end
end

for Lib=%libs
  execstr('funs=string('+Lib+')''');
  funs(1)=[];
  funcprot(0)
  for fun=funs
    if execstr('f='+fun','errcatch')==0 then
      if type(f)==13 then
	add_profiling(fun);
	remove_profiling(fun)
	c=bytecode(f);
	execstr('c1=bytecode('+fun+')');
	if  c<>c1 then disp(fun,Lib),pause,end
      end
    end
  end
end
