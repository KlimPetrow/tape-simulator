#ifndef SORTING_ALGORITHM_H
#define SORTING_ALGORITHM_H

#include "tape_device.h"
#include <string>
#include <vector>

class SortingAlorthm {
public:
  explicit SortingAlorthm(const std::string &configFileName);
  void sort(TapeDevice &inputTape, TapeDevice &outputTape);

private:
  std::vector<std::string> temporaryTapes;
  unsigned int memoryLimit;
  unsigned int readWriteDelay;
  unsigned int rewindDelay;
  unsigned int shiftDelay;

  void createTemporaryTapes(unsigned int count);
  void deleteTemporaryTapes();
  void mergeSort(TapeDevice &inputTape, TapeDevice &outputTape,
                 unsigned int start, unsigned int end);
  void merge(TapeDevice &inputTape1, TapeDevice &inputTape2,
             TapeDevice &outputTape);
  void mergePass(TapeDevice &inputTape, TapeDevice &outputTape,
                 unsigned int start, unsigned int end);
};

#endif // !SORTING_ALGORITHM_H
