#ifndef USB_DEVICE_HID_PS3_DANCE_PAD_DRIVER_H_
#define USB_DEVICE_HID_PS3_DANCE_PAD_DRIVER_H_

#include "usb/device_driver.h"

#include "class/hid/hid_device.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __attribute((packed, aligned(1))) {
    uint8_t buttons1;
    uint8_t buttons2;
    uint8_t hat;
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t rz;

    uint8_t vendor_right;
    uint8_t vendor_left;
    uint8_t vendor_up;
    uint8_t vendor_down;
    uint8_t vendor_north;
    uint8_t vendor_east;
    uint8_t vendor_south;
    uint8_t vendor_west;
    uint8_t vendor_0x28;
    uint8_t vendor_0x29;
    uint8_t vendor_0x2A;
    uint8_t vendor_0x2B;

    uint16_t vendor_0x2C;
    uint16_t vendor_0x2D;
    uint16_t vendor_0x2E;
    uint16_t vendor_0x2F;
} hid_ps3_dance_pad_report_t;

const usbd_driver_t *get_hid_ps3_dance_pad_device_driver();

extern const uint8_t ps3_dance_pad_desc_hid_report[];

uint16_t hid_ps3_dance_pad_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer,
                                         uint16_t reqlen);
void hid_ps3_dance_pad_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type,
                                     uint8_t const *buffer, uint16_t bufsize);

#ifdef __cplusplus
}
#endif

#endif // USB_DEVICE_HID_PS3_DANCE_PAD_DRIVER_H_