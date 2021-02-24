package bgu.spl.net.srv;

import bgu.spl.net.api.StompMessagingProtocol;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;

public class STOMPprotocol implements StompMessagingProtocol {
    private boolean shouldterminate = false;
    private int connectionId;
    private Connections<String> connections;
    private Maps maps = Maps.getInstance();
    private ConcurrentHashMap<String, String> headers = new ConcurrentHashMap<>();
    private AtomicInteger messageid = new AtomicInteger(1);

    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }

    @Override
    public void process(String message) {
        System.out.println(message);
        System.out.println("-----------------------------------------------------------------------------------------");
        User u = (User) maps.getConnectmap().get(connectionId);
        int i = 0;
        String command = "";
        String body = "";
        if (message.charAt(i) != '\u0000') {
            while (message.charAt(i) != '\n') {
                command += message.charAt(i);
                i++;
            }
            i++;
        }
        while (message.charAt(i) != '\u0000') {
            int counter = 0;
            String s1 = "";
            String s2 = "";
            while (message.charAt(i) != '\n') {
                if (message.charAt(i) == ':') {
                    counter++;
                    i++;
                }
                if (counter == 0) {
                    s1 += message.charAt(i);
                } else {
                    s2 += message.charAt(i);
                }
                i++;
            }
            i++;
            headers.put(s1, s2);
            if (message.charAt(i) == '\n') {
                while (message.charAt(i) != '\u0000') {
                    if (message.charAt(i) != '\n')
                        body += message.charAt(i);
                    i++;
                }
            }
        }
        switch (command) {
            case "CONNECT":
                connect();
                break;
            case "SUBSCRIBE":
                subscribe();
                break;
            case "UNSUBSCRIBE":
                unsubscribe();
                break;

            case "SEND":
                if (body.contains(u.getLogin() + " has added the book ")) {
                    String bookname = "";
                    int length = body.length() - 1;
                    while (body.charAt(length) != ' ') {
                        bookname = body.charAt(length) + bookname;
                        length--;
                    }
                    addbook(u, bookname);
                } else if (body.contains("Returning ")) {
                    String name = "";
                    int k = body.length() - 1;
                    while (body.charAt(k) != ' ') {
                        name = body.charAt(k) + name;
                        k--;
                    }

                    String bookname = "";
                    String help=body.substring(10,body.length());
                    int m=0;
                    while(help.charAt(m)!=' '){
                        bookname=bookname+help.charAt(m);
                    m++;
                    }
                    returnbook(bookname, name, u);
                }
                else if (body.equals("book status")) {
                    status(u);
                } else if (body.contains(" wish to borrow ")) {
                    String bookname = "";
                    int j = body.length() - 1;
                    while (body.charAt(j) != ' ') {
                        bookname = body.charAt(j) + bookname;
                        j--;
                    }
                    borrowbook(u, bookname);
                } else if (body.contains(" has ") && !body.contains(" has added")) {
                    String bookname = "";
                    int j = body.length() - 1;
                    while (body.charAt(j) != ' ') {
                        bookname = body.charAt(j) + bookname;
                        j--;
                    }
                    has(u, bookname);
                } else if (body.contains("Taking ") && body.contains(" from ")) {
                    String owner = "";
                    int k = body.length() - 1;
                    while (body.charAt(k) != ' ') {
                        owner = body.charAt(k) + owner;
                        k--;
                    }
                    String bookname = "";
                    int j = 0;
                    while (body.charAt(j) != ' ') {
                        j++;
                    }
                    j++;
                    while (body.charAt(j) != ' ') {
                        bookname += body.charAt(j);//TODO i think here its okay
                        j++;
                    }
                    take(u, bookname, owner);
                } else if (!body.isEmpty()) {
                    String login = "";
                    String bookname = "";
                    int j = 0;
                    while (j < body.length() && body.charAt(j) != ':') {
                        login += body.charAt(j);
                        j++;
                    }
                   j++;
                    while (j < body.length()) {
                        bookname += body.charAt(j);//TODO here okay i think
                        j++;
                    }
                    status1(login, bookname, u);
                }
                break;

            case "DISCONNECT":
                disconnect();
                break;
        }
    }

    @Override
    public boolean shouldTerminate() {
        return shouldterminate;
    }

    public void Terminate() {
        shouldterminate = true;
    }

    public void connect() {
        String id = headers.get("login");
        String passcode = headers.get("passcode");
        if (id != null && passcode != null) {
            User u = (User) maps.getUsers().get(id);
            if (u == null) {                    //first login
                u = new User(connectionId, id, passcode);
                maps.getUsers().put(u.getLogin(), u);
                maps.getConnectmap().put(u.getConnectionId(), u);
                connections.send(connectionId,
                        "CONNECTED" + '\n' +
                                "version:" + headers.get("accept-version") +
                                "\n\n" +
                                '\u0000');
            } else {
                if (passcode.equals(u.getPassword())) {           // right passcode
                    if (maps.getConnectmap().get(u.getConnectionId()) != null) {
                        connections.send(connectionId,
                                "ERROR" + "\n\n"
                                        + "User already logged in\n" +
                                        '\u0000');
                    } else {
                        maps.getConnectmap().put(connectionId, u);
                        connections.send(connectionId,
                                "CONNECTED\n" +
                                        "version:" + headers.get("accept-version") +
                                        "\n\n" + '\u0000');
                    }
                } else {                                                 //wrong passcode
                    connections.send(connectionId,
                            "ERROR" + "\n\n"
                                    + "Wrong password\n" +
                                    '\u0000');
                }
            }
        } else {
            connections.send(connectionId,
                    "ERROR\n\n"
                            + "please insert an id and password\n" +
                            '\u0000');
        }
    }

    public void subscribe() {
        String destination = headers.get("destination");
        String receipt = headers.get("receipt");
        String id = headers.get("id");
        if (destination != null && receipt != null && id != null) {
            User u = (User) maps.getConnectmap().get(connectionId);
            //---------------- filled the user's map with the subscription and the id ---------------------
            u.getSubscriptionsId().put(id, destination);
            if (maps.getTopics().get(destination) != null) { // topic exists.
                //------------------inserted to the topic's queue the user "u"-----------------------------
                ((ConcurrentLinkedQueue<User>) maps.getTopics().get(destination)).add(u);

                //----------inserted to the topic's queue the connectionhandler of the user's id-----------
                ConnectionHandler<String> toadd = (ConnectionHandler<String>) maps.getConnections().get(connectionId);
                ((ConcurrentLinkedQueue<ConnectionHandler<String>>) maps.getTopicsCH().get(destination)).add(toadd);

            } else {                                          // topic doesnt exist.

                //-----------------------------added a new queue of users-----------------------------------
                ConcurrentLinkedQueue<User> users = new ConcurrentLinkedQueue<>();
                maps.getTopics().put(destination, users);
                users.add(u);

                //-----------------------------added a new queue of connectionhandlers----------------------
                ConcurrentLinkedQueue<ConnectionHandler<String>> connectionHandlers = new ConcurrentLinkedQueue<>();
                ConnectionHandler<String> toadd = (ConnectionHandler<String>) maps.getConnections().get(connectionId);
                connectionHandlers.add(toadd);
                maps.getTopicsCH().put(destination, connectionHandlers);
            }
            connections.send(connectionId,
                    "RECEIPT\n" +
                            "receipt-id:" + receipt +
                            "\n\n" +
                            '\u0000');
        } else {
            connections.send(connectionId,
                    "ERROR\n\n" +
                            "headers are not good enough\n" +
                            '\u0000');
        }
    }

    public void unsubscribe() {
        String id = headers.get("receipt");
        String id2 = headers.get("id");
        User u = (User) maps.getConnectmap().get(connectionId);
        if (id != null) {
            String destination = u.getSubscriptionsId().get(id2);
            if (destination != null) {
                ((ConcurrentLinkedQueue<User>) maps.getTopics().get(destination)).remove(u);
                ConnectionHandler toremove = (ConnectionHandler) maps.getConnections().get(connectionId);
                ((ConcurrentLinkedQueue<ConnectionHandler>) maps.getTopicsCH().get(destination)).remove(toremove);

                connections.send(connectionId,
                        "RECEIPT\n" +
                                "receipt-id:" + id +
                                "\n\n" +
                                '\u0000');
            } else {
                connections.send(connectionId,
                        "ERROR\n\n" +
                                "you arent subscribed to this topic\n" +
                                '\u0000');
            }
        } else {
            connections.send(connectionId,
                    "ERROR\n\n" +
                            "headers arent good enough\n" +
                            '\u0000');
        }
    }

    public void disconnect() {
        String reciept = headers.get("receipt");
        if (reciept != null) {
            maps.getConnectmap().remove(connectionId);
            connections.send(connectionId,
                    "RECEIPT" + '\n' +
                            "receipt-id:" + reciept +
                            "\n\n" +
                            '\u0000');
        } else {
            connections.send(connectionId,
                    "ERROR\n\n" +
                            "headers arent good enough\n" +
                            '\u0000');
        }
        Terminate();
    }

    public void addbook(User u, String bookname) {
        String destination = headers.get("destination");
        String subscription = "";
        for (Map.Entry<String, String> a : u.getSubscriptionsId().entrySet()) {
            if (a.getValue().equals(destination)) {
                subscription = a.getKey();
            }
        }
        String s =
                "MESSAGE\n" +
                        "subscription:" + subscription + "\n" +
                        "Message-id:" + messageid.get() + "\n" +
                        "destination:" + destination + "\n\n" +
                        u.getLogin() + " has added the book " + bookname + '\n' +
                        '\u0000';
        connections.send(destination, s);
        messageid.incrementAndGet();
    }

    public void borrowbook(User u, String bookname) {
        String destination = headers.get("destination");
        String subscription = "";
        for (Map.Entry<String, String> a : u.getSubscriptionsId().entrySet()) {
            if (a.getValue().equals(destination)) {
                subscription = a.getKey();
            }
        }
        String s =
                "MESSAGE\n" +
                        "subscription:" + subscription + "\n" +
                        "Message-id:" + messageid.get() + "\n" +
                        "destination:" + destination + "\n\n" +
                        u.getLogin() + " wish to borrow " + bookname
                        + '\n' +
                        '\u0000';
        connections.send(headers.get("destination"), s);
        messageid.incrementAndGet();
    }

    public void has(User u, String bookname) {
        String destination = headers.get("destination");
        String subscription = "";
        for (Map.Entry<String, String> a : u.getSubscriptionsId().entrySet()) {
            if (a.getValue().equals(destination)) {
                subscription = a.getKey();
            }
        }
        String s =
                "MESSAGE\n" +
                        "subscription:" + subscription + "\n" +
                        "Message-id:" + messageid.get() + "\n" +
                        "destination:" + headers.get("destination") + "\n\n" +
                        u.getLogin() + " has " + bookname + "\n" + '\u0000';
        connections.send(headers.get("destination"), s);
        messageid.incrementAndGet();
    }

    public void take(User u, String bookname, String owner) {
        String destination = headers.get("destination");
        String subscription = "";
        for (Map.Entry<String, String> a : u.getSubscriptionsId().entrySet()) {
            if (a.getValue().equals(destination)) {
                subscription = a.getKey();
            }
        }
        String s =
                "MESSAGE\n" +
                        "subscription:" + subscription + "\n" +
                        "Message-id:" + messageid.get() + "\n" +
                        "destination:" + destination + "\n\n" +
                        "Taking " + bookname + " from " + owner
                        + "\n" + '\u0000';
        connections.send(destination, s);
        messageid.incrementAndGet();
    }


    public void returnbook(String bookname, String name, User u) {
        String destination = headers.get("destination");
        String subscription = "";
        for (Map.Entry<String, String> a : u.getSubscriptionsId().entrySet()) {
            if (a.getValue().equals(destination)) {
                subscription = a.getKey();
            }
        }

        String s =
                "MESSAGE\n" +
                        "subscription:" + subscription + "\n" +
                        "Message-id:" + messageid.get() + "\n" +
                        "destination:" + headers.get("destination") + "\n\n" +
                        "Returning " + bookname + " to " + name + "\n"
                        + '\u0000';
        connections.send(destination, s);
        messageid.incrementAndGet();
    }

    public void status(User u) {
        String destination = headers.get("destination");
        String subscription = "";
        for (Map.Entry<String, String> a : u.getSubscriptionsId().entrySet()) {
            if (a.getValue().equals(destination)) {
                subscription = a.getKey();
            }
        }
        String s =
                "MESSAGE\n" +
                        "subscription:" + subscription + "\n"
                        + "Message-id:" + messageid.get() + "\n"
                        + "destination:" + destination + "\n\n" +
                        "Book status\n" + '\u0000';
        connections.send(destination, s);
        messageid.incrementAndGet();
    }

    public void status1(String login, String bookname, User u) {
        String destination = headers.get("destination");
        String subscription = "";
        for (Map.Entry<String, String> a : u.getSubscriptionsId().entrySet()) {
            if (a.getValue().equals(destination)) {
                subscription = a.getKey();
            }
        }
        String s =
                "MESSAGE\n" +
                        "subscription:" + subscription + '\n' +
                        "Message-id:" + messageid + '\n' +
                        "destination:" + headers.get("destination") + "\n\n"
                        + login + ":" + bookname + '\n' + '\u0000';
        connections.send(headers.get("destination"), s);
        messageid.incrementAndGet();
    }
}