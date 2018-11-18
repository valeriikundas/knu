package com.company;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.*;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class Server {

    private static final int BUFFER_SIZE = 74;
    private static final long PROMPT_PERIOD = 5;
    private static ArrayList<Integer> returns = new ArrayList<>();
    private static ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);
    private static int input = 0;
    private boolean fSent = false;

    private static final Runnable periodic = () -> {
        System.out.println("1. Continue\n2. Continue without prompt\n3. Cancel");
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        String command;
        try {
            command = br.readLine();
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }

        if (command.equals("2")) {
            scheduler.shutdown();
        } else if (command.equals("3")) {
            System.out.println("exiting");
            System.exit(0);
        }
    };

    public static void main(String[] args) {
        try {
            Scanner scanner = new Scanner(System.in);
            System.out.println("enter command number");
            input = scanner.nextInt();

            statusCheck();
            Server server = new Server();

            GUI gui = new GUI("start");
            javax.swing.SwingUtilities.invokeAndWait(() -> gui.createAndShowGUI());

            server.start();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void statusCheck() {
        scheduler.scheduleAtFixedRate(periodic, PROMPT_PERIOD, PROMPT_PERIOD, TimeUnit.SECONDS);
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
        System.out.println("server is starting on port " + port);

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
        System.out.println("server has read res=" + res);
        if (Integer.valueOf(2).equals(returns.size())) {
            Integer finalResult = F(returns.get(0), returns.get(1));
            System.out.printf("final results %d %d\n", returns.get(0), returns.get(1));
            System.out.println("server final result = " + finalResult);

            scheduler.shutdown();
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
        System.out.println("server connected to " + clientAddress);
    }
}
