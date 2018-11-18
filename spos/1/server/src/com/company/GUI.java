package com.company;

import java.awt.BorderLayout;
import java.awt.event.*;
import javax.swing.*;

public class GUI extends JFrame implements KeyListener {

    void createAndShowGUI() {
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        addComponentsToPane();
        pack();
        setVisible(true);
    }

    private void addComponentsToPane() {
        JTextField typingArea = new JTextField(20);
        typingArea.addKeyListener(this);
        getContentPane().add(typingArea, BorderLayout.PAGE_START);
    }

    GUI(String name) {
        super(name);
    }

    public void keyTyped(KeyEvent e) {
    }

    public void keyPressed(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
            System.out.println("esc pressed");
            System.exit(0);
        }
    }

    public void keyReleased(KeyEvent e) {
    }
}