#!/usr/bin/perl
my $MAX = 5000;

if(@ARGV == 1)
{
    $MAX = $ARGV[0];
}

for( $i = 1 ; $i <= $MAX ; $i++)
{
    print "v".$i."=".$i;
    print "\n";
}
