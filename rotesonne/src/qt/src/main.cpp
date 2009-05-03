#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>

using namespace boost::filesystem;
using namespace std;

bool find_file(const path & dir_path) {

  if (!exists(dir_path)) {
    return false;
  }

  directory_iterator end_itr;
  for (directory_iterator itr(dir_path); itr != end_itr; ++itr) {
    if (is_directory(itr->status())) {
      if (find_file(itr->path())) {

      }
    } else if (/*itr->leaf() == file_name*/true) {
      cout << itr->path() << endl;
    }
  }
  return false;
}

int main() {

  string path = "/data/music";
  find_file(path);

  return 0;
}
