function scicos_learn(fil)
// Copyright INRIA
funcprot(0);
comm='/'+'/'
x_mdia=funptr('x_mdialog')
c_cho=funptr('x_choose')
xcli=funptr('xclick')
xgetm=funptr('xgetmouse')

clearfun('xclick');newfun('xclick1',xcli);
deff('[c_i,c_x,c_y,c_w,str]=xclick()',[
    '[lhs,rhs]=argn(0)'
    'if lhs==3 then '
    '  [c_i,c_x,c_y]=xclick1()'
    '  write(uapp,strcat(string([c_i,c_x,c_y]),'','')+comm+''xclick'')'
    'elseif lhs==4 then'
    '  [c_i,c_x,c_y,c_w]=xclick1()'
    '  write(uapp,strcat(string([c_i,c_x,c_y,c_w]),'','')+comm+''xclick'')'
    'else'
    '  [c_i,c_x,c_y,c_w,str]=xclick1()'
    '  txt=strcat([string([c_i,c_x,c_y,c_w]),sci2exp(str)],'','')'
    '  write(uapp,txt+comm+''xclick'')'
    'end']);

clearfun('xgetmouse');newfun('xgetmouse1',xgetm)
deff('rep=xgetmouse()',[
    'rep=xgetmouse1()'
    'write(uapp,strcat(string(rep),'','')+comm+''xgemouse'')']);


deff('result=dialog(labels,valueini)',[
    'result=x_dialog(labels,valueini)'
    'res=result'
    'res(1)=res(1)+comm+''x_dialog'''
    'write(uapp,res)'])

deff('num=message(strings ,buttons)',[
'[lhs,rhs]=argn(0)'
'if rhs==2 then'
'  num=x_message(strings ,buttons)'
'  write(uapp,buttons(num)+comm+ ''message'')'
'else'
'  num=1'
'  x_message(strings)'
'end'])

clearfun('x_mdialog');newfun('x_mdialog1',x_mdia);
deff('result=x_mdialog(title,labels,default_inputs_vector)',[
    'result=x_mdialog1(title,labels,default_inputs_vector)'
    'if result<>[] then'
    '  res=result'
    '  res(1)=res(1)+comm+''x_mdialog'''
    '  write(uapp,res)'
    '  write(uapp,''o'')'
    'else'
    '  write(uapp,default_inputs_vector)'
    '  write(uapp,''c'')'
    'end'])

clearfun('x_choose');newfun('x_choose1',c_cho);
deff('num=x_choose(items,title,button)',[
    '[lhs,rhs]=argn(0)'
    'if rhs==3 then '
    '  num=x_choose1(items,title,button)'
    'else'
    '  num=x_choose1(items,title)'
    'end'
    'write(uapp,string(num)+comm+''x_choose'')'])

getf('SCI/macros/util/getvalue.sci');
getf('SCI/macros/auto/scicos.sci')

names=['choosefile';
'do_addnew';
'do_block';
'do_color';
'do_copy';
'do_copy_region';
'do_delete';
'do_delete_region';
'do_help';
'do_move';
'do_palettes';
'do_replace';
'do_run';
'do_tild';
'do_view';
'getlink';
'move';
'prt_align';
'cosclick';]
for k=1:size(names,'r')
  getf('SCI/macros/scicos/'+names(k)+'.sci');
end

deff('c=getcolor(title,cini)',[
'colors=string(1:xget(""lastpattern""))'
'm=prod(size(cini))'
'll=list()'
'm=prod(size(cini))'
'for k=1:m'
'  ll(k)=list(''colors'',cini(k),colors);'
'end'
'c=x_choices(title,ll);'
'write(uapp,string(c)+comm+''getcolor'')'])
uapp=file('open',fil,'unknown');
lines(0);
scicos();
file('close',uapp);
newfun('x_mdialog',x_mdia)
newfun('x_choose',c_cho)
newfun('xclick',xcli)
newfun('xgetmouse',xgetm)


