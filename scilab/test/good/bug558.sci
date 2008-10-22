// exec("bug558.sce");
fileName="bug558.txt";
diary(fileName);
x = 20;
mtlb_save('bug558','x')
diary(0);

w=fileinfo(fileName);
if w(1) == 0 then
  affich_result(%F,558);
else
  affich_result(%T,558);
end;
 
mdelete(fileName);
mdelete('bug558.mat');
    
