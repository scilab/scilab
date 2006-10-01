
Wheel.map 	: Maple source which creates 
	wheel.f and wheelg.f
	wheel.f : Fortran Code for simulation 
	wheelg.f : Fortran code for geometric transformation ( used in graphics)
	dlslv.f: utility function. 

Euler.map : General code dealing with euler equations used by Wheel.map 

all.tex : include systeme.tex which gives in TeX the computations of Maple
	systeme.tex is automatically generated 
	includes also wheel.tex for a picture 

wheel.fig : an xfig picture of the wheel

WARNING : If you are using MapleV.4 yopu should change the add function into 
	matadd in Wheel.map and Euler.map 

