//  Scicos
//
//  Copyright (C) INRIA - Author : EADS-CCR
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function x=mulfstring(a,b)
    if ( a == [] | b == []) then
        x=[];
        return;
    end
    [l,m]=size(a);
    [m,n]=size(b);
    x=[];
    for j=1:n,
        y=[];
        for i=1:l,
            t=" ";
            for k=1:m,
                if (k > 1) then
                    //t=t+'+('+a(i,k)+')*'+'('+b(k,j)+')';
                    t=addfv(t,mulfv(a(i,k),b(k,j)));
                else
                    //t='(' + a(i,k) +')*' + '(' + b(k,j) + ')';
                    t=mulfv(a(i,k),b(k,j));
                end
            end
            y=[y;t];
        end
        x=[x y];
    end
endfunction
