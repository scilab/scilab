//===============================
// unit tests isalphanum
// INRIA 2007
//===============================
TXT = 'A1,B2,C3';
//===============================
if isalphanum(TXT) <> [ %T %T %F %T %T %F %T %T] then pause,end
//===============================
if isalphanum('') <> [] then pause,end
//===============================
