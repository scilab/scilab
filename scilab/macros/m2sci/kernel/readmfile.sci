function txt=readmfile(fil)
// Copyright INRIA

// Read an M-file for M2SCI translator

nbchar=1000
t=[]
txt=[]
cr=10
nl=13
EOL=ascii(10) // End of line

// Open file for reading
fd=mopen(fil,"r")
while %t
  // Read nbchar unsigned characters
  t=[t mget(nbchar,"uc",fd)]
  
  keol=find(t==10|t==13)
  if size(keol,2)>1 then // More than one completion for CR and NL
    if and(t(keol(1:2))==[13,10]) then // File written with CR and NL at the end of a line
      if t($)==13 then // Reading ended in the middle of an end of line sequence (only NL were read)
	t($)=[]
	keol($)=[]
	mseek(-1,fd,'cur')
      end
      t(keol(1:2:$))=[] // Del all NL caracters in t
      keol=keol(1:2:$)-[0 cumsum(ones(1,size(keol,2)/2-1))] // Update indexes for EOL
    end
  end
  
  // Copy t in txt line per line
  k1=1
  k2=0
  for k=1:size(keol,2)
    k2=keol(k)
    txt=[txt;ascii(t(k1:k2-1))]
    k1=k2+1
  end
  t([1:k2])=[]; // If part of a line has been read, it is kept in t
  
  // When EOF has been reached
  if meof(fd)<>0 then
    mclearerr(fd)
    mclose(fd)
    break
  end
end

// If last line is not ended by EOL
if t<>[] then
  txt=[txt;ascii(t)]
end
endfunction
