#include "file_tape_device.h"
#include "sorting_algorithm.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " <input_file> <output_file>"
              << std::endl;
    return 1;
  }

  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  FileTapeDevice inputTape(inputFilename);
  FileTapeDevice outputTape(outputFilename);

  SortingAlgorithm algorithm("config.txt");
  algorithm.sort(inputTape, outputTape);

  std::cout << "Sorting complete!" << std::endl;

  return 0;
}
