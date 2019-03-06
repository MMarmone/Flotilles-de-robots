public class Main {

    public static void main(String[] args){
        Map map = new Map(5000, 5000, 2500, 2500);
        for(int i = 0; i < 5000; i ++) map.add(0, i);
        for(int i = 0; i < 5000; i ++) map.add(i, 5000);
        for(int i = 0; i < 5000; i ++) map.add(5000, 5000-i);
        for(int i = 0; i < 5000; i ++) map.add(5000-i, 0);

        Mapping.saveJSON(map, "map.json");
    }

}
