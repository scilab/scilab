function [stk,txt,top]=sci_fseek()
// Copyright INRIA
txt=[]
origin=stk(top)
offset=stk(top-1)
fid=stk(top-2)
select  origin(1)
case '''bof''' then
  flag='''set'''
case '''cof''' then
  flag='''cur'''
case '''eof''' then
  flag='''end'''
case '-1' then
  flag='''set'''
case '0' then
  flag='''cur'''
case '1' then
  flag='''end''' 
else
  set_infos('Not enough information on '+origin(1)+' to set the proper flag',1)
  flag='fseek_origin('+origin(1)+')'
end
stk=list('mseek'+rhsargs([offset(1),fid(1),flag]),'0','1','1','1')

