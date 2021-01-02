//The following three lines define the WiFi mesh name, password and port. These will work as is or they can be changed especially if someone very nearby might be using this same software in their wood shop.
// In rare cases where you have tow dust collection systems in the same shop or one dust collector system and a shop vac for another you would need to change the MESH_PREFIX when uploading code for each system.

#define   MESH_PREFIX     "blastGates"
#define   MESH_PASSWORD   "somethingSneaky***"
#define   MESH_PORT       5555

// The following are set up as String objects because that is what is returned in the msg sent and received in painlessMesh.
// defining them once here makes it easier to compare messages received by the dust collector or other blast gates

String gCloseOther = String("Close Other Gates"); //The message to be sent to other blast gates from a blast gate that is opening. A toggle button on a dust collector will also send this message and all gates will be closed.
String gVacOff = String("Vac Off"); // Turn the shop vac or dust collector off. Sent from a remote device/button configured to turn a dust collector off. 

// gGateStatus tracks if a gate is open or closed. This is pretty easy with a servo or relay (pneumatics) but less so with a stepper where the postion at startup is not known.
#define GATE_OPEN   1
#define GATE_CLOSED 0
boolean gGateStatus = GATE_CLOSED;
