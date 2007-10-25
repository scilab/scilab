function sheets=readxls(fil)
//Authors Pierrick Mode, Serge Steer INRIA, Copyright INRIA
  if argn(2)==1 then opt=1,end
  fil=pathconvert(fil,%f,%t)
  sheets=mlist(['xls','sheets'],list())
  [fd,Text,Sheetnames,Abspos]=xls_open(fil)
  Text=['', Text];
  for k=1:size(Abspos,'*')
    [Value,TextInd]=xls_read(fd,Abspos(k))
    if Value==[] then
      sheet=mlist(['xlssheet','name','text','value'],Sheetnames(k),[],Value)
    else
      if size(Text,'*')==1 then
	T=emptystr(Value)
      else
	T=matrix(Text(TextInd+1),size(Value))
      end
      sheet=mlist(['xlssheet','name','text','value'],Sheetnames(k),T,Value)
    end
    sheets.sheets($+1)= sheet
  end
  mclose(fd)
endfunction
