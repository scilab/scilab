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
      scs_m(1)(2)(1)=name
      [ok, %wh]=getvalue(['Enter the type of Interf. Func. to create'],..
	  ['Block(0) or SuperBlock(1)'],list('vec','1'),..
	  ['1'])
      if ok then
	scs_m(1)(2)(1)=name
	if  %wh==0 then
	  %path=save_csuper(scs_m,dir)
	else
	  %path=save_super(scs_m,dir)
	end
	errcatch(-1,'continue')
	if %path<>[] then getf(%path),end
	errcatch(-1)
	if iserror(-1) then
	  errclear(-1)
	  message(name+': error. Perhaps bad name')
	end
      end
    end

