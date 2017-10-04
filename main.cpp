#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "bitmap.h"
#include "CampbellLib/CampbellLib.h"

using namespace std;

float Rectify(float num, float min, float max) {
  return ((num < min ? min : (num > max ? max : num)));
}
bool Main(string inPath, string outPath, bool moreOpts = true) {
  Bitmap bmp;
  if (inPath.length() < 1) {
    cout << "Please enter an image name:\n";
    getline(cin, inPath);
  }

  bmp.open(inPath);
  if (!bmp.isImage()) {
    cerr << Campbell::Color::red
         << "Invalid image.\nEither the file provided wasn't a valid bitmap "
            "(24bit depth Windows BMP), or the path was incorrect.\n"
         << Campbell::Color::reset;
    return true;
  }

  vector<vector<Pixel>> pixels = bmp.toPixelMatrix();

  bool addNoise = false;
  if (moreOpts) {
    cout << "Would you like to add film grain? (y/N): ";
    addNoise = Campbell::Strings::getYesNo(false);
  }
  float borderWidth = 0;
  if (moreOpts) {
    cout << "Would you like to add a border? (y/N): ";
    if(Campbell::Strings::getYesNo(false)) {
      while (true) {
        string input;
        cout << "How wide should the border be in pixels? ";
        getline(cin, input);
        if (Campbell::Strings::isNumber(input.c_str())) {
          borderWidth = Campbell::Strings::toNumber(input.c_str());
          break;
        } else {
          cerr << Campbell::Color::red
               << "That's not a valid number. Try 0 for no border.\n"
               << Campbell::Color::reset;
        }
      }
    }
  }

  for (int i = 0; i < (int)pixels.size(); i++) {
    for (int j = 0; j < (int)pixels[i].size(); j++) {
      float newValue =
          (pixels[i][j].red + pixels[i][j].green + pixels[i][j].blue) / 3;
      if (addNoise) {
        newValue += rand() % 50 - 25;
      }
      newValue = Rectify(newValue, 0, 255);
      pixels[i][j] = Pixel(newValue, newValue, newValue);
    }
  }

  for (int i = 0; i < borderWidth; i++) {
    int color = (i < borderWidth / 2) ? 0 : 255;
    pixels.insert(pixels.begin(),
                  vector<Pixel>(pixels[0].size(), {color, color, color}));
    pixels.push_back(vector<Pixel>(pixels[0].size(), {color, color, color}));
    for (int j = 0; j < (int)pixels.size(); j++) {
      pixels[j].insert(pixels[j].begin(), Pixel(color, color, color));
      pixels[j].push_back(Pixel(color, color, color));
    }
  }

  bmp.fromPixelMatrix(pixels);

  if (outPath.length() < 1) {
    cout << "Please enter the place to save the image (nothing for "
            "./oldtimey.bmp): ";
    getline(cin, outPath);
  }

  if (outPath.length() < 1) {
    outPath = "oldtimey.bmp";
  }

  bmp.save(outPath);

  cout << Campbell::Color::green << "File saved to " << outPath << endl
       << Campbell::Color::reset;

  return false;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  string inPath = "";
  string outPath = "";
  bool moreOpts = true;
  if (argc == 2) {
    inPath = argv[1];
  } else if (argc == 3) {
    inPath = argv[1];
    outPath = argv[2];
  } else if (argc == 4) {
    inPath = argv[1];
    outPath = argv[2];
    moreOpts = string(argv[3]) != "false" && string(argv[3]) != "0";
  } else if (argc > 4) {
    cerr << Campbell::Color::red << "Too many arguments.\n"
         << Campbell::Color::reset;
  }

  while (Main(inPath, outPath, moreOpts)) {
    inPath = "";
    outPath = "";
    moreOpts = true;
  }
  return 0;
}
