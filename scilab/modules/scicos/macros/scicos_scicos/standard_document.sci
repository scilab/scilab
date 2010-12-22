//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
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

function texte = standard_document(objet, k)
//**
// standard_document - Scicos block documentation
//

  type_objet = typeof(objet)

  select type_objet
    //## texte ##//
    case "Text" then
      texte = 'This is a TEXT block'

    //## Blocks ##//
    case "Block" then
      modele    = objet.model
      graphique = objet.graphics
      macro     = objet.gui

      //## function type
      fonction  = modele.sim
      if prod(size(fonction)) > 1 then
        if fonction(2) == 0 then
          language = '0 (Scilab function type Scicos 2.2)'
        elseif fonction(2) == 1  then
          language = '1 (Fortran or C code)'
        elseif fonction(2) == 2 then
          language = '2 (C code)'
        elseif fonction(2) == 3 then
          language = '3 (Scilab function)'
        elseif fonction(2) == 4 then
          language = '4 (C code)'
        elseif fonction(2) == 5 then
          language = '5 (Scilab function)'
        elseif fonction(2) == 99 then
          language = '99 (Scicos debug block)'
         elseif fonction(2) < 0  then
          language = '<0 (synchro block)'
        elseif fonction(2) <10000 then
          language = string(fonction(2))+' (dynamic link or...)'
        else
          language = string(fonction(2))+' (internally implicit bloc)'
        end
      else
       if modele.equations==list() then
         language = '0 (Scilab function type Scicos 2.2)'
       else
         language = 'Implicit Modelica block'
       end
      end

      //## block type
      if modele.blocktype == 'l' then
        typ= 'synchro'
      elseif modele.blocktype == 'x'
        typ= 'x-type'
      elseif modele.blocktype == 'm'
        typ = 'memory'
      else
        typ = 'regular'
      end

      //## dep_u
      if modele.dep_ut(1) then
        dependance_u = 'yes'
      else
        dependance_u = 'no'
      end

      //## dep_t
      if modele.dep_ut(2) then
        dependance_t = 'yes'
      else
        dependance_t = 'no'
      end

      //
      identification = graphique.id

      //- Informations generales
      if modele.sim=='super'|modele.sim=='csuper' then
        texte = ['General Information';
                 '-------------------';
                 ' '
                 'Object type                : Super Block';
                 'Identification             : '+identification;
                 'Object number in diagram   : '+string(k);
                 ' '
                 'Drawing function           : '+macro
                 ' ']
      else
        texte = ['General Information';
                 '--------------------';
                 ' '
                 'Object type                : bloc standard';
                 'Identification             : '+identification;
                 'Object number in diagram   : '+string(k);
                 ' ';
                 'Drawing function           : '+macro;
                 'Simulation function        : '+fonction(1);
                 'Simulation Function type   : '+language;
                 ' ';
                 'Bloc type                  : '+typ;
                 'Direct feed through        : '+dependance_u;
                 'Permanently active         : '+dependance_t]

        if %cpr<>list()&needcompile<>4 then
          cor = %cpr.cor
          corinv = %cpr.corinv
          path=list()
          for kp=1:size(super_path,'*')
            path(kp)=super_path(kp);
          end
          path($+1)=k
          ind=cor(path)

          if ind>0&ind<=size(corinv) then
            txt = ['Compiled structure Index   : '+string(cor(path));
                   ' ']
          else
            txt = ['Compiled structure Index   : suppressed';
                   ' ']
          end
        else
          txt = ['Compiled structure Index   : Not available';
                 ' ']
        end
        texte=[texte;txt]

      end

      //- Entrees / sorties
      tableau = ['Port type', 'Number', 'I/E', 'Size', 'Data type', 'Link';
                 '-', '-', '-', '-', '-', '-']

      //- Entrees standard
      for i = 1 : min(size(modele.in,'*'),size(graphique.pin,'*'))
        // General information
        txt = ['Regular input', string(i)]

        // Info. for implicit/explicit port
        ierr=execstr('tt=string(graphique.in_implicit(i))','errcatch')
        if ierr<>0 then
          txt = [txt, '?'] //not specified
        else
          if tt==[] then
            txt = [txt, '?'] //not specified
          else
            txt = [txt, tt]
          end
        end

        // Info. for size of port
        txt = [txt ,'['];
        tt=string(modele.in(i)) //first dim.
        if tt==[] then
          txt($) = txt($)+'?' //not specified
        else
          txt($) = txt($)+tt
        end
        txt($)=txt($)+';'
        ierr=execstr('tt=string(modele.in2(i))','errcatch')//sec. dim.
        if ierr<>0 then 
          txt($) = txt($)+'?' //not specified
        else
          if tt==[] then 
            txt($) = txt($)+'?' //not specified
          else
            txt($) = txt($)+tt
          end
        end
        txt($)=txt($)+']'

        // Info for data type
        ierr=execstr('ttyp=modele.intyp(i)','errcatch')
        if ierr<> 0 then
          tt='?' //not specified
        else
          if ttyp==-1 then tt='-1'
          elseif ttyp==1 tt='double'
          elseif ttyp==2 tt='complex'
          elseif ttyp==3 tt='int32'
          elseif ttyp==4 tt='int16'
          elseif ttyp==5 tt='int8'
          elseif ttyp==6 tt='uint32'
          elseif ttyp==7 tt='uint16'
          elseif ttyp==8 tt='uint8'
          else tt='?' //Unknown
          end
        end
        txt = [txt, tt]

        //Info for link
        txt = [txt,string(graphique.pin(i))]

        //update tableau
        tableau = [tableau; txt]
      end

      //- Sorties standard
      for i = 1 : min(size(modele.out,'*'),size(graphique.pout,'*'))
        //General information.
        txt = ['Regular output', string(i)]
        // Info. for implicit/explicit port
        ierr=execstr('tt=string(graphique.out_implicit(i))','errcatch')
        if ierr<>0 then
          txt = [txt, '?'] //not specified
        else
          if tt==[] then
            txt = [txt, '?'] //not specified
          else
            txt = [txt, tt]
          end
        end

        // Info. for size of port
        txt = [txt ,'['];
        tt=string(modele.out(i)) //first dim.
        if tt==[] then
          txt($) = txt($)+'?' //not specified
        else
          txt($) = txt($)+tt
        end
        txt($)=txt($)+';'
        ierr=execstr('tt=string(modele.out2(i))','errcatch') //sec. dim.
        if ierr<>0 then
          txt($) = txt($)+ '?' //not specified
        else
          if tt==[] then
            txt($) = txt($)+ '?' //not specified
          else
            txt($) = txt($)+ tt
          end
        end
        txt($)=txt($)+']'

        // Info for data type
        ierr=execstr('ttyp=modele.outtyp(i)','errcatch')
        if ierr<>0 then
          tt='?' //not specified
        else
          if ttyp==-1 then tt='-1'
          elseif ttyp==1 then tt='double'
          elseif ttyp==2 tt='complex'
          elseif ttyp==3 tt='int32'
          elseif ttyp==4 tt='int16'
          elseif ttyp==5 tt='int8'
          elseif ttyp==6 tt='uint32'
          elseif ttyp==7 tt='uint16'
          elseif ttyp==8 tt='uint8'
          else tt='?' //Unknown
          end
        end
        txt = [txt, tt]

        //Info for link
        txt = [txt,string(graphique.pout(i))]
        //update tableau
        tableau = [tableau; txt]
      end

      //- Entrees evenements
      for i = 1 : min(size(modele.evtin,'*'),size(graphique.pein,'*'))
        tableau = [tableau; 'Event input', string(i),'',..
                   string(modele.evtin(i)),'', string(graphique.pein(i))]
      end

      //- Sorties evenements
      for i = 1 : min(size(modele.evtout,'*'),size(graphique.peout,'*'))
        tableau = [tableau; 'Event output', string(i),'',..
                   string(modele.evtout(i)),'', string(graphique.peout(i))]
      end

      //
      texte = [texte;
               'Input / output';
               '--------------';
               ' '
               tabule(tableau);
               ' ']

      //- Documentation
      documentation=objet.doc
      if type(documentation)==15 then
        if size(documentation)>=2 then
          funname=documentation(1);
          doc=documentation(2)
          if type(funname)==10 then
            ierr=execstr('docfun='+funname,'errcatch')
            if ierr<>0 then
              messagebox(msprintf(_('function %s not found'),funname),'error','modal')
              return
            end
          else
            docfun=funname
          end
          ierr=execstr('doc=docfun(''get'',doc)','errcatch')
          if ierr==0&doc<>[] then
	    D=_('Documentation');
            texte = [texte;
                     _('Documentation');
                     part('-',ones(1,length(_('Documentation'))))
                     ' '
                     doc;
                     ' ']
          end
        end
      end

    //## Liens ##//
    case 'Link' then
      identification = objet.id
      if objet.ct(2) == 1 then
        sous_type = _('Regular Link')
      else
        sous_type = _('Event link')
      end

      //- Informations generales
      labels=[_('Object type') 
	  _('Identification') 
	  _('Object number in diagram')]
      values=[sous_type;
	      identification;
	      string(k)];
      texte = [_('General information');
	       part('-',ones(1,length(_('General information'))))
               ' '
               part(labels,1:max(length(labels)+10))+':'+values]

      //- Compiled link memory zone
      from=objet.from
      if %cpr<>list() then
        if sous_type == _('Regular Link') then
          scs_m_tmp=scs_m

          while %t
            obji=scs_m_tmp.objs(from(1))
            if obji.model.sim=='lsplit' then

            elseif obji.model.sim=='super' then
              super_path;
              super_path($+1)=from(1)
              scs_m_tmp=obji.model.rpar
              for obji=scs_m_tmp.objs
                if typeof(obji)=='Block' then
                  if obji.model.sim=='output'&obji.model.ipar==from(2) then
                    break
                  end
                end
              end
            else
              break
            end
            #link=obji.graphics.pin
            from=scs_m_tmp.objs(#link).from
          end

          cor = %cpr.cor
          path=list()
          for kp=1:size(super_path,'*')
            path(kp)=super_path(kp);
          end
          path($+1)=from(1)
          ind=cor(path)

          if type(ind)==1&ind<>0 then
            kl=%cpr.sim('outlnk')(%cpr.sim('outptr')(ind)+(from(2)-1));
            kmin=[];kmax=[];count=1;
            for j=1:size(%cpr.state('outtb'))
              if j==kl then
                kmin=count;
                kmax=count+size(%cpr.state.outtb(j),'*')-1;
                break
              else
                count=count+size(%cpr.state.outtb(j),'*');
              end
            end

            txt = [_('Compiled link memory :')
		   '               '+_('Zone')+'  : outtb('+string(kl)+')'
                   '               '+_('Area')+'  : ['+string(kmin)+':'+string(kmax)+']'
                   '               '+_('Type')+'  : '+typeof(%cpr.state.outtb(kl))'
                   '               '+_('Size')+'  : '+sci2exp(size(%cpr.state.outtb(kl)))';
                   ' ']
          else
            txt = [_('Compiled link memory zone  : Not available');' ']
          end
        end
      else
        txt = [_('Compiled link memory zone  : Not available');' ']
      end
      texte=[texte;txt]

      //- Connexions
      tableau = [' ', _('Block'), _('Port') ;
                 '-', '-', '-';
                 _('From'), string(objet.from(1:2));
                 _('to'), string(objet.to(1:2))]

      texte = [texte;
	       _('Connections');
	       part('-',ones(1,length(_('Connections'))))
               ' '
               tabule(tableau);
               ' ']
    //#####//
    else
      texte=[]
    end
endfunction
