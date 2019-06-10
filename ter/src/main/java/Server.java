import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

    private int nRobots;
    private int capacity;

    private BufferedReader[] readers;
    private PrintWriter[] writers;
    private ServerSocket server;
    private Socket[] robots;
    private JSONObject[][] messages;
    private int[] nMessage;

    private boolean[] finished;

    private boolean PRINT_MESSAGE;

    public Server(int port, int capacity){
        this.capacity = capacity;
        this.nRobots = 0;
        try {
            this.server = new ServerSocket(port);
        } catch (IOException e) {
            e.printStackTrace();
        }
        this.readers = new BufferedReader[capacity];
        this.writers = new PrintWriter[capacity];
        this.robots = new Socket[capacity];
        this.messages = new JSONObject[capacity][10];
        this.nMessage = new int[capacity];
        this.finished = new boolean[capacity];
        for(int i = 0; i < capacity; i++) finished[i] = false;
    }

    public void enablePrinting(){
        PRINT_MESSAGE = true;
    }

    public void disablePrinting(){
        PRINT_MESSAGE = false;
    }

    public void accept(){
        while(nRobots < capacity){
            try {
                robots[capacity] = server.accept();
                readers[capacity] = new BufferedReader(new InputStreamReader(robots[capacity].getInputStream()));
                writers[capacity] = new PrintWriter(robots[capacity].getOutputStream(), true);
                capacity++;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void close(){
        try {
            server.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void close(int robot){
        try {
            robots[robot].close();
            finished[robot] = true;
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void send(int robot, String message){
        writers[robot].println(message);
    }

    public String listen(int robot){
        try {
            return readers[robot].readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public int listen(){
        String line = null;
        int robot = 0;
        while(line == null){
            while(finished[robot]) robot++;
            if(robot >= capacity) robot = 0;
            line = listen(robot);
            robot++;
        }
        robot--;
        readMessage(line, robot);
        return robot;
    }

    public JSONObject getLastMessage(int robot){
        JSONObject message = messages[robot][nMessage[robot]-1];
        nMessage[robot]--;
        return message;
    }

    public boolean unreadMessage(int robot){
        return nMessage[robot] > 0;
    }

    public void readMessage(String message, int robot){
        String[] splitted = message.split(";");
        JSONObject json;
        for(int i = 0; i < splitted.length; i++){
            try{
                if(PRINT_MESSAGE) System.out.println(splitted[i]);

                if(splitted[i].equals("END")) close(robot);
                else {
                    json = new JSONObject(splitted[i]);
                    if(nMessage[robot] < 10) messages[robot][nMessage[robot]++] = json;
                }
            } catch (JSONException e){
                // pass
            }
        }
    }

    public boolean finished(){
        for(int i = 0; i < nRobots; i++) if(!finished[i]) return false;
        return true;
    }

}
