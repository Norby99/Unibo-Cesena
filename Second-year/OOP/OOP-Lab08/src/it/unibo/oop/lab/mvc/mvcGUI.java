package it.unibo.oop.lab.mvc;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public final class mvcGUI {

    private final JFrame frame = new JFrame("mvcGUI");

    /*
     * Once the Controller is done, implement this class in such a way that:
     * 
     * 1) It has a main method that starts the graphical application
     * 
     * 2) In its constructor, sets up the whole view
     * 
     * 3) The graphical interface consists of a JTextField in the upper part of the frame, 
     * a JTextArea in the center and two buttons below it: "Print", and "Show history". 
     * SUGGESTION: Use a JPanel with BorderLayout
     * 
     * 4) By default, if the graphical interface is closed the program must exit
     * (call setDefaultCloseOperation)
     * 
     * 5) The behavior of the program is that, if "Print" is pressed, the
     * controller is asked to show the string contained in the text field on standard output. 
     * If "show history" is pressed instead, the GUI must show all the prints that
     * have been done to this moment in the text area.
     * 
     */

    /**
     * builds a new {@link mvcGUI}.
     */
    public mvcGUI(final MyController ctrl) {

        final Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
        final int sw = (int) screen.getWidth();
        final int sh = (int) screen.getHeight();
        frame.setSize(sw / 2, sh / 2);
        frame.setLocationByPlatform(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        JPanel mainCanvas = new JPanel();   // setting up canvases
        mainCanvas.setLayout(new BorderLayout());
        JPanel southCanvas = new JPanel();   // setting up canvases
        southCanvas.setLayout(new BorderLayout());
        
        JTextField textField = new JTextField();    // setting up components
        JTextArea textArea = new JTextArea();
        textArea.setEditable(false);
        JButton btnPrint = new JButton("Print");
        JButton btnShowHistory = new JButton("Show history");
        
        mainCanvas.add(textField, BorderLayout.NORTH);  // positioning components
        mainCanvas.add(textArea, BorderLayout.CENTER);
        southCanvas.add(btnPrint, BorderLayout.WEST);
        southCanvas.add(btnShowHistory, BorderLayout.EAST);
        
        mainCanvas.add(southCanvas, BorderLayout.SOUTH);
        frame.setContentPane(mainCanvas);
        
        btnPrint.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(final ActionEvent e) {
                ctrl.setNextText(textField.getText());
                ctrl.printCurrentString();
            }
        });
        
        btnShowHistory.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(final ActionEvent e) {
                final StringBuilder text = new StringBuilder();
                final List<String> history = ctrl.getPrintedStringList();
                for (final String print: history) {
                    text.append(print);
                    text.append('\n');
                }
                if (!history.isEmpty()) {
                    text.deleteCharAt(text.length() - 1);
                }
                textArea.setText(text.toString());
            }
        });
    }
    
    public void display() {
        this.frame.setVisible(true);
    }
    
    public static void main(final String... a) {
        mvcGUI myGUI = new mvcGUI(new MyController());
        myGUI.display();
    }

}
