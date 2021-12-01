//This program is used to inspect buttons in the process of manufacturing and to
//find and display defective buttons. The picture uses infrared imaging and a
//ppm format in order to be able to manually inspect each button's pixels recursively
//to find a pixel count, which will then be compared to an average with tolerance.

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

class pixel_class {
private:
  int red, green, blue;
  bool exclude;  // if true, do not check this pixel
public:
  void loaddata(int v1, int v2, int v3);
  void datatofile(fstream & ppmfile);
  int getR() { return red; }
  int getG() { return green; }
  int getB() { return blue; }
  void setexclude(bool ex) { exclude = ex; }
  bool getexclude() { return exclude; }
};

void loadButtons();
void findPixelValues();
void findMinMax(int x, int y);
void findPixelTolerance();
void resetExclude();
void findDefects();
void drawBox(int i);
void changesToPicture();

int total, xmin, xmax, ymin, ymax;
int screenx, screeny, maxcolours;
pixel_class picture[600][600];
int pixel_tolerance = 0;
int pixel_amount = 0;
int button_rgb_value = 128;
vector<int> button_pixels;

int main() {
  // Step 1 : read in the image from Buttons.ppm
  loadButtons();

  // Step 2 : identify buttons and draw boxes
  findPixelValues();
  resetExclude();
    cout << "Finding defects...\n";
    cout << "Displaying defects...\n";
  findDefects();
  // Step 3 : output the final .ppm file
  changesToPicture();
  cout << "Finished!\n";
}

void loadButtons() {
  // load the picture from Buttons.ppm
  int x, y, R, G, B;
  fstream infile;
  string infilename, line;
  infilename = "Buttons.ppm";
  infile.open(infilename.c_str(), fstream::in);
  if (infile.is_open() == false) {
    cout << "ERROR: not able to open " << infilename << endl;
    exit(2);
  }
  getline(infile, line);  // line for standard "P3" format for ppm
  getline(infile, line);  // line for the filename
  infile >> screenx >> screeny;  // line for the size of the picture
  infile >> maxcolours;  // line for the light levels, 256 being RGB which we're using
  for (y = 0; y < screeny; y++) {
    for (x = 0; x < screenx; x++) {
      infile >> R >> G >> B;
      picture[x][y].loaddata(R, G, B);
      picture[x][y].setexclude(false);
    }
  }
  infile.close();
  cout << "Picture loaded successfully.\n";
}

void findPixelValues() {
  int x, y;
  cout << "Finding necessary values...\n";
  for (y=0; y < screeny; y++) {
    for (x=0; x < screenx; x++) {
      if (picture[x][y].getR() > button_rgb_value &&
      picture[x][y].getexclude() == false) {
        xmin = x; ymin = y; xmax = x; ymax = y;
        findMinMax(x, y);
        findPixelTolerance();
        button_pixels.push_back(pixel_amount);
        pixel_amount = 0;
      }
    }
  }
  cout << "Buttons detected: " << button_pixels.size() << ".\n";
}

void findMinMax(int x, int y) {
  if (picture[x][y].getR() <= button_rgb_value || picture[x][y].getexclude() == true) {
    return;
  }
  else {
    picture[x][y].setexclude(true);
    if (x > xmax) {
      xmax = x;
    }
    if (x < xmin) {
      xmin = x;
    }
    if (y > ymax) {
      ymax = y;
    }
    if (y < ymin) {
      ymin = y;
    }
    findMinMax(x+1, y);
    findMinMax(x-1, y);
    findMinMax(x, y+1);
    findMinMax(x, y-1);
  }
}

void findPixelTolerance() {
  // After fine tuning, I found that 510 was the ideal tolerance for buttons.
  // I used a tolerance in case buttons vary in size or hole shape.
  // The tolerance could've been calculated with some sort of multilinear
  // regression model where the average is taken into consideration, but the
  // sample size is too small for this.
  for (int y = ymin; y <= ymax; y++) {
    for (int x = xmin; x <= xmax; x++) {
      if (picture[x][y].getR() > button_rgb_value) {
        pixel_amount++;
        if (pixel_amount - 510 > pixel_tolerance) {
          pixel_tolerance = pixel_amount - 510;
        }
      }
    }
  }
}

void resetExclude() {
  int x = 0;
  int y = 0;

  for (y=0; y < screeny; y++) {
    for (x=0; x < screenx; x++) {
      picture[x][y].setexclude(false);
    }
  }
}

void findDefects() {
  int x, y;
  int i=0;
  for (y=0; y < screeny; y++) {
    for (x=0; x < screenx; x++) {
      if (picture[x][y].getR() > button_rgb_value &&
      picture[x][y].getexclude() == false) {
        xmin = x; ymin = y; xmax = x; ymax = y;
        findMinMax(x, y);
        drawBox(i);
        i++;
      }
    }
  }
}

void drawBox(int i) {
  if (button_pixels[i] >= pixel_tolerance) {
    int x = xmin; int y = ymin;
    while (x <= xmax) {
      picture[x][y].loaddata(0, 255, 0);
      picture[x][y].setexclude(true);
      x++;
    }
    x = xmin;
    while (y <= ymax) {
      picture[x][y].loaddata(0, 255, 0);
      picture[x][y].setexclude(true);
      y++;
    }
    y = ymax;
    while (x <= xmax) {
      picture[x][y].loaddata(0, 255, 0);
      picture[x][y].setexclude(true);
      x++;
    }
    while (y >= ymin) {
      picture[x][y].loaddata(0, 255, 0);
      picture[x][y].setexclude(true);
      y--;
    }
  }
  else if (button_pixels[i] < pixel_tolerance) {
    int x = xmin; int y = ymin;
    while (x <= xmax) {
      picture[x][y].loaddata(255, 0, 0);
      picture[x][y].setexclude(true);
      x++;
    }
    x = xmin;
    while (y <= ymax) {
      picture[x][y].loaddata(255, 0, 0);
      picture[x][y].setexclude(true);
      y++;
    }
    y = ymax;
    while (x <= xmax) {
      picture[x][y].loaddata(255, 0, 0);
      picture[x][y].setexclude(true);
      x++;
    }
    x = xmax; y = ymax;
    while (y >= ymin) {
      picture[x][y].loaddata(255, 0, 0);
      picture[x][y].setexclude(true);
      y--;
    }
  }

}

void changesToPicture() {
  fstream buttons_fs;
  string buttons_file = "Buttonsnew.ppm";
  cout << "Saving to file " << buttons_file << "... \n";
  buttons_fs.open(buttons_file.c_str(), fstream::out);
  buttons_fs << "P3\n";
 buttons_fs << "# " << buttons_file << endl;
 buttons_fs << screenx << " " << screeny << endl;
 buttons_fs << maxcolours << endl;
  for (int y = 0; y < screeny; y++) {
    for (int x = 0; x < screenx; x++) {
      picture[x][y].datatofile(buttons_fs);
    }
  }
  buttons_fs.close();
}



//--------------- methods for the pixel_class ------------
void pixel_class::loaddata(int v1, int v2, int v3) {
  red = v1;
  green = v2;
  blue = v3;
}

void pixel_class::datatofile(fstream & ppmfile) {
  // write the data for one pixel to the ppm file
  ppmfile << red << " " << green;
  ppmfile << " " << blue << "  ";
}
