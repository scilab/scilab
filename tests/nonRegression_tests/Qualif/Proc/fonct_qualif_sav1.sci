function tests_suite_up_to_date=find_tests_suite_up_to_date(directory)
//recuperer les batteries de tests en cours sous directory dans une matrice
// verifier que tests_suite sont des repertoires quand directory=envq_work 
// et fichier quand log ???
tests_suite=""
select directory
 case envq_work then 
     
     tests_suite=basename(listfiles(directory)); 
     the_list=""
     j=1
     for i=1:size(tests_suite,"*")
         if isdir (envq_work+tests_suite(i)) then 
//       tests_suite(i) est un répertoire
            the_list(j) = tests_suite(i)
            j=j+1
 //           disp "boubou" 
   //         pause
         end
     end // du for
     tests_suite=the_list  
 case envq_log+envq_ver+"\" then
      tests_suite=basename(listfiles(directory+"*.log")) 
 //      ierr=execstr(tests_suite=basename(listfiles(envq_log+envq_ver+"\*.log")),'errcatch') 
 //      disp ("111")
 //      pause
 case envq_work then
//************************************************************* 
// chercher la liste de frioul
//************************************************************* 
    tests_suite=read(envq_work+"frioul_tests_suite_list.txt",-1,1,'(a)')  
 else
    disp ("PROBLEME")
    abort
 end // du case
//disp("tests_suite de find tests suute up to date")
//pause
tests_suite_up_to_date=sort(tests_suite)
//disp (tests_suite_up_to_date)
//disp (typeof(tests_suite_up_to_date))
//pause
endfunction

function affich_message (graph_affich, texte)
if graph_affich then
//afficher message windows pour indiquer pas de batterie dans la liste de selection
x_message(texte)
else
//afficher message prompt pour indiquer pas de batterie dans la liste de selection
disp(texte)
end // du if 
endfunction

function tests_suite_selected=graph_select_tests_suite(command, tests_suite)
//parcourir la matrice ainsi créée et charger la liste pour le menu de selection des batteries
  the_list=list()
  for i=1:size(tests_suite,"*")
     the_list($+1)=list(tests_suite(i),1,['Non','Oui'])
  end // du for
  rep=x_choices("Tests Suite Selection Menu for "+convstr(command,'u')+" command.",the_list);

  //prendre dans rep les reponses à oui (valeur 2);retrouver la batterie de tests correspondante ; et lancer son execution
  tests_suite_selected=""
  for i=1:size(tests_suite,"*")
    if rep(i)==2 then
  //     disp ("je sui sla")
  //     pause
       tests_suite_selected = tests_suite_selected + " " + tests_suite(i)
  //           disp ("je sui encore la")
  //     pause
    end // du if
  end // du for
endfunction

function tests_suite_selected=input_select_tests_suite(tests_suite)
   disp (" ");
   disp(tests_suite);
   disp (" ");
   //tests_suite_selected=input("Type tests_suite : ","string")  -- input ne prend que le premier string avant le blanc
   disp("Type tests_suite : ")
   the_list=list()
   the_list=read(%io(1),1,1,'(a)') 
   tests_suite_selected=the_list
endfunction

function graph_cmd(command,directory)
available_tests_suite=find_tests_suite_up_to_date(directory)
//disp (" toto")
//pause
if isempty(available_tests_suite) then
   affich_message (%t, "*****   No tests_suite available   *****")
else
   choosen_tests_suite=graph_select_tests_suite(command,available_tests_suite)
 //  disp (" choosen")
//pause
   if ~isempty(choosen_tests_suite) then
    disp ("choosen_tests_suite : " + choosen_tests_suite)
   pause
      exec_cmd(command, directory, choosen_tests_suite)
   //else
   //   return
   end // du if
end // du if
endfunction

function prompt_cmd(command,param_command,directory)
 if isempty(param_command)
      // pas de parametres, saisie au prompt scilab
      available_tests_suite=find_tests_suite_up_to_date(directory)
      if isempty(available_tests_suite) then
         affich_message (%f, "*****   No tests_suite available   *****")
         // sortir de la fonction 
         return
      else
  //       disp (" titi")
  //      pause //affich_message (graph_interf, available_tests_suite)
         choosen_tests_suite=input_select_tests_suite(available_tests_suite)
  //               disp ("bababababa")
  //      pause
         // pour l'instant ne rien faire si erreur dans saisie laisser faire l OS
      end // du if
 else
         // il existe des arguments pour l'instant ne rien faire si erreur dans saisie laisser faire l OS
        choosen_tests_suite = param_command
 end // du if
        disp ("choosen_tests_suite : " + choosen_tests_suite)
      pause  
 if ~isempty(choosen_tests_suite) then
      exec_cmd(command, directory, choosen_tests_suite);
   //else
   //   return
end // du if
endfunction

function exec_cmdd(command, choosen_tests_suite)
 if MSDOS then
 //cmd="cmd.exe /e:on /v:on /c init_qualif.bat&"+command+".bat "+choosen_tests_suite
  cmd="cmd.exe /e:on /v:on /C init_qualif.bat&test.bat "+choosen_tests_suite

      disp (cmd)
      pause
 //  %SystemRoot%\system32\cmd.exe /e:on /v:on /K e:\qualif\proc\init_qualif.bat
      chdir(envq_proc)
      unix_w(cmd)
      //unix_w ("cmd.exe /e:on /v:on /c init_qualif.bat&"+command+".bat "+choosen_tests_suite)
      chdir(envq_qualif)
//      unix_w (envq_proc+"init_qualif.bat&"+command+".bat "+choosen_tests_suite)
 else
      unix_w (command+ " " +choosen_tests_suite)
 end
endfunction

function tests_suite_without_wildcard = no_wildcard (tests_suite, directory)
if ~isempty(grep(tests_suite,'*')) then
   indice=grep(tests_suite,'*')
   the_list=[]
   for i=indice
         if ~isempty(basename(listfiles(directory+tests_suite(i)))) then
            the_list=[the_list;basename(listfiles(directory+tests_suite(i)))]
         end //if
   end // du for
   for i=sort(indice)
         tests_suite(i)=[]
   end // du for   
   tests_suite = [tests_suite ; the_list]
end // du if
tests_suite_without_wildcard=sort(tests_suite)
endfunction

function tests_suite_is_directory = only_directory (tests_suite, directory)
//attention : la tests_suite en parametre doit être construite avec listfiles et sans basename pb scilab-2.7.2
the_list=[]
for i=1:size(tests_suite,"*")
//disp("i = " + directory+tests_suite(i))
    if isdir(directory+tests_suite(i)) then 
    //   tests_suite(i) est un répertoire
       the_list =[the_list;tests_suite(i)]
   end
end // du for
tests_suite_is_directory=the_list  
endfunction

function exec_cmd(command, directory, choosen_tests_suite)

//********************************
// liste des fichier du repertoire directory, on ne garde que les repertoires, puis on ne garde que le nom de fichier et on en fait une liste
// pour clearwork et runtest strcat(strsubst(only_directory(listfiles(envq_work+'*')),envq_work,''),' ') 
// pour clearlog strcat(strsubst(listfiles(envq_log+envq_ver+"\"+'*.log'),envq_log+envq_ver+"\",''),' ')  
// pour copyftp  
//**********************************

//rep=tokens(choosen_tests_suite," ")
disp ("choosen_tests_suite de exec_cmd : "+ choosen_tests_suite)
//disp ("rep : "+ rep)
pause
for i=1:size(choosen_tests_suite,"*")
//for i=1:size(rep,"*")
//    case "copyftp" then 
//    tests_suite=basename(listfiles(directory)); 
//     the_list=""
//     j=1
//****************************
// dos_command=tlist(['commande dos';'copyftp';'clearlog'],"copy toto","supprime toto")  
//dos_command=tlist(['dos command';'copyftp';'runtest';'copyrun';'clearlog';'clearwork'],"copyftp dos","runtest dos","copyrun dos","erase /s /q ","rmdir /s /q ") 
//not_dos_command=tlist(['not dos command';'copyftp';'runtest';'copyrun';'clearlog';'clearwork'],"copyftp unix","runtest unix","copyrun unix","clearlog unix","clearwork unix") 
//system_cmd=tlist(['operating system';'%t';'%f'],dos_command,not_dos_command)
//os_cmd(sci2exp(MSDOS))(command)+directory+choosen_tests_suite

//strsubst(only_directory(listfiles(envq_work+'*')),envq_work,'') 
//unix_g(os_cmd(sci2exp(MSDOS))("clearwork")+envq_work+toto(2)) 


unix_g(os_cmd(sci2exp(MSDOS))(command)+directory+choosen_tests_suite(i))

//*************

end // du for 

endfunction


function affich_date
w=getdate()
disp(date() + "  "+string(w(7))+" h. "+string(w(8))+" mn. "+string(w(9))+" s. ")
endfunction


function affich_result(pasOK, num)
// affiche FAILED ou PASSED, fonction utiliser dans les tests-suites de non regression
   // This part display result
   // le booleen pasOK est à faux
   if ~pasOK then
     disp("*****************************************  Test bug" + string(num)  +" : FAILED ! ");
   else
    disp("Test bug" + string(num)  +" : PASSED");
   end
endfunction

function my_mat = list2mat (my_list)
my_mat = ['']  
//for k=1:size(my_list,"*")
for k=1:size(my_list)
         my_mat (k) = my_list(k)
end
endfunction 


function mat_rep = sel_rep(my_mat)
  mat_rep = ['']
// cas d'une liste
mat_rep = ''
  n=1
  for k=1:size(my_mat,"*")
//  for k=1:size(my_mat)
     if isdir(my_mat(k)) then 
         mat_rep (n) = my_mat(k)
         n = n+1
     end
  end
endfunction

function copyftp(param)
if argn(2)==0 then 
   param = []
end // du if
prompt_cmd("copyftp",param,envq_log)
endfunction


function test (str)
a=[]
interv=strindex(str," ")
debut=1
for i=1:length(interv)
    a=[a;part(str,[debut:(interv(i))-1])]
    debut=interv(i)+1
end
a=[a;part(str,[debut:length(str)])]
disp (a)
endfunction 

function clearwork (param)
if argn(2)==0 then 
   param = []
end // du if
prompt_cmd("clearwork",param,envq_work)
endfunction
       
function clearlog (param)
if argn(2)==0 then 
   param = []
end // du if
prompt_cmd("clearlog",param,envq_log+envq_ver)
endfunction



function showlog (my_cmd)
       if argn(2)==0 then 
             //disp " "
             write(%io(2),"SHOWLOG : ")
             write(%io(2),"Pour visualiser la derniere ligne de un, plusieurs ou tous fichiers log : ")
             write(%io(2),"- toutes les tests_suites faire : showlog *")
             write(%io(2),"- certaines tests_suites faire  : showlog ""nonreg* tests_20?? scicos""")
             write(%io(2),"- une tests_suite faire         : showlog ""scicos""")
             disp " "
       else
             if MSDOS then
                unix_w (envq_proc+"init_qualif.bat&showlog.bat "+my_cmd)
             else
                unix_w ("showlog" +my_cmd)
             end
       end
endfunction


function dirlog (my_cmd)
if argn(2)==0 then 
   param = []
end // du if
prompt_cmd("clearlog",param,envq_log+envq_ver)
endfunction


function dirwork (my_cmd)
      if argn(2)==0 then 
             //disp " "
             write(%io(2),"DIRWORK : ")
             write(%io(2),"Pour visualiser la derniere ligne de un, plusieurs ou tous fichiers log : ")
             write(%io(2),"- toutes les tests_suites faire : showlog *")
             write(%io(2),"- certaines tests_suites faire  : showlog ""nonreg* tests_20?? scicos""")
             write(%io(2),"- une tests_suite faire         : showlog ""scicos""")
             disp " "
       else
             disp(sort(basename(listfiles(envq_work+my_cmd))))

      //if MSDOS then
      //               unix_w (envq_proc+"init_qualif.bat&showlog.bat "+my_cmd)
      //           else
      //               unix_w ("showlog" +my_cmd)
      //       end  
       end
endfunction

function copyrun(param)
if argn(2)==0 then  
             copyftp()
             runtest()
else
    copyftp (param)
    runtest (""""+param+"""")
end
endfunction


function runtestXXXXXXXXXXX(tout)

if argn(2)==0 then 
             //disp " "
             write(%io(2),"RUNTEST : ")
             write(%io(2),"Pour executer en local : ")
             write(%io(2),"- toutes les tests_suites faire : runtest (""*"")")
             write(%io(2),"- certaines tests_suites faire  : runtest (""nonreg* tests_20?? scicos"")")
             write(%io(2),"- une tests_suite faire         : runtest (""scicos"")")
             disp " "
else

//disp (length(varargin))

//for i=1:length(varargin)
//disp (varargin(i))
//end // du for

my_list=tokens(tout," ")

//pause

// exemple : displist(["autre?ep";"n*"])
ierr=0
//my_list = ["autrerep","hif","n*"] 
//my_list = listfiles(envq_work+my_list
the_list=[]
//for i=1:size(my_list)
for i=1:size(my_list,"*")
    if isdir (envq_work+my_list(i)) then 
//  my_list(i) est un répertoire
       the_list = [the_list;my_list(i)]
    else
       the_list = [the_list;basename(listfiles(envq_work+my_list(i)))]
    end
end // du for

//disp("ici")
//pause


the_list2=[]
for i=1:size(the_list,"*")
    if isdir (envq_work+the_list(i)) then 
       the_list2 = [the_list2;the_list(i)]
    end
end // du for

//disp("là")
//pause

for k=1:size(the_list2,"*")
    bibi = the_list2(k)
    chdir (envq_work + basename(the_list2(k)))
    unix_w ("erase "+envq_log+envq_ver+"\test_en_cours.txt")
    diary (envq_log+envq_ver+"\test_en_cours.txt")
    ierr=0
//    diary (envq_log+envq_ver+"\"+the_list2(k)+".txt")
    ierr = exec (the_list2(k) + ".sce", 'errcatch')
    if ierr<>0 then 
       disp("")
       disp " LA TESTS-SUITE " + the_list2(k) + " S ''EST INCORRECTEMENT TERMINEE.----------------------------"
       disp("Error n° "+string(ierr));
       disp(lasterror());
       disp("")
    end
    diary(0)
    if MSDOS then
//disp ("msdos")    
//pause
//    disp ("bibi = " + bibi)
//pause
//unix_w ("type "+envq_log+envq_ver+"\test_en_cours.txt >> "+envq_log+envq_ver+"\"+the_list2(k)+".txt")
unix_w ("type "+envq_log+envq_ver+"\test_en_cours.txt >> "+envq_log+envq_ver+"\"+bibi+".txt")
//    unix_w ("erase "+envq_log+envq_ver+"\test_en_cours.txt")
    else
// monde linux utiliser commande cat
    end
//       disp (basename(the_list2(k)))
end // du for

end
endfunction


function aide(varargin)
// fonction executer au lancement de l'environnement de qualification scilab sous window
   if argn(2)==1 then
                  if (varargin(1)=="all") | (varargin(1)=="ALL" )then 
                 disp (" ")
                 copyftp()
                 runtest()
//               copyrun()
                 clearwork()
                 clearlog()
                 disp (" ")
                 end
   else 
                 disp (" ")
                 write(%io(2),"   ENVIRONNEMENT DE QUALIFICATION SCILAB")
                 disp (" ")
                 write(%io(2),"   commandes disponibles : COPYFTP() - RUNTEST() - COPYRUN() - CLEARWORK() - CLEARLOG()")
                 write(%io(2)," ")
                 write(%io(2),"   pour une aide faire : AIDE (""all"") ou lancer une commande sans parametres ")
                 disp (" ")
   end
endfunction
