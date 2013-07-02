#include <iostream>
#include "image.h"

int main(int argc,char *argv[]){
  Image<int> image(2,2);

  for(int y=0;y<image.height();y++){
    for(int x=0;x<image.width();x++){
      image(x,y) = y*image.width()+x;
      std::cout << image(x,y) << '\t';
    }
    std::cout << std::endl;
  }

  image = image;
  
  std::cout << std::endl;
  for(int y=0;y<image.height();y++){
    for(int x=0;x<image.width();x++)
      std::cout << image(x,y) << '\t';
    std::cout << std::endl;
  }  
}
