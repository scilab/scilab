function [c]=bluecolormap(N)
c=[zeros(N,1) zeros(N,1) (1:N)'/N]+graycolormap(N);
c=c/max(c);
