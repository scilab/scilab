function SaveasInterfFunc_()
    Cmenu=[]
    ok=%f
    while ~ok then
      fname=xgetfile('*.sci')
      if fname<>emptystr() then 
	[dir,name,ext]=splitfilepath(fname)
	select ext
	case 'sci' then
	  ok=%t
	else
	  message('Only *.sci files allowed');
	  ok=%f
	end
      else
	ok=%t
      end
    end
    if fname<>emptystr() then
      scs_m.props.title(1)=name

      [ok, %wh]=getvalue(['Enter the type of Interf. Func. to create'],..
	  ['Block(0) or SuperBlock(1)'],list('vec','1'),..
	  ['1'])
      if ok then
	scs_m.props.title(1)=name
	if  %wh==0 then
	  %path=save_csuper(scs_m,dir)
	else
	  %path=save_super(scs_m,dir)
	end
	if %path<>[] then 
	  if execstr('getf(%path)','errcatch')<>0 then
	     message([name+': error:'
		      lasterror()])
	  end
	end
      end
    end
endfunction
