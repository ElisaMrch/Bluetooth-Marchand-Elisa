#include <errno.h>
#include <zephyr/init.h>
#include <zephyr/sys/__assert.h>
#include <stdbool.h>
#include <zephyr/types.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include "led.h"

#define LOG_LEVEL CONFIG_BT_BAS_LOG_LEVEL
#include <zephyr/logging/log.h>

#define BT_UUID_IHM 0x1410
//#define BT_UUID_BUT_LEVEL 0x1412
#define BT_UUID_LED 0x1414

/////////////////////////LED//////////////////////////////////////

LOG_MODULE_REGISTER(Led);

static uint8_t led = 0U;

static ssize_t write_led(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, const void *buf,
			       uint16_t len, uint16_t offset, uint8_t flag)
{
	//allume et eteind
    int etatled = 0U;
    if (led == 10U) {
		etatled = 0;
	}
	else if (led == 0)
	{
		etatled = 10U;
    }
	return len;
}

BT_GATT_SERVICE_DEFINE(Led,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_16(BT_UUID_IHM)),
	BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(BT_UUID_LED),
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_WRITE |BT_GATT_PERM_READ , NULL, write_led,
			       &led),
	/*BT_GATT_CCC(butlvl_ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),*/
);

uint8_t but_get_led(void)
{
	return led;
}

int but_set_led(uint8_t nvLed)
{
	int Etatled = 0;

	if (nvLed == 10U) {
		Etatled = 0;
	}
	else if (nvLed == 0)
	{
		Etatled = 10U;
	}

	//etatled = bt_gatt_notify(NULL, &led.attrs[1], &led, sizeof(led));

	return Etatled;
}
