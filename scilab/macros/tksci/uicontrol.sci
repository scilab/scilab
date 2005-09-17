// -----------------------------------------------------------
// Allan CORNET
// INRIA 2005
// -----------------------------------------------------------
function h=uicontrol(varargin)
  Rhs=length(varargin);
  if (Rhs == 0) then
    h=CreateUiControl(0,"pushbutton");
    TCL_scf(TCL_gcf());
  else
    if (IsAScalar(varargin(1))==%T) then
      if (Rhs < 2) then
        figure(varargin(1));
        h=CreateUiControl(varargin(1),"pushbutton");
        TCL_scf(TCL_gcf());
      else
        if (Rhs <3) then
          error('Too many input arguments. ',999);
        else
          [bOK,k]=TestParamUiControl(varargin,2);
          
          if ( bOK == %F) then
            error('Incorrect property '+string(varargin(k)),999);
          else
            h=varargin(1);
            k=FindStyleInLine(varargin,2);
            if (k<>-1) then
              h=CreateUiControl(h,varargin(k+1));
            else
              h=CreateUiControl(h,"pushbutton");
            end
            TCL_scf(TCL_gcf());
            for i=2:2:Rhs do
            	if (k<>i) then
            	  if ( (GetUiControlPropertyName(varargin(i))=='value') & (IsAScalar(varargin(i+1))==%T) ) then
            	    set(h,GetUiControlPropertyName(varargin(i)),string(varargin(i+1)));
            	  else
    			        set(h,GetUiControlPropertyName(varargin(i)),varargin(i+1));
    			      end
    			    end
    			  end
          end
        end
      end
    else
      [bOK,k]=TestParamUiControl(varargin,1);
      if ( bOK == %F) then
            error('Incorrect property '+string(varargin(k)),999);
      else
        if (Rhs ==1) then
          error('Too many input arguments. ',999);
        else
          k=FindStyleInLine(varargin,1);
          if (k<>-1) then
            h=CreateUiControl(0,varargin(k+1));
          else
            h=CreateUiControl(0,"pushbutton");
          end
    			TCL_scf(TCL_gcf());
    			for i=1:2:Rhs do
    				if (k<>i) then
    				  if ( (GetUiControlPropertyName(varargin(i))=='value') & (IsAScalar(varargin(i+1))==%T) ) then
       				    set(h,GetUiControlPropertyName(varargin(i)),string(varargin(i+1)));
    				  else
    			      set(h,GetUiControlPropertyName(varargin(i)),varargin(i+1));
    			    end
    			  end
    			end
        end
      end
    end
  end
endfunction
// -----------------------------------------------------------
function h=CreateUiControl(FigureHandle,Style)
  if (IsAScalar(FigureHandle)==%T) then
    TCL_Command="set TclScilabTmpVar [CreateUIControl "+ ..
                 string(FigureHandle)+" "+ ..
                 GetInternalStyleName(GetUiControlStyleName(Style))+ ..
                 "];"
    TCL_EvalStr(TCL_Command);
    h=evstr(TCL_GetVar('TclScilabTmpVar'));
    TCL_UnsetVar('TclScilabTmpVar');
  else
    error(string(FigureHandle)+' Value must be numeric.',999);
  end
endfunction
// -----------------------------------------------------------
function [bOK,k]=TestParamUiControl(ArgsIn,IndiceStart)
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
function k=FindStyleInLine(ArgsIn,IndiceStart)
  k=-1
  for i=length(ArgsIn)-1:-2:IndiceStart do
    if GetUiControlPropertyName(ArgsIn(i))=='style' then
      k=i;
      return;
    end
  end
endfunction
// -----------------------------------------------------------
function InternalStyleName=GetInternalStyleName(ExternalStyleName)
  TableInternalStyle = ["button" ..
                        "checkbutton" ..
                        "checkbutton" ..
                        "entry" ..
                        "label" ..
                        "scale" ..
                        "frame" ..
                        "scrolllistbox" ..
                        "popupmenu"];
                        
  TableExternalStyle = ["pushbutton" ..
                        "radiobutton" ..
                        "checkbox" ..
                        "edit" ..
                        "text" ..
                        "slider" ..
                        "frame" ..
                        "listbox" ..
                        "popupmenu"];
  NBRStyle=9;
  InternalStyleName='';
  for i=1:NBRStyle do
    if (ExternalStyleName==TableExternalStyle(i)) then
      InternalStyleName=TableInternalStyle(i);
      return;
    end
  end
endfunction
// -----------------------------------------------------------
