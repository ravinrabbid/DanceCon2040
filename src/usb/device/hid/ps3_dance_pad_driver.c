#include "usb/device/hid/ps3_dance_pad_driver.h"

#include "usb/device/hid/common.h"

#include "tusb.h"

const tusb_desc_device_t ps3_dance_pad_desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_UNSPECIFIED,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x1CCF,
    .idProduct = 0x1010,
    .bcdDevice = 0x0100,
    .iManufacturer = USBD_STR_MANUFACTURER,
    .iProduct = USBD_STR_PRODUCT,
    .iSerialNumber = 0,
    .bNumConfigurations = 1,
};

enum {
    USBD_ITF_HID,
    USBD_ITF_MAX,
};

const uint8_t ps3_dance_pad_desc_hid_report[] = {
    0x05, 0x01,       // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05,       // Usage (Game Pad)
    0xA1, 0x01,       // Collection (Application)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x35, 0x00,       //   Physical Minimum (0)
    0x45, 0x01,       //   Physical Maximum (1)
    0x75, 0x01,       //   Report Size (1)
    0x95, 0x0D,       //   Report Count (13)
    0x05, 0x09,       //   Usage Page (Button)
    0x19, 0x01,       //   Usage Minimum (0x01)
    0x29, 0x0D,       //   Usage Maximum (0x0D)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x03,       //   Report Count (3)
    0x81, 0x01,       //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,       //   Usage Page (Generic Desktop Ctrls)
    0x25, 0x07,       //   Logical Maximum (7)
    0x46, 0x3B, 0x01, //   Physical Maximum (315)
    0x75, 0x04,       //   Report Size (4)
    0x95, 0x01,       //   Report Count (1)
    0x65, 0x14,       //   Unit (System: English Rotation, Length: Centimeter)
    0x09, 0x39,       //   Usage (Hat switch)
    0x81, 0x42,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
    0x65, 0x00,       //   Unit (None)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x01,       //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x46, 0xFF, 0x00, //   Physical Maximum (255)
    0x09, 0x30,       //   Usage (X)
    0x09, 0x31,       //   Usage (Y)
    0x09, 0x32,       //   Usage (Z)
    0x09, 0x35,       //   Usage (Rz)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x04,       //   Report Count (4)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x06, 0x00, 0xFF, //   Usage Page (Vendor Defined 0xFF00)
    0x09, 0x20,       //   Usage (0x20)
    0x09, 0x21,       //   Usage (0x21)
    0x09, 0x22,       //   Usage (0x22)
    0x09, 0x23,       //   Usage (0x23)
    0x09, 0x24,       //   Usage (0x24)
    0x09, 0x25,       //   Usage (0x25)
    0x09, 0x26,       //   Usage (0x26)
    0x09, 0x27,       //   Usage (0x27)
    0x09, 0x28,       //   Usage (0x28)
    0x09, 0x29,       //   Usage (0x29)
    0x09, 0x2A,       //   Usage (0x2A)
    0x09, 0x2B,       //   Usage (0x2B)
    0x95, 0x0C,       //   Report Count (12)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x0A, 0x21, 0x26, //   Usage (0x2621)
    0x95, 0x08,       //   Report Count (8)
    0xB1, 0x02,       //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x0A, 0x21, 0x26, //   Usage (0x2621)
    0x91, 0x02,       //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x26, 0xFF, 0x03, //   Logical Maximum (1023)
    0x46, 0xFF, 0x03, //   Physical Maximum (1023)
    0x09, 0x2C,       //   Usage (0x2C)
    0x09, 0x2D,       //   Usage (0x2D)
    0x09, 0x2E,       //   Usage (0x2E)
    0x09, 0x2F,       //   Usage (0x2F)
    0x75, 0x10,       //   Report Size (16)
    0x95, 0x04,       //   Report Count (4)
    0x81, 0x02,       //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,             // End Collection
};

#define USBD_PS3_DANCE_PAD_DESC_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_INOUT_DESC_LEN)
const uint8_t ps3_dance_pad_desc_cfg[] = {
    TUD_CONFIG_DESCRIPTOR(1, USBD_ITF_MAX, USBD_STR_LANGUAGE, USBD_PS3_DANCE_PAD_DESC_LEN, 0, USBD_MAX_POWER_MAX),
    TUD_HID_INOUT_DESCRIPTOR(USBD_ITF_HID, 0, 0, sizeof(ps3_dance_pad_desc_hid_report), 0x02, 0x81,
                             CFG_TUD_HID_EP_BUFSIZE, 1),
};

static hid_ps3_dance_pad_report_t last_report = {};

bool send_hid_ps3_dance_pad_report(usb_report_t report) {
    bool result = false;
    if (tud_hid_ready()) {
        result = tud_hid_report(0, report.data, report.size);
    }

    memcpy(&last_report, report.data, tu_min16(report.size, sizeof(hid_ps3_dance_pad_report_t)));

    return result;
}

static const uint8_t ps3_dance_pad_feature_report[] = {0x21, 0x26, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00};

uint16_t hid_ps3_dance_pad_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer,
                                         uint16_t reqlen) {
    (void)itf;
    (void)reqlen;

    switch (report_type) {
    case HID_REPORT_TYPE_INPUT: {
        memcpy(buffer, &last_report, sizeof(hid_ps3_dance_pad_report_t));
        return sizeof(hid_ps3_dance_pad_report_t);
    }
    case HID_REPORT_TYPE_FEATURE:
        if (report_id == 0x00) {
            memcpy(buffer, ps3_dance_pad_feature_report, sizeof(ps3_dance_pad_feature_report));
            return sizeof(ps3_dance_pad_feature_report);
        }
        break;
    default:
        break;
    }
    return 0;
}

typedef struct __attribute((packed, aligned(1))) {
    uint8_t unknown1[2];
    uint8_t player_led;
    uint8_t unknown2[5];
} hid_ps3_dance_pad_ouput_report_t;

void hid_ps3_dance_pad_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type,
                                     uint8_t const *buffer, uint16_t bufsize) {
    (void)itf;

    if (report_type != HID_REPORT_TYPE_OUTPUT || report_id != 0 || bufsize < sizeof(hid_ps3_dance_pad_ouput_report_t)) {
        return;
    }

    hid_ps3_dance_pad_ouput_report_t *report = (hid_ps3_dance_pad_ouput_report_t *)buffer;

    usb_player_led_t player_led = {.type = USB_PLAYER_LED_ID, .id = (report->player_led & 0x0F)};
    usbd_driver_get_player_led_cb()(player_led);
}

const usbd_driver_t *get_hid_ps3_dance_pad_device_driver() {
    static const usbd_driver_t hid_ps3_dance_pad_device_driver = {
        .name = "PS3 Dance Pad",
        .app_driver = &hid_app_driver,
        .desc_device = &ps3_dance_pad_desc_device,
        .desc_cfg = ps3_dance_pad_desc_cfg,
        .desc_bos = NULL,
        .send_report = send_hid_ps3_dance_pad_report,
    };
    return &hid_ps3_dance_pad_device_driver;
}