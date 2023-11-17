#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]) {
    char file[100],buf[100];
    // scanf("\n%s",file);
    strcpy(file,"filea.txt");
    // strcat(file,"\0");
    FILE *fptr;

   if ((fptr = fopen("/home/mayur/CN/6/filea.txt","r")) == NULL){
//    if ((fptr = fopen("filea.txt","r")) == NULL){
//    if ((fptr = fopen(file,"r")) == NULL){
       printf("Error! opening file");
       exit(1);
   }
//    fread(buf,1,sizeof(buf),fptr);
   while(fgets(buf,sizeof(buf),fptr)!=NULL) {
        printf("\n%s\n",buf);
    }
   fclose(fptr); 
  
   return 0;
}