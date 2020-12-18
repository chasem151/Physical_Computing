#!/usr/bin/perl --

# Run gnuplot and read input from filehandler
open my $PROGRAM, '|-', 'gnuplot'
    or die "Error opening gnuplot: $!";

# Print cmds into gnuplot input
say {$PROGRAM} "set terminal jpeg";
say {$PROGRAM} "set output 'histogram.jpg";
say {$PROGRAM} "set autoscale y";
say {$PROGRAM} "set bar 1.0 front";
say {$PROGRAM} "set style fill solid";
say {$PROGRAM} "set boxwidth 0.75 absolute";
say {$PROGRAM} "set style histogram gap 1.0";
say {$PROGRAM} "set style data histograms";
#say {$PROGRAM} "set xtics border scale 1,0 nomirror autojustify norangelimit";
#say {$PROGRAM} "unset ytics";
say {$PROGRAM} "set title \"Frequency per file type in $ARGV[0]\"";
say {$PROGRAM} "plot '-' using 2:xtic(1)";

# Run my-histogram w/local...made(by make) executable
open my $read_from_process, "-|", "./my-histogram", @ARGV;
while (my $line = <$read_from_process>) {
	say {$PROGRAM} $line;
}
say ${PROGRAM} "e\n";
close $PROGRAM;

# pretty output
print "Content-type: text/html\n\n";
print "<!DOCTYPE html>";
print "<html><body>";
print "<h1 style=\"color:red;\" align=\"center\"> CS410 Webserver </h1>";
print "<br>";
# print "<center><img src=\"notes.jpg\" align=\"middle\"></center>";
print "<center><img src=\"./plotresult.jpeg\" align=\"middle\"></center>";
print "</body></html>";