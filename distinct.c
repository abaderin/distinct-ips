#include <arpa/inet.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MMAP_BUFFER_SIZE (1u << 26)
#define MAP_SIZE (1l << 29)

unsigned int count_1bits(unsigned int x) {
  x = x - ((x >> 1) & 0x55555555);
  x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
  x = x + (x >> 8);
  x = x + (x >> 16);
  return x & 0x0000003F;
}

/*
  111.111.111.111 = 15 bytes + newline at the end = 16 bytes
  after parse we need seek to n positions, straight after newline of eof
 */

struct ipv4_bytes {
  unsigned char byte1;
  unsigned char byte2;
  unsigned char byte3;
  unsigned char byte4;
};

union ipv4 {
  struct ipv4_bytes bytes;
  unsigned int whole;
};


unsigned int read_ipv4(FILE *fd) {
  #define MAX_LINE_LEN 16
  char line[MAX_LINE_LEN];
  int c = read(fd, line, MAX_LINE_LEN);
  for (int i=0; i<c; i++) {
    // search for four numbers
    // going four times to point
  }
}

/*
 так, в чем прикол.
 мы 32 бита (unsigned int) используем для хранения 32 адресов
 соответственно надо обеспечить установку бита по адресу
 2 ** 5 = 32
 сдвиг вправо на 5 битов поделит на 32 и таким образом мы получим индекс в массиве
 маска по первым 5 битам (31) даст нам адрес внутри 32 битного числа
 unsigned int addr = index>>5;
 unsigned int mask = 1<<shift_count;
 map[addr] |= mask;
 */

int main(int argc, char* argv[]) {
  printf("sizeof(unsigned int) = %d\n", sizeof(unsigned int));
  printf("sizeof(unsigned long) = %d\n", sizeof(unsigned long));
  printf("initializing array of len: %lu\n", MAP_SIZE);
  unsigned char* map = calloc(MAP_SIZE/sizeof(unsigned int), sizeof(unsigned int));

  // mmap file into memory and for linear time go through it
  


  unsigned long counter = 0;
  for(unsigned long i=0; i<MAP_SIZE; i+=sizeof(unsigned int)) {
    counter += count_1bits(map[i]);
  }
  printf("count is: %lu\n", counter);
  free(map);
  return 0;
}

