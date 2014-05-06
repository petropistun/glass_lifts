/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.0 Professional
Automatic Program Generator
© Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
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

вихід I склопійомника
pc0 - верх 
pc1 - вниз
вихід II склопійомника
pc2 - верх
pc3 - вниз
Кнопки
pc4 - водійська
pc5 - пасажирський виключатель на водійські дверці


Мікроконтролер має бути прошитий на 8,000000 MHz від внутрішнього генератора

pc4, pc5 - через 1 кОм, мають бути підтягнути на +
 - інший контакт кнопки маю бути на корпусі 

*****************************************************/


#include <mega8.h>

#include <delay.h>


#define uint unsigned int

#define BUTTON1 4
#define BUTTON2 5

#define PIN_ON 0 
#define PIN_OFF 1


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

#define PULSE_TIME 25

#define ACCURACY 10

char is_near(uint val1, uint val2)
{
    if (val1 < val2 + val2*ACCURACY/100 && val1 > val2 - val2*ACCURACY/100)
        return 1;
    else
        return 0;
}

char wait_for_button(char num, uint val)
{
    unsigned char adc_input = (1 == num)?BUTTON1:BUTTON2; 
    
    uint new_val = read_adc(adc_input), new_val2;
    delay_ms(15);
    new_val2 = read_adc(adc_input);
    
    if (is_near(val, new_val) && is_near(val, new_val2))
        return 1;
    else
        return 0;
}

char wait_for_release_button(char num)
{
    unsigned char adc_input = (1 == num)?BUTTON1:BUTTON2; 
                                                           
    while(1)
    {
        uint new_val = read_adc(adc_input), new_val2;
        delay_ms(40);
        new_val2 = read_adc(adc_input);
    
        if (is_near(new_val, new_val) && new_val > MAX_VAL * 90/100)
            return 1;
            
        delay_ms(25);
    }
}

void check_action(uint val, char num)
{
    
    if(val > MAX_VAL * 90/100) // все виключено, 
    {
        if(1 == num)
        {
            PORTC.0 = PIN_OFF;
            PORTC.1 = PIN_OFF;
        }
        else if(2 == num)
        {
            PORTC.2 = PIN_OFF;
            PORTC.3 = PIN_OFF;
        }        
    }else if (val <  MAX_VAL * 10/100) //позиція 1   - довгий вверх
    {
        if(1 == num)
        {
            PORTC.1 = PIN_OFF;
            PORTC.0 = PIN_ON;                      
            while( 1 == wait_for_button(num, val)) //чекаємо поки невідпуститься кнопка
            {
               delay_ms(10);
            }            
            
            delay_ms(500); //якщо кнопка відпустилася, чекаю ще трогкши, щоб вона пройшла через короткий режим
                           
        }
        else if(2 == num)
        {
            PORTC.3 = PIN_OFF;
            PORTC.2 = PIN_ON; 
            while( 1 == wait_for_button(num, val)) //чекаємо поки невідпуститься кнопка
            {
               delay_ms(10);
            }            
            
            delay_ms(500); //якщо кнопка відпустилася, чекаю ще трогкши, щоб вона пройшла через короткий режим

        }        
    }
    else if ((val > MAX_VAL * 60/100)) //позиція 2   - короткий верх
    {
        if(1 == num )
        {
            PORTC.1 = PIN_OFF;
            PORTC.0 = PIN_ON;

            while( 1 == wait_for_button(num, val)) //чекаємо поки невідпуститься кнопка
            {
               delay_ms(10);
            }            
            
            //якщо кнопка відпустилася, короткочасно подаємо зворотній імпульс
            PORTC.1 = PIN_ON;
            PORTC.0 = PIN_OFF;
            delay_ms(PULSE_TIME); 
            PORTC.1 = PIN_OFF;
            PORTC.0 = PIN_OFF;


        }
        else if(2 == num)
        {
            PORTC.3 = PIN_OFF;
            PORTC.2 = PIN_ON;

            while( 1 == wait_for_button(num, val)) //чекаємо поки невідпуститься кнопка
            {
               delay_ms(10);
            }            
            
            //якщо кнопка відпустилася, короткочасно подаємо зворотній імпульс
            PORTC.3 = PIN_ON;
            PORTC.2 = PIN_OFF;
            delay_ms(PULSE_TIME); 
            PORTC.3 = PIN_OFF;
            PORTC.2 = PIN_OFF;
        }
        
    }
    else if ((val > MAX_VAL * 30/100)) //позиція 4   - довгий вниз
    {
        if(1 == num)
        {
            PORTC.0 = PIN_OFF;
            PORTC.1 = PIN_ON;                               

            while( 1 == wait_for_button(num, val)) //чекаємо поки невідпуститься кнопка
            {
               delay_ms(10);
            }            
            
            delay_ms(500); //якщо кнопка відпустилася, чекаю ще трогкши, щоб вона пройшла через короткий режим

        }
        else if(2 == num)
        {   
            PORTC.2 = PIN_OFF;
            PORTC.3 = PIN_ON;

            while( 1 == wait_for_button(num, val)) //чекаємо поки невідпуститься кнопка
            {
               delay_ms(10);
            }            
            
            delay_ms(500); //якщо кнопка відпустилася, чекаю ще трогкши, щоб вона пройшла через короткий режим
        }        
    }
    else if ((val > MAX_VAL * 10/100)) //позиція 3   - короткий вниз 
    {
        if(1 == num)
        {
            PORTC.0 = PIN_OFF;
            PORTC.1 = PIN_ON;

            while( 1 == wait_for_button(num, val)) //чекаємо поки невідпуститься кнопка
            {
               delay_ms(10);
            }            
            
            //якщо кнопка відпустилася, короткочасно подаємо зворотній імпульс
            PORTC.0 = PIN_ON;
            PORTC.1 = PIN_OFF;
            delay_ms(PULSE_TIME); 
            PORTC.0 = PIN_OFF;
            PORTC.1 = PIN_OFF;
        }
        else if(2 == num)
        {
            PORTC.2 = PIN_OFF;
            PORTC.3 = PIN_ON;

            while( 1 == wait_for_button(num, val)) //чекаємо поки невідпуститься кнопка
            {
               delay_ms(10);
            }            
            
            //якщо кнопка відпустилася, короткочасно подаємо зворотній імпульс
            PORTC.2 = PIN_ON;
            PORTC.3 = PIN_OFF;
            delay_ms(PULSE_TIME); 
            PORTC.2 = PIN_OFF;
            PORTC.3 = PIN_OFF;
        }        
    }
}

// Declare your global variables here
void main(void)
{
// Declare your local variables here
uint bt1 = 0, bt2 = 0, bt3 = 0;

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
        delay_ms(10);
        bt2 = read_adc(BUTTON1);
        delay_ms(10);
        bt3 = read_adc(BUTTON1);
           
        if( is_near(bt1, bt2) && is_near(bt1, bt3))
        { 
            check_action(bt1, 1);
        }

        bt1 = read_adc(BUTTON2);
        delay_ms(10);
        bt2 = read_adc(BUTTON2);
        delay_ms(10);
        bt3 = read_adc(BUTTON2);
        
        check_action(bt1, 2);          

      }
}
