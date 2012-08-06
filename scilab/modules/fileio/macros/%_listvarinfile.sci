// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001  - INRIA - Serge Steer
// Copyright (C) Enrico Segre
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// set of functions for getting information about variables saved in a Scilab 
//  data file

function varargout=%_listvarinfile(fil)
  lhs=argn(1)
  u=mopen(fil,"rb")
  typenames=[  // in vector form, in order to cover the first 17
      "constant";
      "polynomial";
      "???"
      "boolean";
      "sparse";
      "boolean sparse";
      "Matlab sparse";
      "integer"
      "graphic handle"; //not yet done
      "string";
      "function";
      "???"
      "function";
      "library";
      "list"
      "tlist"
      "mlist"]


  nams=[]
  dims=list()
  typs=[];vols=[]
  
  if lhs==1 then
  //write the display header
    write(%io(2),part("Name",1:25)+ part("Type",1:15)+part("Size",1:16)+..
    	     part("Bytes",1:10))
    write(%io(2),"---------------------------------------------------------------")
  end
  
  while %t
    w=mget(6,"il",u)
    if size(w,"*")<6|meof(u)<>0 then break,end
    nam=code2var(w);

    [typ,dim,vol]=listnextvar(u)
    if typ<=17 then
      typn=typenames(typ)
    elseif typ==128 then
      typn="pointer";
    elseif typ==129 then
      typn="size implicit";
    elseif typ==130 then
      typn="primitive";
    else
       error(msprintf(gettext("%s: Wrong variable type (%s) found in ''%s''. File may be wrong or corrupted.\n"), "listvarinfile", string(typ), fil))
    end

    if lhs==1 then
      if typ <>128 & typ <> 130 then
        write(%io(2),part(nam,1:25)+part(typn,1:15)+..
      	   part(strcat(string(dim)," by "),1:16)+part(string(vol),1:10))
      else //types 9, 128 and 130 are known in scilab, but their API is unpublished
        write(%io(2),part(nam,1:25)+part(typn,1:15)+"unknown         unknown")
        warning(".... unknown structure, I''m forced to stop here (bug 2387)")
      end
    end
    typs=[typs;typ]
    dims($+1)=dim
    nams=[nams;nam]
    vols=[vols;vol]
    if dim==-1 | vol==-1 then break; end // this for types with unknown API
  end
  mclose(u)
  
  varargout=list(nams,typs,dims,vols)
endfunction

/////////////////////////////////////////////////

function [typ,dim,vol]=listnextvar(u)
  typ=mget(1,"il",u) //The variable type
  dim=-1;vol=-1; //flags to break the caller loop in unknown cases
  select typ
  case 1 then //matrix of numbers
    w=mget(3,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);dim=[m,n]
    N=m*n*(it+1)
    mseek(8*N,u,"cur") //skip the values
    //A=mget(N,"dl",u)
    vol=4*4+8*N
  case 2 then //polynomial
    w=mget(3,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);dim=[m,n]
    mseek(4*4,u,"cur") //skeep the formal variable name
    w=mget(m*n+1,"il",u) //get the pointer table
    //I=mget(m*n+1,"il",u)
    N=(w(m*n+1)-1)*(it+1)
    mseek(8*N,u,"cur") //skip the values
    //A=mget(N,"dl",u)
    vol=(4+4+(m*n+1))*4+8*N
  case 4 then //boolean
    w=mget(2,"il",u) //header of the variable 
    m=w(1);n=w(2);dim=[m,n]
    N=m*n
    mseek(4*N,u,"cur") // skip the values
    //A=mget(N,"il",u)
    vol=(4+N)*4
  case 5 then //sparse
    w=mget(4,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);nel=w(4);dim=[m,n]
    mseek(4*(m+nel),u,"cur")// skip the index
    mseek(8*nel*(it+1),u,"cur") // skip the non zero values
    //I=mget(((m+nel),"il",u)
    //A=mget((nel*(it+1)),"dl",u)
    vol=(5+m+nel)*4+8*nel*(it+1)
  case 6 then //boolean sparse
    w=mget(4,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);nel=w(4);dim=[m,n]
    mseek(4*(m+nel),u,"cur")// skip the index
    //I=mget((m+nel),"il",u)
    vol=(5+m+nel)*4
  case 8 then //integer
    w=mget(3,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);dim=[m,n]
    select it
    case 4 then 
      mseek(m*n*4,u,"cur")
      //A=mget((m*n,"il",u)
      vol=4*4+m*n*4
    case 2 then 
      mseek(m*n*2,u,"cur")
      //A=mget((m*n,"sl",u)
      vol=4*4+m*n*2
    case 1 then 
      mseek(m*n*1,u,"cur")
      //A=mget((m*n,"c",u)
      vol=4*4+m*n
    case 14 then 
      mseek(m*n*4,u,"cur")
      //A=mget((m*n,"uil",u)
      vol=4*4+m*n*4
    case 12 then 
      mseek(m*n*2,u,"cur")
      //A=mget((m*n,"usl",u)
      vol=4*4+m*n*2
    case 11 then 
      mseek(m*n*1,u,"cur")
      //A=mget((m*n,"uc",u)
      vol=4*4+m*n
    end
  case 9 then //graphic handle: API?? 
        [dim, vol] = getMatrixHandleInfo(u);
  case 10 then //string
    w=mget(3,"il",u)
    m=w(1);n=w(2);it=w(3);dim=[m,n]
    w=mget(m*n+1,"il",u) //get the pointer table
    N=w($)-1 //total number of chars
    mseek(4*N,u,"cur") //skip the values, N characters coded in N integers
    vol=(4*m*n+1)*4+4*N
  case 11 then // function (not compiled)
    dim=[]
    nout=mget(1,"il",u)
    mseek(nout*6*4,u,"cur") //skip lhs names, nout names coded over 6 integers
    nin=mget(1,"il",u)
    mseek(nin*6*4,u,"cur") //skip rhs names, nin names coded over 6 integers
    N=mget(1,"il",u) //code length
    mseek(N*4,u,"cur") //skip code (N characters coded in N integers)
    vol=(3+(nin+nout)*6+N)*4
  case 13 then // compiled function 
    dim=[]
    nout=mget(1,"il",u)
    mseek(nout*6*4,u,"cur") //skip lhs names, nout names coded over 6 integers
    nin=mget(1,"il",u)
    mseek(nin*6*4,u,"cur") //skip rhs names, nin names coded over 6 integers
    N=mget(1,"il",u) //code length
    mseek(N*4,u,"cur") //skip pseudo-code N integers
    vol=(3+(nin+nout)*6+N)*4
  case 14 then //library
    np=mget(1,"il",u) //path length
    mseek(np*4,u,"cur") //skeep the path
    nclas=29
    nn=mget(1,"il",u) //number of names
    dim=nn
    mseek((nclas+1)*4,u,"cur") //skeep the class pointers
    mseek(nn*6*4,u,"cur") //skip the names
    vol=(2+np+1+nclas+1+nn*6)*4
  case 15 then //list
    [dim, vol] = getListInfo(u);
  case 16 then //tlist
    [dim, vol] = getListInfo(u);
  case 17 then //mlist
    [dim, vol] = getListInfo(u);
  case 128 then //pointer: API?? 
    
  case 129 then //size implicit index (same as polynomial)
    w=mget(3,"il",u) //header of the variable 
    m=w(1);n=w(2);it=w(3);dim=[m,n]
    mseek(4*4,u,"cur") //skeep the formal variable name
    w=mget(m*n+1,"il",u) //get the pointer table
    //I=mget(m*n+1,"il",u)
    N=(w(m*n+1)-1)*(it+1)
    mseek(8*N,u,"cur") //skip the values
    //A=mget(N,"dl",u)
    vol=(4+4+(m*n+1))*4+8*N
  case 130 then //scilab primitive: API?? 
    
  else  //unknown type, or file content plainly wrong

  end
endfunction

//////////////////////////////////////////////////////////
function [items, totalSize] = getListInfo(file_descriptor)
    items = mget(1, "il", file_descriptor);//read item count
    offset = mget(1 + items, "il", file_descriptor);//read offset between vars
    totalSize = (2 + (items + 1)) * 4;
    for i = 1:items
        //bypass Undefined item
        if offset(i) <> offset(i + 1) then
            [t, d, v] = listnextvar(file_descriptor);
            totalSize = totalSize + v;
        end
    end
endfunction

//////////////////////////////////////////////////////////
function [hsize, totalSize] = getMatrixHandleInfo(fd)
    totalSize = 4 + 4;
    version=mget(4, "uc", fd);
    if is_higher_than([3 1 0 1]) then // case 3 1 0 2 and after
        hsize = mget(2, "uc", fd)
        totalSize = totalSize + 2;
        for i=1:hsize(1)
            for j=1:hsize(2)
                totalSize = getSingleHandleInfo(fd, totalSize)
            end
        end
    else
        // a single handle only can be loaded before 3 1 0 2
        hsize = [1,1];
        totalSize = getSingleHandleInfo(fd, totalSize);
    end

endfunction

//////////////////////////////////////////////////////////
function totalSize = getSingleHandleInfo(fd, totalSize)
    [totalSize, handleType] = readString(fd, totalSize); // Type

    select handleType
        case "Figure"
            totalSize = seekBool(fd, totalSize); // Visible
            totalSize = seekShortCount(fd, 2, totalSize); //figure_position
            totalSize = seekShortCount(fd, 2, totalSize); //figure_size
            totalSize = seekShortCount(fd, 2, totalSize); //axes_size
            if is_higher_than( [4 1 2 0] ) then
                totalSize = seekShortCount(fd, 2, totalSize); //viewport
                if is_higher_than([5 4 0 0]) then
                    totalSize = seekStringInt(fd, totalSize); // info_message
                else
                    totalSize = seekString(fd, totalSize); // info_message
                end
                totalSize = seekString(fd, totalSize); // tag
            end

            totalSize = seekBool(fd, totalSize); // auto_resize
            if is_higher_than([5 4 0 0]) then
                totalSize = seekStringInt(fd, totalSize); // figure_name
            else
                totalSize = seekString(fd, totalSize); // figure_name
            end
            totalSize = seekShort(fd, totalSize); //figure_id
            totalSize = seekMatrix(fd, totalSize); // color_map
            totalSize = seekBool(fd, totalSize); // pixmap
            totalSize = seekString(fd, totalSize); // pixel_drawing_mode
            if (is_higher_than([5 1 0 0])) then
                totalSize = seekString(fd, totalSize); // anti_aliasing
            end
            totalSize = seekBool(fd, totalSize); // immediate_drawing
            totalSize = seekInt(fd, totalSize); // background
            totalSize = seekString(fd, totalSize); // rotation_style
            if ( is_higher_than([4 1 2 0]) ) then
                totalSize = seekString(fd, totalSize); // event_handler
                totalSize = seekString(fd, totalSize); // event_handler_enable
            end
            if ( is_higher_than([5 2 0 0]) ) then // Added in 5.4.0 version
                totalSize = seekString(fd, totalSize); // resizefcn
                totalSize = seekString(fd, totalSize); // closerequestfcn
            end

            [totalSize, axesCount] = readInt(fd, totalSize); // children
            for i = 1 : axesCount
                totalSize = getSingleHandleInfo(fd, totalSize);
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Axes"
            totalSize = seekBool(fd, totalSize); // visible
            if and(version==[3 0 0 0]) then
                totalSize = seekBool(fd, totalSize); // axes_visible
            else
                totalSize = seekBoolMatrix(fd, totalSize); // axes_visible
            end

            if is_higher_than( [3 1 0 1] ) then
                totalSize = seekBoolMatrix(fd, totalSize); // axes_reverse
            end

            totalSize = seekIntMatrix(fd, totalSize); // grid
        	if (is_higher_than([5 0 3 0])) then
                totalSize = seekString(fd, totalSize); // grid_position
            end
            totalSize = seekString(fd, totalSize); // x_location
            totalSize = seekString(fd, totalSize); // y_location
            [totalSize, view] = readStringCount(fd, 2, totalSize); // view

            //title
            totalSize = seekBool(fd, totalSize); // visible
            if is_higher_than( [4 1 2 0] ) then
                totalSize = seekStringMatrix(fd, totalSize); // text
            else
                totalSize = seekString(fd, totalSize); // text
            end

            if is_higher_than([4 1 2 0]) then
                totalSize = seekInt(fd, totalSize); // font_foreground
                totalSize = seekBool(fd, totalSize); // fractional_font
            end

            totalSize = seekInt(fd, totalSize); // foreground
            if is_higher_than([3 1 0 0]) then
                totalSize = seekInt(fd, totalSize); // background
                totalSize = seekBool(fd, totalSize); // fill_mode
            end

            totalSize = seekBool(fd, totalSize); // font_style
            totalSize = seekBool(fd, totalSize); // font_size

            if is_higher_than([3 1 0 0]) then
                totalSize = seekBool(fd, totalSize); // auto_rotation
                totalSize = seekDouble(fd, totalSize); // font_angle
                totalSize = seekBool(fd, totalSize); // auto_position
                totalSize = seekDouble(fd, totalSize); // position 1 
                totalSize = seekDouble(fd, totalSize); // position 2
            end

            //x_label
            totalSize = seekBool(fd, totalSize); // visible
            if is_higher_than( [4 1 2 0] ) then
                totalSize = seekStringMatrix(fd, totalSize); // text
            else
                totalSize = seekString(fd, totalSize); // text
            end

            if is_higher_than([4 1 2 0]) then
                totalSize = seekInt(fd, totalSize); // font_foreground
                totalSize = seekBool(fd, totalSize); // fractional_font
            end

            totalSize = seekInt(fd, totalSize); // foreground
            if is_higher_than([3 0 0 0]) then
                totalSize = seekInt(fd, totalSize); // background
                totalSize = seekBool(fd, totalSize); // fill_mode
            end

            totalSize = seekBool(fd, totalSize); // font_style
            totalSize = seekBool(fd, totalSize); // font_size

            if is_higher_than([3 0 0 0]) then
                totalSize = seekBool(fd, totalSize); // auto_rotation
                totalSize = seekDouble(fd, totalSize); // font_angle
                totalSize = seekBool(fd, totalSize); // auto_position
                totalSize = seekDouble(fd, totalSize); // position 1 
                totalSize = seekDouble(fd, totalSize); // position 2
            end

            //y_label
            totalSize = seekBool(fd, totalSize); // visible
            if is_higher_than( [4 1 2 0] ) then
                totalSize = seekStringMatrix(fd, totalSize); // text
            else
                totalSize = seekString(fd, totalSize); // text
            end

            if is_higher_than([4 1 2 0]) then
                totalSize = seekInt(fd, totalSize); // font_foreground
                totalSize = seekBool(fd, totalSize); // fractional_font
            end

            totalSize = seekInt(fd, totalSize); // foreground
            if is_higher_than([3 0 0 0]) then
                totalSize = seekInt(fd, totalSize); // background
                totalSize = seekBool(fd, totalSize); // fill_mode
            end

            totalSize = seekBool(fd, totalSize); // font_style
            totalSize = seekBool(fd, totalSize); // font_size

            if is_higher_than([3 0 0 0]) then
                totalSize = seekBool(fd, totalSize); // auto_rotation
                totalSize = seekDouble(fd, totalSize); // font_angle
                totalSize = seekBool(fd, totalSize); // auto_position
                totalSize = seekDouble(fd, totalSize); // position 1 
                totalSize = seekDouble(fd, totalSize); // position 2
            end

            if view == "3d" then
                //z_label
                totalSize = seekBool(fd, totalSize); // visible
                if is_higher_than( [4 1 2 0] ) then
                    totalSize = seekStringMatrix(fd, totalSize); // text
                else
                    totalSize = seekString(fd, totalSize); // text
                end

                if is_higher_than([4 1 2 0]) then
                    totalSize = seekInt(fd, totalSize); // font_foreground
                    totalSize = seekBool(fd, totalSize); // fractional_font
                end

                totalSize = seekInt(fd, totalSize); // foreground
                if is_higher_than([3 0 0 0]) then
                    totalSize = seekInt(fd, totalSize); // background
                    totalSize = seekBool(fd, totalSize); // fill_mode
                end

                totalSize = seekBool(fd, totalSize); // font_style
                totalSize = seekBool(fd, totalSize); // font_size

                if is_higher_than([3 0 0 0]) then
                    totalSize = seekBool(fd, totalSize); // auto_rotation
                    totalSize = seekDouble(fd, totalSize); // font_angle
                    totalSize = seekBool(fd, totalSize); // auto_position
                    totalSize = seekDouble(fd, totalSize); // position 1 
                    totalSize = seekDouble(fd, totalSize); // position 2
                end
            end

            if is_higher_than([3 0 0 0]) then
                totalSize = seekBoolMatrix(fd, totalSize); // auto_ticks

                //x_ticks
                [totalSize, ticksX] = readShort(fd, totalSize); // ticks count
                if ticksX > 0 then
                    totalSize = seekMatrixCount(fd, ticksX,  totalSize); // locations
                    [totalSize, labels] = readBoolCount(fd, ticksX, totalSize); // label
                    for i = 1:ticksX
                        totalSize = seekStringCount(fd, labels(i), totalSize);
                    end
                end

                //y_ticks
                [totalSize, ticksY] = readShort(fd, totalSize); // ticks count
                if ticksY > 0 then
                    totalSize = seekMatrixCount(fd, ticksY,  totalSize); // locations
                    [totalSize, labels] = readBoolCount(fd, ticksY, totalSize); // label
                    for i = 1:ticksY
                        totalSize = seekStringCount(fd, labels(i), totalSize);
                    end
                end

                //z_ticks
                [totalSize, ticksZ] = readShort(fd, totalSize); // ticks count
                if ticksZ > 0 then
                    totalSize = seekMatrixCount(fd, ticksZ,  totalSize); // locations
                    [totalSize, labels] = readBoolCount(fd, ticksZ, totalSize); // label
                    for i = 1:ticksZ
                        totalSize = seekStringCount(fd, labels(i), totalSize);
                    end
                end
            end
        
            if is_higher_than([4 1 2 0]) then
                totalSize = seekString(fd, totalSize); // box
                totalSize = seekBool(fd, totalSize); // filled
            else
                totalSize = seekBool(fd, totalSize); // box
            end

            totalSize = seekString(fd, totalSize); // sub_ticks

            if ~(is_higher_than([3 1 0 1]) ) then
                totalSize = seekDouble(fd, totalSize); // ticks_color 
            end

            totalSize = seekBool(fd, totalSize); // font_style
            totalSize = seekBool(fd, totalSize); // font_size
            totalSize = seekInt(fd, totalSize); // font_color

            if is_higher_than([4 1 2 0]) then
                totalSize = seekBool(fd, totalSize); // fractional_font
            end

            totalSize = seekBool(fd, totalSize); // isoview
            totalSize = seekBool(fd, totalSize); // cube_scaling
            totalSize = seekDouble(fd, totalSize); // rotation_angles 1
            totalSize = seekDouble(fd, totalSize); // rotation_angles 2

            if is_higher_than([3 0 0 0]) then
                totalSize = seekStringCount(fd, 3, totalSize); // log_flags
            else
                totalSize = seekStringCount(fd, 2, totalSize); // log_flags
            end
            
            totalSize = seekBool(fd, totalSize); // tight_limits
            [totalSize, bounds] = readBool(fd, totalSize); // data_bounds count
            totalSize = seekDoubleCount(fd, bounds, totalSize); // data_bounds
        
            if is_higher_than([3 0 0 0]) then
                [totalSize, zooms] = readBool(fd, totalSize); // zoom_box_size
                if zooms <> 0 then
                    totalSize = seekDoubleCount(fd, zooms, totalSize); // zoom_box
                end
            end
            
            if is_higher_than([3 1 0 1]) then
                totalSize = seekDoubleCount(fd, 4, totalSize); // margins
            end

            totalSize = seekDoubleCount(fd, 4, totalSize); // axes_bounds
            totalSize = seekBool(fd, totalSize); // auto_clear
            totalSize = seekBool(fd, totalSize); // auto_scale
            
            if is_higher_than([4 1 2 0] ) then
                totalSize = seekInt(fd, totalSize); // hidden_axis_color
                totalSize = seekString(fd, totalSize); // arc_drawing_method
            end
 
            totalSize = seekInt(fd, totalSize); // hidden_color
            totalSize = seekBool(fd, totalSize); // line_mode
            totalSize = seekBool(fd, totalSize); // line_style
            totalSize = seekShort(fd, totalSize); // thickness
            totalSize = seekBool(fd, totalSize); // mark_mode
            totalSize = seekBool(fd, totalSize); // mark_style
            totalSize = seekBool(fd, totalSize); // mark_size

            if is_higher_than([3 0 0 0]) then
                totalSize = seekBool(fd, totalSize); // mark_size_unit
                totalSize = seekInt(fd, totalSize); // mark_foreground
                totalSize = seekInt(fd, totalSize); // mark_background
            end

            totalSize = seekInt(fd, totalSize); // foreground
            totalSize = seekInt(fd, totalSize); // background
            [totalSize, clip] = readString(fd, totalSize); // clip_state
            if clip == "on" then
                totalSize = seekDoubleCount(fd, 4, totalSize); // clip_box
            end
    
            global parentAxesView;
            parentAxesView = view;
            [totalSize, axesCount] = readInt(fd, totalSize); // children
            for i = 1 : axesCount
                totalSize = getSingleHandleInfo(fd, totalSize);
            end
            clearglobal parentAxesView;
            totalSize = seekUserdata(fd, totalSize); // Userdata
           
        case "Polyline"
            totalSize = seekBool(fd, totalSize); // visible
            [totalSize, dataCount] = readIntCount(fd, 2, totalSize); // data count
            totalSize = seekDoubleCount(fd, prod(dataCount), totalSize); // data
            if is_higher_than([3 1 0 0]) then
                totalSize = seekBool(fd, totalSize); // closed
            end    

            totalSize = seekBool(fd, totalSize); // line_mode
            if is_higher_than([3 1 0 0]) then
                totalSize = seekBool(fd, totalSize); // fill_mode
            end    
                
            totalSize = seekBool(fd, totalSize); // line_style
            totalSize = seekShort(fd, totalSize); // thickness
            if is_higher_than([3 1 0 1]) then
                totalSize = seekShort(fd, totalSize); // arrow_size_factor
            end

            totalSize = seekBool(fd, totalSize); // polyline_style

            if is_higher_than([3 1 0 1] ) then
                [totalSize, count] = readShort(fd, totalSize); // interp_color_vector size
                totalSize = seekDoubleCount(fd, count, totalSize); // interp_color_vector
                totalSize = seekBool(fd, totalSize); // interp_color_mode
            end

            totalSize = seekBool(fd, totalSize); // mark_mode
            totalSize = seekBool(fd, totalSize); // mark_style
            totalSize = seekBool(fd, totalSize); // mark_size

            if is_higher_than([3 0 0 0]) then
                totalSize = seekBool(fd, totalSize); // mark_size_unit
            end

            totalSize = seekInt(fd, totalSize); // foreground
            if is_higher_than([3 1 0 0]) then
                totalSize = seekInt(fd, totalSize); // background
            end

            if is_higher_than([3 0 0 0]) then
                totalSize = seekInt(fd, totalSize); // mark_foreground
                totalSize = seekInt(fd, totalSize); // mark_background
            end

            if is_higher_than([3 1 0 0]) then
                [totalSize, count] = readShort(fd, totalSize); // x_shift count
                totalSize = seekDoubleCount(fd, count, totalSize); // x_shift
                [totalSize, count] = readShort(fd, totalSize); // y_shift count
                totalSize = seekDoubleCount(fd, count, totalSize); // y_shift
                [totalSize, count] = readShort(fd, totalSize); // z_shift count
                totalSize = seekDoubleCount(fd, count, totalSize); // z_shift
            end

            if is_higher_than([3 1 0 1]) then
                totalSize = seekDouble(fd, totalSize); // bar_width
            end

            [totalSize, state] = readString(fd, totalSize); // clip_state
            if state == "on" then
                totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Plot3d"
            totalSize = seekBool(fd, totalSize); // visible
            totalSize = seekBool(fd, totalSize); // surface_mode
            totalSize = seekInt(fd, totalSize); // foreground
            totalSize = seekShort(fd, totalSize); // thickness
            totalSize = seekBool(fd, totalSize); // mark_mode
            totalSize = seekBool(fd, totalSize); // mark_style
            totalSize = seekBool(fd, totalSize); // mark_size

            if is_higher_than([3 0 0 0]) then
                totalSize = seekBool(fd, totalSize); // mark_size_unit
                totalSize = seekInt(fd, totalSize); // mark_foreground
                totalSize = seekInt(fd, totalSize); // mark_background
            end

            if is_higher_than([5 1 1 0]) then
                totalSize = seekInt(fd, totalSize); // color_mode
                [totalSize, flag] = readInt(fd, totalSize); // color_flag
            else
                totalSize = seekBool(fd, totalSize); // color_mode
                [totalSize, flag] = readBool(fd, totalSize); // color_flag
            end

            //data
            totalSize = seekDoubleMatrix(fd, totalSize); // x
            totalSize = seekDoubleMatrix(fd, totalSize); // y
            totalSize = seekDoubleMatrix(fd, totalSize); // z

            if or(flag == [2,5]) then
                [totalSize, count] = readIntCount(fd, 2, totalSize); // color size
                totalSize = seekIntCount(fd, prod(count), totalSize); // color
            end

            totalSize = seekInt(fd, totalSize); // hidden_color
           
            if is_higher_than([4 1 2 0])
                [totalSize, state] = readString(fd, totalSize); // clip_state
                if state == "on" then
                    totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
                end
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Fac3d"
            totalSize = seekBool(fd, totalSize); // visible
            totalSize = seekBool(fd, totalSize); // surface_mode
            totalSize = seekInt(fd, totalSize); // foreground
            totalSize = seekShort(fd, totalSize); // thickness
            totalSize = seekBool(fd, totalSize); // mark_mode
            totalSize = seekBool(fd, totalSize); // mark_style
            totalSize = seekBool(fd, totalSize); // mark_size
            if is_higher_than([3 0 0 0]) then
                totalSize = seekBool(fd, totalSize); // mark_size_unit
                totalSize = seekInt(fd, totalSize); // mark_foreground
                totalSize = seekInt(fd, totalSize); // mark_background
            end

            if is_higher_than([5 1 1 0]) then
                totalSize = seekInt(fd, totalSize); // color_mode
                [totalSize, flag] = readInt(fd, totalSize); // color_flag
            else
                totalSize = seekBool(fd, totalSize); // color_mode
                [totalSize, flag] = readBool(fd, totalSize); // color_flag
            end

            //data
            totalSize = seekDoubleMatrix(fd, totalSize); // x
            totalSize = seekDoubleMatrix(fd, totalSize); // y
            totalSize = seekDoubleMatrix(fd, totalSize); // z

            if is_higher_than([3 1 0 1]) & flag >= 2 then
                [totalSize, count] = readIntCount(fd, 2, totalSize); // color size
                totalSize = seekIntCount(fd, prod(count), totalSize); // color
                totalSize = seekBool(fd, totalSize); // cdata_mapping
            elseif or(flag==[2 5]) then
                [totalSize, count] = readIntCount(fd, 2, totalSize); // color size
                totalSize = seekIntCount(fd, prod(count), totalSize); // color
            end

            totalSize = seekInt(fd, totalSize); // color_mode

            if is_higher_than([4 1 2 0])
                [totalSize, state] = readString(fd, totalSize); // clip_state
                if state == "on" then
                    totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
                end
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Compound"
            [totalSize, count] = readInt(fd, totalSize); // children count
            for i = 1 : count
                totalSize = getSingleHandleInfo(fd, totalSize); // children
            end

            if is_higher_than([3 1 0 1]) then
                totalSize = readBool(fd, totalSize); // visible
            end
            
            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Agregation"
            [totalSize, count] = readInt(fd, totalSize); // children count
            for i = 1 : count
                totalSize = getSingleHandleInfo(fd, totalSize); // children
            end

            if is_higher_than([3 1 0 1]) then
                totalSize = readBool(fd, totalSize); // visible
            end
            
            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Rectangle"
            totalSize = seekBool(fd, totalSize); // visible
            totalSize = seekShort(fd, totalSize); // thickness
            totalSize = seekBool(fd, totalSize); // mark_mode
            totalSize = seekBool(fd, totalSize); // mark_style
            totalSize = seekBool(fd, totalSize); // mark_size

            if is_higher_than([3 0 0 0]) then
                totalSize = seekBool(fd, totalSize); // mark_size_unit
            end

            totalSize = seekInt(fd, totalSize); // mark_foreground
            totalSize = seekInt(fd, totalSize); // mark_background
            totalSize = seekBool(fd, totalSize); // line_mode
            totalSize = seekBool(fd, totalSize); // line_style
            totalSize = seekBool(fd, totalSize); // fill_mode
            totalSize = seekInt(fd, totalSize); // foreground

            if is_higher_than([3 1 0 1]) then
                totalSize = seekInt(fd, totalSize); // background
            end

            if (is_higher_than([5 0 3 0])) then
                totalSize = seekMatrix(fd, totalSize); // data
            else
                global parentAxesView;
                if parentAxesView == [] | parentAxesView == "2d" then
                    totalSize = seekDoubleCount(fd, 4, totalSize); //data
                else
                    totalSize = seekDoubleCount(fd, 5, totalSize); //data
                end
            end

            [totalSize, state] = readString(fd, totalSize); // clip_state
            if state == "on" then
                totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Arc"
            totalSize = seekBool(fd, totalSize); // visible
            totalSize = seekShort(fd, totalSize); // thickness
            totalSize = seekBool(fd, totalSize); // line_style

            if is_higher_than([3 1 0 1])
                totalSize = seekBool(fd, totalSize); // line_mode
            end

            totalSize = seekBool(fd, totalSize); // fill_mode
            totalSize = seekInt(fd, totalSize); // foreground

            if is_higher_than([3 1 0 1]) then
                totalSize = seekInt(fd, totalSize); // background
            end

            if is_higher_than([5 0 3 0]) then
                totalSize = seekMatrix(fd, totalSize); // data
            else
                global parentAxesView;
                if parentAxesView == [] | parentAxesView == "2d" then
                    totalSize = seekDoubleCount(fd, 6, totalSize); //data
                else
                    totalSize = seekDoubleCount(fd, 7, totalSize); //data
                end
            end

            if is_higher_than([4 1 2 0]) then
                totalSize = seekString(fd, totalSize); // data_mapping
            end

            [totalSize, state] = readString(fd, totalSize); // clip_state
            if state == "on" then
                totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Champ"
            totalSize = seekBool(fd, totalSize); // visible
            totalSize = seekDoubleMatrix(fd, totalSize); // x
            totalSize = seekDoubleMatrix(fd, totalSize); // y
            totalSize = seekDoubleMatrix(fd, totalSize); // fx
            totalSize = seekDoubleMatrix(fd, totalSize); // fy
            totalSize = seekBool(fd, totalSize); // line_style
            totalSize = seekShort(fd, totalSize); // thickness
            totalSize = seekBool(fd, totalSize); // colored
            totalSize = seekDouble(fd, totalSize); // arrow_size

            [totalSize, state] = readString(fd, totalSize); // clip_state
            if state == "on" then
                totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Segs"
            totalSize = seekBool(fd, totalSize); // visible
            totalSize = seekDoubleMatrix(fd, totalSize); // data
            totalSize = seekBool(fd, totalSize); // line_mode
            totalSize = seekBool(fd, totalSize); // line_style
            totalSize = seekShort(fd, totalSize); // thickness
            totalSize = seekDouble(fd, totalSize); // arrow_size
            totalSize = seekIntInt(fd, totalSize); // segs_color
            totalSize = seekBool(fd, totalSize); // mark_mode
            totalSize = seekBool(fd, totalSize); // mark_style
            totalSize = seekBool(fd, totalSize); // mark_size

            if is_higher_than([3 0 0 0]) then
                totalSize = seekBool(fd, totalSize); // mark_size_unit
            end

            totalSize = seekInt(fd, totalSize); // mark_foreground
            totalSize = seekInt(fd, totalSize); // mark_background

            [totalSize, state] = readString(fd, totalSize); // clip_state
            if state == "on" then
                totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Grayplot"
            totalSize = seekBool(fd, totalSize); // visible
            if is_higher_than([3 0 0 0]) then
                totalSize = seekDoubleMatrix(fd, totalSize); // x
                totalSize = seekDoubleMatrix(fd, totalSize); // y
                totalSize = seekDoubleMatrix(fd, totalSize); // z
            else
                totalSize = seekDoubleMatrix(fd, totalSize); // x,y,z
            end

            totalSize = seekString(fd, totalSize); // data_mapping
            [totalSize, state] = readString(fd, totalSize); // clip_state
            if state == "on" then
                totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Matplot"
            totalSize = seekBool(fd, totalSize); // visible
            totalSize = seekDoubleMatrix(fd, totalSize); // data

            [totalSize, state] = readString(fd, totalSize); // clip_state
            if state == "on" then
                totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Fec"
            totalSize = seekBool(fd, totalSize); // visible
            totalSize = seekDoubleMatrix(fd, totalSize); // data
            totalSize = seekDoubleMatrix(fd, totalSize); // triangles
            totalSize = seekDoubleCount(fd, 2, totalSize); // z_bounds
            if is_higher_than( [5 0 3 0] ) then
                totalSize = seekDoubleCount(fd, 2, totalSize); // color_range
                totalSize = seekDoubleCount(fd, 2, totalSize); // outside_colors
                totalSize = seekBool(fd, totalSize); // line_mode
                totalSize = seekInt(fd, totalSize); // foreground
            end

            [totalSize, state] = readString(fd, totalSize); // clip_state
            if state == "on" then
                totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Legend"
            if is_higher_than( [5 0 0 0] ) then
                totalSize = seekBool(fd, totalSize); // visible
                totalSize = seekStringInt(fd, totalSize); // text
                totalSize = seekBool(fd, totalSize); // font_style
                totalSize = seekBool(fd, totalSize); // font_size
                totalSize = seekInt(fd, totalSize); // font_color
                totalSize = seekBool(fd, totalSize); // fractional_font
                [totalSize, count] = readBool(fd, totalSize); // legends size
                for i = 1 : count
                    totalSize = seekIntInt(fd, totalSize); // legends
                end

                totalSize = seekString(fd, totalSize); // legend_location
                totalSize = seekDoubleCount(fd, 2, totalSize); // position
                totalSize = seekBool(fd, totalSize); // line_mode
                totalSize = seekShort(fd, totalSize); // thickness
                totalSize = seekInt(fd, totalSize); // foreground
                totalSize = seekBool(fd, totalSize); // fill_mode
                totalSize = seekInt(fd, totalSize); // background

                [totalSize, state] = readString(fd, totalSize); // clip_state
                if state == "on" then
                    totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
                end

                totalSize = seekUserdata(fd, totalSize); // Userdata
            else
                totalSize = seekBool(fd, totalSize); // visible
                totalSize = seekBool(fd, totalSize); // line_mode
                totalSize = seekBool(fd, totalSize); // mark_mode
                totalSize = seekInt(fd, totalSize); // mark_foreground
                totalSize = seekInt(fd, totalSize); // mark_background
                totalSize = seekStringInt(fd, totalSize); // text
                totalSize = seekInt(fd, totalSize); // foreground
                totalSize = seekBool(fd, totalSize); // font_style
                totalSize = seekBool(fd, totalSize); // font_size
                if is_higher_than( [4 1 2 0] ) then
                    totalSize = seekBool(fd, totalSize); // font_size
                end

                [totalSize, state] = readString(fd, totalSize); // clip_state
                if state == "on" then
                    totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
                end
            end

        case "Text"
            totalSize = seekBool(fd, totalSize); // visible
            if is_higher_than([4 1 2 0]) then
                totalSize = seekStringMatrix(fd, totalSize);
            else
                totalSize = seekStringInt(fd, totalSize);
            end

            [totalSize, count] = readBoolCount(fd, 2, totalSize); // data size
            totalSize = seekDoubleCount(fd, prod(count), totalSize); // data
            totalSize = seekDoubleCount(fd, 2, totalSize); // text_box
            totalSize = seekString(fd, totalSize); // text_box_mode
            totalSize = seekInt(fd, totalSize); // foreground
            totalSize = seekBool(fd, totalSize); // font_style
            totalSize = seekBool(fd, totalSize); // font_size
            totalSize = seekDouble(fd, totalSize); // font_angle

            if is_higher_than([3 1 0 1]) then
                totalSize = seekBool(fd, totalSize); // box
                totalSize = seekBool(fd, totalSize); // line_mode
                totalSize = seekBool(fd, totalSize); // fill_mode
                totalSize = seekInt(fd, totalSize); // font_foreground
                totalSize = seekInt(fd, totalSize); // background
            end

            if is_higher_than( [4 1 2 0] ) then
                totalSize = seekString(fd, totalSize); // alignment
                totalSize = seekBool(fd, totalSize); // fractional_font
            end

            [totalSize, state] = readString(fd, totalSize); // clip_state
            if state == "on" then
                totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
            end

            totalSize = seekUserdata(fd, totalSize); // Userdata

        case "Axis"
            if is_higher_than([3 1 0 0]) then
                totalSize = seekBool(fd, totalSize); // visible
                totalSize = seekBoolInt(fd, totalSize); // tics_direction
                totalSize = seekMatrix(fd, totalSize); // xtics_coord
                totalSize = seekMatrix(fd, totalSize); // ytics_coord
                totalSize = seekInt(fd, totalSize); // tics_color
                totalSize = seekBool(fd, totalSize); // tics_segment
                totalSize = seekBool(fd, totalSize); // tics_style
                totalSize = seekInt(fd, totalSize); // sub_tics
                totalSize = seekStringInt(fd, totalSize); // tics_label
                totalSize = seekInt(fd, totalSize); // label_font_size
                totalSize = seekInt(fd, totalSize); // labels_font_color

                if is_higher_than( [4 1 2 0] ) then
                    totalSize = seekBool(fd, totalSize); // fractional_font
                end
                
                [totalSize, state] = readString(fd, totalSize); // clip_state
                if state == "on" then
                    totalSize = seekDoubleCount(fd, 4, totalSize); //clip_box
                end

                totalSize = seekUserdata(fd, totalSize); // Userdata
            end

        case "uimenu"
            if is_higher_than( [4 1 2 0] ) then
                totalSize = seekBool(fd, totalSize); // Enable
                totalSize = seekMatrix(fd, totalSize); // Foregroundcolor
                totalSize = seekString(fd, totalSize); // Label
                totalSize = seekBool(fd, totalSize); // Visible
                if is_higher_than([5 4 0 0]) then
                    totalSize = seekStringInt(fd, totalSize); // Callback
                else
                    totalSize = seekString(fd, totalSize); // Callback
                end
                totalSize = seekInt(fd, totalSize); // Callback Type
                totalSize = seekString(fd, totalSize); // Tag
            end

            if is_higher_than( [5 1 2 0] ) then
                totalSize = seekBool(fd, totalSize); // Checked
            end

            if is_higher_than( [5 1 1 0] ) then
                [totalSize, axesCount] = readInt(fd, totalSize); // children
                for i = 1 : axesCount
                    totalSize = getSingleHandleInfo(fd, totalSize);
                end
            end

        case "uicontextmenu"
            [totalSize, axesCount] = readInt(fd, totalSize); // children
            for i = 1 : axesCount
                totalSize = getSingleHandleInfo(fd, totalSize);
            end
        case "uicontrol"
            if is_higher_than( [4 1 2 0] ) then
                totalSize = seekString(fd, totalSize); // Style
                totalSize = seekMatrix(fd, totalSize); // BackgroundColor
                totalSize = seekBool(fd, totalSize); // Enable
                totalSize = seekString(fd, totalSize); // FontAngle
                totalSize = seekString(fd, totalSize); // FontName
                totalSize = seekDouble(fd, totalSize); // FontSize
                totalSize = seekString(fd, totalSize); // FontUnits
                totalSize = seekString(fd, totalSize); // FontWeight
                totalSize = seekMatrix(fd, totalSize); // ForegroundColor
                totalSize = seekString(fd, totalSize); // HorizontalAlignment
                totalSize = seekMatrix(fd, totalSize); // ListboxTop
                totalSize = seekDouble(fd, totalSize); // Max
                totalSize = seekDouble(fd, totalSize); // Min
                totalSize = seekMatrix(fd, totalSize); // Position
                totalSize = seekString(fd, totalSize); // Relief
                totalSize = seekMatrix(fd, totalSize); // SliderStep
                totalSize = seekStringMatrix(fd, totalSize); // String
                if is_higher_than( [5 2 0 0] ) then  // Added in 5.4.0 version
                    totalSize = seekStringMatrix(fd, totalSize); // TooltipString
                end
                totalSize = seekString(fd, totalSize); // Units
                totalSize = seekMatrix(fd, totalSize); // Value
                totalSize = seekString(fd, totalSize); // VerticalAlignment
                totalSize = seekBool(fd, totalSize); // Visible
                if is_higher_than([5 4 0 0]) then
                    totalSize = seekStringInt(fd, totalSize); // Callback
                else
                    totalSize = seekString(fd, totalSize); // Callback
                end
                totalSize = seekInt(fd, totalSize); // Callback Type
                totalSize = seekUserdata(fd, totalSize); // Userdata
                totalSize = seekString(fd, totalSize); // Tag
            end
        else
            warning("type " +typ+" unhandled");
        end
endfunction
//////////////////////////////////////////////////////////
//read 1 "il" value
function newSize = seekInt(fd, currentSize)
    data = mget(1, "il", fd);
    //disp("int : " + string(data));
    newSize = currentSize + 4;
endfunction

//////////////////////////////////////////////////////////
//read 1 "il" value
function newSize = seekIntCount(fd, len, currentSize)
    data = mget(len, "il", fd);
    //disp("int : " + string(data));
    newSize = currentSize + 4 * len;
endfunction

//////////////////////////////////////////////////////////
//read 1 "il" value
function newSize = seekIntMatrix(fd, currentSize)
    offset = mget(1, "c", fd);
    data = mget(offset, "il", fd);
    //disp("int[] : " + string(data));
    newSize = currentSize + 4 * offset + 1;
endfunction

//////////////////////////////////////////////////////////
//read 1 "il" value
function newSize = seekIntInt(fd, currentSize)
    offset = mget(1, "il", fd);
    data = mget(offset, "il", fd);
    //disp("int[] : " + string(data));
    newSize = currentSize + 4 * offset + 4;
endfunction

//////////////////////////////////////////////////////////
//read 1 "il" value
function [newSize, data] = readInt(fd, currentSize)
    data = mget(1, "il", fd);
    //disp("int : " + string(data));
    newSize = currentSize + 4;
endfunction

//////////////////////////////////////////////////////////
//read 1 "il" value
function [newSize, data] = readIntCount(fd, len, currentSize)
    data = mget(len, "il", fd);
    //disp("int : " + string(data));
    newSize = currentSize + 4 * len;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" value
function newSize = seekBool(fd, currentSize)
    data = mget(1, "c", fd);
    //disp("bool : " + string(data));
    newSize = currentSize + 1;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" value
function [newSize, data] = readBool(fd, currentSize)
    data = mget(1, "c", fd);
    //disp("bool : " + string(data));
    newSize = currentSize + 1;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" value
function newSize = seekBoolMatrix(fd, currentSize)
    offset = mget(1, "c", fd);
    data = mget(offset, "c", fd);
    //disp("bool : " + string(data));
    newSize = currentSize + offset + 1;
endfunction

function newSize = seekBoolInt(fd, currentSize)
    offset = mget(1, "il", fd);
    data = mget(offset, "c", fd);
    //disp("bool : " + string(data));
    newSize = currentSize + offset + 4;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" value
function [newSize, data] = readBoolCount(fd, len, currentSize)
    data = mget(len, "c", fd);
    //disp("bool : " + string(data));
    newSize = currentSize + len;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" value
function newSize = seekDouble(fd, currentSize)
    data = mget(1, "dl", fd);
    //disp("double : " + string(data));
    newSize = currentSize + 8;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" value
function newSize = seekDoubleCount(fd, len ,currentSize)
    data = mget(len, "dl", fd);
    //disp("bool : " + string(data));
    newSize = currentSize + len * 8;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" value
function newSize = seekShort(fd, currentSize)
    data = mget(1, "sl", fd);
    //disp("short : " + string(data));
    newSize = currentSize + 2;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" value
function [newSize, data] = readShort(fd, currentSize)
    data = mget(1, "sl", fd);
    //disp("short : " + string(data));
    newSize = currentSize + 2;
endfunction

//////////////////////////////////////////////////////////
//read 2 "c" value
function newSize = seekShortCount(fd, len, currentSize)
    data = mget(len, "sl", fd);
    //disp("short : " + string(data));
    newSize = currentSize + len * 2;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" size + size * "c" value
function newSize = seekString(fd, currentSize)
    offset = mget(1, "c", fd);
    data = mget(offset, "c", fd);
    //disp("string : " + ascii(data));
    newSize = currentSize + offset + 1;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" size + size * "c" value
function newSize = seekStringInt(fd, currentSize)
    offset = mget(1, "il", fd);
    data = mget(offset, "c", fd);
    //disp("string : " + ascii(data));
    newSize = currentSize + offset + 4;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" size + size * "c" value
function newSize = seekStringCount(fd, len, currentSize)
    data = mget(len, "c", fd);
    //disp("string : " + ascii(data));
    newSize = currentSize + len;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" size + size * "c" value
function newSize = seekStringMatrix(fd, currentSize)
    row = mget(1, "il", fd);
    col = mget(1, "il", fd);
    for i = 1 : row * col
        if is_higher_than([5 4 0 0])
            currentSize = seekStringInt(fd, currentSize); // String
        else
            currentSize = seekString(fd, currentSize); // String
        end
    end
    newSize = currentSize + 4 * 2;
endfunction

//////////////////////////////////////////////////////////
//read 1 "il" size + size * "dl" value
function newSize = seekMatrix(fd, currentSize)
    offset = mget(1, "il", fd);
    data = mget(offset, "dl", fd);
    //disp("double : " + string(data));
    newSize = currentSize + offset * 8 + 4;
endfunction

//////////////////////////////////////////////////////////
//read 1 "il" size + size * "dl" value
function newSize = seekMatrixCount(fd, len, currentSize)
    data = mget(len, "dl", fd);
    //disp("double : " + string(data));
    newSize = currentSize + len * 8;
endfunction

//////////////////////////////////////////////////////////
//read 1 "c" size + size * "c" value
function newSize = seekDoubleMatrix(fd, currentSize)
    row = mget(1, "il", fd);
    col = mget(1, "il", fd);
    currentSize = seekDoubleCount(fd, row * col, currentSize);
    newSize = currentSize + 4 * 2;
endfunction

//////////////////////////////////////////////////////////
//read user_data
function newSize = seekUserdata(fd, currentSize)
    mget(24, "c", fd);
    newSize = currentSize + 24;
    [typ,dim,vol] = listnextvar(fd); // Userdata
    newSize = newSize + vol;
endfunction

//////////////////////////////////////////////////////////
//read 1 "il" value
function [newSize, data] = readString(fd, currentSize)
    offset = mget(1, "c", fd);
    data = ascii(mget(offset, "c", fd));
    //disp("string : " + data);
    newSize = currentSize + offset + 1;
endfunction

//////////////////////////////////////////////////////////
//read 1 "il" value
function [newSize, data] = readStringCount(fd, len, currentSize)
    data = ascii(mget(len, "c", fd));
    //disp("string : " + data);
    newSize = currentSize + len;
endfunction
//////////////////////////////////////////////////////////
function v=code2var(id)
  nsiz=6
  i1=1
  for l=1:nsiz
    idl=id(l)
    for i=i1:i1+3
      k=int((idl+128)/256)
      if k<0 then k=k-1,end
      ch=idl-256*k
      if ch==40 then
        n=i-1
        v=_code2str(str)
        return
      end
      idl=k
      str(i)=ch
    end
    i1=i1+4
    v=_code2str(str)
  end
endfunction

function r=is_higher_than(v)
//check if current version is strictly higher than the given one
  r=%f
  for k=1:4
    if version(k)>v(k) then r=%t,break,end
    if version(k)<v(k) then r=%f,break,end
  end
endfunction
