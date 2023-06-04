#ifndef TAPE_DEVICE_H
#define TAPE_DEVICE_H

#include <cstdint>
#include <vector>

class TapeDevice {
public:
  virtual ~TapeDevice() {}

  virtual bool read(uint32_t &data) = 0;
  virtual bool write(uint32_t data) = 0;
  virtual bool rewind() = 0;
  virtual bool shift() = 0;
  virtual bool endOfFile() const = 0;
};

#endif // TAPE_DEVICE_H
