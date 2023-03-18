# Product Inspector
This program is intended for the use of quality control in manufacturing.

Using infrared imaging and .ppm formatting, this program searches for defects in products, such as buttons, by comparing the average pixel count of all products 
to the pixel count of each product. The pixel counts of each product are acquired using recursion. Products are highlighted in green if satisfactory and red if 
exceeding the tolerance of variance compared to the average.

## Usage:
Compile using gcc and run. products.ppm must be present for the program to work successfully.

gcc command: "g++ -o "Product Inspector.exe" "product_inspector.cpp"


## Author
Created by Lewis Kendall-Jones

lewiskendalljones@gmail.com

