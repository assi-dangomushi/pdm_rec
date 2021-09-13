/*
   Copyright (c) 2021, Amanogawa Audio Labo
   All rights reserved.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// pdm_rec.c ver. 0.1

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "pi_perif.h"

#define STATUS_FILE "/tmp/pdm_clock"

void print_help(void){
  fprintf(stderr, ""
    "Usage : sudo pdm_rec duration(sec) ch\n"
    "ch:  ch1 | ch2 | ch1+2\n"
    "output data to stdout\n"
    "run 'pdm_start' befor pdm_rec\n"
    "");
}

void ch2(int16_t *data, int32_t *i){
  *(data + *i) = (*pcm_fifo >> 16) - (1<<15);
  *i = *i + 1;
}

void ch1(int16_t *data, int32_t *i){
  *(data + *i) = (*pcm_fifo & 0x0000FFFF) - (1<<15);
  *i = *i + 1;
}

void ch12(int16_t *data, int32_t *i){
  uint32_t tmp;
  *(data + *i) = ((tmp = *pcm_fifo) & 0x0000FFFF) - (1<<15);
  *(data + *i + 1) = (tmp >> 16) - (1<<15);
  *i = *i + 2;
}

int main(int argc, char **argv){

  int32_t clock;
  int r;
  FILE *fp;
  if ((fp = fopen(STATUS_FILE, "r")) == NULL) {
    print_help();
    exit(1);
  }
  r = fread(&clock, sizeof(int) ,1 ,fp);
  fclose(fp);

  if ((argc != 3) || (r != 1)){
    print_help();
    exit(1);
  }
  clock = clock * 1000;

  int32_t duration = strtol(argv[1], NULL, 0);
  if (duration == 0){
    print_help();
    exit(1);
  }

  fprintf(stderr, "clock = %d  duration = %d\n", clock, duration);

  void (*func)(int16_t *data, int32_t *i);

  int ch_length;
  if (!(strcmp((argv[2]), "ch1"))){
    func = ch1;
    ch_length = 1;
  }else if (!(strcmp(argv[2], "ch2"))){
    func = ch2;
    ch_length = 1;
  }else if (!(strcmp(argv[2], "ch1+2"))){
    func = ch12;
    ch_length = 2;
  }else {
    print_help();
    exit(1);
  }

  init_perif();

  int16_t *rec_data = (int16_t*)malloc(sizeof(int16_t) * duration * (clock / 16) * ch_length);
  // fifo 64個捨てる
  uint32_t tmp;
  for (int i = 0; i < 64; i++){ 
    tmp = *pcm_fifo;
  }
  // reset RXERR
  *pcm_cs = *pcm_cs;

  int32_t i = 0;
  while (i < duration * (clock / 16) * ch_length){
    if ((tmp = *pcm_cs) & (1<<20)){  // RXD
      if (tmp & (1<<16)){ // RXERR
        fprintf(stderr, "ERROR %08x  %d\n", tmp, i);
        *pcm_cs = tmp;  // clear RXERR
      }
      (*func)(rec_data, &i);
      //fprintf(stderr , "i = %d\n", i);
      //rec_data[i++] = *pcm_fifo - (1<<15);
    }else{
      usleep(5);
    }
  }
  fwrite(rec_data, sizeof(int16_t), duration * (clock / 16) * ch_length, stdout);
}


