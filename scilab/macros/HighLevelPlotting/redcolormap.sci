function [c]=redcolormap(N)
c=[(1:N)'/N  zeros(N,1) zeros(N,1)]+graycolormap(N);
c=c/max(c);
