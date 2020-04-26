#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include<sys/reg.h>
#include <sys/user.h> 
#include <sys/syscall.h>
#include <sys/stat.h>
#include<fcntl.h>
#include <sys/sysmacros.h>
#include <stdbool.h> 
const int long_size = sizeof(long);

int getdata1(pid_t child, long addr, char *str)
{   
  char *laddr;
  int i, j;
  union u {
    long val;
    char chars[long_size];
  }data;
  
  i = 0;
  int len=0;
  laddr = str;
  while(1) {
    data.val = ptrace(PTRACE_PEEKDATA,child, addr + i * 4,NULL);
    memcpy(laddr, data.chars, long_size);
    ++i;

    for(int j=0;j<long_size;++j){
      if(data.chars[j]=='\0'){
       len=len+j;
       str[len] = '\0';
       return len;
     }
   }
   len+=long_size;
   laddr += long_size;

 }

 str[len] = '\0';
 return len;
}

void putdata1(pid_t child, long addr,
 char *str, int len)
{   char *laddr;
  int i, j;
  union u {
    long val;
    char chars[long_size];
  }data;
  i = 0;
  j = len / long_size;

  laddr = str;
  while(i < j) {
    memcpy(data.chars, laddr, long_size);
    ptrace(PTRACE_POKEDATA, child, addr + i * 4, data.val);
    ++i;
    laddr += long_size;
  }
  j = len % long_size;
  
   data.chars[j]='\0';
   memcpy(data.chars, laddr, j);
   ptrace(PTRACE_POKEDATA, child, addr + i * 4, data.val);

}

bool is_url(char *url,int len_url){

  char *sub_url=url;
  char *str1 = (char*)calloc((10),sizeof(char));
  if(len_url>=7){
    int i;
    for(i=0;i<6;++i){
      str1[i]=sub_url[i];
    }
    if(len_url>7){
      str1[i]=sub_url[i++];
    }
    str1[i]='\0';
    if(strcmp(str1,"http://") == 0 || strcmp(str1,"https://") == 0)
      return true;
  }
  return false;
}



int main(int argc, char *argv[])
{

  char* temp_file_path = "/tmp/url_extend_madhu/index.html";
  int file_name_length = 32;
  char* temp_dir_path = "/tmp/url_extend_madhu";
  char  *url = (char *)calloc((1000), sizeof(char));
  int len_url=0;

  
  char **copy_args = (char **)calloc(argc,sizeof(char*));
  for(int i=1;i<=argc;++i){
    copy_args[i-1]= argv[i];
  }
  copy_args[argc-1]=NULL;

  pid_t child;
  child = fork();
  
  if(child == 0) {
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
   // execl("/bin/cp","cp","http://www.cs.stonybrook.edu","madhu.txt");
    execvp(argv[1],copy_args);
  }
  else {
    long orig_eax;
    long params[3];
    int status;

    int toggle_stat = 0;
    int toggle_lstat = 0;
    int toggle_open = 0;
    int toggle_openat = 0;

    while(1) {
     wait(&status);
     if(WIFEXITED(status))
        break;
        orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX,NULL);


//--------------------------------SYS_stat64 begin-----------------------------------

     if(orig_eax == SYS_stat64) {
     
       if(toggle_stat == 0){
          toggle_stat = 1;
          params[0] = ptrace(PTRACE_PEEKUSER, child, 4 * EBX, NULL);
          params[1] = ptrace(PTRACE_PEEKUSER, child, 4 * ECX, NULL);
          params[2] = ptrace(PTRACE_PEEKUSER, child, 4 * EDX, NULL);

          len_url = getdata1(child,params[0],url);
 
          if(is_url(url,len_url)){
            
            putdata1(child, params[0],temp_file_path,file_name_length);
            remove(temp_file_path);
            mkdir(temp_dir_path,0777);
            
            pid_t child2 = fork();
            if(child2==0){
              execl("/usr/bin/wget","wget","-q","-O",temp_file_path,url,NULL);
            }else{
              wait(NULL);
              pid_t child3 = fork();
              if(child3 == 0){
                execl("/bin/chmod","chmod","444",temp_file_path,NULL);
              }else{
                wait(NULL);
              }
            }
          }

        }else{
          toggle_stat = 0;
        }
      }
//--------------------------------SYS_stat64 end-------------------------------------




//--------------------------------SYS_lstat64 begin-----------------------------------

     if(orig_eax == SYS_lstat64) {
     
       if(toggle_lstat == 0){
          toggle_lstat = 1;
          params[0] = ptrace(PTRACE_PEEKUSER, child, 4 * EBX, NULL);
          params[1] = ptrace(PTRACE_PEEKUSER, child, 4 * ECX, NULL);
          params[2] = ptrace(PTRACE_PEEKUSER, child, 4 * EDX, NULL);

          len_url = getdata1(child,params[0],url);
          

          if(is_url(url,len_url)){
            putdata1(child, params[0],temp_file_path,file_name_length);
            remove(temp_file_path);
            mkdir(temp_dir_path,0777);
            
            pid_t child2 = fork();
            if(child2==0){
              execl("/usr/bin/wget","wget","-q","-O",temp_file_path,url,NULL);
            }else{
              wait(NULL);
              pid_t child3 = fork();
              if(child3 == 0){
                execl("/bin/chmod","chmod","444",temp_file_path,NULL);
              }else{
                wait(NULL);
              }
            }
          }

        }else{
          toggle_lstat = 0;
        }
      }

//--------------------------------SYS_lstat64 end-------------------------------------




//--------------------------------SYS_open begin-----------------------------------

     if(orig_eax == SYS_open) {
     
       if(toggle_open == 0){
          toggle_open = 1;
          params[0] = ptrace(PTRACE_PEEKUSER, child, 4 * EBX, NULL);
          params[1] = ptrace(PTRACE_PEEKUSER, child, 4 * ECX, NULL);
          params[2] = ptrace(PTRACE_PEEKUSER, child, 4 * EDX, NULL);

          len_url = getdata1(child,params[0],url);
          

          if(is_url(url,len_url)){
            putdata1(child, params[0],temp_file_path,file_name_length);
            remove(temp_file_path);
            mkdir(temp_dir_path,0777);
            
            pid_t child2 = fork();
            if(child2==0){
              execl("/usr/bin/wget","wget","-q","-O",temp_file_path,url,NULL);
            }else{
              wait(NULL);
              pid_t child3 = fork();
              if(child3 == 0){
                execl("/bin/chmod","chmod","444",temp_file_path,NULL);
              }else{
                wait(NULL);
              }
            }
          }

        }else{
          toggle_open = 0;
        }
      }
//--------------------------------SYS_open end-------------------------------------




//--------------------------------SYS_openat begin-----------------------------------

     if(orig_eax == SYS_openat) {
     
       if(toggle_openat == 0){
          toggle_openat = 1;
          params[0] = ptrace(PTRACE_PEEKUSER, child, 4 * EBX, NULL);
          params[1] = ptrace(PTRACE_PEEKUSER, child, 4 * ECX, NULL);
          params[2] = ptrace(PTRACE_PEEKUSER, child, 4 * EDX, NULL);

          len_url = getdata1(child,params[1],url);

          if(is_url(url,len_url)){
            
            putdata1(child, params[1],temp_file_path,file_name_length);
            
            remove(temp_file_path);
            mkdir(temp_dir_path,0777);
            
            pid_t child2 = fork();
            if(child2==0){
              execl("/usr/bin/wget","wget","-q","-O",temp_file_path,url,NULL);
            }else{
              wait(NULL);
              pid_t child3 = fork();
              if(child3 == 0){
                execl("/bin/chmod","chmod","444",temp_file_path,NULL);
              }else{
                wait(NULL);
              }
            }
          }

        }else{
          toggle_openat = 0;
        }
      }

//--------------------------------SYS_openat end-------------------------------------

      ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    }
  }
  return 0;
}