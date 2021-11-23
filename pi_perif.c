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

// pi_perif.c ver. 0.2

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include "pi_perif.h"

void *gpio_addr = 0;
void *clock_addr = 0;
void *pwm_addr = 0;
void *pcm_addr = 0;

volatile uint32_t *gpfsel0 = 0;
volatile uint32_t *gpfsel1 = 0;
volatile uint32_t *gpfsel2 = 0;
volatile uint32_t *gpfsel3 = 0;
volatile uint32_t *gpfsel4 = 0;
volatile uint32_t *gpfsel5 = 0;
volatile uint32_t *gplev0 = 0;
volatile uint32_t *gplev1 = 0;
volatile uint32_t *gpset0 = 0;
volatile uint32_t *gpclr0 = 0;

volatile uint32_t *cm_pwmctl = 0;
volatile uint32_t *cm_pwmdiv = 0;
volatile uint32_t *cm_pcmctl = 0;
volatile uint32_t *cm_pcmdiv = 0;

volatile uint32_t *pwm_ctl = 0;
volatile uint32_t *pwm_rng1 = 0;
volatile uint32_t *pwm_dat1 = 0;
  
volatile uint32_t *pcm_cs = 0;
volatile uint32_t *pcm_mode = 0;
volatile uint32_t *pcm_fifo = 0;
volatile uint32_t *pcm_txc = 0;
volatile uint32_t *pcm_rxc = 0;

int chk_pi4(void)
{
  // 判別不能 -1
  // pi4 40
  // pi3B+ 31
  // pi3A+ 32
  // pi3 30
  // pi2 20
  // pi0  0
  // その他 -2
  FILE *fp;
  const int N = 80;
  char buf[N];

  if ((fp = fopen("/proc/device-tree/model","r")) == NULL){
      return -1;
  }
  fgets(buf, N, fp);
  fclose(fp);

  if ((strstr(buf, "Pi 4 ")) != NULL){
    return 40;
  }
  if ((strstr(buf, "Pi 3 Model B Plus")) != NULL){
    return 31;
  }
  if ((strstr(buf, "Pi 3 Model A Plus")) != NULL){
    return 32;
  }
  if ((strstr(buf, "Pi 3 ")) != NULL){
    return 30;
  }
  if ((strstr(buf, "Pi 2 ")) != NULL){
    return 20;
  }
  if ((strstr(buf, "Pi Zero W ")) != NULL){
    return 0;
  }
  return -2;
}

void init_perif(void){
  uint32_t peri_base;
  switch(chk_pi4()){
    case 40:
      peri_base = PERI_BASE_PI4;
      break;
    case 31:
    case 32:
    case 30:
    case 20:
      peri_base = PERI_BASE_PI2;
      break;
    case 0:
      peri_base = PERI_BASE_PI0;
      break;
    default:
      exit(1);
  }

  long page_size = sysconf(_SC_PAGESIZE);
  //fprintf(stderr, "PAGE_SIZE = %d\n", page_size);
  int memfd;
  if ((memfd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
    fprintf(stderr, "Unable to open /dev/mem\n");
    exit (1);
  }
  gpio_addr = mmap(NULL, page_size, (PROT_READ | PROT_WRITE), MAP_SHARED, memfd, GPIO_BASE + peri_base);
  clock_addr = mmap(NULL, page_size, (PROT_READ | PROT_WRITE), MAP_SHARED, memfd, CLOCK_BASE + peri_base);
  pwm_addr = mmap(NULL, page_size, (PROT_READ | PROT_WRITE), MAP_SHARED, memfd, PWM_BASE + peri_base);
  pcm_addr = mmap(NULL, page_size, (PROT_READ | PROT_WRITE), MAP_SHARED, memfd, PCM_BASE + peri_base);
  close(memfd);

  gpfsel0 = (uint32_t*)gpio_addr + GPFSEL0/4;
  gpfsel1 = (uint32_t*)gpio_addr + GPFSEL1/4;
  gpfsel2 = (uint32_t*)gpio_addr + GPFSEL2/4;
  gpfsel3 = (uint32_t*)gpio_addr + GPFSEL3/4;
  gpfsel4 = (uint32_t*)gpio_addr + GPFSEL4/4;
  gpfsel5 = (uint32_t*)gpio_addr + GPFSEL5/4;
  gplev0 = (uint32_t*)gpio_addr + GPLEV0/4;
  gplev1 = (uint32_t*)gpio_addr + GPLEV1/4;
  gpset0 = (uint32_t*)gpio_addr + GPSET0/4;
  gpclr0 = (uint32_t*)gpio_addr + GPCLR0/4;

  cm_pwmctl = (uint32_t*)clock_addr + CM_PWMCTL/4;
  cm_pwmdiv = (uint32_t*)clock_addr + CM_PWMDIV/4;
  cm_pcmctl = (uint32_t*)clock_addr + CM_PCMCTL/4;
  cm_pcmdiv = (uint32_t*)clock_addr + CM_PCMDIV/4;

  pwm_ctl = (uint32_t*)pwm_addr + PWM_CTL/4;
  pwm_rng1 = (uint32_t*)pwm_addr + PWM_RNG1/4;
  pwm_dat1 = (uint32_t*)pwm_addr + PWM_DAT1/4;
  
  pcm_cs = (uint32_t*)pcm_addr + PCM_CS/4;
  pcm_mode = (uint32_t*)pcm_addr + PCM_MODE/4;
  pcm_fifo = (uint32_t*)pcm_addr + PCM_FIFO/4;
  pcm_txc = (uint32_t*)pcm_addr + PCM_TXC/4;
  pcm_rxc = (uint32_t*)pcm_addr + PCM_RXC/4;
}



