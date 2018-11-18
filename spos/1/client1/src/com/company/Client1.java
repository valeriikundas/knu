package com.company;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class Client1 {

    private static final int BUFFER_SIZE = 74;

    private static int f(int x) {
        long pause = 5;
        int rv = x;

        switch (x) {
            case 11: {
                //do nothing
                break;
            }
            case 21: {
                pause = 10;
                break;
            }

            case 12: {
                pause = 10;
                break;
            }
            case 22: {
                //do nothing
                break;
            }

            case 13: {
                rv = 0;
                break;
            }
            case 23: {
                pause = 10000;
                break;
            }

            case 14: {
                pause = 10000;
                break;
            }
            case 24: {
                rv = 0;
                break;
            }

            case 15: {
                break;
            }
            case 25: {
                pause = 10000;
                break;
            }

            case 16: {
                pause = 10000;
                break;
            }
            case 26: {
                break;
            }

            case 17: {
                pause = 1;
                break;
            }
            case 27: {
                pause = 1;
                break;
            }

            case 18: {
                pause = 6;
                break;
            }
            case 28: {
                pause = 6;
                break;
            }

            default: {
                System.out.println("wrong code. nothing fancy will happen");
                break;
            }
        }

        try {
            Thread.sleep(pause * 1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return rv * 100;
    }

    public static void main(String[] args) throws IOException {
        SocketAddress address = new InetSocketAddress("localhost", 8080);
        SocketChannel client = SocketChannel.open(address);
        System.out.println("client1 is starting");

        ByteBuffer b = ByteBuffer.allocate(BUFFER_SIZE);
        int numRead = client.read(b);
        if (numRead == -1) {
            return;
        }
        byte[] data = new byte[numRead];
        System.arraycopy(b.array(), 0, data, 0, numRead);
        String s = new String(data);

        Integer x = Integer.parseInt(s);
        System.out.println("client1 x=" + x);

        int y = f(x);
        System.out.println("client1 y=" + y);

        ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
        buffer.put(String.valueOf(y).getBytes());
        buffer.flip();
        client.write(buffer);
        buffer.clear();
        client.close();
    }
}
