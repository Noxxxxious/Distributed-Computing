package lab9;

import java.util.HashMap;
import java.util.Map;

public class Shop extends Thread{
    private final Map<String, Integer> storage;
    private int product_count;
    private final int MAX_STORAGE = 100;

    public Shop(){
        this.storage = new HashMap<>();
        this.product_count = 0;
    }

    @Override
    public void run(){ ; }

    public synchronized int sell(String product, int quantity){
        if(!storage.containsKey(product)){
            return 0;
        }
        int sold_quantity;
        if(storage.get(product) >= quantity)
            sold_quantity = quantity;
        else
            sold_quantity = storage.get(product);
        removeProduct(product, sold_quantity);
        //printStorage();
        return sold_quantity;
    }

    public synchronized int collect(String product, int quantity){
        if(product_count == MAX_STORAGE){
            return 0;
        }
        int received_quantity;
        if(product_count + quantity <= MAX_STORAGE)
            received_quantity = quantity;
        else
            received_quantity = MAX_STORAGE - product_count;
        addProduct(product, received_quantity);
        //printStorage();
        return received_quantity;
    }

    private void addProduct(String product, int quantity){
        product_count += quantity;
        int new_quantity = quantity;
        if(storage.containsKey(product)){
            new_quantity += storage.get(product);
            storage.remove(product);
        }
        storage.put(product, new_quantity);
    }

    private void removeProduct(String product, int quantity){
        product_count -= quantity;
        int new_quantity = storage.get(product) - quantity;
        storage.remove(product);
        storage.put(product, new_quantity);
    }

    private void printStorage(){
        System.out.println("There are [" + product_count + "] products in shop storage");
        for(Map.Entry<String, Integer> entry : storage.entrySet()){
            System.out.println(entry.getKey() + "[" + entry.getValue() + "]");
        }
    }
}
