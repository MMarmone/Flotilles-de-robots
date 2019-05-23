public class Explorer {

    /*********************************
     * GENERAL SETTINGS
     *********************************/
    // Angles of orientation of the sensors
    double[] angles;
    double[] sensors;

    double DRAW_DISTANCE = 1000;

    /*********************************/

    /*********************************
     * SPECIFIC TO THE ROBOT
     *********************************/
    // Position
    double x, y;
    double angle;

    // Distances from the robot to the closest objects
    double[] distances;
    /*********************************/

    Explorer(){
        this(new double[]{-90, -30, 15, -15, 30, 90}, new double[]{75, 133, 143, 143, 133, 75});
    }

    // Constructor
    Explorer(double[] angles, double[] sensors){
        this.distances = new double[6];
        this.angles = angles;
        this.sensors = sensors;

        // Starting position
        x = 0; y = 0;
        angle = 0;
    }

    void setPos(double x, double y){
        this.x = x;
        this.y = y;
    }

    void setAngles(double[] angles){
        this.angles = angles;
    }

    void setAngle(double angle){
        this.angle = angle;
    }

    void setDistances(double[] distances){
        for(int i = 0; i < distances.length; i++) this.distances[i] = distances[i];
    }

    /**
     * USELESS FOR THE ROBOT ---- ONLY FOR TESTING PURPOSES
     * @param map
     */
    void addToTheMap(Map map){
        /*for(int i = 0; i < distances.length; i++){
            if(distances[i] <= DRAW_DISTANCE && distances[i] > 0) map.add(x, y, angle, angles[i], distances[i]+sensors[i], i, Map.DEGREE);
        }*/
        map.add(x,y);
    }

}
