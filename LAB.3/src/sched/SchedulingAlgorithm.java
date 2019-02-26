package sched;// Run() is called from Scheduling.main() and is where
// the scheduling algorithm written by the user resides.
// User modification should occur within the Run() function.

import java.util.Vector;
import java.io.*;

public class SchedulingAlgorithm {

  public static Results Run(int runtime, Vector processVector, Results result) {
    int comptime = 0;
    int currentProcess = 0;
    int size = processVector.size();
    int completed = 0;

    int quantum = 30;
    int currQuant = 0;

    String resultsFile = "resourses/results/Summary-Processes";
    Vector<Integer> queue = new Vector<>(processVector.size());

    for (int i = 0; i < processVector.size(); i++) queue.add(i);

    result.schedulingType = "Interactive (Preemptive)";
    result.schedulingName = "Round-Robin";
    try {
      //BufferedWriter out = new BufferedWriter(new FileWriter(resultsFile));
      //OutputStream out = new FileOutputStream(resultsFile);
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));
      sProcess process = (sProcess) processVector.elementAt(queue.get(0));
      out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
      while (comptime < runtime) {
        if (process.cpudone == process.cputime) {
          completed++;
          out.println("Process: " + currentProcess + " completed... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
          if (completed == size) {
            result.compuTime = comptime;
            out.close();
            return result;
          }
          currQuant = 0;
          queue.remove(0);
          process = (sProcess) processVector.elementAt(currentProcess = queue.get(0));
          out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
        }      
        else if (process.ioblocking == process.ionext) {
          out.println("Process: " + currentProcess + " I/O blocked... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + currQuant + ")");
          process.numblocked++;
          process.ionext = 0;
          currQuant = 0;
          queue.remove(0);
          queue.add(currentProcess);
          process = (sProcess) processVector.elementAt(currentProcess = queue.get(0));
          out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
        }
        else if (currQuant == quantum) {
            out.println("Process: " + currentProcess + " quantum expired... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + currQuant + ")");
            process.numblocked++;
            currQuant = 0;
            queue.remove(0);
            queue.add(currentProcess);
            process = (sProcess) processVector.elementAt(currentProcess = queue.get(0));
            out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + ")");
        }
        process.cpudone++;
        currQuant++;
        if (process.ioblocking > 0) {
          process.ionext++;
        }
        comptime++;
      }
      out.close();
    } catch (IOException e) { e.printStackTrace(); }
    result.compuTime = comptime;
    return result;
  }
}
