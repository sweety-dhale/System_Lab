import java.io.*; 
import java.util.*;
import java.lang.*; 

//CLASS SHARED AMONG THREADS SYNCHRONIZED USING WAIT AND NOTIFY
class SharedVar{
	public int token_id=1000412; 
	public int service;
	public int no_served=0;
	boolean valueSet=false; //TO SWITCH BETWEEN BANK AND CUSTOMER THREADS
	boolean array[] = new boolean[3]; //ARRAY TO STORE IF THERE IS A CUSTOMER WAITING FOR PARTICULAR SERVICE
	
	//BANK THREAD WILL GO IN WAITING STATE IF TURN FOR CUSTOMER
	public void setChance()
	{
		//BLOCK OR OBJECT LEVEL SYNC
		synchronized(this)
		{
		while(valueSet)
		{
			if(no_served>=120)
			break;
			try{wait();}catch(Exception e){}
		}
		valueSet=true;
		//NOTIFY CUSTOMERS THREAD
		notify();
		}
	}

	//BANK THREAD WILL GO IN WAITING STATE IF TURN FOR CUSTOMER
	public void getChance()
	{	
		//BLOCK OR OBJECT LEVEL SYNC
		synchronized(this)
		{
		while(!valueSet)
		{
			if(no_served>=120)
			break;
			try{wait();}catch(Exception e){}
		}
		valueSet=false;
		//NOTIFY BANK THREAD
		notify();
		}
	}

	//SERVICE OFFERED BY BANK
	public void setService(int service)
	{
		this.service = service;
	}

	//GETTER METHOD FOR SERVICE
	public int getService()
	{
		return service;
	}

	//INCREASE THE TOKEN NUMBER ONCE THE REQUEST IS SERVED
	public int updateToken()
	{
		this.token_id=this.token_id+1;
		return this.token_id;
	}
}
//END OF SHARED CLASS


//THREAD FOR BANK
class Bank extends Thread{
	//SHARED VARIABLE BETWEEN THREADS
	private SharedVar var;
	
	//CONSTRUCTORE OF BANK
	Bank(SharedVar var)
	{
		this.var=var;
	}
	
	//EXECUTION BLOCK FOR BANK
	public void run()
	{
		while(var.no_served<120)
		{
			int x=0;
			do{
			//GENERATE A SERVICE BY BANK
			Random r=new Random();
			x=r.nextInt(3);
			}while(var.array[x]==false); //ITERATE TILL THE SERVICE OFFERED BY BANK IS NOT REQUESTED BY ANY CUSTOMERS
			var.setService(x);
		
			//NOTIFY THE CUSTOMER
			var.setChance();
			
			//SERVICE TIME OF BANK
			try{Thread.sleep(5);}catch(Exception e){}
		}
	}
}
//END OF BANK THREAD

//THREAD FOR CUSTOMER
class Customer extends Thread{
	//SHARED VARIABLE BETWEEN THREADS
	private SharedVar var;

	private int n;

	//CONSTRUCTORE OF BANK
	Customer(SharedVar var,int n)
	{
		this.var=var;
		this.n = n;
	}
	
	//SET SEVICE BASED ON INDEX VALUE
	public String setService(int x)
	{
		String str="";
		if(x==0)
		str="Draft processing";
		else if(x==1)
		str="Cheque processing";
		else if(x==2)
		str ="Net Banking ";

		return str;
	}

	//WRITE RECORD IN LOG FILE
	public void writeToFile(int cust_no,String str,int token)
	{
		String str_file="" + (n+1) +"\t"+str+"\t"+token+"\n";
		try{  
      			BufferedWriter wr = new BufferedWriter(new FileWriter("records.txt", true));   
      			wr.write(str_file);
     		 	wr.close();
		}
		catch(Exception e){}
	}

	//EXECUTION BLOCK FOR CUSTOMER
	public void run()
	{
		while(var.no_served<120)
		{
			//CUSTOMER REQUEST FOR THE SERVICE
			Random r = new Random();
			int x = r.nextInt(3);
		
			//UPDATE THE VALUE OF REQUEST ARRAY
			var.array[x]=true;

			//NOTIFY THE CUSTOMER
			var.getChance();
			int service = var.getService();
			
			// IF SERVICE OFFERED BY BANK IS THE ONE OFFERED BY CUSTOMER
			if(x == service)
			{
				// GET UPDATED TOKEN
				int token = var.updateToken();
				var.no_served=var.no_served+1;
				String str=setService(x);

				//UPDATE LOG FILE
				writeToFile(n+1,str,token);
     
				System.out.println("Customer "+(n+1)+" served");

				//AS THE SERVICE IS OFFERED RELEASE THE SERVICE
				var.array[x]=false;
			}
		//ARRIVAL RATE OF EVERY CUSTOMER
		try{Thread.sleep(1);}catch(Exception e){}
		}
	}
}
//END OF CUSTOMER THREAD

//DRIVER CLASS
class Ques_3 
{ 
    public static void main(String[] args)throws Exception
    { 
	Scanner sc=new Scanner(System.in);
	System.out.print("Enter number of customers : ");
	int n=sc.nextInt();

	//OPEN RECORD FILE IN WRITE MODE
	BufferedWriter wr = new BufferedWriter(new FileWriter("records.txt", false));  
        
        String s1="C-Id\tNameOfService\t\tVoucher\n";                    
        wr.write(s1);	
        wr.close();
	
	//SHARED VAR
	final SharedVar var = new SharedVar();

		//BANK THREAD
		Bank t1=new Bank(var);
		t1.start();

		//SET HIGH PRIORITY TO BANK THREAD
		t1.setPriority(Thread.MAX_PRIORITY);

		for(int i=0;i<n;i++)
		{
		//START N CUSTOMERS THREADS
		Customer t2 = new Customer(var,i);
		t2.start();

		//ARRIVLE RATE OF CUSTOMERS
		try{Thread.sleep(1);}catch(Exception e){}
		}

	}
}
//END OF DRIVER THREAD
