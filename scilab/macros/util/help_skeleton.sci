function txt=help_skeleton(funname)
global LANGUAGE
if type(funname)<>10 then error('argument should be a function name'),end
txt=[]
vars=macrovar(evstr(funname))
inputs=vars(1)
outputs=vars(2)
context=vars(3)
Call=''
if size(outputs,'*')==1 then
  Call=outputs+' = '
elseif size(outputs,'*')>1 then
  Call='['+strcat(outputs,',')+'] = '
end
Call=Call+funname
if size(inputs,'*')>=1 then
  Call=Call+'('+strcat(inputs,',')+')'
end
args=[inputs(:);outputs(:)]

txt=[
'<?xml version=""1.0"" encoding=""ISO-8859-1"" standalone=""no""?>' 
'<!DOCTYPE MAN SYSTEM ""../../man.dtd"">'
'<MAN>'
'  <LANGUAGE>eng</LANGUAGE>'
'  <TITLE>'+funname+'</TITLE>'
'  <TYPE>Scilab Function  </TYPE>'
'  <DATE>'+date()+'</DATE>'
'  <SHORT_DESCRIPTION name=""'+funname+'"">  add short decription here</SHORT_DESCRIPTION>'
''
'  <CALLING_SEQUENCE>'
'  <CALLING_SEQUENCE_ITEM>'+Call+'</CALLING_SEQUENCE_ITEM>'
'  </CALLING_SEQUENCE>'
'']
if size(args,'*') >0 then
txt=[txt;
'  <PARAM>'
'  <PARAM_INDENT>']
for a=args'
txt=[txt;"";
'    <PARAM_ITEM>'
'    <PARAM_NAME>'+a+'</PARAM_NAME>'
'    <PARAM_DESCRIPTION>'
'       : add here the parameter description'
'    </PARAM_DESCRIPTION>'
'    </PARAM_ITEM>']
end
txt=[txt;
'  </PARAM_INDENT>'
'  </PARAM>'
' ']
end
txt=[txt;
'  <DESCRIPTION>'
'   Add here the description of the function'
'  </DESCRIPTION>'
''
'  <EXAMPLE><![CDATA['
'   Add here scilab instructions and comments'
'  ]]></EXAMPLE>'
''
'  <SEE_ALSO>'
'    <SEE_ALSO_ITEM> <LINK> add a key here</LINK> </SEE_ALSO_ITEM>'
'    <SEE_ALSO_ITEM> <LINK> add a key here</LINK> </SEE_ALSO_ITEM>'
'  </SEE_ALSO>'
''
'  <BIBLIO>'
'    Add here the function bibliography if any'
'  </BIBLIO>'
''
'  <AUTHOR>'
'    Add here the author name and references'
'  </AUTHOR>'
'</MAN>'
]

//write(%io(2),txt,'(a)')





endfunction
