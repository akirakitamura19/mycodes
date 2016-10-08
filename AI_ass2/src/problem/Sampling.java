package problem;

import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Random;
import java.util.Set;


import tester.Tester;

public class Sampling {
	private static List<Obstacle> obstacles;
	private static ArmConfig initial;
	private static ArmConfig goal;
	private static ArmConfig subgoal;
	private static ArmConfig restart;
	private int jointnumber;
	private int pacecount;
	private Tester test;
	private boolean gripper;
	private final double pace =0.001;
	private PriorityQueue<Node> queue;
	private ArrayList<String> path;
	private ArrayList<Point2D> sites; 
	private ArrayList<Point2D> map;
	private PriorityQueue<Site> route;
	private Set<Point2D> expanded;
	
	
	Sampling(ProblemSpec problem){
		route = new PriorityQueue<>(new Site());
		obstacles = problem.getObstacles();
		initial = problem.getInitialState();
		restart = problem.getInitialState();
		goal = problem.getGoalState();
		subgoal = problem.getGoalState();
		jointnumber = problem.getJointCount();
		pacecount =0 ;
		test = new Tester();
		gripper = problem.getInitialState().hasGripper();
		queue = new PriorityQueue<>(new Node());
		path =new ArrayList<String>();
		map = new ArrayList<Point2D>();
		expanded = new HashSet<Point2D>();
		
		
		sites = new ArrayList<Point2D>();
		
		for(int i = 0;i < obstacles.size()-1;i++){
			for(int j = i+1; j<obstacles.size();j++){
				double x = (obstacles.get(i).getRect().getCenterX()+obstacles.get(j).getRect().getCenterX())/2;
				double y = (obstacles.get(i).getRect().getCenterY()+obstacles.get(j).getRect().getCenterY())/2;
				Point2D point = new Point2D.Double();
				point.setLocation(x, y);
				sites.add(point);			
			}
		}
		for(int i = 0;i < obstacles.size();i++){
			if(obstacles.get(i).getRect().getMinY()!=0){
				double x = obstacles.get(i).getRect().getCenterX();
				double y = obstacles.get(i).getRect().getMinY()/2;
				Point2D point = new Point2D.Double();
				point.setLocation(x, y);
				sites.add(point);
			}
			if(obstacles.get(i).getRect().getMaxY()!=1){
				double x = obstacles.get(i).getRect().getCenterX();
				double y = (1+obstacles.get(i).getRect().getMinY())/2;
				Point2D point = new Point2D.Double();
				point.setLocation(x, y);
				sites.add(point);
			}
			if(obstacles.get(i).getRect().getMinX()!=0){
				double x = obstacles.get(i).getRect().getMinX()/2;
				double y = obstacles.get(i).getRect().getCenterY();
				Point2D point = new Point2D.Double();
				point.setLocation(x, y);
				sites.add(point);
			}
			if(obstacles.get(i).getRect().getMaxX()!=1){
				double x = (1+obstacles.get(i).getRect().getMaxX())/2;
				double y = obstacles.get(i).getRect().getCenterY();
				Point2D point = new Point2D.Double();
				point.setLocation(x, y);
				sites.add(point);
			}
		}
		System.out.println(sites);
		
	}
	
	public ArrayList<String> search(){
		Random  r = new Random();
		if(gripper != false){
			queue.add(new Node(initial,null));
			while(true){
				if(queue.size()>20){
					Node re = new Node(initial,null);
					queue.clear();
					queue.add(re);
					boolean check = false;
					if(sites.size()!=0){
					for(int i = 0; i< sites.size();i++){
						if(check == false){
							ArmConfig a = new ArmConfig(sites.get(i),goal.getJointAngles(),goal.getGripperLengths());
							if(!test.hasCollision(subgoal, obstacles)){
								subgoal = a;
								check = true;
								sites.remove(i);
							}
						}
					}
					}else{
						System.out.println("No available route.");
						return null;
					}
					
				}
				if(Math.abs(queue.peek().current.getBaseCenter().getX()-subgoal.getBaseCenter().getX())<0.01&&
					Math.abs(queue.peek().current.getBaseCenter().getY()-subgoal.getBaseCenter().getY())<0.001&&
						equalAngle(queue.peek().current,subgoal)){//rounded the value to reach final state
					if(subgoal.getBaseCenter().equals(goal.getBaseCenter())&&subgoal.getJointAngles().equals(goal.getJointAngles())){
						int count =0;
						Node node = new Node(queue.peek().current,queue.peek().parent);
						while(!node.current.equals(restart)){
						path.add(0,node.current.toString());
						node = new Node(node.parent.current,node.parent.parent);
						count +=1;
					}
					path.add(0,restart.toString());
					path.add(goal.toString());
					path.add(0,String.valueOf(count+1));
					System.out.println(path);
					System.out.println("x");
					return path;
					}else{
						initial = new ArmConfig(subgoal);
						subgoal = new ArmConfig(goal);
						System.out.println(subgoal.toString()+" "+goal.toString());
					}
				}				
				///create next random position
				double a = r.nextDouble()*0.001;
				double b = r.nextDouble()*0.001;
				double y = 0;
				double x =0;
				if(a>b){
					x = b;
					y = Math.sqrt(Math.pow(a,2)-Math.pow(x, 2));
				}else{
					x = a;
					y = Math.sqrt(Math.pow(b,2)-Math.pow(x, 2));
				}
				if(Math.sqrt(Math.pow(x, 2)+Math.pow(y,2))>0.001){
					System.out.println("1fdafadsfadsfadsfadsfadsfasdfad");
				}
				Point2D newcor = new Point2D.Double(x*pn()+queue.peek().current.getBaseCenter().getX(), y*pn()+queue.peek().current.getBaseCenter().getY());
			
				
				//create next random joints
				List<Double> newjoint = new ArrayList<Double>();
				for(int i = 0; i<queue.peek().current.getJointAngles().size();i++){
					double angle = pn()*r.nextDouble()*0.1*Math.PI/1800;
					newjoint.add(queue.peek().current.getJointAngles().get(i)+angle);
				}
				
				ArmConfig next = new ArmConfig(newcor,newjoint,goal.getGripperLengths());
				if(test.hasValidJointAngles(next)&&!test.hasCollision(next,obstacles)
						&&!test.hasSelfCollision(next)&&test.fitsBounds(next)){
					System.out.println(next.toString()); //two constraints left
					Node node = new Node(next,queue.peek());
					System.out.println(node.cost);
					queue.add(node);	
				}
				
			}
		}else{	
			ArrayList<Point2D> route = ramdonsearch(initial.getBaseCenter());
			queue.add(new Node(initial,null));
			int c = 1;
			int step = 0;
			subgoal = new ArmConfig(route.get(c),goal.getJointAngles());
			while(true){
//				if(step>10000){
//					step =0;
//					c =0;
//					Point2D point = new Point2D.Double();
//					point.setLocation(queue.peek().current.getBaseCenter().getX(),
//					queue.peek().current.getBaseCenter().getY());
//				//	System.out.println(queue.peek().current.getBaseCenter().toString());
//					route = ramdonsearch(point);
//					subgoal = new ArmConfig(route.get(c),goal.getJointAngles());
//					
//				}
				if(Math.abs(queue.peek().current.getBaseCenter().getX()-subgoal.getBaseCenter().getX())<0.001&&
					Math.abs(queue.peek().current.getBaseCenter().getY()-subgoal.getBaseCenter().getY())<0.001&&
						equalAngle(queue.peek().current,subgoal)){//rounded the value to reach final state
					if(subgoal.getBaseCenter().equals(goal.getBaseCenter())&&subgoal.getJointAngles().equals(goal.getJointAngles())){
						int count =0;
						Node node = new Node(queue.peek().current,queue.peek().parent);
						while(!node.current.equals(restart)){
						path.add(0,node.current.toString());
						node = new Node(node.parent.current,node.parent.parent);
						count +=1;
					}
					path.add(0,restart.toString());
					path.add(goal.toString());
					path.add(0,String.valueOf(count+1));
					System.out.println(path);
					System.out.println("x");
					return path;
					}else{
						c +=1;
						initial = new ArmConfig(subgoal);
					//	if(c==route.size()-1){
						subgoal = new ArmConfig(route.get(c),goal.getJointAngles());
					//	}else{
					//		ArrayList<Double> defaultan = new ArrayList<Double>(); 
					//		for(int i= 0; i<jointnumber;i++){
					//			defaultan.add(1.0);
					//		}
					//		subgoal = new ArmConfig(route.get(c),defaultan);
							//System.out.println(subgoal.toString()+" "+goal.toString());
					//	}
					}
				}				
				///create next random position
				double a = r.nextDouble()*0.001;
				double b = r.nextDouble()*0.001;
				double y = 0;
				double x =0;
				if(a>b){
					x = b;
					y = Math.sqrt(Math.pow(a,2)-Math.pow(x, 2));
				}else{
					x = a;
					y = Math.sqrt(Math.pow(b,2)-Math.pow(x, 2));
				}
				if(Math.sqrt(Math.pow(x, 2)+Math.pow(y,2))>0.001){
					System.out.println("1fdafadsfadsfadsfadsfadsfasdfad");
				}
				Point2D newcor = new Point2D.Double(x*pn()+queue.peek().current.getBaseCenter().getX(), y*pn()+queue.peek().current.getBaseCenter().getY());
			
				
				//create next random joints
				List<Double> newjoint = new ArrayList<Double>();
				for(int i = 0; i<queue.peek().current.getJointAngles().size();i++){
					double angle = pn()*r.nextDouble()*0.1*Math.PI/1800;
					newjoint.add(queue.peek().current.getJointAngles().get(i)+angle);
				}
				
				ArmConfig next = new ArmConfig(newcor,newjoint);
				if(test.hasValidJointAngles(next)&&!test.hasCollision(next,obstacles)
						&&!test.hasSelfCollision(next)&&test.fitsBounds(next)){
					System.out.println(next.toString()); //two constraints left
					Node node = new Node(next,queue.peek());
				//	System.out.println(node.cost);
					queue.add(node);	
				}
				//&&test.isValidStep(queue.peek().current,next)
				step +=1;
			}
		}	
	}
	
	public ArrayList<Point2D> ramdonsearch(Point2D reload){
		expanded.clear();
		ArrayList<Point2D> path = new ArrayList<Point2D>();
		Random  r = new Random();
		for(int i= 0; i<100;i++){
			double x= r.nextDouble();
			double y= r.nextDouble();
			Point2D point = new Point2D.Double();
			point.setLocation(x, y);
			boolean pass = true;
			for(int j = 0; j<obstacles.size();j++){
				if(obstacles.get(j).getRect().contains(point)){
					pass= false;
				}
			}
			if(pass ==true){
				map.add(point);
			}
			map.add(goal.getBaseCenter());
		}
		route.clear();
		route.add(new Site(reload,null,0.0));
		
		while(true){
			if(route.size()!=0){
		//	System.out.println(route.peek().current.getY());
			if(route.peek().current.getX()==goal.getBaseCenter().getX()&&
					route.peek().current.getY()==goal.getBaseCenter().getY()){	
				System.out.println("1");
				Site a = new Site();
				a = route.peek();
				
				while(!a.current.equals(reload)){
					path.add(0,a.parent.current);
					a = a.parent;
				}
				path.add(goal.getBaseCenter());
				//System.out.println("HI  e");
				return path;
			}
			if(!expanded.contains(route.peek().current)){
				expanded.add(route.peek().current);
				for(int i =0; i<map.size();i++){
					if(!hasCrossOb(map.get(i),route.peek().current)){
						route.add(new Site(map.get(i),route.peek(),map.get(i).distance(route.peek().current)+route.peek().cost));
						map.remove(i);
					}
				}
			}
			route.remove();
		//	System.out.println(goal.getBaseCenter().toString());
		//	System.out.println(route.peek());
			}else{
				return path;
			}
		}
		
	}
	
	private static boolean hasCrossOb(Point2D a, Point2D b){
		boolean result = false;
		Line2D line = new Line2D.Double();
		line.setLine(a,b);
		for(int i =0;i< obstacles.size();i++){
			if(obstacles.get(i).getRect().intersectsLine(line)){
				result = true;
			}
		}
		return result;
	}
	
	//TESTING heuristic
	private static double heuristic(ArmConfig current){
		double collision = 0;
		ArrayList<Point2D> points = new ArrayList<Point2D>();
		for(int i = 0; i<obstacles.size();i++){
			if(obstacles.get(i).getRect().getX()!=0&&obstacles.get(i).getRect().getY()!=1){
				Point2D point = new Point2D.Double(obstacles.get(i).getRect().getX(),obstacles.get(i).getRect().getY());
				points.add(point);
			}if(obstacles.get(i).getRect().getMinX()!=1&&obstacles.get(i).getRect().getY()!=1){
				Point2D point = new Point2D.Double(obstacles.get(i).getRect().getMinX(),obstacles.get(i).getRect().getY());
				points.add(point);
			}if(obstacles.get(i).getRect().getMinY()!=0&&obstacles.get(i).getRect().getMinX()!=1){
				Point2D point = new Point2D.Double(obstacles.get(i).getRect().getMinX(),obstacles.get(i).getRect().getMinY());
				points.add(point);	
			}if(obstacles.get(i).getRect().getMinY()!=0&&obstacles.get(i).getRect().getX()!=0){
				Point2D point = new Point2D.Double(obstacles.get(i).getRect().getX(),obstacles.get(i).getRect().getMinY());
				points.add(point);
			}
		}
		double line  = 0;
		double stop = 0;
		for(int i = 0; i< points.size();i++){
			line += points.get(i).distance(current.getBaseCenter());
			stop += points.get(0).distance(points.get(i));
		}
	
		//if(line<stop+0.1){
			return collision;
	//	}else{
	//		return line;
	//	}

	}
	
	private static double cost(ArmConfig current){
		double result = 0;
		Line2D line = new Line2D.Double();
		line.setLine(current.getBaseCenter(), subgoal.getBaseCenter());
		//System.out.println(line.getX1());
		for(int i = 0; i< obstacles.size();i++){
			if(line.intersects(obstacles.get(i).getRect())){
				
				Line2D line1 = new Line2D.Double();
				Line2D line2 = new Line2D.Double();
				Line2D line3 = new Line2D.Double();
				Line2D line4 = new Line2D.Double();
				double x1 = obstacles.get(i).getRect().getMaxX();
				double y1 = obstacles.get(i).getRect().getMaxY();
				double x2 = obstacles.get(i).getRect().getMinX();
			//	System.out.println(x2);
				double y2 = obstacles.get(i).getRect().getMinY();
				line1.setLine(x1,y1,x1,y2);
				line2.setLine(x1,y2,x2,y2);
				line3.setLine(x2,y2,x2,y1);
				line4.setLine(x2,y1,x1,y1);
			//	System.out.println(line1.getX1());
				Point2D a = new Point2D.Double();
				Point2D b = new Point2D.Double();
				a.setLocation(0, 0);
				b.setLocation(0, 0);
			//	System.out.println(a.getX());
				boolean first = false;
				boolean second = false;
				if(line.intersectsLine(line1)){
		
					if(first==false){
						first = true;
						a.setLocation(ipx(line,x1));
					}else{
						if(second == false){
							second = true;
							b.setLocation(ipx(line,x1));
						}
					}
				}
				if(line.intersectsLine(line2)){
				//	System.out.println("yy");
					if(first==false){
						first = true;
						a.setLocation(ipy(line,y2));
					}else{
						if(second == false){
							second = true;
							b.setLocation(ipy(line,y2));
						}
					}
				}
				if(line.intersectsLine(line3)){
				//	System.out.println("yy");
					if(first==false){
						first = true;
						a.setLocation(ipx(line,x2));
					}else{
						if(second == false){
							second = true;
							b.setLocation(ipx(line,x2));
						}
					}
				}
				if(line.intersectsLine(line4)){
				//	System.out.println("yy");
					if(first==false){
						first = true;
						a.setLocation(ipy(line,y1));
					}else{
						if(second == false){
							second = true;
							b.setLocation(ipy(line,y1));
						}
					}
				}
				
				result += a.distance(b);
			
			}
		}
	//	System.out.println(result+" "+current.getBaseCenter().toString());
		return result;
	}
	
	private static Point2D ipx(Line2D a, double x){
		double alpha1 = (a.getY2()-a.getY1())/(a.getX2()-a.getX1());
		double beta1 = a.getY1()-alpha1*a.getX1();
		
	//	System.out.println(alpha1+" "+beta2+" "+alpha2+" "+alpha1);
		double y = x*alpha1 +beta1;
		Point2D result = new Point2D.Double();
		result.setLocation(x, y);
	//	System.out.println(result.toString());
		return result;
	}
	
	private static Point2D ipy(Line2D a, double y){
		double alpha1 = (a.getY2()-a.getY1())/(a.getX2()-a.getX1());
		double beta1 = a.getY1()-alpha1*a.getX1();
	//	System.out.println(alpha1+" "+beta2+" "+alpha2+" "+alpha1);
		double x = (y - beta1)/alpha1;
		Point2D result = new Point2D.Double();
		result.setLocation(x, y);
	//	System.out.println(result.toString());
		return result;
	}
	
	//high dimension distance
	private static double he(ArmConfig current){
		double result = 0;
		double x = current.getBaseCenter().getX() - subgoal.getBaseCenter().getX();
		double y = current.getBaseCenter().getY() - subgoal.getBaseCenter().getY();
		result = Math.pow(x, 2)+Math.pow(y, 2);
		for(int i = 0; i< current.getJointAngles().size();i++){
			 double a = current.getJointAngles().get(i)-subgoal.getJointAngles().get(i);
			 result += Math.pow(a, 2);
		}
		result = Math.sqrt(result);
		return result;
	}
	
	private boolean equalAngle(ArmConfig state1, ArmConfig state2){
		for(int i =0;i<state1.getJointAngles().size();i++){
			if(round(state1.getJointAngles().get(i),1)!=round(state2.getJointAngles().get(i),1)){
				return false;
			}
		}
		return true;
	}
	
	public static double round(double value, int places) {
	    if (places < 0) throw new IllegalArgumentException();

	    long factor = (long) Math.pow(10, places);
	    value = value * factor;
	    long tmp = Math.round(value);
	    return (double) tmp / factor;
	}
	
	private int pn(){
		Random r = new Random();
		if(r.nextInt()%2==0){
			return -1;
		}else{
			return 1;
		}
	}
	
	public static class Node implements Comparator<Node>
	{	
		public ArmConfig current;
	    public Node parent;
	    public double cost;
	 
	 
	    public Node()
	    {
	 
	    }
	 
	    public Node(ArmConfig current, Node parent)
	    {
	    	this.current = current;
	        this.parent = parent;
	        this.cost = cost(current);
	       
	
	    }
	 
	    @Override
	    public int compare(Node node1, Node node2)
	    {
	        if (he(node1.current)+node1.cost< he(node2.current)+node2.cost)
	            return -1;
	        if (he(node1.current)+node1.cost> he(node2.current)+node2.cost)
	            return 1;
	        if (node1.current.maxAngleDiff(goal) < node2.current.maxAngleDiff(goal) )
	            return -1;
	        return 0;
	    }
	 
	    @Override
	    public boolean equals(Object obj)
	    {
	        if (obj instanceof Node)
	        {
	            Node node = (Node) obj;
	            if (this.current.equals(node.current))
	            {
	                return true;
	            }
	        }
	        return false;
	    }
	}
	
	public static class Site implements Comparator<Site>
	{	
		public Point2D current;
	    public Site parent;
	    public double cost;
	 
	 
	    public Site()
	    {
	 
	    }
	 
	    public Site(Point2D current, Site parent, Double cost)
	    {
	    	this.current = current;
	        this.parent = parent;
	        this.cost = cost;
	       
	
	    }
	    
	    @Override
	    public int compare(Site site1, Site site2)
	    {
	        if (site1.cost< site2.cost)
	            return -1;
	        if (site1.cost> site2.cost)
	            return 1;
	        
	        return 0;
	    }
	 
	    @Override
	    public boolean equals(Object obj)
	    {
	        if (obj instanceof Site)
	        {
	            Site site = (Site) obj;
	            if (this.current.equals(site.current))
	            {
	                return true;
	            }
	        }
	        return false;
	    }
	}
	
	public static void main(String[] args) throws IOException {
		ProblemSpec problem = new ProblemSpec();
		problem.loadProblem("1_joint.txt");
		Sampling s = new Sampling(problem);
		System.out.println(s.ramdonsearch(initial.getBaseCenter()).toString());
		ResultWriter.write("result.txt", s.search());
	}
}
