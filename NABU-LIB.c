// ****************************************************************************************
// NABU-LIB C Library
// DJ Sures (c) 2023
// https://nabu.ca
// https://github.com/DJSures/NABU-LIB
// 
// Read the NABU-LIB.h file for details of each function in this file.
//
// **********************************************************************************************



// **************************************************************************
// System
// ------
// **************************************************************************

void initNABULib() {

  __asm
    push af;
    ld a, r;
    ld (__randomSeed), a;
    pop af;
  __endasm;

  srand(_randomSeed);

}

void initNABULIBAudio() {
  ;
}

void nop() {
  __asm
    NOP
  __endasm;
}

void NABU_DisableInterrupts() {

  __asm
    di
  __endasm;
}

void NABU_EnableInterrupts() {

  __asm
    ei
  __endasm;
}

void RightShift(uint8_t *arr, uint16_t len, uint8_t n) {
        
  uint8_t *toPtr = arr + (len - 1);
  uint8_t *fromPtr = toPtr - n;
  uint8_t *endPtr = arr - 1;

  while (fromPtr != endPtr) {

    *toPtr = *fromPtr;
    toPtr--;
    fromPtr--;
  }
  
  fromPtr = arr + (n - 1);
  
  while (fromPtr != endPtr) {

    *fromPtr = 0x20;
    fromPtr--;
  }   
}

// **************************************************************************
// Interrupts
// -----
// **************************************************************************

#ifndef DISABLE_HCCA_RX_INT  
  void isrHCCARX() {
    ;
  }
#endif 

#ifndef DISABLE_KEYBOARD_INT
  void isrKeyboard() {
    ;
  }
#endif

#if BIN_TYPE == BIN_CPM

  // **************************************************************************
  // VT51 for CPM
  // ------------------
  // **************************************************************************

  void vt_clearToEndOfScreen() {

    printf("%cJ", 27);
  }

  void vt_clearToEndOfLine() {

    printf("%cK", 27);
  }

  void vt_clearScreen() {

    if (_EMULATION_MODE == 0)
      putchar(26);
    else
      printf("%cE", 27);
  }

  void vt_clearLine() {

    printf("%cl", 27);
  }

  void vt_clearToStartOfLine() {

    printf("%co", 27);
  }

  void vt_clearToStartOfScreen() {

    printf("%cd", 27);
  }

  void vt_moveCursorDown(uint8_t count) {

    for (uint8_t i = 0; i < count; i++)
      printf("%cB", 27);
  }

  void vt_cursorHome() {

    printf("%cH", 27);
  }

  void vt_moveCursorLeft(uint8_t count) {

    for (uint8_t i = 0; i < count; i++)
      printf("%cD", 27);
  }

  void vt_moveCursorRight(uint8_t count) {

    for (uint8_t i = 0; i < count; i++)
      printf("%cC", 27);
  }

  void vt_moveCursorUp(uint8_t count) {

    for (uint8_t i = 0; i < count; i++)
      printf("%cA", 27);
  }

  void vt_deleteLine() {

    printf("%cM", 27);
  }

  void vt_setCursor(uint8_t x, uint8_t y) {

    putchar(27);
    if (_EMULATION_MODE == 0)
      putchar('=');
    else
      putchar('Y');
      
    putchar(32 + y);
    putchar(32 + x);
  }

  void vt_foregroundColor(uint8_t color) {

    putchar(27);
    putchar('b');
    putchar(color);
  }

  void vt_insertLine() {

    printf("%cL", 27);
  }

  void vt_restoreCursorPosition() {

    printf("%ck", 27);
  }

  void vt_backgroundColor(uint8_t color) {

    putchar(27);
    putchar('c');
    putchar(color);
  }

  void vt_saveCursorPosition() {

    printf("%cj", 27);
  }

  void vt_cursorUpAndInsert() {

    printf("%cI", 27);
  }

  void vt_wrapOff() {

    printf("%cw", 27);
  }

  void vt_wrapOn() {

    printf("%cv", 27);
  }

  void vt_normalVideo() {

    printf("%cq", 27);
  }

  void vt_reverseVideo() {

    printf("%cp", 27);
  }

  bool isCloudCPM() {
    return false;
  }
#endif




// **************************************************************************
// Sound
// -----
// **************************************************************************

void ayWrite(uint8_t reg, uint8_t val) {
  (void)reg;
  (void)val;
}

uint8_t ayRead(uint8_t reg) {
  (void)reg;
  return 0;
}

void playNoteDelay(uint8_t channel, uint8_t note, uint16_t delayLength) {
  (void)channel;
  (void)note;
  (void)delayLength;
}

// **************************************************************************
// Keyboard
// --------
// **************************************************************************

#ifndef DISABLE_KEYBOARD_INT
    uint8_t isKeyPressed() __naked {
      // this is dubious, at best
		__asm
      in    a,(0x32)	  ; read sio control status byte
      and   1			      ; check the rcvr ready bit
			ld		l,a
			ret
		__endasm;
    }

    uint8_t getChar() __naked {

      __asm
        in    a,(0x30)  ; read sio control status byte
        ld		l,a
        ret
      __endasm;
    }

    uint8_t getJoyStatus(uint8_t joyNum) {
        switch (joyNum)
        {
        case 0:
            return ~IO_JOY0;
        case 1:
            return ~IO_JOY1;
        }
        return 0;
    }

  #ifndef DISABLE_VDP
    uint8_t readLine(uint8_t *buffer, uint8_t maxInputLen) {

      uint8_t i = 0;

      while (true) {

        uint8_t c = getChar();

        if (c == 13) {

          break;
        } else if (i < maxInputLen && c >= 0x20 && c <= 126) {

          buffer[i] = c;

          vdp_write(c);

          i++;
        } else if (i == maxInputLen && c != 127) {

          playNoteDelay(0, 60, 10);
        } else if (c == 127 && i > 0) {

          if (vdp_cursor.x == 0) {

            vdp_cursor.x = 39;
            vdp_cursor.y--;
          } else {

            vdp_cursor.x--;
          }

          i--;

          vdp_writeCharAtLocation(vdp_cursor.x, vdp_cursor.y, ' ');
        }
      }

      return i;
    }
  #endif
#endif


#ifndef DISABLE_HCCA_RX_INT

  // **************************************************************************
  // HCCA Receive
  // ------------
  // **************************************************************************

  bool hcca_ping() {
    
    return false;
  }

  bool hcca_isRxBufferAvailable() {
    
    return false;
  }

  uint8_t hcca_getSizeOfDataInBuffer() {
    
    return 0;
  }

  uint8_t hcca_readByte() {
    
    return 0;
  }

  uint16_t hcca_readUInt16() {
    
    return 0;
  }

  int16_t hcca_readInt16() {
    
    return 0;
  }

  uint32_t hcca_readUInt32() {
    
    return 0;
  }

  int32_t hcca_readInt32() {
    
    return 0;
  }

  void hcca_readBytes(uint16_t offset, uint16_t bufferLen, uint8_t *buffer) {
    
    (void)offset;
    (void)bufferLen;
    (void)buffer;
  }


  // **************************************************************************
  // HCCA Transmit
  // -------------
  // **************************************************************************

  void hcca_writeByte(uint8_t c) {
    
    (void)c;
  }

  void hcca_writeUInt32(uint32_t val) {
    
    (void)val;
  }

  void hcca_writeInt32(int32_t val) {
    
    (void)val;
  }

  void hcca_writeUInt16(uint16_t val) {
    
    (void)val;
  }

  void hcca_writeInt16(int16_t val) {
    
    (void)val;
  }

  void hcca_writeString(uint8_t *str) {
    
    (void)str;
  }

  void hcca_writeBytes(uint16_t offset, uint16_t length, uint8_t *bytes) {
    (void)offset;
    (void)length;
    (void)bytes;
  }
#endif


// **************************************************************************
// VDP 
// ---
// **************************************************************************

#ifndef DISABLE_VDP

  void vdp_setRegister(uint8_t registerIndex, uint8_t value) {

    IO_VDPLATCH = value;

    IO_VDPLATCH = 0x80 | registerIndex;
  }

  void vdp_setWriteAddress(uint16_t address) __z88dk_fastcall {

    IO_VDPLATCH = address & 0xff;

    IO_VDPLATCH = 0x40 | ((address >> 8) & 0x3f);
  }

  void vdp_setReadAddress(uint16_t address) __z88dk_fastcall {

    IO_VDPLATCH = address & 0xff;

    IO_VDPLATCH = (address >> 8) & 0x3f;
  }

  void vdp_waitVDPReadyInt() {
    while(true) {
      vdpStatusRegVal = IO_VDPLATCH;
      if ((vdpStatusRegVal & 0x80) != 0) {
        break;
      }
    }
  }
  
  void waitVdpISR() {
    ;
  }
  void vdp_enableVDPReadyInt() {
    vdp_setRegister(1, _vdpReg1Val | 0b00100000 );  
  }
  void vdp_disableVDPReadyInt() {
    vdp_setRegister(1, _vdpReg1Val);
  }
  void vdp_addISR(void (*isr)()) {
    (void)isr;
  }
  void vdp_removeISR() {
    ;
  }



  void vdp_init(uint8_t mode, uint8_t fgColor, uint8_t bgColor, bool big_sprites, bool magnify, bool autoScroll, bool splitThirds) {

    _vdpMode = mode;

    _vdpSplitThirds = splitThirds;

    _vdpSpriteSizeSelected = big_sprites;

    _autoScroll = autoScroll;

    _vdpInterruptEnabled = false;

    vdp_cursor.x = 0;
    vdp_cursor.y = 0;
    
    switch (mode) {

      case VDP_MODE_G2:

        vdp_setRegister(0, 0b00000010); 

        _vdpReg1Val = 0b11000000 | (big_sprites << 1) | magnify; 
        vdp_setRegister(1, _vdpReg1Val); 

        if (_vdpSplitThirds)
          vdp_setRegister(4, 0x03);
        else
          vdp_setRegister(4, 0x00);
        
        _vdpPatternGeneratorTableAddr = 0x00;

        fgColor = 0;
        _vdpCursorMaxX = 31;
        _vdpCursorMaxXFull = 32;
        _vdpTextBufferSize = 768;

        break;

      case VDP_MODE_TEXT:

        vdp_setRegister(0, 0b00000000); 

        _vdpReg1Val = 0b11010010;
        vdp_setRegister(1, _vdpReg1Val);

        vdp_setRegister(4, 0x00);
        _vdpPatternGeneratorTableAddr = 0x00;


        // https://konamiman.github.io/MSX2-Technical-Handbook/md/Appendix5.html#screen-1--graphic-1
        // vdp_setRegister(2, 0x06);
        // vdp_setRegister(3, 0xff);
        // vdp_setRegister(4, 0x03);
        // vdp_setRegister(5, 0x36); 
        // vdp_setRegister(6, 0x07);


        _vdpCursorMaxX = 39;
        _vdpCursorMaxXFull = 40;
        _vdpTextBufferSize = 960;

        break;

      case VDP_MODE_MULTICOLOR:

        vdp_setRegister(0, 0b00000000); 

        _vdpReg1Val = 0b11001000 | (big_sprites << 1) | magnify;
        vdp_setRegister(1, _vdpReg1Val); 

        vdp_setRegister(4, 0x00);
        _vdpPatternGeneratorTableAddr = 0x00;

        fgColor = 0;
        _vdpCursorMaxX = 31;
        _vdpCursorMaxXFull = 32;
        _vdpTextBufferSize = 768;

        break;
    }

    vdp_setRegister(2, 0x06);
    _vdpPatternNameTableAddr = 0x1800;     

    if (_vdpSplitThirds)
      vdp_setRegister(3, 0xff);
    else
      vdp_setRegister(3, 0x9f);
    _vdpColorTableAddr = 0x2000;

    vdp_setRegister(5, 0x36);
    _vdpSpriteAttributeTableAddr = 0x1b00;

    vdp_setRegister(6, 0x07);
    _vdpSpriteGeneratorTableAddr = 0x3800;

    vdp_setRegister(7, (fgColor << 4) | (bgColor & 0x0f));

    vdp_clearScreen();
  }

  void vdp_clearVRAM() {

    vdp_setWriteAddress(0x00);

    for (uint16_t i = 0; i < 0x3FFF; i++)
      vdp_put(0);
  }

  void vdp_initMSXMode(uint8_t bgColor) __z88dk_fastcall {

    // https://konamiman.github.io/MSX2-Technical-Handbook/md/Appendix5.html#screen-1--graphic-1

    vdp_setRegister(0, 0b00000010); 
    vdp_setRegister(1, 0b11000000); 
    vdp_setRegister(2, 0x06);
    vdp_setRegister(3, 0xff); // <- ALL LIES! how is 0xff 0x2000? 
    vdp_setRegister(4, 0x03);
    vdp_setRegister(5, 0x36); 
    vdp_setRegister(6, 0x07);

    vdp_setRegister(7, bgColor & 0x0f);
  }

  void vdp_initTextMode(uint8_t fgColor, uint8_t bgColor, bool autoScroll) {

    vdp_init(VDP_MODE_TEXT, fgColor, bgColor , false, false, autoScroll, false);
  }

  void vdp_initG2Mode(uint8_t bgColor, bool bigSprites, bool scaleSprites, bool autoScroll, bool splitThirds) {

    vdp_init(VDP_MODE_G2, 0, bgColor, bigSprites, scaleSprites, autoScroll, splitThirds);
  }

  void vdp_initMultiColorMode() {

    vdp_init(VDP_MODE_MULTICOLOR, 0, 0, false, false, false, false);
  }

  void vdp_clearScreen() {
      
    vdp_setWriteAddress(_vdpPatternNameTableAddr);

    uint8_t *start = _vdp_textBuffer;
    uint8_t *end = start + (_vdpCursorMaxXFull * 24);

    uint8_t cr = 0x00;

    if (_vdpMode == VDP_MODE_TEXT)
      cr = 0x20;

    do {

      vdp_put(cr);

      *start = 0x20;

      start++;
    } while (start != end);
  }

  void vdp_fillScreen(uint8_t c) __z88dk_fastcall {

    vdp_setWriteAddress(_vdpPatternNameTableAddr);
    uint8_t *start = _vdp_textBuffer;
    uint8_t *end = start + (_vdpCursorMaxXFull * 24);

    do {
      vdp_put(c);
      *start = c;
      start++;
    } while (start != end);
  }

  void vdp_clearRows(uint8_t topRow, uint8_t bottomRow) {
    
    uint16_t name_offset = topRow * _vdpCursorMaxXFull;

    vdp_setWriteAddress(_vdpPatternNameTableAddr + name_offset);

    uint8_t *start = _vdp_textBuffer + (topRow * _vdpCursorMaxXFull);
    uint8_t *end   = _vdp_textBuffer + (bottomRow * _vdpCursorMaxXFull);

    do {
      
      vdp_put(0x20);
      *start = 0x20;

      start++;
    } while (start != end);
  }

  void vdp_loadASCIIFont(uint8_t *font) __z88dk_fastcall {

    vdp_setWriteAddress(_vdpPatternGeneratorTableAddr + 0x100);

    uint8_t *start = font;
    uint8_t *end = start + 768;

    do {

      vdp_put(*start);
      start++;
    } while (start != end);
  }

  void vdp_loadASCIIFontWithInverse(uint8_t *font) __z88dk_fastcall {

    vdp_setWriteAddress(_vdpPatternGeneratorTableAddr + 0x100);

    uint8_t *start = font;
    uint8_t *end = start + 768;

    do {

      vdp_put(*start);
      start++;
    } while (start != end);

    start = font;
    do {

      vdp_put(*start ^ 0xff);
      start++;
    } while (start != end);
  }

  void vdp_putPattern(uint8_t x, uint8_t y, uint8_t patternId) {

    uint16_t name_offset = y * _vdpCursorMaxXFull + x;

    vdp_setWriteAddress(_vdpPatternNameTableAddr + name_offset);
    _vdp_textBuffer[name_offset] = patternId;
    
    // IO_VDPDATA = patternId;
    vdp_put(patternId);
  }

  void vdp_loadPatternTable(uint8_t *patternTable, uint16_t len) {

    // datasheet 2-20 : screen is split into 3 and the pattern table therefore is repeated 3 times

    uint8_t *start = patternTable;
    uint8_t *end = patternTable + len;

    vdp_setWriteAddress(_vdpPatternGeneratorTableAddr);
    do {

      vdp_put(*start);

      start++;
    } while (start != end);

    if (_vdpSplitThirds) { 

      vdp_setWriteAddress(_vdpPatternGeneratorTableAddr + 2048);
      start = patternTable;
      do {

        vdp_put(*start);
        start++;
      } while (start != end);

      vdp_setWriteAddress(_vdpPatternGeneratorTableAddr + 4096);
      start = patternTable;
      do {

        vdp_put(*start);
        start++;
      } while (start != end);
    }
  }

  // Set all values in the color table to color.
  void vdp_setPatternColor(uint8_t color) __z88dk_fastcall {
    vdp_setWriteAddress(_vdpColorTableAddr);
    for (uint16_t i = 0; i < 0x1800; i++) {
      IO_VDPDATA = color;
      nop();
      nop();
      nop();
      nop();
    }
  }

  void vdp_loadColorTable(uint8_t *colorTable, uint16_t len) {

    // datasheet 2-20 : screen is split into 3 and the color table therefore is repeated 3 times
    uint8_t *start = colorTable;
    uint8_t *end = colorTable + len;
      
    vdp_setWriteAddress(_vdpColorTableAddr);
    do {

      vdp_put(*start);
      start++;
    } while (start != end);

    if (_vdpSplitThirds) {

      vdp_setWriteAddress(_vdpColorTableAddr + 2048);
      start = colorTable;
      do {

        vdp_put(*start);
        start++;
      } while (start != end);

      vdp_setWriteAddress(_vdpColorTableAddr + 4096);
      start = colorTable;
      do {

        vdp_put(*start);
        start++;
      } while (start != end);
    }
  }

  void vdp_colorizePattern(uint8_t patternId, uint8_t fg, uint8_t bg) {

    uint16_t pt = patternId * 8;

    uint8_t c = (fg << 4) + bg;

    vdp_setWriteAddress(_vdpColorTableAddr + pt);
    for (uint8_t i = 0; i < 8; i++)
      vdp_put(c);

    if (_vdpSplitThirds) {

      vdp_setWriteAddress(_vdpColorTableAddr + 2048  + pt);
      for (uint8_t i = 0; i < 8; i++)
        vdp_put(c);

      vdp_setWriteAddress(_vdpColorTableAddr + 4096 + pt);
      for (uint8_t i = 0; i < 8; i++)
        vdp_put(c);
    }
  }

  void vdp_plotHires(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2) {

    uint16_t offset = 8 * (x / 8) + y % 8 + 256 * (y / 8);

    vdp_setReadAddress(_vdpPatternGeneratorTableAddr + offset);
    uint8_t pixel = IO_VDPDATA;

    vdp_setReadAddress(_vdpColorTableAddr + offset);
    uint8_t color = IO_VDPDATA;

    if (color1 != NULL) {

      pixel |= 0x80 >> (x % 8); // Set bit a "1"
      color = (color & 0x0F) | (color1 << 4);
    } else {

      pixel &= ~(0x80 >> (x % 8)); // Set bit as "0"
      color = (color & 0xF0) | (color2 & 0x0F);
    }

    vdp_setWriteAddress(_vdpPatternGeneratorTableAddr + offset);
    vdp_put(pixel);

    vdp_setWriteAddress(_vdpColorTableAddr + offset);
    vdp_put(color);
  }

  void vdp_plotColor(uint8_t x, uint8_t y, uint8_t color) {

    if (_vdpMode == VDP_MODE_MULTICOLOR) {

      uint16_t addr = _vdpPatternGeneratorTableAddr + 8 * (x / 2) + y % 8 + 256 * (y / 8);

      vdp_setReadAddress(addr);
      uint8_t dot = IO_VDPDATA;

      vdp_setWriteAddress(addr);

      if (x & 1) // Odd columns
        vdp_put((dot & 0xF0) + (color & 0x0F));
      else
        vdp_put( (dot & 0x0F) + (color <<4));
    } else if (_vdpMode == VDP_MODE_G2) {

      // Draw bitmap
      uint16_t offset = 8 * (x / 2) + y % 8 + 256 * (y / 8);

      vdp_setReadAddress(_vdpColorTableAddr + offset);
      uint8_t color_ = IO_VDPDATA;

      if ((x & 1) == 0) {

        // Even 
        color_ &= 0x0F;
        color_ |= (color << 4);
      } else {

        color_ &= 0xF0;
        color_ |= color & 0x0F;
      }

      vdp_setWriteAddress(_vdpPatternGeneratorTableAddr + offset);
      vdp_put(0xF0);

      vdp_setWriteAddress(_vdpColorTableAddr + offset);
      vdp_put(color_);
    }
  }

  void vdp_disableSprite(uint8_t id) __z88dk_fastcall {

    uint16_t addr = _vdpSpriteAttributeTableAddr + 4 * id;

    vdp_setWriteAddress(addr);
    vdp_put(192);        // y
    vdp_put(0);          // x
    vdp_put(0);          // pattern name #0 (should always be empty so no false collisions)
    vdp_put(0b10000000); // early clock to hide behind border
  }

  void vdp_loadSpritePatternNameTable(uint16_t numSprites, uint8_t *sprite) {

    for (uint8_t i = 0; i < 32; i++)
      vdp_disableSprite(i);

    if (_vdpSpriteSizeSelected) {

      vdp_setWriteAddress(_vdpSpriteGeneratorTableAddr);

      uint16_t end = numSprites * 32;

      for (uint16_t i = 0; i < end; i++) 
        vdp_put(sprite[i]);
    } else {

      vdp_setWriteAddress(_vdpSpriteGeneratorTableAddr);

      uint16_t end = numSprites * 8;
      
      for (uint16_t i = 0; i < end; i++) 
        vdp_put(sprite[i]);
    }
  }

  uint8_t vdp_spriteInit(uint8_t id, uint8_t spritePatternNameId, uint8_t x, uint8_t y, uint8_t color) {

    uint16_t addr = _vdpSpriteAttributeTableAddr + 4 * id;

    vdp_setWriteAddress(addr);
    
    vdp_put(y); // y
    
    vdp_put(x); // x

    if (_vdpSpriteSizeSelected)
      vdp_put(spritePatternNameId * 4); // 16x16 sprite location (name). big sprites take 4 8x8 blocks (datasheet 2-34)
    else
      vdp_put(spritePatternNameId);     // 8x8 sprite location (name

    // Set bit 7 to hide the sprite since we're not using it yet
    vdp_put(color & 0x0f);

    return id;
  }

  void vdp_setSpriteColor(uint8_t id, uint8_t color) {

    uint16_t addr = _vdpSpriteAttributeTableAddr + 4 * id;

    vdp_setWriteAddress(addr + 3);

    vdp_put(color & 0x0f);
  }

  void vdp_setSpritePosition(uint8_t id, uint8_t x, uint8_t y) {

    uint16_t addr = _vdpSpriteAttributeTableAddr + 4 * id;

    vdp_setWriteAddress(addr);
    vdp_put(y);
    vdp_put(x);
  }

  void vdp_setSpritePositionAndColor(uint8_t id, uint8_t x, uint8_t y, uint8_t color) {

    uint16_t addr = _vdpSpriteAttributeTableAddr + 4 * id;

    vdp_setWriteAddress(addr);
    vdp_put(y);
    vdp_put(x);
    vdp_put(id);
    vdp_put(color & 0x0f);
  }

  void vdp_getSpritePosition(uint8_t id, uint8_t *xpos, uint8_t *ypos) {

    uint16_t addr = _vdpSpriteAttributeTableAddr + 4 * id;

    vdp_setReadAddress(addr);

    *ypos = IO_VDPDATA;

    *xpos = IO_VDPDATA;
  }

  void vdp_print(uint8_t *text) __z88dk_fastcall {

    uint8_t *start = text;

    while (*start != 0x00) {

      vdp_write(*start);

      start++;
    }
  }

  void vdp_printColorized(uint8_t *text, uint8_t fgColor, uint8_t bgColor) {

    uint8_t *start = text;

    while (*start != 0x00) {

      vdp_colorizePattern(*start, fgColor, bgColor);

      vdp_write(*start);

      start++;
    }
  }

  void vdp_printPart(uint16_t offset, uint16_t textLength, uint8_t *text) {

    uint8_t *start = text + offset;
    uint8_t *end   = start + textLength;

    while (start != end) {

      vdp_write(*start);

      start++;
    }
  }

  void vdp_newLine() {

    if (vdp_cursor.y == 23) {

      if (_autoScroll)
        vdp_scrollTextUp(0, 23);

      vdp_cursor.x = 0;
    } else {

      vdp_setCursor2(0, ++vdp_cursor.y);
    }
  }

  void vdp_setBackDropColor(uint8_t color) __z88dk_fastcall {

    vdp_setRegister(7, color);
  }

  void vdp_setCursor2(uint8_t col, uint8_t row) {

    if (col > _vdpCursorMaxX) {

      col = 0;
      row++;
    }

    if (row > _vdpCursorMaxY)
      row = 0;

    vdp_cursor.x = col;
    vdp_cursor.y = row;
  }

  void vdp_setCursor(uint8_t direction) __z88dk_fastcall {

    switch (direction) {
      case VDP_CURSOR_UP:
        vdp_setCursor2(vdp_cursor.x, vdp_cursor.y - 1);
        break;
      case VDP_CURSOR_DOWN:
        vdp_setCursor2(vdp_cursor.x, vdp_cursor.y + 1);
        break;
      case VDP_CURSOR_LEFT:
        vdp_setCursor2(vdp_cursor.x - 1, vdp_cursor.y);
        break;
      case VDP_CURSOR_RIGHT:
        vdp_setCursor2(vdp_cursor.x + 1, vdp_cursor.y);
        break;
    }
  }

  void vdp_setTextColor(uint8_t fg, uint8_t bg) {

    vdp_setRegister(7, (fg << 4) + bg);
  }

  void vdp_write(uint8_t chr) __z88dk_fastcall {

    uint16_t name_offset = vdp_cursor.y * _vdpCursorMaxXFull + vdp_cursor.x;

    vdp_setWriteAddress(_vdpPatternNameTableAddr + name_offset);

    vdp_put(chr);

    _vdp_textBuffer[name_offset] = chr;

    if (_autoScroll && vdp_cursor.x == _vdpCursorMaxX && vdp_cursor.y == 23) {

      vdp_scrollTextUp(0, 23);

      vdp_cursor.x = 0;
    }

    vdp_setCursor2(vdp_cursor.x + 1, vdp_cursor.y);
  }

  void vdp_writeCharAtLocation(uint8_t x, uint8_t y, uint8_t c) {

    uint16_t name_offset = y * _vdpCursorMaxXFull + x; 

    _vdp_textBuffer[name_offset] = c;
      
    vdp_setWriteAddress(_vdpPatternNameTableAddr + name_offset);

    vdp_put(c);
  }

  uint8_t vdp_getCharAtLocationVRAM(uint8_t x, uint8_t y) {

    uint16_t name_offset = y * _vdpCursorMaxXFull + x; 

    vdp_setReadAddress(_vdpPatternNameTableAddr + name_offset);

    return IO_VDPDATA;
  }

  uint8_t vdp_getCharAtLocationBuf(uint8_t x, uint8_t y) {

    return _vdp_textBuffer[y * _vdpCursorMaxXFull + x];
  }

  void vdp_setCharAtLocationBuf(uint8_t x, uint8_t y, uint8_t c) {

    _vdp_textBuffer[y * _vdpCursorMaxXFull + x] = c;
  }

  void vdp_refreshViewPort() {
  
    // vdp_setWriteAddress(_vdpPatternNameTableAddr);

    __asm
      ld hl, __vdp_textBuffer;
      ld de, (__vdpPatternNameTableAddr);
      ld bc, (__vdpTextBufferSize);
      ld	a,e;
      out	(0x81),a;
      ld	a,d;
      or	0x40;
      out	(0x81),a;

      ld	d,b
      ld	e,c;

      ld	c,0x80;
      ld	b,e;
      inc	e;
      dec	e;
      jr	z,vdp_write_loop;
      inc	d;

    vdp_write_loop:
	    outi;
	    jp	nz,vdp_write_loop;
	    dec	d;
	    jp	nz,vdp_write_loop;
      in  a,(0x81);
      __endasm;
  }

  void vdp_scrollTextUp(uint8_t topRow, uint8_t bottomRow) {

    vdp_setWriteAddress(_vdpPatternNameTableAddr + (topRow * _vdpCursorMaxXFull));

    uint8_t *to   = _vdp_textBuffer + (topRow * _vdpCursorMaxXFull);
    uint8_t *from = to + _vdpCursorMaxXFull;
    uint8_t *end  = _vdp_textBuffer + ((bottomRow + 1) * _vdpCursorMaxXFull);

    do {

      *to = *from;

      vdp_put(*to);

      to++;
      from++;
    } while (from != end);
    
    do {

      *to = 0x20;
      vdp_put(0x20);
      

      to++;
    } while (to != end);
  }

  void vdp_scrollTextDown(uint8_t topRow, uint8_t bottomRow) {

    uint8_t *fromPtr = _vdp_textBuffer + (bottomRow * _vdpCursorMaxXFull) - 1;
    uint8_t *toPtr   = fromPtr + _vdpCursorMaxXFull;
    uint8_t *endPtr  = (_vdp_textBuffer - 1) + (topRow * _vdpCursorMaxXFull);

    do {

      *toPtr = *fromPtr;

      toPtr--;
      fromPtr--;
    } while (fromPtr != endPtr);
        
    do {

      *toPtr = 0x20;

      toPtr--;
    } while (toPtr != endPtr);

    vdp_setWriteAddress(_vdpPatternNameTableAddr + (topRow * _vdpCursorMaxXFull));
    uint8_t *v = _vdp_textBuffer + (topRow * _vdpCursorMaxXFull);
    uint8_t *e = _vdp_textBuffer + ((bottomRow + 1) * _vdpCursorMaxXFull);

    do {

      vdp_put(*v);

      v++;
    } while (v != e);
  }

  void vdp_writeUInt8ToBinary(uint8_t v) __z88dk_fastcall {

    uint8_t str[9];

    for (uint8_t i = 0; i < 8; i++)
      if (v >> i & 1)
        str[7 - i] = '1';
      else
        str[7 - i] = '0';

    str[8] = 0x00;

    vdp_print(str);
  }

  void vdp_writeUInt16ToBinary(uint16_t v) __z88dk_fastcall {

    uint8_t str[17];

    for (uint8_t i = 0; i < 16; i++)
      if (v >> i & 1)
        str[15 - i] = '1';
      else
        str[15 - i] = '0';

    str[16] = 0x00;

    vdp_print(str);
  }

  void vdp_writeUInt32ToBinary(uint32_t v) __z88dk_fastcall {

    uint8_t str[33];

    for (uint8_t i = 0; i < 32; i++)
      if (v >> i & 1)
        str[31 - i] = '1';
      else
        str[31 - i] = '0';

    str[32] = 0x00;

    vdp_print(str);
  }

  //https://github.com/jblang/TMS9918A/blob/111ce0ba7ff446ac3b2fbaa81d41a741074e431d/examples/tms.asm#L213
  //Limits all ram out operations to at least 8uS.  We could be more efficient on the VDP Blank,
  //but if the code overshoots the vsync timings, we would end up in trouble.  This approach while a bit
  //slower, avoids that risk altogether.
  void vdp_put(uint8_t c) __z88dk_fastcall {
    IO_VDPDATA = c;
    __asm
      // push    bc                      ; 11  | 11
      ld      b, 2                    ; 2 iteration
    TmsRamOutDelay:
      djnz    TmsRamOutDelay          ; 8   | 7  plus (13 | 9) * (iterations-1)
      // pop     bc                      ; 10  | 9
    __endasm;
  }
#endif
