import javax.swing.*;

public class Main {

    private static Map map;
    private static MapGraphic map2D;
    private static JFrame frame;

    public static void main(String[] args) throws InterruptedException {
        map = new Map(5000, 5000, 2500, 2500);
        frame = new JFrame("TER MAP");
        map2D = new MapGraphic(map);

        frame.add(map2D);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);

        for(int i = 0; i < 5000; i ++) {
            map.add(0, i);
            Thread.sleep(1);
            frame.repaint();
        }
        for(int i = 0; i < 5000; i ++){
            map.add(i, 5000);
            Thread.sleep(1);
            frame.repaint();
        }

        for(int i = 0; i < 5000; i ++){
            map.add(5000, 5000-i);
            Thread.sleep(1);
            frame.repaint();
        }

        for(int i = 0; i < 5000; i ++){
            map.add(5000-i, 0);
            Thread.sleep(1);
            frame.repaint();
        }

        Mapping.saveJSON(map, "map.json");

    }



}
