/*
 * Copyright 2012 Jared Boone
 *
 * This file is part of HackRF.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __USB_REQUEST_H__
#define __USB_REQUEST_H__

#include "usb_type.h"

typedef enum {
	USB_RESPONSE_NONE,
	USB_RESPONSE_IN,
	USB_RESPONSE_OUT,
	USB_RESPONSE_STALL,
} USBEndpointype_t;

extern const USBRequestHandlers usb0_request_handlers;
extern const USBRequestHandlers usb1_request_handlers;

void usb_setup_complete(
	USBEndpoint* const endpoint
);

void usb_control_in_complete(
	USBEndpoint* const endpoint
);

void usb_control_out_complete(
	USBEndpoint* const endpoint
);

#endif//__USB_REQUEST_H__
