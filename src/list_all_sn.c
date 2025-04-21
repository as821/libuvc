#include "libuvc/libuvc.h"
#include <stdio.h>
#include <unistd.h>


int main(int argc, char **argv) {
  uvc_context_t *ctx;
  uvc_device_t *dev;
  uvc_device_handle_t *devh;
  uvc_stream_ctrl_t ctrl;
  uvc_error_t res;

  /* Initialize a UVC service context. Libuvc will set up its own libusb
   * context. Replace NULL with a libusb_context pointer to run libuvc
   * from an existing libusb context. */
  res = uvc_init(&ctx, NULL);

  if (res < 0) {
    uvc_perror(res, "uvc_init");
    return res;
  }

  // TODO: print serial numbers of all UVC cameras found


  uvc_error_t ret = UVC_SUCCESS;
  uvc_device_t **list;
  uvc_device_t *test_dev;
  ret = uvc_get_device_list(ctx, &list);
  if (ret != UVC_SUCCESS) {
    return ret;
  }

  int dev_idx = 0;
  while ((test_dev = list[dev_idx++]) != NULL) {
    uvc_device_descriptor_t *desc;

    if (uvc_get_device_descriptor(test_dev, &desc) != UVC_SUCCESS)
      continue;

    printf("Device: %d, %d, %s\n", desc->idVendor, desc->idProduct, desc->serialNumber);
    uvc_free_device_descriptor(desc);
  }

  uvc_free_device_list(list, 1);




  /* Locates the first attached UVC device, stores in dev */
  res = uvc_find_device(
      ctx, &dev,
      0, 0, NULL); /* filter devices: vendor_id, product_id, "serial_num" */


  /* Close the UVC context. This closes and cleans up any existing device handles,
   * and it closes the libusb context if one was not provided. */
  uvc_exit(ctx);
  puts("UVC exited");

  return 0;
}

