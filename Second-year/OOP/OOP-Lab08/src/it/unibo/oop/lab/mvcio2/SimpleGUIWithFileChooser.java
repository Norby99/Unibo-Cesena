package it.unibo.oop.lab.mvcio2;

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

import it.unibo.oop.lab.mvcio.Controller;
import it.unibo.oop.lab.mvcio.SimpleGUI;

public final class SimpleGUIWithFileChooser {
    
    /*
     * TODO: Starting from the application in mvcio:
     * 
     * 1) Add a JTextField and a button "Browse..." on the upper part of the
     * graphical interface.
     * Suggestion: use a second JPanel with a second BorderLayout, put the panel
     * in the North of the main panel, put the text field in the center of the
     * new panel and put the button in the line_end of the new panel.
     * 
     * 2) The JTextField should be non modifiable. And, should display the
     * current selected file.
     * 
     * 3) On press, the button should open a JFileChooser. The program should
     * use the method showSaveDialog() to display the file chooser, and if the
     * result is equal to JFileChooser.APPROVE_OPTION the program should set as
     * new file in the Controller the file chosen. If CANCEL_OPTION is returned,
     * then the program should do nothing. Otherwise, a message dialog should be
     * shown telling the user that an error has occurred (use
     * JOptionPane.showMessageDialog()).
     * 
     * 4) When in the controller a new File is set, also the graphical interface
     * must reflect such change. Suggestion: do not force the controller to
     * update the UI: in this example the UI knows when should be updated, so
     * try to keep things separated.
     */
    private final JFrame frame = new JFrame("SimpleGUI");
    
    public SimpleGUIWithFileChooser(final Controller ctrl) {
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
        SimpleGUIWithFileChooser myGUI = new SimpleGUIWithFileChooser(new Controller());
        myGUI.display();
    }
}
