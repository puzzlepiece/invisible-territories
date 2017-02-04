//Mark Kleback 
//Nature of Code

   int wth = 640;
   int hgt = 960;
   int stepLength= 64;
   int gridW = 10;
   int gridH = 15;
   int direction;
   int randNum, randx, randy;
   //int randNumL = 0;
   //int randNumH = 10;
   
   PVector p1 = new PVector();
   PVector p2 = new PVector();
   ArrayList<PVector> points;
   boolean r, l, u, d;
   boolean canMove;

void settings(){
  size(640, 960);
  //fullScreen();
  
}

void setup(){
   frameRate(8);
   background(255);
   strokeWeight(5);
   stroke(255, 202, 0);
 

 //for(int i = 0; i < gridW; i++){
 //  for(int j = 0; j < gridH; j++){
 //  p.x = i * stepLength;
 //  p.y = j * stepLength;
 //  points.add(p); 
 //  }
 //}
}//

void draw() {
 background(255);
randNum = int (random(10, 50));
//randNumL += 1;
//randNumH += 1;
for(int i = 0; i < randNum; i++){
randx = int (random(gridW));
randy = int (random(gridH));

p1 = new PVector (randx*stepLength, randy*stepLength);
p2 = new PVector();

println("p1.x= " + p1.x);
println("p1.y= " + p1.y);

    r = true;
    l = true;
    u = true;
    d = true;

if(p1.x == 0){
 l = false;
 u = false;
 d = false;
}

if(p1.x == gridW * stepLength){
 r = false; 
}

if(p1.y == 0){
 u = false; 
}

if(p1.y == gridH * stepLength){
 d = false; 
}

println("l= " + l);
println("r= " + r);
println("u= " + u);
println("d= " + d);

  canMove = false;
println("canMove= " + canMove);
while(canMove == false){
    direction = int (random(4));
    println("direction= "+direction);
    switch(direction) {

    case 0://go right

      if (r) {
        p2.x = p1.x + stepLength;
        p2.y = p1.y;
        canMove = true;
        println("p2.x= "+p2.x);
      }

      break;

    case 1://go left
      if (l) {
        p2.x = p1.x - stepLength;
        p2.y = p1.y;
        canMove = true;
        println("p2.x= "+p2.x);
      }
      break;

    case 2://go down
      if (d) {
        p2.y = p1.y + stepLength;
        p2.x = p1.x;
        canMove = true;
        println("p2.y= "+p2.y);
      }
      break;

    case 3://go up
      if (u) { 
        p2.y = p1.y - stepLength;
        p2.x = p1.x;
        canMove = true;
        println("p2.y= "+p2.y);
      }
      break;  
    }
}
    println(p2.x);
    println(p2.y);
    line(p1.x, p1.y, p2.x, p2.y);
}
  saveFrame(); 
}