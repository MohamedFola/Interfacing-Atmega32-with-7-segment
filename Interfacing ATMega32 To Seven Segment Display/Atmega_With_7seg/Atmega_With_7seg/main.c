/*
--> interfacing Atmega32/Atmega16  with 7 segment display
--> Author : Mohamed Hassan Fola

      --------------------------
     |             A            |
     |	        -------         |
0 <--|A	      ||	   ||       |
1 <--|B     F ||       || B     |
2 <--|C       ||   G   ||       |
3 <--|D		    -------         |
4 <--|E       ||       ||       |
5 <--|F	    E ||       || C     |
6 <--|G       ||       ||       |
7 <--|DP        -------	        | 
     |             D            |
      --------------------------
	   
--> Firstly for common anode : to turn a led on put '0' on the pin

                                           7 6 5 4 3 2 1 0
                                          DP G F E D C B A
for number 0 --> A, B, C, D, E, F      --> 1 1 0 0 0 0 0 0 --> 0xC0
for number 1 --> B, C                  --> 1 1 1 1 1 0 0 1 --> 0xF9
for number 2 --> A, B, D, E, G         --> 1 0 1 0 0 1 0 0 --> 0xA4
for number 3 --> A, B, C, D, G         --> 1 0 1 1 0 0 0 0 --> 0xB0
for number 4 --> B, C, F, G            --> 1 0 0 1 1 0 0 1 --> 0x99
for number 5 --> A, C, D, F, G         --> 1 0 0 1 0 0 1 0 --> 0x92
for number 6 --> A, C, D, E, F, G      --> 1 0 0 0 0 0 1 0 --> 0x82
for number 7 --> A, B, C               --> 1 1 1 1 1 0 0 0 --> 0xF8
for number 8 --> A, B, C, D, E, F, G   --> 1 0 0 0 0 0 0 0 --> 0x80
for number 9 --> A, B, C, D, F, G      --> 1 0 0 1 0 0 0 0 --> 0x90

----------------------------------------------------------------------

--> Secondly for common cathode : to turn a led on put '1' on the pin
                                  or you can easily change any '0' in 
								  common anode with '1'

                                           7 6 5 4 3 2 1 0
                                          DP G F E D C B A
for number 0 --> A, B, C, D, E, F      --> 0 0 1 1 1 1 1 1 --> 0x3F
for number 1 --> B, C                  --> 0 0 0 0 0 1 1 0 --> 0x06
for number 2 --> A, B, D, E, G         --> 0 1 0 1 1 0 1 1 --> 0x5B
for number 3 --> A, B, C, D, G         --> 0 1 0 0 1 1 1 1 --> 0x4F
for number 4 --> B, C, F, G            --> 0 1 1 0 0 1 1 0 --> 0x66
for number 5 --> A, C, D, F, G         --> 0 1 1 0 1 1 0 1 --> 0x6D
for number 6 --> A, C, D, E, F, G      --> 0 1 1 1 1 1 0 1 --> 0x7D
for number 7 --> A, B, C               --> 0 0 0 0 0 1 1 1 --> 0x07
for number 8 --> A, B, C, D, E, F, G   --> 0 1 1 1 1 1 1 1 --> 0x7F
for number 9 --> A, B, C, D, F, G      --> 0 1 1 0 1 1 1 1 --> 0x6F
*/

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>

#define Bush_Button_Pin   0
#define Btn_Pressed       ((PINA<<Bush_Button_Pin) & 1)

#define SSD_CC_Direction DDRD
#define SSD_CC_PORT      PORTD

#define SSD_CA_Direction DDRC
#define SSD_CA_PORT      PORTC


int main(void)
{
	int counter = 0;    // to count to 9 as we can't display more than 9 on the seven segment
	
	uint8_t SSD_Common_Anode[10]    = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
	uint8_t SSD_Common_Cathode[10]  = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
		
	// set the direction of port C & D as output which are connected to the two seven segment displays
	SSD_CC_Direction = 0xFF;
	SSD_CA_Direction = 0xFF;
	
	// set the pin of the bush button as input and make it pull up
	DDRA  &= ~(1<<Bush_Button_Pin);
	PORTA |=  (1<<Bush_Button_Pin);
    
    while (1) 
    {
		if (!Btn_Pressed) 
			if((++counter) > 9) counter = 0;
		_delay_ms(200);
		SSD_CC_PORT = SSD_Common_Cathode[counter];
		SSD_CA_PORT = SSD_Common_Anode[counter];
    }
}

