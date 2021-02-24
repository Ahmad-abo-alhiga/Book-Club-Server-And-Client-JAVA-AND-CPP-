package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.STOMPconnections;
import bgu.spl.net.srv.STOMPencoderdecoder;
import bgu.spl.net.srv.STOMPprotocol;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        args = new String[1];
        args[0] = "7777";
//        Server.threadPerClient(Integer.parseInt(args[0]),
//                STOMPprotocol::new,
//                STOMPencoderdecoder::new,
//                new STOMPconnections<>()).serve();

        Server.reactor(Runtime.getRuntime().availableProcessors(),
               // Integer.parseInt(args[0]),
                7777,
                STOMPprotocol::new,
                STOMPencoderdecoder::new,
                new STOMPconnections<>()).serve();
    }
}
