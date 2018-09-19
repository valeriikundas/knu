package com.example.valeriykundas.labyrinth;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.LinkedList;
import java.util.Queue;
import java.util.Random;

import static java.lang.StrictMath.abs;

public class MainActivity extends AppCompatActivity {

    private static final int DIRECTION_UP = 0;
    private static final int DIRECTION_RIGHT = 1;
    private static final int DIRECTION_DOWN = 2;
    private static final int DIRECTION_LEFT = 3;

    private static final int CELL_FREE = 0;
    private static final int CELL_BLOCK = 1;
    private static final int CELL_ENTRANCE = 2;
    private static final int CELL_EXIT = 3;
    private static final int CELL_CURRENT = 9;

    private static final int[][] delta = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

    private static final int board_size = 5;

    private class Point {
        private int first;
        private int second;

        Point() {
            this.first = 0;
            this.second = 0;
        }

        Point(int first, int second) {
            this.first = first;
            this.second = second;
        }
    }

    private int[][] board = new int[board_size][board_size];

    private Point entrancePoint;
    private Point exitPoint;
    private Point currentPoint;

    private int oldStateOfCurrentPoint;

    private boolean running = true;
    private boolean gameWon = false;

    TextView tv_board;

    @NonNull
    private Point getRandomPointCoordinates() {
        Random rand = new Random();
        int coordinate = rand.nextInt(board_size);
        int side = rand.nextInt(4);

        switch (side) {
            case DIRECTION_UP:
                return new Point(0, coordinate);
            case DIRECTION_RIGHT:
                return new Point(coordinate, board_size - 1);
            case DIRECTION_DOWN:
                return new Point(board_size - 1, coordinate);
            case DIRECTION_LEFT:
                return new Point(coordinate, 0);
        }

        return new Point();
    }

    private boolean pathExists() {
        boolean[][] used = new boolean[board_size][board_size];
        Queue<Point> queue = new LinkedList<>();
        queue.add(entrancePoint);
        used[entrancePoint.first][entrancePoint.second] = true;

        while (!queue.isEmpty()) {
            Point head = queue.remove();

            for (int i = 0; i < 4; ++i) {
                Point neighbor = new Point(head.first + delta[i][0], head.second + delta[i][1]);

                if (neighbor.first >= 0 && neighbor.first < board_size && neighbor.second >= 0 && neighbor.second < board_size) {
                    if (!used[neighbor.first][neighbor.second] && board[neighbor.first][neighbor.second] != CELL_BLOCK) {
                        queue.add(neighbor);
                        used[neighbor.first][neighbor.second] = true;
                    }
                }
            }
        }

        return used[exitPoint.first][exitPoint.second];
    }

    private Point addRandomBlock() {
        boolean done = false;
        Point point = new Point();

        while (!done) {
            point = getRandomPointCoordinates();
            if (board[point.first][point.second] == CELL_FREE) {
                board[point.first][point.second] = CELL_BLOCK;
                done = true;
            }
        }
        return point;
    }

    @NonNull
    private String getBoardAsString() {
        StringBuilder builder = new StringBuilder();
        for (int i = 0; i < board_size; ++i) {
            for (int j = 0; j < board_size; ++j)
                builder.append(board[i][j]);
            builder.append("\n");
        }
        return builder.toString();
    }

    private boolean canGoTo(int cellStatus) {
        return cellStatus == CELL_ENTRANCE || cellStatus == CELL_FREE || cellStatus == CELL_EXIT;
    }

    private void addOnClickListener(int id, final int first, final int second) {
        Button button = findViewById(id);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                running = false;
                goByOffset(first, second);
                running = true;
            }
        });
    }

    private void setOnClickListeners() {
        addOnClickListener(R.id.btn_left, 0, -1);
        addOnClickListener(R.id.btn_up, -1, 0);
        addOnClickListener(R.id.btn_down, 1, 0);
        addOnClickListener(R.id.btn_right, 0, 1);
    }

    private void goByOffset(int first, int second) {
        Point nextPoint = currentPoint;
        nextPoint.first += first;
        nextPoint.second += second;
        if (isPointOnBoard(nextPoint) && canGoTo(board[nextPoint.first][nextPoint.second])) {
            board[currentPoint.first][currentPoint.second] = oldStateOfCurrentPoint;
            currentPoint = nextPoint;
            oldStateOfCurrentPoint = board[currentPoint.first][currentPoint.second];
            board[currentPoint.first][currentPoint.second] = CELL_CURRENT;

            if (nextPoint.equals(entrancePoint)) {
                gameWon = true;
                showGameWonMessage();
            }
        }
    }

    private void showGameWonMessage() {
        findViewById(R.id.btn_left).setVisibility(View.INVISIBLE);
        findViewById(R.id.btn_up).setVisibility(View.INVISIBLE);
        findViewById(R.id.btn_down).setVisibility(View.INVISIBLE);
        findViewById(R.id.btn_right).setVisibility(View.INVISIBLE);

        tv_board.setText(R.string.game_won);
    }

    private boolean isPointOnBoard(Point a) {
        return a.first >= 0 && a.first < board_size && a.second >= 0 && a.second < board_size;
    }

    private void refreshBoardView() {
        String boardAsString = getBoardAsString();
        tv_board.setText(boardAsString);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tv_board = findViewById(R.id.tv_board);
        tv_board.setTextSize(50);
        setOnClickListeners();

        //randomize entrance
        entrancePoint = getRandomPointCoordinates();
        board[entrancePoint.first][entrancePoint.second] = CELL_ENTRANCE;

        //randomize exit
        exitPoint = getRandomPointCoordinates();
        board[exitPoint.first][exitPoint.second] = CELL_EXIT;

        //make sure entrance and exit are not close
        boolean done = false;
        while (!done) {
            int distance = abs(entrancePoint.first - exitPoint.first) + abs(entrancePoint.second - exitPoint.second);
            if (distance < 2)
                exitPoint = getRandomPointCoordinates();
            else
                done = true;
        }

        //fill blocks while paths exist
        Point last_added_block = new Point();

        while (pathExists())
            last_added_block = addRandomBlock();

        board[last_added_block.first][last_added_block.second] = CELL_FREE;

        //show board and refresh
        while (true) {
            if (running)
                refreshBoardView();
        }

        //message when came to exit
    }
}
