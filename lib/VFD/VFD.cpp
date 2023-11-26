#include <VFD.h>
#include <Arduino.h>


VFD_Display::VFD_Display( S_VFD_parameters parameters ) : params(parameters)
{
    pinMode(params.clock_pin, OUTPUT);  
    pinMode(params.data_pin, OUTPUT);  
    pinMode(params.drive_pin, OUTPUT);  
    pinMode(params.reset_pin, OUTPUT);  
    pinMode(params.select_pin, OUTPUT);    

    digitalWrite(params.drive_pin, HIGH);
    digitalWrite(params.clock_pin, HIGH);
    digitalWrite(params.reset_pin, HIGH);


}

VFD_Display::~VFD_Display()
{
    digitalWrite(params.drive_pin, HIGH); 
    digitalWrite(params.reset_pin, LOW);
}

void VFD_Display::initialize()
{
    
#ifdef VFD_ARDUINO

    switch(params.drive_pin)
    {
        case 3:
        {
            TCCR2A = 1<<COM2B0 | 1<<WGM21;
            TCCR2B = 1<<CS21 | 1<<CS20;
            OCR2B = 14;
            OCR2A = 14;
            break;
        }

        case 11:
        {
            TCCR2A = 1<<COM2A0 | 1<<WGM21;
            TCCR2B = 1<<CS21 | 1<<CS20;
            OCR2A = 14;
            break;
        }

        case 9:
        {
            TCCR1A = 1<<COM1A0;
            TCCR1B = 1<<CS10 | 1<<WGM12;
            OCR1A = 457;
            break;			
        }

        case 10:
        {
            TCCR1A = 1<<COM1B0;
            TCCR1B = 1<<CS10 | 1<<WGM12;
            OCR1B = 457;
            OCR1A = 457;
            break;			
        }

        default:
        break;
    }

    //Set digits. Not expected to change
    char cmd = VFD_CMD_DIGITS;
    cmd = cmd | VFD_DIGITS;
    send_raw_bytes(&cmd, 1);

#endif
}

void VFD_Display::reset()
{
    digitalWrite(params.reset_pin,LOW);
    delayMicroseconds(10);
    digitalWrite(params.reset_pin,HIGH);
    delayMicroseconds(10);
    
}


void VFD_Display::send_raw_byte( char byte )
{
    
    char bit=0;
    for(int i=0;i<8;i++)
    {
        bit = byte & 0x01;
        byte = byte >> 1;

        digitalWrite(params.clock_pin, LOW); 
        digitalWrite(params.data_pin, bit);
        delayMicroseconds(1);
        digitalWrite(params.clock_pin, HIGH); 
        delayMicroseconds(1);
    }
    
}

void VFD_Display::send_raw_bytes( char* bytes, int count )
{
    digitalWrite(params.select_pin, LOW);
    delayMicroseconds(1);    
    for(int i=0;i<count;i++)
    {
        send_raw_byte(bytes[i]);
        delayMicroseconds(8);   //Data processing time
    }
    delayMicroseconds(16);  //Hold time
    digitalWrite(params.select_pin, HIGH);
}

void VFD_Display::send_chars( const char* chars )
{
    //Reverse the string and fill spaces after null terminator. Also add a new null terminator at the end
    char str[VFD_DIGITS+1]={0};
    static int empty;
    empty=0;
    for(int i=0;i<VFD_DIGITS;i++)
    {
        if(chars[i]==0 || empty==1)
        {
            empty = 1;
            str[VFD_DIGITS-i-1]=' ';
        }else{
            str[VFD_DIGITS-i-1]=chars[i];
        }
        
    }
    str[VFD_DIGITS]=0;

    //Select device
    digitalWrite(params.select_pin, LOW);
    delayMicroseconds(1);   
    //Send DCRAM data write
    send_raw_byte(VFD_REG_DCRAM_W);
    delayMicroseconds(8); 
    //Translate and send the chars
    for(int i=0;i<VFD_DIGITS;i++)
    {
        char b = str[i];
        if(b==0){break;}
        char c = translate_char_to_vfd(b);
        send_raw_byte(c);
        delayMicroseconds(8);   //Data processing time
    }
    delayMicroseconds(16);  //Hold time

    //Deselect
    digitalWrite(params.select_pin, HIGH);
}

char VFD_Display::translate_char_to_vfd( const char c )
{
    if(c >= ' ' && c <= '?')
    {
        return c+16;
    }    
    if(c >= '@' && c <= 'Z')
    {
        return c-48;
    }
    if(c<0)
    {
        return -c;    
    }
    //>80
    return c;
}

void VFD_Display::set_brightness( const char b )
{
    char duty = VFD_CMD_DUTY;
    char cmd = duty | b;
    send_raw_bytes(&cmd, 1);
}

void VFD_Display::test_digits( const char s )
{
    char test = VFD_CMD_TEST;
    char cmd = test | s;
    send_raw_bytes(&cmd, 1);
}