package bbom;

import bbom.controller.MyApp;

import javax.swing.*;

public class Launcher {
    static public void main(String[] args) throws Exception {
        MyApp app = new MyApp();
        SwingUtilities.invokeAndWait(app::initialize);
    }
}
