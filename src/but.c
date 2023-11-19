/** @file
 *  @brief GATT Battery Service
 */

/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <zephyr/init.h>
#include <zephyr/sys/__assert.h>
#include <stdbool.h>
#include <zephyr/types.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include "but.h"

#define LOG_LEVEL CONFIG_BT_BAS_LOG_LEVEL
#include <zephyr/logging/log.h>

#define BT_UUID_IHM 0x1410
#define BT_UUID_BUT_LEVEL 0x1412
#define BT_UUID_LED 0x1414

static uint8_t buton_level = 0U;

LOG_MODULE_REGISTER(but);

static ssize_t read_butlvl(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset)
{
	uint8_t lvl8 = buton_level;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, &lvl8,
				 sizeof(lvl8));
}

static void butlvl_ccc_cfg_changed(const struct bt_gatt_attr *attr,
				       uint16_t value)
{
	ARG_UNUSED(attr);

	bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

	LOG_INF("BAS Notifications %s", notif_enabled ? "enabled" : "disabled");
}

BT_GATT_SERVICE_DEFINE(but,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_16(BT_UUID_IHM)),
	BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(BT_UUID_BUT_LEVEL),
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ, read_butlvl, NULL,
			       &buton_level),
	BT_GATT_CCC(butlvl_ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

uint8_t but_bas_get_buton_level(void)
{
	return buton_level;
}

int but_bas_set_buton_level(uint8_t level)
{
	int rc;

	if (level > 100U) {
		return -EINVAL;
	}

	buton_level = level;

	rc = bt_gatt_notify(NULL, &but.attrs[1], &level, sizeof(level));

	return rc == -ENOTCONN ? 0 : rc;
}


