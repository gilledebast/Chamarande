import processing.serial.*;
import controlP5.*;

ControlP5 cp5;
Serial myPort;

int nb_Sensors = 4;
float[] sensorValue = new float[nb_Sensors];    // global variable for storing mapped sensor values

int sliderValue = 100;
int value = 0;

Slider s;

void setup(){
  size(600,800, P2D);
  background(0);
  
  String portName = "/dev/cu.usbserial-00002014";
  myPort = new Serial(this, portName, 9600);
  myPort.clear();
  myPort.bufferUntil('\n');  // don't generate a serialEvent() until you get a newline (\n) byte
  
  cp5 = new ControlP5(this);
  s = cp5.addSlider("left",  0,1000,100,100,200,30,300);
}

void draw(){
  background(0);
  //text(value, 50, 50);
  s.setValue(sensorValue[0]);
}

void serialEvent (Serial myPort) {
  String inString = myPort.readStringUntil('\n');  // get the ASCII string

  if (inString != null) {  // if it's not empty
    
    inString = trim(inString);  // trim off any whitespace
    try{
      println(inString);
      value = int(inString);
    } catch(Exception e){
      //e.printStackTrace();
    }
  }
}