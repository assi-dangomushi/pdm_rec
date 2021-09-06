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

// pi_perif.h ver 0.1

#pragma once

// Uncomment one of the following lines
//#define PERI_BASE 0x20000000 //pi0
//#define PERI_BASE 0x3F000000 //pi2 pi3
//#define PERI_BASE 0xFE000000 //pi4

#define GPIO_BASE (PERI_BASE + 0x200000) // GPIO controller
#define PCM_BASE (PERI_BASE + 0x203000) // PCM/I2S
#define CLOCK_BASE (PERI_BASE + 0x101000) // Clocks
#define PWM_BASE (PERI_BASE + 0x20C000) // PWM 
#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// CLOCK
#define CLK_PASSWD 0x5a000000
#define CM_GP0CTL 0x70
#define CM_GP0DIV 0x74
#define CM_GP1CTL 0x78
#define CM_GP1DIV 0x7c
#define CM_GP2CTL 0x80
#define CM_GP2DIV 0x84
#define CM_PCMCTL 0x98
#define CM_PCMDIV 0x9c
#define CM_PWMCTL 0xa0
#define CM_PWMDIV 0xa4

// OSC=19.2MHz PLLC=1000MHz PLLD=500MHz HDMI=216MHz
#define OSC 1
#define PLLC 5
#define PLLD 6
#define HDMI 7

// PWM
#define PWM_CTL 0x0
#define PWM_STA 0x4
#define PWM_RNG1 0x10
#define PWM_DAT1 0x14
#define PWM_RNG2 0x20
#define PWM_DAT2 0x24

// PCM
#define PCM_CS 0x0
#define PCM_FIFO 0x4
#define PCM_MODE 0x8
#define PCM_RXC 0xc
#define PCM_TXC 0x10
#define PCM_DREQ 0x14
#define PCM_INTEN 0x18
#define PCM_GRAY 0x20

// GPIO
#define GPFSEL0 0x0
#define GPFSEL1 0x4
#define GPFSEL2 0x8
#define GPFSEL3 0xc
#define GPFSEL4 0x10
#define GPFSEL5 0x14

#define FSEL_INPUT 0
#define FSEL_OUTPUT 1
#define FSEL_ALT0 4
#define FSEL_ALT1 5
#define FSEL_ALT2 6
#define FSEL_ALT3 7
#define FSEL_ALT4 3
#define FSEL_ALT5 2

#define GPSET0 0x1c
#define GPSET1 0x20
#define GPCLR0 0x28
#define GPCLR1 0x2c
#define GPLEV0 0x34
#define GPLEV1 0x38

extern void *gpio_addr;
extern void *clock_addr;
extern void *pwm_addr;
extern void *pcm_addr;

extern volatile uint32_t *gpfsel0;
extern volatile uint32_t *gpfsel1;
extern volatile uint32_t *gpfsel2;
extern volatile uint32_t *gpfsel3;
extern volatile uint32_t *gpfsel4;
extern volatile uint32_t *gpfsel5;
extern volatile uint32_t *gplev0;
extern volatile uint32_t *gplev1;
extern volatile uint32_t *gpset0;
extern volatile uint32_t *gpclr0;

extern volatile uint32_t *cm_pwmctl;
extern volatile uint32_t *cm_pwmdiv;
extern volatile uint32_t *cm_pcmctl;
extern volatile uint32_t *cm_pcmdiv;

extern volatile uint32_t *pwm_ctl;
extern volatile uint32_t *pwm_rng1;
extern volatile uint32_t *pwm_dat1;
  
extern volatile uint32_t *pcm_cs;
extern volatile uint32_t *pcm_mode;
extern volatile uint32_t *pcm_fifo;
extern volatile uint32_t *pcm_txc;
extern volatile uint32_t *pcm_rxc;

void init_perif(void);


