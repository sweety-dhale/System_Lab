/*
*           AUTOMATED VOTING SYSTEM
*/

import java.io.*; 
import java.util.concurrent.*; 
import java.text.ParseException;

// Driver class which manages EVM and CCM
// and also creates threads for each booth
class VotingSystem02
{  
    public static void main(String args[]) throws Exception 
    {  
        try 
        {          
            // creating five threads with name A, B, C, D and E 
            // Each thread corresponds to one EVM(booth)
            EVM02 evm1 = new EVM02("A"); 
            EVM02 evm2 = new EVM02("B"); 
            EVM02 evm3 = new EVM02("C"); 
            EVM02 evm4 = new EVM02("D"); 
            EVM02 evm5 = new EVM02("E"); 
          
            // stating threads A, B, C, D and E 
            evm1.start(); 
            evm2.start(); 
            evm3.start(); 
            evm4.start(); 
            evm5.start(); 
          
            // waiting for threads A, B, C, D and E 
            // to complete their execution 
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
// by each booth and threads read file and 
// get connected with CCM for vote count
class EVM02 extends Thread 
{ 
    String boothID; 
    static int timer;
    static Semaphore sem_t;

    // this constructor create thread by calling 
    // parent class constructor and also initializes
    // timer variable and
    // semaphore variable used for timer increment
    public EVM02(String booth)  
    { 
        super(booth); 
        this.sem_t=new Semaphore(1);
        this.boothID = booth; 
        timer=10;
    } 

    // method to run threads
    public void run() 
    { 
        int boothNum=0;
        if(this.getName().equalsIgnoreCase("A"))
            boothNum=1;
        else 
        if(this.getName().equalsIgnoreCase("B"))
            boothNum=2;
        else
        if(this.getName().equalsIgnoreCase("C"))
            boothNum=3;
        else 
        if(this.getName().equalsIgnoreCase("D"))
            boothNum=4;
        else 
        if(this.getName().equalsIgnoreCase("E"))
            boothNum=5;

        try
        {
            // File is created to write the voting record
            File outFile = new File("RecordFile02.txt");
            FileWriter fw = new FileWriter(outFile);  
            BufferedWriter bw = new BufferedWriter(fw);
            bw.close();
            
            // this manages the threads to be executed 
            // read corresponding input files which contains
            // response of voters  
            // then call CCM methods to update the vote count
            // write the voting record to output file
            // After reading input file when poll is completed
            // a signal is send to CCM and so the number of 
            // booths completed polls is noted down
            switch(boothNum)
            {
                case 1:
                        try
                        {
                            File file = new File("EVM_A.txt"); 
                            FileReader fr = new FileReader(file);
                            BufferedReader br = new BufferedReader(fr); 
                            CCM02 ccm=new CCM02();
                            String str,response; 
                            while ((str = br.readLine()) != null) 
                            {
                                String[] line = str.split(" ",2);
                                response=line[0];
                                sem_t.acquire();
                                ccm.countVote(response);
                                fw = new FileWriter(outFile, true);  
                                bw = new BufferedWriter(fw);
                                bw.write(" "+(timer+=5)+" "+response+" "+boothID+"\n");
                                bw.close();
                                sem_t.release();
                            }
                            br.close();
                            ccm.pollCompleted("A");
                        }
                        catch(Exception e)
                        {
                            System.out.println(e); 
                        }
                        break;

                case 2:
                        try
                        {
                            File file = new File("EVM_B.txt"); 
                            FileReader fr = new FileReader(file);
                            BufferedReader br = new BufferedReader(fr); 
                            CCM02 ccm=new CCM02();
                            String str,response; 
                            while ((str = br.readLine()) != null) 
                            {
                                String[] line = str.split(" ",2);
                                response=line[0];
                                sem_t.acquire();
                                ccm.countVote(response);
                                fw = new FileWriter(outFile, true);  
                                bw = new BufferedWriter(fw);
                                bw.write(" "+(timer+=5)+" "+response+" "+boothID+"\n");
                                bw.close();
                                sem_t.release();
                            }
                            br.close();
                            ccm.pollCompleted("B");
                        }
                        catch(Exception e)
                        {
                            System.out.println(e); 
                        }
                        break;

                case 3:
                        try
                        {
                            File file = new File("EVM_C.txt"); 
                            FileReader fr = new FileReader(file);
                            BufferedReader br = new BufferedReader(fr);
                            CCM02 ccm=new CCM02();
                            String str,response; 
                            while ((str = br.readLine()) != null) 
                            {
                                String[] line = str.split(" ",2);
                                response=line[0];
                                sem_t.acquire();
                                ccm.countVote(response);
                                fw = new FileWriter(outFile, true);  
                                bw = new BufferedWriter(fw);
                                bw.write(" "+(timer+=5)+" "+response+" "+boothID+"\n");
                                bw.close();
                                sem_t.release();
                            }
                            br.close();
                            ccm.pollCompleted("C");
                        }
                        catch(Exception e)
                        {
                            System.out.println(e); 
                        }
                        break;
                
                case 4:
                        try
                        {
                            File file = new File("EVM_D.txt"); 
                            FileReader fr = new FileReader(file);
                            BufferedReader br = new BufferedReader(fr);
                            CCM02 ccm=new CCM02();
                            String str,response; 
                            while ((str = br.readLine()) != null) 
                            {
                                String[] line = str.split(" ",2);
                                response=line[0];
                                sem_t.acquire();
                                ccm.countVote(response);
                                fw = new FileWriter(outFile, true);  
                                bw = new BufferedWriter(fw);
                                bw.write(" "+(timer+=5)+" "+response+" "+boothID+"\n");
                                bw.close();
                                sem_t.release();
                            }
                            br.close();
                            ccm.pollCompleted("D");
                        }
                        catch(Exception e)
                        {
                            System.out.println(e); 
                        }
                        break;

                case 5:
                        try
                        {
                            File file = new File("EVM_E.txt"); 
                            FileReader fr = new FileReader(file);
                            BufferedReader br = new BufferedReader(fr);
                            CCM02 ccm=new CCM02();
                            String str,response; 
                            while ((str=br.readLine()) != null) 
                            {
                                String[] line = str.split(" ",2);
                                response=line[0];
                                sem_t.acquire();
                                ccm.countVote(response);
                                fw = new FileWriter(outFile, true);  
                                bw = new BufferedWriter(fw);
                                bw.write(" "+(timer+=5)+" "+response+" "+boothID+"\n");
                                bw.close();
                                sem_t.release();
                            }
                            br.close();
                            ccm.pollCompleted("E");
                        }
                        catch(Exception e)
                        {
                            System.out.println(e); 
                        }
                        break;
            }  
        } 
        catch(Exception e)
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
class CCM02
{
    private static int cnt_x,cnt_y,cnt_z,booths;
    static Semaphore sem_x,sem_y,sem_z,sem_b;

    // this constructor initializes initial vote count
    // for each candidate and semaphores which controlls 
    // the increment of vote count values
    CCM02()
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
    void countVote(String response)
    {
        try 
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
        catch (Exception e) 
        {
            System.out.println(e);
        }
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
  
