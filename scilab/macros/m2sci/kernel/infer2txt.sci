function txt=infer2txt(infer)
// Copyright INRIA
txt=[]
dims=[]
if typeof(infer)=="infer" then
  for l=1:size(infer.dims)
    dims=[dims,string(infer.dims(l))]
  end
else
  txt=" contains a "+typeof(infer)
  if typeof(infer)=="st" then
    f=getfield(1,infer)
    txt=txt+"ruct with fields: "+strcat(f(3:$)," ")
  end
  return
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
  stdims=double(infer.contents.dims)
  if stdims<>[] then
    for kd1=1:stdims(1)
      for kd2=1:stdims(2)
	fields=getfield(1,infer.contents)
	for kf=3:size(fields,"*")
	  if typeof(infer.contents(kd1,kd2)(fields(kf)))<>"constant" then
	    txt=[txt;"("+string(kd1)+","+string(kd2)+")."+fields(kf)+infer2txt(infer.contents(kd1,kd2)(fields(kf)))];
	  else
	    txt=[txt;"("+string(kd1)+","+string(kd2)+")."+fields(kf)+" No infos"]
	  end
	end
      end
    end
  else
    txt="No infos"
  end
elseif tp==17 then
  tp="Cell"
  if infer.contents<>list() then
    cedims=double(infer.contents.dims)
    if cedims<>[] then
      for kd1=1:cedims(1)
	for kd2=1:cedims(2)
	  if typeof(infer.contents(kd1,kd2).entries)<>"constant" then
	    txt=[txt;"{"+string(kd1)+","+string(kd2)+"}"+infer2txt(infer.contents(kd1,kd2).entries)];
	  else
	    txt=[txt;"{"+string(kd1)+","+string(kd2)+"} No infos"]
	  end
	end
      end
    else
      txt="No infos"
    end
  else
    txt="No infos"
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
