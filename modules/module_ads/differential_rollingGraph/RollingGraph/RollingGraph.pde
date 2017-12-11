import processing.serial.*;
import org.gwoptics.graphics.graph2D.Graph2D;
import org.gwoptics.graphics.graph2D.traces.ILine2DEquation;
import org.gwoptics.graphics.graph2D.traces.RollingLine2DTrace;

Serial myPort;
float[] differentialValue = new float[2];
float[] data_min = new float[2];
float[] data_max = new float[2];

//Average
float[] data_avg = new float[2];
int count = 0;

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
    
  String portName = "/dev/cu.usbserial-00001014";
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
        if(data_min[i] > incomingValues[i]){
          data_min[i] = incomingValues[i];
        } 
        if(data_max[i] < incomingValues[i]){
          data_max[i] = incomingValues[i];
        }
        
        data_avg[i] += incomingValues[i];
        count++;
        
        println("#"+i+" Average value :"+data_avg[i]/count);
        
        differentialValue[i] = map(incomingValues[i],data_min[i],data_max[i],-10,10);
      }
    } catch(Exception e){
      
    }
  }
}

void keyPressed(){
  for(int i=0; i<2; i++){
   reset_data(i);
  }
  count=0;
}

/*void keyReleased(){
  
}*/

void reset_data(int i){
  data_max[i] = 0;
  data_min[i] = 0;
  data_avg[i] = 0;
}