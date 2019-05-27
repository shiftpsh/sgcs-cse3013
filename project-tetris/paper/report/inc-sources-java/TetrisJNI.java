package com.shiftpsh.tetris_ai;

public class TetrisJNI {
    static {
        System.loadLibrary("tetris");
    }

    private native String run(String[] constraints);

    public static String simulate(String[] args) {
        return new TetrisJNI().run(args);
    }
}
