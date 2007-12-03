// Copyright INRIA
//This part of the test verify visual aspect of the dialogs window 
// and in particular scroll bars
// It must be checked visually
mode(-1)
//  x_message
//  =========
x_message(['Simple message';'No scroll bar at all']);
x_message(['message with huge vertical part';
          'Only vertical scroll bar'
          string(1:50)']);
x_message(['message with huge horizontal  part';
          'Only horizontal scroll bar'
           strcat(string(1:150),' ')]);
x_message(['huge message';
          'Vertical and horizontal scroll bars'
           strcat(string(1:150),' ');
           string(1:50)']);
//  x_dialog
//  ========
x_dialog(['Simple dialog';'No scroll bar at all'],['0';'1']);
x_dialog(['dialog with huge vertical label part';
          'Only vertical scroll bar'
          'for the Label part';
          string(1:50)'],['0';'1']);
x_dialog(['dialog with huge horizontal label part';
          'Only horizontal scroll bar'
          'for the Label part';
           strcat(string(1:150),' ')],['0';'1']);
x_dialog(['dialog with huge  label part';
          'Vertical and horizontal scroll bars'
          'for the Label part';
           strcat(string(1:150),' ');
           string(1:50)'],['0';'1']);
x_dialog(['dialog with huge vertical dialog part';
          'Only vertical scroll bar'
          'for the Dialog part'],string(1:50)');
x_dialog(['dialog with huge horizontal dialog part';
          'Only horizontal scroll bar'
          'for the Dialog part'],strcat(string(1:150),' '));
x_dialog(['dialog with huge  dialog part';
          'Vertical and horizontal  scroll bars'
          'for the Dialog part'],[strcat(string(1:150),' ');string(1:50)']);
x_dialog(['dialog with huge  label and dialog part';
          'Vertical and horizontal scroll bars'
          'for the Label part';
           strcat(string(1:150),' ');
           string(1:50)'],[strcat(string(1:150),' ');string(1:50)']);
//  x_choose
// =========
x_choose(string(1:10)',['Simple choose';'No scroll bar at all']);
x_choose(string(1:10)',['choose with huge vertical label part';
          'Only vertical scroll bar'
          'for the Label part';
          string(1:50)']);
x_choose(string(1:10)',['choose with huge horizontal label part';
          'Only horizontal scroll bar'
          'for the Label part';
           strcat(string(1:150),' ')]);
x_choose(string(1:10)',['choose with huge  label part';
          'Vertical and horizontal scroll bars'
          'for the Label part';
           strcat(string(1:150),' ');
           string(1:50)']);
x_choose(string(1:50)',['choose with huge vertical choose part';
          'Only vertical scroll bar'
          'for the Choose part']);
x_choose(strcat(string(1:150),' '),['choose with huge horizontal choose part';
          'Only horizontal scroll bar'
          'for the Choose part']);
x_choose([strcat(string(1:150),' ');string(1:50)'],['choose with huge  choose part';
          'Vertical and horizontal  scroll bars'
          'for the Choose part']);
x_choose([strcat(string(1:150),' ');string(1:50)'],['choose with huge  label and choose part';
          'Vertical and horizontal scroll bars'
          'for the Label part';
           strcat(string(1:150),' ');
           string(1:50)']);

//  x_mdialog
//  ========
x_mdialog(['Simple multiple dialog';'No scroll bar at all'],..
	['A','B'],['0';'1']);
x_mdialog(['dialog with huge vertical label part';
          'Only vertical scroll bar'
          'for the Label part';
          string(1:50)'],['A','B'],['0';'1']);
x_mdialog(['multiple dialog with huge horizontal label part';
          'Only horizontal scroll bar'
          'for the Label part';
           strcat(string(1:150),' ')],['A','B'],['0';'1']);
x_mdialog(['multiple dialog with huge  label part';
          'Vertical and horizontal scroll bars'
          'for the Label part';
           strcat(string(1:150),' ');
           string(1:50)'],['A','B'],['0';'1']);
x_mdialog(['multiple dialog with huge vertical dialog part';
          'Only vertical scroll bar'
          'for the Dialog part'],string(1:50)',string(1:50)');
x_mdialog(['multiple dialog with huge horizontal dialog part';
          'Only horizontal scroll bar'
          'for the Dialog part'],'label',strcat(string(1:150),' '));
x_mdialog(['multiple dialog with huge  dialog part';
          'Vertical and horizontal  scroll bars'
          'for the Dialog part'],..
          ['label';string(1:50)'],[strcat(string(1:150),' ');string(1:50)']);
x_mdialog(['multiple dialog with huge  label and dialog part';
          'Vertical and horizontal scroll bars'
          'for the Label part';
           strcat(string(1:150),' ');
           string(1:50)'],..
           ['label';string(1:50)'],..
           [strcat(string(1:150),' ');string(1:50)']);
//  x_mdialog for matrix 
//  =====================
 n=5;m=4;mat=rand(n,m);
 row='row';labelv=row(ones(1,n))+string(1:n)';
 col='col';labelh=col(ones(1,m))+string(1:m)';
 new=evstr(x_mdialog('Small Matrix to edit',labelv,labelh,string(mat)));

 n=20;m=20;mat=rand(n,m);
 row='row';labelv=row(ones(1,n))+string(1:n)';
 col='col';labelh=col(ones(1,m))+string(1:m)';
 new=evstr(x_mdialog('Big Matrix to edit',labelv,labelh,string(mat)));
// x_choices 

l1=list('choice 1',1,['toggle c1','toggle c2','toggle c3']);
l2=list('choice 2',2,['toggle d1','toggle d2','toggle d3']);
l3=list('choice 3',3,['toggle e1','toggle e2']);
rep=x_choices('Toggle Menu',list(l1,l2,l3));

tog='toggle ';tog=tog(ones(1,500))'+string(1:500);
l1=list('choice 1',1,tog);
l2=list('choice 2',2,['toggle d1','toggle d2','toggle d3']);
l3=list('choice 3',3,['toggle e1','toggle e2']);
rep=x_choices('Toggle Menu',list(l1,l2,l3));
