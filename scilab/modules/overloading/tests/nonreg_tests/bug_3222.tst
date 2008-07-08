clear D;D(1)(2)="3";
if or(D<>[" " "3"]) then pause,end

clear D;D(1).x="3";
if D.x<>"3" then pause,end

clear D;D(1,2).x="3";
if D(1).x<>[]  then pause,end
if D(2).x<>"3"  then pause,end


clear D;D.x(1,2)="3";
if or(D.x<>[" " "3"]) then pause,end

clear D;D(1,1,2)="3";
if D(1,1,2)<>"3"  then pause,end
