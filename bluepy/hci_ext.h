#ifndef HCI_EXT_H_
#define HCI_EXT_H_

#include "hci.h"
#include "hci_lib.h"

#define EVT_LE_EXTENDED_ADVERTISING_REPORT	0x0d
typedef struct {
	uint8_t 	n_reports; // assume always 1
	uint16_t 	event_type;
	uint8_t		addr_type;
	bdaddr_t	addr;
	uint8_t		primary_phy;
	uint8_t 	secondary_phy;
	uint8_t 	advertising_sid;
	uint8_t 	tx_power;
	int8_t 		rssi; 
	uint16_t 	interval;
	uint8_t 	direct_addr_type;
	bdaddr_t 	direct_addr;
	uint8_t 	length;
	uint8_t 	data[];
} __attribute__ ((packed)) le_extended_advertising_info;
#define LE_EXTENDED_ADVERTISING_INFO_SIZE 9

typedef struct {
	uint8_t addr_type;
	uint8_t filt_policy;
	uint8_t phy;
	uint8_t type;
	uint16_t interval;
	uint16_t window;
} __attribute((packed)) le_set_extended_scan_parameters_cp;

static int hci_le_set_extended_scan_parameters(int dd, uint8_t addr_type, uint8_t filt_policy, uint8_t phy, uint8_t type, uint16_t interval, uint16_t window, int to)
{
	struct hci_request rq;
	le_set_extended_scan_parameters_cp ext_scan_cp;
	uint8_t status; 

	memset(&ext_scan_cp, 0x00, sizeof(ext_scan_cp));
	ext_scan_cp.addr_type = type;
	ext_scan_cp.filt_policy = filt_policy;
	ext_scan_cp.phy = phy;
	ext_scan_cp.type = type;
	ext_scan_cp.interval = interval;
	ext_scan_cp.window = window;

	memset(&rq, 0, sizeof(rq));
	rq.ogf = OGF_LE_CTL;
	rq.ocf = 0x0041;
	rq.cparam = &ext_scan_cp;
	rq.clen = sizeof(ext_scan_cp);
	rq.rparam = &status;
	rq.rlen = 1;

	if (hci_send_req(dd, &rq, to) < 0)
		return -1;

	if (status) {
		errno = EIO;
		return -status;
	}

	return 0;
}

typedef struct {
	uint8_t enabled;
	uint8_t filter_dup;
	uint16_t duration;
	uint16_t period;
} __attribute__((packed)) le_set_extended_scan_enable_cp;

static int hci_le_set_extended_scan_enable(int dd, uint8_t enable, uint8_t filter_dup, uint16_t duration, uint16_t period, int to) 
{
	struct hci_request rq;
	le_set_extended_scan_enable_cp ext_scan_cp;
	uint8_t status; 

	memset(&ext_scan_cp, 0x00, sizeof(ext_scan_cp));
	ext_scan_cp.enabled = enable;
	ext_scan_cp.filter_dup = filter_dup;
	ext_scan_cp.duration = duration;
	ext_scan_cp.period = period;

	memset(&rq, 0, sizeof(rq));
	rq.ogf = OGF_LE_CTL;
	rq.ocf = 0x0042;
	rq.cparam = &ext_scan_cp;
	rq.clen = sizeof(ext_scan_cp);
	rq.rparam = &status;
	rq.rlen = 1;

	if (hci_send_req(dd, &rq, to) < 0)
		return -1;

	if (status) {
		errno = EIO;
		return -status;
	}

	return 0;
}

#endif