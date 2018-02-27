// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <ddk/debug.h>
#include <ddk/device.h>
#include <ddk/protocol/platform-bus.h>
#include <ddk/protocol/platform-defs.h>

#include <soc/aml-s905/s905-gpio.h>

#include <limits.h>

#include "odroid.h"

// uncomment to disable LED blinky test
#define GPIO_TEST 1

static const pbus_mmio_t gpio_mmios[] = {
    {
        .base = 0xc8834400,
        .length = 0x1C00,
    },
    {
        .base = 0xc8100000,
        .length = PAGE_SIZE,
    },
};

static const pbus_irq_t gpio_irqs[] = {
    {
        // gpio_irq0
        .irq = 94,
        .mode = ZX_INTERRUPT_MODE_EDGE_HIGH,
    },
    {
        // gpio_irq1
        .irq = 95,
        .mode = ZX_INTERRUPT_MODE_EDGE_HIGH,
    },
    {
        // gpio_irq2
        .irq = 96,
        .mode = ZX_INTERRUPT_MODE_EDGE_HIGH,
    },
    {
        // gpio_irq3
        .irq = 97,
        .mode = ZX_INTERRUPT_MODE_EDGE_HIGH,
    },
    {
        // gpio_irq4
        .irq = 98,
        .mode = ZX_INTERRUPT_MODE_EDGE_HIGH,
    },
    {
        // gpio_irq5
        .irq = 99,
        .mode = ZX_INTERRUPT_MODE_EDGE_HIGH,
    },
    {
        // gpio_irq6
        .irq = 100,
        .mode = ZX_INTERRUPT_MODE_EDGE_HIGH,
    },
    {
        // gpio_irq7
        .irq = 101,
        .mode = ZX_INTERRUPT_MODE_EDGE_HIGH,
    },
    {
        // ao_gpio_irq0
        .irq = 232,
        .mode = ZX_INTERRUPT_MODE_EDGE_HIGH,
    },
    {
        // ao_gpio_irq1
        .irq = 233,
        .mode = ZX_INTERRUPT_MODE_EDGE_HIGH,
    },
};

static pbus_dev_t gpio_dev = {
    .name = "gpio",
    .vid = PDEV_VID_AMLOGIC,
    .pid = PDEV_PID_AMLOGIC_S905,
    .did = PDEV_DID_AMLOGIC_GPIO,
    .mmios = gpio_mmios,
    .mmio_count = countof(gpio_mmios),
    .irqs = gpio_irqs,
    .irq_count = countof(gpio_irqs),
};


zx_status_t odroid_gpio_init(odroid_t* odroid) {
    zx_status_t status = pbus_device_add(&odroid->pbus, &gpio_dev, PDEV_ADD_PBUS_DEVHOST);
    if (status != ZX_OK) {
        zxlogf(ERROR, "odroid_gpio_init: pbus_device_add failed: %d\n", status);
        return status;
    }

    status = pbus_wait_protocol(&odroid->pbus, ZX_PROTOCOL_GPIO);
    if (status != ZX_OK) {
        zxlogf(ERROR, "odroid_gpio_init: pbus_wait_protocol failed: %d\n", status);
        return status;
    }

    status = device_get_protocol(odroid->parent, ZX_PROTOCOL_GPIO, &odroid->gpio);
    if (status != ZX_OK) {
        zxlogf(ERROR, "odroid_gpio_init: device_get_protocol failed: %d\n", status);
        return status;
    }

#if GPIO_TEST
    const pbus_gpio_t gpio_test_gpios[] = {
        {
            // SYS_LED
            .gpio = S905_GPIOAO(13),
        },
    };

    const pbus_dev_t gpio_test_dev = {
        .name = "odroid-gpio-test",
        .vid = PDEV_VID_GENERIC,
        .pid = PDEV_PID_GENERIC,
        .did = PDEV_DID_GPIO_TEST,
        .gpios = gpio_test_gpios,
        .gpio_count = countof(gpio_test_gpios),
    };

    if ((status = pbus_device_add(&odroid->pbus, &gpio_test_dev, 0)) != ZX_OK) {
        zxlogf(ERROR, "odroid_gpio_init could not add gpio_test_dev: %d\n", status);
        return status;
    }
#endif

    return ZX_OK;
}
