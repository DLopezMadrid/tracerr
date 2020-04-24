//
// Created by dani on 21/04/2020.
//

#include "File.h"
File::File(const std::string &fname) {
  try {
    file_ = fopen(fname.c_str(), "wb");
  } catch (...) {
  }
}
void File::Write(void *data, int const &size) {
  fwrite(data, 1, size, file_);
}

File::~File() {
  if (file_ != nullptr) {
    fclose(file_);
  }
}

