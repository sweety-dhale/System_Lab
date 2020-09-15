import java.io.*; 
import java.util.*; 

//CLASS SHARED AMONG THREADS
class SharedVar 
{
	public int chassis,seat,engine,tires,top_cover,painting; //VARIABLES FOR EACH COMPONENT OF CAR
	
	//METHOD LEVEL OR CLASS LEVEL SYNCHRONIZATION

	//GETTER METHOD FOR CHASSIS
	public synchronized int getChassis() 
	{	return chassis;	}

	 //GETTER METHOD FOR SEAT
	public synchronized int getSeat()
	{	return seat;}	

	//GETTER METHOD FOR ENGINE
	public synchronized int getEngine()
	{	return engine;}	

	//GETTER METHOD FOR TIRES
	public synchronized int getTires()
	{	return tires;}	
	
	//GETTER METHOD FOR TOP COVER
	public synchronized int getTopcover()
	{	return top_cover;}	

	//GETTER METHOD FOR PAINT
	public synchronized int getPainting()
	{	return painting;}	

	//SETTER METHOD FOR CHASSIS
	public synchronized void setChassis()
	{	 this.chassis=1;
	}

	//SETTER METHOD FOR SEAT
	public synchronized void setSeat()
	{	 this.seat=1;}

	//SETTER METHOD FOR ENGINE
	public synchronized void setEngine()
	{	 this.engine=1;}

	//SETTER METHOD FOR TIRES
	public synchronized void setTires()
	{	 this.tires=1;}

	//SETTER METHOD FOR TOP COVER
	public synchronized void setTopcover()
	{	 this.top_cover=1;}

	//SETTER METHOD FOR PAINT
	public synchronized void setPainting()
	{	 this.painting=1;}	

}
//END OF SHARED VAR CLASS

//THREAD CLASS FOR WORKERS
class Worker extends Thread 
{ 
    private String msg; //MSG VARIABLE TO IDENTIFY THE WORKER
    private SharedVar var[]; //SHARED VARIABLE FOR CARS TO STORE OBJECTS OF N CARS
    private int no_cars; // NUMBER OF CARS NEED TO BE CONSTRUCTED
    private int start=0; //ITEARTE EVERY THREAD N TIMES

    //CONSTRUCTOR OF THREAD TO SET THE THREAD LEVE VARIABLES
    Worker(String name,SharedVar var[],int no_cars) 
    {
	msg=name;
	this.var=var;
	this.no_cars=no_cars;
    }

    //EXECUTION BLOCK FOR THE THREADS
    public void run() 
    { 
        while(start<no_cars)
	{
		//BLOCK FOR B WORKER		
		if(this.msg=="B")
		{
			//BUSY WAIT TILL THE PREVIOUS CAR IS NOT COMPLETED
			while(start>=1 && var[start-1].getPainting()!=1){}
			
			System.out.println();

			System.out.println("B put chassis on the belt of car no " +(start+1));
			
			//SET CHASSIS FOR THE CURRENT CAR	
			var[start].setChassis();	
		}
		//BLOCK FOR A WORKER
		else if(this.msg=="A")
		{
			//BUSY WAIT TILL THE CHASSIS FOR THE CURRENT CAR IS NOT SET
			while(var[start].getChassis()!=1){}
			
				System.out.println("A put tires on the belt of car no : "+(start+1));
			
				//SET CHASSIS FOR THE CURRENT CAR
				var[start].setTires();
			
			//BUSY WAIT TILL THE TOP FOR THE CURRENT CAR IS NOT SET
			while(var[start].getTopcover()!=1){}

				System.out.println("A painted the car no : "+(start+1));

				//DO PAINTING FOR THE CURRENT CAR
				var[start].setPainting();
		}
		//BLOCK FOR C WORKER
		else if(this.msg=="C")
		{
			//BUSY WAIT TILL THE CHASSIS FOR THE CURRENT CAR IS NOT SET
			while(var[start].getChassis()!=1){}
			
				System.out.println("C put seat on the belt of car no : "+(start+1));

				//SET SEAT FOR THE CURRENT CAR
				var[start].setSeat();
		}
		//BLOCK FOR D WORKER
		else if(this.msg=="D")
		{
			//BUSY WAIT TILL THE CHASSIS FOR THE CURRENT CAR IS NOT SET
			while(var[start].getChassis()!=1){}
			
				System.out.println("D put engine on the belt of car no : "+(start+1));

				//SET ENGINE FOR THE CURRENT CAR
				var[start].setEngine();
			
			//BUSY WAIT TILL THE SEAT ENGINE AND TIRES FOR THE CURRENT CAR IS NOT SET
			while(var[start].getSeat()!=1 || var[start].getEngine()!=1 || var[start].getTires()!=1){}
	
				System.out.println("D put top cover on the belt of car no : "+(start+1));

				//SET TOP COVER FOR THE CURRENT CAR
				var[start].setTopcover();
		}
		start++;
	}
    }
    //END OF RUN METHOD 
} 
//END OF THREAD CLASS
  
// DRIVER CLASS
class Ques_2 
{ 
    public static void main(String[] args) 
    { 
	Scanner sc=new Scanner(System.in);
	System.out.print("Enter the number of cars : ");
	int n = sc.nextInt();
	
	//OBJECTS FOR N CARS
	SharedVar car[] = new SharedVar[n];

	for(int i=0;i<n;i++)
	car[i]= new SharedVar();

	//THREAD FOR EACH WORKER
    	Worker a = new Worker("A",car,n); 
	Worker b = new Worker("B",car,n); 
	Worker c = new Worker("C",car,n); 
	Worker d = new Worker("D",car,n);

		//START THE THREADS 
          	a.start(); 
	 	b.start(); 
		c.start(); 
		d.start();
    } 
} 
//END OF MAIN CLASS
