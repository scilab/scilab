//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//                      - Fady Nassif <fady.nassif@inria.fr>
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

function [ok,bllst]=adjust(bllst,inpptr,outptr,inplnk,outlnk)
    //
    //adjust : a piece of the adjust_inout function.
    //         Only one pass -see c_pass2.sci-
    //
    //
    //in parameters  : bllst  : list of blocks
    //                 inpptr : vector of regular input port number
    //                          (of size nblk+1)
    //                 outptr : vector of regular output port number
    //                          (of size nblk+1)
    //                 inplnk : vector of link number connected to regular input port
    //                          (of size max(inpptr)-1)
    //                 outlnk : vector of link number connected to regular output port
    //                          (of size max(outptr)-1)
    //
    //out parameters : ok : a boolean flag to known if adjust_inout have
    //                     succeeded to resolve the in/out port size
    //                     - ok = %t : all size have been resolved in bllst
    //                     - ok = %f : problem in size adjustement
    //                 bllst : modified list of blocks
    //
    //18/05/06, Alan  : improvement in order to take into
    //account two dimensional ports size.
    //
    //28/12/06, Alan : type for source port and target port must
    //                 be the same.
    //
    //04/01/07, Fady :Can test the case of negatives equals target's dimensions.
    //
    //10/05/07, Alan : - if-then-else event-select case



    //Adjust in2/out2, inttyp/outtyp
    //in accordance to in/out in bllst
    [ko,bllst]=adjust_in2out2(bllst);
    if ~ko then ok=%f,return, end //if adjust_in2out2 failed then exit
    //adjust_inout with flag ok=%f
    //
    [outoin,outoinptr]=connmat(inpptr,outptr,inplnk,outlnk)
    for i=1:length(bllst)
        if size(bllst(i).in,1)<>size(bllst(i).intyp,2) then
            bllst(i).intyp=bllst(i).intyp(1)*ones(size(bllst(i).in,1),1);
        end
        if size(bllst(i).out,1)<>size(bllst(i).outtyp,2) then
            bllst(i).outtyp=bllst(i).outtyp(1)*ones(size(bllst(i).out,1),1);
        end
    end
    //loop on number of block
    for hh=1:length(bllst)
        ok=%t
        for blkout=1:length(bllst) //second loop on number of block
            for portout=1:outptr(blkout+1)-outptr(blkout) //loop on number of regular
                //output port
                jj=outptr(blkout)+portout-1 //get current number of output port

                if [outoinptr(jj):outoinptr(jj+1)-1]<>[] then
                    for kk=outoinptr(jj):outoinptr(jj+1)-1 //loop on
                        blkin=outoin(kk,1)  //
                        portin=outoin(kk,2) //

                        //nnin/nnout are the size (two dimensions) of the
                        //target port and the source port of the observed link
                        //before adjust
                        nnout(1,1)=bllst(blkout).out(portout)
                        nnout(1,2)=bllst(blkout).out2(portout)
                        nnin(1,1)=bllst(blkin).in(portin)
                        nnin(1,2)=bllst(blkin).in2(portin)
                        //intyp/outtyp are the type of the
                        //target port and the source port of the observed link
                        intyp=bllst(blkin).intyp(portin)
                        outtyp=bllst(blkout).outtyp(portout)

                        //check intyp outtyp
                        if(intyp>0 & outtyp>0) then
                            if intyp<>outtyp then
                                if (intyp==1 & outtyp==2) then
                                    bllst(blkin).intyp(portin)=2;
                                elseif (intyp==2 & outtyp==1) then
                                    bllst(blkout).outtyp(portout)=2;
                                else
                                    bad_connection(corinv(blkout),portout,..
                                    nnout,outtyp,..
                                    corinv(blkin),portin,..
                                    nnin,intyp,1)
                                    ok=%f;
                                    return
                                end
                            end
                        elseif(outtyp>0&intyp<0) then
                            ww=find(bllst(blkin).intyp==intyp)
                            bllst(blkin).intyp(ww)=outtyp
                            ww=find(bllst(blkin).outtyp==intyp)
                            bllst(blkin).outtyp(ww)=outtyp
                        elseif(outtyp<0&intyp>0) then
                            ww=find(bllst(blkout).outtyp==outtyp)
                            bllst(blkout).outtyp(ww)=intyp
                            ww=find(bllst(blkout).intyp==outtyp)
                            bllst(blkout).intyp(ww)=intyp
                        else
                            ok=%f
                        end

                        //loop on the two dimensions of source/target port
                        for ndim=1:2
                            //nin/nout are the size (two dimensions) of the
                            //target port and the source port of the observed link
                            nout(1,1)=bllst(blkout).out(portout)
                            nout(1,2)=bllst(blkout).out2(portout)
                            nin(1,1)=bllst(blkin).in(portin)
                            nin(1,2)=bllst(blkin).in2(portin)

                            //first case : dimension of source and
                            //             target ports are explicitly informed
                            //             informed with positive size
                            if (nout(1,ndim)>0&nin(1,ndim)>0) then
                                //if dimension of source and target port doesn't match
                                //then call bad_connection, set flag ok to false and exit
                                if nin(1,ndim)<>nout(1,ndim) then
                                    bad_connection(corinv(blkout),portout,..
                                    nnout,outtyp,..
                                    corinv(blkin),portin,nnin,intyp)
                                    ok=%f;return
                                end

                                //second case : dimension of source port is
                                //              positive and dimension of
                                //              target port is negative
                            elseif (nout(1,ndim)>0&nin(1,ndim)<0) then
                                //find vector of input ports of target block with
                                //first/second dimension equal to size nin(1,ndim)
                                //and assign it to nout(1,ndim)
                                ww=find(bllst(blkin).in==nin(1,ndim))
                                if ww<>[] then
                                    bllst(blkin).in(ww)=nout(1,ndim)
                                end
                                ww=find(bllst(blkin).in2==nin(1,ndim))
                                if ww<>[] then
                                    bllst(blkin).in2(ww)=nout(1,ndim)
                                end
                                //find vector of output ports of target block with
                                //first/second dimension equal to size nin(1,ndim)
                                //and assign it to nout(1,ndim)
                                ww=find(bllst(blkin).out==nin(1,ndim))
                                if ww<>[] then
                                    bllst(blkin).out(ww)=nout(1,ndim)
                                end
                                ww=find(bllst(blkin).out2==nin(1,ndim))
                                if ww<>[] then
                                    bllst(blkin).out2(ww)=nout(1,ndim)
                                end


                                //third case : dimension of source port is
                                //             negative and dimension of
                                //             target port is positive
                            elseif (nout(1,ndim)<0&nin(1,ndim)>0) then
                                //find vector of output ports of source block with
                                //first/second dimension equal to size nout(1,ndim)
                                //and assign it to nin(1,ndim)
                                ww=find(bllst(blkout).out==nout(1,ndim))
                                if ww<>[] then
                                    bllst(blkout).out(ww)=nin(1,ndim)
                                end
                                ww=find(bllst(blkout).out2==nout(1,ndim))
                                if ww<>[] then
                                    bllst(blkout).out2(ww)=nin(1,ndim)
                                end
                                //find vector of input ports of source block with
                                //first/second dimension equal to size nout(1,ndim)
                                //and assign it to nin(1,ndim)
                                ww=find(bllst(blkout).in==nout(1,ndim))
                                if ww<>[] then
                                    bllst(blkout).in(ww)=nin(1,ndim)
                                end
                                ww=find(bllst(blkout).in2==nout(1,ndim))
                                if ww<>[] then
                                    bllst(blkout).in2(ww)=nin(1,ndim)
                                end


                                //fourth case : a dimension of source port is
                                //              null
                            elseif (nout(1,ndim)==0) then
                                //set ww to be the vector of size of the ndim
                                //dimension of input port of the source block
                                if ndim==1 then
                                    ww=bllst(blkout).in(:)
                                elseif ndim==2 then
                                    ww=bllst(blkout).in2(:)
                                end

                                //test if all size of the ndim dimension of input
                                //port of the source block is positive
                                if min(ww)>0 then

                                    //test if the dimension of the target port
                                    //is positive
                                    if nin(1,ndim)>0 then

                                        //if the sum of the size of the ndim dimension of the input
                                        //port of the source block is equal to the size of the ndim dimension
                                        //of the target port, then the size of the ndim dimension of the source
                                        //port is equal to the size of ndim dimension of the target port
                                        if sum(ww)==nin(1,ndim) then
                                            if ndim==1 then
                                                bllst(blkout).out(portout)=nin(1,ndim)
                                            elseif ndim==2 then
                                                bllst(blkout).out2(portout)=nin(1,ndim)
                                            end
                                            //else call bad_connection, set flag ok to false and exit
                                        else
                                            bad_connection(corinv(blkout),0,0,1,-1,0,0,1)
                                            ok=%f;return
                                        end

                                        //if the ndim dimension of the target port is negative
                                        //then the size of the ndim dimension of the source port
                                        //is equal to the sum of the size of the ndim dimension
                                        //of input ports of source block, and flag ok is set to false
                                    else
                                        if ndim==1 then
                                            bllst(blkout).out(portout)=sum(ww)
                                        elseif ndim==2 then
                                            bllst(blkout).out2(portout)=sum(ww)
                                        end
                                        ok=%f
                                    end

                                else
                                    //set nww to be the vector of all negative size of input ports
                                    //of the source block
                                    nww=ww(find(ww<0))

                                    //if all negative size have same size and if size of the
                                    //ndim dimension of target port is positive then assign
                                    //size of the ndim dimension of the source port to nin(1,ndim)
                                    if norm(nww-nww(1),1)==0 & nin(1,ndim)>0 then
                                        if ndim==1 then
                                            bllst(blkout).out(portout)=nin(1,ndim)
                                        elseif ndim==2 then
                                            bllst(blkout).out2(portout)=nin(1,ndim)
                                        end

                                        //compute a size to be the difference between the size
                                        //of the ndim dimension of target block and sum of positive
                                        //size of input ports of source block divided by the number
                                        //of input ports of source block with same negative size
                                        k=(nin(1,ndim)-sum(ww(find(ww>0))))/size(nww,"*")

                                        //if this size is a positive integer then assign it
                                        //to the size of the ndim dimension of input ports of the
                                        //source block which have negative size
                                        if k==int(k)&k>0 then
                                            if ndim==1 then
                                                bllst(blkout).in(find(ww<0))=k
                                            elseif ndim==2 then
                                                bllst(blkout).in2(find(ww<0))=k
                                            end

                                            //else call bad_connection, set flag ok to false and exit
                                        else
                                            bad_connection(corinv(blkout),0,0,1,-1,0,0,1)
                                            ok=%f;return
                                        end

                                        //set flag ok to false
                                    else
                                        ok=%f
                                    end

                                end

                                //fifth case : a dimension of target port is
                                //             null
                            elseif (nin(1,ndim)==0) then
                                //set ww to be the vector of size of the ndim
                                //dimension of output port of the target block
                                if ndim==1 then
                                    ww=bllst(blkin).out(:)
                                elseif ndim==2 then
                                    ww=bllst(blkin).out2(:)
                                end

                                //test if all size of the ndim dimension of output
                                //port of the target block is positive
                                if min(ww)>0 then
                                    //test if the dimension of the source port
                                    //is positive
                                    if nout(1,ndim)>0 then

                                        //if the sum of the size of the ndim dimension of the output
                                        //port of the target block is equal to the size of the ndim dimension
                                        //of the source port, then the size of the ndim dimension of the target
                                        //port is equal to nout(1,ndim)
                                        if sum(ww)==nout(1,ndim) then
                                            if ndim==1 then
                                                bllst(blkin).in(portin)=nout(1,ndim)
                                            elseif ndim==1 then
                                                bllst(blkin).in2(portin)=nout(1,ndim)
                                            end
                                            //else call bad_connection, set flag ok to false and exit
                                        else
                                            bad_connection(corinv(blkin),0,0,1,-1,0,0,1)
                                            ok=%f;return
                                        end
                                    else
                                        //if the ndim dimension of the source port is negative
                                        //then the size of the ndim dimension of the target port
                                        //is equal to the sum of the size of the ndim dimension
                                        //of output ports of target block, and flag ok is set to false
                                        if ndim==1 then
                                            bllst(blkin).in(portin)=sum(ww)
                                        elseif ndim==2 then
                                            bllst(blkin).in2(portin)=sum(ww)
                                        end
                                        ok=%f
                                    end

                                else
                                    //set nww to be the vector of all negative size of output ports
                                    //of the target block
                                    nww=ww(find(ww<0))

                                    //if all negative size have same size and if size of the
                                    //ndim dimension of the source port is positive then assign
                                    //size of the ndim dimension of the target port to nout(1,ndim)
                                    if norm(nww-nww(1),1)==0 & nout(1,ndim)>0 then
                                        if ndim==1 then
                                            bllst(blkin).in(portin)=nout(1,ndim)
                                        elseif ndim==2 then
                                            bllst(blkin).in2(portin)=nout(1,ndim)
                                        end

                                        //compute a size to be the difference between the size
                                        //of the ndim dimension of the source block and sum of positive
                                        //size of output ports of the target block divided by the number
                                        //of output ports of target block with same negative size
                                        k=(nout(1,ndim)-sum(ww(find(ww>0))))/size(nww,"*")

                                        //if this size is a positive integer then assign it
                                        //to the size of the ndim dimension of output ports of the
                                        //target block which have negative size
                                        if k==int(k)&k>0 then
                                            if ndim==1 then
                                                bllst(blkin).out(find(ww<0))=k
                                            elseif ndim==2 then
                                                bllst(blkin).out2(find(ww<0))=k
                                            end

                                            //else call bad_connection, set flag ok to false and exit
                                        else
                                            bad_connection(corinv(blkin),0,0,1,-1,0,0,1)
                                            ok=%f;return
                                        end

                                        //set flag ok to false
                                    else
                                        ok=%f
                                    end
                                end

                                //sixth (& last) case : dimension of both source
                                //                      and target port are negatives
                            else
                                ok=%f  //set flag ok to false
                            end

                        end
                    end
                else
                    //there is no target port
                    nout(1,1)=bllst(blkout).out(portout)
                    nout(1,2)=bllst(blkout).out2(portout)
                    //loop on the two dimensions of source port
                    for ndim=1:2
                        //a dimension of the source port is
                        //             null
                        if (nout(1,ndim)==0) then
                            if ndim==1 then
                                ww=bllst(blkout).in(:)
                                if min(ww)>0 then
                                    bllst(blkout).out(portout)=sum(ww)
                                    ok=%f
                                end
                            elseif ndim==2 then
                                ww=bllst(blkout).in2(:)
                                if min(ww)>0 then
                                    bllst(blkout).out2(portout)=sum(ww)
                                    ok=%f
                                end
                            end
                        end
                    end
                end
            end
        end

        if ok then return, end //if ok then exit adjust
    end
    //if failed then display message and set flag ok to false
    messagebox(["Not enough information to determine port sizes";
    "Compile the diagram before running."],"modal");
    ok=%f

endfunction

//connmat :
//
//in parameters  : inpptr : vector of regular input port number
//                          (of size nblk+1)
//                 outptr : vector of regular output port number
//                          (of size nblk+1)
//                 inplnk : vector of link number connected to regular input port
//                          (of size max(inpptr)-1)
//                 outlnk : vector of link number connected to regular output port
//                          (of size max(outptr)-1)
//
//out parameters : outoin :
//
//                 outoinptr :
//
function [outoin,outoinptr]=connmat(inpptr,outptr,inplnk,outlnk)
    outoin=[];outoinptr=1
    for k=1:outptr($)-1
        ii=[]
        for j=outlnk(k)
            ii=[ii;find(inplnk==j)]
        end
        outoini=[];jj=0
        for j=ii
            //         m=int32(max(find(inpptr<=int32(j))))
            m=max(find(inpptr<=j))
            n=j-inpptr(m)+1
            outoini=[outoini;[m,n]]
            jj=jj+1
        end
        outoinptr=[outoinptr;outoinptr($)+jj]
        outoin=[outoin;outoini]
    end
endfunction
