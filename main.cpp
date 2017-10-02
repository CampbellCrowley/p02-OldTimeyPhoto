#include <iostream>
#include <vector>
#include "../Bitmap/bitmap.h"

using namespace std;

bool Main(string inPath, string outPath) {
  Bitmap bmp;
  if (inPath.length() < 1) {
    cout << "Please enter an image name:\n";
    getline(cin, inPath);
  }

  bmp.open(inPath);
  if (!bmp.isImage()) {
    cerr << "Invalid image.\nEither the file provided wasn't a valid bitmap "
            "(24bit depth Windows BMP), or the path was incorrect.\n";
    return true;
  }

  vector<vector<Pixel>> pixels = bmp.toPixelMatrix();

  for (int i=0; i<pixels.size(); i++) {
    for (int j=0; j<pixels[i].size(); j++) {
      float newValue =
          (pixels[i][j].red + pixels[i][j].green + pixels[i][j].blue) / 3;
      pixels[i][j] = Pixel(newValue, newValue, newValue);
    }
  }

  bmp.fromPixelMatrix(pixels);

  if (outPath.length() < 1) {
    cout << "Please enter the place to save the image (nothing for "
            "./oldtimey.bmp):\n";
    getline(cin, outPath);
  }

  if (outPath.length() < 1) {
    outPath = "oldtimey.bmp";
  }

  bmp.save(outPath);

  cout << "File saved to " << outPath << endl;

  return false;
}

int main(int argc, char *argv[]) {
  string inPath = "";
  string outPath = "";
  if (argc == 2) {
    inPath = argv[1];
  } else if (argc == 3) {
    inPath = argv[1];
    outPath = argv[2];
  } else if (argc > 3) {
    cerr << "Too many arguments.\n";
  }

  while (Main(inPath, outPath)) {
    inPath = "";
    outPath = "";
  }
  return 0;
}
