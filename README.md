# Z80-Retro-Lib

This repository contains C Libraries based on NABU-LIB from DJSures [https://github.com/DJSures/NABU-LIB.git](https://github.com/DJSures/NABU-LIB.git)

The library functions have been altered, where appropriate, to work with the Z80-Retro.  There are a few important differences between the NABU and the Retro.

( My understanding of this stuff is a little limited - but this is a start.)

## Differences Between NABU and Retro

| |Nabu|Z80-Retro|Comments
|-|---|---|---
|Clock Speed|3.58Mhz|10Mhz
|VDP|TMS9918A|TMS9118A|Identical except for how VDP Ram is arrange
|VDP Port|0xA0, 0xA1|0x80,0x81|
|Audio|AY-3-8910|None|This version of the library stubs out the audio functions.
|Keyboard|RS422 Serial KB|None
|Serial|None|RS232 Serial with no flow control
|Joystick IO|Part of Keyboard protocol|IO Ports 0xA9 (0), 0xA8 (1)|This difference is completely abstracted by the library.
|Interrupts|Keyboard ISR, VDP ISR|No ISRs for serial input or VDP|The retro does not use interrupts.
|Control Register|Accessed via AY-3-8910 Port A|Direct access through IO port

## VDP Design Considreations

The Retro does not have the interrupt sent to the CPU.  This means we don't get to use asyncrhonous interrupt service routines in the retro.  We can, however, poll for the interrupt status flag in the VDP.

One gotcha I was made aware of recently was that reading the status register also resets the VRAM pointer address.  So, if your code is streaming data into VRAM, AND an interrupt triggers to read the status register, this will reset the VRAM pointer and your data might become _garbled_ on screen.

Timings are critical in the VDP.

Because of this, both the NABU-LIB and the Z80-Retro-Lib have a slightly slower than possible vdp_refreshViewPort() routine.

Here is a listing of the relevant code from the library with cycle counts included: - See: [https://github.com/reidrac/z80count](https://github.com/reidrac/z80count) for the tool used to get these counts.

```asm
      ld hl, __vdp_textBuffer; [10]
      ld de, (__vdpTextBufferSize); [20]

      vdp_refreshViewPortLoop3:

        ld a, (hl); [7]
        out (0x80), a; [11]

        inc hl; [6]
        dec de; [6]

        ld A, D; [4]
        or E; [4]
        jp nz, vdp_refreshViewPortLoop3; [10]

      pop de; [10]
      pop hl; [10]
```

The total count for the loop is: 48

| |Nabu|Z80-Retro|Unit|Comment
|-|---|---|---|---
CLOCK|3.58|10|Mhz	
PERIOD|0.27933|0.1|uS	
outir (16 cycles)|4.46928|1.6|uS|Time taken to complete a single VRAM write using `outir`
loop (48 cycles)|13.40784|4.8|uS|Time taken to complete a single VRAM write using `out in a loop`

## User input design considerations

The NABU Library relies on the NABU interrupt handler for the keyboard and the joystick.

The Retro has the joysticks IO Address mapped at A9 for Joystick 0 and A8 for Joystick 1.  The library also inverts the data read from these addresses to account for the hardware differences.

## IO Ports

The following table lists out the IO Ports for the Z80 Retro - Taken from : 

* https://github.com/Z80-Retro/2063-Z80-cpm/blob/main/lib/io.asm
* https://github.com/Z80-Retro/example-filesystem/blob/main/progs/tms9118/sprites.asm


| Name          | Port |Comment
| ------------- | ---- |-----------------------------------------------
| **GENERAL**   |      |
| gpio_in       | 0x00 | GP input port
| gpio_out      | 0x10 | GP output port
| prn_dat       | 0x20 | printer data out
| **SIO**       |      |
| sio_ad        | 0x30 | SIO port A, data
| sio_bd        | 0x31 | SIO port B, data
| sio_ac        | 0x32 | SIO port A, control
| sio_bc        | 0x33 | SIO port B, control
| **CTC**       |      |
| ctc_0         | 0x40 | CTC port 0
| ctc_1         | 0x41 | CTC port 1
| ctc_2         | 0x42 | CTC port 2
| ctc_3         | 0x43 | CTC port 3
| **FLASH**     |      |
| flash_disable | 0x70 | dummy-read from this port to disable the FLASH
| **VDP**       |      |
| vdp_vram      | 0x80 | VDP port for accessing the VRAM
| vdp_reg       | 0x81 | VDP port for accessing the registers
| joy0          | 0xa8 | I/O port for joystick 0 [J3 connector]
| joy1          | 0xa9 | I/O port for joystick 1 [J4 connector]

## BIT Assignments

The various IO devices have these associated bit settings.

| Device / Action  | Mask | Comment
| ---------------- | ---- | -------
| **JOYSTICK**     |      |
| joy_left         | 0x04 | and-mask for left
| joy_right        | 0x20 | and-mask for right
| joy_up           | 0x80 | and-mask for up
| joy_down         | 0x40 | and-mask for down
| joy_btn          | 0x01 | and-mask for button
| **GP Output**    |      |
| gpio_out_sd_mosi | 0x01 | 0b00000001
| gpio_out_sd_clk  | 0x02 | 0b00000010
| gpio_out_sd_ssel | 0x04 | 0b00000100
| gpio_out_prn_stb | 0x08 | 0b00001000
| gpio_out_a15     | 0x10 | 0b00010000
| gpio_out_a16     | 0x20 | 0b00100000
| gpio_out_a17     | 0x40 | 0b01000000
| gpio_out_a18     | 0x80 | 0b10000000
| **LO Bank**      |      |
| gpio_out_lobank  | 0xF0 | 0b11110000 (all lobank addr bits)
| **GP Input**     |      |
| gpio_in_prn_err  | 0x01 | 0b00000001
| gpio_in_prn_stat | 0x02 | 0b00000010
| gpio_in_prn_papr | 0x04 | 0b00000100
| gpio_in_prn_bsy  | 0x08 | 0b00001000
| gpio_in_prn_ack  | 0x10 | 0b00010000
| gpio_in_user1    | 0x20 | 0b00100000
| gpio_in_sd_det   | 0x40 | 0b01000000
| gpio_in_sd_miso  | 0x80 | 0b10000000
