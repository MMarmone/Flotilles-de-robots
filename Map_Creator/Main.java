import javax.swing.*;

public class Main {

    private static Map map;
    private static MapGraphic map2D;
    private static JFrame frame;

    public static void main(String[] args) {
        map = new Map(500,500);
        frame = new JFrame("TER MAP");
        map2D = new MapGraphic(map);

        frame.add(map2D);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);

        Explorer robot = new Explorer(10, 10);
        robot.setDistances(new double[]{0, 0, 0, 0, 0, 10});

        robot.setAngle(10);

        for(int turn = 0; turn < 4; turn++) {
            for (int i = 0; i < 6000; i++) {
                robot.addToTheMap(map);
                robot.forward(1);
                map.generateMap();
                frame.repaint();
            }
            robot.right(90);
        }
    }



}
