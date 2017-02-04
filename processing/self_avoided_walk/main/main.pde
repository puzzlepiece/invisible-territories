//Mark Kleback 
//Nature of Code

walker w;
XML xml;

void settings(){
  size(640, 480);
  //fullScreen();
  
}

void setup(){
   frameRate(30);
   background(255);
  w= new walker();
  
  xml = new XML("territory");
  
  
}

void draw() {
  w.step();
  w.display();
  
  saveXML(xml, "data/territory.xml");
}