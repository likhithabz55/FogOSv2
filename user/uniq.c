#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define NULL (void*)0

struct uniq_options {
  int count_lines;
  int dup_lines;
  int print_all_dup_lines;
  int ignore_case;
  int print_unique;
  int skip_chars;
}; 

//helper function similar to gets to read lines from file to a buffer
char* fgets(char* buf, int max, int fd) {
  memset(buf, 0, max);  // fill buffer with zeros
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

//helper function to convert characters to lowercase
char to_lower(char c) {
  if (c >= 'A' && c <= 'Z') {
     return c - 'A' + 'a';
  }
  return c;
}

//helper function similar to strcmp, but considers ignore case, skip-chars options
int equals(char *str1, char * str2, struct uniq_options *flags) {
  int n = flags->skip_chars;
  if( n >= strlen(str1) && n >= strlen(str2)) {
    return 0;
  }    
  for(; str1[n] && str2[n]; n++ ) {
    char c1 = str1[n];
    char c2 = str2[n];    
    if(flags->ignore_case == 1) {
      c1 = to_lower(c1);
      c2 = to_lower(c2);
    }
    if( c1 != c2) return (uchar)c1 - (uchar)c2;    
  }   
  return (uchar)str1[n] - (uchar)str2[n];  	  
}	

//helper function to compare strings upto n characters
int strncmp(char *str1, char *str2, int n) {
  int x = 0;
  while(x < n && *str1 && *str1 == *str2) {
    str1++, str2++;
    x++;
  }
  if(x == n) {
    return 0;
  }    
  return (uchar)*str1 - (uchar)*str2;
}  

//prints for different flags
void print_line(const char *line, int count, struct uniq_options *flags)
{
  if (flags->count_lines == 1) {
     printf("%d %s", count, line);
  }
  else if(flags->print_unique == 1) {
     if (count == 1) {
        printf("%s", line);
     }	
  }
  else if (flags->dup_lines == 1 || flags->print_all_dup_lines == 1) {
     if (count > 1) {
        printf("%s", line);
     }
  }
  else {
    printf("%s", line);
  }
}	

//uniq function implementation
void uniq(int fd, struct uniq_options *flags) {
  char buffer[1024];
  char curr_buf[1024] = {0};
  int count = 0;
  if (fgets(buffer, sizeof(buffer), fd) == NULL)
    return;
  
  strcpy(curr_buf, buffer);
  count++;
  while (fgets(buffer, sizeof(buffer), fd) != NULL)
  {
    if (equals(curr_buf, buffer, flags) == 0) {
       if (flags->print_all_dup_lines == 1) {
          printf("%s", curr_buf);
       }	    
       count++;
    }
    else {
       print_line(curr_buf, count, flags);
       strcpy(curr_buf, buffer);
       count = 1;
    }
  }
  print_line(curr_buf, count, flags);  
}	

//Helper function to convert string to int
int parseInt(char* s) {
  if (s == 0 || *s == '\0') {
     return -1;
  }
  int n = 0;
  for(int j = 0; s[j]; j++) {
    if (s[j] < '0' || s[j] > '9') {
      return -1;
    }
    n = n*10 + s[j] - '0';
  }
  return n;
}	

struct uniq_options* parseFlags(int argc, char *argv[], int i, struct uniq_options *flags) {
  if((strcmp(argv[i], "-c") == 0) || (strcmp(argv[i], "--count") == 0)) {
    flags->count_lines = 1;
  }
  else if((strcmp(argv[i], "-d") == 0) || (strcmp(argv[i], "--repeated") == 0)) {
    flags->dup_lines = 1;
  }
  else if(strcmp(argv[i], "-D") == 0) {
    flags->print_all_dup_lines = 1;
  }
  else if((strcmp(argv[i], "-i") == 0) || (strcmp(argv[i], "--ignore-case") == 0)) {
    flags->ignore_case = 1;
  }
  else if((strcmp(argv[i], "-u") == 0) || (strcmp(argv[i], "--unique") == 0)) {
    flags->print_unique = 1;
  }
  else if(strncmp(argv[i], "--skip-chars=", 13) == 0) {
    int val = parseInt(&argv[i][13]);
    if( val < 0 ) {
      printf("Missing bytes\n");
      return NULL;
    }    
    flags->skip_chars = val;
  }
  else if(strcmp(argv[i], "-s") == 0) {
    if(++i < argc) {
      int val = parseInt(argv[i]);
      if( val < 0 ) {
	printf("Missing bytes\n");
        return NULL;	
      }
      flags->skip_chars = val;
    }
    else {
      printf("Missing bytes\n");
      return NULL;
    }
  }    
  else {
    printf("Unknown flag found!!\n");
    return NULL;
  }    
  return flags;
}	

int main(int argc, char* argv[])
{
  int fd;
  struct uniq_options flags = {0};
  int i = 1;
  for(; i < argc; i++) {
     if(argv[i][0] == '-') {
       if(parseFlags(argc, argv, i, &flags) == NULL) break;
       if(flags.skip_chars > 0 && (strcmp(argv[i], "-s") == 0)) i++;
     }
     else break;
  }
  if(i == argc) {
    uniq(0, &flags);
  }
  else if((fd = open(argv[i], O_RDONLY)) < 0) {
    printf("No such File or directory!!\n");
    exit(1);
  }
  else {
    uniq(fd, &flags);
    close(fd);
  }
  exit(0);  
}	
