# Product Inspector
This program is intended for the usage of quality control in manufacturing.

Using image processing with .ppm formatting, this program searches for defects in products, such as buttons, 
by comparing the highest pixel count of all products to the pixel count of each product. The pixel counts of 
each product are acquired using recursion. Products are highlighted in green if satisfactory and red if 
exceeding the tolerance of variance compared to the highest pixel count.

## Criteria:
- All products for inspection must be in the same .ppm file
- Products must not be overlapping or touching
- Defects must cause a loss in pixels (chipping, imprecise drilling, missing parts etc.)
- Defects must not cause a gain in pixels (incomplete drilling, mold overpour, melting, etc.)
- The .ppm file must be formatted to P3
- The image must be in black and white

## Usage:
Compile using gcc and run. products.ppm must be present for the program to work successfully.

gcc command: g++ -o "Product Inspector.exe" product_inspector.cpp

Alternatively, use the .exe provided (unrecommended, not tested for all platforms).

## Author
Created by Lewis Kendall-Jones

lewiskendalljones@gmail.com

