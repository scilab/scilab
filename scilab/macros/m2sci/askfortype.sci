function r=askfortype(nam)
// Copyright INRIA
types=['1','10','4']
rowd=['1','1','?','?']
cold=['1','?','1','?']
l1=list('variable type',1,['Matrix','String','Boolean']);
l2=list('Dimension',2,['1 x 1','row','column','general']);
label='Give type and dimensions of variable '+nam
rep=x_choices(label,list(l1,l2));
if rep==[] then 
  r=[] 
else
  r=list(types(rep(1)),rowd(rep(2)),cold(rep(2)))
end

  
endfunction
