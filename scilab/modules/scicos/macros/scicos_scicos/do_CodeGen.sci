//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//                      - Ramine Nikoukhah <ramine.nikoukhah@inria.fr>
//                      - Rachid Djenidi
//
//                      - Scilab 5 update by Simone Mannori 
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

function [scs_m,edited,needcompile,Cmenu,ok]=do_CodeGen(scs_m,%pt)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  global Scicos_commands
  K=find(Select(:,2)==%win)
  if K==[] then
    K = getobj(scs_m, %pt(:))
  else
    K=Select(K,1)
  end
  Cmenu=[];//%pt=[];
  
  if K==[] then
    messagebox(_("No selected block in the current Scicos window."),'error','modal')
    return
  end  	
  if size(K,'*')>1 then
    messagebox(_("Only one block can be selected in current window for this operation."),'error','modal')
    return
  end 
  k = K;
  ALL = %f;
  //** check if we have clicked near an object
  if k==[] then
    ALL = %t;
    //return
    //** check if we have clicked near a block
  elseif typeof(scs_m.objs(k))<>'Block' then
    messagebox(_("Code generation applies only to Super Blocks."),'modal')
    Cmenu=[];%pt=[];
    return
  end
    if ~ALL then
      //** If the clicked/selected block is really a superblock
      //**             <k>
      if scs_m.objs(k).model.sim(1)=='super' | scs_m.objs(k).gui =='DSUPER'  then
         global scs_m_top

         //## test to know if a simulation have not been finished
         if alreadyran then
            Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
                             '[alreadyran,%cpr]=do_terminate();'+...
                             '%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1';
                             '%pt='+sci2exp(%pt)+';Cmenu='"XcosMenuCodeGeneration"'';]

         //## test to know if the precompilation of that sblock have been done
         elseif ( isequal(scs_m_top,[]) | isequal(scs_m_top,list()) ) then
	   Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
                             'global scs_m_top; scs_m_top=adjust_all_scs_m(scs_m,'+sci2exp(k)+');'+...
                             '%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1';
                             '%pt='+sci2exp(%pt)+';Cmenu='"XcosMenuCodeGeneration"'';]

         else
            // Got to target sblock.
            scs_m_top=goto_target_scs_m(scs_m_top)
            //## call do_compile_superblock
            [ok, XX, gui_path, flgcdgen, szclkINTemp, freof] = ...
                              do_compile_superblock42(scs_m_top, k);

            clearglobal scs_m_top;

            //**quick fix for sblock that contains scope
            gh_curwin=scf(curwin)

            if ok==%t then
              scs_m = changeports(scs_m,list('objs',k), XX);  //scs_m.objs(k)=XX
              scs_m = draw_sampleclock(scs_m,XX,k,flgcdgen, szclkINTemp, freof);
              edited      = %t ;
              needcompile = 4  ;

              // The interface function must be defined on the first level
              Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
                               'ppprot=funcprot();funcprot(0);';
                               'ierr=execstr(''exec('''''+gui_path+''''');'',''errcatch'');';
                                'funcprot(ppprot);clear ppprot;';
                               'if ierr<> 0 then messagebox(''Cannot load the '''''+gui_path+''''' file'',''modal'');end';
                                '%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1;'
                               'Cmenu='"XcosMenuReplot'"']
            else
              if ok<>-1 then
                Cmenu = "XcosMenuOpenSet"
              end
            end
            //## remove variables
            clear XX flgcdgen k szclkINTemp freof gui_path
         end

      else
        //** the clicked/selected block is NOT a superblock
        messagebox(_("Code Generation only works for a Superblock !\n"),"modal","error")
      end

   //@@
   else

     //@@ do_terminate if necessary
     if alreadyran then [alreadyran,%cpr]=do_terminate(), end

     //## call do_compile_superblock
     [ok, XX, gui_path, flgcdgen, szclkINTemp, freof] = ...
                            do_compile_superblock42(scs_m, -1);

     //** quick fix for sblock that contains scope
     gh_curwin=scf(curwin)

     if ok==%t then
       props             = scs_m.props;
       scs_m             = get_new_scs_m()
       scs_m.props       = props
       XX.graphics.pein  = 2
       XX.graphics.peout = 2
       YY                = scicos_link(xx   = [20;20;70;70;20;20],...
                                       yy   = [-5.70;-25;-25;60;60;45.7],...
                                       ct   = [5;-1],...
                                       from = [1,1,0],...
                                       to   = [1,1,1])
       scs_m.objs(1)     = XX
       scs_m.objs(2)     = YY

       edited      = %t ;
       needcompile = 4  ;
       Cmenu       = "XcosMenuReplot"
       %pt=[]
       //## remove variables
       clear XX YY flgcdgen k szclkINTemp freof gui_path
     end

   end

endfunction


function scs_m = goto_target_scs_m(scs_m)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  //## look if we want generate a sblock
  //## contained in a sblock
  kk=super_path

  //## scs_temp becomes the scs_m of the upper-level sblock
  if size(kk,'*')>1 then
    while size(kk,'*')>1 do
      scs_m=scs_m.objs(kk(1)).model.rpar
      kk(1)=[];
    end
    scs_m=scs_m.objs(kk).model.rpar
  elseif size(kk,'*')>0 then
    scs_m=scs_m.objs(kk).model.rpar
  end
endfunction
