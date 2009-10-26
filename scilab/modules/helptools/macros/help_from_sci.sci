function [helptxt,demotxt]=help_from_sci(funname,helpdir,demodir)
// Generate help files and demo files from the head comments section of a .sci source file. 
//
// Calling Sequence
//  help_from_sci() // generate an empty function template
//  help_from_sci(funname,helpdir) // generate helpdir/funname.xml from funname.sci 
//  help_from_sci(dirname,helpdir) // process dirname/*.sci and create helpdir/*.xml help files.
//  help_from_sci(dirname,helpdir,helpdir) // as above but also creating helpdir/*.dem.sce demo files.
//  [helptxt,demotxt]=help_from_sci(funname) // return funname.xml and funname.dem.sce code as two text matrixes.
// Parameters
//  funname: the name of a single .sci source file to be processed.
//  dirname: directory name where all .sci files will be processed.
//  helpdir: optional path where the .xml help file will be created.
//  demodir: optional path where .dem.sce demo files will be created based on code from the Examples section.
//  helptxt: returns the XML help code if helpdir is empty, or the path to the new .xml file. 
//  demotxt: returns the demo code if demodir is empty, or the path to the new .dem.sc file. 
//
// Description
//  help_from_sci is a revised version of the help_skeleton function.
//  Its objective is to generate .xml help files based on the head comments section 
//  of .sci source files. Optionally .dem.sce demo files can be generated based on 
//  code from the Examples section in the head comments section of .sci files. 
//  
//  In order for help_from_sci to format the .xml file properly the  
//  head comments section should comply with some simple formatting rules.
//
//  The first comment line following the function definition should contain a short description 
//  of the function.
//
//  The remaining comments are formatted according to the following (optional) headlines:
//  "Calling Sequence", "Parameters", "Description", "Examples", "See also", "Used functions", 
//  "Authors" and "Bibliography". 
//
//  The following guidelines should be used when writing the source code comments:
//  <itemizedlist>
//    <listitem><literal>Calling Sequence</literal> - one example pr. line.</listitem>
//    <listitem><literal>Parameters</literal> - separate parameter name and
//    description by a ":". Keep the description of each parameter on the same line.</listitem>
//    <listitem><literal>Description</literal> - formatting of the text can be done 
//    using XML commands. 
//    Adding an empty comment line in the Description section is interpreted as the 
//    start of a new paragraph.</listitem>
//    <listitem><literal>See also</literal> - list one function name pr line.</listitem>
//    <listitem><literal>Authors</literal> - write one author on each line following 
//    the Authors headline. Use ";" to separate the authors name 
//    from any add additional information.</listitem>
//    <listitem><literal>Bibliography</literal> - write one reference pr line 
//    following the References headline.</listitem>
//  </itemizedlist>
//
// Examples
//  help_from_sci()   // Open an empty source code template in the xpad editor.
//  // Save this template as test_fun.sci in the current directory before running
//  // the next example commands.
//
//  help_from_sci('test_fun')        // return the xml skeleton as a text string
//
//  help_from_sci('test_fun','.')    // create the xml help file in the current directory.
//
//  // create both a xml help file and a demo file in the current directory.
//  help_from_sci('test_fun','.','.')
//
//  // From a toolbox root directory a typical calling sequence would be:
//  // help_from_sci('macros','help\en_US','demos')
//  // This command would process all .sci files in the macros directory 
//  // and use the head comments section to update all .xml help files in the 
//  // help\en_US directory an rebuild the .dem.sce files in the demos\ directory. 
//
// See also
//  help
//  help_skeleton
//  xmltohtml
// Authors
//  T. Pettersen ; torbjorn.pettersen@broadpark.no

//  Copyright Torbjørn Pettersen  2008
//
//  This software is a toolbox for Scilab
//
//  This software is governed by the CeCILL  license under French law and
//  abiding by the rules of distribution of free software.  You can  use, 
//  modify and/ or redistribute the software under the terms of the CeCILL
//  license as circulated by CEA, CNRS and INRIA at the following URL
//  "http://www.cecill.info". 
//
//  As a counterpart to the access to the source code and  rights to copy,
//  modify and redistribute granted by the license, users are provided only
//  with a limited warranty  and the software's author,  the holder of the
//  economic rights,  and the successive licensors  have only  limited
//  liability. 
//
//  In this respect, the user's attention is drawn to the risks associated
//  with loading,  using,  modifying and/or developing or reproducing the
//  software by the user in light of its specific status of free software,
//  that may mean  that it is complicated to manipulate,  and  that  also
//  therefore means  that it is reserved for developers  and  experienced
//  professionals having in-depth computer knowledge. Users are therefore
//  encouraged to load and test the software's suitability as regards their
//  requirements in conditions enabling the security of their systems and/or
//  data to be ensured and,  more generally, to use and operate it in the 
//  same conditions as regards security. 
//
//  The fact that you are presently reading this means that you have had
//  knowledge of the CeCILL license and that you accept its terms.

if execstr("getversion(""scilab"");","errcatch") <> 0 then
	error(gettext("Scilab 5.0 or more is required."));
end

if argn(2)==0 then
  template=[ ..
    'function [z]=function_template(x,y)'
    '// Short description on the first line following the function header.'
    '//'
    '// Calling Sequence'
    '//   [z]=template_function(x,y) // calling examples, one pr. line'
    '//'
    '// Parameters'
    '// x: the x parameter   // single line description of each parameter.'
    '// y: the y parameter   // parameter name and description must be '
    '// z: the z parameter   // separated by "":"".'
    '//'
    '// Description'
    '// Here is a description of the function.'
    '// Add an empty comment line to format the text into separate paragraphs.'
    '//'
    '// XML format commands may also be used directly in the text.'
    '// <itemizedlist>'
    '// <listitem>An itemized list is shown here</listitem>'
    '// </itemizedlist>'
    '// See the source code for help_from_sci.sci for more examples on how to write the initial comment section.'
    '//'
    '// Examples'
    '// [z]=test_fun(1,2) // examples of use'
    '//'
    '// // An empty comment line in the Examples section will add a halt() statement'
    '// // in the demo file test_fun.dem.sce generated by help_from_sci.'
    '// See also'
    '//  help_from_sci'
    '//  help_skeleton'
    '//'
    '// Authors'
    '//  Author name ; should be listed one pr line. Use "";"" to separate names from additional information '
    '// Bibliography'
    '//   Literature references one pr. line'
    ''
    '// start of coding after on empty line to mark the end of the head_comments section'
    'z = sin(x).*cos(x+y);'
    'endfunction'
  ];
  f=mopen(TMPDIR+filesep()+'function_template.sci','w'); mfprintf(f,'%s\n',template); mclose(f);
  if (isdef('editor') | (funptr('editor')<>0)) then
    editor(TMPDIR+filesep()+'function_template.sci'); helptxt=[];
  end
  return;
end

if argn(2)<3 then demodir=[]; end
if argn(2)<2 then helpdir=[]; end

if ~isempty(demodir) & ~isdir(demodir) then 
  error(gettext("...demodir must be a directory."));
end

if isdir(funname) then
  printf('\nReading from directory %s \n',funname);
  files=findfiles(funname,'*.sci');   // read *.sci files.
  for i=1:size(files,'r'),
    [tmp,out]=fileparts(files(i));
    if isempty(helpdir) then
      help_from_sci(funname+filesep()+files(i),'.',demodir);
      printf(' ...reading %s/%s.sci ... writing %s.xml',funname,out,out);
    else
      help_from_sci(funname+filesep()+files(i),helpdir,demodir);
      printf(' ...reading %s/%s.sci ... writing %s/%s.xml',funname,out,helpdir,out);
    end
    if ~isempty(demodir) then 
      printf(' and %s/%s.dem.sce\n',demodir,out); 
    else
      printf('\n');
    end
  end
  printf('help_from_sci processed %i files.\n',i);
  helptxt=''; return;
end

out=tokens(pathconvert(funname),filesep()); out=out($);
out=tokens(out,'.'); out=out(1);      // remove .sci (...wont work for fil.name.sci)

demotxt=['mode(1)'
         '//'
         '// Demo of '+out+'.sci'
         '//'
         ''];

verno=ver(); verno=verno(1,2);
helptxt=[
'<?xml version=""1.0"" encoding=""ISO-8859-1""?>'
''
'<!--'
' * '
' * This help file was generated from '+out+'.sci using help_from_sci().'
' * '
' -->'
''
'<refentry version=""5.0-subset Scilab"" xml:id=""'+out+'"" xml:lang=""en""'
'          xmlns=""http://docbook.org/ns/docbook""'
'          xmlns:xlink=""http://www.w3.org/1999/xlink""'
'          xmlns:svg=""http://www.w3.org/2000/svg""'
'          xmlns:ns3=""http://www.w3.org/1999/xhtml""'
'          xmlns:mml=""http://www.w3.org/1998/Math/MathML""'
'          xmlns:db=""http://docbook.org/ns/docbook"">'
''
'  <info>'
'    <pubdate>$LastChangedDate: '+date()+' $</pubdate>'
'  </info>'
''
]

if isempty(strindex(funname,'.sci')) then funname=funname+'.sci'; end;
if isempty(fileinfo(funname)) then error(funname+' does not exists'); end;
f=mopen(funname,'r');
if isempty(f) then error('Failed to read '+funname+'.sci'); end
line=' '; doc=[]; 

while isempty(strindex(line,'function ')) & ~meof(f), line=mgetl(f,1); end 
line=mgetl(f,1); short_descr=stripblanks(strsubst(line,'//',''),%T);
helptxt=[helptxt;
'  <refnamediv>'
'    <refname>'+out+'</refname><refpurpose>'+short_descr+'</refpurpose>'
'  </refnamediv>'
''];

cmds=['CALLING SEQUENCE','PARAMETERS','DESCRIPTION','EXAMPLES','SEE ALSO',..
      'AUTHORS','BIBLIOGRAPHY','USED FUNCTIONS'];

doing='search'; i=strindex(line,'//');
line=mgetl(f,1); 
while (~isempty(stripblanks(line)) & ~meof(f)),
  if stripblanks(line)=='//' then 
    if doing=='Description' then 
      in='new_descr_param'; 
    else
      in='';
    end
  else 
    in=strsplit(line,i(1)+1); 
    in=stripblanks(in(2)); 
  end

  IN=convstr(in,'u');
  if find(cmds==IN) then
    [add_txt,doing]=change_activity(doing,in)
    helptxt=[helptxt;add_txt];
  else
    if doing=='Calling Sequence' then
      helptxt=[helptxt;'   <synopsis>'+in+'</synopsis>'];
    elseif doing=='Parameters' then
      i=strindex(in,':'); 
      if ~isempty(i) then
        if length(in)>i(1) then
          in=strsplit(in,i(1)); par_name=in(1); par_descr=in(2);
        else
          par_name=in; par_descr=' ';
        end
        helptxt=[helptxt;'   <varlistentry><term>'+par_name+'</term>'];
        helptxt=[helptxt;'      <listitem><para>'+par_descr+'</para></listitem></varlistentry>'];
      end
    elseif doing=='Description' & in=='new_descr_param' then
      helptxt=[helptxt;'   </para>';'   <para>'];
    elseif doing=='Description' then
      helptxt=[helptxt;in];
    elseif doing=='Examples' & convstr(in,'u')~='EXAMPLES' then
      demotxt=[demotxt;in];
      if isempty(stripblanks(in)) then demotxt=[demotxt;'halt()   // Press return to continue']; end
      helptxt=[helptxt;in];
    elseif doing=='See also' & convstr(in,'u')~='SEE ALSO' & ~isempty(stripblanks(in)) then
      helptxt=[helptxt;'   <member><link linkend=""'+in+'"">'+in+'</link></member>'];
    elseif doing=='Authors' & convstr(in,'u')~='AUTHORS' & ~isempty(stripblanks(in)) then
      [name,ref]=chop(in,';');
      if isempty(ref) then
        helptxt=[helptxt;'   <varlistentry><term>'+name+'</term></varlistentry>'];
      else
        helptxt=[helptxt;'   <varlistentry><term>'+name+'</term><listitem><para>'+ref+'</para></listitem></varlistentry>'];
      end
    elseif doing=='Bibliography' & convstr(in,'u')~='BIBLIOGRAPHY' & ~isempty(stripblanks(in)) then

      helptxt=[helptxt;'   <para>'+in+'</para>'];
    elseif doing=='Used functions' & convstr(in,'u')~='USED FUNCTIONS' & ~isempty(stripblanks(in)) then
      helptxt=[helptxt;'   <para>'+in+'</para>'];
    end
  end
  line=mgetl(f,1); i=strindex(line,'//');
end
helptxt=[helptxt;change_activity(doing,'FINISHED')];
mclose(f);
if ~isempty(helpdir) then
  mputl(helptxt,pathconvert(helpdir,%t,%f)+out+'.xml');
  helptxt=pathconvert(helpdir,%t,%f)+out+'.xml'
end
demotxt=[demotxt;'//========= E N D === O F === D E M O =========//'];
if ~isempty(demodir) then
  f=mopen(demodir+filesep()+out+'.dem.sce','w'); 
  mfprintf(f,'%s\n',demotxt); 
  mclose(f);  
  demotxt=demodir+filesep()+out+'.dem.sce';
end
endfunction

////////////////////////////////////////////////////////////////////////
function [head,tail]=chop(str,tok)
  i=regexp(str,'/'+tok+'/','o');
  if isempty(i) then
    head=str;
    tail=[];
  else
    head=part(str,1:i-1);
    tail=part(str,i+1:length(str));
  end  
endfunction

////////////////////////////////////////////////////////////////////////
function [txt,doing]=change_activity(currently_doing,start_doing)
  doing=start_doing; 
  select convstr(currently_doing,"u")
    case 'CALLING SEQUENCE' then
      txt=['</refsynopsisdiv>'];
    case 'PARAMETERS' then
      txt=['   </variablelist>';'</refsection>'];
    case 'DESCRIPTION' then
      txt=['</para>';'</refsection>'];
    case 'EXAMPLES' then
      txt=['   </programlisting>';'</refsection>'];      
    case 'SEE ALSO' then
      txt=['   </simplelist>';'</refsection>'];
    case 'AUTHORS' then
      txt=['   </variablelist>';'</refsection>'];
    case 'BIBLIOGRAPHY' then
      txt=['</refsection>'];
    case 'USED FUNCTIONS' then
      txt=['</refsection>'];
    else
      txt='';
  end    

  select convstr(start_doing,"u"),
      case 'CALLING SEQUENCE'
      txt=[txt;'';'<refsynopsisdiv>';'   <title>Calling Sequence</title>'];
    case 'PARAMETERS'
      txt=[txt;'';'<refsection>';'   <title>Parameters</title>';'   <variablelist>'];
    case 'DESCRIPTION'
      txt=[txt;'';'<refsection>';'   <title>Description</title>';'   <para>'];
    case 'EXAMPLES'
      txt=[txt;'';'<refsection>';'   <title>Examples</title>';'   <programlisting role=""example"">'];
    case 'SEE ALSO'
      txt=[txt;'';'<refsection>';'   <title>See also</title>';'   <simplelist type=""inline"">'];
    case 'AUTHORS'
      txt=[txt;'';'<refsection>';'   <title>Authors</title>';'   <variablelist>'];
    case 'BIBLIOGRAPHY'
      txt=[txt;'';'<refsection>';'   <title>Bibliography</title>'];
    case 'USED FUNCTIONS'
      txt=[txt;'';'<refsection>';'   <title>Used functions</title>'];
    case 'FINISHED'
      txt=[txt;'</refentry>'];
  end
endfunction
