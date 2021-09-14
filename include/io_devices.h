/*
 * File: io_devices.h
 * Description: Class for handling io devices for space invaders
 */

#ifndef IO_DEVICES
#define IO_DEVICES
 #include <string>

class Io_devices {
public:
    /* Public interface */
	void    Initialize();
    uint8_t Read_device(uint8_t device_number);
    void    Write_device(uint8_t device_number, uint8_t data);

private:
    /* 16-bit Shift register on device 3 */
    uint16_t shift_register_value;
    uint8_t  shift_register_offset;

    uint8_t Get_shift_register_result();
    void    Set_shift_register_result_offset(uint8_t amount);
    void    Fill_shift_register(uint8_t data);
};

#endif
