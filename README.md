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

