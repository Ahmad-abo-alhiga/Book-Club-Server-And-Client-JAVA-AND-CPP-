package bgu.spl.net.srv;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class STOMPconnections<T> implements Connections<T> {
    Maps map = Maps.getInstance();

    @Override
    public boolean send(int connectionId, T msg) {
        System.out.println("the msg:\n" + msg + "\nwas sent to " + connectionId + '\n');
        ConcurrentHashMap<Integer, ConnectionHandler<String>> clients = map.getConnections();
        ConnectionHandler<String> client = clients.get(connectionId);
        if (client != null)
            client.send((String) msg);
        return true;
    }

    @Override
    public void send(String channel, T msg) {
        System.out.println("the msg:\n" + msg + "\nwas sent to " + channel + '\n');
        ConcurrentHashMap<String, ConcurrentLinkedQueue<ConnectionHandler<String>>> topics = map.getTopicsCH();
        ConcurrentLinkedQueue<ConnectionHandler<String>> subscribed = topics.get(channel);
        if (subscribed != null) {
            for (ConnectionHandler<String> a : subscribed) {
                Integer id = a.getid();
                if (map.getConnectmap().get(id) != null)
                    a.send((String) msg);
            }
        }
    }

    @Override
    public void disconnect(int connectionId) {
        ConcurrentHashMap<Integer, ConnectionHandler<String>> clients = map.getConnections();
        ConcurrentHashMap<String, ConcurrentLinkedQueue<ConnectionHandler<String>>> topics = map.getTopicsCH();
        ConnectionHandler<String> x = clients.remove(connectionId);
        for (Map.Entry<String, ConcurrentLinkedQueue<ConnectionHandler<String>>> a : topics.entrySet()) {
            ConcurrentLinkedQueue<ConnectionHandler<String>> list = a.getValue();
            synchronized (list) {
                list.removeIf(x::equals);
            }
        }
    }
}
