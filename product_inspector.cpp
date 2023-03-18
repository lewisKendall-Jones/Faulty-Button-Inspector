// Created by Lewis Kendall Jones.
// lewiskendalljones@gmail.com

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

class pixel_class {
private:
  int red, green, blue;
  bool exclude;  // flag to avoid reading the same pixel twice
public:
  void loadData(int v1, int v2, int v3);
  void dataToFile(fstream & ppmfile);
  int getR() { return red; }
  int getG() { return green; }
  int getB() { return blue; }
  void setExclude(bool ex) { exclude = ex; }
  bool getExclude() { return exclude; }
};

void loadProducts();
void findPixelValues();
void findMinMax(int x, int y);
int countPixels(int pixel_amount);
void resetExclude();
void findDefects();
void drawBox(int i);
void exportResults();

int total, xmin, xmax, ymin, ymax;
int screenx, screeny, maxcolours;
pixel_class picture[600][600];
int highest_pixel = 0;
int tolerance = 510; //Adjust to the product as necessary.
int product_rgb_value = 128; // The minimum brightness for a pixel to be recognised as part of a product
vector<int> product_pixels;

int main() {
  // Step 1 : read in the image from products.ppm
  loadProducts();

  // Step 2 : identify products and draw boxes
  findPixelValues();
  resetExclude();
    cout << "Finding and displaying defects...\n";
  findDefects();
  // Step 3 : output the final .ppm file
  exportResults();
  cout << "Finished!\n";
  cout << "Press enter to exit.";
  cin.get();
}

void loadProducts() {
  // load the picture from products.ppm
  int x, y, R, G, B;
  fstream infile;
  string infilename, line;
  infilename = "products.ppm";
  infile.open(infilename.c_str(), fstream::in);
  if (infile.is_open() == false) {
    cout << "ERROR: not able to open " << infilename << endl;
    exit(2);
  }
  getline(infile, line);  // "P3", the standard format for PPM
  getline(infile, line);  // filename
  infile >> screenx >> screeny;  // picture resolution
  infile >> maxcolours;  // light levels, 256 being rgb
  for (y = 0; y < screeny; y++) {
    for (x = 0; x < screenx; x++) {
      infile >> R >> G >> B;
      picture[x][y].loadData(R, G, B);
      picture[x][y].setExclude(false);
    }
  }
  infile.close();
  cout << "Picture loaded successfully.\n";
}

//Counts the total pixels per product and stores to the product_pixels vector
void findPixelValues() {
  int x, y;
  int pixel_amount = 0;
  cout << "Finding necessary values...\n";
  for (y=0; y < screeny; y++) {
    for (x=0; x < screenx; x++) {
      if (picture[x][y].getR() > product_rgb_value && picture[x][y].getExclude() == false) {
        xmin = x; ymin = y; xmax = x; ymax = y;
        findMinMax(x, y);
        pixel_amount = countPixels(pixel_amount);
        if (pixel_amount - tolerance > highest_pixel) {
            highest_pixel = pixel_amount - tolerance;
        }
        product_pixels.push_back(pixel_amount);
        pixel_amount = 0;
      }
    }
  }
  cout << "Products detected: " << product_pixels.size() << ".\n";
}

// Finds the top left and bottom right corners of products for highlighting
void findMinMax(int x, int y) {
  if (picture[x][y].getR() <= product_rgb_value || picture[x][y].getExclude() == true) {
    return;
  }
  else {
    picture[x][y].setExclude(true);
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

int countPixels(int pixel_amount) {
  for (int y = ymin; y <= ymax; y++) {
    for (int x = xmin; x <= xmax; x++) {
      if (picture[x][y].getR() > product_rgb_value) {
        pixel_amount++;
      }
    }
  }
  return pixel_amount;
}

void resetExclude() {
  int x = 0;
  int y = 0;

  for (y=0; y < screeny; y++) {
    for (x=0; x < screenx; x++) {
      picture[x][y].setExclude(false);
    }
  }
}

void findDefects() {
  int x, y;
  int i=0;
  for (y=0; y < screeny; y++) {
    for (x=0; x < screenx; x++) {
      if (picture[x][y].getR() > product_rgb_value && picture[x][y].getExclude() == false) {
        xmin = x; ymin = y; xmax = x; ymax = y;
        findMinMax(x, y); // sets excludes ('if' statement will only run once per product)
        drawBox(i);
        i++;
      }
    }
  }
}

void drawBox(int i) {
  if (product_pixels[i] >= highest_pixel) {
    int x = xmin; int y = ymin;
    while (x <= xmax) {
      picture[x][y].loadData(0, 255, 0);
      picture[x][y].setExclude(true);
      x++;
    }
    x = xmin;
    while (y <= ymax) {
      picture[x][y].loadData(0, 255, 0);
      picture[x][y].setExclude(true);
      y++;
    }
    y = ymax;
    while (x <= xmax) {
      picture[x][y].loadData(0, 255, 0);
      picture[x][y].setExclude(true);
      x++;
    }
    while (y >= ymin) {
      picture[x][y].loadData(0, 255, 0);
      picture[x][y].setExclude(true);
      y--;
    }
  }
  else if (product_pixels[i] < highest_pixel) {
    int x = xmin; int y = ymin;
    while (x <= xmax) {
      picture[x][y].loadData(255, 0, 0);
      picture[x][y].setExclude(true);
      x++;
    }
    x = xmin;
    while (y <= ymax) {
      picture[x][y].loadData(255, 0, 0);
      picture[x][y].setExclude(true);
      y++;
    }
    y = ymax;
    while (x <= xmax) {
      picture[x][y].loadData(255, 0, 0);
      picture[x][y].setExclude(true);
      x++;
    }
    x = xmax; y = ymax;
    while (y >= ymin) {
      picture[x][y].loadData(255, 0, 0);
      picture[x][y].setExclude(true);
      y--;
    }
  }

}

void exportResults() {
  fstream products_fs;
  string products_file = "inspection_results.ppm";
  cout << "Saving to file " << products_file << "... \n";
  products_fs.open(products_file.c_str(), fstream::out);
  products_fs << "P3\n";
 products_fs << "# " << products_file << endl;
 products_fs << screenx << " " << screeny << endl;
 products_fs << maxcolours << endl;
  for (int y = 0; y < screeny; y++) {
    for (int x = 0; x < screenx; x++) {
      picture[x][y].dataToFile(products_fs);
    }
  }
  products_fs.close();
}


void pixel_class::loadData(int v1, int v2, int v3) {
  red = v1;
  green = v2;
  blue = v3;
}

void pixel_class::dataToFile(fstream & ppmfile) {
  // write the data for one pixel to the ppm file
  ppmfile << red << " " << green;
  ppmfile << " " << blue << "  ";
}