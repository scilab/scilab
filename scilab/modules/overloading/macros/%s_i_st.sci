// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function out=%s_i_st(varargin)

    // - Modified by Vincent COUVERT (16/08/2004) so that insertion of an empty matrix
    //   is understood as an element deletion
    //   Only one non-colon index can be used
    // - Modified by Serge Steer INRIA (04/05/2010) to fix problems in element
    //   deletion part
    // varargin: (1)=i, (2)=j, (3)=value, (4)=st  st(i,j)=value

    if size(varargin)>=3 & isempty(varargin($-1)) & ..
        and(type(varargin(1))<>[10 15]) then
        // st(i,:)=[] or st(:,j)=[] or st(i)=[] or st(:,j,:,:)=[]
        //remove the substruct
        out=varargin($);
        dims=size(out)

        // Make the dimensions and the indices fit
        Ndims=size(dims,"*")
        nindex=size(varargin)-2
        if nindex>Ndims then
            //index in excess must be equal to 1 or to :
            for k=Ndims+1:nindex
                i=varargin(k)
                if size(i,"*")>1|(i<>1&i<>eye()) then
                    msg = _("%s: A null assignment can have only one non-colon index.\n")
                    error(msprintf(msg, "%s_i_st"));
                end
            end
            nindex=Ndims
        elseif nindex<Ndims then
            //collapse dimensions in excess
            dims=[dims(1:nindex-1) prod(dims(nindex:$))]
            Ndims=nindex;
            if size(dims,"*")==1 then dims=[dims 1],end
        end

        // Check the compatibility of the index (at most one index cannot span
        // all the elements ot the associated struct dimension)
        cj=[];
        for k=1:nindex
            ind=varargin(k)
            if type(ind)==2 then // polynomial (like $)
                ind = horner(ind,dims(k));
            elseif typeof(ind)=="implicitlist"
                if ind(1)==1 & ind(2)==1 & ind(3)==$
                    ind = eye()
                else
                    a = horner([ind(1) ind(2) ind(3)],dims(k));
                    execstr("ind=a(1):a(2):a(3)");
                end
            end
            if or(size(ind)<>[-1 -1]) then
                ind=floor(ind);
                //check if index is valid
                if ~isreal(ind)|or(ind<=0) then
                    msg = _("%s: Invalid index\n");
                    error(msprintf(msg, "%s_i_st"))
                end
                //remove indices that exceed the associated struct dimension
                ind(ind>dims(k))=[];
                //compute the complement with respect to the associated dimension of st
                ind=setdiff(1:dims(k),ind)
                if ind<>[]&cj==[] then
                    cj=ind
                    loc=k,
                else
                    msg = _("%s: A null assignment can have only one non-colon index.\n")
                    error(msprintf(msg, "%s_i_st"));
                end
            end
        end

        // Generate the result
        if cj==[] then  //st(:,:)=[]  --> empty struct
            Fout=getfield(1,out)
            Fout=Fout(3:$)
            for f=Fout
                out(f)=list()
            end
        else
            //replace st(:,j,:,:)=[] by st=st(:,cj,:,:) where cj is the
            //complement of j with respect to the associated dimension of st
            varargin(loc)=cj
            out=out(varargin(1:Ndims))
        end

    elseif size(varargin)==3 & type(varargin(1))==10 then // out.i=in
        i=varargin(1);
        in=varargin(2);
        out=varargin(3);
        out=generic_i_st(i,in,out)
    else
        error(msprintf(_("%s: Not yet implemented.\n"),"%s_i_st"));
    end
endfunction
