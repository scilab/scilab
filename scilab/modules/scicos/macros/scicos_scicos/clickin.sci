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

function [o, modified, newparameters, needcompile, edited] = clickin(o)
//**
//**
//
//  o             : structure of clicked object, may be modified
//
//  modified      : boolean, indicates if simulation time parameters
//                  have been changed
//
//  newparameters : only defined for super blocks, gives pointers to
//                  sub-blocks for which parameters or states have been changed
//
//  needcompile   : indicates if modification implies a new compilation
//
//

if needcompile==4 then
      %cpr = list() ;
end  // for standard_document to work

modified = %f ;        //** not very clear internal flags
newparameters = list();
needcompile = 0 ;

//**
if %diagram_open then //** %diagram_open is a global variable that signal if the diagram is show

  Cmenu = check_edge(o, Cmenu, %pt);

  if Cmenu==("Link") then
    //we have clicked near a port
    [Cmenu] = resume("Link") ; //** EXIT with Link operation
  end

end
//**

//**---------------------------------------------------------------------
if typeof(o)=="Block" then
  //** ----------------------------- Block ------------------------------

  //**----------------Look for a SuperBlock ----------------------------
  if o.model.sim=="super" then

      lastwin = curwin ; // save the current window

      global inactive_windows;
      jkk = [] ;

      for jk=1:size(inactive_windows(2),'*')
         if isequal(inactive_windows(1)(jk), super_path) then
           jkk = [jkk,jk]
         end
      end

      curwinc = -1 ;

      for jk=jkk
        curwinc = inactive_windows(2)(jk) ;
        inactive_windows(1)(jk) = null()  ;
        inactive_windows(2)(jk) = []      ;
        curwin = curwinc ;
      end

      if curwinc<0 then
        curwin = get_new_window(windows) ; //** need a brand new window where open the
      end                                  //** super block

      if %diagram_open then      //** if the window is already open
        gh_curwin = scf(curwin); //** recover the handle
      end                        //**

      ierr = execstr('[o_n,needcompile,newparameters]='+o.gui+'(''set'',o)','errcatch') ;
      // the errcatch here is introduced to get around the bug #2553

      if ierr<>0 then //** if any erros is detected
        disp('Error in GUI of block '+o.gui)
        edited = %f ;
        return ; //** EXIT point
    end

    //** Added to unset the last upper left point
    //** of the dialog box
    //-- TCL_UnsetVar('numx')
    //-- TCL_UnsetVar('numy')

    //** Check is this comments is still valid
    //edited variable is returned by SUPER_f -- NO LONGER TRUE
    if ~%exit then
      edited = ~isequalbitwise(o,o_n) //diffobjs(o,o_n)
      if edited then
	o = o_n ;
	modified = prod( size(newparameters) )>0 ;
      end
    end

    curwin = lastwin ;
    if (~(or(curwin==winsid()))) then
          Cmenu = resume("Open/Set"); //** if the curwin is not present
    end

    gh_curwin = scf(curwin);


  //**-------------------- Mask C superblock  -----------------------------
  elseif o.model.sim=="csuper"& o.model.ipar==1 then
    ierr=execstr('[o_n,needcompile,newparameters]='+o.gui+'(''set'',o)','errcatch')

    //** Added to unset the last upper left point
    //** of the dialog box
    //-- TCL_UnsetVar('numx')
    //-- TCL_UnsetVar('numy')

    if ierr<>0 then
      disp('Error in GUI of block '+o.gui);
      edited = %f ;
      return ; //** EXIT point
    end
    modified = prod(size(newparameters))>0 // never used because if there is a change
                                           // needcompile>=2 and newparams not used
    edited = ~isequalbitwise(o,o_n) ;
    if edited then
      o = o_n ;
    end

  //**-------------------- C superblock ??? -----------------------------
  elseif o.model.sim=="csuper" then
    ierr=execstr('[o_n,needcompile,newparameters]='+o.gui+'(''set'',o)','errcatch')

    //** Added to unset the last upper left point
    //** of the dialog box
    //-- TCL_UnsetVar('numx')
    //-- TCL_UnsetVar('numy')

    if ierr<>0 then
      disp('Error in GUI of block '+o.gui)
      edited = %f ;
      return ; //** EXIT point
    end
    modified = prod(size(newparameters))>0
    edited = modified  // Not sure it is correct.
    if edited then
      o = o_n ;
    end



  //**--------------------- Standard block -------------------------------
  else

    //** RELEASE
    ierr = execstr('o_n='+o.gui+'(''set'',o)','errcatch') ; //** stardard, protected, operation

    //** ierr = execstr('o_n='+o.gui+'(''set'',o)'); //** DEBUG ONLY : no error catch :(

    //** Added to unset the last upper left point
    //** of the dialog box
    //-- TCL_UnsetVar('numx')
    //-- TCL_UnsetVar('numy')

    if ierr<>0 then
      disp('Error in GUI of block '+o.gui)
      edited = %f ;
      return
    end
    //Alan - 09/02/07 : replace <> operator by ~isequal
    //because <> operator crash for sublist with int elements
    //edited = or(o<>o_n) ;
    edited = ~isequal(o,o_n) ;
    if edited then
      model = o.model
      model_n = o_n.model
      if ~is_modelica_block(o) then
	modified = or(model.sim<>model_n.sim)|..
		      ~isequal(model.state,model_n.state)|..
		      ~isequal(model.dstate,model_n.dstate)|..
		      ~isequal(model.odstate,model_n.odstate)|..
		      ~isequal(model.rpar,model_n.rpar)|..
		      ~isequal(model.ipar,model_n.ipar)|..
		      ~isequal(model.opar,model_n.opar)|..
		      ~isequal(model.label,model_n.label)

	if ~modified then
	  for i=1:lstsize(model.opar)
	    if typeof(model.opar(i))<>typeof(model_n.opar(i)) then
	      modified = %t
	      break
	    end
	  end
	end

	if or(model.in<>model_n.in)|or(model.out<>model_n.out)|...
	   or(model.in2<>model_n.in2)|or(model.out2<>model_n.out2)|...
           or(model.outtyp<>model_n.outtyp)|or(model.intyp<>model_n.intyp) then
	  // input or output port sizes or type changed
	  needcompile=1
	end

        if or(model.firing<>model_n.firing)  then
	  // initexe changed
	  needcompile=2
	end

        if (size(model.in,'*')<>size(model_n.in,'*'))|..
	      (size(model.out,'*')<>size(model_n.out,'*'))|..
	       (size(model.evtin,'*')<>size(model_n.evtin,'*'))|..
	        (size(model.evtout,'*')<>size(model_n.evtout,'*')) then
	  // number of input (evt or regular ) or output (evt or regular) changed
	  needcompile=4
	end

        if model.sim=='input'|model.sim=='output' then
	  if model.ipar<>model_n.ipar then
	    needcompile=4
	  end
	end

        if or(model.blocktype<>model_n.blocktype)|..
	      or(model.dep_ut<>model_n.dep_ut)  then
	  // type 'c','d','z','l' or dep_ut changed
	  needcompile=4
	end

        if (model.nzcross<>model_n.nzcross)|(model.nmode<>model_n.nmode) then
	  // size of zero cross changed
	  needcompile=4
	end

        if prod(size(model_n.sim))>1 then
	  if model_n.sim(2)>1000 then  // Fortran or C Block
	    if model.sim(1)<>model_n.sim(1) then  //function name has changed
	      needcompile=4
	    end
	  end
	end

      else //implicit block
	//force compilation if an implicit block has been edited
	modified=or(model_n<>model)
	eq=model.equations;eqn=model_n.equations;

	if or(eq.model<>eqn.model)|or(eq.inputs<>eqn.inputs)|or(eq.outputs<>eqn.outputs) then
	  needcompile=4
	end

        if (size(o.model.sim)>1) then
	  if (o.model.sim(2)==30004) then // only if it is the Modelica generic block
	    if or(o.graphics.exprs<>o_n.graphics.exprs) then  // if equation in generic Modelica Mblock change
	      needcompile=4
	      modified=%t;
	    end
	  end
	end

      end
      o=o_n

    end

  end

//**---------------------- Link -------------------------------------------------
elseif typeof(o)=="Link" then

  [Cmenu] = resume("Link")

//**---------------------- Text -------------------------------------------------
elseif typeof(o)=="Text" then

  ierr=execstr('o_n='+o.gui+'(''set'',o)','errcatch') ;

  //** Added to unset the last upper left point
  //** of the dialog box
  //-- TCL_UnsetVar('numx')
  //-- TCL_UnsetVar('numy')

  if ierr<>0 then
    disp('Error in GUI of block '+o.gui)
    edited=%f
    return
  end
  edited = or(o<>o_n) ;
  o = o_n ;

end

endfunction
