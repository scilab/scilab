function txt=help_skeleton(funname,path,language)
	
	global LANGUAGE
	
	// Vérification des paramètres
	// -----------------------------------------------------------------------------------------
	
	[lhs,rhs]=argn(0);
	
	if rhs > 3 | rhs < 1 then error(39); end
	if type(funname)<>10 then error('argument should be a function name'); end
	
	if rhs > 1 then
		if type(path) <> 10 then error(55,2); end
	end
	
	if rhs == 3 then
		if type(language) <> 10 then error(55,3); end
	else
		language = LANGUAGE;
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
	// -----------------------------------------------------------------------------------------
	w = getdate();
	xml_date = msprintf("%02d-%02d-%04d",w(6),w(2),w(1));
	
	select language
	
	// -----------------------------------------------------------------------------------------
	// FRENCH
	// -----------------------------------------------------------------------------------------
	
	case 'fr' then
		
		txt=[
			"<?xml version=""1.0"" encoding=""ISO-8859-1"" standalone=""no""?>" 
			"<!DOCTYPE MAN SYSTEM ""file://"+pathconvert(SCI,%f,%f)+"/modules/helptools/help.dtd"">"
			"<MAN>"
			"  <LANGUAGE>fr</LANGUAGE>"
			"  <TITLE>"+funname+"</TITLE>"
			"  <TYPE>Fonction Scilab</TYPE>"
			"  <DATE>"+xml_date+"</DATE>"
			"  <SHORT_DESCRIPTION name="""+funname+""">Ajouter ici une description rapide de la fonction</SHORT_DESCRIPTION>"
			""
			"  <CALLING_SEQUENCE>"
			"    <CALLING_SEQUENCE_ITEM>"+Call+"</CALLING_SEQUENCE_ITEM>"
			"  </CALLING_SEQUENCE>"
			""];
		
		if size(args,'*') >0 then
			
			txt=[txt;
				"  <PARAM>"
				"    <PARAM_INDENT>"];
				
			for a=args'
				txt=[txt;
					"";
					"      <PARAM_ITEM>"
					"        <PARAM_NAME>"+a+"</PARAM_NAME>"
					"        <PARAM_DESCRIPTION>"
					"          <SP>"
					"            Ajouter ici la description du paramètre"
					"         </SP>"
					"       </PARAM_DESCRIPTION>"
					"      </PARAM_ITEM>"];
			end
			
			txt=[txt;
				"    </PARAM_INDENT>"
				"  </PARAM>"
				""];
			
		end
		
		txt=[txt;
			"  <DESCRIPTION>"
			"    <DESCRIPTION_INDENT>"
			"      <DESCRIPTION_ITEM>"
			"        <P>"
			"          Ajouter ici un paragraphe sur la description détaillée de la fonction."
			"          D''autres paragraphes peuvent être ajoutés"
			"        </P>"
			"      </DESCRIPTION_ITEM>"
			"      <DESCRIPTION_ITEM>"
			"        <P>"
			"          Ajouter ici un paragraphe sur la description détaillée de la fonction."
			"        </P>"
			"      </DESCRIPTION_ITEM>"
			"    </DESCRIPTION_INDENT>"
			"  </DESCRIPTION>"
			""
			"  <EXAMPLE>"
			"    <![CDATA["
			"      Ajouter ici les instructions et commentaires scilab"
			"    ]]>"
			"  </EXAMPLE>"
			""
			"  <SEE_ALSO>"
			"    <SEE_ALSO_ITEM><LINK>Ajouter ici un lien</LINK></SEE_ALSO_ITEM>"
			"    <SEE_ALSO_ITEM><LINK>Ajouter ici un lien</LINK></SEE_ALSO_ITEM>"
			"  </SEE_ALSO>"
			""
			"  <AUTHORS>"
			"    <AUTHORS_ITEM label=""Entrer ici le nom de l''auteur"">"
			"      Ajouter ici les références de l''auteur"
			"    </AUTHORS_ITEM>"
			"  </AUTHORS>"
			""
			"  <BIBLIO>"
			"    <SP>"  
			"      S''il elle existe, ajouter ici la bibiographie de l''auteur"
			"    </SP>"
			"  </BIBLIO>"
			""
			"  <USED_FUNCTIONS>"
			"    <SP>"
			"      Ajouter ici les fonctions utilisées ainsi que leurs références"
			"    </SP>"
			"  </USED_FUNCTIONS>"
			"</MAN>"
		];
		
		
	// -----------------------------------------------------------------------------------------
	// ENGLISH (par défaut)
	// -----------------------------------------------------------------------------------------
	
	else
		
		txt=[
			"<?xml version=""1.0"" encoding=""UTF-8""?>" 
			"<!DOCTYPE MAN SYSTEM ""file://"+pathconvert(SCI,%f,%f)+"/modules/helptools/help.dtd"">"
			"<MAN>"
			"  <LANGUAGE>eng</LANGUAGE>"
			"  <TITLE>"+funname+"</TITLE>"
			"  <TYPE>Scilab Function</TYPE>"
			"  <DATE>"+xml_date+"</DATE>"
			"  <SHORT_DESCRIPTION name="""+funname+""">Add short description here</SHORT_DESCRIPTION>"
			""
			"  <CALLING_SEQUENCE>"
			"    <CALLING_SEQUENCE_ITEM>"+Call+"</CALLING_SEQUENCE_ITEM>"
			"  </CALLING_SEQUENCE>"
			""];
		
		if size(args,'*') >0 then
			
			txt=[txt;
				"  <PARAM>"
				"    <PARAM_INDENT>"];
				
			for a=args'
				txt=[txt;
					"";
					"      <PARAM_ITEM>"
					"        <PARAM_NAME>"+a+"</PARAM_NAME>"
					"        <PARAM_DESCRIPTION>"
					"          <SP>"     
					"            Add here the parameter description"
					"         </SP>"         
					"       </PARAM_DESCRIPTION>"
					"      </PARAM_ITEM>"];
			end
			
			txt=[txt;
				"    </PARAM_INDENT>"
				"  </PARAM>"
				""];
			
		end
		
		txt=[txt;
			"  <DESCRIPTION>"
			"    <DESCRIPTION_INDENT>"
			"      <DESCRIPTION_ITEM>"
			"        <P>"     
			"          Add here a paragraph of the function description. "
			"          Other paragraph can be added "
			"        </P>"
			"      </DESCRIPTION_ITEM>"
			"      <DESCRIPTION_ITEM>"
			"        <P>"     
			"          Add here a paragraph of the function description "
			"        </P>"
			"      </DESCRIPTION_ITEM>"
			"    </DESCRIPTION_INDENT>"      
			"  </DESCRIPTION>"
			""
			"  <EXAMPLE>"
			"    <![CDATA["
			"      Add here scilab instructions and comments"
			"    ]]>"
			"  </EXAMPLE>"
			""
			"  <SEE_ALSO>"
			"    <SEE_ALSO_ITEM><LINK>Add a key here</LINK></SEE_ALSO_ITEM>"
			"    <SEE_ALSO_ITEM><LINK>Add a key here</LINK></SEE_ALSO_ITEM>"
			"  </SEE_ALSO>"
			""
			"  <AUTHORS>"
			"    <AUTHORS_ITEM label=""enter here the author name"">"
			"      Add here the author references"
			"    </AUTHORS_ITEM>"
			"  </AUTHORS>"
			""
			"  <BIBLIO>"
			"    <SP>"  
			"      Add here the function bibliography if any"
			"    </SP>"
			"  </BIBLIO>"
			""
			"  <USED_FUNCTIONS>"
			"    <SP>"
			"      Add here the used function name and  references"
			"    </SP>"  
			"  </USED_FUNCTIONS>"     
			"</MAN>"
		];
		
	end
	
	if rhs >= 2 then
		mputl(txt,pathconvert(path,%t,%f)+funname+'.xml')
		txt=pathconvert(path,%t,%f)+funname+'.xml'
	end
	
endfunction
