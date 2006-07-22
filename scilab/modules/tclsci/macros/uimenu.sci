// -----------------------------------------------------------
// Allan CORNET
// INRIA 2005
// -----------------------------------------------------------
function h=uimenu(varargin)
  Rhs=length(varargin);
  if (Rhs == 0) then
    h=CreateUiMenu(0);
    TCL_scf(TCL_gcf());
  else
    if (IsAScalar(varargin(1))==%T) then
      if (Rhs < 2) then
        h=CreateUiMenu(varargin(1));
        TCL_scf(TCL_gcf());
      else
        [bOK,k]=TestParamUiMenu(varargin,2);
        if ( bOK == %F) then
          error('Incorrect property '+string(varargin(k)),999);
        else
            hPar=varargin(1);
            h=CreateUiMenu(hPar);
            
            for i=2:2:Rhs do
            	if GetUiControlPropertyName(varargin(i))=='parent' then
            		hPar=varargin(i+1);
            		h=CreateUiMenu(hPar);
            		TCL_EvalStr("set TclScilabTmpVar [deleteMenu ()];");
            	else
                set(h,GetUiControlPropertyName(varargin(i)),varargin(i+1));
              end  
            end
        end
      end
    else
      [bOK,k]=TestParamUiMenu(varargin,1);
      if ( bOK == %F) then
        error('Incorrect property '+string(varargin(k)),999);
      else
        TCL_scf(TCL_gcf());
        if GetUiControlPropertyName(varargin(1))<>'parent' then
          h=CreateUiMenu(0);
        end
                    
        for i=1:2:Rhs do
          if GetUiControlPropertyName(varargin(i))=='parent' then
          	hPar=varargin(i+1);
            h=CreateUiMenu(hPar);
          else
            set(h,GetUiControlPropertyName(varargin(i)),varargin(i+1));
          end
        end
      end
    end
  end
endfunction
// -----------------------------------------------------------
function h=CreateUiMenu(FigureHandle)
  if (IsAScalar(FigureHandle)==%T) then
    TCL_Command="set TclScilabTmpVar [CreateUIMenu "+string(FigureHandle)+"];"
    TCL_EvalStr(TCL_Command);
    h=evstr(TCL_GetVar('TclScilabTmpVar'));
  else
    error(string(FigureHandle)+' Value must be numeric.',999);
  end
endfunction
// -----------------------------------------------------------
function [bOK,k]=TestParamUiMenu(ArgsIn,IndiceStart)
  bOK=%F;
  k=-1;
  for i=IndiceStart:2:length(ArgsIn) do
    PropertyName=GetUiControlPropertyName(ArgsIn(i));
    if (PropertyName=='') then
    	bOK=%F;
    	k=i;
      return;
    else
      bOK=%T;
    end
  end
endfunction
// -----------------------------------------------------------
