#pragma once

#include <Arduino.h>

#define VFD_ARDUINO         //Uncomment when using arduino or similar
//#define VFD_ESP             //Uncomment when using ESP32 or similar


//DEFAULTS
#define VFD_CLOCK_PIN   4
#define VFD_DATA_PIN    5
#define VFD_DRIVE_PIN   VFD_DRIVE_UNO_PIN_10   //This pin goes to V_ON.
#define VFD_SELECT_PIN  3
#define VFD_RESET_PIN   2

//DISP INFO
#define VFD_DIGITS      12 //Not expected to change

//REGISTERS
#define VFD_REG_DCRAM_W         0x10
#define VFD_CMD_DUTY            0x50
#define VFD_CMD_DIGITS          0x60
#define VFD_CMD_TEST            0x70

#ifdef VFD_ARDUINO
enum E_vfd_drive_pins
{
    VFD_DRIVE_UNO_PIN_3 = 3,
    VFD_DRIVE_UNO_PIN_11 = 11,
    VFD_DRIVE_UNO_PIN_9 = 9,
    VFD_DRIVE_UNO_PIN_10 = 10
};

struct S_VFD_parameters
{
    int clock_pin = VFD_CLOCK_PIN;
    int data_pin = VFD_DATA_PIN; 
    enum E_vfd_drive_pins drive_pin = VFD_DRIVE_PIN; 
    int select_pin = VFD_SELECT_PIN; 
    int reset_pin = VFD_RESET_PIN;
};

#endif

class VFD_Display
{
public:
    
	VFD_Display( S_VFD_parameters parameters = {} );

    ~VFD_Display();

	void initialize();
	void reset();
    void send_chars( const char* chars );

    /*0-15*/
    void set_brightness( const char b ); 
    /*0=no test, 1=all off, 2=all on*/
    void test_digits( const char s ); 

    //Utility
    void send_raw_byte( char byte );
	void send_raw_bytes( char* bytes, int count  );
    char translate_char_to_vfd( const char c );
private:
    S_VFD_parameters params;
    String current_string;
};