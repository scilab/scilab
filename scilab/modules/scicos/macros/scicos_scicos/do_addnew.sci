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

function [scs_m, fct] = do_addnew(scs_m)
//**   
//**
//** "fct" is a flag control variable used by the calling function Addnewblock
//**       it is used to signal modification of the datastructure
//**
//**---------- This part handle the file related functions and error handling --------------

  fct = [] ;
  [ok,name] = getvalue( 'Get block GUI function name', ['Name'],list('str',1),emptystr() )

  if ~ok then return, end ; //** --> Exit point

  name = stripblanks(name);

  if name==emptystr() then
      message("No block name specified");
      return
  end ; //** --> Exit point

  to_get = %f ;

  if exists(name)==0 then
    to_get = %t ;
  else
    execstr('tp=type('+name+')')
    to_get = tp<>11 & tp<>13 
  end

  if to_get then // try to get it

    message(['Problem loading block '+name+'.';
             'Use Activate_Scilab_Window and redefine it in Scilab.'] ) ;
    return;

    //path = name+'.sci'
    //path = getfile("*.sci", path, title="File containing function: "+name)
    //if length(path)<=0 then return,end
    //[u,err]=file('open',path,'old','formatted')
    //if err<>0 then
    //  message(path+' file, Not found')
    //  return
    //end
    //if execstr('exec(u)','errcatch')<>0 then
    //  file('close',u)
    //  message([name + " erroneous function:"; lasterror()])
    //  return
    //end
    //file('close',u)
    //if ~exists(name) then
    //   message(name+" is not defined in this file")
    //   return
    //end
    //fct = path

  else

    fct = emptystr()

  end //**

  //define the block
  ierror = execstr('blk='+name+'(''define'')','errcatch')
  if ierror <>0 & ierror <>4 then
    message([ "Error in GUI function"; lasterror() ])
    fct = [] ;
    return
  end

  if ierror == 4 then
    irr=message(['Error in GUI function--The error was:';
		            lasterror();'It could be an old GUI';
	              'Should I try to translate (no guarantee)?'],['yes','no'])

    if irr==2 then

      fct=[]
      return

    else
      funcprot_val=funcprot()
      funcprot(0)
      standard_define=standard_define_old;
      funcprot(funcprot_val)
      ierror=execstr('blk='+name+'(''define'')','errcatch')

      if ierror <>0 then
	         message("Translation did not work, sorry")
	         fct=[]
	         return
      end

      do_version = do_version; //load do_version and its subfunctions

      ierror = execstr('blk=up_to_date(blk)','errcatch');

      if ierror <>0 then
        message("Translation did not work, sorry")
        fct=[]
        return
      end

    end //** irr

  end //** ierror

  //**------ Al@n's update 2 ---------/////////////
  // update blk !

  o_new=scicos_block();
  T = getfield(1,blk);

  for k=2:size(T,2)
     select T(k)
     //*********** graphics **********//
     case 'graphics' then
        ogra  = blk.graphics;
        G     = getfield(1,ogra);
        G_txt = [];
        for l=2:size(G,2)
          G_txt = G_txt + G(1,l) + ...
                    "=" + "ogra." + G(1,l);
          if l<>size(G,2) then
              G_txt = G_txt + ',';
          end
        end
        G_txt = 'ogra=scicos_graphics(' + G_txt + ')';
        ierr  = execstr(G_txt,'errcatch')
        if ierr<>0 then
          error("Problem in convertion of graphics in block.")
        end
        o_new.graphics = ogra;
        //*******************************//

        //************* model ***********//
        case 'model' then
          omod  = blk.model;
          M     = getfield(1,blk.model);
          M_txt = [];
          for l=2:size(M,2)
             M_txt = M_txt + M(1,l) + ...
                      "=" + "omod." + M(1,l);
             if l<>size(M,2) then
                 M_txt = M_txt + ',';
             end
          end
          M_txt = 'omod=scicos_model(' + M_txt + ')';
          ierr  = execstr(M_txt,'errcatch')
          if ierr<>0 then
            error("Problem in convertion of model in block.")
          end
          o_new.model = omod;
          //*******************************//

          //************* other ***********//
          else
            T_txt = "blk."+T(k);
            T_txt = "o_new." + T(k) + "=" + T_txt;
            ierr  = execstr(T_txt,'errcatch')
            if ierr<>0 then
               error("Problem in convertion in objs.")
            end
          //*******************************//

     end  //end of select T(k)
  end  //end of for k=
  blk = o_new;
  //**------------------------------------/////////

  //**------ Al@n's update ---------/////////////
  if blk.model.sim(1)=='super'|...
      blk.model.sim(1)=='csuper' then

    scs_m_super = blk.model.rpar;

    //check version
    current_version = get_scicos_version()
    scicos_ver = find_scicos_version(scs_m_super)

    //do version
    if scicos_ver<>current_version then
      ierr=execstr('scs_m_super=do_version(scs_m_super,scicos_ver)','errcatch')
      if ierr<>0 then
        message("Can''t import block in scicos, sorry (problem in version)")
        fct=[]
        return
      end
      blk.model.rpar = scs_m_super;

      //check name
      if scs_m_super.props.title(1)<>name then
         scs_m_super.props.title(1)=name
      end

      //generate a new interfacing function in TMPDIR
      if blk.model.sim(1)=='super' then
        save_super(scs_m_super,TMPDIR,blk.graphics.gr_i,blk.graphics.sz)
      elseif blk.model.sim(1)=='csuper' then
        save_csuper(scs_m_super,TMPDIR,blk.graphics.gr_i,blk.graphics.sz)
        tt=mgetl(TMPDIR+'/'+name+'.sci');
        mputl(tt,TMPDIR+'/'+name+'_new.sci');
        //**------ R@min's update ---------/////////////
         [u,err]=file('open',TMPDIR+'/'+name+'.sci','unknown')
         if err<>0 then
           message('The file '+TMPDIR+'/'+name+'.sci'+' cannot be opened.')
           return
         end

         Txte=['function [x,y,typ]='+name+'(job,arg1,arg2)';
               'x=[];y=[],typ=[]';
               'if job<>''define'' then ';
               '  if argn(2)<2 then arg1=[],end'
               '  [x,y,typ]=%fonct'+name+'(job,arg1)';
               'else'
               ];
         ierr=execstr('write(u,Txte,''(a)'')','errcatch','n')
         if ierr<>0 then 
               message('Impossible to write in '+TMPDIR+'/'+name+'.sci'+'; possibly locked.')
               file('close',u)
               return
         end
         dimen=blk.graphics.sz// /20  // why RN
         dimen=dimen(:)'
         textdef=['  model=scicos_model()']
         model=blk.model
         cc=getfield(1,model)
         cos2cosf(u,model.rpar,0)

         for ch=cc(2:$)
           if ch=='rpar' then
             textdef=[textdef;
                      '  model.rpar=scs_m_1'];
           else
             chval=sci2exp(evstr('model.'+ch))
             textdef=[textdef;
                      '  model.'+ch+'='+chval(1);
                      chval(2:$)];
           end
         end

         textdef=[textdef;
                  '  exprs='+sci2exp(blk.graphics.exprs)
                 ]

         gr_i_tmp = sci2exp(blk.graphics.gr_i);
         if size(gr_i_tmp,1)<>1 then
           textdef=[textdef;
                    '  gr_i='+gr_i_tmp(1)
                    '       '+gr_i_tmp(2:$)];
         else
           textdef=[textdef;
                    '  gr_i='+gr_i_tmp];
         end

         textdef=[textdef;
                  '  x=standard_define('+sci2exp(dimen)+',model,exprs,gr_i)']
         txt=[textdef
              'end'
              'endfunction'
              ''
              fun2string(evstr(name),'%fonct'+name);
             ]
         write(u,txt,'(a)')
         file('close',u)
        //**-------------------------------/////////////
      end
      nam_file=strcat([name,name+'_new']+'.sci',' ')
      message(["Old csuper/super block have been detected !";
               "New interfacing functions "+nam_file;
               " have been re-generated in "+TMPDIR+".";
               "Please save and edit the generated file at your convenience";
               "to have an updated interfacing function of that block."])
    end
  end
  //**------------------------------------/////////////

//**--------------------------------------------------------------------------------------
//** The requested block is valid, then it is accepted
//** ---------------------- The graphics interaction begin here --------------------------

//** This code basically works ( :-) ) : it is the "Replot" that need attention :)

  xinfo("Choose block position in the window");

  blk.graphics.sz = 20 * blk.graphics.sz;
  %xc = 100; %yc = 100; //** default start position
   xy = [%xc,%yc] ;
  blk.graphics.orig = xy ; 

  gh_blk = drawobj(blk); //** draw the block (in the buffer) - using the corresponding Interface Function
                         //** Scilab Language - of the specific block (blk) and get back the graphic handler
	  	         //** to handle the block as a single entity

//**-----------------------------------------------------------------
//** ---> main loop that move the empty box until you click
  rep(3)=-1 ;
  while rep(3)==-1 , //move loop

    // get new position
    rep = xgetmouse([%t,%t]); //** 

    //** Protection from window closing
    if rep(3)==-1000 then //active window has been closed
      [%win,Cmenu] = resume(curwin,"Quit")
    end

    xm = rep(1) ; //** coordinate acquisition  
    ym = rep(2) ;
    dx = xm - %xc ; //** incremental coordinate 
    dy = ym - %yc ;
    drawlater();
     move (gh_blk , [dx dy]);
     //**draw(gh_blk.parent); //** re-draw the graphic object and show on screen
     drawnow(); 
     //** show_pixmap() ; //** not useful on Scilab 5

     %xc = xm ;%yc = ym ; //** position update

  end //** ---> of the while loop
//**----------------------------------------------------------------------
  
  //** out-of-the-loop window closing protection 
  gh_window = gcf(); 
  if gh_window.figure_id <> curwin then
    //active window has been closed
    [%win, Cmenu] = resume(curwin,"Quit")
  end

  xinfo(" "); 

  xy = [%xc,%yc];
  blk.graphics.orig = xy ; //** update object position in the data strucure

  scs_m_save = scs_m       ;
  nc_save    = needcompile ;
   
  scs_m.objs($+1) = blk ; //** add the object to the data structure

  needcompile = 4     
  [scs_m_save, nc_save, enable_undo, edited] = resume(scs_m_save,nc_save,%t,%t)

endfunction

//**---------------------------------------------------------------------------------------------------

function objsi = up_to_date(o)
  if size(o(2)) > 8 then
    if type(o(2)(9))==15 then 
      gr_io=o(2)(9)(1);
      if o(2)(9)(2)<>[] then
	back_col=o(2)(9)(2);,
      end
    else
      gr_io=o(2)(9);
      back_col=8
    end
    gr_i=convert_gri(o(5),gr_io);
    if gr_i==[] then gr_i=gr_io;, end
  elseif size(o(2)) < 9 then
    gr_i=[];
    back_col=8
  end
  gr_i=list(gr_i,back_col)

  mdl=o(3);
  if size(o(3))<=12 then 
    mdl(13)=''; mdl(14)=[] ; mdl(15)='';
  elseif size(o(3))<=13 then 
    mdl(14)=[] ; mdl(15)='';
  elseif size(o(3))<=14 then 
    mdl(15)='';
  end

  if mdl(1)(1)=='super'|mdl(1)(1)=='csuper' then
    if type(mdl(8))==15 then
      mdl(8)=do_version27(mdl(8))
    end
  end

  graphics=scicos_graphics(orig=o(2)(1),sz=o(2)(2),flip=o(2)(3),..
			   exprs=o(2)(4),pin=o(2)(5),pout=o(2)(6),..
			   pein=o(2)(7),peout=o(2)(8),gr_i=gr_i,..
			   id=mdl(15))

  model=scicos_model(sim=mdl(1),in=mdl(2),out=mdl(3),evtin=mdl(4),..
		     evtout=mdl(5),state=mdl(6),dstate=mdl(7),..
		     rpar=mdl(8),ipar=mdl(9),blocktype=mdl(10),..
		     firing=mdl(11),dep_ut=mdl(12),label=mdl(13))
  
  
  objsi=scicos_block(graphics=graphics,model=model,gui=o(5),..
		     doc=mdl(14))
  if objsi.gui=='ESELECT_f' then objsi.model.sim(2)=-2,end

endfunction


