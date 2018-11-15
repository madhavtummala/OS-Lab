import java.util.*;
import java.io.*;
import java.util.concurrent.TimeUnit;

public class RR{

    int qtm = 3;
    ArrayList<Integer> wtime = new ArrayList<Integer>();
    ArrayList<Integer> ttime = new ArrayList<Integer>();

    public static void main(String args[]){
        RR rrobj = new RR();
        try{
            rrobj.run();
        }
        catch(IOException ex){
            System.out.println("File not found exception..!");
        }
    }

    static class Proc{
        int cputime;
        int rand;
        LinkedList<Integer> cpuList;
        LinkedList<Integer> ioList;
        boolean ready;

        Proc(int cputime){
            rand=-1;
            this.cputime = cputime;
            ready=true;
            cpuList = new LinkedList<>();
            ioList = new LinkedList<>();
        }
    }

    void run() throws IOException{

        File f=new File("test1.txt");
        BufferedReader br=new BufferedReader(new FileReader(f));
        String st="";

        Proc proc[]=new Proc[100];
        int art[]=new int[100];
        Arrays.fill(art,1000000);

        int count = 0, time = 0, complete = 0;
        int i, val, min = 10000;
        int eNext = 0, last = -1;
        int j = 1, iq = -1, index = 0, considered = 0;

        boolean anyScheduled=false;

        while((st = br.readLine()) != null){
            String x[] = st.split(" ");
            val = Integer.parseInt(x[0]);
            proc[count] = new Proc(val);
            art[count] = val;
            i = 1;
            while(true){
                val = Integer.parseInt(x[i]);
                if(val == -1)
                	break;
                else if(proc[count].cpuList.size() == proc[count].ioList.size())
                	proc[count].cpuList.add(val);
                else
                	proc[count].ioList.add(val);
                i++;
            }
            count++;
        }

        index = count;

        for(i = 0; i < count; i++){
            printResult(proc[i]);
            wtime.add(500);
            ttime.add(0);
            System.out.println();
        }  

        while(complete < count){

            anyScheduled = false;
            for(i = 0; i < count+1; i++){
               	if(i == count){
                   	if(!anyScheduled)
                   		time++;
                   	continue;
               	}
               	if(proc[i].cputime == -1){
                  if(ttime.get(i)==0) ttime.set(i,time-proc[i].cputime);
               		continue;
                }
               	considered++;
               	if(!proc[i].ready){
                   	if(proc[i].ioList.size()>0){
                       val = proc[i].ioList.getFirst();
                       if(proc[i].rand+val <= time){
                           proc[i].ready = true;
                           proc[i].ioList.removeFirst();
                       }
                   }
               	}
                if(!proc[i].ready)
                	continue;
               	else
               	anyScheduled=true;
               	if(proc[i].cpuList.getFirst() <= qtm){
                    time+= proc[i].cpuList.getFirst();
                    // ttime.get(i)=time-ttime.get(i);
                    System.out.print((i+1)+":("+proc[i].cpuList.getFirst()+")   ");
                   	proc[i].cpuList.removeFirst();
                   	if(proc[i].cpuList.size() == 0){
                    	complete++;
                       	proc[i].cputime = -1;
                   	}
                   	else{
                       	proc[i].rand = time;
                       	proc[i].ready = false;
                   	}
               	}
               	else{
                   	time += qtm;
                   	System.out.print((i+1)+":("+qtm+")   ");
                   	val = proc[i].cpuList.getFirst();
                   	proc[i].cpuList.removeFirst();
                   	proc[i].cpuList.addFirst(val-qtm);
                   	proc[i].rand = time;
               	}

            }

            System.out.println();
        }

        for(int k=0;k<count;k++){
          wtime.set(k,ttime.get(k)-wtime.get(k));
          System.out.println("TurnAroundTime: "+ttime.get(k)+"    WaitingTime: "+wtime.get(k));
        }
        int sum1=0,sum2=0;
        for(int k=0;k<count;k++){
          sum1+=ttime.get(k);
          sum2+=wtime.get(k);
        }
        System.out.println("AverageTurnAroundTime: "+(sum1/count)+"    AverageWaitingTime: "+(sum2/count)+"    Throughput: "+((double)count/(double)time));
        System.out.println();  
    }

    int gsmin(int arr[]){

        int x = arr[0], i, ind = 0;
        for(i = 1; i < 100; i++){
            if(arr[i] < x && arr[i] != 100000){
                ind = i;
                x = arr[i];
            }
        }
        if(x != 100000)
            return ind;
        else
            return -1;
    }

    void printResult(Proc x){
      //int bursttime=0;
        System.out.print("Time: "+x.cputime+"     ");
        Iterator itr = x.cpuList.iterator();
        while(itr.hasNext()){
            System.out.print(itr.next()+" ");
            //bursttime = bursttime + (int)itr.next();
        }
        System.out.print("      ");
        Iterator itr2 = x.ioList.iterator();
        while(itr2.hasNext()){
            System.out.print(itr2.next()+" ");
        }
        // return bursttime;
    }


}