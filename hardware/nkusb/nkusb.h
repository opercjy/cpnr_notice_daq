// hardware/nkusb/nkusb.h
#ifndef NKUSB_H
#define NKUSB_H

#include <libusb.h>
#include "nkusbconst.h" // NK_SID_ANY 정의를 위해 포함

#ifdef __cplusplus
extern "C" {
#endif

extern int nkusb_init(libusb_context ** ctx);
extern int nkusb_open_device(uint16_t vendor_id, uint16_t product_id, int sid, libusb_context * ctx);
extern int nkusb_claim_interface(uint16_t vendor_id, uint16_t product_id, int sid, int interface);
extern int nkusb_release_interface(uint16_t vendor_id, uint16_t product_id, int sid, int interface);
extern void nkusb_close_device(uint16_t vendor_id, uint16_t product_id, int sid);
extern void nkusb_exit(libusb_context * ctx);
extern void nkusb_print_open_devices(void);
extern int nkusb_is_device_open(uint16_t vendor_id, uint16_t product_id, int sid);
extern libusb_device_handle * nkusb_get_device_handle(uint16_t vendor_id, uint16_t product_id, int sid);

#ifdef __cplusplus
}
#endif

#endif // NKUSB_H