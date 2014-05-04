/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.0 Professional
Automatic Program Generator
� Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : sklo_pidjomnuk
Version : 
Date    : 27.06.2013
Author  : Pier
Company : 
Comments: 


Chip type               : ATmega8
Program type            : Application
AVR Core Clock frequency: 8,000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************

����� I ������������
pc0 - ���� 
pc1 - ����
����� II ������������
pc2 - ����
pc3 - ����
������
pc4 - ��������
pc5 - �����������

pb1 pb2 - � ��������


̳������������ �� ���� �������� �� 8,000000 MHz �� ����������� ����������

*****************************************************/


#include <mega8.h>

#include <delay.h>


#define LONG_TIME 30 /*30 == 3 ���*/

#define uint unsigned int

#define BUTTON1 4
#define BUTTON2 5

char long1up = 0, long2up = 0, long1down = 0, long2down = 0;


#define ADC_VREF_TYPE 0x40

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
// Wait for the AD conversion to complete
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCW;
}

#define MAX_VAL 1024

void check_action(uint val, char num)
{
    
    if(val < MAX_VAL * 10/100) // ��� ���������, 
    {
        if(1 == num && 0 == long1up && 0 == long1down)
        {
            PORTC.0 = 0;
            PORTC.1 = 0;
        }
        else if(2 == num && 0 == long2up && 0 == long2down)
        {
            PORTC.2 = 0;
            PORTC.3 = 0;
        }        
    }else if (val >  MAX_VAL * 90/100) //������� 1   - ������ �����
    {
        if(1 == num && 0 == long2up && 0 == long2down)
        {
            long1up = LONG_TIME;
            long1down = 0; 
            PORTC.1 = 0;
            PORTC.0 = 1;
        }
        else if(2 == num && 0 == long1up && 0 == long1down)
        {
            long2up = LONG_TIME;
            long2down = 0; 
            PORTC.3 = 0;
            PORTC.2 = 1;
        }        
    }
    else if ((val < MAX_VAL * 40/100)) //������� 2   - �������� ����
    {
        if(1 == num && 0 == long1up && 0 == long1down)
        {
            PORTC.1 = 0;
            PORTC.0 = 1;
        }
        else if(2 == num && 0 == long2up && 0 == long2down)
        {
            PORTC.3 = 0;
            PORTC.2 = 1;
        }
        
        if(1 == num && 0 < long1down)
        {
            long1down = 0;
            PORTC.1 = 0;
            PORTC.0 = 0;
            delay_ms(300);            
        }
        else if(2 == num && 0 < long2down)
        {                       
            long2down = 0;
            PORTC.3 = 0;
            PORTC.2 = 0;
            delay_ms(300);
        }
                
    }
    else if ((val < MAX_VAL * 70/100)) //������� 4   - ������ ����
    {
        if(1 == num && 0 == long2up && 0 == long2down)
        {
            long1up = 0;
            long1down = LONG_TIME; 
            PORTC.0 = 0;
            PORTC.1 = 1;
        }
        else if(2 == num && 0 == long1up && 0 == long1down)
        {   
            long2up = 0;
            long2down = LONG_TIME; 
            PORTC.2 = 0;
            PORTC.3 = 1;
        }        
    }
    else if ((val < MAX_VAL * 90/100)) //������� 3   - �������� ���� 
    {
        if(1 == num && 0 == long1up && 0 == long1down)
        {
            PORTC.0 = 0;
            PORTC.1 = 1;
        }
        else if(2 == num && 0 == long2up && 0 == long2down)
        {
            PORTC.2 = 0;
            PORTC.3 = 1;
        }        

        if(1 == num && 0 < long1up )
        {
            long1up = 0;
            PORTC.0 = 0;
            PORTC.1 = 0;
            delay_ms(300);
        }
        else if(2 == num && 0 < long2up)
        {                       
            long2up = 0;
            PORTC.2 = 0;
            PORTC.3 = 0;
            delay_ms(300);
        }        
    }
}

void close_all()
{
    PORTC.0 = 0;
    PORTC.1 = 0;
    PORTC.2 = 0;
    PORTC.3 = 0;

    long1down = 0;  
    long2down = 0;  

    long1up = LONG_TIME;
    long2up = LONG_TIME;
}

// Declare your global variables here
void main(void)
{
// Declare your local variables here
uint bt1 = 0, bt2 = 0;

long1up = 0; long2up = 0; long1down = 0; long2down = 0;

// Input/Output Ports initialization
// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0b1111;
DDRB=0x00;

// Port C initialization
// Func6=In Func5=In Func4=In Func3=Out Func2=Out Func1=Out Func0=Out 
// State6=T State5=T State4=T State3=0 State2=0 State1=0 State0=0 
PORTC=0x00;
DDRC=0x0F;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0xfF;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
TCCR0=0x00;
TCNT0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
MCUCR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;

// USART initialization
// USART disabled
UCSRB=0x00;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC Clock frequency: 500,000 kHz
// ADC Voltage Reference: AVCC pin
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x81;

// SPI initialization
// SPI disabled
SPCR=0x00;

// TWI initialization
// TWI disabled
TWCR=0x00;
 
while (1)
      {
      // Place your code here
      
        bt1 = read_adc(BUTTON1);
        check_action(bt1, 1);

        bt2 = read_adc(BUTTON2);
        check_action(bt2, 2);          

        if(long1up > 0)
        {                         
            PORTC.2 = 0;
            PORTC.3 = 0;
            PORTC.1 = 0;
            PORTC.0 = 1;
            long1up--;
        }
        else if(long2up > 0)
        {
            PORTC.1 = 0;
            PORTC.0 = 0;
            PORTC.3 = 0;
            PORTC.2 = 1;
            long2up--;
        }
        else if(long1down > 0)
        {
            long1down--;
        }
        else if(long2down > 0)
        {
            long2down--;
        }

        if(0 == PINB.1 || 1 == PINB.2)
        {
            close_all();
            PORTD.0 = 1;
        }   
        else
        {
            PORTD.0 = 0;
        }
                        
        delay_ms(100);
        
      
        /*PORTC.0 = 1;
        delay_ms(volt);
        PORTC.0 = 0;
        
        PORTC.1 = 1;
        delay_ms(volt);
        PORTC.1 = 0;

        PORTC.2 = 1;
        delay_ms(volt);
        PORTC.2 = 0;

        PORTC.3 = 1;
        delay_ms(volt);
        PORTC.3 = 0;           */
      }
}
