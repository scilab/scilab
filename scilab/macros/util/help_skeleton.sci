function txt=help_skeleton(funname,path)
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
'<!DOCTYPE MAN SYSTEM ""file://'+pathconvert(SCI,%t,%t)+'man/manrev.dtd"">'
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
'       <SP>'     
'       : add here the parameter description'
'       </SP>'         
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
     '     <DESCRIPTION_INDENT>'     
     '     <DESCRIPTION_ITEM>'     
     '     <P>'     
     '      Add here a paragraph of the function description. '
     '      Other paragraph can be added '
     '     </P>'   
     '     </DESCRIPTION_ITEM>' 
     '     <DESCRIPTION_ITEM>'     
     '     <P>'     
     '      Add here a paragraph of the function description '
     '     </P>'   
     '     </DESCRIPTION_ITEM>' 
     '     </DESCRIPTION_INDENT>'       
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
    '  <AUTHORS>'
     '    <AUTHORS_ITEM label=''enter here the author name''>'      
     '    Add here the author  references'
     '    </AUTHORS_ITEM>'
     '  </AUTHORS>'
     ''
     '  <BIBLIO>'
     '    <SP>'  
     '    Add here the function bibliography if any'
     '    </SP>'  
     '  </BIBLIO>'
     ''
     '  <USED_FUNCTIONS>'
     '     <SP>'  
     '     Add here the used function name and  references'
     '     </SP>'  
     '  </USED_FUNCTIONS>'     
     '</MAN>'
    ]
if argn(2)==2 then
  mputl(txt,pathconvert(path,%t,%f)+funname+'.xml')
  txt=pathconvert(path,%t,%f)+funname+'.xml'
end
endfunction
