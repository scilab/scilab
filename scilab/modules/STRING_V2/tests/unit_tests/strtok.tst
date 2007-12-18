//=================================================
if strtok('') <> '' then pause,end
//=================================================
if strtok('','') <> '' then pause,end
//=================================================
if strtok('','a') <> '' then pause,end
if strtok('a') <> '' then pause,end
//=================================================
TOKENS = [];
token = strtok("A string of ,,tokens and some  more tokens"," ,");
TOKENS = [TOKENS,token];
while( token <> '' )
  token = strtok(" ,");
  TOKENS = [TOKENS,token];
end
REF = 'A string of tokens and some more tokens ';
if strcat(TOKENS,' ')<> REF then pause,end
//=================================================
