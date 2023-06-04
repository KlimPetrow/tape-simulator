#ifndef FILE_TAPE_DEVICE_H
#define FILE_TAPE_DEVICE_H

#include "tape_device.h"
#include <cstdint>
#include <fstream>
#include <string>
#include <sys/types.h>

class FileTapeDevice : public TapeDevice {
public:
  explicit FileTapeDevice(const std::string &filename);
  ~FileTapeDevice();

  bool read(uint32_t &data) override;
  bool write(uint32_t data) override;
  bool rewind() override;
  bool shift() override;
  bool endOfFile() const override;

private:
  std::fstream file;
  std::string filename;
  bool eof;
};

#endif // FILE_TAPE_DEVICE_H
