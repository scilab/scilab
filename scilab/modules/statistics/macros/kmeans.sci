// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function [C, centers] = kmeans(X, k)

    arguments
        X {mustBeA(X, "double")}
        k (1,1) {mustBeA(k, "double"), mustBeInteger, mustBePositive}
    end

	if k > size(X, 1) then
		error(msprintf(_("%s: Wrong value for input argument #%d: Must be less than or equal to %d.\n"), "kmeans", 2, size(X, 1)));
	end

	maxiter = 100;
	tol = 10^-6;

	// init class centers (sampling without replacement)
	n = size(X, 1);
	index = grand(1, "prm", 1:n);
	index = index(1:k);
	centers = X(index, :);

	// initial assignment
	D = pdist2(X, centers);
	[tmp, C] = min(D, "c");

	for t = 1:maxiter
		// update step: re-computation of centers
		newcenters = centers;
		for kk = 1:k
			idx_kk = find(C == kk);
			if ~isempty(idx_kk) then
				newcenters(kk, :) = mean(X(idx_kk, :), 1);
			end
		end

		// convergence check
		cv = sum((newcenters - centers).^2);
		centers = newcenters;
		if cv <= tol then
			break
		end

		// assignment step
		D = pdist2(X, centers);
		[tmp, C] = min(D, "c");
	end

	D = pdist2(X, centers);
	[tmp, C] = min(D, "c");
end
