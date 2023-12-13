OscP5 oscP5;
NetAddress myRemoteLocation;

void setupOSC() { 
    oscP5 = new OscP5(this,12001);
    myRemoteLocation = new NetAddress("127.0.0.1",8888);
}

void mouseDragged() { 
    
    OscMessage myMessage = new OscMessage("/test");
    
    myMessage.add(mouseX); /* add an int to the osc message */
    oscP5.send(myMessage, myRemoteLocation); 
    
    println("I have send a message " + mouseX );
}