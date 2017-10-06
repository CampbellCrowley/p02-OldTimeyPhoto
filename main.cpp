#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include "CampbellLib/CampbellLib.h"
#include "bitmap.h"

using namespace std;

// Returns num after rectifying between min and max.
float Rectify(float num, float min, float max) {
  return ((num < min ? min : (num > max ? max : num)));
}

// Check if file will be able to save to specified path.
bool isValidOutPath(string filename) {
  std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);

  if (file.fail()) {
    cerr << Campbell::Color::red << filename
         << " could not be opened for editing. Is it already open by another "
            "program or is it read-only?\n"
         << Campbell::Color::reset;
    file.close();
    return false;
  } else {
    file.close();
    return true;
  }
}

// The main program to be looped if necessary.
bool Main(string inPath, string outPath, bool moreOpts = true) {
  Bitmap bmp;
  // Determine file to load.
  if (inPath.length() < 1) {
    cout << "Please enter an image name to load: ";
    getline(cin, inPath);
  }

  // Attempt to open file. If it fails, allow user to choose new file.
  bmp.open(inPath);
  if (!bmp.isImage()) {
    cerr << Campbell::Color::red
         << "Invalid image.\nEither the file provided wasn't a valid bitmap "
            "(24bit depth Windows BMP), or the path was incorrect.\n"
         << Campbell::Color::reset;
    return true;
  }

  // Convert image to 2D vector of Pixels.
  vector<vector<Pixel>> pixels = bmp.toPixelMatrix();

  // Let user pick options of moreOpts are enabled.
  bool makeBW = true;
  if (moreOpts) {
    cout << "Would you like to make the image black and white? (Y/n): ";
    makeBW = Campbell::Strings::getYesNo();
  }
  bool addNoise = false;
  if (moreOpts) {
    cout << "Would you like to add film grain? (y/N): ";
    addNoise = Campbell::Strings::getYesNo(false);
  }
  float borderWidth = 0;
  if (moreOpts) {
    cout << "Would you like to add a border? (y/N): ";
    if (Campbell::Strings::getYesNo(false)) {
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
  bool invertColors = false;
  if (moreOpts) {
    cout << "Would you like to invert all colors? (y/N): ";
    invertColors = Campbell::Strings::getYesNo(false);
  }

  // Process image.
  for (int i = 0; i < (int)pixels.size(); i++) {
    for (int j = 0; j < (int)pixels[i].size(); j++) {
      if (invertColors) {
        pixels[i][j] = Pixel(255 - pixels[i][j].red, 255 - pixels[i][j].green,
                             255 - pixels[i][j].blue);
      }
      if (makeBW) {
        float newValue =
            (pixels[i][j].red + pixels[i][j].green + pixels[i][j].blue) / 3;
        if (addNoise) {
          newValue += rand() % 50 - 25;
        }
        newValue = Rectify(newValue, 0, 255);
        pixels[i][j] = Pixel(newValue, newValue, newValue);
      } else if (addNoise) {
        float newRed = Rectify(pixels[i][j].red += rand() % 50 - 25, 0, 255);
        float newGreen =
            Rectify(pixels[i][j].green += rand() % 50 - 25, 0, 255);
        float newBlue = Rectify(pixels[i][j].blue += rand() % 50 - 25, 0, 255);
        pixels[i][j] = Pixel(newRed, newGreen, newBlue);
      } else if (!invertColors) {
        i = pixels.size();
        break;
      }
    }
  }

  // Add Border
  for (int i = 0; i < borderWidth; i++) {
    // Half the border is white, then black.
    int color = (i < borderWidth / 2) ? 0 : 255;
    pixels.insert(pixels.begin(),
                  vector<Pixel>(pixels[0].size(), {color, color, color}));
    pixels.push_back(vector<Pixel>(pixels[0].size(), {color, color, color}));
    for (int j = 0; j < (int)pixels.size(); j++) {
      pixels[j].insert(pixels[j].begin(), Pixel(color, color, color));
      pixels[j].push_back(Pixel(color, color, color));
    }
  }

  // TODO: Determine if this fails.
  bmp.fromPixelMatrix(pixels);

  // Determine output location.
  while (true) {
    if (outPath.length() < 1) {
      cout << "Please enter the place to save the image (nothing for "
              "./oldtimey.bmp): ";
      getline(cin, outPath);
    }
    if (outPath.length() < 1) {
      outPath = "oldtimey.bmp";
    }
    if (!isValidOutPath(outPath)) {
      outPath = "";
    } else {
      break;
    }
  }

  // Save file.
  bmp.save(outPath);

  cout << Campbell::Color::green << "File saved to " << outPath << endl
       << Campbell::Color::reset;

  if (moreOpts) {
    cout << "Would you like to edit another photo? (Y/n): ";
    return Campbell::Strings::getYesNo();
  }

  return false;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  string inPath = "";
  string outPath = "";
  bool moreOpts = true;
  // Read arguments passed in from the command line.
  if (argc >= 2) {
    inPath = argv[1];
  }
  if (argc >= 3) {
    outPath = argv[2];
  }
  if (argc >= 4) {
    moreOpts = string(argv[3]) != "false" && string(argv[3]) != "0";
  }
  if (argc > 4) {
    cerr << Campbell::Color::red << "Too many arguments.\n"
         << Campbell::Color::reset
         << "First argument is file to read, second is file to write to, and "
            "third is true or false whether to prompt for more options.\n";
    inPath = "";
    outPath = "";
    moreOpts = true;
  }

  // Loop the main program until the user decides to exit.
  while (Main(inPath, outPath, moreOpts)) {
    inPath = "";
    outPath = "";
    moreOpts = true;
  }
  return 0;
}
