// ****************************************************************************************
// NABU-LIB C Library
// DJ Sures (c) 2023
// https://nabu.ca
// 
// Read the NABU-LIB.h file for details of each function in this file.
// 
// *Note: Some TMS9918 graphic functions were from: https://github.com/michalin/TMS9918_Arduino/tree/main/examples
//        Modified by DJ Sures 2022 for NABU PC
//
// **********************************************************************************************



uint8_t _rnFS_INT_BACKUP = 0;


// **************************************************************************
// INTERRUPTLESS HCCA COMMANDS
// ---------------------------
// These are HCCA commands that do not require the interrupt. The interrupt
// will be disabled while these commands are executed. This is because they
// will focus entirely on hcca data transfer for optimal speed. Interrupts
// use too many cycles for large amounts of data.
// **************************************************************************
void hcca_DiFocusInterrupts() {
  ;
}

void hcca_DiRestoreInterrupts() {
  ;
}

void hcca_DiWriteByte(uint8_t c) {
  (void)c;
}

void hcca_DiWriteUInt32(uint32_t val) {
  (void)val;
}

void hcca_DiWriteInt32(int32_t val) {
  (void)val;
}

void hcca_DiWriteUInt16(uint16_t val) {
  (void)val;
}

void hcca_DiWriteInt16(int16_t val) {
  (void)val;
}

void hcca_DiWriteString(uint8_t *str) {
  (void)str;
}

void hcca_DiWriteBytes(uint16_t offset, uint16_t length, uint8_t *bytes) {
  (void)offset;
  (void)length;
  (void)bytes;
}

// -----------------------------------------------------------------------------------

inline uint8_t hcca_DiReadByte() {
  return 0;
}

uint16_t hcca_DiReadUInt16() {
  return 0;
}

int16_t hcca_DiReadInt16() {
  return 0;
}

uint32_t hcca_DiReadUInt32() {
  return 0;
}

int32_t hcca_DiReadInt32() {
  return 0;
}

void hcca_DiReadBytes(uint16_t offset, uint16_t bufferLen, uint8_t *buffer) {
  (void)offset;
  (void)bufferLen;
  (void)buffer;
}

// **************************************************************************
// FILE & DIRECTORIES
// ------------------
//
// These functions are for reading and creating files and directories.
// **************************************************************************

uint8_t rn_fileOpen(uint8_t filenameLen, uint8_t* filename, uint16_t fileFlag, uint8_t fileHandle) {
  (void)filenameLen;
  (void)filename;
  (void)fileFlag;
  (void)fileHandle;
  return 0;
}

void rn_fileHandleClose(uint8_t fileHandle) {
  (void)fileHandle;
}

int32_t rn_fileSize(uint8_t filenameLen, uint8_t* filename) {
  (void)filenameLen;
  (void)filename;
  return 0;
}

int32_t rn_fileHandleSize(uint8_t fileHandle) {
  (void)fileHandle;
  return 0;
}

uint16_t rn_fileHandleRead(uint8_t fileHandle, uint8_t* buffer, uint16_t bufferOffset, uint32_t readOffset, uint16_t readLength) {
  (void)fileHandle;
  (void)buffer;
  (void)bufferOffset;
  (void)readOffset;
  (void)readLength;
  return 0;
}

void rn_fileHandleAppend(uint8_t fileHandle, uint16_t dataOffset, uint16_t dataLen, uint8_t* data) {
  (void)fileHandle;
  (void)dataOffset;
  (void)dataLen;
  (void)data;
}

void rn_fileHandleInsert(uint8_t fileHandle, uint32_t fileOffset, uint16_t dataOffset, uint16_t dataLen, uint8_t* data) {
  (void)fileHandle;
  (void)fileOffset;
  (void)dataOffset;
  (void)dataLen;
  (void)data;
}

void rn_fileHandleDeleteRange(uint8_t fileHandle, uint32_t fileOffset, uint16_t deleteLen) {
  (void)fileHandle;
  (void)fileOffset;
  (void)deleteLen;
}

void rn_fileHandleEmptyFile(uint8_t fileHandle) {
  (void)fileHandle;
}

void rn_fileHandleReplace(uint8_t fileHandle, uint32_t fileOffset, uint16_t dataOffset, uint16_t dataLen, uint8_t* data) {
  (void)fileHandle;
  (void)fileOffset;
  (void)dataOffset;
  (void)dataLen;
  (void)data;
}

void rn_fileDelete(uint8_t filenameLen, uint8_t* filename) {
  (void)filenameLen;
  (void)filename;
}

void rn_fileHandleCopy(uint8_t srcFilenameLen, uint8_t* srcFilename, uint8_t destFilenameLen, uint8_t* destFilename, uint8_t copyMoveFlag) {
  (void)srcFilenameLen;
  (void)srcFilename;
  (void)destFilenameLen;
  (void)destFilename;
  (void)copyMoveFlag;
}

void rn_fileHandleMove(uint8_t srcFilenameLen, uint8_t* srcFilename, uint8_t destFilenameLen, uint8_t* destFilename, uint8_t copyMoveFlag) {
  (void)srcFilenameLen;
  (void)srcFilename;
  (void)destFilenameLen;
  (void)destFilename;
  (void)copyMoveFlag;
}

uint16_t rn_fileList(uint8_t pathLen, uint8_t* path, uint8_t wildcardLen, uint8_t* wildcard, uint8_t fileListFlags) {
  (void)pathLen;
  (void)path;
  (void)wildcardLen;
  (void)wildcard;
  (void)fileListFlags;
  return 0;
}

void rn_fileListItem(uint16_t fileItemIndex, FileDetailsStruct* s) {
  (void)fileItemIndex;
  (void)s;
}

void rn_fileDetails(uint8_t filenameLen, uint8_t* filename, FileDetailsStruct* s) {
  (void)filenameLen;
  (void)filename;
  (void)s;
}

void rn_fileHandleDetails(int8_t fileHandle, FileDetailsStruct* s) {
  (void)fileHandle;
  (void)s;
}

uint16_t rn_fileHandleReadSeq(uint8_t fileHandle, uint8_t* buffer, uint16_t bufferOffset, uint16_t readLength) {
  (void)fileHandle;
  (void)buffer;
  (void)bufferOffset;
  (void)readLength;
  return 0;
}

int32_t rn_fileHandleSeek(uint8_t fileHandle, int32_t offset, uint8_t seekOption) {
  (void)fileHandle;
  (void)offset;
  (void)seekOption;
  return 0;
}

// **************************************************************************
// TCP/IP CLIENT
// -------------
//
// These functions are for communicating with TCP server ports
// **************************************************************************

uint8_t rn_TCPOpen(uint8_t hostnameLen, uint8_t* hostname, uint16_t port, uint8_t fileHandle) {
  (void)hostnameLen;
  (void)hostname;
  (void)port;
  (void)fileHandle;
  return 0;
}

void rn_TCPHandleClose(uint8_t fileHandle) {
  (void)fileHandle;
}

int32_t rn_TCPHandleSize(uint8_t fileHandle) {
  (void)fileHandle;
  return 0;
}

int32_t rn_TCPHandleRead(uint8_t tcpHandle, uint8_t* buffer, uint16_t bufferOffset, uint16_t readLength) {
  (void)tcpHandle;
  (void)buffer;
  (void)bufferOffset;
  (void)readLength;
  return 0;
}

int32_t rn_TCPHandleWrite(uint8_t tcpHandle, uint16_t dataOffset, uint16_t dataLen, uint8_t* data) {
  (void)tcpHandle;
  (void)dataOffset;
  (void)dataLen;
  (void)data;
  return 0;
}

// **************************************************************************
// PRINTER
// -------
//
// These functions are for printing to a file on the host PC
// **************************************************************************
void rn_Printer(uint8_t c) {
  (void)c;
}

void rn_PunchOut(uint8_t c) {
  (void)c;
}
