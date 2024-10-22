#!/usr/bin/env python3

#
# Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
#
# SPDX-License-Identifier: BSD-3-Clause
#

# sudo pip3 install pyusb

import os
import time
import usb.core
import usb.util

# find our device
dev = usb.core.find(idVendor=0x2e8a, idProduct=0x0049)

# was it found?
if dev is None:
    raise ValueError('Device not found')

# get an endpoint instance
cfg = dev.get_active_configuration()
intf = cfg[(0, 0)]

outep = usb.util.find_descriptor(
    intf,
    # match the first OUT endpoint
    custom_match= \
        lambda e: \
            usb.util.endpoint_direction(e.bEndpointAddress) == \
            usb.util.ENDPOINT_OUT)

inep = usb.util.find_descriptor(
    intf,
    # match the first IN endpoint
    custom_match= \
        lambda e: \
            usb.util.endpoint_direction(e.bEndpointAddress) == \
            usb.util.ENDPOINT_IN)

assert inep is not None
assert outep is not None

random_bytes = os.urandom(64)
# for i in random_bytes:
#     print(i, end=' ')
# print()
start_time = time.time()
outep.write(random_bytes)
end_time = time.time()
from_device = inep.read(len(random_bytes))
# print(from_device)

execution_time_us = (end_time - start_time) * 1_000_000
mb_per_second = len(random_bytes) / (execution_time_us / 1_000_000) / 1024 / 1024 * 8
print(f"Device wrote {len(random_bytes)} bytes in {execution_time_us:.2f} us, {mb_per_second:.2f} Mb/s")
