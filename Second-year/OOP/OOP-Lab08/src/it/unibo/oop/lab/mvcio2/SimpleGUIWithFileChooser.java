package it.unibo.oop.lab.mvcio2;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import it.unibo.oop.lab.mvcio.Controller;

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
    private final JFrame frame = new JFrame("SimpleGUIWithFileChooser");
    
    public SimpleGUIWithFileChooser(final Controller ctrl) {
        final Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();   // setting up the frame
        final int sw = (int) screen.getWidth();
        final int sh = (int) screen.getHeight();
        frame.setSize(sw / 2, sh / 2);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationByPlatform(true);
        
        JPanel mainCanvas = new JPanel();   // setting up the components
        mainCanvas.setLayout(new BorderLayout());
        JPanel browseCanvas = new JPanel();
        browseCanvas.setLayout(new BorderLayout());
        
        JTextArea textArea = new JTextArea();
        JButton btnSave = new JButton("Save");
        JTextField pathText = new JTextField(ctrl.getCurrentFileAsString());
        pathText.setEditable(false);
        JButton btnBrowse = new JButton("Browse...");
        
        mainCanvas.add(textArea, BorderLayout.CENTER);   // adding the components to the mainCanvas
        mainCanvas.add(btnSave, BorderLayout.SOUTH);
        
        browseCanvas.add(pathText, BorderLayout.CENTER);   // adding the components to the browseCanvas
        browseCanvas.add(btnBrowse, BorderLayout.EAST);
        
        mainCanvas.add(browseCanvas, BorderLayout.NORTH);
        
        frame.setContentPane(mainCanvas);
        
        btnSave.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(final ActionEvent e) {
                try {
                    ctrl.saveToFile(textArea.getText());
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
            }
        });
        
        btnBrowse.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(final ActionEvent e) {
                final JFileChooser fc = new JFileChooser("Choose where to save");
                fc.setSelectedFile(ctrl.getCurrentFile());
                final int result = fc.showSaveDialog(frame);
                switch (result) {
                case JFileChooser.APPROVE_OPTION:
                    ctrl.setCurrentFile(fc.getSelectedFile());
                    pathText.setText(ctrl.getCurrentFileAsString());
                    break;
                case JFileChooser.CANCEL_OPTION:
                    break;
                default:
                    JOptionPane.showMessageDialog(frame, result, "Meh!", JOptionPane.ERROR_MESSAGE);
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
