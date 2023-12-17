#pragma once

#include <Arduino.h>

////////////////////////////////
/*Configuration to be modified*/
////////////////////////////////

//#define VFD_ARDUINO_UNO           //Uncomment when using arduino or similar
//#define VFD_ESP                   //Uncomment when using ESP32 or similar
#define VFD_RP2040                  //Uncomment when using RP2040

//ESP Config
#define VFD_ESP_PWM_CHAN 1 //The timer channel used for driving filament on DRIVE_PIN

///////////////////////////
/*Defaults to be modified*/
///////////////////////////

//DEFAULTS FOR UNO
#ifdef VFD_ARDUINO_UNO
#define VFD_CLOCK_PIN   27
#define VFD_DATA_PIN    14
#define VFD_DRIVE_PIN   VFD_DRIVE_UNO_PIN_10   //This pin goes to V_ON.
#define VFD_SELECT_PIN  26
#define VFD_RESET_PIN   25
#endif

//DEFAULTS FOR ESP
#if defined VFD_ESP || defined VFD_RP2040
#define VFD_CLOCK_PIN   11
#define VFD_DATA_PIN    10
#define VFD_DRIVE_PIN   9 //VFD_DRIVE_UNO_PIN_10   //This pin goes to V_ON.
#define VFD_SELECT_PIN  12
#define VFD_RESET_PIN   13
#endif

////////////////////////////////////////////////////
/*Special config. These do not have to be modified*/
////////////////////////////////////////////////////

/*Only works for ESP and RP2040. Uno is preset to 17000*/
#define VFD_DRIVE_FREQUENCY 17000
/*Not expected to change.*/
#define VFD_DIGITS      12 

//REGISTERS
#define VFD_REG_DCRAM_W         0x10
#define VFD_CMD_DUTY            0x50
#define VFD_CMD_DIGITS          0x60
#define VFD_CMD_TEST            0x70


///////////////////////////////////////////////////
/*Functions, classes and more. Not to be modified*/
///////////////////////////////////////////////////

#ifdef VFD_ARDUINO_UNO
/*On the arduino UNO, you only have a few timer pins. These are defined in E_vfd_drive_pins enum.*/
enum E_vfd_drive_pins
{
    VFD_DRIVE_UNO_PIN_3 =   3,
    VFD_DRIVE_UNO_PIN_11 =  11,
    VFD_DRIVE_UNO_PIN_9 =   9,
    VFD_DRIVE_UNO_PIN_10 =  10
};

/*Used to configurate the VFD if non default pins are used.*/
struct S_VFD_parameters
{
    int clock_pin = VFD_CLOCK_PIN;
    int data_pin = VFD_DATA_PIN;
    enum E_vfd_drive_pins drive_pin = VFD_DRIVE_PIN;
    int select_pin = VFD_SELECT_PIN;
    int reset_pin = VFD_RESET_PIN;
};
#endif

#if defined VFD_ESP || defined VFD_RP2040

/*Used to configurate the VFD if non default pins are used.*/
struct S_VFD_parameters
{
    int clock_pin = VFD_CLOCK_PIN;
    int data_pin = VFD_DATA_PIN;
    int drive_pin = VFD_DRIVE_PIN;
    int select_pin = VFD_SELECT_PIN;
    int reset_pin = VFD_RESET_PIN;
};
#endif

/*VFD class. Contains the functions to control the VFD.*/
class VFD_Display
{
public:
    
    VFD_Display( S_VFD_parameters parameters = {} );
    ~VFD_Display();

    /*Initializes the display. Depending on the microcontroller, this sets the configuration, pins and timers.*/
    void initialize();
    /*Resets the display*/
    void reset();
    /*Sends a null-terminated char array to the display.*/
    void send_chars( const char* chars );
    /*0-15*/
    void set_brightness( const char b );
    /*0=no test, 1=all off, 2=all on*/
    void test_digits( const char s );
    
    
private:
    S_VFD_parameters params;
    String current_string;

    char translate_char_to_vfd( const char c );
    void send_raw_byte( char byte );
    void send_raw_bytes( char* bytes, int count );
};