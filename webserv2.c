#define _GNU_SOURCE
#define SIZE 20
#include <stdbool.h>
// std streams
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// unix specs
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
// processing
#include <dirent.h>
#include <sched.h>
#include <signal.h>
//networking / IPC
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
// custom includes
struct DataItem {
   int data;   
   int key;
};
struct DataItem* hashArray[SIZE]; 
struct DataItem* dummyItem;
struct DataItem* item;
/*
#ifndef PORT
#define PORT 8080
#endif
*/ 
//instead- INADDR_ANY dynamically to support this server's mobility

// see setsockopt(), getsockopt() to set or know the rx buffer's size

//static int const  ERR_NOTFOUND = 404;
//static int const  ERR_NOTIMPL = 501;
int hashCode(int key) {
   return key % SIZE;
}

struct DataItem *search(int key) {
   //get the hash 
   int hashIndex = hashCode(key);  
	
   //move in array until an empty 
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->key == key)
         return hashArray[hashIndex]; 
			
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SIZE;
   }        
	
   return NULL;        
}

void insert(int key,int data) {

   struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
   item->data = data;  
   item->key = key;

   //get the hash 
   int hashIndex = hashCode(key);

   //move in array until an empty or deleted cell
   while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SIZE;
   }
	
   hashArray[hashIndex] = item;
}

struct DataItem* delete(struct DataItem* item) {
   int key = item->key;

   //get the hash 
   int hashIndex = hashCode(key);

   //move in array until an empty
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->key == key) {
         struct DataItem* temp = hashArray[hashIndex]; 
			
         //assign a dummy item at deleted position
         hashArray[hashIndex] = dummyItem; 
         return temp;
      }
		
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SIZE;
   }      
	
   return NULL;        
}

void display() {
   int i = 0;
	
   for(i = 0; i<SIZE; i++) {
	
      if(hashArray[i] != NULL)
         printf(" (%d,%d)",hashArray[i]->key,hashArray[i]->data);
      else
         printf(" ~~ ");
   }
	
   printf("\n");
}

int server(char *num){
    int portNum, sockNum;
    struct sockaddr_in serverAddr;

    portNum = atoi(num);

    // Open new socket
    if((sockNum = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "Error: Could not open socket.\n");
        exit(1);
    }

    // Set up socket address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(portNum);
    
    int size = sizeof(serverAddr);
    
    // Bind socket (socketNum) to address serverAddr
    if(bind(sockNum, (struct sockaddr *) &serverAddr, size) == -1){
        perror("Error: if.\n");
        exit(1);
    }
    return sockNum;
}

/* void correct_remote_address(FILE *fp){ // see man for: recv()
    char buf[BUFSIZ]; // max stream (MACRO-BUFSIZ-stdio.h)
    while(fgets(buf,BUFSIZ,fp)!= NULL && strcmp(buf,"\r\n") !=0);
} */

int wont_stat(char *ptr){ // HTML 404: "Not Found"
    struct stat info;
    return(stat(ptr,&info) == -1);
}

int dir_extension(char *ptr){
    struct stat info;
    return( stat(ptr,&info) != -1 && S_ISDIR(info.st_mode));
}

char *file_extension(char *ptr){ // to navigate through the .ext
    char *character;
    if((character = strrchr(ptr,'.')) != NULL) // test 1st occurence '.'
        return character+1; // check the next char... recursive ? no clue lol
    return "";
}

char *extension(char * request){ // returns .xxx
    if(strchr(request, '.')){
        char *ext = strchr(request,'.');
        return ext;
    }
    return request;
}

int approve_request(int socket_fd, char* request){
    if(strlen(request) == 0){
        request = ".";
    }
    if(access(request, F_OK) != -1){
        return 1;
    }
    else{
        struct stat info;
        if(lstat(request,&info) < 0){
            fprintf(stderr, "stat error for %s.\n", request);
        }
        if(S_ISDIR(info.st_mode) > 0) return 1;

        //http_error(socket_fd) ?? -- idk
    }
}

// Output error message depending on errorno and exit
void http_error(int errornum, int client, char *ftype){ // could be: int socket_fd as only input args -- write calls should be write(socket_fd, ...)
    char *buf = malloc(512);
   
    switch(errornum){
    // Not Found
    case 404: // see wont_stat() ^^
        sprintf(buf, "HTTP/1.1  404 NOT FOUND\r\n");   
        write(client, buf, strlen(buf));
        memmove(ftype, ftype+1, strlen(ftype));
        //printf("\n%s\n", ftype);
        if(!strcmp(ftype, "html") || !strcmp(ftype, "txt")) {
            //printf("\nhere %s\n", ftype);
            sprintf(buf, "Content-Type: text/%s\r\n\r\n", ftype);
        }
        if(!strcmp(ftype, "jpeg") || !strcmp(ftype, "jpg") || !strcmp(ftype, "gif")) {
            //printf("\nhere2 %s\n", ftype);
            sprintf(buf, "Content-Type: text/%s\r\n\r\n", ftype);
        }
        //sprintf(buf, "Content-Type: text/html\r\n\r\n");
        write(client, buf, strlen(buf));
        sprintf(buf, "404 Not Found\r\n\r\n");
        write(client, buf, strlen(buf));
        sprintf(buf, "The specified URL could not be found.");
        write(client, buf, strlen(buf));
        break;
    // Not Implemented
    case 501:
        sprintf(buf, "HTTP/1.1 501 NOT IMPLEMENTED\r\n");
        write(client, buf, strlen(buf));
        sprintf(buf, "Content-Type: text/html\r\n\r\n");
        write(client, buf, strlen(buf));
        sprintf(buf, "501 Not Implemented\r\n");
        write(client, buf, strlen(buf));
        sprintf(buf, "The server does not support the functionality");
        write(client, buf, strlen(buf));
        sprintf(buf, " required to fullfill the request.\r\n");
        write(client, buf, strlen(buf));
        break;

    default:
        fprintf(stderr, "Could not find error message.\n");
        break;
    }
    
    close(client);  
    exit(0);
}

// Handle a request by the client
int getRequest(void* request){ // , int socket_fd
    int client = *((int *)request);
    char buffer[1024];
    char filePath[256];
    //char fileType[20];
    char input[256];
    //char *inputs[2];
    char output[4096];
    int re;
    char ptr[1024];

    FILE *file, *fp_sd, *new_file;
    re = read(client, buffer, 1024);
    
    if( re <= 0){
        fprintf(stderr, "Could not read from browser.\n");
        close(client);
        exit(1);
    }else if(re < 1024){
        buffer[re] = 0;
    }
    
    // Server only handles get requests
    if(strncmp(buffer, "GET ", 4) != 0 && strncmp(buffer, "get ", 4) != 0){
        http_error(501, client, "none");
                exit(1);
    }



    // if requesting a directory -- use dir_extension() ^^
    if(strncmp(buffer, "GET / ", 6) == 0 || strncmp(buffer,"get / ", 6) == 0){
        strcpy(buffer, "GET /. ");
    }

    

    //*buffer = dir_extension(buffer);


    // i: buffer position       Get /j/batman
                    //          Get /.j/batman 
    int i = 5;

    // j: filePath position
    int j = 0;

    // Isolate file path from the request
    while(buffer[i] != ' ' &&  buffer[i] != '\n' &&  buffer[i] != '\0'){    
        filePath[j] = buffer[i];
        i++;
        j++;
    }
    filePath[j] = 0;

    char **inputs;
    //char *inputs[2];
    //printf("fpath before %s\n", filePath);
    //*filePath = strtok(filePath, "?");
    if(strchr(filePath, '?')){
        //printf("here");
        char *fPath = strtok(filePath, "?");
        strcpy(filePath, fPath);
        fPath = strtok(NULL, "?");
        int counter = 0;
        //char *inputs[2];
        /* fPath = strtok(NULL, "&");
        while (fPath != NULL) {
            //inputs[i++] = fPath;
            printf("\ninputs: %s\n", fPath);
            fPath = strtok(NULL, "&");
        } */
        strcpy(input, fPath);
        int count = 1;

        //printf("here");
        for(int i = 0; i < strlen(input); i++){
            if(input[i] == '&') {
                count++;
            }
        }
        inputs = (char *)malloc(count*sizeof(char*));
        char *value = strtok(input, "&");
        int index = 0;
        while(value != NULL) {
            inputs[index] = malloc(strlen(value)*sizeof(char));
            //printf("value: %s\n index: %d\n", value, index);
            strcpy(inputs[index], value);
            //printf("input at index %d: %s\n", index, inputs[index]);
            value = strtok(NULL, "&");
            index++;
            //strcpy(inputs[index], value);
        }
        //test.cgi?hello&goodbye&hi
        //test.cgi
        //[hello, goobye,hi]

        /* for(int i = 0; i < count; i++){
            inputs[i] = malloc(strlen(value)*sizeof(char));
            printf("value1: %s\n", value);
            printf("copying");
            strcpy(inputs[i], value);
            value = strtok(NULL, "&");
            //strcpy(inputs[i+1], value);
            printf("value2: %s\n", value);
            if(value == NULL){
                printf("here");
                break;
            }
        } */

        //printf("\nsize: %d", sizeof(inputs));
        /* printf("\nindex: %d %s", 2, inputs[2]);
        printf("\nindex: %d %s", 1, inputs[1]);
        printf("\nindex: %d", 2); */

        //int j = 0;
        //for(j= 0; j <= 2; j++){
            //printf("\nindex: %d %s", j, inputs[j]);
            //printf("\ninput: %s", inputs[j]);
        //}
        
        //int index = 0;
        //char *value = strtok(input, "&");
        //strcpy(inputs[index], value);
        //while(value != NULL){
            //strcpy(inputs[index], value);
            //index++;
            //value = strtok(NULL, "&");
        //}
        //printf("count: %d", count);
        //printf("\nfpath: %s\n", fPath);
    }
    //for(int i = 0; i <2; i++){
        //printf("%s", inputs[i]);
    //}
    //printf("\nfpath: %s\n", input);
    //printf("filepath after %s\n", filePath);

    char *fileType = extension(filePath);

   /*  int count = 0;

    printf("here");
    for(int i = 0; i < strlen(input); i++){
        if(input[i] == '&') {
            count++;
        }
    }

    printf("count: %d", count); */

    /* char *inList[count];

    int index = 0;
    char *value = strtok(input, "&");
    while(value != NULL){
        strcpy(inList[i], value);
        i++;
        value = strtok(NULL, "&");
    }

    for(int j = 0; j < count; j++) {
        printf(inList[j]);
    }
 */
    //printf("\nftype: %s\n", fType);

    /* // Get file extension
    j = strlen(filePath) - 1;
    i = 0;
    // Navigate back to character just before file extension
    while(filePath[j] != '.' && j > 0){
        j--;
    }
    // Traverse to end of file path to get file type
    while(j < strlen(filePath) && i < 19){
        fileType[i] = filePath[j];
        i++;
        j++;
    }

    //printf("\nfiletype: %s\n", fileType);  */

    /*
    hello.txt
    .txt
    test.cgi?i=1&i=2
    test.cgi
    */

    

    // Check if file exists
    //printf("Requested file is %s\n", filePath);

    if((file = fopen(filePath, "r")) == NULL){
        http_error(404, client, fileType);
        exit(1);
    }

    
    // Output success message
    sprintf(output, "HTTP/1.1 200 OK\r\n");
    write(client, output, strlen(output));
    //printf("here2");

    /* // Get file extension
    j = strlen(filePath) - 1;
    i = 0;
    // Navigate back to character just before file extension
    while(filePath[j] != '.' && j > 0){
        j--;
    }
    // Traverse to end of file path to get file type
    while(j < strlen(filePath) && i < 19){
        fileType[i] = filePath[j];
        i++;
        j++;
    } */

    //work on figuring out gnuplot
    
    // Handle the request based on the extension of the file
    // to handle cgi input args
        int flag_args = 0;
        char cgi_args_raw[1024];
        char *token;
        token = strtok(request,"?");
        strcpy(request,token);
        token = strtok(NULL, "");
        if(token != NULL){
            strcpy(cgi_args_raw,token);
            flag_args = 1;
        }
        char cmd[400];
        char readbuf[80];

    //printf("filetype: %s", fileType);
    if(strcmp(fileType, ".cgi") == 0){ // do we need to fflush() this?? before dup2
            /*
        int pipe1[2]; //add signal handlers
        pipe(pipe1);
        //fflush(stdout);
        */
        // Create new process to run CGI and get results through a pipe
        //printf("here2");
        int pipe1[2];
        pipe(pipe1);
        int pid = fork();
        if(pid == 0){

            dup2(client, 1);
            close(pipe1[0]);

            
           // printf("here2");
            //dup2(pipe1[1], 1);
            //close(pipe1[0]);
            // Set proper permissions
        //int parameter = strtol("0755", 0, 8);
          //  chmod(filePath, parameter);
        //sprintf(cmd, "chmod 755 %s && %s", request, request);
            /* if (flag_args == 1) {
                char tempbuf[80];
                char *token1;
                char *token2;
                token1 = strtok(cgi_args_raw, "&");
                while (token1 != NULL) {
                    strcpy(tempbuf, token1);
                    token2 = strrchr(tempbuf, '=');
                    strcat(cmd, " ");
                    strcat(cmd, &token2[1]);
                    token1 = strtok(NULL, "&");
                }
            } */

            //fork a process 
                //check if fork = 0
                //dup2 in child to set stdout to be client socket descriptor
                //and exec command
                //exec test.cgi



           /*  FILE *pipe;
                if( (pipe = popen(cmd, "r")) == NULL ) {  //use fork and exec instead of popen (inheriting from shell instead of webserver)
                    perror("popen error: ");
                    exit(1);
                } */

                

                //sprintf(output, "HTTP/ 1.1 200 OK\r\n");
                //write(client, output, strlen(output));

                //not necessary
                /* while(fgets(readbuf, 80, pipe)) {
                    write(client, readbuf, strlen(readbuf));
                } */

                // Close pipe
                /* if (pclose(pipe) < 0) {
                    perror("pclose error: ");
                    exit(1);
                } */
                int param = strtol("0755", 0, 8);
                chmod(filePath, param);

                //printf("here2");
                execl(filePath, filePath, NULL); 
                exit(0);
        
        }else{ // figure out how to handle python scripts
            close(pipe1[1]);
            //pclose(pipe);
            waitpid(pid, NULL, 0);

            //Write result to client    
            //printf("here1");
            char buf;
            while(read (pipe1[0],&buf, sizeof(buf)) > 0){
                write(client, &buf, 1);
            }

            close(pipe1[0]);
        }
        
    }else if(strcmp(fileType, ".jpg") == 0 || strcmp(fileType, ".jpeg") == 0 || strcmp(fileType, ".gif") == 0){
        // Figure out content type
        if(strcmp(fileType, ".gif") == 0)
            sprintf(output,"Content-Type: image/gif\r\n\r\n");
        else
            sprintf(output,"Content-Type: image/jpeg\r\n\r\n");
        // Write content type to client
        write(client, output, strlen(output));
        
        // Get image file descriptor
        int img = open(filePath, O_RDONLY, 0);
        // Get file metadata
        struct stat stat_struct;
        if(fstat(img, &stat_struct)== -1)
            perror("stat error\n");
        size_t total = 0;
        ssize_t bytesSent;
        int imgSize = stat_struct.st_size;
        if(imgSize == -1)
            perror("File size problem\n");

        // Send file to client
        while(total < imgSize){
            bytesSent = sendfile(client, img, 0, imgSize-total);
            if(bytesSent <= 0)
                perror("Error: sending file\n");
            total += bytesSent;
        }
    
    }else if(strcmp(fileType, ".html") == 0 || (strcmp(fileType, ".txt") == 0)) {
        //printf("here3");
        // Decide content type
        if((strcmp(fileType, ".txt") == 0))
            sprintf(output, "Content-Type: text/plain\r\n\r\n");
        else
            sprintf(output, "Content-Type: text/html\r\n\r\n");
        // Write content type to client
        write(client, output, strlen(output));

        
        //sprintf(output, "Output of file:\r\n\r\n");
        //write(client, output, strlen(output));
        
        //Write the contents of the file to client
        //printf("here");
        //fgets(output, sizeof(output), file);
        //printf("\n%s\n", output);
        while(!feof(file)){
            fgets(output, sizeof(output), file);
            //printf("here2");
            //printf("\n%s\n", output);
            send(client, output, strlen(output), 0);
            //sprintf(output, output);
            //write(client,output,strlen(output));
            //fgets(output, sizeof(output), file);
        }
    }else{ // directory listing request
    
       /* strcpy(ptr,buffer);
        strcpy(ptr, "./");
        if( wont_stat(ptr) ){

            dir_extension(ptr);
            sprintf(output, "Content-Type: text/plain\r\n\r\nDirectory Listing: \n\n");
            write(client, output, strlen(output));
            
            FILE *fd = open(fd , "w");
            dup2(fd , 1);
            dup2(fd , 2);
            sprintf(output,"ls","ls","-l",ptr,NULL);
            write(client, output, strlen(output));
        }  */


       DIR *folder;
        struct dirent *de;
        // if dir exists
        if((folder = opendir(filePath)) != NULL){
            // Write content type to client
            sprintf(output, "Content-Type: text/plain\r\n\r\nDirectory Listing: \n\n");
            write(client, output, strlen(output));
            // Write the file/folder names to client
            while((de = readdir(folder)) != NULL){
                sprintf(output, de->d_name);
                // Ignore current and parent directory links
                if(strcmp(output, ".") != 0 && strcmp(output, "..") != 0){
                    write(client, output, strlen(output));
                    write(client, "\n", 1);
                }
            }
            closedir(folder); 
            
        }
        else{
            http_error(404, client, fileType);
        }
    }

    close(client);
    exit(0);
    return 0;
}

/* void serve(int portnum){
	int listen_fd, client_fd;
	struct sockaddr_in host_addr;
	struct sockaddr_in client_addr;

	int approved = 1;
	int client_size;

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Server-socket error");
		exit(-1);
	}

	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &approved, sizeof(int)) == -1){
		perror("Server-setsockopt error");
		exit(-1);
	}
}
// pipe/send file is done with histogram.cgi */


/* int main(int argc, char*argv[]){
    int sockFd, clientFd;
    struct sockaddr_in clientAddr;
    int usesThreads = 0;

    /* char *s = extension("my-histogram.cgi");
    printf("\n %s \n", s); */

	/* host_addr.sin_family = AF_INET; // any avail port
	host_addr.sin_port = htons(portnum); // network bytes
	host_addr.sin_addr.s_addr = htonl(INADDR_ANY); // fill w/host's IP

	printf("Server-Using %s and port %d.\n", inet_ntoa(host_addr.sin_addr), portnum);
	memset(&(host_addr.sin_zero), '\0', 8); // default constructor https://stackoverflow.com/questions/24666186/why-memset-sockaddr-in-to-0

	if(bind(listen_fd, (struct sockaddr *)&host_addr, sizeof(host_addr)) < -1){
		perror("Server-bind error");
		exit(-1);
	} */

    /*// 2 for testing purposes...break it!
	if(listen(listen_fd, 2) == -1){
		perror("Server-listen error");
		exit(-1);
	}

	char http_buf[200]; // request
	while(1){
		client_size = sizeof(client_addr);
		if((client_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_size)) == -1){
			perror("Server-accept() error");
			continue;
		}
		if(fork() == 0){
			close(listen_fd); // child doesn't need
            //correct_remote_address(client_fd); // recv() another client http GET buf
			getRequest((void *)&client_fd); // right here guys?? idk
			close(client_fd);
			exit(0);
		}
		close(client_fd); // parent doesn't need
	}
} */
// pipe/send file is done with histogram.cgi */


int main(int argc, char*argv[]){
    int sockFd, clientFd;
    struct sockaddr_in clientAddr;
    int usesThreads = 0;

    dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
    dummyItem->data = -1;  
    dummyItem->key = -1;
    insert(1, 4412);
    insert(2, 1234);
    insert(42, 4321);
   
    char name[5];
    printf("Please enter a valid passkey: ");
    fgets(name,5,stdin);
//    printf("Glad to meet you, %s\n",name); 
    int x = atoi(name); 		
    item = search(42);

    if(item != NULL){
   //    printf("Element found: %d\n", item->data);
       if(item->data == x){
           printf("We have a match %d\n", x);
           system("python3 gotwoangle.py");
	   //system("python3 stream.py");
       }
    } else if(item->data != x) {
       printf("Element not found...you are an imposter! %d\n",x);
    }

    delete(item);
    item = search(42);
    /* char *s = extension("my-histogram.cgi");
    printf("\n %s \n", s); */

    /* char *b = "GET /mnt/c/Users/kshin/Documents/GitHub/Physical-Computing";

    dir_extension(b);

    printf("\n%s\n", b);

    if(strncmp(b, "GET / ", 6) == 0 || strncmp(b,"get / ", 6) == 0){
        strcpy(b, "GET /. ");
    }

    printf("\n%s\n", b); */
    
    // Error check arguments

    if(argc != 3){
        fprintf(stderr, "Error: The input should only be a port number. \n");
        exit(1);
    }
    
    sockFd = server(argv[1]);
    usesThreads = atoi(argv[2]);

    // Start listening for connections, 100 is maximum number of connections
    if(listen(sockFd, 100) < 0){
        fprintf(stderr, "Listen error.\n");
    }
        
    printf("The web server is now online!\n");
    while(1){
        int size =  sizeof(clientAddr);
        
        //Create and bind client socket and address and get client file descriptor
        if((clientFd = accept(sockFd, (struct sockaddr *) &clientAddr, &size)) < 0){
            printf("Error: Did not accept properly.%i: %s\n", errno, strerror(errno));
            continue;
        }

        // If user requested threads
        if(usesThreads == 1){
            void* stack = malloc(16000);
            pid_t pid = clone(&getRequest, (char*) stack + 16000, SIGCHLD | CLONE_FS | CLONE_SIGHAND | CLONE_VM, ((void *)&clientFd));
        }else if(fork() == 0){
            close(sockFd);
    
            getRequest(((void *)&clientFd));    
            close(clientFd);
            exit(0);
        }
        
        close(clientFd);    
    }
}
