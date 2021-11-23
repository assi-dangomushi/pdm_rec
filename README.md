# pdm_rec
MEMS  Digital Microphone(PDM) Recording Program for Raspberry Pi

Record on memory, and output to stdout.

## Install


```
gcc -O2 -o pdm_start pdm_start.c pi_perif.c
gcc -O2 -o pdm_rec pdm_rec.c pi_perif.c
sudo cp pdm_start /usr/bin/
sudo cp pdm_rec /usr/bin/
```

## Usage

connect MEMS Microphone
GPIO 18 is PCM\_CLK
GPIO 20 is PCM\_DIN

You Must run pdm\_start befor pdm\_rec

`sudo pdm_satrt clock(MHz)`
 Example : sudo pdm_start 4.8

The actual clock will not exceed the specified value. 

`sudo pdm_rec duration ch`

duration(sec) is limited by memory.
ch: ch1 | ch2 | ch1+2

prevent overflow error, use `chrt`



for example:

```
sudo pdm_start 2.4
sudo chrt -f 99 pdm_rec 180 ch1 > rec01.dat
```

rec01.dat is

* 180sec monoral
* fs = 2.4MHz/16 = 150kHz
* 16bit signed LitteEndian
* RAW data

## convert to audio file

use SOX.
example: convert to 44.1kHz flac, gain +20dB

```
sox -t raw -c 1 -r 150000 -e s -b 16 rec01.dat  -b 24 -r 44100 rec01.flac gain 20
```


