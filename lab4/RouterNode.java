import javax.swing.*;        

public class RouterNode {
    private int myID;
    private GuiTextArea myGUI;
    private RouterSimulator sim;
    private int[] costs = new int[RouterSimulator.NUM_NODES];
    private int[][] routingTable = new int[2][RouterSimulator.NUM_NODES];
    private RouterPacket[] buffer= new RouterPacket[RouterSimulator.NUM_NODES];
    private boolean[] bitMap = new boolean[RouterSimulator.NUM_NODES];
    private boolean poisonEnabled = false;

    //--------------------------------------------------
    public RouterNode(int ID, RouterSimulator sim, int[] costs) {
	myID = ID;
	this.sim = sim;
	myGUI =new GuiTextArea("  Output window for Router #"+ ID + "  ");
	System.arraycopy(costs, 0, this.costs, 0, RouterSimulator.NUM_NODES);
	
	//Modified code
	//Initialize recv buffer and bitmap
	for(int i=0; i<RouterSimulator.NUM_NODES;i++){
	    bitMap[i] = false;
	    buffer[i] = null;
	}


	//Init routing table
	resetRoutingTable();

	//Send our distance vector to neighboor
	sendDistanceVectorToNeighbours();

    }
    
    //--------------------------------------------------
    public void recvUpdate(RouterPacket pkt) {
	

	//PoisonReverse
	if(poisonEnabled)
	    {
		//TODO
		buffer[pkt.source] = pkt;
	    }


	//Check to see if a change have been made in the routing table
	boolean changed = updateRoutingTable(pkt);


	//If table was updated, send the new version to other neighbours
	if(changed){
	    sendDistanceVectorToNeighbours();
	}
    }
   
    //update routing table, return true if the table was updated
    private boolean updateRoutingTable(RouterPacket pkt){
	boolean output = false;
	//iterate through the new packet
	for(int i=0;i<RouterSimulator.NUM_NODES;i++){
	    //skip self
	    if(i==myID){
		continue;
	    }
	    //skip source node
	    else if(pkt.sourceid==i){
		continue;
	    }
	    else{
		//If the shorter path found, update
		int pathcost = pkt.mincost[i] +routingTable[0][pkt.sourceid];
		if(routingTable[0][i] > pathcost){
		    routingTable[0][i]=pathcost;
		    routingTable[1][i]=routingTable[1][pkt.sourceid];
		    output=true;
		}
	    }
	}
	return output;
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
	myGUI.println("Distancetable: WHAT IS THIS?");
	
	//TODO - FORMAT PROPERLY MAYBE
	//Print routingTable
	myGUI.println(formatRoutingTable());


    }

    private void poisenedReverse()
    {

    }

    private String formatRoutingTable()
    {
	String output = "";
	String node = "";
	output += "Our distance vector and routes:\n";


	output += "Node  | Cost | Route \n";

	for(int j = 0; j<RouterSimulator.NUM_NODES; j++)
	    {
		node = "";
		if(routingTable[1][j] == -1)
		    node = "-\n";
		else if(routingTable[1][j] == -2)
		    node = "?\n";
		else
		    {
			node = routingTable[1][j] + "\n";
		    }
		output += j + "     | " + routingTable[0][j] + "    | " + node;
	    }

	return output;
    }

    //--------------------------------------------------
    public void updateLinkCost(int dest, int newcost) {
	costs[dest] = newcost;

	//Check to see if a change have been made in the routing table
	boolean changed = updateRoutingTable(pkt);

	//If table was updated, send the new version to other neighbours
	if(changed){
	    sendDistanceVectorToNeighbours();
	}
    }


    //Send our distance vector to neighboor
    private void sendDistanceVectorToNeighbours()
    {

	int[] mincost = new int[RouterSimulator.NUM_NODES];


	//Update our routing package from our table
	for(int j = 0; j<RouterSimulator.NUM_NODES; j++)
	    {
		mincost[j] = routingTable[0][j];
	    }

	//Send our routing table to all neighbours
	for(int node = 0; node<RouterSimulator.NUM_NODES; node++)
	    {
		//Only send to neighbours
		if(costs[node]==0 || costs[node]==999)
		    {
			continue;
		    }
		else 
		    {
			//Send new router packet
			RouterPacket pkt = new RouterPacket(myID, node, mincost);
			sendUpdate(pkt);
		    }
	    }

    }



    // Reset Routing Table
    private void resetRoutingTable()
    {
	//Reset routing table
	for(int j = 0; j<RouterSimulator.NUM_NODES; j++)
	    {
		if(j == myID)
		    {
			//Init self
			routingTable[0][j] = 0;
			routingTable[1][j] = -1; //Path to self
		    }
		else
		    {
			//Init to infinity
			if(costs[j]==999){
			    routingTable[1][j] = -2; //Unknown path
			}
			else{
			    routingTable[1][j] = j; //known path
			}
			routingTable[0][j] = costs[j];

		    }
	    }
    }

}
