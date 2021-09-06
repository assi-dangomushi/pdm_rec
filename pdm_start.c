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

//pdm_start.c ver. 0.1

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "pi_perif.h"

#define STATUS_FILE "/tmp/pdm_clock"

void print_help(void){
  fprintf(stderr, ""
    "Usage : sudo pdm_start clock\n"
    "clock:  1.2 | 1.6 | 1.92 | 2.4 | 3.2 | 4.8  (MHz)\n"
    );
}

int main(int argc, char **argv){

  if (argc != 2){
    print_help();
    exit(1);
  }
  int clock = (int)(atof(argv[1])*1000);
  int div;
  switch (clock){
    case 1200:
      div = 16;
      break;
    case 1600:
      div = 12;
      break;
    case 1920:
      div = 10;
      break;
    case 2400:
      div = 8;
      break;
    case 3200:
      div = 6;
      break;
    case 4800:
      div = 4;
      break;
    default:
      fprintf(stderr, "clock is invalid！\n");
      print_help();
      exit(1);
  }

  init_perif();

  // set GPIO(18, 20) to ALT0  GPIO18:PCM_CLK  GPIO20:PCM_DIN
  *gpfsel1 = (*gpfsel1 & ~(7<<24)) | (FSEL_ALT0<<24); //GPIO18
  *gpfsel2 = (*gpfsel2 & ~(7<<0)) | (FSEL_ALT0<<0); //GPIO20

  // stop PCM CLOCK
  *cm_pcmctl = (*cm_pcmctl & ~(1<<4)) | CLK_PASSWD;   //ENAB:0
  while(*cm_pcmctl & (1<<7)) { // BUSY
    ;
  }

  // set PCM clock source
  // SRC:OSC(19.2MHz)|ENAB:0|FLIP:0|MASH:0   19.2/8 = 2.4MHz(standard mode)
  *cm_pcmctl = OSC | (0<<4) | (0<<8) | (0<<9) | CLK_PASSWD;
  // DIVF:0|DIVI:div
  *cm_pcmdiv = 0 | (div<<12) | CLK_PASSWD; 
  // start PCM CLOCK
  *cm_pcmctl = *cm_pcmctl | (1<<4) | CLK_PASSWD;   //ENAB

  //stop pcm
  *pcm_cs = *pcm_cs & ~(1<<0);  //EN:0
  // set MODE
  // FSLEN:0|FLEN:0|FSM:0|CLKM:0|FRXP:1|PDME:1|PDMN:0|CLK_DIS:0
  *pcm_mode = 0 | (0<<10) | (0<<21) | (0<<23) | (1<<25) | (1<<26) | (0<<27) | (0<<28);  

  // set RXC
  // CH2WID:0|CH2POS:0|CH2EN:1|CH2WEX:0|CH1WID:0|CH1POS:0|CH1EN:1|CH1WEX:0
  *pcm_rxc = 0 | (0<<4) | (1<<14) | (0<<15) | (0<<16) | (0<<20) |(1<<30) | (0<<31);
  // start PCM
  // EN:1|RXON:1|TXON:0|TXCLR:0|RXCLR:0|TXTHR:0|RXTHR:0
  *pcm_cs = 1 | (1<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5) | (0<<7);
   
  sleep(1);
  FILE *fp;
  fp = fopen(STATUS_FILE, "w");
  fwrite(&clock, sizeof(int) , 1, fp);
  fclose(fp);
}

