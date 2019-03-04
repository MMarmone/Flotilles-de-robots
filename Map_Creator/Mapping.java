import java.io.FileWriter;
import java.io.IOException;

public class Mapping {

    /**
     * Convert a map to a string, in a JSON format.
     * @param map
     * @return
     */
    public static String toString(Map map) {
        int[][] stringMap = map.getMap();
        int width = stringMap[0].length;
        int height = stringMap.length;

        StringBuilder builder = new StringBuilder();
        builder.append("{\n");
        builder.append("\"width\":" + width + ",\n");
        builder.append("\"height\":" + height + ",\n");
        builder.append("\"map\":{\n");
        for (int h = 0; h < height; h++) {
            builder.append("\t\"" + h + "\":[");
            for (int w = 0; w < width - 1; w++) {
                builder.append(stringMap[w][h] + ",");
            }
            builder.append(stringMap[width-1][h] + "],\n");
        }
        builder.append("\t}\n}");
        return builder.toString();
    }

    /**
     * Save the map under a file.
     * The map is saved as a JSON, using the toString() method.
     * @param map
     * @param filename
     */
    public static void saveJSON(Map map, String filename){
        try {
            FileWriter writer = new FileWriter(filename);
            String mapString = toString(map);
            writer.write(mapString);
            writer.flush();
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
