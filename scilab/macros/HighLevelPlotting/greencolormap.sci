function [c]=greencolormap(N)
c=[zeros(N,1) (1:N)'/N zeros(N,1)]+graycolormap(N);
c=c/max(c);
