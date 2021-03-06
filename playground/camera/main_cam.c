#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <softPwm.h>
#include <pcf8591.h>
#include <pthread.h>
#include <signal.h>


#define PCF 120
#define SERVO 1
#define butPin 23
#define ledPin 17


int Joystick_Read(void);
void servo_control(int tmp);
void *thread_joystick(void *arg);
int camera(void);

char message[] = "";
//raspistill -o image.jpg


int main(void){
	int tmp;
	//if(wiringPiSetupGpio()==-1)
	//	return 1;

	wiringPiSetup ();
	pinMode(butPin,INPUT);
	pinMode(ledPin,OUTPUT);
	
 	
 	pcf8591Setup (PCF, 0x48) ; // Setup pcf8591 on base pin 120, and address 0x48
 	softPwmCreate(SERVO,0,200);

	int res[2];
  	pthread_t a_thread[2];
  	void *thread_result;

  	res[0] = pthread_create(&a_thread[0], NULL, thread_joystick, (void *)message);
 	//res[1] = pthread_create(&a_thread[1], NULL, thread_camera, (void *)message);
 	if (res[0] != 0) {
   	    perror("Thread creation failed");
    	    exit(EXIT_FAILURE);
  	}
  	  printf("Waiting for thread to finish...\n");	
	 camera();
 		
	res[0] = pthread_join(a_thread[0], &thread_result);
	//res[1] = pthread_join(a_thread[1], &thread_result);
 	if (res[0] != 0) {
   	    perror("Thread join failed");
    	    exit(EXIT_FAILURE);
   	}
   	printf("Thread joined, it returned %s\n", (char *)thread_result);
   	printf("Message is now %s\n", message);
    	exit(EXIT_SUCCESS);
	
 	return 0 ;
}


int Joystick_Read(void)
{
	int tmp, value[3];

	value[0] = analogRead (PCF + 0) ;
 	//printf("x = %d ", value[0]);
 	value[1] = analogRead (PCF + 1) ;
 	//printf("y = %d ", value[1]);
 	value[2] = analogRead (PCF + 2) ;
 	//printf("z = %d \n", value[2]);
 	if(value[0]==value[1] && value[1]==value[2])
    tmp = 0; // printf("%s\n", dir[0]); // left
 	if(value[1] > 250 && value[0] < 225 && value[2] < 225)
    tmp = 2; // printf("%s\n", dir[2]); // up
 	if(value[1] < 200 && value[1] != value[2] && value[1] != value[0]) // down
    tmp = 3; // printf("%s\n", dir[3]);
 	if(value[2]<10 && value[0] > 150 && value[0] < 200 && value[1] >200 && value[1] <250)  // right
    tmp = 1; // printf("%s\n", dir[1]);
 	if(value[0]<10 && value[1] >200 && value[1] < 250 && value[2] > 150 && value[2] <200) // button
    tmp = 4; // printf("%s\n", dir[4]);

    return tmp;
}

void servo_control(int tmp)
{
		if(tmp== 1)softPwmWrite(SERVO,15);	//0  degree
		else if(tmp== 2)softPwmWrite(SERVO,24);//90 degree
		else if(tmp== 3)softPwmWrite(SERVO,5);//-90 degree
		else if(tmp== 4)return;
}

void *thread_joystick(void *arg) {
	while(1)
	{
	int tmp = Joystick_Read();
	servo_control(tmp);
	delay(100);
	}
	return((void *)0);
}

int camera(void){	
		const char *command="raspistill -o image.jpg";
		FILE *fp=NULL;

		
		while(1){
		int ret=digitalRead(butPin);

		if(ret==0)
		{		
			//digitalWrite(ledPin,HIGH);
			printf("pressed!");
			//system("raspistill -o image.jpg");
			fp=popen(command,"r");
			pclose(fp);
			printf("close\n");
		
		}
		else if(ret==1){
			//delay(5);
			printf("w\n");
		}
		
		}


}

