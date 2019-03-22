import javax.swing.*;

public class Main {

    private static Map map;
    private static MapGraphic map2D;
    private static JFrame frame;

    public static void main(String[] args) throws InterruptedException {
        map = new Map(500,500);
        frame = new JFrame("TER MAP");
        map2D = new MapGraphic(map, true);

        frame.add(map2D);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);

        Explorer robot = new Explorer(1, 1);
        robot.setDistances(new double[]{0, 0, 0, 0, 0, 0.1});

        robot.setPos(10,10);
        robot.setAngle(20);


        for(int turn = 0; turn < 4; turn++) {
            for (int i = 0; i < 200; i++) {
                robot.addToTheMap(map);
                robot.forward(6);
                map.generateMap();
                frame.repaint();
                Thread.sleep(20);
            }
            robot.right(90);
        }

    }



}
