function [model,ok]=build_modelica_block(blklstm,cmmat,NiM,NoM,name,path)
// Serge Steer 2003, Copyright INRIA
// given the blocks definitions in blklstm and connections in cmmat this
// function first create  the associated modelicablock  and writes its code
// in the file named 'imppart_'+name+'.mo' in the directory given by path
// Then modelica compiler is called to produce the C code of scicos block
// associated to this modelica block. filbally the C code is compiled and
// dynamically linked with Scilab.
// The correspondind model data structure is returned.
name='imppart_'+stripblanks(name);
path=pathconvert(stripblanks(path),%t,%t)
[txt,rpar,ipar]=create_modelica1(blklstm,cmmat,name);
mputl(txt,path+name+'.mo');
mprintf('   Modelica code generated at '+path+name+'.mo\n')
[ok,name1,nx,nin,nout,ng,nm,nz]=compile_modelica(path+name+'.mo');

if ~ok then return,end

//nx is the state dimension
//ng is the number of surfaces

//build model data structure of the block equivalent to the implicit part
model=scicos_model(sim=list(name,10004),.. 
	           in=ones(nin,1),out=ones(nout,1),..
		   state=zeros(nx*2,1),..
		   dstate=zeros(nz,1),..
		   rpar=rpar,..
		   ipar=ipar,..
		   dep_ut=[%f %t],nzcross=ng,nmode=nm)
endfunction
