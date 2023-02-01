#include <fcntl.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define FILEMODE S_IRWXU | S_IRGRP | S_IROTH
#define MAP_SIZE (1l << 32)

int main(int argc, char* argv[]) {
  int fd;
  int ret;
  struct stat st;
  char* addr;

  if ((fd = open(argv[1], O_RDONLY, FILEMODE)) < 0)
    perror("error in file opening");

  if ((ret = fstat(fd, &st)) < 0)
    perror("Error in fstat");

  if ((addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
    perror("Error in mmap");

  unsigned long len = st.st_size;

  unsigned char *map = calloc(MAP_SIZE/4096, 4096);
  if (map == NULL)
    perror("Error in calloc");

  unsigned long count = 0;
  unsigned char bytes[4];
  unsigned int ip;
  int is_number = FALSE;
  int ip_position = 0; // bytes[0]
  unsigned long number_count;

  for (unsigned long i=0; i<len; i++) {
    if (addr[i] >= '0' && addr[i] <= '9') {
      if (!is_number) {
        number_count = 1;
        is_number = TRUE;
      } else {
        number_count++;
      }
    } else {
      if (is_number) {
        // convert into byte
        int acc = 0;
        int multiplier = 1;
        unsigned long number_substr_bound = i-number_count;
        for(unsigned long s_pos=i-1; s_pos>=number_substr_bound; s_pos--)
        {
          acc += (addr[s_pos]-48) * multiplier;
          multiplier *= 10;
          if (s_pos == 0) break;
        }

        bytes[ip_position++] = acc;
        if (ip_position == 4) {
          ip_position = 0;
          ip = bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
          /* unsigned int index_in_array = (whole >> 5) / sizeof(unsigned int); */
          /* unsigned int index_in_uint = 1 << (whole & 31u); */
          /* map[index_in_array] |= index_in_uint; */
          map[ip] = (unsigned char)1;
        }
      }
      is_number = FALSE;
    }
  }

  unsigned int counter = 0;
  unsigned int iterations = 0;
  /* unsigned int bound = MAP_SIZE / sizeof(unsigned int); */
  /* printf("iterating %du times\n", bound); */
  for (unsigned long i=0; i<MAP_SIZE; i+=16) {
    unsigned int shift = 0;
    unsigned int x;
    iterations++;

    /* x = map[i+(shift++)]; */
    /* x = x - ((x >> 1) & 0x55555555); */
    /* x = (x & 0x33333333) + ((x >> 2) & 0x33333333); */
    /* x = x + (x >> 8); */
    /* x = x + (x >> 16); */
    /* counter += x & 0x0000003F; */

    /* x = map[i + (shift++)]; */
    /* x = x - ((x >> 1) & 0x55555555); */
    /* x = (x & 0x33333333) + ((x >> 2) & 0x33333333); */
    /* x = x + (x >> 8); */
    /* x = x + (x >> 16); */
    /* counter += x & 0x0000003F; */

    /* x = map[i + (shift++)]; */
    /* x = x - ((x >> 1) & 0x55555555); */
    /* x = (x & 0x33333333) + ((x >> 2) & 0x33333333); */
    /* x = x + (x >> 8); */
    /* x = x + (x >> 16); */
    /* counter += x & 0x0000003F; */

    /* x = map[i + (shift++)]; */
    /* x = x - ((x >> 1) & 0x55555555); */
    /* x = (x & 0x33333333) + ((x >> 2) & 0x33333333); */
    /* x = x + (x >> 8); */
    /* x = x + (x >> 16); */
    /* counter += x & 0x0000003F; */
    counter += map[i + (shift++)];
    counter += map[i + (shift++)];
    counter += map[i + (shift++)];
    counter += map[i + (shift++)];

    counter += map[i + (shift++)];
    counter += map[i + (shift++)];
    counter += map[i + (shift++)];
    counter += map[i + (shift++)];

    counter += map[i + (shift++)];
    counter += map[i + (shift++)];
    counter += map[i + (shift++)];
    counter += map[i + (shift++)];

    counter += map[i + (shift++)];
    counter += map[i + (shift++)];
    counter += map[i + (shift++)];
    counter += map[i + (shift++)];
  }

  printf("count = %du\n", counter);
  printf("total iterations = %du\n", iterations);

  free(map);
  close(fd);
  return 0;
}
