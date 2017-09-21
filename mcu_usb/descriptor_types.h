#ifndef __DESCRIPTOR_TYPES__
#define __DESCRIPTOR_TYPES__

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

typedef struct USBDescriptorDevice
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



// Enum for possible Class, Subclass and Protocol values of device and interface descriptors.
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


#endif