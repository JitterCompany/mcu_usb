#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

//#include <lpc_usb_lib/USB.h>
#include "usb_type.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/** Public API **/

// may need adjustments for more than 6 endpoints or long string descriptors
#ifndef DESC_BUFF_SZ
#define DESC_BUFF_SZ (256)
#endif

// may need adjustments for more than 7 string descriptors
#ifndef DESC_MAX_STR_COUNT
#define DESC_MAX_STR_COUNT (8)
#endif

#define CONTROL_ENDPOINT_SIZE (64)
#define NO_DESCRIPTOR         (0)

#define MANUFACTURER_INDEX 1
#define PRODUCT_INDEX 2
#define SERIAL_INDEX 3

/* API overview:
 * The lines below show an overview of the available functions.
 * See the function declarations for documentation on how to use each function.
 *

dev_desc = descriptor_make_device(idVendor, idProduct, bcdDevice);

cfg_desc =descriptor_make_configuration(dev_desc,
				bConfigurationValue, bmAttributes, bMaxPower);

if_desc = descriptor_make_interface(cfg_desc,
				bInterfaceNumber, bAlternateSetting);

ep_desc = descriptor_make_endpoint(cfg_desc, if_desc,
				bEndpointAddress, bmAttributes, wMaxPacketSize, bInterval);

str_id = descriptor_string(string);
success = descriptor_ok();

 *
 */




/*
 * Check if the descriptor API has encountered any errors so far
 * At least check this function after specifying all descriptors to ensure
 * the result will be as expected.
 */
bool descriptor_ok();


/*
 * Create the device descriptor
 *
 * This function resets the descriptor buffer, clearing all previously configured
 * descriptors. So make sure you call this functions before configuring the other
 * descriptors. All descriptor fields are set to sensible defaults.
 * Further descriptor_xxx functions such as descriptor_string()
 * may be used to edit the descriptor and/or add additional descriptors.
 *
 * NB: after creating a device descriptor, you need to create at least one
 * configuration descriptor to get a working USB device
 *
 * @param idVendor		Vendor ID as in the USB standard
 * @param idProduct		Product ID as in the USB standard
 * @param bcdDevice		BCD-encoded device revision, see USB standard
 *
 * @return				pointer to a device descriptor. The descriptor itself
 * 						is stored in an internal buffer. The pointer may be used
 * 						to call further descriptor_xxx API functions.
 */
 USBDescriptorDevice *descriptor_make_device(uint16_t idVendor,
        uint16_t idProduct, uint16_t bcdDevice);


/*
 * Creates a configuration descriptor belonging to a previously created device
 * descriptor. The device descriptor is automatically updated where necessary
 *
 * @param device		device descriptor, see descriptor_make_device().
 * 						Null is accepted, but will not result in a valid
 * 						configuration descriptor.
 *
 * @param bConfigurationValue	Configuration descriptor id. 1 For the first
 * 								configuration, etc. see USB spec.
 *
 * @param bmAttributes			attributes as in the USB spec, or-ed together.
 * 								The required 1 << 7 bit is always set.
 *
 * @param bMaxPower				power consumption in 2mA per step. Use the
 * 								USB_CONFIG_POWER_MA() macro to get this value
 *
 * @return						pointer to the generated descriptor,
 * 								NULL on failure.
 * 								Use this pointer to add interfaces
 * 								to the configuration.
 *
 * 								NB: add all required interfaces before adding
 * 								additional configurations
 */
 USBDescriptorConfiguration *descriptor_make_configuration(
    USBDescriptorDevice *device,
    uint8_t bConfigurationValue, uint8_t bmAttributes, uint8_t bMaxPower);


/*
 * Creates an interface descriptor belonging to a previously created config
 * descriptor. The config descriptor is automatically updated where necessary.
 *
 * @param config		Existing configuration descriptor,
 * 						see descriptor_make_configuration().
 * 						Null is accepted, but will not result in a valid
 * 						interface descriptor.
 *
 * @param bInterfaceNumber		Interface number as per the USB spec
 *
 * @param bAlternateSetting		Alternate setting as per the USB spec.
 * 								Multiple interfaces with the same interface
 * 								number but different alternateSettigns may be
 * 								specified, allowing the host to switch
 * 								them on the fly.
 *
 * @return						pointer to the generated descriptor,
 * 								NULL on failure.
 * 								Use this pointer to add endpoints
 * 								to the configuration.
 *
 * 								NB: add all required endpoints before adding
 * 								additional interfaces or configurations
 */
 USBDescriptorInterface *descriptor_make_interface(
    USBDescriptorConfiguration *config,
    uint8_t bInterfaceNumber, uint8_t bAlternateSetting);


/*
 * Creates an endpoint descriptor belonging to a previously created interface
 * descriptor. The config and interface descriptors are automatically updated
 * where necessary.
 *
 * @param config		Existing configuration descriptor,
 * 						see descriptor_make_configuration().
 * 						Null is accepted, but will not result in a valid
 * 						endpoint descriptor.
 *
 * @param interface		Existing interface descriptor,
 * 						see descriptor_make_interface().
 * 						Null is accepted, but will not result in a valid
 * 						endpoint descriptor.
 *
 *
 * @return						pointer to the generated descriptor,
 * 								NULL on failure.
 * 								The pointer may be used to edit the endpoint
 * 								descriptor manually
 *
 * 								NB: creation order is important. First create
 * 								a device descriptor, then a configuration
 * 								descriptor, then an interface descriptor, then
 * 								create the endpoints for that interface.
 *
 * 								After adding all endpoints for the current
 * 								interface, additional interfaces or
 * 								configurations may be created.
 */
 bool descriptor_make_endpoint(
    USBDescriptorConfiguration *config,
    USBDescriptorInterface *interface,
    uint8_t bEndpointAddress, uint8_t bmAttributes,
    uint16_t wMaxPacketSize, uint8_t bInterval);


/*
 * Create a descriptor string index from a string
 *
 * This function accepts a string and assigns a USB string index to it.
 * The string index number can be used to assign to descriptor fields such as
 * device_descriptor->iProduct.
 * When the USB host requests the descriptor string index, a string descriptor
 * containing the supplied string is automatically generated.
 *
 * @param string		This string will be associated with the generated index.
 *
 * @result				The generated string index on success.
 *
 * 						Returns NO_DESCRIPTOR if the maximum is reached.
 * 						It is safe to pass this value to a descriptor.
 * 						NO_DESCRIPTOR is a valid index number, indicating to the
 * 						host that no string descriptor is available
 */
uint8_t descriptor_string(const char *const string);



/** Internal API **/

/*
 * Calculate the length of a string descriptor containing the specified string
 */
size_t descriptor_string_size(const char *const string);


/*
 * convert a c-style string to a string descriptor.
 *
 * @param result_desc	a pointer to the resulting string descriptor is written
 * 						to this pointer if the returned length is nonzero
 * @param string		c-style string to convert to string descriptor.
 * 						Must be NULL-terminated!
 *
 * @return				descriptor size if succesfull,
 * 						0 if there is not enough space to store the string
 * 						descriptor or the string itself is 0
 */
size_t descriptor_from_string(const USBDescriptorString **result_desc,
                              const char *const string);


/*
 * allocate a chunk of the storage buffer for a new
 * descriptor.
 *
 * @param num_bytes		amount of storage to claim
 * @param commit		set to true. If set to false, next_addr is not updated
 * 						and the next alloc will write over this value!
 * 						Only set to false to alloc temporary data!
 *
 * @return				pointer to the claimed chunk if enough storage
 * 						was available, NULL if not
 */
uint8_t *descriptor_storage_alloc(uint16_t requested_num_bytes, bool commit);


/*
 * Find a descriptor of specified type in the descriptor buffer.
 * A pointer to the first match after skipping skip_count matches is returned,
 * or NULL if no match could be found.
 *
 * @param bDescriptorType	type of the descriptor that is requested.
 *
 * @param skip_count		Set to 0 to return the first result.
 * 							This is the amount of valid results that are
 * 							skipped before returning.
 *
 * @return 					Pointer to the place in the buffer where the
 * 							descriptor was found, or NULL of not found.
 * 							Cast the result to the right type to use it.
 */
//void *descriptor_find(uint8_t bDescriptorType, uint8_t skip_count);

#endif

