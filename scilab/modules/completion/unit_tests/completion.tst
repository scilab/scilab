lines(0);

// parameters tests
r = completion('w')
r = completion('w','functions')
r = completion('w','commands')
r = completion('w','variables')
r = completion('w','macros')
r = completion('w','graphics_properties')
r = completion('w','files')

[functions,commands,variables,macros,graphics_properties,files] = completion('w')
[functions,commands,variables,macros,graphics_properties] = completion('w')
[functions,commands,variables,macros] = completion('w')
[functions,commands,variables] = completion('w')
[functions,commands] = completion('w')

// memory leak

for i=1:1000000,a=completion('w');end;
for i=1:1000000,a=completion('w',"functions");end;
for i=1:1000000,a=completion('w',"commands");end;
for i=1:1000000,a=completion('w',"variables");end;
for i=1:1000000,a=completion('w',"macros");end;
for i=1:1000000,a=completion('w',"graphics_properties");end;
for i=1:1000000,a=completion('w',"files");end;
