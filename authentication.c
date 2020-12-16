#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 20

struct hashinfo {
   int data;   
   int key;
};

struct hashinfo* data[SIZE]; 
struct hashinfo* temp;
struct hashinfo* item;

int hashCode(int key) {
   return key % SIZE;
}

struct hashinfo *search(int key) {
   //get the hash 
   int index = hashCode(key);  
	
   //move in array until an empty 
   while(data[index] != NULL) {
	
      if(data[index]->key == key)
         return data[index]; 
			
      //go to next cell
      ++index;
		
      //wrap around the table
      index %= SIZE;
   }        
	
   return NULL;        
}

void insert(int key,int data) {

   struct hashinfo *item = (struct hashinfo*) malloc(sizeof(struct hashinfo));
   item->data = data;  
   item->key = key;

   //get the hash 
   int index = hashCode(key);

   //move in array until an empty or deleted cell
   while(data[index] != NULL && data[index]->key != -1) {
      //go to next cell
      ++index;
		
      //wrap around the table
      index %= SIZE;
   }
	
   data[index] = item;
}

struct hashinfo* delete(struct hashinfo* item) {
   int key = item->key;

   //get the hash 
   int index = hashCode(key);

   //move in array until an empty
   while(data[index] != NULL) {
	
      if(data[index]->key == key) {
         struct hashinfo* temp = data[index]; 
			
         //assign a dummy item at deleted position
         data[index] = temp; 
         return temp;
      }
		
      //go to next cell
      ++index;
		
      //wrap around the table
      index %= SIZE;
   }      
	
   return NULL;        
}

void display() {
   int i = 0;
	
   for(i = 0; i<SIZE; i++) {
	
      if(data[i] != NULL)
         printf(" (%d,%d)",data[i]->key,data[i]->data);
      else
         printf(" ~~ ");
   }
	
   printf("\n");
}

int main() {
   temp = (struct hashinfo*) malloc(sizeof(struct hashinfo));
   temp->data = -1;  
   temp->key = -1; 

   /*
    char * cmd;

    // if(authed == TRUE){
    cmd = "python3 gotwoangle.py"
    system(cmd);
*/
   insert(1, 4412);
   insert(2, 1234);
   insert(42, 4321);
   
    // 1. Construct fgets that takes STDIN numeric input to be built into 4-digit passwords...
    // - possibly requires decoding char* argv with atoi()
    // 2. search(index of that passkey)

   display();
   item = search(42);

   if(item != NULL) {
      printf("Element found: %d\n", item->data);
   } else {
      printf("Element not found\n");
   }

   delete(item);
   item = search(42); // to check if the data quickly disappears

   if(item != NULL) { // passkey valid
      printf("Element found: %d\n", item->data);
      system("python3 gotwoangle.py"); // open the floodgates!!! (opens door)
   } else {
      printf("Element not found\n");
   }
}
