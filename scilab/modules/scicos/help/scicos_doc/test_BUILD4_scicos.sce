//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//                      - Fady Nassif <fady.nassif@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

// test_BUILD4_scicos.sce
// This is the scilab script for the parametrization
// of scitexgendoc and for definition of
// scicos files to be documented

// alan,27/04/2007 : change html target directories path with the "trunk" look
//                   scicos_doc/help
//                   scicos_doc/help/images
//                   scicos_doc/help/en_US
//                   scicos_doc/help/fr_FR
//
// alan,28/04/2007 : add module flag "scicos" in %gendoc list
//                   add palpath flag in %gendoc list

// fady,04/05/2007 : add the integer palette.


//**--------------definition of some functions----------------**//
//gen_outline_block
function txt=gen_outline_all(list_of_scistruc,list_of_scifunc,listfABCD)
  txt = ["<WHATIS>";
         "  <TITLE eng=""Scicos Documentation"" fr=""Documentation Scicos""></TITLE>";
         "  <DATE>19 Septembre 2007</DATE>";
         "";]

  txt=[txt;
       "  <CHAPTER eng=""Scicos Documentation by categories : "" fr=""Documentation Scicos par cat�gories : "">"
       "  </CHAPTER>"]

  txt=[txt;
       "  <CHAPTER eng=""Batch / built_in functions"" fr=""Fonctions ligne de commande / utilitaires"">"]
  for i=1:size(list_of_scifunc,1)
     txt=[txt;
          "   <SCI varpath="""" name="""+basename(list_of_scifunc(i,2))+"""></SCI>"]
  end
  txt=[txt;
       "  </CHAPTER>"]

  txt=[txt;
       "  <CHAPTER eng=""Scilab Data Structures"" fr=""Structures de donn�e scilab"">"]
  for i=1:size(list_of_scistruc,1)
     txt=[txt;
          "   <SCI varpath="""" name="""+basename(list_of_scistruc(i,2))+"""></SCI>"]
  end
  txt=[txt;
       "  </CHAPTER>"]

  txt=[txt;
       "  <CHAPTER eng=""Blocks"" fr=""Blocs"">";]
  for i=1:size(listfABCD,1)
     txt=[txt;
          "   <BLK varpath="""" name="""+basename(listfABCD(i,2))+"""></BLK>"]
  end
  txt = [txt;
         "  </CHAPTER>"
         "</WHATIS>"];
endfunction

//gen_outline_block
function txt=gen_outline_block(listf)
  txt = ["<WHATIS>";
         "  <TITLE eng=""Scicos Documentation"" fr=""Documentation Scicos""></TITLE>";
         "  <DATE>19 Septembre 2007</DATE>";
         "";]
  for i=1:size(listf,1)
     txt=[txt;
          "   <BLK varpath="""" name="""+basename(listf(i,2))+"""></BLK>"]
  end
  txt = [txt;"</WHATIS>"];
endfunction

//gen_outline_pal
function txt=gen_outline_pal(listf)
  txt=[];
  txt='<PAL varpath=""palpath"" name="""+listf(find(listf(:,3)=='pal'),2)+""">";
  for i=2:size(listf,1)
     txt=[txt;
          "   <BLK varpath="""" name="""+basename(listf(i,2))+"""></BLK>"]
  end
  txt=[txt;
       "</PAL>"];
endfunction

//gen_outline_pal
function txt=gen_outline_pal2(listf)

  txt=[];
  txt='<PAL varpath=""palpath"" name="""+listf(find(listf(:,3)=='pal'),2)+""">";
  list_of_blocks = return_block_pal(listf(find(listf(:,3)=='pal'),1)+...
                                    listf(find(listf(:,3)=='pal'),2));
  [s,k]=gsort(convstr(list_of_blocks),'r','i');
  list_of_blocks=[list_of_blocks(k)];
  for i=1:size(list_of_blocks,1)
     txt=[txt;
          "   <BLK varpath="""" name="""+list_of_blocks(i)+"""></BLK>"]
  end
  txt=[txt;
       "</PAL>"];
endfunction

//gen_outline_dir
function txt=gen_outline_dir(listf)
  txt=[];
//  txt='<PAL varpath=""palpath"" name="""+listf(find(listf(:,3)=='pal'),2)+""">";
  txt="  <SUBCHAPTER eng=""subchapter"" fr=""souschapitre"">";
  for i=2:size(listf,1)
     txt=[txt;
          "   <BLK varpath="""" name="""+basename(listf(i,2))+"""></BLK>"]
  end
  txt=[txt;
       "</SUBCHAPTER>"];
endfunction

gen_outline_pal=gen_outline_pal2;

//gen_scs_outline
function tt=generate_scs_outline()
 tt = ["<WHATIS>";
              "  <TITLE eng=""Scicos Documentation"" fr=""Documentation Scicos""></TITLE>";
              "  <DATE>19 Septembre 2007</DATE>";
              "";
              "  <CHAPTER eng=""Batch functions"" fr=""Fonctions en ligne de commande"">"
              "    <SCI varpath=""autopath"" name=""scicos.sci""></SCI>"
              "    <SCI varpath=""autopath"" name=""scicosim.sci""></SCI>"
              "    <SCI varpath=""autopath"" name=""scicos_simulate.sci""></SCI>"
              "    <SCI varpath=""autopath"" name=""lincos.sci""></SCI>"
              "    <SCI varpath=""autopath"" name=""steadycos.sci""></SCI>"
              "  </CHAPTER>"
              "";
              "  <CHAPTER eng=""Editor"" fr=""Editeur"">";
              "    <SCI varpath="""" name=""Menu_entries""></SCI>"
              "    <SCI varpath="""" name=""Keyboard_shortcuts""></SCI>"
              "    <SCI varpath="""" name=""Context""></SCI>"
              "    <SCI varpath="""" name=""Multiwindow""></SCI>"
              "  </CHAPTER>"
              "";
              "  <CHAPTER eng=""Blocks"" fr=""Blocs"">";
              "   <SECTION eng=""Blocks list"" fr=""Liste des blocs"">";
              "     <SCI varpath="""" name=""ABCD_Blocks""></SCI>"
              "   </SECTION>";
              ""
              "   <SECTION eng=""Blocks by palette"" fr=""Blocs par palette"">";];

 tt = [tt;
              "  "+gen_outline_pal(listf_of_sources);
              ""
              "  "+gen_outline_pal(listf_of_linear);
              ""
              "  "+gen_outline_pal(listf_of_nonlinear);
              ""
              "  "+gen_outline_pal(listf_of_branching);
              ""
              "  "+gen_outline_pal(listf_of_misc);
              ""
              "  "+gen_outline_pal(listf_of_threshold);
              ""
              "  "+gen_outline_pal(listf_of_sinks);
              ""
              "  "+gen_outline_pal(listf_of_events);
              ""
              "  "+gen_outline_pal(listf_of_elec);
              ""
              "  "+gen_outline_pal(listf_of_thermo);
              ""
              "  "+ gen_outline_pal(listf_of_matop);
              ""
              "  "+ gen_outline_pal(listf_of_intop);
              ""
              "  "+ gen_outline_pal(listf_of_demos);]

  tt = [tt;
               "   </SECTION>";
               "  </CHAPTER>"
               ""
               "  <CHAPTER eng=""Programming Scicos blocks"" fr=""Programmation des blocs Scicos"">"
//               "   <SUBCHAPTER eng=""Basics"" fr=""El�ments de base"">";
//               "        <SCI varpath="""" name=""block_type""></SCI>"
//               "        <SCI varpath="""" name=""data_type""></SCI>"
//               "        <SCI varpath="""" name=""use_flag""></SCI>"
//               "   </SUBCHAPTER>";
               "   <SUBCHAPTER eng=""C computational functions (Type 4)"" fr=""Fonctions de calcul en C (Type 4)"">";
               "     <SCI varpath="""" name=""C_struct""></SCI>"
               "     <SCI varpath="""" name=""C_macros""></SCI>"
               "     <SCI varpath="""" name=""C_utils""></SCI>"
               "   </SUBCHAPTER>";
               "   <SUBCHAPTER eng=""Scilab computational functions (Type 5)"" fr=""Fonctions de calcul Scilab (Type 5)"">";
               "     <SCI varpath="""" name=""sci_struct""></SCI>"
               "     <SECTION eng=""Utilities functions"" fr=""Fonctions utilitaires"">";
               "        <SCI varpath=""autopath"" name=""curblock""></SCI>"
               "        <SCI varpath=""autopath"" name=""getblocklabel""></SCI>"
               "        <SCI varpath=""autopath"" name=""getscicosvars""></SCI>"
               "        <SCI varpath=""autopath"" name=""phase_simulation""></SCI>"
               "        <SCI varpath=""autopath"" name=""pointer_xproperty""></SCI>"
               "       <SCI varpath=""autopath"" name=""scicos_time""></SCI>"
               "       <SCI varpath=""autopath"" name=""set_xproperty""></SCI>"
               "       <SCI varpath=""autopath"" name=""set_blockerror""></SCI>"
               "     </SECTION>";
               "   </SUBCHAPTER>";
               "  </CHAPTER>"
               ""
               "  <CHAPTER eng=""Scilab Utilities functions"" fr=""Fonctions utilitaires Scilab"">"
               "    <SCI varpath=""autopath"" name=""buildouttb""></SCI>"
               "    <SCI varpath=""autopath"" name=""scicos_debug""></SCI>"
               "    <SCI varpath=""utilpath"" name=""create_palette""></SCI>"
               "    <SCI varpath=""utilpath"" name=""get_scicos_version""></SCI>"
               "    <SCI varpath=""autopath"" name=""var2vec""></SCI>"
               "    <SCI varpath=""autopath"" name=""vec2var""></SCI>"
               "  </CHAPTER>"
               ""
               "  <CHAPTER eng=""Scilab Data Structures"" fr=""Structures de donn�e scilab"">"
               "   <SECTION eng=""Diagram"" fr=""Diagramme"">";
               "    <SCI varpath=""opath2(1)"" name=""scicos_diagram""></SCI>"
               "    <SCI varpath=""opath2(1)"" name=""scicos_params""></SCI>"
               "   </SECTION>";
               "   <SECTION eng=""Blocks"" fr=""Blocs"">";
               "    <SCI varpath=""opath2(1)"" name=""scicos_block""></SCI>"
               "    <SCI varpath=""opath2(1)"" name=""scicos_model""></SCI>"
               "    <SCI varpath=""opath2(1)"" name=""scicos_graphics""></SCI>"
               "   </SECTION>";
               "   <SECTION eng=""Links"" fr=""Liens"">";
               "    <SCI varpath=""opath2(1)"" name=""scicos_link""></SCI>"
               "   </SECTION>";
               "   <SECTION eng=""Compilation/Simulation"" fr=""Compilation/Simulation"">";
               "    <SCI varpath=""opath2(1)"" name=""scicos_cpr""></SCI>"
               "    <SCI varpath=""opath2(1)"" name=""scicos_state""></SCI>"
               "    <SCI varpath=""opath2(1)"" name=""scicos_sim""></SCI>"
               "   </SECTION>";
               "  </CHAPTER>"
               ""
               "  <SCI varpath="""" name=""About_scicos""></SCI>"
               "</WHATIS>"];
endfunction

//gen_void_list_doc : generate void tex txt to do doc of a
//                    typed list
function [txt] = gen_void_list_doc(typdoc,lang,list_t)
  if lang=='fr' then
    tt_desc='entrez ici la description';
    tt_typ ='Type : ';
  else
    tt_desc='enter here the description';
    tt_typ ='Type : ';
  end

  fields_lst=getfield(1,list_t);
  txt=['\begin{itemize}'];
  for i=2:size(fields_lst,2)
     txt=[txt;
          '  '+'\item{\bf '+fields_lst(i)'+'}\\';
          '        '+tt_desc
          '        '+ tt_typ
          ''];
  end

  txt=[txt
       '\end{itemize}'];
endfunction

//gen_hyper_tex : do a hyperlink for scilst doc
function txt=gen_hyper_tex(txt)

   txt = strsubst(txt,'{scicos_diagram}',...
                       '\htmladdnormallink{scicos_diagram}{scicos_diagram.htm}')
   txt = strsubst(txt,'{scicos_scs_m}',...
                       '\htmladdnormallink{scs_m}{scicos_diagram.htm}')
   txt = strsubst(txt,'{scicos_params}',...
                       '\htmladdnormallink{params}{scicos_params.htm}')
   txt = strsubst(txt,'{scicos_block}',...
                       '\htmladdnormallink{block}{scicos_block.htm}')
   txt = strsubst(txt,'{scicos_model}',...
                       '\htmladdnormallink{model}{scicos_model.htm}')
   txt = strsubst(txt,'{scicos_graphics}',...
                       '\htmladdnormallink{graphics}{scicos_graphics.htm}')
   txt = strsubst(txt,'{scicos_link}',...
                       '\htmladdnormallink{link}{scicos_link.htm}')
   txt = strsubst(txt,'{scicos_cpr}',...
                       '\htmladdnormallink{cpr}{scicos_cpr.htm}')
   txt = strsubst(txt,'{scicos_state}',...
                    '\htmladdnormallink{state}{scicos_state.htm}')
   txt = strsubst(txt,'{scicos_sim}',...
                       '\htmladdnormallink{sim}{scicos_sim.htm}')
endfunction
//gen_tex_scilst : do a general pre treatment
//                 for scilst data
function txt_out=gen_pre_tex(txt_in,ind)
  item_tsz=[];
  txt_out=[];
  ind_itt=[]

  txt_in=gen_hyper_tex(txt_in);

  for i=1:size(txt_in,1)

    if length(txt_in(i))<>0 then
      pp = part(txt_in(i),length(txt_in(i)))
    else
      pp=[]
    end

    if strindex(txt_in(i),'  -- ')<>[] then
      if ind_itt==[] then
        txt_out($+1)='  \begin{itemize} '
      end
      ind_itt=size(txt_out,1)+1;
      if strindex(txt_in(i),' : ')<>[] then
        txt_in(i)=strsubst(txt_in(i),'  -- ','  \item{\bf ');
        txt_in(i)=strsubst(txt_in(i),' : ','}\\ ');
      else
        txt_in(i)=strsubst(txt_in(i),'  -- ','  \item ');
      end
    end

    if strindex(txt_in(i),'Size :')<>[] then
      if item_tsz<>[] then
          //if ind<>3 then
            txt_out(item_tsz)=txt_out(item_tsz)+' \null \\ '
          //end
          item_tsz = [];
      end
      //txt_in(i)=' \null \\ '+txt_in(i);
    end
    if strindex(txt_in(i),'Taille :')<>[] then
      if item_tsz<>[] then
//          if ind<>3 then
            txt_out(item_tsz)=txt_out(item_tsz)+' \null \\ '
//          end
          item_tsz = [];
      end
      //txt_in(i)=' \null \\ '+txt_in(i);
    end
    if strindex(txt_in(i),'Type :')<>[] then
      txt_in(i)=txt_in(i);
    end

    if pp=='.' then
      txt_out($+1)=txt_in(i)+'\\ '
      item_tsz=size(txt_out,1);
    else
      txt_out($+1)=txt_in(i)
    end
  end

  if ind_itt<>[] then
    for i=ind_itt:size(txt_out,1)
      if txt_out(i)==" " then
        txt_out=[txt_out(1:i-1);
                 '  \end{itemize}'
                 txt_out(i:$)]
        break
      end
    end
  end
endfunction

//gen_tex_scilst : generate body tex for a scicos
//                 scilst doc
function txt_tex=gen_tex_scilst(%scicos_help,ttype)

  execstr('ttl=%scicos_help.scilst.'+ttype);
  txt_tex=['\subsection{'+ttype+'}';
           gen_pre_tex(ttl(2),0)
           ''
           '  \begin{itemize}'];

  for i=3:size(ttl(1),2)
    //disp(ttl(1)(i))
    txt_tex=[txt_tex;
             '    \item{\bf '+ttl(1)(i)+'}\\'];
    txt_tex=[txt_tex;
             '         '+gen_pre_tex(ttl(i),i)
             ''];
  end

  txt_tex=[txt_tex;
           '  \end{itemize}'];
endfunction

//gen_scs_editor_help : main generator for scitexgendoc of tex files
//for the scicos scilst
function gen_scs_scilst_help(typdoc,%gd)

   for i=1:size(%gd.lang,1)
      //**-----set language-----**//
      LANGUAGE=%gd.lang(i)
      //**---load %scicos_help---**//
     [scicos_pal, %scicos_menu, %scicos_short, %scicos_help, ...
         %scicos_display_mode, modelica_libs,scicos_pal_libs, ...
          %scicos_lhb_list, %CmenuTypeOneVector ] = initial_scicos_tables();

      //**---loop on number of scilst---**//
      for j=1:size(list_of_scistruc,1)
        //**------ entries ------**//
        //** generate body of the tex file
        ttype=strsubst(basename(list_of_scistruc(j,2)),'scicos_','')
        tt=gen_tex_scilst(%scicos_help,ttype)
        //** generate tex head
        head_tex=get_head_tex(list_of_scistruc(j,:),typdoc,i,%gd)
        //** change title of html head
        if %gd.lang(i)=='fr' then
           head_tex=strsubst(head_tex,'Fonction Scilab','Liste Scicos')
        elseif %gd.lang(i)=='eng' then
           head_tex=strsubst(head_tex,'Scilab Function','Scicos list')
        end

        name=get_extname(list_of_scistruc(j,:),%gd)

        if fileinfo(%gd.lang(i)+'/'+name+'/'+name+'_scifunc.tex')<>[] then
           if %gd.lang(i)=='fr' then
             txt_scifunc = ['\subsection{Contenu du ficher}']
           else
             txt_scifunc = ['\subsection{File content}']
           end
           txt_scifunc = [txt_scifunc;
                          '\input{'+name+'_scifunc}']
        else
           txt_scifunc = []
        end

        if fileinfo(%gd.lang(i)+'/'+name+'/'+name+'_see_also.tex')<>[] then
           if %gd.lang(i)=='fr' then
             txt_salso = ['\subsection{Voir aussi}']
           else
             txt_salso = ['\subsection{See also}']
           end
           txt_salso = [txt_salso;
                          '\input{'+name+'_see_also}']
        else
           txt_salso = []
        end
        //** generate txt of tex file
        txt_scilst=[head_tex;
                   '\subsection{Module}'
                   '\begin{itemize}'
                   ' \item{\htmladdnormallink{Scicos}{whatis.htm}}'
                   '\end{itemize}'
                   latexsubst(tt)
                   txt_scifunc
                   txt_salso
                   '\htmlinfo*'
                   '\end{document}']
        //**---------------------**//

        //create lang directory
        if fileinfo(%gd.lang(i)+'/')==[] then
         mkdir(%gd.lang(i))
        end

        //create object directory for
        //tex compilation
        if fileinfo(%gd.lang(i)+'/'+...
                     name)==[] then
          mkdir(%gd.lang(i)+'/'+name)
        end

        mputl(txt_scilst,%gd.lang(i)+...
              '/'+name+...
              '/'+name+'.tex');

        //disp('ici');pause

      end
   end
endfunction

//gen_entries : generate tex file of entries of editor menu from
//initial_scicos_tables
function [txt_en] = gen_entries (typdoc,lang)
 [scicos_pal, %scicos_menu, %scicos_short, %scicos_help, ...
  %scicos_display_mode, modelica_libs,scicos_pal_libs, ...
  %scicos_lhb_list, %CmenuTypeOneVector ] = initial_scicos_tables();

 txt_en=[]
 txt_fr=[]
 %scicos_help_fr=%scicos_help;
 %scicos_menu_fr=%scicos_menu;

 //** loop on number of menu
 for i=1:lstsize(%scicos_menu)
   //** get the menu title
   menu_title=%scicos_menu(i)(1)

   //** get entries
   menu_lst=%scicos_menu(i)(2:$)

   //** Add title in txt of help
   txt_en=[txt_en;
           '\subsection{'+latexsubst(menu_title)+' menu}'
           '\begin{itemize}']

   //** loop on number of entries
   for j=1:size(menu_lst,2)
     //** get entry
     entry_title = menu_lst(j);

     //** Add title:entry in txt of help
     txt_en=[txt_en;
             '\item {\bf '+latexsubst(menu_title)+':'+entry_title]
             '      '

     //** Check if there is a shorcut
     sc_ind=find(%scicos_short(:,2)==entry_title)
     if sc_ind<>[] then
       txt_en($) = txt_en($) + ' ('+%scicos_short(sc_ind,1)+')'
     end

     //**retrieve help in %scicos_help.menu
     if execstr('help_txt = %scicos_help.menu(entry_title)','errcatch')==0 then
       txt_en($) = txt_en($) + '}\\'
       txt_en=[txt_en;
               '      '+latexsubst(help_txt);
               '']
     else
      txt_en($) = txt_en($) + '}'
       txt_en=[txt_en;
               '';
               '']
     end
   end

   txt_en = [txt_en;
             '\end{itemize}']
 end
endfunction

//gen_shortc : generate tex file of keyborad shorcuts of the editor
//from initial_scicos_tables
function [txt_en] = gen_shortc (typdoc,lang)
 [scicos_pal, %scicos_menu, %scicos_short, %scicos_help, ...
  %scicos_display_mode, modelica_libs,scicos_pal_libs, ...
  %scicos_lhb_list, %CmenuTypeOneVector ] = initial_scicos_tables();

 txt_en = [];

 txt_en = [txt_en;
           '\begin{itemize}']
 //** loop on number of menu
 for i=1:size(%scicos_short,1)

   txt_en = [txt_en;
             '  \item{\bf '''+%scicos_short(i,1)+''' :} ' + %scicos_short(i,2)]

 end

 txt_en = [txt_en;
           '\end{itemize}']
endfunction

//gen_scs_editor_help : main generator for scitexgendoc of tex files
//for the scicos editor
function gen_scs_editor_help(typdoc,%gd)

   for i=1:size(%gd.lang,1)
      //**------ entries ------**//
      //** generate body of the tex file
      tt=gen_entries(typdoc,%gd.lang(i))
      //** generate tex head
      head_tex=get_head_tex(["","Menu_entries","sci"],typdoc,i,%gd)
      //** change title of html head
      if %gd.lang(i)=='fr' then
         head_tex=strsubst(head_tex,'Fonction Scilab','Editeur Scicos')
      elseif %gd.lang(i)=='eng' then
         head_tex=strsubst(head_tex,'Scilab Function','Scicos Editor')
      end

      //** generate txt of tex file
      txt_entries=[head_tex;
                   tt
                   '\htmlinfo*'
                   '\end{document}']
      //**---------------------**//


      //**------ shortcuts ------**//
      //** generate body of the tex file
      tt=gen_shortc(typdoc,%gd.lang(i))
      //** generate tex head
      head_tex=get_head_tex(["","Keyboard_shortcuts","sci"],typdoc,i,%gd)
      //** change title of html head
      if %gd.lang(i)=='fr' then
         head_tex=strsubst(head_tex,'Fonction Scilab','Editeur Scicos')
      elseif %gd.lang(i)=='eng' then
         head_tex=strsubst(head_tex,'Scilab Function','Scicos Editor')
      end

      //** add permanent shorcuts
      if %gd.lang(i)=='fr' then
         tt_cust = 'Raccourcis clavier modifiables par l''utilisateur :'
         tt_perm = ['Raccourcis clavier permanents :'
                    ''
                    '\begin{itemize}'
                    '   \item {\bf ''Ctrl + c'' :} Copie les objets s�lectionn�s dans le clipboard scicos'
                    '   \item {\bf ''Ctrl + v'' :} Colle les objets du clipboard scicos'
                    '   \item {\bf ''Ctrl + a'' :} S�lectionne tous les objets de la fen�tre courante'
                    '   \item {\bf ''Ctrl + s'' :} Sauvegarde le diagramme courant'
                    '   \item {\bf ''Ctrl + clic gauche'' :} Ajoute des objets � la selection courante'
                    '\end{itemize}']
      elseif %gd.lang(i)=='eng' then
         tt_cust = 'Customizable keyboard shortcuts :'
         tt_perm = ['Permanent keyboard shortcuts :'
                    ''
                    '\begin{itemize}'
                    '   \item {\bf ''Ctrl + c'' :} Copy the selected items in the scicos clipboard'
                    '   \item {\bf ''Ctrl + v'' :} Paste items from the scicos clipboard'
                    '   \item {\bf ''Ctrl + a'' :} Select all items of the current window'
                    '   \item {\bf ''Ctrl + s'' :} Save the current diagram'
                    '   \item {\bf ''Ctrl + left click'' :} Add items to the current selection'
                    '\end{itemize}']
      end

      //** generate txt of tex file
      txt_shortcuts=[head_tex;
                     ''
                     tt_cust
                     ''
                     tt
                     ''
                     tt_perm
                     '\htmlinfo*'
                     '\end{document}']
      //**---------------------**//

      //**------ Context ------**//
      //** generate body of the tex file

      //** generate tex head
      head_tex=get_head_tex(["","Context","sci"],typdoc,i,%gd)
      //** change title of html head
      if %gd.lang(i)=='fr' then
         head_tex=strsubst(head_tex,'Fonction Scilab','Editeur Scicos')
      elseif %gd.lang(i)=='eng' then
         head_tex=strsubst(head_tex,'Scilab Function','Scicos Editor')
      end

      name=get_extname(["","Context","sci"],%gd) 

      if fileinfo(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_mod.tex')<>[] then

        tt=['\subsection{Modules}'
            mgetl(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_mod.tex')];
      else
        tt=[];
      end

      if fileinfo(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_long.tex')<>[] then
        tt=[tt;
            mgetl(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_long.tex')];
      end

      if fileinfo(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_authors.tex')<>[] then
        if %gd.lang(i)=='fr' then
          tt_sub = '\subsection{Auteurs}'
        else
          tt_sub = '\subsection{Authors}'
        end
        tt=[tt;
            tt_sub;
            mgetl(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_authors.tex')];
      end

      //** generate txt of tex file
      txt_context=[head_tex;
                   tt
                   '\htmlinfo*'
                   '\end{document}']
      //**---------------------**//

      //**------ Multiwindow ------**//
      //** generate body of the tex file

      //** generate tex head
      head_tex=get_head_tex(["","Multiwindow","sci"],typdoc,i,%gd)
      //** change title of html head
      if %gd.lang(i)=='fr' then
         head_tex=strsubst(head_tex,'Fonction Scilab','Editeur Scicos')
      elseif %gd.lang(i)=='eng' then
         head_tex=strsubst(head_tex,'Scilab Function','Scicos Editor')
      end

      name=get_extname(["","Multiwindow","sci"],%gd) 

      if fileinfo(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_mod.tex')<>[] then

        tt=['\subsection{Module}'
            mgetl(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_mod.tex')];
      else
        tt=[];
      end

      if fileinfo(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_long.tex')<>[] then
        tt=[tt;
            mgetl(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_long.tex')];
      end

      if fileinfo(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_authors.tex')<>[] then
        if %gd.lang(i)=='fr' then
          tt_sub = '\subsection{Auteurs}'
        else
          tt_sub = '\subsection{Authors}'
        end
        tt=[tt;
            tt_sub;
            mgetl(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_authors.tex')];
      end

      //** generate txt of tex file
      txt_multiwin=[head_tex;
                   tt
                   '\htmlinfo*'
                   '\end{document}']
      //**---------------------**//

      //create lang directory
      if fileinfo(%gd.lang(i)+'/')==[] then
       mkdir(%gd.lang(i))
      end

      name=get_extname(["","Menu_entries","sci"],%gd)

      //create object directory for
      //tex compilation
      if fileinfo(%gd.lang(i)+'/'+...
                   name)==[] then
        mkdir(%gd.lang(i)+'/'+name)
      end

      mputl(txt_entries,%gd.lang(i)+...
            '/'+name+...
            '/'+name+'.tex');

      name=get_extname(["","Keyboard_shortcuts","sci"],%gd)

      //create object directory for
      //tex compilation
      if fileinfo(%gd.lang(i)+'/'+...
                   name)==[] then
        mkdir(%gd.lang(i)+'/'+name)
      end

      mputl(txt_shortcuts,%gd.lang(i)+...
            '/'+name+...
            '/'+name+'.tex');

      name=get_extname(["","Context","sci"],%gd)

      //create object directory for
      //tex compilation
      if fileinfo(%gd.lang(i)+'/'+...
                   name)==[] then
        mkdir(%gd.lang(i)+'/'+name)
      end

      mputl(txt_context,%gd.lang(i)+...
            '/'+name+...
            '/'+name+'.tex');

      name=get_extname(["","Multiwindow","sci"],%gd)

      //create object directory for
      //tex compilation
      if fileinfo(%gd.lang(i)+'/'+...
                   name)==[] then
        mkdir(%gd.lang(i)+'/'+name)
      end

      mputl(txt_multiwin,%gd.lang(i)+...
            '/'+name+...
            '/'+name+'.tex');
   end
endfunction

//gen_scs_about_help : main generator for scitexgendoc of tex files
//for the scicos about
function gen_scs_about_help(typdoc,%gd)

   for i=1:size(%gd.lang,1)

      tt=[];

      //** generate tex head
      head_tex=get_head_tex(["","About_scicos","sci"],typdoc,i,%gd)
      //** change title of html head
      if %gd.lang(i)=='fr' then
         head_tex=strsubst(head_tex,'Fonction Scilab','Au sujet de Scicos')
      elseif %gd.lang(i)=='eng' then
         head_tex=strsubst(head_tex,'Scilab Function','About Scicos')
      end

      name=get_extname(["","About_scicos","sci"],%gd)

      if fileinfo(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_mod.tex')<>[] then

        tt=['\subsection{Module}'
            mgetl(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_mod.tex')];
      else
        tt=[];
      end

      if fileinfo(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_long.tex')<>[] then
        tt=[tt;
            mgetl(%gd.lang(i)+...
                  '/'+name+...
                  '/'+name+'_long.tex')];
      end

      //** generate txt of tex file
      txt_about=[head_tex;
                   tt
                   '\htmlinfo*'
                   '\end{document}']
      //**---------------------**//

      //create lang directory
      if fileinfo(%gd.lang(i)+'/')==[] then
       mkdir(%gd.lang(i))
      end
      //create object directory for
      //tex compilation
      if fileinfo(%gd.lang(i)+'/'+...
                   name)==[] then
        mkdir(%gd.lang(i)+'/'+name)
      end

      mputl(txt_about,%gd.lang(i)+...
            '/'+name+...
            '/'+name+'.tex');

   end
endfunction

//gen_scs_prgblk : main generator for scitexgendoc of tex files
//for the list_of_prgblk
function gen_scs_prgblk(typdoc,%gd)

   for i=1:size(%gd.lang,1)

      tt=[];

      for j=1:size(list_of_prgblk,1)

         //** generate tex head
         head_tex=get_head_tex(list_of_prgblk(j,:),typdoc,i,%gd)

         //**special patch for scicos_block4.h
         //**---------------------**//
         if list_of_prgblk(j,2)=='scicos_block4.h' then
           //** change title of html head
           if %gd.lang(i)=='fr' then
              head_tex=strsubst(head_tex,'Routine bas-niveau','En-t�te bloc scicos C')
              tt=['\subsection{Contenu du fichier}'
                  '{\tiny'
                  '\verbatiminput{'+list_of_prgblk(j,1)+'scicos_block4.h}'
                  '}']
           elseif %gd.lang(i)=='eng' then
              head_tex=strsubst(head_tex,'Low level routine','C scicos block header')
              tt=['\subsection{File content}'
                  '{\tiny'
                  '\verbatiminput{'+list_of_prgblk(j,1)+'scicos_block4.h}'
                  '}']
           end
           //** generate txt of tex file
           txt=[head_tex;
                tt
               '\htmlinfo*'
               '\end{document}']
           name=get_extname(list_of_prgblk(j,:),%gd)
           mputl(txt,%gd.lang(i)+...
                 '/'+name+...
                 '/'+name+'.tex');
         else
         //**---------------------**//

           //** change title of html head
           if %gd.lang(i)=='fr' then
              head_tex=strsubst(head_tex,'Fonction Scilab','Programmation des blocs scicos')
           elseif %gd.lang(i)=='eng' then
              head_tex=strsubst(head_tex,'Scilab Function','Programming scicos blocks')
           end

           name=get_extname(list_of_prgblk(j,:),%gd)

           if fileinfo(%gd.lang(i)+...
                       '/'+name+...
                       '/'+name+'_mod.tex')<>[] then

             tt=['\subsection{Module}'
                 mgetl(%gd.lang(i)+...
                       '/'+name+...
                       '/'+name+'_mod.tex')];
           else
             tt=[];
           end

           if fileinfo(%gd.lang(i)+...
                       '/'+name+...
                       '/'+name+'_long.tex')<>[] then
             tt=[tt;
                 '\subsection{Description}'
                 mgetl(%gd.lang(i)+...
                       '/'+name+...
                       '/'+name+'_long.tex')];
           end

           if fileinfo(%gd.lang(i)+...
                       '/'+name+...
                       '/'+name+'_authors.tex')<>[] then
             if %gd.lang(i)=='fr' then
               tt_sub = '\subsection{Auteurs}'
             else
               tt_sub = '\subsection{Authors}'
             end
             tt=[tt;
                tt_sub;
                mgetl(%gd.lang(i)+...
                      '/'+name+...
                      '/'+name+'_authors.tex')];
           end

           //** generate txt of tex file
           txt=[head_tex;
                '\tableofcontents'
                tt
                '\htmlinfo*'
                '\end{document}']
           //**---------------------**//

           //create lang directory
           if fileinfo(%gd.lang(i)+'/')==[] then
             mkdir(%gd.lang(i))
           end
           //create object directory for
           //tex compilation
           if fileinfo(%gd.lang(i)+'/'+...
                        name)==[] then
             mkdir(%gd.lang(i)+'/'+name)
           end

           mputl(txt,%gd.lang(i)+...
                 '/'+name+...
                 '/'+name+'.tex');

         end
      end
   end
endfunction

//gen_scicos_whatis : generate the whatis fileS
function gen_scicos_whatis(%gd)
  gen_whatis(%gd.mpath.data(1)+'/ABCD_Blocks.xml',%gd);
//   for i=1:size(%gd.mpath.html,1)
//     unix_g(%gd.cmd.mv+%gd.mpath.html(i)+...
//             'whatis.htm '+%gd.mpath.html(i)+'ABCD_Blocks.htm');
//   end
  gen_whatis(%gd.mpath.data(1)+'/whatis_scicos.xml',%gd);
//   for i=1:size(%gd.mpath.html,1)
//     unix_g(%gd.cmd.mv+%gd.mpath.html(i)+...
//             'whatis.htm '+%gd.mpath.html(i)+'whatis_scicos.htm');
//   end
  gen_whatis(%gd.mpath.data(1)+'/whatis.xml',%gd);
  for i=1:size(%gd.mpath.html,1)
    tt=mgetl(%gd.mpath.html(i)+'whatis.htm');
    tt=strsubst(tt,...
                '<b><LI>Scicos Documentation by categories : </b>',...
                '<b><LI>Scicos Documentation by categories : </b> <A HREF=""whatis_scicos.htm"">whatis_scicos</A> ');

    tt=strsubst(tt,...
                '<b><LI>Documentation Scicos par cat�gories : </b>',...
                '<b><LI>Documentation Scicos par cat�gories : </b> <A HREF=""whatis_scicos.htm"">whatis_scicos</A> ');

    mputl(tt,%gd.mpath.html(i)+'whatis.htm');
  end
endfunction

//gen_scicos_doc : generate all the scicos doc
function gen_scicos_doc(my_list,typdoc,%gd)
  generate_aux_tex_file(my_list,typdoc,%gd);
  import_data_to_file('all',%gd);
  generate_aux_tex_file(my_list,typdoc,%gd);
  gen_scs_editor_help(typdoc,%gd);
  gen_scs_prgblk(typdoc,%gd);
  gen_scs_scilst_help(typdoc,%gd);
  gen_scs_about_help(typdoc,%gd);
  generate_html_file(my_list,%gd);
  gen_scicos_whatis(%gd)
endfunction

//**----------------------------------------------------------**//

//**-------------------path definition-----------------------**//
//define directory of scicos_doc
doc_path=get_absolute_file_path('test_BUILD4_scicos.sce');

//define path of interfacing function of scicos block
opath2=SCI+"/macros/"+...
       ["scicos"                   //1
        "scicos_blocks/Branching"  //2
        "scicos_blocks/Electrical" //3
        "scicos_blocks/Events"     //4
        "scicos_blocks/Hydraulics" //5
        "scicos_blocks/Linear"     //6
        "scicos_blocks/Misc"       //7
        "scicos_blocks/NonLinear"  //8
        "scicos_blocks/PDE"        //9
        "scicos_blocks/Sinks"      //10
        "scicos_blocks/Sources"    //11
        "scicos_blocks/Threshold"  //12
        "scicos_blocks/MatrixOp"   //13
        "scicos_blocks/IntegerOp"   //14
       ]+"/";

//define path of cosf file of scicos palettes
palpath=SCI+'/macros/scicos/';

//define auto path
autopath=SCI+'/macros/auto/';

//define util path
utilpath=SCI+'/macros/util/';
//**----------------------------------------------------------**//


//**----------scitexgendoc global parameters------------------**//
//define %gendoc list (for scitexgendoc)
%gendoc=gendoc_def(lang=['fr' 'eng'],..
                   man_path=doc_path+'man/',..
                   rout_path=[SCI+'/routines/scicos/';
                              SCI+'/macros/scicos/'],..
                   mod_flag='Scicos',..
                   path_flag='SCI',..
                   pal_path=palpath,..
                   block_flag='scs',..
                   web_path='',..
                   pdf_path='',..
                   clean_html=%t,..
                   ext_block='',..
                   ext_mblock='',..
                   ext_sci='',..
                   with_gimp=%f,..
                   verbose=%t,..
                   with_log=%t,..
                   html_subtitle_color='blue',..
                   whatis_name='whatis_scicos.htm',...
                   name_log='BUILD_4_scicos_doc.log');

//set behavior of documentation target directories
//(0: scilab 4.1x 1:scilab 5)
%gendoc=set_gendoc_def(%gendoc,1);

//set source directories of data files
%gendoc.mpath.data=%gendoc.mpath.man+...
                   ['fr/data_revB/';
                    'eng/data_revB/'];

//set target directories of html files
// %gendoc.mpath.html=doc_path+'/help/'+...
//                    ['fr_FR/';
//                     'en_US/'];
%gendoc.mpath.html=SCI+'/man/'+...
                   ['fr/scicos/';
                    'eng/scicos/'];
%gendoc.mpath.html_img='../../images/scicos/';

//create target directories
create_gendoc_dirs(%gendoc);
//**----------------------------------------------------------**//

//define a variable for protected local xml/tex files
if ~exists('%already_import') then
  %already_import = %f;
end

//**-------------------Files declaration----------------------**//
//define list of files to be documented
// my_list(1,) : the path of the file
// my_list(2,) : the name of the file with extension
// my_list(3,) : the type of the object
//               current scitexgendoc version only support
//                 'mblock' : scicos modelica block
//                 'block'  : scicos block
//                 'diagr'  : scicos diagram
//                 'pal'    : scicos palette
//                 'scilib' : library of scilab macro
//                 'sci'    : scilab macro
//                 'sim'    : scilab script of scicos diagram simulation
//                 'sce'    : scilab script
//                 'rout'   : low level routine

//TO BE DONE
my_listf=[// "","EXPRESSION.sci","block"; //CEST ENDOUBLE
          ];

//util macros
listf_of_utilsci=[utilpath,"create_palette.sci","sci";
                  utilpath,"get_scicos_version.sci","sci"];

//auto macros
listf_of_autosci=[autopath,"scicos.sci","sci";
                  autopath,"scicos_simulate.sci","sci";
                  autopath,"lincos.sci","sci";
                  autopath,"steadycos.sci","sci"];

//Interfaced function
listf_of_interf=["","buildouttb","sci";
                 "","curblock","sci";
                 "","getblocklabel","sci";
                 "","phase_simulation","sci";
                 "","pointer_xproperty","sci";
                 "","scicos_debug","sci";
                 "","scicos_time","sci"
                 "","set_xproperty","sci";
                 "","scicosim.sci","sci";
                 "","getscicosvars","sci";
                 "","var2vec","sci";
                 "","vec2var","sci";
                 "","set_blockerror","sci";];

//**--palettes--*/

//Sources palette
listf_of_sources=[palpath,"Sources.cosf","pal"];
listf = basename(listfiles(opath2(11)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_sources=[listf_of_sources;
                    opath2(11),listf(i),"block"];
end
clear i;clear listf;

//linear palette
listf_of_linear=[palpath,"Linear.cosf","pal"];
listf = basename(listfiles(opath2(6)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  if listf(i)<>'SOM_f.sci' then //!!!!!!! YAUNB�GICI
    listf_of_linear=[listf_of_linear;
                     opath2(6),listf(i),"block"];
  end
end
clear i;clear listf;

//nonlinear palette
listf_of_nonlinear=[palpath,"Non_linear.cosf","pal"];
listf = basename(listfiles(opath2(8)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_nonlinear=[listf_of_nonlinear;
                      opath2(8),listf(i),"block"];
end
clear i;clear listf;

//branching palette
listf_of_branching=[palpath,"Branching.cosf","pal"];
listf = basename(listfiles(opath2(2)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_branching=[listf_of_branching;
                      opath2(2),listf(i),"block"];
end
clear i;clear listf;

//Threshold palette
listf_of_threshold=[palpath,"Threshold.cosf","pal"];
listf = basename(listfiles(opath2(12)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_threshold=[listf_of_threshold;
                      opath2(12),listf(i),"block"];
end
clear i;clear listf;

//Oldblocks palette
listf_of_oldblocks=[palpath,"OldBlocks.cosf","pal"];

//Sinks palette
listf_of_sinks=[palpath,"Sinks.cosf","pal"];
listf = basename(listfiles(opath2(10)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_sinks=[listf_of_sinks;
                  opath2(10),listf(i),"block"];
end
clear i;clear listf;

//Events palette
listf_of_events=[palpath,"Events.cosf","pal"];
listf = basename(listfiles(opath2(4)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_events=[listf_of_events;
                   opath2(4),listf(i),"block"];
end
clear i;clear listf;

//Modelica blocks palettes
listf_of_elec=[palpath,"Electrical.cosf","pal"];
listf = basename(listfiles(opath2(3)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  listf_of_elec=[listf_of_elec;
                 opath2(3),listf(i),"mblock"];
end
clear i;clear listf;
listf_of_thermo=[palpath,"ThermoHydraulics.cosf","pal"];
listf = ['Bache.sci';'PerteDP.sci';'PuitsP.sci';'SourceP.sci';'VanneReglante.sci'];
for i = 1:size(listf,1)
  listf_of_thermo=[listf_of_thermo;
                   opath2(5),listf(i),"mblock"];
end
clear i;clear listf;

//MatrixOp palette
listf_of_matop=[palpath,"Matrix.cosf","pal"];
listf = basename(listfiles(opath2(13)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  if listf(i)<>'INTGAINBLK.sci' then //!!!!!!! YAUNB�GICI
    listf_of_matop=[listf_of_matop;
                   opath2(13),listf(i),"block"];
  end
end
clear i;clear listf;

//IntegerOp palette
listf_of_intop=[palpath,"Integer.cosf","pal"];
listf = basename(listfiles(opath2(14)+"*.sci"))+".sci";
for i = 1:size(listf,1)
    listf_of_intop=[listf_of_intop;
                   opath2(14),listf(i),"block"];
end
clear i;clear listf;

//Misc directory
listf_of_misc=[palpath,"Others.cosf","pal"];
listf = basename(listfiles(opath2(7)+"*.sci"))+".sci";
for i = 1:size(listf,1)
  if listf(i)<>'m_sin.sci' & listf(i)<>'DSUPER.sci' & ...
        listf(i)<>'bplatform2.sci' then //!!!!!!! YAUNB�GICI
    listf_of_misc=[listf_of_misc;
                   opath2(7),listf(i),"block"];
  end
end
listf_of_misc=[listf_of_misc;
               opath2(9),"PDE.sci","block"]; //??
clear i;clear listf;

listf_of_demos=[palpath,"DemoBlocks.cosf","pal"];

listf_of_ABCD=["","ABCD_Blocks","sci"];
//**------------*/

//**--Editor--*/
list_of_editor = ["","Menu_entries","sci";
                  "","Keyboard_shortcuts","sci";
                  "","Context","sci";
                  "","Multiwindow","sci"];
//**------------*/

//**--scilab structure--*/
list_of_scistruc = [opath2(1),"scicos_diagram.sci","sci";
                    opath2(1),"scicos_params.sci","sci";
                    opath2(1),"scicos_link.sci","sci";
                    opath2(1),"scicos_model.sci","sci";
                    opath2(1),"scicos_graphics.sci","sci";
                    opath2(1),"scicos_state.sci","sci";
                    opath2(1),"scicos_block.sci","sci";
                    opath2(1),"scicos_cpr.sci","sci";
                    opath2(1),"scicos_sim.sci","sci";];
//**------------*/

//**--program blocs --*/
list_of_prgblk   = ["","C_struct","sci";
                    "","C_macros","sci";
                    "","C_utils","sci";
                    SCI+'/routines/scicos/',"scicos_block4.h","rout";
                    "","sci_struct","sci";
                    "","data_type","sci";
                    "","block_type","sci";
                    "","use_flag","sci";];
//**------------*/

//**-- About_scicos --*/
list_of_about   = ["","About_scicos","sci"];


//**-- Demos --*/
lists_of_diagr_demos=[SCI+'/demos/scicos/','Lorentz.cosf',"diagr"];
//**------------*/

//**----------------------------------------------------------**//


//**------------------outline.xml generation------------------**//
outline_txt=generate_scs_outline();
mputl(outline_txt,%gendoc.mpath.data(1)+'/whatis_scicos.xml');
//**----------------------------------------------------------**//


//load scicos libraries and palettes of blocks
load SCI/macros/scicos/lib;
exec(loadpallibs,-1);

//Examples of selection :
// 1 - my_list=my_listf(122:138,:); //documentation of files from 122 to 138
// 2 - my_list=my_listf(find(my_listf(:,2)=='scicos.sci'),:) //doc of the file scicos.sci
// 3 - my_list=listf_of_events; //doc of the event palette

//generate doc for all scicos files define above
my_list = [my_listf;
           listf_of_utilsci;
           listf_of_autosci;
           listf_of_interf;
           listf_of_sources;
           listf_of_linear;
           listf_of_nonlinear;
           listf_of_branching;
           listf_of_threshold;
           listf_of_oldblocks;
           listf_of_sinks;
           listf_of_events;
           listf_of_elec;
           listf_of_thermo;
           listf_of_matop;
           listf_of_intop;
           listf_of_misc;
           listf_of_demos;
           list_of_editor;
           listf_of_ABCD;
           list_of_scistruc
           list_of_prgblk
           list_of_about
           lists_of_diagr_demos];

//**------------------ABCDblocks.xml generation------------------**//
list_of_ABCDblocks=[my_list(find(my_list(:,3)=='block'),:);
                    my_list(find(my_list(:,3)=='mblock'),:)];
[s,k]=gsort(convstr(list_of_ABCDblocks(:,2)),'r','i');
list_of_ABCDblocks=list_of_ABCDblocks(k,:);
outline_txt=gen_outline_block(list_of_ABCDblocks);
mputl(outline_txt,%gendoc.mpath.data(1)+'/ABCD_Blocks.xml');
//**----------------------------------------------------------**//

//**------------------whatis.xml generation------------------**//
list_of_ABCDblocks=[my_list(find(my_list(:,3)=='block'),:);
                    my_list(find(my_list(:,3)=='mblock'),:)];
[s,k]=gsort(convstr(list_of_ABCDblocks(:,2)),'r','i');
list_of_ABCDblocks=list_of_ABCDblocks(k,:);

list_of_scifunc=[listf_of_autosci;listf_of_interf];
[s,k]=gsort(convstr(list_of_scifunc(:,2)),'r','i');
list_of_scifunc = list_of_scifunc(k,:);

[s,k]=gsort(convstr(list_of_scistruc(:,2)),'r','i');
list_of_scistruc = list_of_scistruc(k,:);

outline_txt=gen_outline_all(list_of_scistruc,list_of_scifunc,list_of_ABCDblocks);
mputl(outline_txt,%gendoc.mpath.data(1)+'/whatis.xml');
//**----------------------------------------------------------**//

//STEP_1 : Get the current set of xml/tex files of B4_scicos doc.

//create skeleton of xml files
generate_xml_file(my_list,%gendoc);

//import all data in xml/tex files
//from data files
if %already_import then
  irr=message(["                       WARNING";
               "";
               "You have already import data in xml/tex files by";
               "running this script.";
               "By importing data now, you can earase locally";
               "the modifications that probably you have just made !";
               ""
               "        Do you really know what you are doing ?"],["yes","no"]);
  if irr==1 then, import_data_to_file('all',%gendoc), end;
  clear irr;
else
  import_data_to_file('all',%gendoc);
  %already_import = %t;
end

//STEP_2 : Inform xml/tex files with your informations.
//         You can begin the work of documentation with :
//          choice 1 : in generated xml files
//                     directories are
//                        scicos_doc/eng/xml
//                        scicos_doc/fr/xml
//
//          choice 2 : directly in LaTeX (for expert)
//                     directories are
//                        scicos_doc/eng/tex
//                        scicos_doc/fr/tex
//
//   PS : choice 1 & 2 can be mixed !

//STEP_3 : Generate auxiliaries tex files compilable with LaTeX.
//         This is done from xml/tex files provided by STEP 1 and 2.
//
//   PS : - All work of generation of this tex files is done
//          in the current directory.
//           For each documented object a directory is created.
//
//        - If your xml/tex files of STEP_2 are well formed
//          you can run latex at the root of each generated directories
//          without any errors.
//
//-->generate_aux_tex_file(my_list,'html',%gendoc);


//STEP_3a : do a second pass for import_data and generation of
//          auxiliaries tex files (not necessary)
//          That improves interfacing functions and
//          computational functions man pages
//
//-->import_data_to_file('all',%gendoc);generate_aux_tex_file(my_list,'html',%gendoc);

//STEP_3b : Generate tex files for the scicos editor
//
//-->gen_scs_editor_help('html',%gendoc);

//STEP_4 : Convert auxiliaries tex file in html.
//
//-->generate_html_file(my_list,%gendoc);

//STEP_5 : Look your generated html files in target html directories.
//         GOTO STEP_2 if you are not happy, else continue.
//
//  PS : NEVER re-run import_data_to_file at this step because
//       you will earase your modifications that you have just
//       made in xml/tex files of STEP_2.

//STEP_6 : generate whatis.htm (for all languages)
//
//-->gen_whatis(%gendoc.mpath.data(1)+'/outline.xml',%gendoc);

//STEP_7 : Update data files with your modifications.
//
//         Before doing that, save locally all your xml/tex files that you have
//         modified/created (don't save generated TeX files of STEP_3,
//         only files of STEP_2).
//
//         Then two choices can be done :
//
//         a - Ask to Alan to explain to you how commit your modification
//             in data files OR send him by email your tex/xml files that
//             you have locally saved (in a single tar.gz file please).
//
//         b - Demerden sie sich !
//             i    - Use :
//                   --> export_file_to_data('all',%gendoc);
//                   This cmd will update all the data files from you local xml/tex files
//                   modified/updated in STEP_2.
//                   (warning should properly work only with subversion of scitexgendoc >= 97)
//             ii   - erase all you generated files
//                   $ rm -fr scicos_doc/eng/xml
//                   $ rm -fr scicos_doc/eng/tex
//                   $ rm -fr scicos_doc/fr/xml
//                   $ rm -fr scicos_doc/fr/tex
//                   $ rm -fr scicos_doc/help
//             iii  - re-generate all html doc of scicos (STEP_1:STEP_5)
//             iiii - look if you dont't have forget any data in html doc freshly regenerated.
//                    If something is wrong please check what's your errors by comparing data files
//                    of STEP_7-b-i with current data files of the svn rep of B4_scicos.

//STEP_8 : Commit data files of scicos_doc/eng/data - scicos_doc/fr/data
//         in the svn rep of B4_scicos
