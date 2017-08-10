// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Francois DELBECQUE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function res=determ(W,k)

    // determinant of a polynomial or rational matrix by FFT
    // W=square polynomial matrix
    // k=``predicted'' degree of the determinant of W i.e. k is
    // an integer larger or equal to the actual degree of W.
    // Method: evaluate the determinant of W for the Fourier frequencies
    // and apply inverse fft to the coefficients of the determinant.
    // See also detr

    if and(typeof(W)<>["rational","polynomial","constant"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A floating point number or polynomial or rational fraction array expected.\n"),"determ",1))
    end
    if size(W,1)<>size(W,2) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"determ",1))
    end

    if W==[] then
        res=1;
        return;
    end;


    n1=size(W,1)

    // small cases

    if n1==1 then
        res=W;
        return;
    elseif n1==2 then
        res = W(1,1)*W(2,2) - W(1,2)*W(2,1);
        return;
    end

    //upper bound of the determinant degree

    maj = n1*max(degree(W))+1;

    if argn(2)==1 then
        k=1;
        while k < maj,
            k=2*k;
        end
    end

    // Default Values
    e=0*ones(k,1);
    e(2)=1;

    // Paramètres de clean
    epsa=1.d-10;
    epsr=0;//no relative rounding

    if k==1 then
        ksi=1;
    else
        ksi=fft(e,-1);
    end

    fi=[];

    if ~isreal(W,0) then
        // Cas Complexe
        for kk=1:k,
            fi=[fi,det(horner(W,ksi(kk)))];
        end
        Temp0 = poly(fft(fi,1),varn(W),"c");
        Temp1 = clean(real(Temp0),epsa,epsr)+%i*clean(imag(Temp0),epsa,epsr);

    else
        // Cas Réel
        for kk=1:k,fi=[fi,det(freq(W,ones(W),ksi(kk)))];end
        Temp1 = clean(real(poly(fft(fi,1),varn(W),"c")),epsa,epsr);
    end

    if argn(2)==1 then

        // Cas où k est défini dans les paramètres d'entrée.
        // On va maintenant annuler tous les coefficients
        // dont le degré est supérieur à maj

        Temp2 = coeff(Temp1);
        for i=1:maj,
            Temp2(i) = 0;
        end
        res = Temp1 - poly(Temp2,varn(W),"coeff");
        return;

    else
        // Cas où k n'est pas défini dans les paramètres d'entrée
        res = Temp1;
        return;
    end

endfunction
