
function winNum=gcw()

if winsid()==[]
   fig(0);
   winNum=0;
else
   winNum=xget('window');
end

