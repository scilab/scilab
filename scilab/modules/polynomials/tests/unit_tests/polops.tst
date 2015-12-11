// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

lines(0)
s=poly(0,'s');
//polynomial operations
p1=1+1.5*s;
if p1<>poly([1 1.5],'s','c') then pause,end
p2=p1*p1;
if norm(coeff(p2-poly([1 3 2.25],'s','c')))>10*%eps then pause,end
if norm(coeff(4*p2-poly([4 12 9],'s','c')))>10*%eps then pause,end
if norm(coeff(p2*4-poly([4 12 9],'s','c')))>10*%eps then pause,end
p2=p2*4;
if norm(coeff(p2/4-poly([1 3 2.25],'s','c')))>10*%eps then pause,end
if norm(coeff(4\p2-poly([1 3 2.25],'s','c')))>10*%eps then pause,end
p2=p2/4;
if norm(coeff((p2-1)-poly([0 3 2.25],'s','c')))>10*%eps then pause,end
if norm(coeff((1-p2)-poly([0 -3 -2.25],'s','c')))>10*%eps then pause,end
if norm(coeff(p2-p2))>10*%eps then pause,end
//complex
p1=1+1.5*s+0*%i;
if norm(coeff(p1-poly([1 1.5],'s','c')))>10*%eps then pause,end
p2=p1*p1;
if norm(coeff(p2-poly([1 3 2.25],'s','c')))>10*%eps then pause,end
if norm(coeff(4*p2-poly([4 12 9],'s','c')))>10*%eps then pause,end
if norm(coeff(p2*4-poly([4 12 9],'s','c')))>10*%eps then pause,end
if norm(coeff((4+0*%i)*p2-poly([4 12 9],'s','c')))>10*%eps then pause,end
if norm(coeff(p2*(4+0*%i)-poly([4 12 9],'s','c')))>10*%eps then pause,end
p2=p2*4;
if norm(coeff(p2/4-poly([1 3 2.25],'s','c')))>10*%eps then pause,end
if norm(coeff(4\p2-poly([1 3 2.25],'s','c')))>10*%eps then pause,end
if norm(coeff(p2/(4+0*%i)-poly([1 3 2.25],'s','c')))>10*%eps then pause,end
if norm(coeff((4+0*%i)\p2-poly([1 3 2.25],'s','c')))>10*%eps then pause,end
p2=p2/4;
if norm(coeff((p2-1)-poly([0 3 2.25],'s','c')))>10*%eps then pause,end
if norm(coeff((1-p2)-poly([0 -3 -2.25],'s','c')))>10*%eps then pause,end
if norm(coeff((p2-(1+0*%i))-poly([0 3 2.25],'s','c')))>10*%eps then pause,end
if norm(coeff(((1+0*%i)-p2)-poly([0 -3 -2.25],'s','c')))>10*%eps then pause,end
//
if norm(coeff(p2-p2))>10*%eps then pause,end
//
//concatenations  extraction
p1=1+1.5*s;p2=1;
p3=[p1 p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2 p1]))>%eps then pause,end
p2=p1+4*s**2;
p3=[p1 p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2 p1]))>%eps then pause,end
//en colonne
p1=1+1.5*s;p2=1;
p3=[p1;p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2;p1]))>%eps then pause,end
p2=p1+4*s**2;
p3=[p1;p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2;p1]))>%eps then pause,end
//
p1=1+1.5*s;p2=1+%i;
p3=[p1 p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2 p1]))>%eps then pause,end
p2=p1+4*s**2+%i;
p3=[p1 p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2 p1]))>%eps then pause,end
//en colonne
p1=1+1.5*s;p2=1+%i;
p3=[p1;p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2;p1]))>%eps then pause,end
p2=p1+4*s**2+%i;
p3=[p1;p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2;p1]))>%eps then pause,end
// complex reel
//en ligne
p1=1+1.5*s+%i;p2=1;
p3=[p1 p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2 p1]))>%eps then pause,end
p2=p1+4*s**2;
p3=[p1 p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2 p1]))>%eps then pause,end
//en colonne
p1=1+1.5*s+%i;p2=1;
p3=[p1;p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2;p1]))>%eps then pause,end
p2=p1+4*s**2;
p3=[p1;p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2;p1]))>%eps then pause,end
//complex complex
//en ligne
p1=1+1.5*s+%i;p2=1+%i;
p3=[p1 p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2 p1]))>%eps then pause,end
p2=p1+4*s**2+%i;
p3=[p1 p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2 p1]))>%eps then pause,end
//en colonne
p1=1+1.5*s+%i;p2=1+%i;
p3=[p1;p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2;p1]))>%eps then pause,end
p2=p1+4*s**2+%i;
p3=[p1;p2];
if norm(coeff(p3(1)-p1))>%eps then pause,end
if norm(coeff(p3(2)-p2))>%eps then pause,end
if norm(coeff(p3(1:2)-p3))>%eps then pause,end
if norm(coeff(p3([2 1])-[p2;p1]))>%eps then pause,end
//
//                real real
//
p1=[1+1.5*s;s**2];
//
if norm(coeff(p1'-[p1(1),p1(2)]))>10*%eps then pause,end
if norm(coeff((-p1)+p1))>10*%eps then pause,end
if norm(coeff((-p1')+p1'))>10*%eps then pause,end
//
x=[1+s**3; 4];
p3=p1(1)*x(1)+p1(2)*x(2);
if norm(coeff(p1'*x-p3))>10*%eps then pause,end
p3=[p1(1)*x(1),p1(1)*x(2);p1(2)*x(1), p1(2)*x(2)];
if norm(coeff(p1*x'-p3))>10*%eps then pause,end
//
x=[1.5;1/3];
p3=p1(1)*x(1)+p1(2)*x(2);
if norm(coeff(p1'*x-p3))>10*%eps then pause,end
if norm(coeff(x'*p1-p3))>10*%eps then pause,end
p3=[p1(1)*x(1),p1(1)*x(2);p1(2)*x(1), p1(2)*x(2)];
if norm(coeff(p1*x'-p3))>10*%eps then pause,end
if norm(coeff(x*p1'-p3'))>10*%eps then pause,end
//
x=4;
if norm(coeff(x*p1-[x*p1(1);x*p1(2)]))>10*%eps then pause,end
if norm(coeff(p1*x-[x*p1(1);x*p1(2)]))>10*%eps then pause,end
if norm(coeff(x*p1'-[x*p1(1),x*p1(2)]))>10*%eps then pause,end
if norm(coeff(p1'*x-[x*p1(1),x*p1(2)]))>10*%eps then pause,end
//
x=[1+s**3; 4];
p3=[p1(1)*x(1);p1(2)*x(2)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
x=[1.5;1/3];
p3=[p1(1)*x(1);p1(2)*x(2)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
if norm(coeff(x.*p1-p3))>10*%eps then pause,end
x=[1+s**3,4];
p3=[p1(1)*x(1),p1(2)*x(2)];
if norm(coeff(p1'.*x-p3))>10*%eps then pause,end
x=[1.5,1/3];
p3=[p1(1)*x(1),p1(2)*x(2)];
if norm(coeff(p1'.*x-p3))>10*%eps then pause,end
if norm(coeff(x.*p1'-p3))>10*%eps then pause,end

// matrices
p1=[1+1.5*s,s**2, 0;1 s s+1];
//
if norm(coeff((-p1)+p1))>10*%eps then pause,end
//
x=[1,s;-1,2*s;2,0];
p3=[p1(1,1)*x(1,1)+p1(1,2)*x(2,1)+p1(1,3)*x(3,1),...
        p1(1,1)*x(1,2)+p1(1,2)*x(2,2)+p1(1,3)*x(3,2);
    p1(2,1)*x(1,1)+p1(2,2)*x(2,1)+p1(2,3)*x(3,1),...
        p1(2,1)*x(1,2)+p1(2,2)*x(2,2)+p1(2,3)*x(3,2)];
if norm(coeff(p1*x-p3))>10*%eps then pause,end
//
x=[1 1.5;2 1/3;sqrt(7) 8];
p3=[p1(1,1)*x(1,1)+p1(1,2)*x(2,1)+p1(1,3)*x(3,1),...
        p1(1,1)*x(1,2)+p1(1,2)*x(2,2)+p1(1,3)*x(3,2);
    p1(2,1)*x(1,1)+p1(2,2)*x(2,1)+p1(2,3)*x(3,1),...
        p1(2,1)*x(1,2)+p1(2,2)*x(2,2)+p1(2,3)*x(3,2)];
if norm(coeff(p1*x-p3))>10*%eps then pause,end
//
x=5;
p3=[x*p1(1,1),x*p1(1,2),x*p1(1,3);
    x*p1(2,1),x*p1(2,2),x*p1(2,3)];

if norm(coeff(p1*x-p3))>10*%eps then pause,end
if norm(coeff(x*p1-p3))>10*%eps then pause,end

//
x=[1+s**3, 4,-s;0,s+1,10*s**4];
p3=[p1(1,1)*x(1,1),p1(1,2)*x(1,2),p1(1,3)*x(1,3);
    p1(2,1)*x(2,1),p1(2,2)*x(2,2),p1(2,3)*x(2,3)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
x=[1 1.5;2 1/3;sqrt(7) 8]';
p3=[p1(1,1)*x(1,1),p1(1,2)*x(1,2),p1(1,3)*x(1,3);
    p1(2,1)*x(2,1),p1(2,2)*x(2,2),p1(2,3)*x(2,3)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
if norm(coeff(x.*p1-p3))>10*%eps then pause,end

//
//               real    complex
//
p1=[1+1.5*s;s**2];
//
if norm(coeff(p1'-[p1(1),p1(2)]))>10*%eps then pause,end
if norm(coeff((-p1)+p1))>10*%eps then pause,end
if norm(coeff((-p1')+p1'))>10*%eps then pause,end
//
x=[1+s**3; 4+%i];
p3=p1(1)*x(1)+p1(2)*x(2);
if norm(coeff(p1'*x-p3))>10*%eps then pause,end
p3=[p1(1)*x(1),p1(1)*x(2);p1(2)*x(1), p1(2)*x(2)];
if norm(coeff(p1*conj(x')-p3))>10*%eps then pause,end
//
x=[1.5+%i;1/3];
p3=p1(1)*x(1)+p1(2)*x(2);
if norm(coeff(p1'*x-p3))>10*%eps then pause,end
if norm(coeff(conj(x')*p1-p3))>10*%eps then pause,end
p3=[p1(1)*x(1),p1(1)*x(2);p1(2)*x(1), p1(2)*x(2)];
if norm(coeff(p1*conj(x')-p3))>10*%eps then pause,end
if norm(coeff(x*p1'-conj(p3')))>10*%eps then pause,end
//
x=4+%i;
if norm(coeff(x*p1-[x*p1(1);x*p1(2)]))>10*%eps then pause,end
if norm(coeff(p1*x-[x*p1(1);x*p1(2)]))>10*%eps then pause,end
if norm(coeff(x*p1'-[x*p1(1),x*p1(2)]))>10*%eps then pause,end
if norm(coeff(p1'*x-[x*p1(1),x*p1(2)]))>10*%eps then pause,end
//
x=[1+s**3; 4-3*%i];
p3=[p1(1)*x(1);p1(2)*x(2)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
x=[1.5+%i/5;1/3];
p3=[p1(1)*x(1);p1(2)*x(2)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
if norm(coeff(x.*p1-p3))>10*%eps then pause,end
x=[1+s**3,4-%i];
p3=[p1(1)*x(1),p1(2)*x(2)];
if norm(coeff(p1'.*x-p3))>10*%eps then pause,end
x=[1.5,1/3-3*%i];
p3=[p1(1)*x(1),p1(2)*x(2)];
if norm(coeff(p1'.*x-p3))>10*%eps then pause,end
if norm(coeff(x.*p1'-p3))>10*%eps then pause,end

// matrices
p1=[1+1.5*s,s**2, 0;1 s s+1];
//
if norm(coeff((-p1)+p1))>10*%eps then pause,end
//
x=[1,s+%i ;-1,2*s;2,0];
p3=[p1(1,1)*x(1,1)+p1(1,2)*x(2,1)+p1(1,3)*x(3,1),...
        p1(1,1)*x(1,2)+p1(1,2)*x(2,2)+p1(1,3)*x(3,2);
    p1(2,1)*x(1,1)+p1(2,2)*x(2,1)+p1(2,3)*x(3,1),...
        p1(2,1)*x(1,2)+p1(2,2)*x(2,2)+p1(2,3)*x(3,2)];
if norm(coeff(p1*x-p3))>10*%eps then pause,end
//
x=[1 1.5;2 1/3;%i-sqrt(7) 8];
p3=[p1(1,1)*x(1,1)+p1(1,2)*x(2,1)+p1(1,3)*x(3,1),...
        p1(1,1)*x(1,2)+p1(1,2)*x(2,2)+p1(1,3)*x(3,2);
    p1(2,1)*x(1,1)+p1(2,2)*x(2,1)+p1(2,3)*x(3,1),...
        p1(2,1)*x(1,2)+p1(2,2)*x(2,2)+p1(2,3)*x(3,2)];
if norm(coeff(p1*x-p3))>10*%eps then pause,end
//
x=5-2*%i;
p3=[x*p1(1,1),x*p1(1,2),x*p1(1,3);
    x*p1(2,1),x*p1(2,2),x*p1(2,3)];

if norm(coeff(p1*x-p3))>10*%eps then pause,end
if norm(coeff(x*p1-p3))>10*%eps then pause,end

//
x=[1+s**3, 4-3*%i,-s;0,s+1,10*s**4];
p3=[p1(1,1)*x(1,1),p1(1,2)*x(1,2),p1(1,3)*x(1,3);
    p1(2,1)*x(2,1),p1(2,2)*x(2,2),p1(2,3)*x(2,3)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
x=[1 1.5;2-%i/7, 1/3;sqrt(7) 8]';
p3=[p1(1,1)*x(1,1),p1(1,2)*x(1,2),p1(1,3)*x(1,3);
    p1(2,1)*x(2,1),p1(2,2)*x(2,2),p1(2,3)*x(2,3)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
if norm(coeff(x.*p1-p3))>10*%eps then pause,end
//
//                complex real
//
p1=[1+1.5*s;s**2+%i];
//
if norm(coeff(conj(p1')-[p1(1),p1(2)]))>10*%eps then pause,end
if norm(coeff((-p1)+p1))>10*%eps then pause,end
if norm(coeff((-conj(p1'))+conj(p1')))>10*%eps then pause,end
//
x=[1+s**3; 4];
p3=p1(1)*x(1)+p1(2)*x(2);
if norm(coeff(conj(p1')*x-p3))>10*%eps then pause,end
p3=[p1(1)*x(1),p1(1)*x(2);p1(2)*x(1), p1(2)*x(2)];
if norm(coeff(p1*x'-p3))>10*%eps then pause,end
//
x=[1.5;1/3];
p3=p1(1)*x(1)+p1(2)*x(2);
if norm(coeff(conj(p1')*x-p3))>10*%eps then pause,end
if norm(coeff(x'*p1-p3))>10*%eps then pause,end
p3=[p1(1)*x(1),p1(1)*x(2);p1(2)*x(1), p1(2)*x(2)];
if norm(coeff(p1*x'-p3))>10*%eps then pause,end
if norm(coeff(x*conj(p1')-conj(p3')))>10*%eps then pause,end
//
x=4;
if norm(coeff(x*p1-[x*p1(1);x*p1(2)]))>10*%eps then pause,end
if norm(coeff(p1*x-[x*p1(1);x*p1(2)]))>10*%eps then pause,end
if norm(coeff(x*conj(p1')-[x*p1(1),x*p1(2)]))>10*%eps then pause,end
if norm(coeff(conj(p1')*x-[x*p1(1),x*p1(2)]))>10*%eps then pause,end
//
x=[1+s**3; 4];
p3=[p1(1)*x(1);p1(2)*x(2)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
x=[1.5;1/3];
p3=[p1(1)*x(1);p1(2)*x(2)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
if norm(coeff(x.*p1-p3))>10*%eps then pause,end
x=[1+s**3,4];
p3=[p1(1)*x(1),p1(2)*x(2)];
if norm(coeff(conj(p1').*x-p3))>10*%eps then pause,end
x=[1.5,1/3];
p3=[p1(1)*x(1),p1(2)*x(2)];
if norm(coeff(conj(p1').*x-p3))>10*%eps then pause,end
if norm(coeff(x.*conj(p1')-p3))>10*%eps then pause,end

//matrices
p1=[1+1.5*s,s**2, 0;1 s s+1-%i/3];
//
if norm(coeff((-p1)+p1))>10*%eps then pause,end
//
x=[1,s ;-1,2*s;2,0];
p3=[p1(1,1)*x(1,1)+p1(1,2)*x(2,1)+p1(1,3)*x(3,1),...
        p1(1,1)*x(1,2)+p1(1,2)*x(2,2)+p1(1,3)*x(3,2);
    p1(2,1)*x(1,1)+p1(2,2)*x(2,1)+p1(2,3)*x(3,1),...
        p1(2,1)*x(1,2)+p1(2,2)*x(2,2)+p1(2,3)*x(3,2)];
if norm(coeff(p1*x-p3))>10*%eps then pause,end
//
x=[1 1.5;2 1/3;sqrt(7) 8];
p3=[p1(1,1)*x(1,1)+p1(1,2)*x(2,1)+p1(1,3)*x(3,1),...
        p1(1,1)*x(1,2)+p1(1,2)*x(2,2)+p1(1,3)*x(3,2);
    p1(2,1)*x(1,1)+p1(2,2)*x(2,1)+p1(2,3)*x(3,1),...
        p1(2,1)*x(1,2)+p1(2,2)*x(2,2)+p1(2,3)*x(3,2)];
if norm(coeff(p1*x-p3))>10*%eps then pause,end
//
x=5;
p3=[x*p1(1,1),x*p1(1,2),x*p1(1,3);
    x*p1(2,1),x*p1(2,2),x*p1(2,3)];

if norm(coeff(p1*x-p3))>10*%eps then pause,end
if norm(coeff(x*p1-p3))>10*%eps then pause,end

//
x=[1+s**3,4,-s;0,s+1,10*s**4];
p3=[p1(1,1)*x(1,1),p1(1,2)*x(1,2),p1(1,3)*x(1,3);
    p1(2,1)*x(2,1),p1(2,2)*x(2,2),p1(2,3)*x(2,3)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
x=[1 1.5;2, 1/3;sqrt(7) 8]';
p3=[p1(1,1)*x(1,1),p1(1,2)*x(1,2),p1(1,3)*x(1,3);
    p1(2,1)*x(2,1),p1(2,2)*x(2,2),p1(2,3)*x(2,3)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
if norm(coeff(x.*p1-p3))>10*%eps then pause,end
//
//                complex complex
//
p1=[1+1.5*s;s**2+%i];
//
if norm(coeff(conj(p1')-[p1(1),p1(2)]))>10*%eps then pause,end
if norm(coeff((-p1)+p1))>10*%eps then pause,end
if norm(coeff((-conj(p1'))+conj(p1')))>10*%eps then pause,end
//
x=[1+s**3; 4-%i];
p3=p1(1)*x(1)+p1(2)*x(2);
if norm(coeff(conj(p1')*x-p3))>10*%eps then pause,end
p3=[p1(1)*x(1),p1(1)*x(2);p1(2)*x(1), p1(2)*x(2)];
if norm(coeff(p1*conj(x')-p3))>10*%eps then pause,end
//
x=[1.5+%i/5;1/3];
p3=p1(1)*x(1)+p1(2)*x(2);
if norm(coeff(conj(p1')*x-p3))>10*%eps then pause,end
if norm(coeff(conj(x')*p1-p3))>10*%eps then pause,end
p3=[p1(1)*x(1),p1(1)*x(2);p1(2)*x(1), p1(2)*x(2)];
if norm(coeff(p1*conj(x')-p3))>10*%eps then pause,end
if norm(coeff(x*conj(p1')-conj(p3')))>10*%eps then pause,end
//produit d'un vecteur de polynome par un scalaire
x=4+%i/7;
if norm(coeff(x*p1-[x*p1(1);x*p1(2)]))>10*%eps then pause,end
if norm(coeff(p1*x-[x*p1(1);x*p1(2)]))>10*%eps then pause,end
if norm(coeff(x*conj(p1')-[x*p1(1),x*p1(2)]))>10*%eps then pause,end
if norm(coeff(conj(p1')*x-[x*p1(1),x*p1(2)]))>10*%eps then pause,end
//
x=[1+s**3; 4-%i];
p3=[p1(1)*x(1);p1(2)*x(2)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
x=[1.5+2*%i;1/3];
p3=[p1(1)*x(1);p1(2)*x(2)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
if norm(coeff(x.*p1-p3))>10*%eps then pause,end
x=[1+s**3+%i,4-%i];
p3=[p1(1)*x(1),p1(2)*x(2)];
if norm(coeff(conj(p1').*x-p3))>10*%eps then pause,end
x=[1.5+3*%i,1/3];
p3=[p1(1)*x(1),p1(2)*x(2)];
if norm(coeff(conj(p1').*x-p3))>10*%eps then pause,end
if norm(coeff(x.*conj(p1')-p3))>10*%eps then pause,end

//matrices
p1=[1+1.5*s,s**2, 0;1 s s+1-%i/3];
//
if norm(coeff((-p1)+p1))>10*%eps then pause,end
//
x=[1-%i,s ;-1,2*s;2,0+%i*s**4];
p3=[p1(1,1)*x(1,1)+p1(1,2)*x(2,1)+p1(1,3)*x(3,1),...
        p1(1,1)*x(1,2)+p1(1,2)*x(2,2)+p1(1,3)*x(3,2);
    p1(2,1)*x(1,1)+p1(2,2)*x(2,1)+p1(2,3)*x(3,1),...
        p1(2,1)*x(1,2)+p1(2,2)*x(2,2)+p1(2,3)*x(3,2)];
if norm(coeff(p1*x-p3))>10*%eps then pause,end
//
x=[1 1.5+5*%i;2 1/3;%i-sqrt(7) 8];
p3=[p1(1,1)*x(1,1)+p1(1,2)*x(2,1)+p1(1,3)*x(3,1),...
        p1(1,1)*x(1,2)+p1(1,2)*x(2,2)+p1(1,3)*x(3,2);
    p1(2,1)*x(1,1)+p1(2,2)*x(2,1)+p1(2,3)*x(3,1),...
        p1(2,1)*x(1,2)+p1(2,2)*x(2,2)+p1(2,3)*x(3,2)];
if norm(coeff(p1*x-p3))>10*%eps then pause,end
//
x=5+8*%i;
p3=[x*p1(1,1),x*p1(1,2),x*p1(1,3);
    x*p1(2,1),x*p1(2,2),x*p1(2,3)];

if norm(coeff(p1*x-p3))>10*%eps then pause,end
if norm(coeff(x*p1-p3))>10*%eps then pause,end

//
x=[1+s**3,4+10*%i,-s;0,s+1,10*s**4+%i*s**3];
p3=[p1(1,1)*x(1,1),p1(1,2)*x(1,2),p1(1,3)*x(1,3);
    p1(2,1)*x(2,1),p1(2,2)*x(2,2),p1(2,3)*x(2,3)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
x=[1 1.5;2, 1/3;sqrt(7)/%i, 8]';
p3=[p1(1,1)*x(1,1),p1(1,2)*x(1,2),p1(1,3)*x(1,3);
    p1(2,1)*x(2,1),p1(2,2)*x(2,2),p1(2,3)*x(2,3)];
if norm(coeff(p1.*x-p3))>10*%eps then pause,end
if norm(coeff(x.*p1-p3))>10*%eps then pause,end

//
//
//
//        real real
z=poly(0,'z');p1=[1+3*z;z;2*z;z*4];p2=[0*z 1+z z-1 -z];p3=[p1 p2'];
pp1=p1;pp2=p2;pp3=p3;
pp1(2)=pp2(1);if pp1(2) <> pp2(1) then pause,end
pp2(3)=pp1(4);if pp2(3) <> pp1(4) then pause,end
pp3(2,1)=pp2(1);pp3(3,2)=pp1(4);if pp3 <> [pp1 pp2'] then pause,end
pp1(1:4)=pp2';if pp1<> pp2' then pause,end
pp1((1:4)')=pp2';if pp1<> pp2' then pause,end
pp1=p1;pp2=p2;
pp1(6)=-8;if pp1 <> [p1;0;-8] then pause,end
pp2(7)=+z**3;,if pp2 <> [p2,0,0,+z**3;] then pause,end
pp1=p1;pp2=p2;
pp1(5:8)=p1;if pp1 <> [p1;p1] then pause,end
pp2(6:9)=p2;if pp2 <> [p2 0 p2] then pause,end
pp1=p1;pp2=p2;
pp1(:)=p2';if pp1 <> p2' then pause,end
pp2(:)=p1';if pp2 <> p1' then pause,end
pp1=p1;pp2=p2;
pp1(8:-1:5)=p1;if pp1 <> [p1;p1(4:-1:1)] then pause,end
pp2(8:-1:5)=p2;if pp2 <> [p2 p2(4:-1:1)] then pause,end
pp3=p3;
pp3(1:4,1)=p2';if pp3 <> [p2' p2'] then pause,end
pp3(1:4,2)=p1;if pp3 <> [p2' p1] then pause,end
pp3=p3;
pp3(:,1)=p2';if pp3 <> [p2' p2'] then pause,end
pp3(:,2)=p1;if pp3 <> [p2' p1] then pause,end
pp3(:,:)=p3;if pp3 <> p3 then pause,end
pp3=p3;
pp3(:,4)=p1;if pp3 <> [p3 [0;0;0;0] p1] then pause,end
pp3=p3;
pp3(6:9,:)=p3;if pp3 <> [p3;[0 0];p3] then pause,end
//
p1=[1+3*s;s;2*s;s*4];p2=[0*s-2.5*%i, 1+s, s-1+3*%i ,-s];p3=[p1 conj(p2')];
pp1=p1;pp2=p2;pp3=p3;
pp1(2)=pp2(1);if pp1(2) <> pp2(1) then pause,end
pp2(3)=pp1(4);if pp2(3) <> pp1(4) then pause,end
pp3(2,1)=pp2(1);pp3(3,2)=pp1(4);if pp3 <> [pp1 conj(pp2')] then pause,end
pp1(1:4)=conj(pp2');if pp1<> conj(pp2') then pause,end
pp1((1:4)')=conj(pp2');if pp1<> conj(pp2') then pause,end
pp1=p1;pp2=p2;
pp1(6)=-8;if pp1 <> [p1;0;-8] then pause,end
pp2(7)=+s**3;,if pp2 <> [p2,0,0,+s**3;] then pause,end
pp1=p1;pp2=p2;
pp1(5:8)=p1;if pp1 <> [p1;p1] then pause,end
pp2(6:9)=p2;if pp2 <> [p2 0 p2] then pause,end
pp1=p1;pp2=p2;
pp1(:)=conj(p2');if pp1 <> conj(p2') then pause,end
pp2(:)=p1';if norm(coeff(pp2-p1'))>10*%eps then pause,end
pp1=p1;pp2=p2;
pp1(8:-1:5)=p1;if pp1 <> [p1;p1(4:-1:1)] then pause,end
pp2(8:-1:5)=p2;if pp2 <> [p2 p2(4:-1:1)] then pause,end
pp3=p3;
pp3(1:4,1)=conj(p2');if pp3 <> [conj(p2') conj(p2')] then pause,end
pp3(1:4,2)=p1;if pp3 <> [conj(p2') p1] then pause,end
pp3=p3;
pp3(:,1)=conj(p2');if pp3 <> [conj(p2') conj(p2')] then pause,end
pp3(:,2)=p1;if pp3 <> [conj(p2') p1] then pause,end
pp3(:,:)=p3;if pp3 <> p3 then pause,end
pp3=p3;
pp3(:,4)=p1;if pp3 <> [p3 [0;0;0;0] p1] then pause,end
pp3=p3;
pp3(6:9,:)=p3;if pp3 <> [p3;[0 0];p3] then pause,end

//
p1=[1+3*s;s-%i;2*s;s*4+3*%i*s**2];p2=[0*s 1+s s-1 -s];p3=[p1 p2'];
pp1=p1;pp2=p2;pp3=p3;
pp1(2)=pp2(1);if norm(coeff(pp1(2)-pp2(1)))>10*%eps then pause,end
pp2(3)=pp1(4);if norm(coeff(pp2(3)-pp1(4)))>10*%eps then pause,end
pp3(2,1)=pp2(1);pp3(3,2)=pp1(4);if pp3 <> [pp1 conj(pp2')] then pause,end
pp1(1:4)=pp2';if pp1<> pp2' then pause,end
pp1((1:4)')=pp2';if pp1<> pp2' then pause,end
pp1=p1;pp2=p2;
pp1(6)=-8;if pp1 <> [p1;0;-8] then pause,end
pp2(7)=+s**3;,if pp2 <> [p2,0,0,+s**3] then pause,end
pp1=p1;pp2=p2;
pp1(5:8)=p1;if pp1 <> [p1;p1] then pause,end
pp2(6:9)=p2;if pp2 <> [p2 0 p2] then pause,end
pp1=p1;pp2=p2;
pp1(:)=p2';if norm(coeff(pp1-p2'))>10*%eps then pause,end
pp2(:)=p1';if norm(coeff(pp2-p1'))>10*%eps then pause,end
pp1=p1;pp2=p2;
pp1(8:-1:5)=p1;if pp1 <> [p1;p1(4:-1:1)] then pause,end
pp2(8:-1:5)=p2;if pp2 <> [p2 p2(4:-1:1)] then pause,end
pp3=p3;
pp3(1:4,1)=p2';if norm(coeff(pp3-[p2' p2']))>10*%eps then pause,end
pp3(1:4,2)=p1;if norm(coeff(pp3-[p2' p1]))>10*%eps then pause,end
pp3=p3;
pp3(:,1)=p2';if norm(coeff(pp3-[p2' p2']))>10*%eps then pause,end
pp3(:,2)=p1;if norm(coeff(pp3-[p2' p1]))>10*%eps then pause,end
pp3(:,:)=p3;if pp3 <> p3 then pause,end
pp3=p3;
pp3(:,4)=p1;if pp3 <> [p3 [0;0;0;0] p1] then pause,end
pp3=p3;
pp3(6:9,:)=p3;if pp3 <> [p3;[0 0];p3] then pause,end
p1=[1+3*s;s;2*s;s*4];p2=[0*s-2.5*%i, 1+s, s-1+3*%i ,-s];p3=[p1 conj(p2')];
pp1=p1;pp2=p2;pp3=p3;
pp1(2)=pp2(1);if pp1(2) <> pp2(1) then pause,end
pp2(3)=pp1(4);if pp2(3) <> pp1(4) then pause,end
pp3(2,1)=pp2(1);pp3(3,2)=pp1(4);if pp3 <> [pp1 conj(pp2')] then pause,end
pp1(1:4)=conj(pp2');if pp1<> conj(pp2') then pause,end
pp1((1:4)')=conj(pp2');if pp1<> conj(pp2') then pause,end
pp1=p1;pp2=p2;
pp1(6)=-8;if pp1 <> [p1;0;-8] then pause,end
pp2(7)=+s**3;,if pp2 <> [p2,0,0,+s**3] then pause,end
pp1=p1;pp2=p2;
pp1(5:8)=p1;if pp1 <> [p1;p1] then pause,end
pp2(6:9)=p2;if pp2 <> [p2 0 p2] then pause,end
pp1=p1;pp2=p2;
pp1(:)=conj(p2');if pp1 <> conj(p2') then pause,end
pp2(:)=p1';if norm(coeff(pp2-p1'))>10*%eps then pause,end
pp1=p1;pp2=p2;
pp1(8:-1:5)=p1;if pp1 <> [p1;p1(4:-1:1)] then pause,end
pp2(8:-1:5)=p2;if pp2 <> [p2 p2(4:-1:1)] then pause,end
pp3=p3;
pp3(1:4,1)=conj(p2');if pp3 <> [conj(p2') conj(p2')] then pause,end
pp3(1:4,2)=p1;if pp3 <> [conj(p2') p1] then pause,end
pp3=p3;
pp3(:,1)=conj(p2');if pp3 <> [conj(p2') conj(p2')] then pause,end
pp3(:,2)=p1;if pp3 <> [conj(p2') p1] then pause,end
pp3(:,:)=p3;if pp3 <> p3 then pause,end
pp3=p3;
pp3(:,4)=p1;if pp3 <> [p3 [0;0;0;0] p1] then pause,end
pp3=p3;
pp3(6:9,:)=p3;if pp3 <> [p3;[0 0];p3] then pause,end

//
p1=[1+3*s;s+%i*s**3;2*s;s*4+%i];p2=[0*s-2.5*%i,1+s,s-1+3*%i,-s];
p3=[p1 conj(p2')];
pp1=p1;pp2=p2;pp3=p3;
pp1(2)=pp2(1);if pp1(2) <> pp2(1) then pause,end
pp2(3)=pp1(4);if pp2(3) <> pp1(4) then pause,end
pp3(2,1)=pp2(1);pp3(3,2)=pp1(4);if pp3 <> [pp1 conj(pp2')] then pause,end
pp1(1:4)=conj(pp2');if pp1<> conj(pp2') then pause,end
pp1((1:4)')=conj(pp2');if pp1<> conj(pp2') then pause,end
pp1=p1;pp2=p2;
pp1(6)=-8;if pp1 <> [p1;0;-8] then pause,end
pp2(7)=+s**3;,if pp2 <> [p2,0,0,+s**3] then pause,end
pp1=p1;pp2=p2;
pp1(5:8)=p1;if pp1 <> [p1;p1] then pause,end
pp2(6:9)=p2;if pp2 <> [p2 0 p2] then pause,end
pp1=p1;pp2=p2;
pp1(:)=conj(p2');if pp1 <> conj(p2') then pause,end
pp2(:)=p1';if norm(coeff(pp2-p1'))>10*%eps then pause,end
pp1=p1;pp2=p2;
pp1(8:-1:5)=p1;if pp1 <> [p1;p1(4:-1:1)] then pause,end
pp2(8:-1:5)=p2;if pp2 <> [p2 p2(4:-1:1)] then pause,end
pp3=p3;
pp3(1:4,1)=conj(p2');if pp3 <> [conj(p2') conj(p2')] then pause,end
pp3(1:4,2)=p1;if pp3 <> [conj(p2') p1] then pause,end
pp3=p3;
pp3(:,1)=conj(p2');if pp3 <> [conj(p2') conj(p2')] then pause,end
pp3(:,2)=p1;if pp3 <> [conj(p2') p1] then pause,end
pp3(:,:)=p3;if pp3 <> p3 then pause,end
pp3=p3;
pp3(:,4)=p1;if pp3 <> [p3 [0;0;0;0] p1] then pause,end
pp3=p3;
pp3(6:9,:)=p3;if pp3 <> [p3;[0 0];p3] then pause,end
//
//
//real case
p11=1+3*s;p12=s;p13=2*s;p14=s*4;p1=[p11;p12;p13;p14];
p21=0*s;p22=1+s;p23=s-1;p24=-s;p2=[p21 p22 p23 p24];
p3=[p1 p2'];
if p1(1) <> p11 then pause,end
if p1(4) <> p14 then pause,end
if p2(2) <> p22 then pause,end
if p2(3) <> p23 then pause,end
if p1([1 3]) <> [p11;p13] then pause,end
if p1([1;3]) <> p1([1 3]) then pause,end
if p2([2 4]) <> [p22 p24] then pause,end
if p2([2;4]) <> p2([2 4]) then pause,end
if p1(:) <> p1 then pause,end
if p2(:) <> p2' then pause,end
if p3(1) <> p1(1) then pause,end
if p3(8) <> p2(4) then pause,end
if p3(3) <> p1(3) then pause,end
if p3(5) <> p2(1) then pause,end
if p3(1,1) <> p1(1) then pause,end
if p3(4,2) <> p2(4) then pause,end
if p3(3,2) <> p2(3) then pause,end
if p3([1 3],1) <> p1([1 3]) then pause,end
if p3([1;3],1) <> p1([1 3]) then pause,end
if p3([2 4],[2 1]) <> [p2([2 4])' p1([2 4])] then pause,end
if p3(:,1) <> p1 then pause,end
if p3(:,2) <> p2' then pause,end
if p3(1,:) <> [p1(1) p2(1)] then pause,end
if p3([1 4],:) <> [p1([1 4]) p2([1 4])'] then pause,end
if p3(:,:) <> p3 then pause,end
//
p11=1+3*s+%i;p12=s+%i*s**3;p13=2*s;p14=s*4;p1=[p11;p12;p13;p14];
p21=%i+0*s;p22=1+s;p23=s-1;p24=-s+%i*s**2;p2=[p21 p22 p23 p24];
p3=[p1 conj(p2')];
if p1(1) <> p11 then pause,end
if norm(coeff(p1(4)-p14))>10*%eps then pause,end
if norm(coeff(p2(2)- p22 ))>10*%eps then pause,end
if norm(coeff(p2(3)- p23 ))>10*%eps then pause,end
if norm(coeff(p1([1 3])- [p11;p13] ))>10*%eps then pause,end
if norm(coeff(p1([1;3])- p1([1 3]) ))>10*%eps then pause,end
if norm(coeff(p2([2 4])- [p22 p24] ))>10*%eps then pause,end
if norm(coeff(p2([2;4])- p2([2 4]) ))>10*%eps then pause,end
if norm(coeff(p1(:)- p1 ))>10*%eps then pause,end
if norm(coeff(p2(:)- conj(p2)' ))>10*%eps then pause,end
if norm(coeff(p3(1)- p1(1) ))>10*%eps then pause,end
if norm(coeff(p3(8)- p2(4) ))>10*%eps then pause,end
if norm(coeff(p3(3)- p1(3) ))>10*%eps then pause,end
if norm(coeff(p3(5)- p2(1) ))>10*%eps then pause,end
if norm(coeff(p3(1,1)- p1(1) ))>10*%eps then pause,end
if norm(coeff(p3(4,2)- p2(4) ))>10*%eps then pause,end
if norm(coeff(p3(3,2)- p2(3) ))>10*%eps then pause,end
if norm(coeff(p3([1 3],1)- p1([1 3]) ))>10*%eps then pause,end
if norm(coeff(p3([1;3],1)- p1([1 3]) ))>10*%eps then pause,end
if norm(coeff(p3([2 4],[2 1])-[conj(p2([2 4])') p1([2 4])]))>10*%eps
 then pause,end
if norm(coeff(p3(:,1)- p1 ))>10*%eps then pause,end
if norm(coeff(p3(:,2)- conj(p2') ))>10*%eps then pause,end
if norm(coeff(p3(1,:)- [p1(1) p2(1)] ))>10*%eps then pause,end
if norm(coeff(p3([1 4],:)- [p1([1 4]) conj(p2([1 4])')] ))>10*%eps
  then pause,end
if norm(coeff(p3(:,:)- p3 ))>10*%eps then pause,end


//hidden empty index in insertion
s=poly(0,'s');
a=[1+s 2;3 4];a_ref=a;

a(1,[%f %f])=[];
if or(a<>a_ref) then pause,end
a([%f %f],1)=[];
if or(a<>a_ref) then pause,end
a([%f %f],[%f %f])=[];
if or(a<>a_ref) then pause,end

a = [%s, 2*%s];
a(2) = 1;
assert_checkequal(a, [%s,1]);

-%s;
a=1+%s-%s^2;
b=-a;
assert_checkequal(b,-1-%s+%s^2);

// polynom == double and double == polynom
a = 1;
aa = ones(3,3);
ac = a + %i;
aac = aa + aa * %i;

p = 1 + %s * 0;
pp = ones(3,3) + ones(3,3) * %s * 0;
pc = p + %i;
ppc = pp + pp * %i;

// real
assert_checktrue(a  == p);
assert_checktrue(aa == p);
assert_checktrue(a  == pp);
assert_checktrue(aa == pp);
assert_checktrue(p  == a);
assert_checktrue(pp == a);
assert_checktrue(p  == aa);
assert_checktrue(pp == aa);

// real complex
assert_checkfalse(a  == pc);
assert_checkfalse(aa == pc);
assert_checkfalse(a  == ppc);
assert_checkfalse(aa == ppc);
assert_checkfalse(p  == ac);
assert_checkfalse(pp == ac);
assert_checkfalse(p  == aac);
assert_checkfalse(pp == aac);

// complex real
assert_checkfalse(ac  == p);
assert_checkfalse(aac == p);
assert_checkfalse(ac  == pp);
assert_checkfalse(aac == pp);
assert_checkfalse(pc  == a);
assert_checkfalse(ppc == a);
assert_checkfalse(pc  == aa);
assert_checkfalse(ppc == aa);

//complex
assert_checktrue(ac  == pc);
assert_checktrue(aac == pc);
assert_checktrue(ac  == ppc);
assert_checktrue(aac == ppc);
assert_checktrue(pc  == ac);
assert_checktrue(ppc == ac);
assert_checktrue(pc  == aac);
assert_checktrue(ppc == aac);

// not all equal
// real
aa(2) = 2;
expected = ones(3,3) == 1;
expected(2) = %f;

assert_checkequal(expected, aa  == p);
assert_checkequal(expected, pp  == aa);
assert_checkequal(expected, p   == aa);
assert_checkequal(expected, aa  == pp);
assert_checkequal(expected, pp  == aa);

pp(4) = %s;
pp(6) = 2;
expected = ones(3,3) == 1;
expected(4) = %f;
expected(6) = %f;
expected2 = expected;
expected2(2) = %f;

assert_checkequal(expected, a   == pp);
assert_checkequal(expected, pp  == a);
assert_checkequal(expected2, aa == pp);
assert_checkequal(expected2, pp == aa);

// complex
aac(2) = 2;
expected = ones(3,3) == 1;
expected(2) = %f;

assert_checkequal(expected, aac  == pc);
assert_checkequal(expected, ppc  == aac);
assert_checkequal(expected, pc   == aac);
assert_checkequal(expected, aac  == ppc);
assert_checkequal(expected, ppc  == aac);

ppc(4) = %s;
ppc(6) = 2;
expected = ones(3,3) == 1;
expected(4) = %f;
expected(6) = %f;
expected2 = expected;
expected2(2) = %f;

assert_checkequal(expected, ac   == ppc);
assert_checkequal(expected, ppc  == ac);
assert_checkequal(expected2, aac == ppc);
assert_checkequal(expected2, ppc == aac);
