public class Explorer {

    /*********************************
     * GENERAL SETTINGS
     *********************************/
    // Index of the positions
    final int FAR_LEFT = 0, LEFT = 1, UP_LEFT = 2, UP_RIGHT = 3, RIGHT = 4, FAR_RIGHT = 5;
    // Angles of orientation of the sensors
    final double[] angles = new double[]{-90, -45, 20, -20, 45, 90};
    // Safety distance in mm
    final int SAFETY_DISTANCE = 100;
    // Max distance at which we add a point to the map (in mm)
    final int DRAW_DISTANCE = 400;
    // Error margin when moving along an object (in mm)
    final int ERROR_MARGIN = 20;
    // Distance at which we will move along an object (in mm)
    final int FOLLOW_DISTANCE = 150;

    /*********************************
     * DEPENDS ON THE ROBOT'S SPEED
     *********************************/
    // Tick needed per angle
    double TICK_PER_ANGLE;
    double ANGLE_PER_TICK;
    // Tick per mm
    double TICK_PER_MM;
    double MM_PER_TICK;
    /*********************************/



    /*********************************
     * SPECIFIC TO THE ROBOT
     *********************************/
    // Position
    double x, y;
    double angle;

    // Distances from the robot to the closest objects
    double[] distances;

    // If the robot will enter in contact with an object
    boolean[] contact;

    // Previous informations
    double[] prev_distances;
    boolean[] prev_contact;

    // Which side do we follow ? (-1 is none)
    int FOLLOWING_SIDE = -1;
    /*********************************/

    // Constructor
    Explorer(double TICK_PER_ANGLE, double TICK_PER_MM){
        distances = new double[6];
        contact = new boolean[6];

        prev_distances = new double[6];
        prev_contact = new boolean[6];

        this.TICK_PER_ANGLE = TICK_PER_ANGLE;
        this.TICK_PER_MM = TICK_PER_MM;

        this.ANGLE_PER_TICK = 1 / TICK_PER_ANGLE;
        this.MM_PER_TICK = 1 / TICK_PER_MM;

        // Starting position
        x = 0; y = 0;
        angle = 0;
    }

    void setPos(int x, int y){
        this.x = x;
        this.y = y;
    }

    void setAngle(int angle){
        this.angle = angle;
    }

    double getX(){
        return x;
    }

    double getY(){
        return y;
    }

    double getAngle(){
        return angle;
    }

    double[] getDistances(){
        return distances;
    }

    void setDistances(double[] distances){
        for(int i = 0; i < distances.length; i++) this.distances[i] = distances[i];
    }

    /**
     * Update the distance using the ultrasounds
     */
    void updateDistance(){

        // Copy the previous informations
        for(int i = 0; i < distances.length; i++){
            prev_distances[i] = distances[i];
            prev_contact[i] = contact[i];
        }

        // Récupérer les informations des capteurs

        for(int i = 0; i < distances.length; i++){
            if(distances[i] <= SAFETY_DISTANCE) contact[i] = true;
            else contact[i] = false;
        }

    }

    /**
     * Move the robot
     */
    void explore(){
        updateDistance();
        if(FOLLOWING_SIDE == -1) find();
        else follow();
    }

    /**
     * Move the robot "randomly" until finding an object to move along
     */
    void find(){
        // Nothing ahead, go forward
        if(!contact[UP_RIGHT] && !contact[UP_LEFT]) forward(50);
        // If there is something ahead, on the right, then we turn the robot to follow it
        else if(contact[UP_RIGHT]) left(90);
        // Same on the left
        else if(contact[UP_LEFT]) right(90);

        // If we're alongside an object on our left or right
        if(distances[FAR_LEFT] <= FOLLOW_DISTANCE + ERROR_MARGIN) FOLLOWING_SIDE = FAR_LEFT;
        else if (distances[FAR_RIGHT] <= FOLLOW_DISTANCE + ERROR_MARGIN) FOLLOWING_SIDE = FAR_RIGHT;
    }

    /**
     * Move the robot to follow an object on a certain side
     */
    void follow(){
        // If there is something on the side at a good distance, we continue to go forward
        if(isThereSomething(FOLLOWING_SIDE, FOLLOW_DISTANCE + ERROR_MARGIN)) {
            // If we're too close, we try to turn a bit
            if(distances[FOLLOWING_SIDE] <= FOLLOW_DISTANCE - ERROR_MARGIN) {
                if(FOLLOWING_SIDE == FAR_RIGHT) left(10);
                if(FOLLOWING_SIDE == FAR_LEFT) right(10);
            }
            forward(50);
        }
        else{
            // If there is no more something at the side, we try to turn and go forward
            if(FOLLOWING_SIDE == FAR_RIGHT) left(10);
            else right(10);
            forward(50);

            // Still nothing ? We stop to follow and try to explore something else
            if(!isThereSomething(FOLLOWING_SIDE, FOLLOW_DISTANCE + ERROR_MARGIN)) FOLLOWING_SIDE = -1;
        }
    }

    boolean isThereSomething(int direction, int distance){
        return distances[direction] <= distance;
    }

    // Direction
    void forward(){
        // Wheels go forward
    }
    void left(){
        // Wheels go left
    }
    void right(){
        // Wheels go right
    }
    void backward(){
        // Wheels go backward
    }

    /**
     * Update the position of the robot given the distance travelled
     * @param distance
     */
    void updatePos(double distance){
        double angle = 3.141592654 * this.angle / 180;
        x += Math.cos(angle) * distance;
        y += Math.sin(angle) * distance;
    }

    /**
     * Move forward for the specified distance (in mm)
     * @param distance
     */
    void forward(int distance){
        for(int i = 0; i < distance*TICK_PER_MM; i++){
            updatePos(MM_PER_TICK);
            forward();
        }
    }

    /**
     * Move left for the specified angle (in degree)
     * @param angle
     */
    void left(int angle){
        for(int i = 0; i < angle*TICK_PER_ANGLE; i++){
            addAngle(-ANGLE_PER_TICK);
            left();
        }
    }

    /**
     * Move right for the specified angle (in degree)
     * @param angle
     */
    void right(int angle){
        for(int i = 0; i < angle*TICK_PER_ANGLE; i++){
            addAngle(ANGLE_PER_TICK);
            right();
        }
    }

    /**
     * Move backward for the specified distance (in mm)
     * @param distance
     */
    void backward(int distance){
        for(int i = 0; i < distance*TICK_PER_MM; i++){
            updatePos(-MM_PER_TICK);
            backward();
        }
    }

    /**
     * Add the @param angle to the general angle. It will ensure that the general angle belong to [0, 360[ (in degree)
     * @param angle
     */
    void addAngle(double angle){
        this.angle += angle;
        if(this.angle >= 360) this.angle -= 360;
        if(this.angle < 0) this.angle += 360;
    }


    /**
     * Transform the robot's information into a JSON
     * @return
     */
    String toJSON(){
        StringBuilder builder = new StringBuilder();
        builder.append("{\n");
        builder.append("\"x\":" + x + ",\n");
        builder.append("\"y\":" + y + ",\n");
        builder.append("\"angle\":" + angle + ",\n");

        builder.append("\"distances\": [");
        for (int i = 0; i < distances.length-1; i++) builder.append(distances[i] + ", ");
        builder.append(distances[distances.length-1] + "],");

        builder.append("\n}");
        return builder.toString();
    }

    /**
     * USELESS FOR THE ROBOT ---- ONLY FOR TESTING PURPOSES
     * @param map
     */
    void addToTheMap(Map map){
        for(int i = 0; i < distances.length; i++){
            if(distances[i] <= DRAW_DISTANCE && distances[i] > 0) map.add(x, y, angle, angles[i], distances[i], Map.DEGREE);
        }
    }

}
