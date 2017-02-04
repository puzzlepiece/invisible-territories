class walker {
PVector spot = new PVector();
ArrayList<PVector> route;
int stepLength= 80;
int side;
//int endSide = 5;
int count;
boolean saveImage = false;
boolean clear;
int wth = 640;
int hgt = 480;
int gridW = 8;
int gridH = 6;

 
 //constructor
 walker(){ //initialize walker
   route = new ArrayList<PVector>();
   count = 1; 
   side = int(random(4));
   println("side="+side);
   switch(side){//choose initial point on the side
    case 0: //up side
      spot.x = int (random (1, gridW)) * stepLength;
      spot.y = 0; 
      //println("spot.x="+spot.x);
      //println("spot.y="+spot.y);
      break;
    
    case 1: //down side
      spot.x = int (random (1, gridW)) * stepLength;
      spot.y = hgt; 
      //println("spot.x="+spot.x);
      //println("spot.y="+spot.y);
      break;
      
    case 2: //left side
      spot.x = 0;
      spot.y = int (random (1, gridH)) * stepLength; 
      //println("spot.x="+spot.x);
      //println("spot.y="+spot.y);
      break;
      
    case 3: //right side
      spot.x = wth;
      spot.y = int (random (1, gridH)) * stepLength; 
      //println("spot.x="+spot.x);
      //println("spot.y="+spot.y);
      break;
   }
   
   println("spot="+spot.copy());
     
   route.add(spot.copy()); 
   
     
   }

///////////////////////////////////////////////////////////////////
/* display */
   void display(){
    if(saveImage == true && w.count > 1){
      
    saveFrame("images/test-######.jpg");
    saveImage = false;
    
  }
    if (clear == true && count > 1){
     background(255); 
     clear = false;
    }
    
    noFill();
     beginShape();
     strokeWeight(10);
     stroke(255, 0, 0);
     for (PVector v: route){ //redraw + 1 step
      vertex(v.x, v.y); 
     }
     endShape();
   }
   
///////////////////////////////////////////////////////////////////
/* move */
  void step() {     
 
/*----------------- check which direction is blocked -----------------*/
    boolean r, l, u, d;
    
    r = true;
    l = true;
    u = true;
    d = true;

    if(spot.x == 0 || spot.x == wth){
     u = false;
     d = false;
    }
    
    if(spot.y == 0 || spot.y == hgt){
     l = false;
     r = false;
    }
    

    for (int i = 0; i < route.size(); i++) {

      PVector visitedSpot = route.get(i);

      /*no repeating self, no crossing the border*/
      if (spot.x + stepLength == visitedSpot.x && spot.y == visitedSpot.y || spot.x +stepLength > wth) { 
        r =false;
      }
      if (spot.x -stepLength == visitedSpot.x && spot.y == visitedSpot.y || spot.x - stepLength < 0 ) {
        l = false;
      }
      if (spot.y +stepLength == visitedSpot.y && spot.x == visitedSpot.x || spot.y +stepLength > hgt) {
        d = false;
      }
      if (spot.y - stepLength == visitedSpot.y && spot.x == visitedSpot.x || spot.y -stepLength < 0) {
        u = false;
      }
    }

/*----------------- step to the randomly chosen direction -----------------*/
   
    if(r || l || u || d == true){ //right left up down are not all blocked
    boolean canMove = false;
    
   while(canMove == false)
   { int rand = int(random(0, 4));

    switch(rand) {

    case 0://go right

      if (r) {
        spot.x +=stepLength;
        canMove = true;
      }

      break;

    case 1://go left
      if (l) {
        spot.x -= stepLength;
        canMove = true;
      }
      break;

    case 2://go down
      if (d) {
        spot.y +=stepLength;
        canMove = true;
      }
      break;

    case 3://go up
      if (u) { 
        spot.y -= stepLength;
        canMove = true;
      }
      break;  
    }
   }
  }
//if all directions are blocked
//check if the reason is reaching the borders, if so check if the starting and ending points are 
//on the parallel borders, if so, mark "saving image" to true
  
  else if(!r && !d && !u && !l){//right left up down are all blocked
  if((spot.x +stepLength > wth ||  spot.x - stepLength < 0 || spot.y +stepLength > hgt || spot.y -stepLength < 0 ) 
  && route.size() > 12
  ){
  
  //only if the next step will head over the border (aka. the current standing point is on the border), 
  //the image will be saved before clearing the screen
  println("side=" + side);
  println("vertex amount=" + route.size());
  saveImage = true;
  println("length: " + route.size());
  XML newPath = xml.addChild("path");
      
  for (PVector v: route){
      vertex(v.x, v.y); 
    XML vert = newPath.addChild("vertex");
    vert.setInt("x", int(v.x));
    vert.setInt("y", int(v.y));
    }
  }

  //Set "clearing the screen" as true as long as all the paths are blocked
 
  route = new ArrayList();
  
  count++;
  clear = true;
  
     side = int(random(4));
   println("side="+side);
   switch(side){
    case 0: //up side
      spot.x = int (random (1, gridW)) * stepLength;
      spot.y = 0; 
      //println("spot.x="+spot.x);
      //println("spot.y="+spot.y);
      break;
    
    case 1: //down side
      spot.x = int (random (1, gridW)) * stepLength;
      spot.y = hgt; 
      //println("spot.x="+spot.x);
      //println("spot.y="+spot.y);
      break;
      
    case 2: //left side
      spot.x = 0;
      spot.y = int (random (1, gridH)) * stepLength; 
      //println("spot.x="+spot.x);
      //println("spot.y="+spot.y);
      break;
      
    case 3: //right side
      spot.x = wth;
      spot.y = int (random (1, gridH)) * stepLength; 
      //println("spot.x="+spot.x);
      //println("spot.y="+spot.y);
      break;
   }
}

 route.add(spot.copy()); 
  }//step
}//walker class