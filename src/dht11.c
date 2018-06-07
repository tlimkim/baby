#include "main.h"

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXTIMINGS    83
#define DHTPIN        7 //wPi pin. physical num 7

int dht11_dat[5] = {0, 0, 0, 0, 0};
int lcdflag_ = 0;

void read_dht11_dat()
{
    if (wiringPiSetup() == -1) {
      printf("here \n");
	    exit(1);
    }
    delay(1000);

  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  float f; 
  int loop=0;

  dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, LOW);
  delay(18);
  digitalWrite(DHTPIN, HIGH);
  delayMicroseconds(35);
  pinMode(DHTPIN, INPUT);
 
  for (i=0;i<MAXTIMINGS;i++)
  {
    loop=i;
    counter = 0;
    while (digitalRead(DHTPIN)==laststate)
    {
      counter++;
      delayMicroseconds(1);
      if (counter==255)
        break;
    }
    laststate = digitalRead(DHTPIN);

    if (counter==255) 
      break;
    
    if ((i>=4)&&(i%2==0))
    {
      dht11_dat[j/8]<<=1;
      if (counter>25)
        dht11_dat[j/8] |= 1;
        j++;
    }
  }

  if (j<40)
     return;
	
  /*
   * debug    
   */
  /*
   printf("j:: %d\n",j);
  for (i=0;i<5;i++){
    printf("%d: %d\n",i,dht11_dat[i]);
  }
  */

  if ((j>=40)&&(dht11_dat[4]==((dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3])&0xFF))) {

    f = dht11_dat[2] * 9. / 5. + 32;
    printf("[Hum] %d.%d %% \n[Temp] %d.%d C (%.1f F)\n",      
    dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
    
    if (dht11_dat[0] >= 78) {
      lcd_print("Watch out your Baby ");
      lcdflag_ = 1;
    } else if (dht11_dat[0] <= 78 && lcdflag_ == 1) {
      lcd_clear();
    }
  } else {
    printf("Data not good, skip\n");
  }
}
/*
int 
main( void )
{
  printf("Raspberry Pi wiringPi DHT11 Temperature test program\n");

  if (wiringPiSetup() == -1)
    exit(1);

  while (1)
  {
    read_dht11_dat();
    delay(1000); 
  }
  return(0);
}*/
      
