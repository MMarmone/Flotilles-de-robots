import org.json.JSONObject;

import javax.swing.*;
import java.io.*;
import java.net.*;

public class Main {
    static final int port = 8080;

    private static Map map;
    private static MapGraphic map2D;
    private static JFrame frame;
    private static Explorer robot;
    private static BufferedInputStream reader;

    /**
     * Ajoute les données à la map
     * @param data
     */
    private static void data(JSONObject data){
        JSONObject distances = data.getJSONObject("distances");
        double[] dist = new double[6];
        double x, y, angle;
        x = data.getDouble("x");
        y = data.getDouble("y");
        angle = data.getDouble("angle");
        for(int i = 0; i < 6; i++) dist[i] = distances.getDouble(""+i);

        robot.setPos(x, y);
        robot.setAngle(angle);
        robot.setDistances(dist);
        robot.addToTheMap(map);

        map.generateMap();
        frame.repaint();
    }


    private static String listen() throws IOException {
        int stream;
        byte[] b = new byte[16384];
        stream = reader.read(b);
        return new String(b, 0, stream);
    }

    public static void main(String[] args) throws Exception {
        ServerSocket s = new ServerSocket(port);
        Socket soc = s.accept();
        reader = new BufferedInputStream(soc.getInputStream());
        JSONObject json;

        map = new Map(500,500);
        frame = new JFrame("TER MAP");
        map2D = new MapGraphic(map, true);

        frame.add(map2D);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);

        robot = new Explorer();


        while (true) {
            String str = listen();
            System.out.println("STR = " + str);
            // Fin du parcours
            if (str.equals("END")) break;
            // Si la ligne n'est pas vide
            if(!str.isEmpty() && !str.equals("")) {
                json = new JSONObject(str);
                data(json);
            }
        }

        reader.close();
        soc.close();
    }
}