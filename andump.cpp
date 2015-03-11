
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define LOGD(...) printf(__VA_ARGS__);

void hexdump(void *mem, unsigned len, unsigned mod){
  int x = 0, y = 0, line_count = 0;
  char line[1024];
  char ch[4] = {0};

  line_count = len / mod;
  line_count = len % mod ? line_count + 1 : line_count;
  
  for(x = 0; x < line_count; x++){
    memset(line, 0, sizeof(line));
    sprintf(ch, "|%08X| ", x * mod);
    strcat(line, ch);
    /*hex print*/
    for(y = mod * x; y < mod *(x+1); y++){
      if (y < len){
        sprintf(ch, "%02X ", ((unsigned char *)mem)[y]);
      }else{
        sprintf(ch, "00 ");
      }
      strcat(line, ch);
    }

    /*char print*/
    strcat(line, "|");
    for(y = mod * x; y < mod *(x+1); y++){
      unsigned ascii = (int)((unsigned char *)mem)[y];
      if ( ascii < 32 || 126 < ascii )
      {
        strcat(line, ".");
      }
      else
      {
        if ( ascii == 0x25 ){
          strcat(line, "%");
        }
        sprintf(ch, "%c", ascii);
        strcat(line, ch);
      }
    }
    y = 0;
    strcat(line, "|\n");
    LOGD(line);
  }
}

void usage(){
  printf("./andump.out [file]");
}

int main(int argc, const char *argv[]){
  int fd = -1;
  struct stat st = {0};
  char *buf = NULL;
  fd = open(argv[1], O_RDONLY);
  fstat(fd, &st);
  buf = (char *)malloc(st.st_size);
  read(fd, buf, st.st_size);
  hexdump(buf, st.st_size, 16);
  free(buf);
  close(fd);
  return 0;
}
