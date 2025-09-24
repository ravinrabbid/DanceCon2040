#include "usb/device/hid/spice2x_driver.h"

#include "usb/device/hid/common.h"

#include "tusb.h"

const tusb_desc_device_t spice2x_desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_UNSPECIFIED,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x1209,
    .idProduct = 0x39DD,
    .bcdDevice = 0x0100,
    .iManufacturer = USBD_STR_MANUFACTURER,
    .iProduct = USBD_STR_PRODUCT,
    .iSerialNumber = USBD_STR_SERIAL,
    .bNumConfigurations = 1,
};

enum {
    USBD_ITF_HID,
    USBD_ITF_MAX,
};

const uint8_t spice2x_desc_hid_report[] = {
    0x05, 0x01, // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05, // Usage (Game Pad)
    0xA1, 0x01, // Collection (Application)
    0xA1, 0x02, //   Collection (Logical)
    0x85, 0x01, //     Report ID (1)
    0x09, 0x39, //   Usage (Hat switch)
    0x75, 0x04, //   Report Size (4)
    0x95, 0x01, //   Report Count (1)
    0x15, 0x00, //   Logical Minimum (0)
    0x25, 0x07, //   Logical Maximum (7)
    0x35, 0x00, //   Physical Minimum (0)
    0x46, 0x3B, //
    0x01,       //   Physical Maximum (315)
    0x65, 0x14, //   Unit (System: English Rotation, Length: Centimeter)
    0x81, 0x42, //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
    0x05, 0x09, //     Usage Page (Button)
    0x75, 0x01, //     Report Size (1)
    0x95, 0x0A, //     Report Count (10)
    0x19, 0x01, //     Usage Minimum (0x01)
    0x29, 0x0A, //     Usage Maximum (0x0A)
    0x15, 0x00, //     Logical Minimum (0)
    0x25, 0x01, //     Logical Maximum (1)
    0x35, 0x00, //     Physical Minimum (0)
    0x45, 0x01, //     Physical Maximum (1)
    0x81, 0x02, //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x01, //     Report Size (1)
    0x95, 0x02, //     Report Count (2)
    0x81, 0x03, //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x09, //     Usage Page (Button)
    0x75, 0x01, //     Report Size (1)
    0x95, 0x0B, //     Report Count (11)
    0x19, 0x0B, //     Usage Minimum (0x0B)
    0x29, 0x15, //     Usage Maximum (0x15)
    0x15, 0x00, //     Logical Minimum (0)
    0x25, 0x01, //     Logical Maximum (1)
    0x35, 0x00, //     Physical Minimum (0)
    0x45, 0x01, //     Physical Maximum (1)
    0x81, 0x02, //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x01, //     Report Size (1)
    0x95, 0x05, //     Report Count (5)
    0x81, 0x03, //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,       //   End Collection

    0xA1, 0x02, //   Collection (Logical)
    0x85, 0x02, //     Report ID (2)
    0x15, 0x00, //     Logical Minimum (0)
    0x26, 0xFF, //
    0x00,       //     Logical Maximum (255)
    0x35, 0x00, //     Physical Minimum (0)
    0x46, 0xFF, //
    0x00,       //     Physical Maximum (1)

    0x05, 0x0A,                 //     Usage Page (Ordinal)
    0x09, 0x01,                 //     Usage (0x01)
    0xA1, 0x02,                 //     Collection (Logical)
    0x05, 0x08,                 //       Usage Page (LEDs)
    0x09, 0x4B,                 //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_UP_LEFT, //       String Index
    0x75, 0x08,                 //       Report Size (8)
    0x95, 0x01,                 //       Report Count (1)
    0x91, 0x02, //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,       //     End Collection

    0x05, 0x0A,            //     Usage Page (Ordinal)
    0x09, 0x02,            //     Usage (0x02)
    0xA1, 0x02,            //     Collection (Logical)
    0x05, 0x08,            //       Usage Page (LEDs)
    0x09, 0x4B,            //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_UP, //       String Index
    0x75, 0x08,            //       Report Size (8)
    0x95, 0x01,            //       Report Count (1)
    0x91, 0x02,            //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                  //     End Collection

    0x05, 0x0A,                  //     Usage Page (Ordinal)
    0x09, 0x03,                  //     Usage (0x03)
    0xA1, 0x02,                  //     Collection (Logical)
    0x05, 0x08,                  //       Usage Page (LEDs)
    0x09, 0x4B,                  //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_UP_RIGHT, //       String Index
    0x75, 0x08,                  //       Report Size (8)
    0x95, 0x01,                  //       Report Count (1)
    0x91, 0x02, //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,       //     End Collection

    0x05, 0x0A,              //     Usage Page (Ordinal)
    0x09, 0x04,              //     Usage (0x04)
    0xA1, 0x02,              //     Collection (Logical)
    0x05, 0x08,              //       Usage Page (LEDs)
    0x09, 0x4B,              //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_LEFT, //       String Index
    0x75, 0x08,              //       Report Size (8)
    0x95, 0x01,              //       Report Count (1)
    0x91, 0x02,              //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                    //     End Collection

    0x05, 0x0A,                //     Usage Page (Ordinal)
    0x09, 0x05,                //     Usage (0x05)
    0xA1, 0x02,                //     Collection (Logical)
    0x05, 0x08,                //       Usage Page (LEDs)
    0x09, 0x4B,                //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_CENTER, //       String Index
    0x75, 0x08,                //       Report Size (8)
    0x95, 0x01,                //       Report Count (1)
    0x91, 0x02, //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,       //     End Collection

    0x05, 0x0A,               //     Usage Page (Ordinal)
    0x09, 0x06,               //     Usage (0x06)
    0xA1, 0x02,               //     Collection (Logical)
    0x05, 0x08,               //       Usage Page (LEDs)
    0x09, 0x4B,               //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_RIGHT, //       String Index
    0x75, 0x08,               //       Report Size (8)
    0x95, 0x01,               //       Report Count (1)
    0x91, 0x02, //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,       //     End Collection

    0x05, 0x0A,                   //     Usage Page (Ordinal)
    0x09, 0x07,                   //     Usage (0x07)
    0xA1, 0x02,                   //     Collection (Logical)
    0x05, 0x08,                   //       Usage Page (LEDs)
    0x09, 0x4B,                   //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_DOWN_LEFT, //       String Index
    0x75, 0x08,                   //       Report Size (8)
    0x95, 0x01,                   //       Report Count (1)
    0x91, 0x02, //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,       //     End Collection

    0x05, 0x0A,              //     Usage Page (Ordinal)
    0x09, 0x08,              //     Usage (0x08)
    0xA1, 0x02,              //     Collection (Logical)
    0x05, 0x08,              //       Usage Page (LEDs)
    0x09, 0x4B,              //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_DOWN, //       String Index
    0x75, 0x08,              //       Report Size (8)
    0x95, 0x01,              //       Report Count (1)
    0x91, 0x02,              //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                    //     End Collection

    0x05, 0x0A,                    //     Usage Page (Ordinal)
    0x09, 0x09,                    //     Usage (0x09)
    0xA1, 0x02,                    //     Collection (Logical)
    0x05, 0x08,                    //       Usage Page (LEDs)
    0x09, 0x4B,                    //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_DOWN_RIGHT, //       String Index
    0x75, 0x08,                    //       Report Size (8)
    0x95, 0x01,                    //       Report Count (1)
    0x91, 0x02, //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,       //     End Collection

    0xC0, //     End Collection

    0xA1, 0x02, //   Collection (Logical)
    0x85, 0x03, //     Report ID (3)
    0x15, 0x00, //     Logical Minimum (0)
    0x26, 0xFF, //
    0x00,       //     Logical Maximum (255)
    0x35, 0x00, //     Physical Minimum (0)
    0x46, 0xFF, //
    0x00,       //     Physical Maximum (1)

    0x05, 0x0A,                    //     Usage Page (Ordinal)
    0x09, 0x01,                    //     Usage (0x01)
    0xA1, 0x02,                    //     Collection (Logical)
    0x05, 0x08,                    //       Usage Page (LEDs)
    0x09, 0x4B,                    //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_STATUS_RED, //       String Index
    0x75, 0x08,                    //       Report Size (8)
    0x95, 0x01,                    //       Report Count (1)
    0x91, 0x02, //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,       //     End Collection

    0x05, 0x0A,                      //     Usage Page (Ordinal)
    0x09, 0x02,                      //     Usage (0x02)
    0xA1, 0x02,                      //     Collection (Logical)
    0x05, 0x08,                      //       Usage Page (LEDs)
    0x09, 0x4B,                      //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_STATUS_GREEN, //       String Index
    0x75, 0x08,                      //       Report Size (8)
    0x95, 0x01,                      //       Report Count (1)
    0x91, 0x02, //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,       //     End Collection

    0x05, 0x0A,                     //     Usage Page (Ordinal)
    0x09, 0x03,                     //     Usage (0x03)
    0xA1, 0x02,                     //     Collection (Logical)
    0x05, 0x08,                     //       Usage Page (LEDs)
    0x09, 0x4B,                     //       Usage (Generic Indicator)
    0x79, USBD_STR_LED_STATUS_BLUE, //       String Index
    0x75, 0x08,                     //       Report Size (8)
    0x95, 0x01,                     //       Report Count (1)
    0x91, 0x02, //       Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,       //     End Collection

    0xC0, //   End Collection
    0xC0, // End Collection
};

#define USBD_SPICE2X_DESC_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_INOUT_DESC_LEN)
const uint8_t spice2x_desc_cfg[] = {
    TUD_CONFIG_DESCRIPTOR(1, USBD_ITF_MAX, USBD_STR_LANGUAGE, USBD_SPICE2X_DESC_LEN, 0, USBD_MAX_POWER_MAX),
    TUD_HID_INOUT_DESCRIPTOR(USBD_ITF_HID, 0, 0, sizeof(spice2x_desc_hid_report), 0x02, 0x81, CFG_TUD_HID_EP_BUFSIZE,
                             1),
};

static hid_spice2x_report_t last_report = {};

bool send_hid_spice2x_report(usb_report_t report) {
    bool result = false;
    if (tud_hid_ready()) {
        result = tud_hid_report(0, report.data, report.size);
    }

    memcpy(&last_report, report.data, tu_min16(report.size, sizeof(hid_spice2x_report_t)));

    return result;
}

uint16_t hid_spice2x_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer,
                                   uint16_t reqlen) {
    (void)itf;
    (void)report_id;
    (void)reqlen;

    if (report_type == HID_REPORT_TYPE_INPUT) {
        memcpy(buffer, &last_report, sizeof(hid_spice2x_report_t));
        return sizeof(hid_spice2x_report_t);
    }
    return 0;
}

typedef struct __attribute((packed, aligned(1))) {
    uint8_t up_left;
    uint8_t up;
    uint8_t up_right;
    uint8_t left;
    uint8_t center;
    uint8_t right;
    uint8_t down_left;
    uint8_t down;
    uint8_t down_right;
} hid_spice2x_panel_led_ouput_report_t;

typedef struct __attribute((packed, aligned(1))) {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} hid_spice2x_status_led_ouput_report_t;

void hid_spice2x_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer,
                               uint16_t bufsize) {
    (void)itf;

    if (report_type == HID_REPORT_TYPE_OUTPUT) {
        if (report_id == 0 && bufsize > 0) {
            report_id = buffer[0];
            buffer = &buffer[1];
            bufsize--;
        }

        switch (report_id) {
        case 0x02: // HID Panel Lights
            if (bufsize == sizeof(hid_spice2x_panel_led_ouput_report_t)) {
                hid_spice2x_panel_led_ouput_report_t *report = (hid_spice2x_panel_led_ouput_report_t *)buffer;

                const usb_panel_led_t panel_led = {
                    .up_left = report->up_left,
                    .up = report->up,
                    .up_right = report->up_right,
                    .left = report->left,
                    .center = report->center,
                    .right = report->right,
                    .down_left = report->down_left,
                    .down = report->down,
                    .down_right = report->down_right,
                };

                usbd_driver_get_panel_led_cb()(panel_led);
            }
            break;
        case 0x03: // HID Status Led
            if (bufsize == sizeof(hid_spice2x_status_led_ouput_report_t)) {
                hid_spice2x_status_led_ouput_report_t *report = (hid_spice2x_status_led_ouput_report_t *)buffer;

                usb_player_led_t player_led = {
                    .type = USB_PLAYER_LED_COLOR, .red = report->red, .green = report->green, .blue = report->blue};
                usbd_driver_get_player_led_cb()(player_led);
            }
            break;
        default:
            break;
        }
    }
}

const usbd_driver_t *get_hid_spice2x_device_driver() {
    static const usbd_driver_t hid_spice2x_device_driver = {
        .name = "Spice2x",
        .app_driver = &hid_app_driver,
        .desc_device = &spice2x_desc_device,
        .desc_cfg = spice2x_desc_cfg,
        .desc_bos = NULL,
        .send_report = send_hid_spice2x_report,
    };

    return &hid_spice2x_device_driver;
}