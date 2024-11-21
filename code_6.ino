import processing.serial.*;
import java.util.concurrent.*;

Serial myPort;
String angle="";
String distance="";
String data="";
String noObject;
float pixsDistance;
int iAngle, iDistance;
int index1=0;
int index2=0;
PFont orcFont;
CopyOnWriteArrayList<PVector> objects = new CopyOnWriteArrayList<PVector>();
boolean use360Mode = true; // Set to true for 360° mode, false for 180° mode

void setup() {
  size(800, 800);
  smooth();
  println("Available serial ports:");
  printArray(Serial.list());
  
  String portName = Serial.list()[1]; // Default to first port
  
  println("Attempting to connect to port " + portName);
  
  try {
    myPort = new Serial(this, portName, 9600);
    myPort.bufferUntil('\n');
    println("Connected successfully to port: " + portName);
  } catch (Exception e) {
    println("Error opening serial port: " + e.getMessage());
    println("Make sure Arduino is connected and no other program is using the port.");
    exit();
  }
}

void draw() {
  fill(98, 245, 31);
  noStroke();
  fill(0, 4); 
  rect(0, 0, width, height - height * 0.065); 
  
  fill(98, 245, 31);
  drawRadar(); 
  drawLine();
  drawObject();
  drawText();
}

void serialEvent (Serial myPort) {
  data = myPort.readStringUntil('\n');
  if (data != null) {
    data = data.trim();
    println("Raw data: " + data);  // Debug line
    
    String[] parts = split(data, ',');
    if (parts.length == 2) {
      angle = parts[0];
      distance = parts[1].replace(".", "");
      
      iAngle = int(angle);
      iDistance = int(distance);
      
      println("Processed - Angle: " + iAngle + ", Distance: " + iDistance);  // Debug line
      
      if (iDistance < 40) {
        objects.add(new PVector(iAngle, iDistance));
      }
      
      while (objects.size() > 10) {
        objects.remove(0);
      }
    }
  }
}

void drawRadar() {
  pushMatrix();
  translate(width / 2, height / 2);
  noFill();
  strokeWeight(2);
  stroke(98, 245, 31);
  
  arc(0, 0, width - width * 0.0625, width - width * 0.0625, 0, TWO_PI);
  arc(0, 0, width - width * 0.27, width - width * 0.27, 0, TWO_PI);
  arc(0, 0, width - width * 0.479, width - width * 0.479, 0, TWO_PI);
  arc(0, 0, width - width * 0.687, width - width * 0.687, 0, TWO_PI);
  
  for (int i = 0; i < 360; i += 30) {
    line(0, 0, (width / 2) * cos(radians(i)), -(width / 2) * sin(radians(i)));
  }
  popMatrix();
}

void drawObject() {
  pushMatrix();
  translate(width / 2, height / 2);
  strokeWeight(9);
  stroke(255, 10, 10);
  
  for (PVector obj : objects) {
    float angle = obj.x;
    float distance = obj.y;
    pixsDistance = distance * ((height - height * 0.1666) * 0.025);
    float x = pixsDistance * cos(radians(angle));
    float y = -pixsDistance * sin(radians(angle));
    point(x, y);
  }
  popMatrix();
}

void drawLine() {
  pushMatrix();
  strokeWeight(9);
  stroke(30, 250, 60);
  translate(width / 2, height / 2);
  line(0, 0, (height - height * 0.12) * cos(radians(iAngle)), -(height - height * 0.12) * sin(radians(iAngle)));
  popMatrix();
}

void drawText() {
  pushMatrix();
  if (iDistance > 40) {
    noObject = "Out of Range";
  } else {
    noObject = "In Range";
  }
  fill(0, 0, 0);
  noStroke();
  rect(0, height - height * 0.0648, width, height);
  fill(98, 245, 31);
  textSize(25);
  
  text("10cm", width - width * 0.3854, height - height * 0.0833);
  text("20cm", width - width * 0.281, height - height * 0.0833);
  text("30cm", width - width * 0.177, height - height * 0.0833);
  text("40cm", width - width * 0.0729, height - height * 0.0833);
  textSize(40);
  text("Radar", width - width * 0.875, height - height * 0.0277);
  text("Angle: " + iAngle + "°", width - width * 0.48, height - height * 0.0277);
  text("Distance: ", width - width * 0.26, height - height * 0.0277);
  if (iDistance < 40) {
    text("         " + iDistance + " cm", width - width * 0.225, height - height * 0.0277);
  }
  textSize(25);
  fill(98, 245, 60);
  
  for (int i = 0; i < 360; i += 30) {
    pushMatrix();
    translate(width / 2 + (width / 2 - 20) * cos(radians(i)), height / 2 - (width / 2 - 20) * sin(radians(i)));
    rotate(-radians(i));
    text(i + "°", 0, 0);
    popMatrix();
  }
  popMatrix(); 
}
