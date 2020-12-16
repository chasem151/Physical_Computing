#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[]){
	char *fp = argv[1];
	char cmd[400];
	char rd_buf[80];
	char wr_buf[85];

	// For each pattern, run 'grep -e' on the file
	for ( int i = 2; i < argc; i++) {
		strcpy(cmd, "grep -o -e ");
		strcat(cmd, argv[i]);
		strcat(cmd, " ");
		strcat(cmd, fp);
		strcat(cmd, " | wc -w");
        // try snprintf() which may prevent overruns
		//sprintf(cmd, "grep -o -e ", argv[i], " ", fp , " | wc -w");
        // the grep per ftype: -o matches w/ nonzero pieces of lines, -e is patterns kept @ftype
        FILE *rx_pipe;
	    if((rx_pipe = popen(cmd, "r")) == NULL){
	        perror("popen error: ");
	        exit(1);
	    }

	    while(fgets(rd_buf, 80, rx_pipe)) {
	    	sprintf(wr_buf, "%s %s", argv[i], rd_buf);
	        write(1, wr_buf, strlen(wr_buf));
	    }

	    // Close pipe
	    if (pclose(rx_pipe) < 0) {
	        perror("pclose error: ");
	        exit(1);
	    }
		
		pclose(rx_pipe);
	}

	return 0;
}