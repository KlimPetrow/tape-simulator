#include "sorting_algorithm.h"
#include "tape_device.h"
#include "file_tape_device.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>

SortingAlgorithm::SortingAlgorithm(const std::string &configFilename) {
  std::ifstream configFile(configFilename);
  if (configFile) {
    configFile >> memoryLimit >> readWriteDelay >> rewindDelay >> shiftDelay;
    configFile.close();
  } else {
    memoryLimit = 0;
    readWriteDelay = 0;
    rewindDelay = 0;
    shiftDelay = 0;
  }
}

void SortingAlgorithm::sort(TapeDevice &inputTape, TapeDevice &outputTape) {
  createTemporaryTapes(2);
  mergeSort(inputTape, outputTape, 0, temporaryTapes.size() - 1);
  deleteTemporaryTapes();
}

void SortingAlgorithm::createTemporaryTapes(unsigned int count) {
  temporaryTapes.clear();
  for (unsigned int i = 0; i < count; i++) {
    std::string tempFileName = "tmp/temp_tape" + std::to_string(i) + ".dat";
    temporaryTapes.push_back(tempFileName);
    std::ofstream tempFile(tempFileName, std::ios::binary);
    tempFile.close();
  }
}

void SortingAlgorithm::deleteTemporaryTapes() {
  for (const auto &tempFilename : temporaryTapes) {
    std::remove(tempFilename.c_str());
  }
  temporaryTapes.clear();
}

void SortingAlgorithm::mergeSort(TapeDevice &inputTape, TapeDevice &outputTape,
                                 unsigned int start, unsigned int end) {
  if (start < end) {
    unsigned int mid = (start + end) / 2;
    mergeSort(inputTape, outputTape, start, mid);
    mergeSort(inputTape, outputTape, mid + 1, end);
    mergePass(inputTape, outputTape, start, end);
  }
}

void SortingAlgorithm::merge(TapeDevice &inputTape1, TapeDevice &inputTape2,
                             TapeDevice &outputTape) {
  uint32_t data1, data2;
  bool eof1 = inputTape1.endOfFile();
  bool eof2 = inputTape2.endOfFile();

  while (!eof1 || !eof2) {
    if (eof1) {
      while (!eof2) {
        inputTape2.read(data2);
        outputTape.write(data2);
      }
      break;
    }
    if (eof2) {
      while (!eof1) {
        inputTape1.read(data1);
        outputTape.write(data1);
      }
      break;
    }
    inputTape1.read(data1);
    inputTape2.read(data2);

    if (data1 <= data2) {
      outputTape.write(data1);
      inputTape2.shift();
    } else {
      outputTape.write(data2);
      inputTape1.shift();
    }
    eof1 = inputTape1.endOfFile();
    eof2 = inputTape2.endOfFile();
  }
}

void SortingAlgorithm::mergePass(TapeDevice &inputTape, TapeDevice &outputTape,
                                 unsigned int start, unsigned int end) {
  unsigned int mid = (start + end) / 2;
  if (start == end) {
    inputTape.rewind();
    outputTape.rewind();
    unsigned int count = 0;

    while (!inputTape.endOfFile()) {
      uint32_t data;
      inputTape.read(data);
      outputTape.write(data);
      count++;
      if (count % memoryLimit == 0) {
        outputTape.rewind();
        outputTape.shift();
      }
    }
  } else {
    mergePass(inputTape, outputTape, start, mid);
    mergePass(inputTape, outputTape, mid + 1, end);
    inputTape.rewind();
    outputTape.rewind();

    for (unsigned int i = start; i <= end; i++) {
      if (i == mid) {
        inputTape.rewind();
        outputTape.rewind();
      }

      TapeDevice *inputTape1;
      TapeDevice *inputTape2;

      if (i < mid) {
        inputTape1 = new FileTapeDevice(temporaryTapes[i]);
        inputTape2 = new FileTapeDevice(temporaryTapes[i + 1]);
      } else {
        inputTape1 = new FileTapeDevice(temporaryTapes[end - (i - mid)]);
        inputTape2 = new FileTapeDevice(temporaryTapes[end - (i - mid) + 1]);
      }

      merge(*inputTape1, *inputTape2, outputTape);

      delete inputTape1;
      delete inputTape2;

      outputTape.rewind();
      outputTape.shift();
    }
  }
}
