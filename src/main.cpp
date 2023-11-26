#include "VFD.h"



VFD_Display vfd_display;

void setup()
{



    Serial.begin(9600);
    vfd_display.initialize();

    vfd_display.reset();

    vfd_display.set_brightness(15);
    vfd_display.test_digits(2);
    delay(2000);
    vfd_display.test_digits(0);

    vfd_display.send_chars("            ");
}

void loop()
{
    char scrollchars[13];
    String str = "AAA!        AAAAAAA!      TEST    H ";
    static int scrollpos;
    for(int i=0;i<12;i++)
    {
        scrollchars[i]=str[(i+scrollpos)%str.length()];
    }
    scrollchars[12]=0;
    scrollpos++;
    
    vfd_display.send_chars(scrollchars);
    delay(50);
}
