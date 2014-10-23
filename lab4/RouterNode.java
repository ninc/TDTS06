import javax.swing.*;   
import java.util.Arrays;

public class RouterNode {
    private int myID;
    private GuiTextArea myGUI;
    private RouterSimulator sim;
    private int[] costs = new int[RouterSimulator.NUM_NODES];
    private boolean poisonEnabled = true;
    
    private int[][] myNeighboursDistTable = new int[RouterSimulator.NUM_NODES][RouterSimulator.NUM_NODES];
    private int[] route = new int[RouterSimulator.NUM_NODES];
    private boolean[] neighbours = new boolean[RouterSimulator.NUM_NODES];
    private int[] myDistTable = new int[RouterSimulator.NUM_NODES];


    //--------------------------------------------------
    public RouterNode(int ID, RouterSimulator sim, int[] costs) {
	myID = ID;
	this.sim = sim;
	myGUI =new GuiTextArea("  Output window for Router #"+ ID + "  ");
	System.arraycopy(costs, 0, this.costs, 0, RouterSimulator.NUM_NODES);

	// Init tables & print
	myDistTable = costs;
	initTables();
	printDistanceTable();
	
	//Send our distance vector to neighboor
	send();
    }
    
    //Set all distances to infinity
    private void initTables(){
	for (int i = 0; i < RouterSimulator.NUM_NODES; i++) {
            if (myID == i) {
                myNeighboursDistTable[i] = costs;
            } else {
                int[] tmp = new int[RouterSimulator.NUM_NODES];
                Arrays.fill(tmp, RouterSimulator.INFINITY);
                myNeighboursDistTable[i] = tmp;
            }
            if (costs[i] == RouterSimulator.INFINITY) {
                neighbours[i] = false;
                route[i] = -1;
            } else if (i != myID) {
                neighbours[i] = true;
                route[i] = i;
            }
	}
    }


    //--------------------------------------------------
    public void recvUpdate(RouterPacket pkt) {
	if(pkt.destid == myID){
	    myNeighboursDistTable[pkt.sourceid] = pkt.mincost;
	    updateRoutingTable();
	}

    }
   
    //update routing table, send packet if updated
    private void updateRoutingTable(){
	int[] current = new int[RouterSimulator.NUM_NODES];
	Arrays.fill(current, RouterSimulator.INFINITY);
	
	//iterate through neighbours distance tables
	
	for(int i=0;i<myNeighboursDistTable[myID].length;i++){
	    //if shortest path goes through current node
	    if(i==myID){
		current[i] = myDistTable[i];
		route[i] = i;
		continue;
	    }
	    //iterate through neighbours
	    for(int j = 0; j < neighbours.length; j++){
		//if a neighbour exist
		if(neighbours[j]){
		    int cost = costs[j] + myNeighboursDistTable[j][i];
		    //if shorter path found
		    if(cost < current[i]){
			current[i] = cost;
			route[i] = j;
      		    }
		}
		
	    }
	}
	//if better path found, send new routingtable to neighbours
	if(!Arrays.equals(current, myDistTable)){
	    System.arraycopy(current, 0, myDistTable, 0, RouterSimulator.NUM_NODES);
	    send();
	}
    }

    // Sends the nodes routing table to its neighbours
    private void send()
    {

	RouterPacket pkt;
        if(poisonEnabled){
	    // Create a false distance table
            int[] tmpDistTable = new int[RouterSimulator.NUM_NODES];
            System.arraycopy(myDistTable,0,tmpDistTable,0,RouterSimulator.NUM_NODES);
            for(int i = 0;i<neighbours.length;i++){
                if(neighbours[i]){
		    //Poisoned reverse
                    for(int j = 0;j<route.length;j++){
			// All routes that goes through my node, set to infinity
                        if(route[j] == i && j!=i){
                            tmpDistTable[j] = RouterSimulator.INFINITY;
                        }
                    }
		    // Send new packet
                    pkt = new RouterPacket(myID,i,tmpDistTable);
                    sendUpdate(pkt);
                }
            }
        } else {
            for (int i = 0; i < neighbours.length; i++) {
                if (neighbours[i]) {
		    // Send new packet
                    pkt = new RouterPacket(myID, i, myDistTable);
                    sendUpdate(pkt);
                }
            }
        }


    }


    //--------------------------------------------------
    private void sendUpdate(RouterPacket pkt) {
	sim.toLayer2(pkt);
    }
  

    //--------------------------------------------------
    public void printDistanceTable() {
	myGUI.println("Current table for " + myID +
		      "  at time " + sim.getClocktime());

	//TODO - distancetable?????
	printNeighbourTable();
	
	//TODO - FORMAT PROPERLY MAYBE
	printMyRoutingTable();


    }

    // print neighbour tables according to specification
    private void printNeighbourTable()
    {

        myGUI.println("Neighbour Distance Table");
        String str = F.format("dst    |", 15);
        for (int i = 0; i < RouterSimulator.NUM_NODES; i++) {
            str += (F.format(i, 15));
        }
        myGUI.println(str);
        for (int i = 0; i < str.length(); i++) {
            myGUI.print("-");
        }
        myGUI.println();
        for (int i = 0; i < RouterSimulator.NUM_NODES; i++) {
            str = F.format("nbr  " + i + " |", 15);
            for (int j = 0; j < RouterSimulator.NUM_NODES; j++) {
                str += (F.format(myNeighboursDistTable[i][j], 15));
            }
            if (i != myID && neighbours[i])
                myGUI.println(str);
        }
        myGUI.println();
    }

    // print my routing table according to specification
    private void printMyRoutingTable()
    {
	String str;
	myGUI.println("My Distance table and routes");
        str = F.format("dst    |", 15);
        for (int i = 0; i < RouterSimulator.NUM_NODES; i++) {
            str += (F.format(i, 15));
        }
        myGUI.println(str);
        for (int i = 0; i < str.length(); i++) {
            myGUI.print("-");
        }
        str = F.format("cost  |", 15);
        for (int i = 0; i < RouterSimulator.NUM_NODES; i++) {
            str += F.format(myDistTable[i], 15);
        }
        myGUI.println();
        myGUI.println(str);
        str = F.format("route |", 15);
        for (int i = 0; i < RouterSimulator.NUM_NODES; i++) {
            str += F.format(route[i], 15);
        }
        myGUI.println(str);
        myGUI.println();
        myGUI.println("--------------------------------------------");
        myGUI.println();

    }
    
    //--------------------------------------------------
    public void updateLinkCost(int dest, int newcost) {
	costs[dest] = newcost;
	updateRoutingTable();
    }
	
}
    
