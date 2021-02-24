package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;


//-------------------------------singleton class that holds my maps-------------------------------------------
public class Maps<T> {
    //-------------------------------------------by user id---------------------------------------------------------
    private ConcurrentHashMap<Integer, User> connectmap = new ConcurrentHashMap<>();
    //-------------------------------by username-----------------------------------------------------------------
    private ConcurrentHashMap<String, User> Users = new ConcurrentHashMap<>();
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<User>> topics = new ConcurrentHashMap<>();
    private ConcurrentHashMap<Integer , ConnectionHandler<T>>connections = new ConcurrentHashMap<>();
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<ConnectionHandler<T>>> topicsCH = new ConcurrentHashMap<>();

    //--------------------------------------------------------------------------------------------------------------
    private static class MapsSingelton {
        private static Maps mapHolder = new Maps();
    }

    public static Maps getInstance() {
        return MapsSingelton.mapHolder;
    }

    //----------------------------getters--------------------------------------
    public ConcurrentHashMap<Integer, User> getConnectmap() {
        return connectmap;
    }

    public ConcurrentHashMap<String, User> getUsers() {
        return Users;
    }

    public ConcurrentHashMap<String, ConcurrentLinkedQueue<User>> getTopics() {
        return topics;
    }

    public ConcurrentHashMap<Integer, ConnectionHandler<T>> getConnections() {
        return connections;
    }

    public ConcurrentHashMap<String, ConcurrentLinkedQueue<ConnectionHandler<T>>> getTopicsCH() {
        return topicsCH;
    }
}
