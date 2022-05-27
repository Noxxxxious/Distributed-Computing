package lab9;
import java.util.ArrayList;

public class App {
    private static final ArrayList<Consumer> consumersList = new ArrayList<>();
    private static final ArrayList<Producer> producersList = new ArrayList<>();
    private static final Shop shop = new Shop();
    private static final int PRODUCER_CLIENT_COUNT = 3;

    public static void main(String[] args){
        shop.start();
        for(int i = 0; i < PRODUCER_CLIENT_COUNT; i++){
            producersList.add(new Producer(i, shop));
            consumersList.add(new Consumer(i, shop));

            producersList.get(i).start();
            consumersList.get(i).start();
        }
    }
}
