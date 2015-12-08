/*
 *  Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *  Not a Contribution.
 *  Copyright (C) 2009-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef HCI_UART_H
#define HCI_UART_H
#include "hw_rome.h"

/* Variables to identify the platform */
/*BT HS UART TTY DEVICE */
#define BT_HS_UART_DEVICE "/dev/ttyHS0"

/**** baud rates ****/
#define USERIAL_BAUD_300        BAUDRATE_Reserved +1
#define USERIAL_BAUD_600        BAUDRATE_Reserved +2
#define USERIAL_BAUD_1200       BAUDRATE_Reserved +3
#define USERIAL_BAUD_2400       BAUDRATE_Reserved +4
#define USERIAL_BAUD_9600       BAUDRATE_9600
#define USERIAL_BAUD_19200      BAUDRATE_19200
#define USERIAL_BAUD_57600      BAUDRATE_57600
#define USERIAL_BAUD_115200     BAUDRATE_115200
#define USERIAL_BAUD_230400     BAUDRATE_230400
#define USERIAL_BAUD_460800     BAUDRATE_460800
#define USERIAL_BAUD_921600     BAUDRATE_921600
#define USERIAL_BAUD_1M               BAUDRATE_1000000
#define USERIAL_BAUD_1_5M       BAUDRATE_Reserved +5
#define USERIAL_BAUD_2M         BAUDRATE_2000000
#define USERIAL_BAUD_3M         BAUDRATE_3000000
#define USERIAL_BAUD_3_2M         BAUDRATE_3200000
#define USERIAL_BAUD_4M         BAUDRATE_4000000
#define USERIAL_BAUD_AUTO       BAUDRATE_AUTO

/****   Default BT Baudrate ****/
#define BT_BAUD_RATE   USERIAL_BAUD_3_2M

/**** Data Format ****/
/* Stop Bits */
#define USERIAL_STOPBITS_1      1
#define USERIAL_STOPBITS_1_5    (1<<1)
#define USERIAL_STOPBITS_2      (1<<2)

/* Parity Bits */
#define USERIAL_PARITY_NONE     (1<<3)
#define USERIAL_PARITY_EVEN     (1<<4)
#define USERIAL_PARITY_ODD      (1<<5)

/* Data Bits */
#define USERIAL_DATABITS_5      (1<<6)
#define USERIAL_DATABITS_6      (1<<7)
#define USERIAL_DATABITS_7      (1<<8)
#define USERIAL_DATABITS_8      (1<<9)

/* Flow control */
#define USERIAL_CTSRTS               (1<<15)

/* HCI Packet types */
#define HCI_COMMAND_PKT     0x01
#define HCI_ACLDATA_PKT      0x02
#define HCI_SCODATA_PKT     0x03
#define HCI_EVENT_PKT           0x04
#define HCI_VENDOR_PKT        0xff

/* HCI Command/Event Opcode */
#define HCI_RESET                       0x0C03
#define EVT_CMD_COMPLETE       0x0E

/* Command opcode pack/unpack */
#define cmd_opcode_pack(ogf, ocf)   (uint16_t)((ocf & 0x03ff)|(ogf << 10))

#if (BT_WAKE_VIA_USERIAL_IOCTL==TRUE)
/* These are the ioctl values used for bt_wake ioctl via UART driver. you may
 * need to redefine them on you platform!
 * Logically they need to be unique and not colide with existing uart ioctl's.
 */
#ifndef USERIAL_IOCTL_BT_WAKE_ASSERT
#define USERIAL_IOCTL_BT_WAKE_ASSERT   0x8003
#endif
#ifndef USERIAL_IOCTL_BT_WAKE_DEASSERT
#define USERIAL_IOCTL_BT_WAKE_DEASSERT 0x8004
#endif
#ifndef USERIAL_IOCTL_BT_WAKE_GET_ST
#define USERIAL_IOCTL_BT_WAKE_GET_ST   0x8005
#endif
#endif // (BT_WAKE_VIA_USERIAL_IOCTL==TRUE)

/* UART CLOCK IOCTLS*/
#define USERIAL_OP_CLK_ON 0x5441
#define USERIAL_OP_CLK_OFF 0x5442
#define USERIAL_OP_CLK_STATE 0x5443
/******************************************************************************
**  Type definitions
******************************************************************************/

/* Structure used to configure serial port during open */
typedef struct
{
    uint16_t fmt;       /* Data format */
    uint8_t  baud;      /* Baud rate */
} tUSERIAL_CFG;

typedef enum {
#if (BT_WAKE_VIA_USERIAL_IOCTL==TRUE)
    USERIAL_OP_ASSERT_BT_WAKE,
    USERIAL_OP_DEASSERT_BT_WAKE,
    USERIAL_OP_GET_BT_WAKE_STATE,
#endif
    USERIAL_OP_FLOW_ON,
    USERIAL_OP_FLOW_OFF,
    USERIAL_OP_NOP,
} userial_vendor_ioctl_op_t;

/* UPIO signals */
enum {
    UPIO_BT_WAKE = 0,
    UPIO_HOST_WAKE,
    UPIO_LPM_MODE,
    UPIO_MAX_COUNT
};

/* UPIO assertion/deassertion */
enum {
    UPIO_UNKNOWN = 0,
    UPIO_DEASSERT,
    UPIO_ASSERT
};

#define VND_PORT_NAME_MAXLEN    256

/* vendor serial control block */
typedef struct
{
    int fd;                     /* fd to Bluetooth device */
    struct termios termios;     /* serial terminal of BT port */
    char port_name[VND_PORT_NAME_MAXLEN];
} vnd_userial_cb_t;

typedef struct {
    uint8_t     ncmd;
    uint16_t    opcode;
} __attribute__ ((packed)) evt_cmd_complete;

typedef struct {
    uint8_t     status;
    uint8_t     ncmd;
    uint16_t    opcode;
} __attribute__ ((packed)) evt_cmd_status;

typedef struct {
    uint16_t    opcode;
    uint8_t     plen;
} __attribute__ ((packed))  hci_command_hdr;

typedef struct {
    uint8_t     evt;
    uint8_t     plen;
} __attribute__ ((packed))  hci_event_hdr;

/******************************************************************************
**  Extern
******************************************************************************/
extern vnd_userial_cb_t vnd_userial;


/*******************************************************************************
**
** Function        userial_vendor_init
**
** Description     Initialize userial vendor-specific control block
**
** Returns         None
**
*******************************************************************************/
void userial_vendor_init(void);

/*******************************************************************************
**
** Function        userial_vendor_open
**
** Description     Open the serial port with the given configuration
**
** Returns         device fd
**
*******************************************************************************/
int userial_vendor_open(tUSERIAL_CFG *p_cfg);

/*******************************************************************************
**
** Function        userial_vendor_close
**
** Description     Conduct vendor-specific close work
**
** Returns         None
**
*******************************************************************************/
void userial_vendor_close(void);

/*******************************************************************************
**
** Function        userial_vendor_set_baud
**
** Description     Set new baud rate
**
** Returns         None
**
*******************************************************************************/
void userial_vendor_set_baud(uint8_t userial_baud);

/*******************************************************************************
**
** Function        userial_vendor_get_baud
**
** Description     Get current baud rate
**
** Returns         int
**
*******************************************************************************/
int userial_vendor_get_baud(void);

/*******************************************************************************
**
** Function        userial_vendor_ioctl
**
** Description     ioctl inteface
**
** Returns         int error
**
*******************************************************************************/
int userial_vendor_ioctl(userial_vendor_ioctl_op_t op, int *p_data);

/*******************************************************************************
**
** Function        read_hci_event
**
** Description     Read HCI event during vendor initialization
**
** Returns         int: size to read
**
*******************************************************************************/
int read_hci_event(int fd, unsigned char* buf, int size);

#endif /* HCI_UART_H */
