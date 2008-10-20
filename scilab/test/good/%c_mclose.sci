function %c_mclose(str)
if convstr(str)=='all' then
  file('close',file())
else
 [units,typ,nams]=file()
 k=find(nams==str)
 if k<>[] then
   file('close',k)
 else
   write(%io(2),'No such file opened file')
 end
end
endfunction
