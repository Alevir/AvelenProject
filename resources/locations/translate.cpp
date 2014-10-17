#include <fstream>

int main(int argc, char** argv) {
  
  char buffer[1];
  std::ifstream input;
  std::ofstream output;
  input.open(argv[1]);
  output.open(argv[2], std::ofstream::trunc | std::ofstream::out);

  do {
    input.read(buffer, sizeof(buffer));
    if (input.good()) {
      if(buffer[0] == '@') buffer[0] = '\2';
      if(buffer[0] == '$') buffer[0] = '\1';
      output.write(buffer, sizeof(buffer));
    }
  } while (! input.eof());

  input.close();
  output.close();
  
}
