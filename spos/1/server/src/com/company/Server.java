package com.company;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Scanner;
import java.util.Set;

public class Server {

    private static final int BUFFER_SIZE = 74;
    private static final long PROMPT_PERIOD = 5;
    private static ArrayList<Integer> returns = new ArrayList<>();
    private static int input = 0;
    private boolean fSent = false;
    private static long startTime = System.currentTimeMillis();

    private static boolean running = true;
    private static final Runnable periodic = () -> {
        while (running) {
            if (System.currentTimeMillis() - startTime > PROMPT_PERIOD * 1000) {
                System.out.println("1. Continue\n2. Continue without prompt\n3. Cancel");
                Scanner scanner = new Scanner(System.in);

                String command = scanner.next();

                startTime = System.currentTimeMillis();

                if (command.equals("2")) {
                    running = false;
                } else if (command.equals("3")) {
                    System.out.println("exiting");
                    System.exit(0);
                }
            }
        }
    };

    public static void main(String[] args) {
        try {
            Scanner scanner = new Scanner(System.in);
            System.out.println("enter command number");
            input = scanner.nextInt();

            Server server = new Server();


            new Runnable() {
                @Override
                public void run() {
                    try {
                        Thread.sleep(2000);
                        Client.main(new String[0]);
                        Client.main(new String[0]);
                    } catch (IOException e) {
                        e.printStackTrace();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            };

            server.start();
            periodic.run();

            //GUI gui = new GUI("startTime");
            //javax.swing.SwingUtilities.invokeAndWait(() -> gui.createAndShowGUI());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static Integer F(Integer a, Integer b) {
        return a * b;
    }

    private void start() throws IOException {
        Selector selector = Selector.open();
        ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.configureBlocking(false);

        int port = 8080;
        InetSocketAddress endpoint = new InetSocketAddress("localhost", port);
        serverSocketChannel.socket().bind(endpoint);
        serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);

        while (true) {

            int readyCount = selector.select();
            if (Integer.valueOf(0).equals(readyCount)) {
                continue;
            }

            Set<SelectionKey> readyKeys = selector.selectedKeys();
            Iterator iterator = readyKeys.iterator();
            while (iterator.hasNext()) {
                SelectionKey selectionKey = (SelectionKey) iterator.next();
                iterator.remove();

                if (selectionKey.isAcceptable()) {
                    this.accept(selectionKey);
                } else if (selectionKey.isReadable()) {
                    this.read(selectionKey);
                } else if (selectionKey.isWritable()) {
                    this.write(selectionKey);
                }
            }
        }
    }

    private void write(SelectionKey selectionKey) throws IOException {
        SocketChannel client = (SocketChannel) selectionKey.channel();
        ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
        int value = input;
        if (fSent)
            value += 20;
        else {
            fSent = true;
            value += 10;
        }
        System.out.printf("send %d\n", value);

        byte[] bytes = String.valueOf(value).getBytes();
        buffer.put(bytes);
        buffer.flip();
        client.write(buffer);

        Selector selector = selectionKey.selector();
        client.register(selector, SelectionKey.OP_READ);
    }

    private void read(SelectionKey selectionKey) throws IOException {
        SocketChannel client = (SocketChannel) selectionKey.channel();

        ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
        int numRead = client.read(buffer);

        if (numRead == -1) {
            client.close();
            selectionKey.cancel();
            return;
        }

        byte[] data = new byte[numRead];
        System.arraycopy(buffer.array(), 0, data, 0, numRead);
        String s = new String(data);
        Integer res = Integer.parseInt(s);
        returns.add(res);
        buffer.flip();
        if (Integer.valueOf(2).equals(returns.size())) {
            Integer finalResult = F(returns.get(0), returns.get(1));
            System.out.println("server final result = " + finalResult);

            System.exit(0);
        }

        if (res.equals(0)) {
            System.out.println("client returned 0 -> quitting");
            System.exit(0);
        }
    }

    private void accept(SelectionKey key) throws IOException {
        ServerSocketChannel serverChannel = (ServerSocketChannel) key.channel();

        SocketChannel client = serverChannel.accept();
        client.configureBlocking(false);

        Selector selector = key.selector();
        client.register(selector, SelectionKey.OP_WRITE);

        String clientAddress = client.getRemoteAddress().toString();
    }
}
