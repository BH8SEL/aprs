#include "kiss.h"

#include "cfg/cfg_kiss.h"

#include <algo/rand.h>

#define LOG_LEVEL  KISS_LOG_LEVEL
#define LOG_FORMAT KISS_LOG_FORMAT
#include <cfg/log.h>

Serial *kiss_ser;
AX25Ctx *kiss_ax25;
Afsk *kiss_afsk;

ticks_t kiss_queue_ts;
uint8_t kiss_queue_state;
size_t kiss_queue_len = 0;
struct Kiss_msg kiss_queue[KISS_QUEUE];

#define KISS_PORTS 4
kiss_in_callback_t kiss_in_callbacks[KISS_PORTS];

uint8_t kiss_txdelay;
uint8_t kiss_txtail;
uint8_t kiss_persistence;
uint8_t kiss_slot_time;
uint8_t kiss_duplex;

static void kiss_cmd_process(struct Kiss_msg *k);

void kiss_init(Serial *ser, AX25Ctx *ax25, Afsk *afsk)
{
	kiss_txdelay = 50;
	kiss_persistence = 63;
	kiss_txtail = 5;
	kiss_slot_time = 10;
	kiss_duplex = KISS_DUPLEX_HALF;

	kiss_ser = ser;
	kiss_ax25 = ax25;
	kiss_afsk = afsk;
}

void kiss_set_in_callback(uint8_t port, kiss_in_callback_t fnc)
{
	if ((port == 0) || ((port - 1) > KISS_PORTS))
		return;

	kiss_in_callbacks[port - 1] = fnc;
}

void kiss_serial_poll()
{
	static struct Kiss_msg k = {.pos = 0};
	static bool escaped = false;
	int c;

	c = ser_getchar_nowait(kiss_ser);

	if (c == EOF) {
		return;
	}
	
	// sanity checks
	// no serial input in last 2 secs?
	if ((k.pos != 0) && (timer_clock() - k.last_tick  >  ms_to_ticks(2000L))) {
		LOG_INFO("Serial - Timeout\n");
		k.pos = 0;
	}
	
	// about to overflow buffer? reset
	if (k.pos >= (CONFIG_AX25_FRAME_BUF_LEN - 2)) {
		LOG_INFO("Serial - Packet too long %d >= %d\n", k.pos, CONFIG_AX25_FRAME_BUF_LEN - 2);
		k.pos = 0;
	}

	if (c == KISS_FEND) {
		if ((!escaped) && (k.pos > 0)) {
			kiss_cmd_process(&k);
		}

		k.pos = 0;
		escaped = false;
		return;
	} else if (c == KISS_FESC) {
		escaped = true;
		return;
	} else if (c == KISS_TFESC) {
		if (escaped) {
			escaped = false;
			c = KISS_FESC;
		}
	} else if (c == KISS_TFEND) {
		if (escaped) {
			escaped = false;
			c = KISS_FEND;
		}
	} else if (escaped) {
		escaped = false;
	}
	
	k.buf[k.pos] = c & 0xff;
	k.pos++;
	k.last_tick = timer_clock();
}

static void kiss_cmd_process(struct Kiss_msg *k)
{
	uint8_t cmd;
	uint8_t port;

	cmd = k->buf[0] & 0x0f;
	port = k->buf[0] >> 4;

	if (port != 0) {
		kiss_in_callback_t fnc;

		if (port > 5)
			return;

		if (cmd != KISS_CMD_DATA)
			return;

		fnc = kiss_in_callbacks[port - 1];
		if (fnc) {
			fnc(k->buf + 1, k->pos - 1);
		}
		return;
	}

	if (cmd == KISS_CMD_DATA) {
		LOG_INFO("Kiss - queueing message\n");
		kiss_queue_message(k->buf + 1, k->pos - 1);
		//ax25_sendRaw(kiss_ax25, k->buf+1, k->pos-1);
		return;
	}

	if (k->pos < 2) {
		LOG_INFO("Kiss - discarting packet - too short\n");
		return;
	}
	
	if (cmd == KISS_CMD_TXDELAY) {
		LOG_INFO("Kiss - setting txdelay %d\n", k->buf[1]);
		kiss_txdelay = k->buf[1];
	} else if (cmd == KISS_CMD_P) {
		LOG_INFO("Kiss - setting persistence %d\n", k->buf[1]);
		kiss_persistence = k->buf[1];
	} else if (cmd == KISS_CMD_SlotTime) {
		LOG_INFO("Kiss - setting slot_time %d\n", k->buf[1]);
		kiss_slot_time = k->buf[1];
	} else if (cmd == KISS_CMD_TXtail) {
		LOG_INFO("Kiss - setting txtail %d\n", k->buf[1]);
		kiss_txtail = k->buf[1];
	} else if (cmd == KISS_CMD_FullDuplex) {
		LOG_INFO("Kiss - setting duplex %d\n", k->buf[1]);
		kiss_duplex = k->buf[1];
	}
}

void kiss_queue_message(uint8_t *buf, size_t len)
{
	if (kiss_queue_len == KISS_QUEUE)
		return;

	memcpy(kiss_queue[kiss_queue_len].buf, buf, len);
	kiss_queue[kiss_queue_len].pos = len;
	kiss_queue_len ++;
}

void kiss_queue_process()
{
	uint8_t random;

	if (kiss_queue_len == 0) {
		return;
	}
/*
	if (kiss_afsk->cd) {
		return;
	}
*/
	if (kiss_ax25->dcd) {
		return;
	}

	if (kiss_queue_state == KISS_QUEUE_DELAYED) {
		if (timer_clock() - kiss_queue_ts <= ms_to_ticks(kiss_slot_time * 10)) {
			return;
		}
		LOG_INFO("Queue released\n");
	}

	random = (uint32_t)rand() & 0xff;
	LOG_INFO("Queue random is %d\n", random);
	if (random > kiss_persistence) {
		LOG_INFO("Queue delayed for %dms\n", kiss_slot_time * 10);

		kiss_queue_state = KISS_QUEUE_DELAYED;
		kiss_queue_ts = timer_clock();

		return;
	}

	LOG_INFO("Queue sending packets: %d\n", kiss_queue_len);
	for (size_t i = 0; i < kiss_queue_len; i++) {
		ax25_sendRaw(kiss_ax25, kiss_queue[i].buf, kiss_queue[i].pos);
	}

	kiss_queue_len = 0;
	kiss_queue_state = KISS_QUEUE_IDLE;
}

void kiss_send_host(uint8_t ch, uint8_t *buf, size_t len)
{
	size_t i;

	kfile_putc(KISS_FEND, &kiss_ser->fd);
	kfile_putc((ch << 4) & 0xf0, &kiss_ser->fd);

	for (i = 0; i < len; i++) {

		uint8_t c = buf[i];

		if (c == KISS_FEND) {
			kfile_putc(KISS_FESC, &kiss_ser->fd);
			kfile_putc(KISS_TFEND, &kiss_ser->fd);
			continue;
		}

		kfile_putc(c, &kiss_ser->fd);

		if (c == KISS_FESC) {
			kfile_putc(KISS_TFESC, &kiss_ser->fd);
		}
	}

	kfile_putc(KISS_FEND, &kiss_ser->fd);
}

