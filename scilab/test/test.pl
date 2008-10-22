#!usr/bin/perl
my $a = "one";
my $b = "two";
my $c = "three";
my $d = "four";

sub3($a, $b, $c, $d);
print join(" ", $a, $b, $c, $d), "\n";

sub sub1 {
  my ($var1, $var2, $var3, $var4) = @_;
  return sub2($var1, $var2, $var3, $var4);
}

sub sub2 {
  my $var1 = shift;
  my $var2 = shift;
  my $var3 = shift;
  my $var4 = shift;
  return  sub3($var2, $var1, $var4, $var3);
}

sub sub3 {
  $_[0] = "i";
  $_[1] = "think";
  $_[2] = "therefore";
  $_[3] = "i am";
}