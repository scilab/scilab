// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function txt=infer2txt(infer)
txt=[]
dims=[]
if typeof(infer)=="infer" then
  for l=1:size(infer.dims)
    dims=[dims,string(infer.dims(l))]
  end
else
  error(gettext("Not yet implemented."))
end
dims=strcat(dims," ")

tp=infer.type.vtype
if tp==1 then
  tp="Double"
elseif tp==10 then
  tp="String"
elseif or(tp==[4,6]) then
  tp="Boolean"
elseif tp==16 then
  tp="Struct"
  for k=1:lstsize(infer.contents.index)
    if typeof(infer.contents.index(k))<>"list" then
      txt=[txt;expression2code(list(infer.contents.index(k)))+infer2txt(infer.contents.data(k))]
    else
      txt=[txt;expression2code(infer.contents.index(k))+infer2txt(infer.contents.data(k))]
    end
  end
elseif tp==17 then
  tp="Cell"
  for k=1:lstsize(infer.contents.index)
    if typeof(infer.contents.index(k))<>"list" then
      txt=[txt;expression2code(list(infer.contents.index(k)))+infer2txt(infer.contents.data(k))]
    else
      txt=[txt;expression2code(infer.contents.index(k))+infer2txt(infer.contents.data(k))]
    end
  end
elseif tp==9 then
  tp="Handle"
else
  tp="Unknown"
end
if infer.type.property==Real then
  prop="Real"
elseif infer.type.property==Complex then
  prop="Complex"
else
  prop="Unknown"
end
txt=["|"+dims+"|"+tp+"|"+prop;txt]
endfunction
