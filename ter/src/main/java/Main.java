import org.json.JSONObject;

import javax.swing.*;
import java.io.*;
import java.net.*;

public class Main {
    private static Map map;
    private static MapGraphic map2D;
    private static JFrame frame;
    private static Explorer robot;

    /**
     * Ajoute les données à la map
     * @param data
     */
    private static void data(JSONObject data){
        if(data.has("x") && data.has("y") && data.has("angle") && data.has("distances")) {
            JSONObject distances = data.getJSONObject("distances");
            double[] dist = new double[6];
            double x, y, angle;
            x = data.getDouble("x");
            y = data.getDouble("y");
            angle = data.getDouble("angle");
            for (int i = 0; i < 6; i++) dist[i] = distances.getDouble("" + i);

            robot.setPos(x, y);
            robot.setAngle(angle);
            robot.setDistances(dist);
            robot.addToTheMap(map);
        }
    }

    public static void main(String[] args) {
        map = new Map(500,500);
        frame = new JFrame("TER MAP");
        map2D = new MapGraphic(map, true);

        frame.add(map2D);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);

        robot = new Explorer();

        Server server = new Server(8080, 1);
        server.enablePrinting();
        server.accept();
        server.send(0,"cc sa va;");
        JSONObject json;

        while (!server.finished()) {
            int speaker = server.listen();
            while (server.unreadMessage(speaker)){
                json = server.getLastMessage(speaker);
                data(json);
            }
            map.generateMap();
            frame.repaint();
        }
    }
}