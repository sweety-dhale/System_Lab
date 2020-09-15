/*
*           AUTOMATED VOTING SYSTEM
*/

import java.io.*; 
import java.util.concurrent.*; 
import java.text.ParseException;

// Driver class which manages EVM and CCM
// and also creates threads for each booth
class VotingSystem01
{  
    public static void main(String args[]) throws Exception 
    {  
        try 
        {          
            // creating five threads with name A, B, C, D and E 
            // Each thread corresponds to one EVM(booth)
            EVM evm1 = new EVM("A"); 
            EVM evm2 = new EVM("B"); 
            EVM evm3 = new EVM("C"); 
            EVM evm4 = new EVM("D"); 
            EVM evm5 = new EVM("E"); 
          
            // stating threads A, B, C, D and E 
            evm1.start(); 
            evm2.start(); 
            evm3.start(); 
            evm4.start(); 
            evm5.start(); 
          
            // waiting for threads A, B, C, D and E  
            evm1.join(); 
            evm2.join(); 
            evm3.join(); 
            evm4.join(); 
            evm5.join();  
        } 
        catch (Exception e) 
        {
            System.out.println(e);       
        }     
    }  
} 

// This class manages the work to be done 
// by Central Controller machine and update 
// the vote count and keep track of booths that
// completed poll and when all the booths complete 
// their tasks, a message is displayed which 
// declares the winner
class CCM
{
    private static int cnt_x,cnt_y,cnt_z,booths;
    static Semaphore sem_x,sem_y,sem_z,sem_b;

    // this constructor initializes initial vote count
    // for each candidate and semaphores which controlls 
    // the increment of vote count values
    CCM()
    {
        cnt_x=0; cnt_y=0;
        cnt_z=0; booths=0;
        this.sem_b = new Semaphore(1);
        this.sem_x = new Semaphore(1); 
        this.sem_y = new Semaphore(1); 
        this.sem_z = new Semaphore(1); 
    }

    // this method checks what response has been 
    // send by EVM and according calls corresponding 
    // method to increment the vote count
    void countVote(String response,String boothID)
    {
        if(response.equalsIgnoreCase("X"))
            countVote_X();
        else 
        if(response.equalsIgnoreCase("Y"))
            countVote_Y();
        else
        if(response.equalsIgnoreCase("Z"))
            countVote_Z();
    }

    // increment vote count of X
    private void countVote_X()
    {
        try 
        {
            sem_x.acquire();
            cnt_x+=1;
            sem_x.release();   
        } 
        catch (InterruptedException e) 
        {
            System.out.println(e);
        }
    }

    // increment vote count of Y
    private void countVote_Y()
    {
        try 
        {
            sem_y.acquire();
            cnt_y+=1;
            sem_y.release();   
        } 
        catch (InterruptedException e) 
        {
            System.out.println(e);
        }
    }

    // increment vote count of Z
    private void countVote_Z()
    {
        try 
        {
            sem_z.acquire();
            cnt_z+=1;
            sem_z.release();   
        } 
        catch (InterruptedException e) 
        {
            System.out.println(e);
        }
    }

    // keeps track of booths that have completed 
    // the poll and declares winners when all 
    // booths completes poll
    void pollCompleted(String booth)
    {
        try 
        {
            sem_b.acquire();
            booths+=1;
            System.out.println("Poll completed at booth "+booth);

            if(booths==5)
            {
                System.out.println("Poll completed at all booths");
                System.out.println("Vote count of X: "+cnt_x);
                System.out.println("Vote count of Y: "+cnt_y);
                System.out.println("Vote count of Z: "+cnt_z);
                if(cnt_x>cnt_y)
                {
                    if(cnt_x>cnt_z)
                        System.out.println("And the winner is X"); 
                    else 
                    if(cnt_x<cnt_z)
                        System.out.println("And the winner is Z"); 
                    else
                        System.out.println("There is tie between X and Z"); 
                }
                else
                if(cnt_x<cnt_y)
                {
                    if(cnt_y>cnt_z)
                        System.out.println("And the winner is Y"); 
                    else
                    if(cnt_y<cnt_z)
                        System.out.println("And the winner is Z"); 
                    else
                        System.out.println("There is tie between Y and Z");
                }
                else
                {
                    if(cnt_x==cnt_y && cnt_x==cnt_z)
                        System.out.println("There is tie between X,Y and Z");
                    else
                        System.out.println("There is tie between X and Y");
                }
            }
            sem_b.release();
        } 
        catch (InterruptedException e) 
        {
            System.out.println(e);
        }  
    }     
}

// This class manages the work to be done 
// by each booth and threads read file and 
// get connected with CCM for vote count
class EVM extends Thread 
{ 
    String booth; 
    static int timer;
    static Semaphore sem_t;

    // this constructor create thread by calling 
    // parent class constructor and also initializes
    // timer variable and
    // semaphore variable used for timer increment
    public EVM(String booth)  
    { 
        super(booth); 
        this.sem_t=new Semaphore(1);
        this.booth = booth; 
        timer=10;
    } 

    // method to run threads
    public void run() 
    {     
        File inFile,outFile;
        FileReader fr;
        FileWriter fw;
        BufferedWriter bw; 
        BufferedReader br;
        CCM ccm=new CCM();
        try
        {
            // File is created to read the voters response
            inFile = new File("input.txt"); 
            fr = new FileReader(inFile);
            br = new BufferedReader(fr);

            // File is created to write the voting record
            outFile = new File("RecordFile01.txt");
            fw = new FileWriter(outFile);  
            bw = new BufferedWriter(fw);
            bw.close();

            // this manages the threads to be executed 
            // read corresponding input files which contains
            // response of voters  
            // then call CCM methods to update the vote count
            // write the voting record to output file
            // After reading input file when poll is completed
            // a signal is send to CCM and so the number of 
            // booths completed polls is noted down
            String str,time,response,boothID; 
            while ((str = br.readLine()) != null) 
            {
                String[] line = str.split(" ",3);
                time=line[0];       
                response=line[1];   
                boothID=line[2];   
                
                if(booth.equals(boothID))
                {
                    sem_t.acquire();
                    ccm.countVote(response,boothID);
                    fw = new FileWriter(outFile, true);  
                    bw = new BufferedWriter(fw);
                    bw.write(" "+(timer+=5)+" "+response+" "+boothID+"\n");
                    sem_t.release();
                    bw.close();
                }
            }
            ccm.pollCompleted(booth);
        }
        catch(Exception e)
        {
            System.out.println(e); 
        }  
    } 
}
 
