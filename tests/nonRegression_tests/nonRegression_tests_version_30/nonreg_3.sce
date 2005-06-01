mode (-1)

disp("")
disp("Tests de non-régression pour Scilab-3.0 : début");
disp("")

lines(0);

// dans la variable num_test mettre tous les numéros de bugs corrigés dans la version 3.0

num_test = [1321,1262,1249,1239,1227,1224,1200,1198,1183,1149,1110,1107,1084,1082,1074,1057,1025,1014,1004,1003,979,967,946,945,943,942,925,924,883,862,859,857,853,801,800,871,797,793,790,750,731,699,682,681,656,654,650,649,632,629,625,623,612,610,608,604,584,575,570,569,568,566,564,558,557,553,549,546,545,542,534,528,525,502,498,488,487,480,477,476,475,474,470,467,464,461,71,55]

// 683,973

for k = num_test
	exec('bug' + string(k) + '.sci')
	clear k
end

disp("") 
disp("Tests de non-régression pour Scilab-3.0 : fin");
disp("")

quit;