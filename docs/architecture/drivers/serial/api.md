# Serial Driver API

## 1. Header

Public interface:

```c
#include "origin/drivers/serial/serial.h"
```

## 2. Functions

### `serial_init`

```c
void serial_init(void);
```

Initializes the COM1 UART for serial transmission.

**Configuration:**
- Base I/O address: `0x3F8`
- Baud rate: 115200
- Data bits: 8
- Parity: None
- Stop bits: 1
- FIFO: Enabled
- Interrupts: Disabled

**Parameters:** None.

**Return value:** None.

**Preconditions:** Must execute in a privileged context with access to x86 port-mapped I/O.

### `serial_write_char`

```c
void serial_write_char(const char character);
```

Transmits one character through COM1.

The function polls the UART Line Status Register at `COM1 + 5` until bit 5 (`THRE`) indicates that the transmitter holding register is empty. It then writes the character to the COM1 data register at `0x3F8`.

**Parameters:**
- `character`: Character to transmit.

**Return value:** None.

**Blocking behavior:** Busy-waits until the transmitter holding register is available. No timeout is implemented.

### `serial_write`

```c
void serial_write(const char *message);
```

Transmits a null-terminated string through COM1 by repeatedly calling `serial_write_char()`.

**Parameters:**
- `message`: Pointer to a null-terminated string.

**Return value:** None.

**Blocking behavior:** Each character is transmitted synchronously. The function returns after the terminating null character is encountered.

**Preconditions:** `message` must point to a valid, readable, null-terminated string. The function does not validate null pointers.

## 3. Usage

```c
serial_init();

serial_write("ORIGIN OS: serial initialized\n");
serial_write_char('A');
```

Initialize the driver before calling either transmission function.

## 4. Limitations

- Supports COM1 only.
- Transmit-only; receiving data is not implemented.
- Uses polling rather than interrupts.
- No transmit queue or buffering is implemented in software.
- No timeout or error reporting is provided.
- Requires x86-64 port I/O support and an accessible UART device.