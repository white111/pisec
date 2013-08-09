/*
Copyright (C) Dominik Guz

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/sem.h>
#include <time.h>
#include <wiringPi.h> 

#define NO 0
#define NC 1

#define HIGH 1
#define LOW 0

    void savehwstate();
    void shmini();
    void readstate();
	bool outputnow(int outno);
	bool softa = 0;

using namespace std;

/*
digitalWrite(int pin, int value) - write 0 or 1 to pin
digitalRead(int pin) - read pin value
*/

/* Define how many pins are used as inputs and outputs ,
*/

const char inPins = 10;
const char outPins = 7; 
const char* shmobjpath = "/var/www/html/pisecSHM";
const char shmprojid = 97;
const char shmsize = outPins+2;

/*
n = no = char(110)
y = yes = char(121)
r = reset = char(114)
used for compability with PHP IPC funcs that parse memory as chars and output string
*/

	//startup arm / violation state
	char armed = 110;
	char violation = 110;

	//synchronize with shared memory every X sec
	int websyncsec = 4;
	/*
	WiringPi Pins corresponding to GPIO like : 
	0-6 (GPIO 17, 18, 21(r1), 27(R2), 22, 23, 24, 25)
	7   (GPIO 4)
	PWM (GPIO 18)
	8-9 (GPIO 0,1(R1) | 2,3(R2)
	10-14 (GPIO 8, 7, 10, 9, 11)
	15-16 (GPIO 14, 15)
	17-20 (GPIO 28, 29, 30, 31) - R2
	*/

	//wiringPi pin numbers, corresponding to inputs/outputs, order has to match with port COMs (NO/NC) 
	char inPinNo[inPins] = {0, 1, 2, 3, 4, 5, 6, 10, 11, 12 }; //define wiringPi numbers for each input
	char outPinNo[outPins] = {8,9,13,14,15,16,17}; //define wiringPi numbers for each output

	char state[outPins+2]; //0-arm 1-violation 2-outPins - output state 
	char soa[outPins]; // software output activation 

	//hardware arm/disarm inputs, default to first two inputs 
	char armPinId = 0;
	char disarmPinId = 1;
	char armPin =   inPinNo[armPinId];
	char disarmPin = inPinNo[disarmPinId];

	int rc;

	//shared memory objects - full file path 

	//data object
    int shmid;
    void *shm;
    key_t key = ftok(shmobjpath, shmprojid);  
    
    void savehwstate();
    void shmini();
    void readstate();

	//define default states for each input
	bool inputsCOM[inPins] = { NO, NO, NO, NO, NO, NO, NO, NO, NO, NO };

	//default output activation states  
	// 1 - arm = 0 vio = 0 
	// 2 - arm = 1 vio = 0
	// 3 - arm = 0 vio = 1
	// 4 - arm = 1 vio = 1
	//5-10 - sum of previous 
	// >10 - always off
	int outputsCOM[outPins] = {1,2,4,11,11,11,11};


/*
Read and write system state to shared memory 
*/

	//ini shared memory creating if needed
	void shmini()
	{
		//create files if needed..

		//init SHM
		if ((shmid = shmget(key, shmsize, IPC_CREAT | 0666)) < 0)
		{
			perror("shmget");
			exit(1);
		}
		if ((shm = shmat(shmid, NULL, 0)) == (void *) -1) 
                {
			perror("shmat");
			exit(1);
		}	

	}

	void shmdeini()
	{
		    /* Detach the shared memory segment from the current process.    */
            shmdt(shm);
	}

	void savehwstate()
	{
		 memcpy((char *)shm, &state, sizeof(char)*shmsize);
	}

	void readstate()
	{
		 char lstate[outPins+2];
		 memcpy(lstate, &state, sizeof(char)*shmsize); //copy current state
		 memcpy(state, (char *)shm, sizeof(char)*shmsize); //copy from memory
		//check changed outputs
		for(int i=2; i<outPins+2;i++)
		{
			if (lstate[i] != state[i] ) //current output different then last - software activated
				 { 
				 soa[i-2] = state[i];
				 softa =1;
				 }
		}
	
	}

	
	void putout()
	{	
		for (int i  = 0; i<outPins; i++)
		{
				if ( outputnow(i) ) //system state and output condition match
					{
						digitalWrite(outPinNo[i], 1); 
						state[i+2] = 121; //y
					}
				else //deactivate output it not used in this state
					{  
						digitalWrite(outPinNo[i], 0); 
						state[i+2] = 110; //n
					} 

		}

	}


	bool outputnow(int outno)
	{
		int outputcon = outputsCOM[outno];
		bool conok = false;

		if ( soa[outno] == 114 )  //soa not used, check arm/vio
		{
		switch (outputcon)
		{
		case 1 : 
			if (armed == 110 && violation == 110) { conok = true; } 
			break;
		case 2 :
			if ( armed == 121 && violation == 110) {conok = true; }
			break;
		case 3 :
			if ( armed == 110 && violation == 121) {conok = true; } // 3 or 2 + 1
			break;
		case 4 :
			if ( armed == 121 && violation == 121) {conok = true; }  // 4 or 3 + 1
			break;
		case 5 : 
			if ( ( armed == 121 && violation == 110  ) || ( armed == 110 && violation == 121) ) {conok = true; } //case 3 + 2 or 5		
			break;
		case 6 :
			if ( ( armed == 110 && violation == 121) || ( armed == 121 && violation == 110)  || (armed == 110 && violation == 110)   )  {conok = true; } //case 3 + 2 + 1
			break;
		case 7 :
			if ( ( armed == 121 && violation == 121)  || ( armed == 110 && violation == 121) )  {conok = true; } //case 4 + 3
			break;
		case 8 :
			if (   ( armed == 121 && violation == 121)  ||  ( armed == 110 && violation == 121) ||  (armed == 110 && violation == 110) )  {conok = true; } //case  4 + 3 + 1
			break;
		case 9 :
			if (  ( armed == 121 && violation == 121) || ( armed == 110 && violation == 121)  || ( armed == 121 && violation == 110) )  {conok = true; } //case 4 + 3 + 2 
			break;
		case 10 :
			if (   ( armed == 121 && violation == 121) || ( armed == 110 && violation == 121)  || ( armed == 121 && violation == 110) || (armed == 110 && violation == 110)  )  {conok = true; } //case 
			break;
		default:
			conok = false;
		};

		} //soa = 114

		else if (soa[outno] == 121 )  //manual software activation
		 { conok = true; }
		else if ( soa[outno] == 110 ) {conok = false;}   //manual software deactivation

		return conok;
	}

	void setup()
	{
		//setup output / input pin direction and value
		for(int i = 0; i<outPins; i++) 
		{
			pinMode(outPinNo[i], OUTPUT);
			digitalWrite(outPinNo[i], LOW);	
		}
 

		for(int i = 0; i<inPins; i++) 
		{
			pinMode(inPinNo[i], INPUT);
			digitalWrite(inPinNo[i], LOW);	
		
		}

		//set default states
		fill(state, &state[outPins+2],110);
		fill(soa, &soa[outPins] , 114);

		//save default values to shared memorry before starting
		putout();
		savehwstate();
	}

	void sendalert()
	{
	//invoke python script, image + sms
	popen("sudo python photosms.py", "w");
	}


	int main()
	{

	daemon(1,1);
	if ( wiringPiSetup()  < 0 ) //call wiringPi init
	exit(-1);
	
	shmini();
	setup();

		clock_t lwebchk = clock();
		bool inputschanged = false;
		int sleepct = 0;

		while(1)
		{
			inputschanged = false; //reset inputs state

			if ( ((clock() - lwebchk) >  2*CLOCKS_PER_SEC ) || (sleepct == 2)  ) 	//get state from shared memory to sync with web input
			{ 
				readstate();				
				lwebchk = clock();
				sleepct = 0;
			}
			//by default arm pin is first input, disarm is second, change if needed
			if ( (digitalRead(armPin) != inputsCOM[armPinId]) || ( (state[0] - armed) > 0 )   )  //arm pin state different then normal or shared state armed  while local not
			{  
				armed = 121;
				state[0] = 121;
				putout(); //first output to save correct out states
				savehwstate(); //save arm state
			}
			else if ( (digitalRead(disarmPin) != inputsCOM[disarmPinId]) || ( (state[0] - armed )<0 ) )   //arm pin state different then normal or shared disarmed while local armed
			{
				armed = 110;
				state[0] = 110;
				violation = 110; //reset violation 
				state[1] = 110;
				putout();	
				savehwstate(); //save arm state
			}
			else if ( softa )   //changed by readstate()
			 { 
				putout();
			  } //run outputs if changed by software

			//check line  change
			for (int i = 0; i<inPins; i++)
			{
				if ( violation != 121 )  //check only if no ongoing alarm 
				{
					if ( (digitalRead(inPinNo[i]) != inputsCOM[i]) && (armed == 121) ) //input state different from normal, system armed 
					{
						violation = 121;
						state[1] = 121;
						inputschanged = true;
					}
				}
			}

			//write to shared and activate output after checking inputs, only if breached, and not already alarmed
			if (inputschanged)
			{
				putout();
				savehwstate();
			}
		sleep(1); 
		sleepct++;
		softa = 0;
		}
		
		//unlink from shared memory
		return 0;

	}
