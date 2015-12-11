//
// This file is part of the Xcos skeleton toolbox
//
// see license.txt for more licensing information

package XcosToolboxSkeleton

  model Sum "Perfom the summation of the two inputs"
    Real in1 "the first input";
    Real in2 "the second input";
    Real out "the output" ;
  equation
    out = in1 + in2;
  end Sum;

end XcosToolboxSkeleton;


