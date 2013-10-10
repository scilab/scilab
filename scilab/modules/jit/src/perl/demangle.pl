#!/usr/bin/perl
# In the generated file, replace the mangled symbols by the classical C++
# symbol

use strict;

while (<>) {
		if(/(.*)(\".*\")(.*)/) {
				my $command = "c++filt -n $2";
				my $k = `$command`;
				chomp $k;
				print "$1\"$k\"$3";
		} else {
				print "$_"
    }
} 

