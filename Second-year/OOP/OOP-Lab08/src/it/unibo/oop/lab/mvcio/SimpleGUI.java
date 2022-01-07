package it.unibo.oop.lab.mvcio;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;

public final class SimpleGUI {

    private final JFrame frame = new JFrame("SimpleGUI");

    /*
     * Once the Controller is done, implement this class in such a way that:
     * 
     * 1) It has a main method that starts the graphical application
     * 
     * 2) In its constructor, sets up the whole view
     * 
     * 3) The graphical interface consists of a JTextArea with a button "Save" right
     * below (see "ex02.png" for the expected result). SUGGESTION: Use a JPanel with
     * BorderLayout
     * 
     * 4) By default, if the graphical interface is closed the program must exit
     * (call setDefaultCloseOperation)
     * 
     * 5) The program asks the controller to save the file if the button "Save" gets
     * pressed.
     * 
     * Use "ex02.png" (in the res directory) to verify the expected aspect.
     */

    /**
     * builds a new {@link SimpleGUI}.
     */
    public SimpleGUI(final Controller ctrl) {
        /*
         * Make the frame half the resolution of the screen. This very method is
         * enough for a single screen setup. In case of multiple monitors, the
         * primary is selected.
         */
        final Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();   // setting up the frame
        final int sw = (int) screen.getWidth();
        final int sh = (int) screen.getHeight();
        frame.setSize(sw / 2, sh / 2);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationByPlatform(true);
        
        JPanel canvas = new JPanel();   // setting up the components
        canvas.setLayout(new BorderLayout());
        JTextArea textArea = new JTextArea();
        JButton btnSave = new JButton("Save");
        
        canvas.add(textArea, BorderLayout.CENTER);   // adding the components to the canvas and frame
        canvas.add(btnSave, BorderLayout.SOUTH);
        frame.setContentPane(canvas);
        
        btnSave.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(final ActionEvent e) {
                try {
                    ctrl.saveToFile(textArea.getText());
                } catch (IOException e1) {
                    // TODO Auto-generated catch block
                    e1.printStackTrace();
                }
            }
        });
    }
    
    public void display() {
        frame.setVisible(true);
    }
    
    public static void main(final String... a) {
        SimpleGUI myGUI = new SimpleGUI(new Controller());
        myGUI.display();
    }
}
