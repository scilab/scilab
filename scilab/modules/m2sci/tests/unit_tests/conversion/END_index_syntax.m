% end in comments
% a(end:-1:2) in comments
a = [1 2 3 4 5 6 7];
k = 3
endy = 4;
b.end = 3
if true, a=1, end, b=3
if true then
    % With matrices
    % -------------
    a(b.end:end) = 1
    a(append) = 8
    a(endy:end) = 8
    a(1:endy:end) = 3

    a(end) = 7
    a(6:end) = [3 4]
    a(1,2:end) = [3 4]
    a(end,2:end) = [3 4]
    a(end,2:end-1) = 3
    a(end+1) = 8
    a(end-k) = 7
    a(6:end) = [3 4], a(end) = 5
    endy(3) = 4
    endy(end/2) = 7
    append(end:-1:2) = 1:endy

    a (b.end : end) = 1
    a (append ) = 8
    a (endy: end ) = 8
    a (1: endy: end) = 3

    a ( end ) = 7
    a (6 : end ) = [3 4]
    a (1, 2 : end ) = [3 4]
    a ( end , 2 : end ) = [3 4]
    a ( end , 2 : end - 1) = 3
    a ( end + 1) = 8
    a ( end - k) = 7
    a ( 6 : end ) = [3 4], a( end ) = 5
    endy (3) = 4
    endy ( end / 2) = 7
    append ( end : -1 : 2 ) = 1:endy

    % With cells
    % ----------
    a{b.end:end} = 1
    a{append} = 8
    a{endy:end} = 8
    a{1:endy:end} = 3

    a{end} = 7
    a{6:end} = [3 4]
    a{1,2:end} = [3 4]
    a{end,2:end} = [3 4]
    a{end,2:end-1} = 3
    a{end+1} = 8
    a{end-k} = 7
    a{6:end} = [3 4], a{end} = 5
    endy{3} = 4
    endy{end/2} = 7
    append{end:-1:2} = 1:endy

    a {b.end : end} = 1
    a {append } = 8
    a {endy: end } = 8
    a {1: endy: end} = 3

    a { end } = 7
    a {6 : end } = [3 4]
    a {1, 2 : end } = [3 4]
    a { end , 2 : end } = [3 4]
    a { end , 2 : end - 1} = 3
    a { end + 1} = 8
    a { end - k} = 7
    a { 6 : end } = [3 4], a{ end } = 5
    endy {3} = 4
    endy { end / 2} = 7
    append { end : -1 : 2 } = 1:endy

 end
