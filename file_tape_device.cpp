#include "file_tape_device.h"
#include <cstdint>

FileTapeDevice::FileTapeDevice(const std::string &filename)
    : filename(filename), eof(false) {
  file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
}

bool FileTapeDevice::read(uint32_t &data) {
  if (file) {
    file.read(reinterpret_cast<char *>(&data), sizeof(data));
    if (file.eof()) {
      eof = true;
      return false;
    }
    return true;
  }
  return false;
}

bool FileTapeDevice::write(uint32_t data) {
  if (file) {
    file.write(reinterpret_cast<const char *>(&data), sizeof(data));
    return true;
  }
  return false;
}

bool FileTapeDevice::shift() {
  if (file) {
    file.seekg(sizeof(uint32_t), std::ios::cur);
    return true;
  }
  return false;
}

bool FileTapeDevice::endOfFile() const { return eof; }
