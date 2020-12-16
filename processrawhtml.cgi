#!/usr/bin/perl --

# Raw data (input) processed into HTML format

# Script: Counts 10 numbers from a specified starting numbers and displays the
# 		  the values in a specified font-size
# if the input is /processrawhtml.cgi/?var1=4&var2=50&var3=20 
# -- count var3 times from var2 outputting in HTML size var1

# ARGV[0] = html text size (1-7) 
# ARGV[1] = starting index
# ARGV[2] = max num of values

print "Content-type: text/html\n\n";
print "<!DOCTYPE html>";
print "<html><body>";
print "<h1> Count $ARGV[2] digits from input value $ARGV[1]: </h1>";
print "<h2> (Using font size $ARGV[0]) </h2>";
print "<font size=\"$ARGV[0]\">";
for (my $i = $ARGV[1]; $i < $ARGV[1]+$ARGV[2]; $i++) {
	print "$i<br>";
}
print "</font>";
print "</body></html>";