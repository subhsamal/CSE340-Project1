/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#ifndef __INPUT_BUFFER__H__
#define __INPUT_BUFFER__H__

#include <string>

class InputBuffer {
  public:
    void GetChar(char&);
    char UngetChar(char);
    std::string UngetString(std::string);
    bool EndOfInput();

  private:
    std::vector<char> input_buffer;
};

#endif  //__INPUT_BUFFER__H__
