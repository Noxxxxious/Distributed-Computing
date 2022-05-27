package lab9;

import java.util.Random;

public class Producer extends Thread{
    private final int producerID;
    private final Shop shop;

    public Producer(int producerID, Shop shop){
        this.producerID = producerID;
        this.shop = shop;
    }
    @Override
    public void run(){
        while(true){
            try{
                sleep((long) (Math.random() * 6000));
                String product = randomProduct();
                int quantity = (int)(Math.random()* 10) + 1;
                int availableStorage = shop.collect(product, quantity);
                System.out.println("Producer " + producerID + " delivers: " + product + "[" + quantity + "]");
                System.out.println(transactionOutcome(quantity, availableStorage, product));
            } catch (InterruptedException e){
                e.printStackTrace();
            }
        }
    }

    private String randomProduct(){
        String[] products = {"keyboard", "mouse", "headphones"};
        int rand = new Random().nextInt(products.length);
        return products[rand];
    }

    private String transactionOutcome(int quantity, int storage, String product){
        if(quantity <= storage){
            return product + "[" + quantity + "] was received.";
        }
        else if(storage != 0){
            return "[" + storage + "] out of [" + quantity + "] was received, no more space in storage.";
        }
        else{
            return "storage full, nothing was received.";
        }
    }
}
