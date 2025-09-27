#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define NULL (void*)0

char* fgets(char* buf, int max, int fd) {
  int i, cc = 0;
  char c;
  for(i = 0; i + 1 < max;) {
    cc = read(fd, &c, 1);
    if(cc < 1) break;
    buf[i++] = c;
    if(c == '\n' || c == '\r') {
      break;
    }
  }
  if(i == 0 && cc <= 0) {
    return NULL;
  }    
  buf[i] = '\0';
  return buf;
}

void uniq(int fd) {
  char buffer[1024];
  char curr_buf[1024];
  int line_num = 0;
  while(fgets(buffer, sizeof(buffer), fd) != NULL) {
    line_num++;	  
    if(line_num == 1) {
      strcpy(curr_buf, buffer);
      printf("%s", buffer);
    }
    else {
      if(strcmp(curr_buf, buffer) != 0) {
        strcpy(curr_buf, buffer);
        printf("%s", buffer);
      }
    }      
  }
  return;
}  

int main(int argc, char* argv[])
{
  int fd;
  if(argc == 1) {
    printf("Reading from Standard Input...\n");
  }
  else if(argc > 1) {
    for(int i = 1; i < argc; i++) {
       if((fd = open(argv[i], O_RDONLY)) < 0) { 	    
	 printf("No such File or directory!!\n");
       }
       else {
	 uniq(fd);
	 close(fd);
       }	 
    }	
  }
  exit(0);  
}	
