// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function txt=help_skeleton(funname,path,language)
  
  
// Vérification des paramètres
// --------------------------------------------------------------------------------
  
  [lhs,rhs]=argn(0);
  
  if rhs > 3 | rhs < 1 then error(39); end
  if type(funname)<>10 then error('argument should be a function name'); end
  
  if rhs > 1 then
    if type(path) <> 10 then error(55,2); end
  end
  
  if rhs == 3 then
    if type(language) <> 10 then error(55,3); end
  else
    language = getlanguage();
  end
  
  txt=[];
  vars=macrovar(evstr(funname));
  inputs=vars(1);
  outputs=vars(2);
  context=vars(3);
  Call='';
  
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
  
  // Gestion de la date
  // --------------------------------------------------------------------------------
  w = getdate();
  xml_date = msprintf("%02d-%02d-%04d",w(6),w(2),w(1));
  
  xmlns='xmlns=""http://docbook.org/ns/docbook""'
  xlink='xmlns:xlink=""http://www.w3.org/1999/xlink""'
  svg='xmlns:svg=""http://www.w3.org/2000/svg""'
  mml=' xmlns:mml=""http://www.w3.org/1998/Math/MathML""'
  db='xmlns:db=""http://docbook.org/ns/docbook""'
  vers='version=""5.0-subset Scilab""'
  refs=strcat([xmlns xlink svg mml db vers],' ')
  select language
    
    // --------------------------------------------------------------------------------
    // FRENCH
    // --------------------------------------------------------------------------------
  case 'fr_FR' then
    //header
    txt=['<?xml version=""1.0"" encoding='"ISO-8859-1""?>'
	 '<!--'
	 ' * Ajouter ici d''éventuels commentaires sur le fichier XML'
	 '-->'
	 '<refentry '+refs+' xml:lang=""fr"" xml:id=""'+funname+'"">'
	 '  <info>'
	 '    <pubdate>$LastChangedDate: '+xml_date+' $</pubdate>'
	 '  </info>'
	 '  <refnamediv>'
	 '    <refname>'+funname+'</refname>'
	 '    <refpurpose>  Ajouter ici une description rapide de la fonction </refpurpose>'
	 '  </refnamediv>'
	 '  <refsynopsisdiv>'
	 '    <title>Séquence d''appel</title>'
	 '    <synopsis>'+Call+'</synopsis>'
	 '  </refsynopsisdiv>'];
    //Arguments
    if size(args,'*') >0 then
      txt=[txt;
	   '  <refsection>'
	   '    <title>Paramètres</title>'
	   '    <variablelist>'];
      for a=args'
	txt=[txt;
	     '      <varlistentry>'
	     '        <term>'+a+'</term>'
	     '        <listitem>'
	     '          <para>'
	     '            Ajouter ici la description du paramètre'
	     '          </para>'
	     '        </listitem>'
	     '      </varlistentry>'];
      end
      txt=[txt;
	   '    </variablelist>'
	   '  </refsection>'];
    end
    //Description
    txt=[txt;
	 '  <refsection>'
	 '    <title>Description</title>'
	 '    <para>'
	 '          Ajouter ici un paragraphe sur la description détaillée de la fonction.'
	 '          D''autres paragraphes peuvent être ajoutés."
	 '    </para>'
	 '  </refsection>'];
    //Exemple
    txt=[txt;
	 '  <refsection>'
	 '    <title>Exemples</title>'
	 '    <programlisting role=""example""><![CDATA['
	 '       Ajouter ici les instructions et commentaires scilab'
	 '    ]]></programlisting>'
	 '  </refsection>'];
    //See Also
    txt=[txt;
	 '  <refsection>'
	 '    <title>Voir aussi</title>'
	 '    <simplelist type=""inline"">'
	 '      <member>'
	 '        <link linkend=""ajouter le nom d''une référence"">Ajouter ici une référence</link>'
	 '      </member>'
	 '      <member>'
	 '        <link linkend=""ajouter le nom d''une référence"">Ajouter ici une référence</link>'
	 '      </member>'
	 '    </simplelist>'
	 '  </refsection>'];
    //Authors
    txt=[txt;
	 '  <refsection>'
	 '    <title>Auteurs</title>'
	 '    <simplelist type=""vert"">'
	 '      <member>ajouter le nom et les références de l''auteur</member>'
	 '      <member>ajouter le nom et les références d''un autre auteur</member>'
	 '    </simplelist>'
	 '  </refsection>'];
    //Bibliography
    txt=[txt;
	 '  <refsection>'
	 '     <title>Bibliographie</title>'
	 '       <para>'
	 '         ajouter ici la bibliographie relative à la fonction'
	 '       </para>'
	 '     </refsection>'];
    //Used functions
    txt=[txt;
	 '  <refsection>'
	 '     <title>Fonctions utilisées</title>'
	 '       <para>'
	 '         ajouter les références des codes Scilab,C,... utilisés'
	 '       </para>'
	 '   </refsection>'];

    
    //footer
    txt=[txt;
	 '</refentry>'];
    
    
    // --------------------------------------------------------------------------------
    // ENGLISH (par défaut)
    // --------------------------------------------------------------------------------
    
  else
    //header
    txt=['<?xml version=""1.0"" encoding='"ISO-8859-1""?>'
	 '<!--'
	 ' * Ajouter ici d''éventuels commentaires sur le fichier XML'
	 '-->'
	 '<refentry '+refs+' xml:lang=""fr"" xml:id=""'+funname+'"">'
	 '  <info>'
	 '    <pubdate>$LastChangedDate: '+xml_date+' $</pubdate>'
	 '  </info>'
	 '  <refnamediv>'
	 '    <refname>'+funname+'</refname>'
	 '    <refpurpose>  Add short description here. </refpurpose>'
	 '  </refnamediv>'
	 '  <refsynopsisdiv>'
	 '    <title>Calling Sequence</title>'
	 '    <synopsis>'+Call+'</synopsis>'
	 '  </refsynopsisdiv>'];
    //Arguments
    if size(args,'*') >0 then
      txt=[txt;
	   '  <refsection>'
	   '    <title>Parameters</title>'
	   '    <variablelist>']
      for a=args'
	txt=[txt;
	     '      <varlistentry>'
	     '        <term>'+a+'</term>'
	     '        <listitem>'
	     '          <para>'
	     '            Add here the parameter description'
	     '          </para>'
	     '        </listitem>'
	     '      </varlistentry>']
      end
      txt=[txt;
	   '    </variablelist>'
	   '  </refsection>'];
    end
    //Description
    txt=[txt;
	 '  <refsection>'
	 '    <title>Description</title>'
	 '    <para>'
	 '          Add here a paragraph of the function description. '
	 '          Other paragraph can be added "
	 '    </para>'
	 '  </refsection>'];
    //Exemple
    txt=[txt;
	 '  <refsection>'
	 '    <title>Examples</title>'
	 '    <programlisting role=""example""><![CDATA['
	 '        Add here scilab instructions and comments'
	 '    ]]></programlisting>'
	 '  </refsection>'];
    //See Also
    txt=[txt;
	 '  <refsection>'
	 '    <title>See Also</title>'
	 '    <simplelist type=""inline"">'
	 '      <member>'
	 '        <link linkend=""add a reference name"" >add a reference</link>'
	 '      </member>'
	 '      <member>'
	 '        <link linkend=""add a reference name"">add a reference</link>'
	 '      </member>'
	 '    </simplelist>'
	 '  </refsection>'];
    //Authors
    txt=[txt;
	 '  <refsection>'
	 '    <title>Authors</title>'
	 '    <simplelist type=""vert"">'
	 '      <member>add the author name and author reference</member>'
	 '      <member>add another author name and it''s reference</member>'
	 '    </simplelist>'
	 '  </refsection>'];
    //Bibliography
    txt=[txt;
	 '  <refsection>'
	 '     <title>Bibliography</title>'
	 '       <para>'
	 '         Add here the function bibliography'
	 '       </para>'
	 '     </refsection>'];
    //Used functions
    txt=[txt;
	 '  <refsection>'
	 '     <title>Used Functions</title>'
	 '       <para>'
	 '         Add here the Scilab, C,... used code references'
	 '       </para>'
	 '     </refsection>'];

    //footer
    txt=[txt;
	 '</refentry>'];		

    
  end
  
  if rhs >= 2 then
    mputl(txt,pathconvert(path,%t,%f)+funname+'.xml');
    txt=pathconvert(path,%t,%f)+funname+'.xml'
  end
  
endfunction
