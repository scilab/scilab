% Test file for function realmin()
% Copyright INRIA
% Matlab version: 7.0.0.19901 (R14)
% V.C.

realmin;
realmin('double');
realmin('single');
typ='double';
realmin(typ);

val=realmin;
val=realmin('double');
val=realmin('single');
val=realmin(typ);