#ifndef __USB_TYPE_H__
#define __USB_TYPE_H__

#include <stdint.h>
#include <stdbool.h>

// TODO: Move this to some common compiler-tricks location.
#define ATTR_PACKED __attribute__((packed))
#define ATTR_ALIGNED(x)	__attribute__ ((aligned(x)))
#define ATTR_SECTION(x) __attribute__ ((section(x)))

#define VERSION_BCD(x) CPU_TO_LE16((((VERSION_TENS(x) << 4) | VERSION_ONES(x)) << 8) | \
								   ((VERSION_TENTHS(x) << 4) | VERSION_HUNDREDTHS(x)))

typedef struct USBEndpoint USBEndpoint;
	

typedef struct ATTR_PACKED {
	uint8_t request_type;
	uint8_t request;
	union {
		struct {
			uint8_t value_l;
			uint8_t value_h;
		};
		uint16_t value;
	};
	union {
		struct {
			uint8_t index_l;
			uint8_t index_h;
		};
		uint16_t index;
	};
	union {
		struct {
			uint8_t length_l;
			uint8_t length_h;
		};
		uint16_t length;
	};
} USBSetup;

typedef enum {
	USB_STANDARD_REQUEST_GET_STATUS = 0,
	USB_STANDARD_REQUEST_CLEAR_FEATURE = 1,
	USB_STANDARD_REQUEST_SET_FEATURE = 3,
	USB_STANDARD_REQUEST_SET_ADDRESS = 5,
	USB_STANDARD_REQUEST_GET_DESCRIPTOR = 6,
	USB_STANDARD_REQUEST_SET_DESCRIPTOR = 7,
	USB_STANDARD_REQUEST_GET_CONFIGURATION = 8,
	USB_STANDARD_REQUEST_SET_CONFIGURATION = 9,
	USB_STANDARD_REQUEST_GET_INTERFACE = 10,
	USB_STANDARD_REQUEST_SET_INTERFACE = 11,
	USB_STANDARD_REQUEST_SYNCH_FRAME = 12,
} USBStandardRequest;

typedef enum {
	USB_SETUP_REQUEST_TYPE_shift = 5,
	USB_SETUP_REQUEST_TYPE_mask = 3 << USB_SETUP_REQUEST_TYPE_shift,
	
	USB_SETUP_REQUEST_TYPE_STANDARD = 0 << USB_SETUP_REQUEST_TYPE_shift,
	USB_SETUP_REQUEST_TYPE_CLASS = 1 << USB_SETUP_REQUEST_TYPE_shift,
	USB_SETUP_REQUEST_TYPE_VENDOR = 2 << USB_SETUP_REQUEST_TYPE_shift,
	USB_SETUP_REQUEST_TYPE_RESERVED = 3 << USB_SETUP_REQUEST_TYPE_shift,
	
	USB_SETUP_REQUEST_TYPE_DATA_TRANSFER_DIRECTION_shift = 7,
	USB_SETUP_REQUEST_TYPE_DATA_TRANSFER_DIRECTION_mask = 1 << USB_SETUP_REQUEST_TYPE_DATA_TRANSFER_DIRECTION_shift,
	USB_SETUP_REQUEST_TYPE_DATA_TRANSFER_DIRECTION_HOST_TO_DEVICE = 0 << USB_SETUP_REQUEST_TYPE_DATA_TRANSFER_DIRECTION_shift,
	USB_SETUP_REQUEST_TYPE_DATA_TRANSFER_DIRECTION_DEVICE_TO_HOST = 1 << USB_SETUP_REQUEST_TYPE_DATA_TRANSFER_DIRECTION_shift,
} USBSetupRequestType;

typedef enum {
	USB_TRANSFER_DIRECTION_OUT = 0,
	USB_TRANSFER_DIRECTION_IN = 1,
} USBTransferDirection;
	
typedef enum {
	USB_DESCRIPTOR_TYPE_DEVICE = 1,
	USB_DESCRIPTOR_TYPE_CONFIGURATION = 2,
	USB_DESCRIPTOR_TYPE_STRING = 3,
	USB_DESCRIPTOR_TYPE_INTERFACE = 4,
	USB_DESCRIPTOR_TYPE_ENDPOINT = 5,
	USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER = 6,
	USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION = 7,
	USB_DESCRIPTOR_TYPE_INTERFACE_POWER = 8,
} USBDescriptorType;

typedef enum {
	USB_TRANSFER_TYPE_CONTROL = 0,
	USB_TRANSFER_TYPE_ISOCHRONOUS = 1,
	USB_TRANSFER_TYPE_BULK = 2,
	USB_TRANSFER_TYPE_INTERRUPT = 3,
} USBTransferType;

typedef enum {
	USB_SPEED_LOW = 0,
	USB_SPEED_FULL = 1,
	USB_SPEED_HIGH = 2,
	USB_SPEED_SUPER = 3,
} USBSpeed;



/** Enum for possible Class, Subclass and Protocol values of device and interface descriptors. */
enum USBDescriptor_ClassSubclassProtocol
{
	USB_CSCP_NoDeviceClass          = 0x00, /**< Descriptor Class value indicating that the device does not belong
											 *   to a particular class at the device level.
											 */
	USB_CSCP_NoDeviceSubclass       = 0x00, /**< Descriptor Subclass value indicating that the device does not belong
											 *   to a particular subclass at the device level.
											 */
	USB_CSCP_NoDeviceProtocol       = 0x00, /**< Descriptor Protocol value indicating that the device does not belong
											 *   to a particular protocol at the device level.
											 */
	USB_CSCP_VendorSpecificClass    = 0xFF, /**< Descriptor Class value indicating that the device/interface belongs
											 *   to a vendor specific class.
											 */
	USB_CSCP_VendorSpecificSubclass = 0xFF, /**< Descriptor Subclass value indicating that the device/interface belongs
											 *   to a vendor specific subclass.
											 */
	USB_CSCP_VendorSpecificProtocol = 0xFF, /**< Descriptor Protocol value indicating that the device/interface belongs
											 *   to a vendor specific protocol.
											 */
	USB_CSCP_IADDeviceClass         = 0xEF, /**< Descriptor Class value indicating that the device belongs to the
											 *   Interface Association Descriptor class.
											 */
	USB_CSCP_IADDeviceSubclass      = 0x02, /**< Descriptor Subclass value indicating that the device belongs to the
											 *   Interface Association Descriptor subclass.
											 */
	USB_CSCP_IADDeviceProtocol      = 0x01, /**< Descriptor Protocol value indicating that the device belongs to the
											 *   Interface Association Descriptor protocol.
											 */
};

typedef struct
{
	uint8_t  bLength; 			// Size of the descriptor, in bytes. 
	uint8_t  bDescriptorType; 	// Type of the descriptor, 
	
	uint16_t bcdUSB;  			// BCD of the supported USB specification. 
	uint8_t  bDeviceClass; 		// USB device class. 
	uint8_t  bDeviceSubClass; 	// USB device subclass. 
	uint8_t  bDeviceProtocol; 	// USB device protocol. 
	uint8_t  bMaxPacketSize0; 	// Size of the control (address 0) endpoint's bank in bytes. 
	uint16_t idVendor; 			// Vendor ID for the USB product. 
	uint16_t idProduct; 		// Unique product ID for the USB product. 
	uint16_t bcdDevice; 		// Product release (version) number. 
	uint8_t  iManufacturer; 	// String index for the manufacturer's name. The
	// host will request this string via a separate
	// control request for the string descriptor.
	//   @note If no string supplied, use @ref NO_DESCRIPTOR.
	
	uint8_t  iProduct; 			// String index for the product name/details.
	
	// @see ManufacturerStrIndex structure entry.
	
	uint8_t iSerialNumber; 		// String index for the product's globally unique hexadecimal
	// serial number, in uppercase Unicode ASCII.
	
	uint8_t  bNumConfigurations;// Total number of configurations supported by the device.
	
} __attribute__ ((packed)) USBDescriptorDevice;

typedef struct {
	uint8_t  bLength; 			// Size of the descriptor, in bytes. 
	uint8_t  bDescriptorType; 	// Type of the descriptor, 
	uint16_t bString[];
} __attribute__ ((packed)) USBDescriptorString;

typedef struct
{
	uint8_t  bLength; /**< Size of the descriptor, in bytes. */
	uint8_t  bDescriptorType; /**< Type of the descriptor, either a value in @ref USB_DescriptorTypes_t or a value
								  *   given by the specific class.
								  */
	uint16_t wTotalLength; /**< Size of the configuration descriptor header,
							   *   and all sub descriptors inside the configuration.
							   */
	uint8_t  bNumInterfaces; /**< Total number of interfaces in the configuration. */
	uint8_t  bConfigurationValue; /**< Configuration index of the current configuration. */
	uint8_t  iConfiguration; /**< Index of a string descriptor describing the configuration. */
	uint8_t  bmAttributes; /**< Configuration attributes, comprised of a mask of zero or
							*   more USB_CONFIG_ATTR_* masks.
							*/
	uint8_t  bMaxPower; /**< Maximum power consumption of the device while in the
						 *   current configuration, calculated by the @ref USB_CONFIG_POWER_MA()
						 *   macro.
						 */
} __attribute__ ((packed)) USBDescriptorConfiguration;

typedef struct
{
	uint8_t bLength; /**< Size of the descriptor, in bytes. */
	uint8_t bDescriptorType; /**< Type of the descriptor, either a value in @ref USB_DescriptorTypes_t or a value
							  *   given by the specific class.
							  */
	uint8_t bInterfaceNumber; /**< Index of the interface in the current configuration. */
	uint8_t bAlternateSetting; /**< Alternate setting for the interface number. The same
								*   interface number can have multiple alternate settings
								*   with different endpoint configurations, which can be
								*   selected by the host.
								*/
	uint8_t bNumEndpoints; /**< Total number of endpoints in the interface. */
	uint8_t bInterfaceClass; /**< Interface class ID. */
	uint8_t bInterfaceSubClass; /**< Interface subclass ID. */
	uint8_t bInterfaceProtocol; /**< Interface protocol ID. */
	uint8_t iInterface; /**< Index of the string descriptor describing the
						 *   interface.
						 */
} __attribute__ ((packed)) USBDescriptorInterface;

typedef struct
{
	uint8_t  bLength; /**< Size of the descriptor, in bytes. */
	uint8_t  bDescriptorType; /**< Type of the descriptor, either a value in @ref USB_DescriptorTypes_t or a
							   *   value given by the specific class.
							   */
	uint8_t  bEndpointAddress; /**< Logical address of the endpoint within the device for the current
								*   configuration, including direction mask.
								*/
	uint8_t  bmAttributes; /**< Endpoint attributes, comprised of a mask of the endpoint type (EP_TYPE_*)
							*   and attributes (ENDPOINT_ATTR_*) masks.
							*/
	uint16_t wMaxPacketSize; /**< Size of the endpoint bank, in bytes. This indicates the maximum packet size
							  *   that the endpoint can receive at a time.
							  */
	uint8_t  bInterval; /**< Polling interval in milliseconds for the endpoint if it is an INTERRUPT or
						 *   ISOCHRONOUS type.
						 */
} __attribute__ ((packed)) USBDescriptorEndpoint;

typedef struct {
	USBDescriptorConfiguration* descriptor;
	const uint32_t number;
	const USBSpeed speed;
} USBConfiguration;

typedef enum {
	USB_TRANSFER_STAGE_SETUP,
	USB_TRANSFER_STAGE_DATA,
	USB_TRANSFER_STAGE_STATUS,
} USBTransferStage;

typedef enum {
	USB_REQUEST_STATUS_OK = 0,
	USB_REQUEST_STATUS_STALL = 1,
} USBRequestStatus;
	
typedef USBRequestStatus (*usb_request_handler_fn)(
	USBEndpoint* const endpoint,
	const USBTransferStage stage
);

typedef struct {
	usb_request_handler_fn standard;
	usb_request_handler_fn class;
	usb_request_handler_fn vendor;
	usb_request_handler_fn reserved;
} USBRequestHandlers;

typedef struct {
	USBDescriptorDevice *descriptor;
	const USBDescriptorString **descriptor_strings;
	const uint8_t* const qualifier_descriptor;
	
	USBConfiguration* (*configurations)[];
	const USBConfiguration* configuration; 	// Pointer to current configuration
	uint8_t controller; 				// USB0 or USB1 peripheral;
	const USBRequestHandlers *request_handlers;
} USBDevice;

struct USBEndpoint {
	USBSetup setup;
	uint8_t buffer[8];	// Buffer for use during IN stage.
	uint_fast8_t address;
	USBDevice *device;
	USBEndpoint* in;
	USBEndpoint* out;
	void (*setup_complete)(USBEndpoint* const endpoint);
	void (*transfer_complete)(USBEndpoint* const endpoint);
};

typedef void (*Endpoint_cb)(USBEndpoint* const endpoint);


#endif//__USB_TYPE_H__
