import processing.serial.*;
import org.gwoptics.graphics.graph2D.Graph2D;
import org.gwoptics.graphics.graph2D.traces.ILine2DEquation;
import org.gwoptics.graphics.graph2D.traces.RollingLine2DTrace;

Serial myPort;
float[] differentialValue = new float[2];

class eq implements ILine2DEquation{
  public double computePoint(double x,int pos) {
   return differentialValue[0];
  }  
}

class eq2 implements ILine2DEquation{
  public double computePoint(double x,int pos) {
    return differentialValue[1];
  }  
}

RollingLine2DTrace r,r2;
Graph2D g;
  
void setup(){
  size(600,300);
  
  r  = new RollingLine2DTrace(new eq() ,100,0.1f);
  r.setTraceColour(0, 0, 0);
  
  r2 = new RollingLine2DTrace(new eq2(),100,0.1f);
  r2.setTraceColour(255, 0, 0);

  g = new Graph2D(this, 400, 200, false);

  g.addTrace(r);
  g.addTrace(r2);
  
  g.position.y = 50;
  g.position.x = 100;

  g.setYAxisTickSpacing(100);
  g.setXAxisMax(5f);
 
  g.setYAxisMin(-10f);
  g.setYAxisMax(10f);
    
  String portName = "/dev/cu.usbserial-00002014";
  myPort = new Serial(this, portName, 9600);
  myPort.clear();
  myPort.bufferUntil('\n');  // don't generate a serialEvent() until you get a newline (\n) byte
}

void draw(){
  background(200);
  g.draw();
}

void serialEvent (Serial myPort) {
  String inString = myPort.readStringUntil('\n');  // get the ASCII string
  if (inString != null) {  // if it's not empty  
    inString = trim(inString);  // trim off any whitespace
    try{
      int incomingValues[] = int(split(inString, ","));
      for (int i = 0; i < incomingValues.length; i++) {
       differentialValue[i] = incomingValues[i];
      }
    } catch(Exception e){
      
    }
  }
}