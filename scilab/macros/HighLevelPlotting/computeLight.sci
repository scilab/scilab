function [l]=computeLight(n,vect)

l=n'*vect;
l=l.*(l>0);
