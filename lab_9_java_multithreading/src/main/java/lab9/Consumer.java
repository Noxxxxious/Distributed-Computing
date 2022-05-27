package lab9;

import java.util.Random;

public class Consumer extends Thread{
    private final int consumerID;
    private final Shop shop;

    public Consumer(int consumerID, Shop shop){
        this.consumerID = consumerID;
        this.shop = shop;
    }

    @Override
    public void run(){
        while(true){
            try{
                sleep((long) (Math.random() * 6000));
                String product = randomProduct();
                int quantity = (int)(Math.random()* 10) + 1;
                int supply = shop.sell(product, quantity);
                System.out.println("Consumer " + consumerID + " attempts to buy: " + product + "[" + quantity + "]");
                System.out.println(transactionOutcome(quantity, supply, product));
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

    private String transactionOutcome(int quantity, int supply, String product){
        if(quantity <= supply){
            return product + "[" + quantity + "] was bought.";
        }
        else if(supply != 0){
            return "[" + supply + "] out of [" + quantity + "] was bought, no more product in storage.";
        }
        else{
            return "storage empty, nothing was bought.";
        }
    }
}
