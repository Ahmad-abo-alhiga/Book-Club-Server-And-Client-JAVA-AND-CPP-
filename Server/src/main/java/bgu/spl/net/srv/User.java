package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;

public class User {
    private int connectionId;
    private String login;
    private String password;
    private boolean connected;
    private ConcurrentHashMap<String, String> subscriptionsId = new ConcurrentHashMap<>(); // id and the topic

    public User(int CHid, String name, String password) {
        this.connectionId = CHid;
        this.login = name;
        this.password = password;
        connected = false;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public String getLogin() {
        return login;
    }

    public String getPassword() {
        return password;
    }

    public boolean isConnected() {
        return connected;
    }

    public ConcurrentHashMap<String, String> getSubscriptionsId() {
        return subscriptionsId;
    }
}
