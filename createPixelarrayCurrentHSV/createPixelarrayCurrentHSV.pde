/**
 * Load and Display 
 * 
 * Images can be loaded and displayed to the screen at their actual size
 * or any other size. 
 */

PImage img;  // Declare variable "a" of type PImage

void setup() {
  size(50, 50);
  // The image file must be in the data folder of the current sketch 
  // to load successfully
  img = loadImage("proG1.png");  // Load the image into the program  
  printShit();  
  /*img = loadImage("R.png");  // Load the image into the program  
  printShit();  
  img = loadImage("O.png");  // Load the image into the program  
  printShit();  
  img = loadImage("G.png");  // Load the image into the program  
  printShit();  
  img = loadImage("L.png");  // Load the image into the program  
  printShit(); 
  img = loadImage("O.png");  // Load the image into the program  
  printShit(); 
  img = loadImage("V.png");  // Load the image into the program  
  printShit();  
  img = loadImage("E.png");  // Load the image into the program  
  printShit();
  */
}

void draw() {
  // Displays the image at its actual size at point (0,0)
  image(img, 0, 0);
  // Displays the image at point (0, height/2) at half of its size
  
}
void printShit(){
  println("const byte imageGloveVal[] PROGMEM  = {");
  for(int i = 0; i < img.pixels.length; i++) {
     int pixel = int(brightness(img.pixels[i]));
    if( pixel < 32){
      print("0, ");
    }
    else if( pixel < 64){
      print("64, ");
    }
    else if( pixel < 96){
      print("96, ");
    }
    else if( pixel < 128){
      print("128, ");
    }
    else if( pixel < 160){
      print("160, ");
    }
    else if( pixel < 192){
      print("192, ");
    }
    else {
      print("255, ");
    }
  }
  println("};");
  println();
  println("const byte imageGloveHue[] PROGMEM  = {");
  for(int i = 0; i < img.pixels.length; i++) {
     int pixel = int(hue(img.pixels[i]));    
      print(pixel + ", ");
    }
    println("};");
    println();
    println("const byte imageGloveSat[] PROGMEM  = {");
  for(int i = 0; i < img.pixels.length; i++) {
     int pixel = int(saturation(img.pixels[i]));    
      if(pixel == 0){
        print(pixel + ", ");
      }
      else{
      print(min(pixel + 64,255) + ", ");
      }
    }
    println("};");
}