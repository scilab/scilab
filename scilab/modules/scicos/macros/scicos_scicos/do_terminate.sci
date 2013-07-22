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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [alreadyran,%cpr] = do_terminate(scs_m,%cpr)
    // Terminate the current simulation
    // Ne rend pas la main à l'utilisateur en cas d'erreur


    //** if sim or and state is not in %cpr
    if prod(size(%cpr))<2 then
        alreadyran = %f ;
        return          ;  //** EXIT
    end

    par = scs_m.props ;

    //** if the simulation have already ran
    //** and is not finished
    if alreadyran then
        alreadyran = %f ;
        state=%cpr.state;

        //** win = xget('window');
        gh_win = gcf();

        //** run scicosim via 'finish' flag
        ierr = execstr("[state,t]=scicosim(%cpr.state,par.tf,par.tf,%cpr.sim,"+..
        "''finish'',par.tol)","errcatch")

        //** xset('window',win)
        scf(gh_win);

        %cpr ; //get write access at the variable
        %cpr.state = state ;//not always called with second arg

        //**----------------------------------
        if ierr<>0 then
            str_err=split_lasterror(lasterror());
            title_err="End problem."

            kfun = curblock() ;
            corinv = %cpr.corinv
            if kfun<>0 then  //** block error
                path = corinv(kfun)

                if type(path)==15 then //** modelica block
                    spec_err="The modelica block returns the error :";
                    messagebox([title_err;spec_err;str_err],"modal");

                else //** all other type of blocks
                    obj_path=path(1:$-1)
                    spec_err="block"
                    blk=path($)
                    scs_m_n=scs_m;
                    //** check if we can open a window
                    //** Alan: we can improve that piece of code
                    //**       to also returns the name of the comput. func.
                    for i=1:size(path,"*")
                        if scs_m_n.objs(path(i)).model.sim=="super" then
                            scs_m_n=scs_m_n.objs(path(i)).model.rpar;
                        elseif scs_m_n.objs(path(i)).model.sim=="csuper" then
                            obj_path=path(1:i-1);
                            blk=path(i);
                            //spec_err='csuper block (block '+string(path(i+1))+')'
                            spec_err="csuper block"
                            break;
                        end
                    end
                    spec_err="The highlighted "+spec_err+" returns the error :";
                    //**
                    scf(curwin)
                    //** call bad_connection
                    bad_connection(path,...
                    [title_err;spec_err;str_err],0,1,0,-1,0,1)
                end
            else//** simulateur error
                messagebox(["End problem:";str_err],"modal")
            end
        end
        //**---------------------------------

        //** xset('window',curwin)
        scf(curwin);

    end
endfunction
