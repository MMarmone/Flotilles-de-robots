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

        /*
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
        */


        Explorer robot = new Explorer(1, 1);
        robot.setDistances(new double[]{0, 0, 0, 0, 0, 10});

        robot.setAngle(6);

        for(int turn = 0; turn < 4; turn++) {
            for (int i = 0; i < 490; i++) {
                robot.addToTheMap(map);
                robot.forward(10);
                System.out.println("(" + robot.getX() + ", " + robot.getY() + ")");
                Thread.sleep(1);
                frame.repaint();
            }
            robot.right(90);
        }

    }



}
